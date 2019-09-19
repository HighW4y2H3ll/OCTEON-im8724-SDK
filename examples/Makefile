######################*license start*###################################
#   Copyright (c) 2003-2008 Cavium Inc. (support@cavium.com). All rights
#   reserved.
#
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#       * Redistributions of source code must retain the above copyright
#         notice, this list of conditions and the following disclaimer.
#
#       * Redistributions in binary form must reproduce the above
#         copyright notice, this list of conditions and the following
#         disclaimer in the documentation and/or other materials provided
#         with the distribution.
#
#       * Neither the name of Cavium Inc. nor the names of
#         its contributors may be used to endorse or promote products
#         derived from this software without specific prior written
#         permission.
#
#   TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
#   AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
#   OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
#   RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
#   REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
#   DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
#   OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
#   PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
#   POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
#   OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
#
#
#   For any questions regarding licensing please contact marketing@cavium.com

#
# File version info: $Id: Makefile 124429 2015-09-01 21:58:44Z cchavva $

examples = adma application-args coredump crypto debugger hello \
	   hotplug-app linux-filter low-latency-mem mailbox named-block \
	   passthrough passthrough_o3 pcie-scan power-throttle queue \
	   timer traffic-gen uart zip
EXIT=exit

.PHONY: all
all:
	for f in ${examples}; do echo "**** $$f" ; ${MAKE} -C $$f || ${EXIT} ; done

.PHONY: clean
clean:
	for f in ${examples}; do ${MAKE} -C $$f clean; done
	for f in ${examples}; do ${MAKE} -C $$f clean OCTEON_TARGET=cvmx_n32; done
	for f in ${examples}; do ${MAKE} -C $$f clean OCTEON_TARGET=linux_64; done
	for f in ${examples}; do ${MAKE} -C $$f clean OCTEON_TARGET=linux_n32; done

