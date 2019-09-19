#include <common.h>
#include <asm/mipsregs.h>
#include <asm/arch/octeon_boot.h>
#include <asm/arch/octeon_board_common.h>
#include <asm/arch/lib_octeon_shared.h>
#include <asm/arch/lib_octeon.h>
#include <asm/arch/cvmx.h>
#include <asm/arch/cvmx-app-init.h>
#include <asm/arch/octeon-model.h>

DECLARE_GLOBAL_DATA_PTR;

int early_board_init(void)
{
	int cpu_ref = (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) / 1000000;

	octeon_board_get_clock_info(IM8724_DEF_DRAM_FREQ);
	octeon_board_get_descriptor(CVMX_BOARD_TYPE_CUST_IM8724, 1, 0);

        /* Set for CN78XX alternative 100 MHz reference clock,
         * instead of 50 Hz: (DEFAULT_CPU_REF_FREQUENCY_MHZ * 1000 * 1000ull) */
        gd->arch.ddr_ref_hertz = 100000000ull;

	/* Even though the CPU ref freq is stored in the clock descriptor, we
	 * don't read it here.  The MCU reads it and configures the clock, and
	 * we read how the clock is actually configured.
	 * The bootloader does not need to read the clock descriptor tuple for
	 * normal operation on rev 2 and later boards.
	 */
	octeon_board_get_mac_addr();

	/* Read CPU clock multiplier */
	gd->cpu_clk = octeon_get_cpu_multiplier() * cpu_ref * 1000000;

	return 0;
}

int late_board_init(void)
{
        char buf[256];
        char *board_name;
        board_name = getenv("boardname");
        if (board_name) {
                sprintf(buf, "u-boot-octeon_%s.bin", board_name);
                setenv("octeon_stage3_bootloader", buf);
                debug("Detected board type %s\n", board_name);
                sprintf(buf, "%s (SPI stage 2)", board_name);
                setenv("prompt", buf);
        }
	return 0;
}
