
#include <common.h>

#include "cvmx-helper-util.h"
#include "cvmx.h"
#include "cvmx-access-native.h"
#include "cvmx-helper-bgx.h"
#include <cvmx-qlm.h>


static int do_oct_read_qlmregs (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
int		qlm;
uint64_t	reg;
uint64_t	val = 0;

	if (argc != 2)
		return CMD_RET_USAGE;

	qlm = simple_strtoul(argv[1], NULL, 10);

	reg = 0x1180090000000 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_PHY_CTL]    0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x1180090000008 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_REFCLK_SEL] 0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x1180090000010 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_PLL_STAT]   0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x1180090000080 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_CFG]        0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x1180090000088 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_SPD]        0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x11800900000A0 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_QLM_STAT]   0x%llx => 0x%llx\n", qlm, reg, val);

	reg = 0x1180090000118 + qlm*0x1000000 ;
	val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
	printf("[GSER%02d_LANE_MODE]  0x%llx => 0x%llx\n", qlm, reg, val);

	return CMD_RET_SUCCESS;
}


/*============================================================================================================================*/

U_BOOT_CMD(
	oct_qlm_dump, 2, 1, do_oct_read_qlmregs,
	"Read cn78xx QLM GSER registers",
	"oct_qlm_dump qlm  (qlm id)\n"
);

