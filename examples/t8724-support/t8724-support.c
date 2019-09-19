#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cvmx-platform.h"
#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"

#include "imt_cpld.h"
#include "fans_mgmt.h"
#include "leds_mgmt.h"
#include "cortina_phys.h"

int main (int argc, char **argv)
{
	cvmx_sysinfo_t *sysinfo;
	cvmx_coremask_t coremask_all;

	cvmx_user_app_init();
	cvmx_coremask_copy(&coremask_all, &(cvmx_sysinfo_get())->core_mask);
	sysinfo = cvmx_sysinfo_get();

	/* The initial core configures the system. */
 	if (cvmx_is_init_core()) {
		printf ("Octeon RiscCore (built %s@%s)\n", __DATE__, __TIME__);

		/* Init cpld access */
		if(!cpld_init()) {
			printf("IM8724 CPLD init error!\n");
			return -1;
		}

		if (sysinfo->board_type == CVMX_BOARD_TYPE_CUST_IM8724)
		{
			printf("IM8724 board specific initialization is finished!\n");
		}

		/* Init fans management */
		fans_init();

		/* Init leds management */
		leds_init();

		/* Init pim modules settings */
		pim_set_defines();
	}
	cvmx_coremask_barrier_sync(&coremask_all);
   
	printf("Starting main application loop!\n");
	while (1)
	{
 		if (cvmx_is_init_core()) {
			/* Processt fans status */
			fans_mgmt();

			/* Process leds status */
			leds_mgmt();
		}
		cvmx_wait(10000000);
	}
	return 0;
}
