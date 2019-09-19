/***********************license start***************
 * Copyright (c) 2003-2015  Cavium Inc. (support@cavium.com). All rights
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

 * This Software, including technical data, may be subject to U.S. export  control
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


/**
 * @file
 *
 * Configuration and status register (CSR) address and type definitions for
 * Octeon. Include cvmx-csr.h instead of this file directly.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision: 130007 $<hr>
 *
 */
#ifndef __CVMX_CSR_TYPEDEFS_H__
#define __CVMX_CSR_TYPEDEFS_H__
#include "cvmx-agl-defs.h"
#include "cvmx-ase-defs.h"
#include "cvmx-asxx-defs.h"
#include "cvmx-asx0-defs.h"
#include "cvmx-bbp-defs.h"
#include "cvmx-bbxa-defs.h"
#include "cvmx-bbxbx-defs.h"
#include "cvmx-bbxc-defs.h"
#include "cvmx-bch-defs.h"
#include "cvmx-bgxx-defs.h"
#include "cvmx-bts-defs.h"
#include "cvmx-ciu2-defs.h"
#include "cvmx-ciu3-defs.h"
#include "cvmx-ciu-defs.h"
#include "cvmx-cprix-defs.h"
#include "cvmx-dbg-defs.h"
#include "cvmx-dencx-defs.h"
#include "cvmx-dfa-defs.h"
#include "cvmx-dfm-defs.h"
#include "cvmx-dlfe-defs.h"
#include "cvmx-dpi-defs.h"
#include "cvmx-dtx-defs.h"
#include "cvmx-eoi-defs.h"
#include "cvmx-fdeqx-defs.h"
#include "cvmx-fpa-defs.h"
#include "cvmx-gmxx-defs.h"
#include "cvmx-gpio-defs.h"
#include "cvmx-gserx-defs.h"
#include "cvmx-hna-defs.h"
#include "cvmx-ila-defs.h"
#include "cvmx-ilk-defs.h"
#include "cvmx-iob1-defs.h"
#include "cvmx-iob-defs.h"
#include "cvmx-iobn-defs.h"
#include "cvmx-iobp-defs.h"
#include "cvmx-ipd-defs.h"
#include "cvmx-key-defs.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-l2d-defs.h"
#include "cvmx-l2t-defs.h"
#include "cvmx-lapx-defs.h"
#include "cvmx-lbk-defs.h"
#include "cvmx-led-defs.h"
#include "cvmx-lmcx-defs.h"
#include "cvmx-mdabx-defs.h"
#include "cvmx-mdbwx-defs.h"
#include "cvmx-mhbwx-defs.h"
#include "cvmx-mio-defs.h"
#include "cvmx-mixx-defs.h"
#include "cvmx-mpix-defs.h"
#include "cvmx-mpi-defs.h"
#include "cvmx-ndf-defs.h"
#include "cvmx-npei-defs.h"
#include "cvmx-npi-defs.h"
#include "cvmx-nqm-defs.h"
#include "cvmx-oclax-defs.h"
#include "cvmx-ocx-defs.h"
#include "cvmx-osm-defs.h"
#include "cvmx-pci-defs.h"
#include "cvmx-pcieepx-defs.h"
#include "cvmx-pcieepvfx-defs.h"
#include "cvmx-pciercx-defs.h"
#include "cvmx-pcmx-defs.h"
#include "cvmx-pcm-defs.h"
#include "cvmx-pcsx-defs.h"
#include "cvmx-pcsxx-defs.h"
#include "cvmx-pemx-defs.h"
#include "cvmx-pescx-defs.h"
#include "cvmx-pip-defs.h"
#include "cvmx-pki-defs.h"
#include "cvmx-pko-defs.h"
#include "cvmx-pnbx-defs.h"
#include "cvmx-pow-defs.h"
#include "cvmx-prch-defs.h"
#include "cvmx-psm-defs.h"
#include "cvmx-rad-defs.h"
#include "cvmx-rdecx-defs.h"
#include "cvmx-rfifx-defs.h"
#include "cvmx-rfif-defs.h"
#include "cvmx-rmap-defs.h"
#include "cvmx-rnm-defs.h"
#include "cvmx-rst-defs.h"
#include "cvmx-sata-defs.h"
#include "cvmx-sdlx-defs.h"
#include "cvmx-sli-defs.h"
#include "cvmx-slitb-defs.h"
#include "cvmx-smix-defs.h"
#include "cvmx-smi-defs.h"
#include "cvmx-spemx-defs.h"
#include "cvmx-spxx-defs.h"
#include "cvmx-spx0-defs.h"
#include "cvmx-sriox-defs.h"
#include "cvmx-sriomaintx-defs.h"
#include "cvmx-srxx-defs.h"
#include "cvmx-sso-defs.h"
#include "cvmx-stxx-defs.h"
#include "cvmx-tdecx-defs.h"
#include "cvmx-tim-defs.h"
#include "cvmx-tofb-defs.h"
#include "cvmx-tospx-defs.h"
#include "cvmx-trax-defs.h"
#include "cvmx-uahcx-defs.h"
#include "cvmx-uctlx-defs.h"
#include "cvmx-ulfe-defs.h"
#include "cvmx-usbcx-defs.h"
#include "cvmx-usbdrdx-defs.h"
#include "cvmx-usbnx-defs.h"
#include "cvmx-vdecx-defs.h"
#include "cvmx-vrmx-defs.h"
#include "cvmx-wpse-defs.h"
#include "cvmx-wrce-defs.h"
#include "cvmx-wrde-defs.h"
#include "cvmx-wrse-defs.h"
#include "cvmx-wtxe-defs.h"
#include "cvmx-xcv-defs.h"
#include "cvmx-xsx-defs.h"
#include "cvmx-zip-defs.h"

#include "cvmx-pexp-defs.h"
#endif /* __CVMX_CSR_TYPEDEFS_H__ */
