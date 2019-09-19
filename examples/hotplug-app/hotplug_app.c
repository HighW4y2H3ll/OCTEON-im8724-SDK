/***********************license start***************
 * Copyright (c) 2003-2017  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cvmx-config.h"
#include "cvmx.h"
#include "cvmx-sysinfo.h"
#include "cvmx-app-hotplug.h"

/* Data type for argument passed to the application hotplug shutdown callback
   routine */

typedef struct hotplug_callback_args {
    void *mem_ptr;
} hotplug_callback_args_t;

void shutdown_callback_func(hotplug_callback_args_t* data);
void unplug_callback_func(hotplug_callback_args_t* data);
void cores_removed_callback(cvmx_coremask_t * coremask,
		hotplug_callback_args_t* data);
void cores_added_callback(cvmx_coremask_t * coremask,
		hotplug_callback_args_t* data);

static int volatile core_uplug_requested   = 0;
static int volatile app_shutdown_requested = 0;

CVMX_SHARED hotplug_callback_args_t hotplug_callback_data;


int main(int argc, char **argv )
{
    cvmx_sysinfo_t *sys_info_ptr;
    cvmx_coremask_t coremask;
    int app_shutdown = 1;
    int i;

    hotplug_callback_data.mem_ptr = (void*)0xA5A5A5A5;
    cvmx_coremask_clear_all( &coremask );

    cvmx_user_app_init();

    sys_info_ptr = cvmx_sysinfo_get();

    if (cvmx_is_init_core())
    {
        cvmx_app_hotplug_callbacks_t cb;
        bzero(&cb, sizeof(cb));
        cb.cores_added_callback =  (void *) cores_added_callback;
        cb.shutdown_callback =  (void *) shutdown_callback_func;
        cb.unplug_core_callback =  (void *) unplug_callback_func;
        cb.cores_removed_callback =  (void *) cores_removed_callback ;

        /* Register application for hotplug. this only needs to be done once */
        cvmx_app_hotplug_register_cb(&cb, &hotplug_callback_data, app_shutdown);
        printf("hotplug registered\n");
    }

    /* Activate hotplug on all cores */
    if (cvmx_app_hotplug_activate())
    {
        printf("ERROR: cvmx_hotplug_activate() failed\n");
        return -1;
    }

    /* hotplug registration is complete */


    printf("Hotplug app is running on core%d, init core=%d\n",
		cvmx_get_core_num(), (int)sys_info_ptr->init_core );

    /* Display command-line arguments */
    printf("argc=%d, args: ", argc);
    for( i = 0; i < argc; i++)
	printf("'%s', ", argv[i]);
    printf("\n");

    /* wait for the hotplug notification */
    while(1)
    {

	/* Report changes in coremask */
	if( cvmx_coremask_cmp( &coremask, &sys_info_ptr->core_mask))
	{
	    printf("Sysinfo coremask changed to: ");
	    cvmx_coremask_print( &sys_info_ptr->core_mask );
	    cvmx_coremask_copy( &coremask, &sys_info_ptr->core_mask );
	}


        if (core_uplug_requested || app_shutdown_requested)
        {
            printf("core=%d : is unplugged\n",cvmx_get_core_num());
            cvmx_app_hotplug_core_shutdown();
        }
#if 0
	/* Periodically print progress counters */
	{
	int64_t cnt=0;
	int interval = 100000000;
	if (cnt % interval == 0)
            {
		printf("core=%d : cnt=%ld\n",cvmx_get_core_num(),cnt);
            }
        cnt++;
	}
#endif
    }

    return 0;
}

/*
 * This callback notifies all cores when they are being shutdown
 */
void shutdown_callback_func(hotplug_callback_args_t* data)
{
#if QUIET_CALLBACKS == 0
    printf("Shutdown callback called for core #%d data %p\n",
               cvmx_get_core_num(), data->mem_ptr);
#endif
    app_shutdown_requested = 1;
}

/*
 * This callback is invoked for all cores that are being unplugged
 * from a running application.
 */
void unplug_callback_func(hotplug_callback_args_t* data)
{
#if QUIET_CALLBACKS == 0
    printf("Unplug callback called for core #%d data %p\n",
	    cvmx_get_core_num(), data->mem_ptr);
#endif
    core_uplug_requested = 1;
}

/*
 * This callback is invoked on the remaining cures
 * when some cores are unplugged from an application,
 * with the <coremask> indicating which cores are being unplugged.
 * The cores being unpligged will be notified via <unplug_callback>.
 * There is no guarantee of a particular order between these two notifications.
 */
void cores_removed_callback(cvmx_coremask_t * coremask,
		hotplug_callback_args_t* data)
{
#if QUIET_CALLBACKS == 0
    printf("Cores removed callback called for core #%d data %p cores_removed=",
	    cvmx_get_core_num(),  data->mem_ptr);
    cvmx_coremask_print( coremask );
#endif
}


/*
 * This callback is invoked when cores are added to an app,
 * it will be run on all prior app cores,
 * and the <coremask> will inform them of the new cores that have been
 * added.
 * This callback is invoked after the new cores have been started,
 * and the sysinfo_ptr->coremask already shows all cores the app is running on.
 */
void cores_added_callback(cvmx_coremask_t * coremask,
		hotplug_callback_args_t* data)
{
#if QUIET_CALLBACKS == 0
    printf("Cores added callback called for core #%d  data %p coremask=",
	    cvmx_get_core_num(), data->mem_ptr);
    cvmx_coremask_print( coremask );
#endif
}
