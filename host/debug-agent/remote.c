#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <poll.h>
#include <sched.h>
#include "agent.h"
#include "cvmx.h"
#include "cvmx-debug.h"
#include "octeon-remote.h"
#include "cvmx-swap.h"

#define FOR_EACH_CORE(CORE, MASK)            \
    for (CORE = 0; CORE < CVMX_MAX_CORES; CORE++) \
        if (MASK & (1ull << CORE))

#define DRSEG	0xffffffffff300000
#define DBS	(DRSEG + 0x2000)

#define SETRESP(...) set_response(response, responselen, __VA_ARGS__)

/* 0 means all debugable cores.  */
static uint64_t active_cores = 0;
/* Those that enterred the debug exception handler and need to be resumed with
   step-all.  */
static uint64_t stopped_cores = 0;

static int focus_core = -1;
static bool step_all;
static bool step_isr = true;

static int tlb_entries = -1;

static bool local = false;
static uint64_t local_cores = 0;

/* Maximium number of hardware breakpoints/watchpoints allowed */
#define MAX_OCTEON_HW_BREAKPOINTS 4

static cvmx_debug_core_context_t focus_core_context;
#define FOCUS_CORE_VALUE(FIELD) focus_core_context.FIELD

static void invalidate_focus_core_context(void)
{
    memset(&focus_core_context, -1, sizeof(focus_core_context));
}

static void set_perf_control_reg (cvmx_debug_core_context_t *context, int perf_event, int perf_counter)
{
    cvmx_core_perf_control_t control;

    control.u32 = 0;
    control.s.u = 1;
    control.s.s = 1;
    control.s.k = 1;
    control.s.ex = 1;
    control.s.w = 1;
    control.s.m = 1 - perf_counter;
    control.s.event = perf_event;

    context->cop0.perfctrl[perf_counter] = control.u32;
}

static uint64_t get_debug_globals_addr (void)
{
    static uint64_t debug_globals_addr;
    if (!debug_globals_addr)
    {
        uint64_t size;
        if (!octeon_remote_named_block_find(CVMX_DEBUG_GLOBALS_BLOCK_NAME, &debug_globals_addr, &size))
        {
            debug_printf("No remote debug named block \n");
            return 0;
        }
        debug_printf("Remote debug named block is %"PRIx64" at 0x%"PRIx64"\n", size, debug_globals_addr);
    }
    return debug_globals_addr;
}



/* Return zero if fails.  */
static uint64_t core_context_addr(int core)
{
    assert(core != -1);
    uint64_t addr = get_debug_globals_addr();
    if (addr == 0)
        return 0;
    return addr + offsetof (cvmx_debug_globals_t, contextes) + core * sizeof(cvmx_debug_core_context_t);
}

static bool check_version (void)
{
    uint64_t version;
    uint64_t addr = get_debug_globals_addr();
    if (addr == 0)
        return -1;
    version = octeon_remote_read_mem64(addr);
    if (version != CVMX_DEBUG_GLOBALS_VERSION)
    {
       debug_printf ("Wrong version: %"PRId64"; expected version: %d.\n", version, CVMX_DEBUG_GLOBALS_VERSION);
       return 1;
    }
  return 0;

}

static cvmx_debug_state_t get_state(void)
{
    unsigned int i;
    uint64_t addr = get_debug_globals_addr() + offsetof (struct cvmx_debug_globals_s, state[0]);
    cvmx_debug_state_t state;
    uint64_t state1[sizeof(cvmx_debug_state_t)/sizeof(uint64_t)];
    octeon_remote_read_mem(&state1, addr, sizeof(state1));

    for(i = 0; i < sizeof(cvmx_debug_state_t)/sizeof(uint64_t); i++)
        state1[i] = cvmx_be64_to_cpu(state1[i]);
    memcpy(&state, state1, sizeof(state));
    return state;
}

static void set_step_all(uint64_t step)
{
    uint64_t addr = get_debug_globals_addr() + offsetof (struct cvmx_debug_globals_s, state[0]);
    addr += offsetof(cvmx_debug_state_t, step_all);

    octeon_remote_write_mem64(addr, step);

}

static uint64_t get_step_all(void)
{
    return get_state().step_all;
}

static void set_active_cores(uint64_t active_cores)
{
    uint64_t addr = get_debug_globals_addr() + offsetof (struct cvmx_debug_globals_s, state[0]);
    addr += offsetof(cvmx_debug_state_t, active_cores);

    octeon_remote_write_mem64(addr, active_cores);

}

static uint64_t get_active_cores(void)
{
    return get_state().active_cores;
}

static uint64_t get_known_cores(void)
{
    uint64_t known_cores = get_state().known_cores;
    /* if we are running on a linux user land, we don't want to disable
       the cores which this linux is running on. */
    if (local)
      known_cores &= ~local_cores;
    return known_cores;
}

static uint64_t get_step_isr(void)
{
    return get_state().step_isr;
}

static void set_step_isr(uint64_t step)
{
    uint64_t addr = get_debug_globals_addr() + offsetof (struct cvmx_debug_globals_s, state[0]);
    addr += offsetof(cvmx_debug_state_t, step_isr);

    octeon_remote_write_mem64(addr, step);
}

static uint64_t get_focus_core(void)
{
    return get_state().focus_core;
}

static void set_focus_core(uint64_t core)
{
    uint64_t addr = get_debug_globals_addr() + offsetof (struct cvmx_debug_globals_s, state[0]);
    addr += offsetof(cvmx_debug_state_t, focus_core);

    octeon_remote_write_mem64(addr, core);
}

static void context_be_to_cpu(cvmx_debug_core_context_t *context)
{
#define F(X) context->X = cvmx_be64_to_cpu(context->X)
    CVMX_DEBUG_BASIC_CONTEXT;
    CVMX_DEBUG_FP_CONTEXT;
    CVMX_DEBUG_COP0_CONTEXT;
    CVMX_DEBUG_COP1_CONTEXT; 
    CVMX_DEBUG_HW_IBP_CONTEXT;
    CVMX_DEBUG_HW_DBP_CONTEXT;
    CVMX_DEBUG_TLB_CONTEXT;
#undef F
}

static inline void context_cpu_to_be(cvmx_debug_core_context_t *context)
{
#define F(X) context->X = cvmx_cpu_to_be64(context->X)
    CVMX_DEBUG_BASIC_CONTEXT;
    CVMX_DEBUG_FP_CONTEXT;
    CVMX_DEBUG_COP0_CONTEXT;
    CVMX_DEBUG_COP1_CONTEXT;
    CVMX_DEBUG_HW_IBP_CONTEXT;
    CVMX_DEBUG_HW_DBP_CONTEXT;
    CVMX_DEBUG_TLB_CONTEXT;
#undef F
}


/* Return success if we can trust the context.  */
static int read_core_context(int core, cvmx_debug_core_context_t *context)
{
    int rc;
    uint64_t addr;
    addr = core_context_addr(core);
    if (!addr)
        return false;

    octeon_remote_read_mem(context, addr, sizeof(*context));
    context_be_to_cpu(context);
    rc = context->remote_controlled;
    debug_printf("read core: %d with rc: %d.\n", core, rc);
    return rc;
}

static bool write_core_context(int core, cvmx_debug_core_context_t *context)
{
    uint64_t addr;
    addr = core_context_addr(core);
    if (!addr)
        return false;

    context_cpu_to_be(context);
    octeon_remote_write_mem(addr, context, sizeof(*context));
    return true;
}

static inline bool is_member(int core, uint64_t mask)
{
    if (core == -1)
        return false;
    return (1ull << core) & mask;
}

static inline uint64_t core_to_mask(int core)
{
    if (core == -1)
        return 0;
    return 1ull << core;
}

static inline int lowest_core(uint64_t mask)
{
    return ffsll(mask) - 1;
}

/* Stop the cores in the mask. */
static bool stop_cores(uint64_t mask)
{
    stopped_cores = mask;
    octeon_remote_write_csr(CVMX_CIU_DINT, mask);
    /* Wait for the exception handler to write the state.  */
    usleep (3000);

    if (is_member(focus_core, mask))
    {
	int rc = read_core_context(focus_core, &focus_core_context);
        if (rc && rc != COMMAND_STEP && rc != COMMAND_CONTINUE)
            return true;
        send_verbose_error_response("Failed to stop focus core: %d\n", focus_core);
        mask &= ~(1ull << focus_core);
    }
    FOR_EACH_CORE(focus_core, mask)
    {
	int rc = read_core_context(focus_core, &focus_core_context);
        if (rc && rc != COMMAND_STEP && rc != COMMAND_CONTINUE)
            return true;
    }

    focus_core = -1;
    send_verbose_error_response("Failed to stop cores in mask 0x%"PRIx64"\n", stopped_cores);
    stopped_cores = 0;
    return false;
}

/* Start the cores up again. */
static void start_cores(int command)
{
    {
        /* Just write out 0 for the remote_controlled part of the context, no need to read it in. */
        uint64_t addr;
        addr = core_context_addr(focus_core) + offsetof (cvmx_debug_core_context_t, remote_controlled); 
        octeon_remote_write_mem64 (addr, command);
    }
    usleep (4);
    invalidate_focus_core_context();
}

static uint64_t debugable_cores(void)
{
    uint64_t known_cores = get_state().handler_cores;
    /* if we are running on a linux user land, we don't want to disable
       the cores which this linux is running on. */
    if (local)
      known_cores &= ~local_cores;
    return known_cores;
}

static uint64_t old_commtype;

/* Save the communication type.  Set the communication type for remote. */
static void save_and_set_comm_type (void)
{
    uint64_t addr = get_debug_globals_addr();
    uint64_t newcomm = COMM_REMOTE + 1;

    old_commtype = octeon_remote_read_mem64(addr + offsetof (cvmx_debug_globals_t, comm_type));

    octeon_remote_write_mem64(addr + offsetof (cvmx_debug_globals_t, comm_changed), newcomm);
    /* Wait for the exception handler to write the state.  */
    usleep (6000);
}

/* Restore the old communication type. */

static void restore_comm_type (void)
{
    uint64_t addr = get_debug_globals_addr();
    uint64_t newcomm = old_commtype + 1;
    octeon_remote_write_mem64(addr + offsetof (cvmx_debug_globals_t, comm_changed), newcomm);
    usleep (6000);
}

/* Returns the number of TLB entries for the remote machine. */

int get_tlb_entries (void)
{
   uint64_t addr = get_debug_globals_addr();
   uint64_t tlbs;
   addr += offsetof (cvmx_debug_globals_t, tlb_entries);
   tlbs = octeon_remote_read_mem64(addr);
   return tlbs; 
}

/* Return the core mask for the local machine. */
uint64_t get_local_coremask (void)
{
    uint64_t mask = 0;
    unsigned corenum;
    char line[255];
    FILE *cpuinfo = fopen ("/proc/cpuinfo", "r");
    char *c;
    /* If we cannot open /proc/cpuinfo assume we are not running locally. */
    if (!cpuinfo)
        return 0;
    while (!feof(cpuinfo))
    {
        c = fgets(line, sizeof(line), cpuinfo);
	if (c == NULL) {
            perror("fgets");
            return 0;
	}
        /* parse "core			: NN". */
        if(strncmp(line, "core\t\t\t: ", 9) == 0)
        {
	    char *temp = line + 9;
            corenum = strtol(temp, NULL, 10);
            mask |= 1ull << corenum;
        }
    }

    fclose (cpuinfo);
    return mask;
}

/* Load and boot the program. */
static void load_and_boot (char *octeon_pci_bootcmd, char *execfile, int argc, char **argv)
{
    unsigned long long address = 0, cur_address;
    FILE *infile;
    char buffer[65536];
    uint64_t filesize = 0;
    int seen_debug = 0;
    int address_supplied = 0;

    if (octeon_pci_bootcmd == NULL)
        octeon_pci_bootcmd = "oct-pci-reset";

  /* If an option other than oct-pci-reset is set through "set pci-bootcmd" 
     then invoke the command before starting the program. For more details 
     look Bug #372. */
    if (octeon_pci_bootcmd[0] != 0 && system (octeon_pci_bootcmd) != 0)
    {
       printf ("Executing %s failed\n", octeon_pci_bootcmd);
       exit (1);
    }

   if (argc > 1)
   {
       address = strtoul (argv[1], NULL, 16);
       address_supplied = 1;
   }

    if (address == 0)
    {
        /* If address == 0, then look up load named block, and load the file there */
        uint64_t base_addr, size;
        if (octeon_remote_named_block_find("__tmp_load", &base_addr, &size))
        {
            address = base_addr;
            debug_printf("Loading to address 0x%llx\n", address);
        }
        else
        {
            printf("ERROR: Unable to find named block for loading image.\n");
            exit(-1);
        }
    }

    infile = fopen(execfile, "r");
    if (infile == NULL)
    {
        printf ("Unable to open exec file: %s", execfile);
        exit (-1);
    }

    /* Explicitly lock access to the bus, otherwise this is done for every
    ** read/write, which slows the load way down.
    */
    octeon_remote_lock();
    cur_address = address;
    while (!feof(infile))
    {
        int count = fread(buffer, 1, sizeof(buffer), infile);
        if (count > 0)
        {
            octeon_remote_write_mem(cur_address, buffer, count);
            cur_address += count;
            filesize += count;
        }
    }
    octeon_remote_unlock();

    sprintf(buffer, "setenv filesize 0x%llx", (long long)filesize);
    if (octeon_remote_send_bootcmd(buffer, 50))
    {
        printf("Error setting filesize in u-boot.\n");
        exit (-1);
    }
    sprintf(buffer, "setenv fileaddr 0x%llx", (long long)address);
    if (octeon_remote_send_bootcmd(buffer, 50))
    {
        printf("Error setting fileaddr in u-boot.\n");
        exit (-1);
    }

   fclose(infile);


    buffer[0] = 0;
    /* If debug was not supplied on the command-line add it.  */
    for (; argc; argv++, argc--)
    {
        if (strcmp (*argv, "endbootarg") == 0 && !seen_debug)
        {
            strcat(buffer, " debug");
            seen_debug = 1;
        }
        strcat (buffer, " "); 
        strcat (buffer, *argv); 
        if (strncmp (*argv, "debug", 5) == 0)
            seen_debug = 1;
    }
    if (!address_supplied)
        strcat(buffer," 0");
    
    if (!seen_debug)
        strcat(buffer, " debug");

    printf("\nSending bootloader command: %s\n\n", buffer);

    if (octeon_remote_send_bootcmd(buffer, 50))
    {
        printf("Error sending command to bootloader.\n");
        exit (-1);
    }
    sleep (2);
}

void remote_open(char *targetstr, char *octeon_pci_bootcmd, char *execfile, int argc, char **argv)
{
    if (octeon_remote_open(targetstr, debug) < 0)
        exit(1);

    /* If the target is Linux then we need to mark this as a local
       use so we cannot debug the cores that this linux is
       running on. */
    if (strncmp (targetstr, "LINUX", 5) == 0)
       local = true;

    /* If debug-agent is passed an argument and this was not local debugging, then load and boot the program.
       Currently local booting is not support.  */
    if (argc > 0 && !local)
        load_and_boot (octeon_pci_bootcmd, execfile, argc, argv);

    if (get_debug_globals_addr() == 0)
    {
        send_verbose_error_response("Application does not have debugger enabled.\n");
        exit(1);
    }


    local_cores = get_local_coremask();
    debug_printf ("local cores = 0x%llx.\n", (unsigned long long)local_cores);

    if (check_version())
    {
        send_verbose_error_response("Version mismatch between debug agent and debugger stub.\n");
        exit (1);
    }

    active_cores = get_known_cores();
    if (!active_cores)
    {
        send_verbose_error_response("No cores are debugable\n");
        exit(1);
    }

    save_and_set_comm_type();
    set_active_cores (active_cores);
    focus_core = lowest_core(active_cores);
    set_focus_core (focus_core);

    if (argc == 0 || local)
        if (!stop_cores(active_cores))
        {
           start_cores (COMMAND_CONTINUE);
           restore_comm_type ();
           exit(1);
        }

    /* Wait/get the focus core just in case it was changed.  */
    while (read_core_context(focus_core, &focus_core_context) == COMMAND_NOT_FOCUS)
      focus_core = get_focus_core ();

    tlb_entries = get_tlb_entries();
}

void remote_close(void)
{
    /* Restore to the previous communication type if not local. */
    if (!local)
        restore_comm_type();
    /* Start the cores again.  */
    start_cores (COMMAND_CONTINUE);
    octeon_remote_close();
}

static __attribute__((format (printf, 3, 4))) void set_response(char *response, ssize_t *responselen, char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    if (!create_packet(response, responselen, format, ap))
        send_verbose_error_response("Packet exceeds max size (%zd): \"%30s ...\"\n", *responselen, response);
    va_end(ap);
}

static int gdb_regnum_to_offs(int regnum)
{
    if (regnum < 32)
        return offsetof(cvmx_debug_core_context_t, regs[regnum]);
    /* Just return fp registers offset in the context */
    if(37 < regnum && regnum < 70)
    {
        return offsetof(cvmx_debug_core_context_t, fp_regs[regnum-38]);
    }

    switch (regnum)
    {
#define CASE(NUM, FIELD) case NUM: return offsetof(cvmx_debug_core_context_t, FIELD)
        CASE(32, cop0.status);
        CASE(33, lo);
        CASE(34, hi);
        CASE(35, cop0.badvaddr);
        CASE(36, cop0.cause);
        CASE(37, cop0.depc);
        CASE(70, cop1.fcsr);
        CASE(71, cop1.fir);
#undef CASE
    }
    return -1;
}

/**
 * Convert a virtual address into a physical address
 *
 * @param vaddr      Virtual address
 * @return Physical address or 0xffffffffffffffff on failure (TLB miss)
 */
uint64_t virt_to_phys_addr(uint64_t vaddr)
{
    int i;

    /* KSEG0 and KSEG1 (uncached) addresses don't get translated */
    if ((vaddr>>30) == 0x3fffffffeull)
        return vaddr & 0x1fffffffull;

    /* XKPHYS addresses don't get translated */
    if ((vaddr>>62) == 0x2ull)
        return vaddr & 0xfffffffffull;

    /* The ASID is the lower 8 bits (7..0) of entryhi */
    int current_asid = FOCUS_CORE_VALUE(cop0.entryhi) & 0xff;

    for (i=0; i < tlb_entries; i++)
    {
        /* The pagemask lower 13 bits (12..0) are zero. We need to fix this.
            This also means that the which bit is included after the fix, so
            shift right by one */
        uint64_t pagemask = (FOCUS_CORE_VALUE(tlbs[i].pagemask) | 0x1fff) >> 1;
        /* Create a bitmask for the upper bits that aren't envolved in the
            page selection. This is the pagemask shifted one since each TLB
            maps two pages */
        uint64_t uppermask = (~pagemask)<<1;
        /* The ASID is the lower 8 bits (7..0) of entryhi */
        int asid = FOCUS_CORE_VALUE(tlbs[i].entryhi) & 0xff;
        /* We need either entrylo0 or entrylo1. Choose based on the bit just
            outside the pagemask. dpop returns the number of bits in a mask */
        int which = (vaddr >> cvmx_pop(pagemask)) & 0x1;
        int valid = FOCUS_CORE_VALUE(tlbs[i].entrylo[which]) & 0x2;
        int global = FOCUS_CORE_VALUE(tlbs[i].entrylo[which]) & 0x1;

        if (((current_asid == asid) || global) && (valid) &&
            ((vaddr & uppermask) == (FOCUS_CORE_VALUE(tlbs[i].entryhi) & uppermask)))
        {
            uint64_t paddr = FOCUS_CORE_VALUE(tlbs[i].entrylo[which]) >> 6;
            paddr = paddr << 12;
            paddr |= vaddr & pagemask;
            return paddr;
        }
    }

    return 0xffffffffffffffffull;
}

static void remote_wait(char *response, ssize_t *responselen)
{
    struct pollfd pfd;
    int known_cores;

    pfd.fd = gdb_fd;
    pfd.events = POLLIN;

    while (1)
    {
        int status;
	int command;
        cvmx_debug_core_context_t context;
	command = read_core_context(focus_core, &context);
	if (command && command != COMMAND_CONTINUE && command != COMMAND_STEP)
	{
	  int core = focus_core;
	  debug_printf ("core %d stopped with command: %d.\n", core, command);
	  /* Wait until the focus core has been decided on. */
          while (command == COMMAND_NOT_FOCUS)
          {
            usleep (1);
	    focus_core = get_focus_core ();
	    command = read_core_context(focus_core, &context);
          }
	  break;
	}
        status = poll(&pfd, 1, 1000);
        if (status == -1)
            perror_and_exit("poll");
        if (status)
        {
            SETRESP("+");
            return;
        }
    }

    known_cores = get_known_cores();
    /* The application has ended so return D0. */
    if (known_cores == 0)
    {
        /* Start all the finished cores. */
        start_cores(COMMAND_CONTINUE);
        SETRESP("D0"); 
        return;
    }

    read_core_context(focus_core, &focus_core_context);

    /* Assemble a "T" response.  */
    if (focus_core_context.cop0.debug & 0xc)
        SETRESP("T8:%x", (int) focus_core_context.hw_dbp.status);
    else
        SETRESP("T9");
}

/* Receive GDB protocol packets from gdb over gdb_stream, perform the
   corresponding operation over the remote-lib and send a response back over
   gdb_stream.  */

void remote_handle_packet(char *request, size_t requestlen, char *response, ssize_t *responselen)
{
    char code;
    char *orig_request = request;

    /* Skip $.  */
    request++;
    code = *request++;
    switch(code)
    {
        case '?':   /* What protocol do I speak? */
            SETRESP("S0A");
            break;

        case '\003':   /* Control-C */
            stop_cores(active_cores);
            /* Wait/get the focus core just in case it was changed.  */
            while (read_core_context(focus_core, &focus_core_context) == COMMAND_NOT_FOCUS)
                focus_core = get_focus_core ();
	    SETRESP("T9");
            break;
        case 'F':   /* Change the focus core */
	    {
		int core;
		sscanf(request, "%x", &core);

                /* Don't set focus to a core in reset */
                if (is_member(core, octeon_remote_read_csr (CVMX_CIU_PP_RST)))
		{
                    SETRESP ("!Core is in reset. It can't become the focus core\n");
		}
		/* Only cores in the exception handler may become the focus.
		   If a core not in the exception handler got focus the
		   debugger would hang since nobody would talk to it.  */
                else if (!is_member(core, debugable_cores()))
		{
                    SETRESP ("!Core is not in the exception handler. Focus not changed.\n");
		}
		else if (focus_core != core)
		{
		    set_focus_core(core);
                    if (focus_core != -1)
		    {
                       focus_core_context.remote_controlled = COMMAND_NOT_FOCUS;
                       write_core_context(focus_core, &focus_core_context);
		    }
                    usleep(2);
                    focus_core = core;
                    /* Wait until the focus core is ready to receive a command. */
                    while (read_core_context(focus_core, &focus_core_context) == COMMAND_NOT_FOCUS)
                        ;
                    
		}
		/* Fall through if failed.  Send back the old value.  */

	    }
	    /* fall through */
        case 'f':   /* Get the focus core */
	    focus_core = get_focus_core();
            read_core_context(focus_core, &focus_core_context);
	    SETRESP("F%02x", focus_core);
            break;
        case 'I':   /* Set the active cores */
            sscanf(request, "%"SCNx64, &active_cores);

            /* Limit the active mask to the known to exist cores */
            active_cores = active_cores & get_known_cores();

            /* Lazy user hack to have 0 be all cores */
            if (active_cores == 0)
                active_cores = debugable_cores();

            /* The focus core must be in the active_cores mask */
            if ((active_cores & (1ull << focus_core)) == 0)
            {
                SETRESP("!Focus core was added to the masked.");
                active_cores |= 1ull << focus_core;
            }

            set_active_cores(active_cores);

            /* Fall through. The reply to the set active cores command is the
               same as the get active cores command */
        case 'i':   /* Get the active cores */
            active_cores = get_active_cores();
            SETRESP("I%"PRIx64, active_cores);
            break;

        case 'A':   /* Setting the step mode all or one */
            step_all = *request == '1';
            set_step_all(step_all);
            /* Fall through. The reply to the set step-all command is the
	       same as the get step-all command */

        case 'a':   /* Getting the current step mode */
            step_all = get_step_all();
            SETRESP("A%x", step_all);
            break;

        case 'J': /* Set the flag for skip-over-isr in Single-Stepping mode */
        {
            if (*request == '1')
                step_isr = 1;   /* Step in ISR */
            else
                step_isr = 0;   /* Step over ISR */
	    set_step_isr (step_isr);
        }
        /* Fall through. The reply to the set step-isr command is the
           same as the get step-isr command */

        case 'j':   /* Reply with step_isr status  */
	    step_isr = get_step_isr ();
            SETRESP("J%x", step_isr);
            break;

        case 'g':   /* read a register from global place. */
	    {
		int regnum, offs;
                uint64_t *p = (uint64_t *) &focus_core_context;
		if (sscanf(request, "%x", &regnum) != 1)
                    goto error_packet;
                offs = gdb_regnum_to_offs(regnum) / sizeof(uint64_t);
                if (offs < 0)
                    SETRESP("E.unknown regnum: %d", regnum);
                else
                    SETRESP("%"PRIx64, p[offs]);
	    }
            break;

        case 'G':   /* set the value of a register. */
	    {
		int regnum, offs;
                uint64_t *p = (uint64_t *) &focus_core_context;
		uint64_t value;

		/* Get the register number to read */
		if (sscanf(request, "%x,%"SCNx64, &regnum, &value) != 2)
		    goto error_packet;

		offs = gdb_regnum_to_offs(regnum) / sizeof(uint64_t);
		if (offs < 0)
                    goto error_packet;
                else
                    p[offs] = value;
                /* There is no response. */
                *responselen = 0;
	    }
            break;

        case 'm':   /* Memory read. mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
	    {
		uint64_t addr, i, length;
		char *reply;

		if (sscanf(request, "%"SCNx64",%"SCNx64, &addr, &length) != 2)
		{
		    debug_printf("m packet corrupt: %s\n", request);
		    goto error_packet;
		}
		if (length >= 1024)
		{
		    debug_printf("m packet length out of range: %"PRIx64"\n", length);
		    goto error_packet;
		}

		reply = alloca(length * 2 + 1);
                /*  Fast path: There is no 4k page crossing.  */
                if ((addr & ~(1<<12)) == ((addr + length) & ~(1<<12)))
                {
                    unsigned char *membuf = alloca(length);
                    uint64_t paddr = virt_to_phys_addr (addr);
                    if (paddr == 0xffffffffffffffffull)
                    {
                        debug_printf("unknown virtual addr %"PRIx64"\n", addr);
                        goto error_packet;
                    }
                    octeon_remote_read_mem(membuf, paddr, length);
                    for (i = 0; i < length; i++)
                        sprintf(&reply[i * 2], "%02x", (unsigned)membuf[i]);
                }
                else
                {
		    for (i = 0; i < length; i++)
		    {
                        unsigned char byte;
                        uint64_t paddr = virt_to_phys_addr (addr + i);
                        if (paddr == 0xffffffffffffffffull)
                        {
                            debug_printf("unknown virtual addr %"PRIx64"\n", addr + i);
                            goto error_packet;
                        }
                        octeon_remote_read_mem(&byte, paddr, 1);
                        sprintf(&reply[i * 2], "%02x", (unsigned)byte);
		    }
                }
		SETRESP("%s", reply);
	    }
            break;

        case 'M':   /* Memory write. MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
	    {
		long long addr, i, length;
		char value[1024];
                int t;

		if ((t = sscanf(request, "%llx,%llx:%1024s", &addr, &length, value)) != 3)
		{
		    debug_printf("M packet corrupt: %d, %s\n", t, request);
		    goto error_packet;
		}

		for (i = 0; i < length; i++)
		{
		    char c;
                    int n;
                    int t;
                    char tempstr[3] = {0, 0, 0};
                    memcpy (tempstr, &value[i * 2], 2);
            
		    
                    uint64_t paddr = virt_to_phys_addr (addr + i);
                    if (paddr == 0xffffffffffffffffull)
                    {
                        debug_printf("unknown virtual addr %llx\n", addr + i);
		        goto error_packet;
                    }
                    n = sscanf(tempstr, "%2x", &t);
                    if (n != 1)
                    {
		        debug_printf("M packet corrupt, index: %d: %s\n", (int)i, tempstr);
		        goto error_packet;
                    }
                    c = t;
                    octeon_remote_write_mem(paddr, &c, 1);
		}
		SETRESP("+");
	    }
            break;

        case 'e':  /* Set/get performance counter events. e[1234]XX..X: [01]
                      is the performance counter to set X is the performance
                      event.  [34] is to get the same thing.  */
        {
            int perf_event = 0;
            int counter, encoded_counter;
            cvmx_debug_core_context_t *context = &focus_core_context;
            sscanf(request, "%1d%x", &encoded_counter, &perf_event);

            switch (encoded_counter)
            {
                case 1: /* Set performance counter0 event. */
                case 2: /* Set performance counter1 event. */

                counter = encoded_counter - 1;
                context->cop0.perfval[counter] = 0;
                set_perf_control_reg(context, perf_event, counter);
                /* There is no response. */
                *responselen = 0;
                break;

                case 3: /* Get performance counter0 event. */
                case 4: /* Get performance counter1 event. */
                {
                    cvmx_core_perf_control_t c;
                    counter = encoded_counter - 3;
                    /* Pass performance counter0 event and counter to
                       the debugger.  */
                    c.u32 = context->cop0.perfctrl[counter];
                    SETRESP("%llx,%llx", (long long) context->cop0.perfval[counter], (long long) c.s.event);
                }
                break;
            }
        }
        break;

#if 0
        case 't': /* Return the trace buffer read data register contents. */
        {
            uint64_t tra_data;
            uint64_t tra_ctl;
            char tmp[64];

            /* If trace buffer is disabled no trace data information is available. */
            if ((tra_ctl & 0x1) == 0)
            {
                cvmx_debug_putpacket("!Trace buffer not enabled\n");
                cvmx_debug_putpacket("t");
            }
            else
            {
                cvmx_debug_putpacket("!Trace buffer is enabled\n");
                tra_data = cvmx_read_csr(OCTEON_TRA_READ_DATA);
                mem2hex (&tra_data, tmp, 8);
                strcpy (debug_output_buffer, "t");
                strcat (debug_output_buffer, tmp);
                cvmx_debug_putpacket(debug_output_buffer);
            }
        }
        break;
#endif

        case 'Z': /* Insert hardware breakpoint: Z[di]NN..N,AA.A, [di] data or
                     instruction, NN..Nth at address AA..A */
        {
            enum type_t
            {
                WP_LOAD = 1,
                WP_STORE = 2,
                WP_ACCESS = 3
            };

            int num, size;
            long long addr;
            enum type_t type;
            char bp_type;
            const int BE = 1, TE = 4;
            int n;
            cvmx_debug_core_context_t *context = &focus_core_context;

            n = sscanf(request, "%c%x,%llx,%x,%x", &bp_type, &num, &addr, &size, &type);
            switch (bp_type)
            {
                case 'i':	// Instruction hardware breakpoint
                    if (n != 3 || num > 4)
                    {
                        debug_printf("Z packet corrupt: %s\n", request);
                        goto error_packet;
                    }

                    context->hw_ibp.address[num] = addr;
                    context->hw_ibp.address_mask[num] = 0;
                    context->hw_ibp.asid[num] = 0;
                    context->hw_ibp.control[num] = BE | TE;
                    break;

                case 'd':	// Data hardware breakpoint
                {
                    uint64_t dbc = 0xff0 | BE | TE;
                    uint64_t dbm;
                    if (n != 5 || num > 4)
                    {
                        debug_printf("Z packet corrupt: %s\n", request);
                        goto error_packet;
                    }

                    /* Set DBC[BE,TE,BLM]. */
                    context->hw_dbp.address[num] = addr;
                    context->hw_dbp.asid[num] = 0;

                    dbc |= type == WP_STORE ? 0x1000 : type == WP_LOAD ? 0x2000 : 0;
                    /* Mask the bits depending on the size for
                    debugger to stop while accessing parts of the
                    memory location.  */
                    dbm = (size == 8) ? 0x7 : ((size == 4) ? 3
                                        : (size == 2) ? 1 : 0);
                    context->hw_dbp.address_mask[num] = dbm;
                    context->hw_dbp.control[num] = dbc;
                    break;
                }
                default:
                    debug_printf("z packet corrupt: %s\n", request);
                    goto error_packet;
            }
            /* There is no response. */
            *responselen = 0;
        }
        break;

        case 'z': /* Remove hardware breakpoint: z[di]NN..N remove NN..Nth
breakpoint.  */
        {
            int num;
            char bp_type;
            cvmx_debug_core_context_t *context = &focus_core_context;

            if (sscanf(request, "%c%x", &bp_type, &num) != 2 || num > 4)
            {
                debug_printf("z packet corrupt: %s\n", request);
                goto error_packet;
            }

            switch (bp_type)
            {
                case 'i':	// Instruction hardware breakpoint
                    context->hw_ibp.address[num] = 0;
                    context->hw_ibp.address_mask[num] = 0;
                    context->hw_ibp.asid[num] = 0;
                    context->hw_ibp.control[num] = 0;
                    break;
                case 'd':	// Data hardware breakpoint
                    context->hw_dbp.address[num] = 0;
                    context->hw_dbp.address_mask[num] = 0;
                    context->hw_dbp.asid[num] = 0;
                    context->hw_dbp.control[num] = 0;
                    break;
                default:
                    debug_printf("z packet corrupt: %s\n", request);
                    goto error_packet;
            }
            /* There is no response. */
            *responselen = 0;
        }
        break;

        case 's':   /* Single step. sAA..AA Step one instruction from AA..AA (optional) */
        case 'c':   /* Continue. cAA..AA Continue at address AA..AA (optional) */
            {
                bool step = code == 's';

                write_core_context(focus_core, &focus_core_context);

		start_cores (step ? COMMAND_STEP : COMMAND_CONTINUE);
                stopped_cores = 0;
		usleep (200);

                /* Wait for the cores to stop unless we get stopped with a
                   ^C.  */
                remote_wait(response, responselen);
            }
            break;

        case '+':   /* Don't know. I think it is a communications sync */
            /* There is no response. */
            *responselen = 0;
            /* Ignoring this command */
            break;

        error_packet:
        default:
            debug_printf("Wrong request: \"%.*s\"\n", (int) requestlen, orig_request);
            SETRESP("-");
    }
}
