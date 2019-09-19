#/***********************license start***************
# * Copyright (c) 2003-2007  Cavium Inc. (support@cavium.com). All rights
# * reserved.
# *
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions are
# * met:
# *
# *     * Redistributions of source code must retain the above copyright
# *       notice, this list of conditions and the following disclaimer.
# *
# *     * Redistributions in binary form must reproduce the above
# *       copyright notice, this list of conditions and the following
# *       disclaimer in the documentation and/or other materials provided
# *       with the distribution.
# *
# *     * Neither the name of Cavium Inc. nor the names of
# *       its contributors may be used to endorse or promote products
# *       derived from this software without specific prior written
# *       permission.
# *
# * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
# * AND WITH ALL FAULTS AND CAVIUM NETWORKS MAKES NO PROMISES, REPRESENTATIONS
# * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
# * RESPECT TO THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY
# * REPRESENTATION OR DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT
# * DEFECTS, AND CAVIUM SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES
# * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR
# * PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET
# * POSSESSION OR CORRESPONDENCE TO DESCRIPTION.  THE ENTIRE RISK ARISING OUT
# * OF USE OR PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
# *
# *
# * For any questions regarding licensing please contact marketing@cavium.com
# *
# ***********************license end**************************************/
# This makefile fragment contains the architecture independent
# configuration options needed for proper simple exectutive compilation.
# This should be included by any makefiles using all or portions of the
# simple executive, and the CFLAGS_COMMON_CONFIG variable should be added
# to the CFLAGS used to compile the simple executive with.



DEPRECATED_MODELS= OCTEON_PASS2 OCTEON_PASS1
MODELS_STRING=  $(shell cat ${OCTEON_ROOT}/octeon-models.txt)

ifndef OCTEON_MODEL
  ${error OCTEON_MODEL not set.  Supported values: ${MODELS_STRING}}
endif

# Do 36XX to 38XX translation
OCTEON_MODEL_TMP := $(subst CN36XX,CN38XX,${OCTEON_MODEL})
OCTEON_MODEL:=${OCTEON_MODEL_TMP}

ifndef OCTEON_ROOT
  ${error OCTEON_ROOT not set}
endif

#
MATCHED_MODEL=${findstring ${OCTEON_MODEL}, ${DEPRECATED_MODELS}}
ifeq (${MATCHED_MODEL}, ${OCTEON_MODEL})
${warning OCTEON_MODEL of ${OCTEON_MODEL} is deprecated. Please use a model from ${OCTEON_ROOT}/octeon-models.txt.}
else
MATCHED_MODEL=${findstring ${OCTEON_MODEL}, ${MODELS_STRING}}

ifneq (${MATCHED_MODEL}, ${OCTEON_MODEL})
${error Invalid OCTEON_MODEL: ${OCTEON_MODEL}.  Valid values: ${MODELS_STRING}}
endif



endif




CFLAGS_COMMON_CONFIG += -DOCTEON_MODEL=${MATCHED_MODEL}
