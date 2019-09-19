
Using cvmx-flash to Write Crash Dumps to Flash
----------------------------------------------

The cvmx-flash functions can be used in conjunction with the cvmx-interrupt
infrastructure to store trap information in flash for later debugging. The
basic idea is to store the text of a crash dump into a fix flash location.
Later, a crash analysis can be performed by reading the crash dump directly
from flash.

In implementing this, use cvmx_interrupt_set_exception() to replace the default
exception handler early in main. The exception handler can then generate a text
crash dump and send it out the UART as well as write it to flash. Below is a
sample function that does this. It saves crash dumps at offset
1MB + 128KB * core id in flash. These can be read from 0x1fd00000, 0x1fd20000,
etc in flash.

Lines to add to main()
----------------------
    cvmx_flash_initialize();
    cvmx_interrupt_set_exception(flash_dump_exception_handler);

New exception handler
---------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cvmx.h"
#include "cvmx-flash.h"
#include "cvmx-interrupt.h"

#define COP0_CAUSE      "$13,0"
#define COP0_STATUS     "$12,0"
#define COP0_BADVADDR   "$8,0"
#define COP0_EPC        "$14,0"
#define READ_COP0(dest, R) asm volatile ("dmfc0 %[rt]," R : [rt] "=r" (dest))

static void flash_dump_exception_handler(uint64_t registers[32])
{
    static char dump[128<<10];    /* One per core */
    char *      dump_ptr = dump;
    int         dump_space = sizeof(dump);
    uint64_t    trap_print_cause;
    int         count;
    const char *message;

    memset(dump_ptr, 0, dump_space);

    message="*** Begining crash dump\n";
    write(1, message, strlen(message));

    count = snprintf(dump_ptr, dump_space, "******************************\n");
    dump_ptr += count;
    dump_space -= count;

    count = snprintf(dump_ptr, dump_space, "Core %lu: Unhandled Exception. "
			"Cause register decodes to:\n", cvmx_get_core_num());
    dump_ptr += count;
    dump_space -= count;

    READ_COP0(trap_print_cause, COP0_CAUSE);
    switch ((trap_print_cause >> 2) & 0x1f)
    {
        case 0x0:
            count = snprintf(dump_ptr, dump_space, "Interrupt\n");
            break;
        case 0x1:
            count = snprintf(dump_ptr, dump_space, "TLB Mod\n");
            break;
        case 0x2:
            count = snprintf(dump_ptr, dump_space, "tlb load/fetch\n");
            break;
        case 0x3:
            count = snprintf(dump_ptr, dump_space, "tlb store\n");
            break;
        case 0x4:
            count = snprintf(dump_ptr, dump_space, "address exc, load/fetch\n");
            break;
        case 0x5:
            count = snprintf(dump_ptr, dump_space, "address exc, store\n");
            break;
        case 0x6:
            count = snprintf(dump_ptr, dump_space, "bus error, inst. fetch\n");
            break;
        case 0x7:
            count = snprintf(dump_ptr, dump_space, "bus error, load/store\n");
            break;
        case 0x8:
            count = snprintf(dump_ptr, dump_space, "syscall\n");
            break;
        case 0x9:
            count = snprintf(dump_ptr, dump_space, "breakpoint \n");
            break;
        case 0xa:
            count = snprintf(dump_ptr, dump_space, "reserved instruction\n");
            break;
        case 0xb:
            count = snprintf(dump_ptr, dump_space, "cop unusable\n");
            break;
        case 0xc:
            count = snprintf(dump_ptr, dump_space, "arithmetic overflow\n");
            break;
        case 0xd:
            count = snprintf(dump_ptr, dump_space, "trap\n");
            break;
        case 0xf:
            count = snprintf(dump_ptr, dump_space, "floating point exc\n");
            break;
        case 0x12:
            count = snprintf(dump_ptr, dump_space, "cop2 exception\n");
            break;
        case 0x16:
            count = snprintf(dump_ptr, dump_space, "mdmx unusable\n");
            break;
        case 0x17:
            count = snprintf(dump_ptr, dump_space, "watch\n");
            break;
        case 0x18:
            count = snprintf(dump_ptr, dump_space, "machine check\n");
            break;
        case 0x1e:
            count = snprintf(dump_ptr, dump_space, "cache error\n");
            break;
        default:
            count = snprintf(dump_ptr, dump_space, "Reserved exception.\n");
            break;
    }
    dump_ptr += count;
    dump_space -= count;

    count = snprintf(dump_ptr, dump_space, "******************************\n");
    {
        static const char *name[32] = {"r0","at","v0","v1","a0","a1","a2","a3",
            "t0","t1","t2","t3","t4","t5","t6","t7","s0","s1","s2","s3","s4",
	    "s5","s6","s7", "t8","t9", "k0","k1","gp","sp","s8","ra"};
        uint64_t reg;
        for (reg=0; reg<16; reg++)
        {
            count = snprintf(dump_ptr, dump_space,
	           "%3s ($%02ld): 0x%016lx \t %3s ($%02ld): 0x%016lx\n",
		   name[reg], reg, registers[reg], name[reg+16], reg+16,
		   registers[reg+16]);
            dump_ptr += count;
            dump_space -= count;
        }
        READ_COP0(reg, COP0_CAUSE);
        count = snprintf(dump_ptr, dump_space,
	                 "%16s: 0x%016lx\n", "COP0_CAUSE", reg);
        dump_ptr += count;
        dump_space -= count;
        READ_COP0(reg, COP0_STATUS);
        count = snprintf(dump_ptr, dump_space,
	                 "%16s: 0x%016lx\n", "COP0_STATUS", reg);
        dump_ptr += count;
        dump_space -= count;
        READ_COP0(reg, COP0_BADVADDR);
        count = snprintf(dump_ptr, dump_space,
	                 "%16s: 0x%016lx\n", "COP0_BADVADDR", reg);
        dump_ptr += count;
        dump_space -= count;
        READ_COP0(reg, COP0_EPC);
        count = snprintf(dump_ptr, dump_space,
	                 "%16s: 0x%016lx\n", "COP0_EPC", reg);
        dump_ptr += count;
        dump_space -= count;
    }
    count = snprintf(dump_ptr, dump_space, "******************************\n");
    dump_ptr += count;
    dump_space -= count;

    /* Write the crash out the serial port */
    write(1, dump, sizeof(dump) - dump_space);

    void *flash_ptr = cvmx_flash_get_base(0);
    if (flash_ptr)
    {
        message="*** Writing crash dump to flash at 1MB plus core offset\n";
        write(1, message, strlen(message));
        flash_ptr += (1<<20) + cvmx_get_core_num()*sizeof(dump);
        if (cvmx_flash_write(flash_ptr, dump, sizeof(dump) - dump_space))
        {
            message="*** ERROR: Failed to write crash dump to flash\n";
            write(1, message, strlen(message));
        }
        else
        {
            message="*** Crash dump complete\n";
            write(1, message, strlen(message));
        }
    }

    /* Loop forever here */
    while (1)
        asm volatile ("wait");
}

---------------------

