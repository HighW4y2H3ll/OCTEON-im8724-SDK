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
 * Automatically generated error messages for cn70xx.
 *
 * This file is auto generated. Do not edit.
 *
 * <hr>$Revision$<hr>
 *
 * <hr><h2>Error tree for CN70XX</h2>
 * @dot
 * digraph cn70xx
 * {
 *     rankdir=LR;
 *     node [shape=record, width=.1, height=.1, fontsize=8, font=helvitica];
 *     edge [fontsize=7, font=helvitica];
 *     cvmx_root [label="ROOT|<root>root"];
 *     cvmx_ciu_int0_sum0 [label="CIU_INTX_SUM0(0)|<powiq>powiq"];
 *     cvmx_pow_iq_int [label="POW_IQ_INT|<iq_int>iq_int"];
 *     cvmx_ciu_int0_sum0:powiq:e -> cvmx_pow_iq_int [label="powiq"];
 *     cvmx_root:root:e -> cvmx_ciu_int0_sum0 [label="root"];
 *     cvmx_ciu_int_sum1 [label="CIU_INT_SUM1|<l2c>l2c|<ipd>ipd|<pow>pow|<rad>rad|<pip>pip|<pko>pko|<pem2>pem2|<pem0>pem0|<pem1>pem1|<fpa>fpa|<mio>mio|<dfa>dfa|<tim>tim|<lmc0>lmc0|<key>key|<rst>rst|<iob>iob|<agl>agl|<agx0>agx0|<agx1>agx1|<sli>sli|<dpi>dpi"];
 *     cvmx_ciu_cib_l2c_raw0 [label="CIU_CIB_L2C_RAWX(0)|<tadx_int_l2dsbe>tadx_int_l2dsbe|<tadx_int_l2ddbe>tadx_int_l2ddbe|<tadx_int_sbfsbe>tadx_int_sbfsbe|<tadx_int_sbfdbe>tadx_int_sbfdbe|<tadx_int_fbfsbe>tadx_int_fbfsbe|<tadx_int_fbfdbe>tadx_int_fbfdbe|<tadx_int_tagsbe>tadx_int_tagsbe|<tadx_int_tagdbe>tadx_int_tagdbe|<tadx_int_noway>tadx_int_noway|<tadx_int_holewr>tadx_int_holewr|<tadx_int_holerd>tadx_int_holerd|<tadx_int_bigwr>tadx_int_bigwr|<tadx_int_bigrd>tadx_int_bigrd|<tadx_int_wrdislmc>tadx_int_wrdislmc|<tadx_int_rddislmc>tadx_int_rddislmc|<tadx_int_rtgsbe>tadx_int_rtgsbe|<tadx_int_rtgdbe>tadx_int_rtgdbe|<mcix_int_vbfsbe>mcix_int_vbfsbe|<mcix_int_vbfdbe>mcix_int_vbfdbe|<cbcx_int_rsdsbe>cbcx_int_rsdsbe|<cbcx_int_rsddbe>cbcx_int_rsddbe|<cbcx_int_ioccmdsbe>cbcx_int_ioccmdsbe|<cbcx_int_ioccmddbe>cbcx_int_ioccmddbe"];
 *     cvmx_ciu_int_sum1:l2c:e -> cvmx_ciu_cib_l2c_raw0 [label="l2c"];
 *     cvmx_ipd_int_sum [label="IPD_INT_SUM|<prc_par0>prc_par0|<prc_par1>prc_par1|<prc_par2>prc_par2|<prc_par3>prc_par3|<bp_sub>bp_sub|<dc_ovr>dc_ovr|<cc_ovr>cc_ovr|<c_coll>c_coll|<d_coll>d_coll|<bc_ovr>bc_ovr"];
 *     cvmx_ciu_int_sum1:ipd:e -> cvmx_ipd_int_sum [label="ipd"];
 *     cvmx_pow_ecc_err [label="POW_ECC_ERR|<sbe>sbe|<dbe>dbe|<rpe>rpe|<iop>iop"];
 *     cvmx_ciu_int_sum1:pow:e -> cvmx_pow_ecc_err [label="pow"];
 *     cvmx_rad_reg_error [label="RAD_REG_ERROR|<doorbell>doorbell"];
 *     cvmx_ciu_int_sum1:rad:e -> cvmx_rad_reg_error [label="rad"];
 *     cvmx_pip_int_reg [label="PIP_INT_REG|<prtnxa>prtnxa|<badtag>badtag|<skprunt>skprunt|<todoovr>todoovr|<feperr>feperr|<beperr>beperr|<punyerr>punyerr"];
 *     cvmx_ciu_int_sum1:pip:e -> cvmx_pip_int_reg [label="pip"];
 *     cvmx_pko_reg_error [label="PKO_REG_ERROR|<parity>parity|<doorbell>doorbell|<currzero>currzero"];
 *     cvmx_ciu_int_sum1:pko:e -> cvmx_pko_reg_error [label="pko"];
 *     cvmx_pem2_int_sum [label="PEMX_INT_SUM(2)|<se>se|<up_b1>up_b1|<up_b2>up_b2|<up_bx>up_bx|<un_b1>un_b1|<un_b2>un_b2|<un_bx>un_bx|<rdlk>rdlk|<crs_er>crs_er|<crs_dr>crs_dr|<exc>exc"];
 *     cvmx_pem2_dbg_info [label="PEMX_DBG_INFO(2)|<spoison>spoison|<rtlplle>rtlplle|<recrce>recrce|<rpoison>rpoison|<rcemrc>rcemrc|<rnfemrc>rnfemrc|<rfemrc>rfemrc|<rpmerc>rpmerc|<rptamrc>rptamrc|<rumep>rumep|<rvdm>rvdm|<acto>acto|<rte>rte|<mre>mre|<rdwdle>rdwdle|<rtwdle>rtwdle|<dpeoosd>dpeoosd|<fcpvwt>fcpvwt|<rpe>rpe|<fcuv>fcuv|<rqo>rqo|<rauc>rauc|<racur>racur|<racca>racca|<caar>caar|<rarwdns>rarwdns|<ramtlp>ramtlp|<racpp>racpp|<rawwpp>rawwpp|<ecrc_e>ecrc_e|<rtry_pe>rtry_pe|<hdrq_pe>hdrq_pe|<datq_pe>datq_pe|<p_d_sbe>p_d_sbe|<p_d_dbe>p_d_dbe|<p_c_sbe>p_c_sbe|<p_c_dbe>p_c_dbe|<n_d_sbe>n_d_sbe|<n_d_dbe>n_d_dbe|<n_c_sbe>n_c_sbe|<n_c_dbe>n_c_dbe|<c_d_sbe>c_d_sbe|<c_d_dbe>c_d_dbe|<c_c_sbe>c_c_sbe|<c_c_dbe>c_c_dbe"];
 *     cvmx_pem2_int_sum:exc:e -> cvmx_pem2_dbg_info [label="exc"];
 *     cvmx_ciu_int_sum1:pem2:e -> cvmx_pem2_int_sum [label="pem2"];
 *     cvmx_pem0_int_sum [label="PEMX_INT_SUM(0)|<se>se|<up_b1>up_b1|<up_b2>up_b2|<up_bx>up_bx|<un_b1>un_b1|<un_b2>un_b2|<un_bx>un_bx|<rdlk>rdlk|<crs_er>crs_er|<crs_dr>crs_dr|<exc>exc"];
 *     cvmx_pem0_dbg_info [label="PEMX_DBG_INFO(0)|<spoison>spoison|<rtlplle>rtlplle|<recrce>recrce|<rpoison>rpoison|<rcemrc>rcemrc|<rnfemrc>rnfemrc|<rfemrc>rfemrc|<rpmerc>rpmerc|<rptamrc>rptamrc|<rumep>rumep|<rvdm>rvdm|<acto>acto|<rte>rte|<mre>mre|<rdwdle>rdwdle|<rtwdle>rtwdle|<dpeoosd>dpeoosd|<fcpvwt>fcpvwt|<rpe>rpe|<fcuv>fcuv|<rqo>rqo|<rauc>rauc|<racur>racur|<racca>racca|<caar>caar|<rarwdns>rarwdns|<ramtlp>ramtlp|<racpp>racpp|<rawwpp>rawwpp|<ecrc_e>ecrc_e|<rtry_pe>rtry_pe|<hdrq_pe>hdrq_pe|<datq_pe>datq_pe|<p_d_sbe>p_d_sbe|<p_d_dbe>p_d_dbe|<p_c_sbe>p_c_sbe|<p_c_dbe>p_c_dbe|<n_d_sbe>n_d_sbe|<n_d_dbe>n_d_dbe|<n_c_sbe>n_c_sbe|<n_c_dbe>n_c_dbe|<c_d_sbe>c_d_sbe|<c_d_dbe>c_d_dbe|<c_c_sbe>c_c_sbe|<c_c_dbe>c_c_dbe"];
 *     cvmx_pem0_int_sum:exc:e -> cvmx_pem0_dbg_info [label="exc"];
 *     cvmx_ciu_int_sum1:pem0:e -> cvmx_pem0_int_sum [label="pem0"];
 *     cvmx_pem1_int_sum [label="PEMX_INT_SUM(1)|<se>se|<up_b1>up_b1|<up_b2>up_b2|<up_bx>up_bx|<un_b1>un_b1|<un_b2>un_b2|<un_bx>un_bx|<rdlk>rdlk|<crs_er>crs_er|<crs_dr>crs_dr|<exc>exc"];
 *     cvmx_pem1_dbg_info [label="PEMX_DBG_INFO(1)|<spoison>spoison|<rtlplle>rtlplle|<recrce>recrce|<rpoison>rpoison|<rcemrc>rcemrc|<rnfemrc>rnfemrc|<rfemrc>rfemrc|<rpmerc>rpmerc|<rptamrc>rptamrc|<rumep>rumep|<rvdm>rvdm|<acto>acto|<rte>rte|<mre>mre|<rdwdle>rdwdle|<rtwdle>rtwdle|<dpeoosd>dpeoosd|<fcpvwt>fcpvwt|<rpe>rpe|<fcuv>fcuv|<rqo>rqo|<rauc>rauc|<racur>racur|<racca>racca|<caar>caar|<rarwdns>rarwdns|<ramtlp>ramtlp|<racpp>racpp|<rawwpp>rawwpp|<ecrc_e>ecrc_e|<rtry_pe>rtry_pe|<hdrq_pe>hdrq_pe|<datq_pe>datq_pe|<p_d_sbe>p_d_sbe|<p_d_dbe>p_d_dbe|<p_c_sbe>p_c_sbe|<p_c_dbe>p_c_dbe|<n_d_sbe>n_d_sbe|<n_d_dbe>n_d_dbe|<n_c_sbe>n_c_sbe|<n_c_dbe>n_c_dbe|<c_d_sbe>c_d_sbe|<c_d_dbe>c_d_dbe|<c_c_sbe>c_c_sbe|<c_c_dbe>c_c_dbe"];
 *     cvmx_pem1_int_sum:exc:e -> cvmx_pem1_dbg_info [label="exc"];
 *     cvmx_ciu_int_sum1:pem1:e -> cvmx_pem1_int_sum [label="pem1"];
 *     cvmx_fpa_int_sum [label="FPA_INT_SUM|<fed0_sbe>fed0_sbe|<fed0_dbe>fed0_dbe|<fed1_sbe>fed1_sbe|<fed1_dbe>fed1_dbe|<q0_und>q0_und|<q0_coff>q0_coff|<q0_perr>q0_perr|<q1_und>q1_und|<q1_coff>q1_coff|<q1_perr>q1_perr|<q2_und>q2_und|<q2_coff>q2_coff|<q2_perr>q2_perr|<q3_und>q3_und|<q3_coff>q3_coff|<q3_perr>q3_perr|<q4_und>q4_und|<q4_coff>q4_coff|<q4_perr>q4_perr|<q5_und>q5_und|<q5_coff>q5_coff|<q5_perr>q5_perr|<q6_und>q6_und|<q6_coff>q6_coff|<q6_perr>q6_perr|<q7_und>q7_und|<q7_coff>q7_coff|<q7_perr>q7_perr|<pool0th>pool0th|<pool1th>pool1th|<pool2th>pool2th|<pool3th>pool3th|<pool4th>pool4th|<pool5th>pool5th|<pool6th>pool6th|<pool7th>pool7th|<free0>free0|<free1>free1|<free2>free2|<free3>free3|<free4>free4|<free5>free5|<free6>free6|<free7>free7|<paddr_e>paddr_e"];
 *     cvmx_ciu_int_sum1:fpa:e -> cvmx_fpa_int_sum [label="fpa"];
 *     cvmx_mio_boot_err [label="MIO_BOOT_ERR|<adr_err>adr_err|<wait_err>wait_err"];
 *     cvmx_ciu_int_sum1:mio:e -> cvmx_mio_boot_err [label="mio"];
 *     cvmx_dfa_error [label="DFA_ERROR|<dblovf>dblovf|<dc0perr>dc0perr|<dlc0_ovferr>dlc0_ovferr|<dfanxm>dfanxm|<replerr>replerr"];
 *     cvmx_ciu_int_sum1:dfa:e -> cvmx_dfa_error [label="dfa"];
 *     cvmx_tim_reg_error [label="TIM_REG_ERROR|<mask>mask"];
 *     cvmx_ciu_int_sum1:tim:e -> cvmx_tim_reg_error [label="tim"];
 *     cvmx_ciu_cib_lmc0_raw0 [label="CIU_CIB_LMCX_RAWX(0,0)|<int_sec_errx>int_sec_errx|<int_ded_errx>int_ded_errx|<int_nxm_wr_err>int_nxm_wr_err"];
 *     cvmx_ciu_int_sum1:lmc0:e -> cvmx_ciu_cib_lmc0_raw0 [label="lmc0"];
 *     cvmx_key_int_sum [label="KEY_INT_SUM|<key_sbe>key_sbe|<key_dbe>key_dbe"];
 *     cvmx_ciu_int_sum1:key:e -> cvmx_key_int_sum [label="key"];
 *     cvmx_ciu_cib_rst_raw0 [label="CIU_CIB_RST_RAWX(0)|<int_linkx>int_linkx|<int_perstx>int_perstx"];
 *     cvmx_ciu_int_sum1:rst:e -> cvmx_ciu_cib_rst_raw0 [label="rst"];
 *     cvmx_iob_int_sum [label="IOB_INT_SUM|<np_sop>np_sop|<np_eop>np_eop|<p_sop>p_sop|<p_eop>p_eop|<np_dat>np_dat|<p_dat>p_dat"];
 *     cvmx_ciu_int_sum1:iob:e -> cvmx_iob_int_sum [label="iob"];
 *     cvmx_agl_gmx_bad_reg [label="AGL_GMX_BAD_REG|<ovrflw>ovrflw|<txpop>txpop|<txpsh>txpsh|<out_ovr>out_ovr|<loststat>loststat"];
 *     cvmx_ciu_int_sum1:agl:e -> cvmx_agl_gmx_bad_reg [label="agl"];
 *     cvmx_agl_gmx_rx0_int_reg [label="AGL_GMX_RXX_INT_REG(0)|<skperr>skperr|<ovrerr>ovrerr"];
 *     cvmx_ciu_int_sum1:agl:e -> cvmx_agl_gmx_rx0_int_reg [label="agl"];
 *     cvmx_agl_gmx_tx_int_reg [label="AGL_GMX_TX_INT_REG|<pko_nxa>pko_nxa|<undflw>undflw"];
 *     cvmx_ciu_int_sum1:agl:e -> cvmx_agl_gmx_tx_int_reg [label="agl"];
 *     cvmx_gmx0_bad_reg [label="GMXX_BAD_REG(0)|<out_ovr>out_ovr|<loststat>loststat|<statovr>statovr|<inb_nxa>inb_nxa"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_bad_reg [label="agx0"];
 *     cvmx_gmx0_rx0_int_reg [label="GMXX_RXX_INT_REG(0,0)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_rx0_int_reg [label="agx0"];
 *     cvmx_gmx0_rx1_int_reg [label="GMXX_RXX_INT_REG(1,0)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_rx1_int_reg [label="agx0"];
 *     cvmx_gmx0_rx2_int_reg [label="GMXX_RXX_INT_REG(2,0)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_rx2_int_reg [label="agx0"];
 *     cvmx_gmx0_rx3_int_reg [label="GMXX_RXX_INT_REG(3,0)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_rx3_int_reg [label="agx0"];
 *     cvmx_gmx0_tx_int_reg [label="GMXX_TX_INT_REG(0)|<pko_nxa>pko_nxa|<undflw>undflw|<ptp_lost>ptp_lost"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_gmx0_tx_int_reg [label="agx0"];
 *     cvmx_pcs0_int0_reg [label="PCSX_INTX_REG(0,0)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_pcs0_int0_reg [label="agx0"];
 *     cvmx_pcs0_int1_reg [label="PCSX_INTX_REG(1,0)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_pcs0_int1_reg [label="agx0"];
 *     cvmx_pcs0_int2_reg [label="PCSX_INTX_REG(2,0)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_pcs0_int2_reg [label="agx0"];
 *     cvmx_pcs0_int3_reg [label="PCSX_INTX_REG(3,0)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_pcs0_int3_reg [label="agx0"];
 *     cvmx_pcsx0_int_reg [label="PCSXX_INT_REG(0)|<txflt>txflt|<rxbad>rxbad|<rxsynbad>rxsynbad|<bitlckls>bitlckls|<synlos>synlos|<algnlos>algnlos|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx0:e -> cvmx_pcsx0_int_reg [label="agx0"];
 *     cvmx_gmx1_bad_reg [label="GMXX_BAD_REG(1)|<out_ovr>out_ovr|<loststat>loststat|<statovr>statovr|<inb_nxa>inb_nxa"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_bad_reg [label="agx1"];
 *     cvmx_gmx1_rx0_int_reg [label="GMXX_RXX_INT_REG(0,1)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_rx0_int_reg [label="agx1"];
 *     cvmx_gmx1_rx1_int_reg [label="GMXX_RXX_INT_REG(1,1)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_rx1_int_reg [label="agx1"];
 *     cvmx_gmx1_rx2_int_reg [label="GMXX_RXX_INT_REG(2,1)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_rx2_int_reg [label="agx1"];
 *     cvmx_gmx1_rx3_int_reg [label="GMXX_RXX_INT_REG(3,1)|<carext>carext|<skperr>skperr|<ovrerr>ovrerr|<loc_fault>loc_fault|<rem_fault>rem_fault|<bad_seq>bad_seq|<bad_term>bad_term|<unsop>unsop|<uneop>uneop|<undat>undat|<hg2fld>hg2fld|<hg2cc>hg2cc|<wol>wol"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_rx3_int_reg [label="agx1"];
 *     cvmx_gmx1_tx_int_reg [label="GMXX_TX_INT_REG(1)|<pko_nxa>pko_nxa|<undflw>undflw|<ptp_lost>ptp_lost"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_gmx1_tx_int_reg [label="agx1"];
 *     cvmx_pcs1_int0_reg [label="PCSX_INTX_REG(0,1)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_pcs1_int0_reg [label="agx1"];
 *     cvmx_pcs1_int1_reg [label="PCSX_INTX_REG(1,1)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_pcs1_int1_reg [label="agx1"];
 *     cvmx_pcs1_int2_reg [label="PCSX_INTX_REG(2,1)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_pcs1_int2_reg [label="agx1"];
 *     cvmx_pcs1_int3_reg [label="PCSX_INTX_REG(3,1)|<an_err>an_err|<txfifu>txfifu|<txfifo>txfifo|<txbad>txbad|<rxbad>rxbad|<rxlock>rxlock|<an_bad>an_bad|<sync_bad>sync_bad|<dbg_sync>dbg_sync"];
 *     cvmx_ciu_int_sum1:agx1:e -> cvmx_pcs1_int3_reg [label="agx1"];
 *     cvmx_sli_int_sum [label="PEXP_SLI_INT_SUM|<rml_to>rml_to|<bar0_to>bar0_to|<iob2big>iob2big|<m0_up_b0>m0_up_b0|<m0_up_wi>m0_up_wi|<m0_un_b0>m0_un_b0|<m0_un_wi>m0_un_wi|<m1_up_b0>m1_up_b0|<m1_up_wi>m1_up_wi|<m1_un_b0>m1_un_b0|<m1_un_wi>m1_un_wi|<m2_up_b0>m2_up_b0|<m2_up_wi>m2_up_wi|<m2_un_b0>m2_un_b0|<m2_un_wi>m2_un_wi|<m3_up_b0>m3_up_b0|<m3_up_wi>m3_up_wi|<m3_un_b0>m3_un_b0|<m3_un_wi>m3_un_wi|<pidbof>pidbof|<psldbof>psldbof|<pout_err>pout_err|<pin_bp>pin_bp|<pgl_err>pgl_err|<pdi_err>pdi_err|<pop_err>pop_err|<pins_err>pins_err|<sprt0_err>sprt0_err|<sprt1_err>sprt1_err|<sprt2_err>sprt2_err|<sprt3_err>sprt3_err|<ill_pad>ill_pad"];
 *     cvmx_ciu_int_sum1:sli:e -> cvmx_sli_int_sum [label="sli"];
 *     cvmx_dpi_int_reg [label="DPI_INT_REG|<nderr>nderr|<nfovr>nfovr|<dmadbo>dmadbo|<req_badadr>req_badadr|<req_badlen>req_badlen|<req_ovrflw>req_ovrflw|<req_undflw>req_undflw|<req_anull>req_anull|<req_inull>req_inull|<req_badfil>req_badfil|<sprt0_rst>sprt0_rst|<sprt1_rst>sprt1_rst|<sprt2_rst>sprt2_rst|<sprt3_rst>sprt3_rst"];
 *     cvmx_ciu_int_sum1:dpi:e -> cvmx_dpi_int_reg [label="dpi"];
 *     cvmx_dpi_pkt_err_rsp [label="DPI_PKT_ERR_RSP|<pkterr>pkterr"];
 *     cvmx_ciu_int_sum1:dpi:e -> cvmx_dpi_pkt_err_rsp [label="dpi"];
 *     cvmx_dpi_req_err_rsp [label="DPI_REQ_ERR_RSP|<qerr>qerr"];
 *     cvmx_ciu_int_sum1:dpi:e -> cvmx_dpi_req_err_rsp [label="dpi"];
 *     cvmx_dpi_req_err_rst [label="DPI_REQ_ERR_RST|<qerr>qerr"];
 *     cvmx_ciu_int_sum1:dpi:e -> cvmx_dpi_req_err_rst [label="dpi"];
 *     cvmx_agl_gmx_bad_reg -> cvmx_agl_gmx_rx0_int_reg [style=invis];
 *     cvmx_agl_gmx_rx0_int_reg -> cvmx_agl_gmx_tx_int_reg [style=invis];
 *     cvmx_gmx0_bad_reg -> cvmx_gmx0_rx0_int_reg [style=invis];
 *     cvmx_gmx0_rx0_int_reg -> cvmx_gmx0_rx1_int_reg [style=invis];
 *     cvmx_gmx0_rx1_int_reg -> cvmx_gmx0_rx2_int_reg [style=invis];
 *     cvmx_gmx0_rx2_int_reg -> cvmx_gmx0_rx3_int_reg [style=invis];
 *     cvmx_gmx0_rx3_int_reg -> cvmx_gmx0_tx_int_reg [style=invis];
 *     cvmx_gmx0_tx_int_reg -> cvmx_pcs0_int0_reg [style=invis];
 *     cvmx_pcs0_int0_reg -> cvmx_pcs0_int1_reg [style=invis];
 *     cvmx_pcs0_int1_reg -> cvmx_pcs0_int2_reg [style=invis];
 *     cvmx_pcs0_int2_reg -> cvmx_pcs0_int3_reg [style=invis];
 *     cvmx_pcs0_int3_reg -> cvmx_pcsx0_int_reg [style=invis];
 *     cvmx_gmx1_bad_reg -> cvmx_gmx1_rx0_int_reg [style=invis];
 *     cvmx_gmx1_rx0_int_reg -> cvmx_gmx1_rx1_int_reg [style=invis];
 *     cvmx_gmx1_rx1_int_reg -> cvmx_gmx1_rx2_int_reg [style=invis];
 *     cvmx_gmx1_rx2_int_reg -> cvmx_gmx1_rx3_int_reg [style=invis];
 *     cvmx_gmx1_rx3_int_reg -> cvmx_gmx1_tx_int_reg [style=invis];
 *     cvmx_gmx1_tx_int_reg -> cvmx_pcs1_int0_reg [style=invis];
 *     cvmx_pcs1_int0_reg -> cvmx_pcs1_int1_reg [style=invis];
 *     cvmx_pcs1_int1_reg -> cvmx_pcs1_int2_reg [style=invis];
 *     cvmx_pcs1_int2_reg -> cvmx_pcs1_int3_reg [style=invis];
 *     cvmx_dpi_int_reg -> cvmx_dpi_pkt_err_rsp [style=invis];
 *     cvmx_dpi_pkt_err_rsp -> cvmx_dpi_req_err_rsp [style=invis];
 *     cvmx_dpi_req_err_rsp -> cvmx_dpi_req_err_rst [style=invis];
 *     cvmx_root:root:e -> cvmx_ciu_int_sum1 [label="root"];
 * }
 * @enddot
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-error.h>
#include <asm/octeon/cvmx-error-custom.h>
#include <asm/octeon/cvmx-csr-typedefs.h>
#else
#include "cvmx.h"
#include "cvmx-error.h"
#include "cvmx-error-custom.h"
#endif

int cvmx_error_initialize_cn70xx(void)
{
    cvmx_error_info_t info;
    int fail = 0;

    /* CVMX_CIU_INTX_SUM0(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_INTX_SUM0(0);
    info.status_mask        = 0;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = __CVMX_ERROR_REGISTER_NONE;
    info.parent.status_addr = 0;
    info.parent.status_mask = 0;
    info.func               = __cvmx_error_decode;
    info.user_info          = 0;
    fail |= cvmx_error_add(&info);

    /* CVMX_POW_IQ_INT */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_POW_IQ_INT;
    info.status_mask        = 0xffull<<0 /* iq_int */;
    info.enable_addr        = CVMX_POW_IQ_INT_EN;
    info.enable_mask        = 0xffull<<0 /* int_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INTX_SUM0(0);
    info.parent.status_mask = 1ull<<60 /* powiq */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR POW_IQ_INT[IQ_INT]: Input queue interrupt bits\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_CIU_INT_SUM1 */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_INT_SUM1;
    info.status_mask        = 0;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = __CVMX_ERROR_REGISTER_NONE;
    info.parent.status_addr = 0;
    info.parent.status_mask = 0;
    info.func               = __cvmx_error_decode;
    info.user_info          = 0;
    fail |= cvmx_error_add(&info);

    /* CVMX_CIU_CIB_L2C_RAWX(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<0 /* tadx_int_l2dsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<0 /* tadx_int_l2dsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_L2DSBE]: Set when L2C_TAD0_INT[L2DSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_L2DSBE] before clearing L2C_TAD0_INT[L2DSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<1 /* tadx_int_l2ddbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<1 /* tadx_int_l2ddbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_L2DDBE]: Set when L2C_TAD0_INT[L2DDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_L2DDBE] before clearing L2C_TAD0_INT[L2DDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<2 /* tadx_int_sbfsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<2 /* tadx_int_sbfsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_SBFSBE]: Set when L2C_TAD0_INT[SBFSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_SBFSBE] before clearing L2C_TAD0_INT[SBFSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<3 /* tadx_int_sbfdbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<3 /* tadx_int_sbfdbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_SBFDBE]: Set when L2C_TAD0_INT[SBFDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_SBFDBE] before clearing L2C_TAD0_INT[SBFDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<4 /* tadx_int_fbfsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<4 /* tadx_int_fbfsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_FBFSBE]: Set when L2C_TAD0_INT[FBFSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_FBFSBE] before clearing L2C_TAD0_INT[FBFSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<5 /* tadx_int_fbfdbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<5 /* tadx_int_fbfdbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_FBFDBE]: Set when L2C_TAD0_INT[FBFDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_FBFDBE] before clearing L2C_TAD0_INT[FBFDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<6 /* tadx_int_tagsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<6 /* tadx_int_tagsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_TAGSBE]: Set when L2C_TAD0_INT[TAGSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_TAGSBE] before clearing L2C_TAD0_INT[TAGSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<7 /* tadx_int_tagdbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<7 /* tadx_int_tagdbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_TAGDBE]: Set when L2C_TAD0_INT[TAGDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_TAGDBE] before clearing L2C_TAD0_INT[TAGDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<8 /* tadx_int_noway */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<8 /* tadx_int_noway */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_NOWAY]: Set when L2C_TAD0_INT[NOWAY] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_NOWAY] before clearing L2C_TAD0_INT[NOWAY].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<9 /* tadx_int_holewr */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<9 /* tadx_int_holewr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_HOLEWR]: Set when L2C_TAD0_INT[HOLEWR] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_HOLEWR] before clearing L2C_TAD0_INT[HOLEWR].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<10 /* tadx_int_holerd */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<10 /* tadx_int_holerd */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_HOLERD]: Set when L2C_TAD0_INT[HOLERD] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_HOLERD] before clearing L2C_TAD0_INT[HOLERD].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<11 /* tadx_int_bigwr */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<11 /* tadx_int_bigwr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_BIGWR]: Set when L2C_TAD0_INT[BIGWR] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_BIGWR] before clearing L2C_TAD0_INT[BIGWR].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<12 /* tadx_int_bigrd */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<12 /* tadx_int_bigrd */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_BIGRD]: Set when L2C_TAD0_INT[BIGRD] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_BIGRD] before clearing L2C_TAD0_INT[BIGRD].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<13 /* tadx_int_wrdislmc */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<13 /* tadx_int_wrdislmc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_WRDISLMC]: Set when L2C_TAD0_INT[WRDISLMC] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_WRDISLMC] before clearing L2C_TAD0_INT[WRDISLMC].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<14 /* tadx_int_rddislmc */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<14 /* tadx_int_rddislmc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_RDDISLMC]: Set when L2C_TAD0_INT[RDDISLMC] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_RDDISLMC] before clearing L2C_TAD0_INT[RDDISLMC].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<15 /* tadx_int_rtgsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<15 /* tadx_int_rtgsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_RTGSBE]: Set when L2C_TAD0_INT[RTGSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_RTGSBE] before clearing L2C_TAD0_INT[RTGSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<16 /* tadx_int_rtgdbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<16 /* tadx_int_rtgdbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[TADX_INT_RTGDBE]: Set when L2C_TAD0_INT[RTGDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [TADX_INT_RTGDBE] before clearing L2C_TAD0_INT[RTGDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<17 /* mcix_int_vbfsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<17 /* mcix_int_vbfsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[MCIX_INT_VBFSBE]: Set when L2C_MCI0_INT[VBFSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [MCIX_INT_VBFSBE] before clearing L2C_MCI0_INT[VBFSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<18 /* mcix_int_vbfdbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<18 /* mcix_int_vbfdbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[MCIX_INT_VBFDBE]: Set when L2C_MCI0_INT[VBFDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [MCIX_INT_VBFDBE] before clearing L2C_MCI0_INT[VBFDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<19 /* cbcx_int_rsdsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<19 /* cbcx_int_rsdsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[CBCX_INT_RSDSBE]: Set when L2C_CBC0_INT[RSDSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [CBCX_INT_RSDSBE] before clearing L2C_CBC0_INT[RSDSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<20 /* cbcx_int_rsddbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<20 /* cbcx_int_rsddbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[CBCX_INT_RSDDBE]: Set when L2C_CBC0_INT[RSDDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [CBCX_INT_RSDDBE] before clearing L2C_CBC0_INT[RSDDBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<21 /* cbcx_int_ioccmdsbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<21 /* cbcx_int_ioccmdsbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[CBCX_INT_IOCCMDSBE]: Set when L2C_CBC0_INT[IOCCMDSBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [CBCX_INT_IOCCMDSBE] before clearing L2C_CBC0_INT[IOCCMDSBE].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_L2C_RAWX(0);
    info.status_mask        = 1ull<<22 /* cbcx_int_ioccmddbe */;
    info.enable_addr        = CVMX_CIU_CIB_L2C_ENX(0);
    info.enable_mask        = 1ull<<22 /* cbcx_int_ioccmddbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_L2C;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<24 /* l2c */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_L2C_RAWX(0)[CBCX_INT_IOCCMDDBE]: Set when L2C_CBC0_INT[IOCCMDDBE] set. Edge-sensitive interrupt, so software should clear\n"
        "    [CBCX_INT_IOCCMDDBE] before clearing L2C_CBC0_INT[IOCCMDDBE].\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_IPD_INT_SUM */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<0 /* prc_par0 */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<0 /* prc_par0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[PRC_PAR0]: Set when a parity error is dected for bits\n"
        "    [31:0] of the PBM memory.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<1 /* prc_par1 */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<1 /* prc_par1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[PRC_PAR1]: Set when a parity error is dected for bits\n"
        "    [63:32] of the PBM memory.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<2 /* prc_par2 */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<2 /* prc_par2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[PRC_PAR2]: Set when a parity error is dected for bits\n"
        "    [95:64] of the PBM memory.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<3 /* prc_par3 */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<3 /* prc_par3 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[PRC_PAR3]: Set when a parity error is dected for bits\n"
        "    [127:96] of the PBM memory.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<4 /* bp_sub */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<4 /* bp_sub */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[BP_SUB]: Set when a backpressure subtract is done with a\n"
        "    supplied illegal value.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<5 /* dc_ovr */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<5 /* dc_ovr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[DC_OVR]: Set when the data credits to the IOB overflow.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<6 /* cc_ovr */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<6 /* cc_ovr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[CC_OVR]: Set when the command credits to the IOB overflow.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<7 /* c_coll */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<7 /* c_coll */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[C_COLL]: Set when the packet/WQE commands to be sent to IOB\n"
        "    collides.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<8 /* d_coll */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<8 /* d_coll */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[D_COLL]: Set when the packet/WQE data to be sent to IOB\n"
        "    collides.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IPD_INT_SUM;
    info.status_mask        = 1ull<<9 /* bc_ovr */;
    info.enable_addr        = CVMX_IPD_INT_ENB;
    info.enable_mask        = 1ull<<9 /* bc_ovr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<25 /* ipd */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IPD_INT_SUM[BC_OVR]: Set when the byte-count to send to IOB overflows.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_POW_ECC_ERR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_POW_ECC_ERR;
    info.status_mask        = 1ull<<0 /* sbe */;
    info.enable_addr        = CVMX_POW_ECC_ERR;
    info.enable_mask        = 1ull<<2 /* sbe_ie */;
    info.flags              = CVMX_ERROR_FLAGS_ECC_SINGLE_BIT;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<23 /* pow */;
    info.func               = __cvmx_error_handle_pow_ecc_err_sbe;
    info.user_info          = (long)
        "ERROR POW_ECC_ERR[SBE]: Single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_POW_ECC_ERR;
    info.status_mask        = 1ull<<1 /* dbe */;
    info.enable_addr        = CVMX_POW_ECC_ERR;
    info.enable_mask        = 1ull<<3 /* dbe_ie */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<23 /* pow */;
    info.func               = __cvmx_error_handle_pow_ecc_err_dbe;
    info.user_info          = (long)
        "ERROR POW_ECC_ERR[DBE]: Double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_POW_ECC_ERR;
    info.status_mask        = 1ull<<12 /* rpe */;
    info.enable_addr        = CVMX_POW_ECC_ERR;
    info.enable_mask        = 1ull<<13 /* rpe_ie */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<23 /* pow */;
    info.func               = __cvmx_error_handle_pow_ecc_err_rpe;
    info.user_info          = (long)
        "ERROR POW_ECC_ERR[RPE]: Remote pointer error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_POW_ECC_ERR;
    info.status_mask        = 0x1fffull<<16 /* iop */;
    info.enable_addr        = CVMX_POW_ECC_ERR;
    info.enable_mask        = 0x1fffull<<32 /* iop_ie */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<23 /* pow */;
    info.func               = __cvmx_error_handle_pow_ecc_err_iop;
    info.user_info          = (long)
        "ERROR POW_ECC_ERR[IOP]: Illegal operation errors\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_RAD_REG_ERROR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_RAD_REG_ERROR;
    info.status_mask        = 1ull<<0 /* doorbell */;
    info.enable_addr        = CVMX_RAD_REG_INT_MASK;
    info.enable_mask        = 1ull<<0 /* doorbell */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<30 /* rad */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR RAD_REG_ERROR[DOORBELL]: A doorbell count has overflowed\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PIP_INT_REG */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<3 /* prtnxa */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<3 /* prtnxa */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[PRTNXA]: Non-existent port\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<4 /* badtag */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<4 /* badtag */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[BADTAG]: A bad tag was sent from IPD\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<5 /* skprunt */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<5 /* skprunt */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[SKPRUNT]: Packet was engulfed by skipper\n"
        "    This interrupt can occur with received PARTIAL\n"
        "    packets that are truncated to SKIP bytes or\n"
        "    smaller.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<6 /* todoovr */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<6 /* todoovr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[TODOOVR]: Todo list overflow (see PIP_BCK_PRS[HIWATER])\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<7 /* feperr */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<7 /* feperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[FEPERR]: Parity Error in front end memory\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<8 /* beperr */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<8 /* beperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[BEPERR]: Parity Error in back end memory\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PIP_INT_REG;
    info.status_mask        = 1ull<<12 /* punyerr */;
    info.enable_addr        = CVMX_PIP_INT_EN;
    info.enable_mask        = 1ull<<12 /* punyerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<26 /* pip */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PIP_INT_REG[PUNYERR]: Frame was received with length <=4B when CRC\n"
        "    stripping in IPD is enable\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PKO_REG_ERROR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PKO_REG_ERROR;
    info.status_mask        = 1ull<<0 /* parity */;
    info.enable_addr        = CVMX_PKO_REG_INT_MASK;
    info.enable_mask        = 1ull<<0 /* parity */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<27 /* pko */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PKO_REG_ERROR[PARITY]: Read parity error at port data buffer\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PKO_REG_ERROR;
    info.status_mask        = 1ull<<1 /* doorbell */;
    info.enable_addr        = CVMX_PKO_REG_INT_MASK;
    info.enable_mask        = 1ull<<1 /* doorbell */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<27 /* pko */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PKO_REG_ERROR[DOORBELL]: A doorbell count has overflowed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PKO_REG_ERROR;
    info.status_mask        = 1ull<<2 /* currzero */;
    info.enable_addr        = CVMX_PKO_REG_INT_MASK;
    info.enable_mask        = 1ull<<2 /* currzero */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<27 /* pko */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PKO_REG_ERROR[CURRZERO]: A packet data pointer has size=0\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_INT_SUM(2) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<1 /* se */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<1 /* se */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[SE]: System Error, RC DEode Only.\n"
        "    (cfg_sys_err_rc)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<4 /* up_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<4 /* up_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UP_B1]: Received P-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<5 /* up_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<5 /* up_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UP_B2]: Received P-TLP for Bar2 when bar2 is disabeld.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<6 /* up_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<6 /* up_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UP_BX]: Received P-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<7 /* un_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<7 /* un_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UN_B1]: Received N-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<8 /* un_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<8 /* un_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UN_B2]: Received N-TLP for Bar2 when bar2 is disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<9 /* un_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<9 /* un_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[UN_BX]: Received N-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<11 /* rdlk */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<11 /* rdlk */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[RDLK]: Received Read Lock TLP.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<12 /* crs_er */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<12 /* crs_er */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[CRS_ER]: Had a CRS Timeout when Retries were enabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 1ull<<13 /* crs_dr */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(2);
    info.enable_mask        = 1ull<<13 /* crs_dr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(2)[CRS_DR]: Had a CRS Timeout when Retries were disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(2);
    info.status_mask        = 0;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<50 /* pem2 */;
    info.func               = __cvmx_error_decode;
    info.user_info          = 0;
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_DBG_INFO(2) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<0 /* spoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<0 /* spoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[SPOISON]: Poisoned TLP sent\n"
        "    peai__client0_tlp_ep & peai__client0_tlp_hv\n"
        "    peai__client1_tlp_ep & peai__client1_tlp_hv (atomic_op).\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<2 /* rtlplle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<2 /* rtlplle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RTLPLLE]: Received TLP has link layer error\n"
        "    pedc_radm_trgt1_dllp_abort & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<3 /* recrce */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<3 /* recrce */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RECRCE]: Received ECRC Error\n"
        "    pedc_radm_trgt1_ecrc_err & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<4 /* rpoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<4 /* rpoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RPOISON]: Received Poisoned TLP\n"
        "    pedc__radm_trgt1_poisoned & pedc__radm_trgt1_hv\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<5 /* rcemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<5 /* rcemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RCEMRC]: Received Correctable Error Message (RC Mode only)\n"
        "    pedc_radm_correctable_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<6 /* rnfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<6 /* rnfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RNFEMRC]: Received Non-Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_nonfatal_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<7 /* rfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<7 /* rfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RFEMRC]: Received Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_fatal_err\n"
        "    Bit set when a message with ERR_FATAL is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<8 /* rpmerc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<8 /* rpmerc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RPMERC]: Received PME Message (RC Mode only)\n"
        "    pedc_radm_pm_pme\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<9 /* rptamrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<9 /* rptamrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RPTAMRC]: Received PME Turnoff Acknowledge Message\n"
        "    (RC Mode only)\n"
        "    pedc_radm_pm_to_ack\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<10 /* rumep */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<10 /* rumep */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RUMEP]: Received Unlock Message (EP Mode Only)\n"
        "    pedc_radm_msg_unlock\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<11 /* rvdm */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<11 /* rvdm */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RVDM]: Received Vendor-Defined Message\n"
        "    pedc_radm_vendor_msg\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<12 /* acto */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<12 /* acto */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[ACTO]: A Completion Timeout Occured\n"
        "    pedc_radm_cpl_timeout\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<13 /* rte */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<13 /* rte */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RTE]: Replay Timer Expired\n"
        "    xdlh_replay_timeout_err\n"
        "    This bit is set when the REPLAY_TIMER expires in\n"
        "    the PCIE core. The probability of this bit being\n"
        "    set will increase with the traffic load.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<14 /* mre */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<14 /* mre */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[MRE]: Max Retries Exceeded\n"
        "    xdlh_replay_num_rlover_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<15 /* rdwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<15 /* rdwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RDWDLE]: Received DLLP with DataLink Layer Error\n"
        "    rdlh_bad_dllp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<16 /* rtwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<16 /* rtwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RTWDLE]: Received TLP with DataLink Layer Error\n"
        "    rdlh_bad_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<17 /* dpeoosd */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<17 /* dpeoosd */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[DPEOOSD]: DLLP protocol error (out of sequence DLLP)\n"
        "    rdlh_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<18 /* fcpvwt */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<18 /* fcpvwt */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[FCPVWT]: Flow Control Protocol Violation (Watchdog Timer)\n"
        "    rtlh_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<19 /* rpe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<19 /* rpe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RPE]: When the PHY reports 8B/10B decode error\n"
        "    (RxStatus = 3b100) or disparity error\n"
        "    (RxStatus = 3b111), the signal rmlh_rcvd_err will\n"
        "    be asserted.\n"
        "    rmlh_rcvd_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<20 /* fcuv */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<20 /* fcuv */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[FCUV]: Flow Control Update Violation (opt. checks)\n"
        "    int_xadm_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<21 /* rqo */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<21 /* rqo */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RQO]: Receive queue overflow. Normally happens only when\n"
        "    flow control advertisements are ignored\n"
        "    radm_qoverflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<22 /* rauc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<22 /* rauc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RAUC]: Received an unexpected completion\n"
        "    radm_unexp_cpl_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<23 /* racur */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<23 /* racur */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RACUR]: Received a completion with UR status\n"
        "    radm_rcvd_cpl_ur\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<24 /* racca */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<24 /* racca */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RACCA]: Received a completion with CA status\n"
        "    radm_rcvd_cpl_ca\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<25 /* caar */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<25 /* caar */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[CAAR]: Completer aborted a request\n"
        "    radm_rcvd_ca_req\n"
        "    This bit will never be set because Octeon does\n"
        "    not generate Completer Aborts.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<26 /* rarwdns */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<26 /* rarwdns */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RARWDNS]: Recieved a request which device does not support\n"
        "    radm_rcvd_ur_req\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<27 /* ramtlp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<27 /* ramtlp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RAMTLP]: Received a malformed TLP\n"
        "    radm_mlf_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<28 /* racpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<28 /* racpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RACPP]: Received a completion with poisoned payload\n"
        "    radm_rcvd_cpl_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<29 /* rawwpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<29 /* rawwpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RAWWPP]: Received a write with poisoned payload\n"
        "    radm_rcvd_wreq_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<30 /* ecrc_e */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<30 /* ecrc_e */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[ECRC_E]: Received a ECRC error.\n"
        "    radm_ecrc_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<31 /* rtry_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<31 /* rtry_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[RTRY_PE]: Detected a Retry RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<32 /* hdrq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<32 /* hdrq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[HDRQ_PE]: Detected a Header Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<33 /* datq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<33 /* datq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[DATQ_PE]: Detected a Data Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<34 /* p_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<34 /* tpfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[P_D_SBE]: Detected a TLP Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<35 /* p_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<35 /* tpfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[P_D_DBE]: Detected a TLP Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<36 /* p_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<36 /* tpfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[P_C_SBE]: Detected a TLP Posted Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<37 /* p_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<37 /* tpfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[P_C_DBE]: Detected a TLP Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<38 /* n_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<38 /* tnfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[N_D_SBE]: Detected a TLP Non-Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<39 /* n_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<39 /* tnfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[N_D_DBE]: Detected a TLP Non-Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<40 /* n_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<40 /* tnfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[N_C_SBE]: Detected a TLP Non-Posted fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<41 /* n_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<41 /* tnfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[N_C_DBE]: Detected a TLP Non-Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<42 /* c_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<42 /* tpcsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[C_D_SBE]: Detected a TLP CPL Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<43 /* c_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<43 /* tpcdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[C_D_DBE]: Detected a TLP CPL Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<44 /* c_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<44 /* tpcsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[C_C_SBE]: Detected a TLP CPL Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(2);
    info.status_mask        = 1ull<<45 /* c_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(2);
    info.enable_mask        = 1ull<<45 /* tpcdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(2);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(2)[C_C_DBE]: Detected a TLP CPL Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_INT_SUM(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<1 /* se */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<1 /* se */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[SE]: System Error, RC DEode Only.\n"
        "    (cfg_sys_err_rc)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<4 /* up_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<4 /* up_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UP_B1]: Received P-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<5 /* up_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<5 /* up_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UP_B2]: Received P-TLP for Bar2 when bar2 is disabeld.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<6 /* up_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<6 /* up_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UP_BX]: Received P-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<7 /* un_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<7 /* un_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UN_B1]: Received N-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<8 /* un_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<8 /* un_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UN_B2]: Received N-TLP for Bar2 when bar2 is disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<9 /* un_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<9 /* un_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[UN_BX]: Received N-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<11 /* rdlk */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<11 /* rdlk */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[RDLK]: Received Read Lock TLP.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<12 /* crs_er */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<12 /* crs_er */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[CRS_ER]: Had a CRS Timeout when Retries were enabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 1ull<<13 /* crs_dr */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(0);
    info.enable_mask        = 1ull<<13 /* crs_dr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(0)[CRS_DR]: Had a CRS Timeout when Retries were disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(0);
    info.status_mask        = 0;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<48 /* pem0 */;
    info.func               = __cvmx_error_decode;
    info.user_info          = 0;
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_DBG_INFO(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<0 /* spoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<0 /* spoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[SPOISON]: Poisoned TLP sent\n"
        "    peai__client0_tlp_ep & peai__client0_tlp_hv\n"
        "    peai__client1_tlp_ep & peai__client1_tlp_hv (atomic_op).\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<2 /* rtlplle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<2 /* rtlplle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RTLPLLE]: Received TLP has link layer error\n"
        "    pedc_radm_trgt1_dllp_abort & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<3 /* recrce */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<3 /* recrce */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RECRCE]: Received ECRC Error\n"
        "    pedc_radm_trgt1_ecrc_err & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<4 /* rpoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<4 /* rpoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RPOISON]: Received Poisoned TLP\n"
        "    pedc__radm_trgt1_poisoned & pedc__radm_trgt1_hv\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<5 /* rcemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<5 /* rcemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RCEMRC]: Received Correctable Error Message (RC Mode only)\n"
        "    pedc_radm_correctable_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<6 /* rnfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<6 /* rnfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RNFEMRC]: Received Non-Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_nonfatal_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<7 /* rfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<7 /* rfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RFEMRC]: Received Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_fatal_err\n"
        "    Bit set when a message with ERR_FATAL is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<8 /* rpmerc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<8 /* rpmerc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RPMERC]: Received PME Message (RC Mode only)\n"
        "    pedc_radm_pm_pme\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<9 /* rptamrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<9 /* rptamrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RPTAMRC]: Received PME Turnoff Acknowledge Message\n"
        "    (RC Mode only)\n"
        "    pedc_radm_pm_to_ack\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<10 /* rumep */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<10 /* rumep */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RUMEP]: Received Unlock Message (EP Mode Only)\n"
        "    pedc_radm_msg_unlock\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<11 /* rvdm */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<11 /* rvdm */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RVDM]: Received Vendor-Defined Message\n"
        "    pedc_radm_vendor_msg\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<12 /* acto */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<12 /* acto */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[ACTO]: A Completion Timeout Occured\n"
        "    pedc_radm_cpl_timeout\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<13 /* rte */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<13 /* rte */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RTE]: Replay Timer Expired\n"
        "    xdlh_replay_timeout_err\n"
        "    This bit is set when the REPLAY_TIMER expires in\n"
        "    the PCIE core. The probability of this bit being\n"
        "    set will increase with the traffic load.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<14 /* mre */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<14 /* mre */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[MRE]: Max Retries Exceeded\n"
        "    xdlh_replay_num_rlover_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<15 /* rdwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<15 /* rdwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RDWDLE]: Received DLLP with DataLink Layer Error\n"
        "    rdlh_bad_dllp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<16 /* rtwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<16 /* rtwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RTWDLE]: Received TLP with DataLink Layer Error\n"
        "    rdlh_bad_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<17 /* dpeoosd */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<17 /* dpeoosd */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[DPEOOSD]: DLLP protocol error (out of sequence DLLP)\n"
        "    rdlh_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<18 /* fcpvwt */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<18 /* fcpvwt */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[FCPVWT]: Flow Control Protocol Violation (Watchdog Timer)\n"
        "    rtlh_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<19 /* rpe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<19 /* rpe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RPE]: When the PHY reports 8B/10B decode error\n"
        "    (RxStatus = 3b100) or disparity error\n"
        "    (RxStatus = 3b111), the signal rmlh_rcvd_err will\n"
        "    be asserted.\n"
        "    rmlh_rcvd_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<20 /* fcuv */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<20 /* fcuv */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[FCUV]: Flow Control Update Violation (opt. checks)\n"
        "    int_xadm_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<21 /* rqo */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<21 /* rqo */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RQO]: Receive queue overflow. Normally happens only when\n"
        "    flow control advertisements are ignored\n"
        "    radm_qoverflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<22 /* rauc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<22 /* rauc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RAUC]: Received an unexpected completion\n"
        "    radm_unexp_cpl_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<23 /* racur */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<23 /* racur */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RACUR]: Received a completion with UR status\n"
        "    radm_rcvd_cpl_ur\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<24 /* racca */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<24 /* racca */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RACCA]: Received a completion with CA status\n"
        "    radm_rcvd_cpl_ca\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<25 /* caar */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<25 /* caar */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[CAAR]: Completer aborted a request\n"
        "    radm_rcvd_ca_req\n"
        "    This bit will never be set because Octeon does\n"
        "    not generate Completer Aborts.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<26 /* rarwdns */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<26 /* rarwdns */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RARWDNS]: Recieved a request which device does not support\n"
        "    radm_rcvd_ur_req\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<27 /* ramtlp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<27 /* ramtlp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RAMTLP]: Received a malformed TLP\n"
        "    radm_mlf_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<28 /* racpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<28 /* racpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RACPP]: Received a completion with poisoned payload\n"
        "    radm_rcvd_cpl_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<29 /* rawwpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<29 /* rawwpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RAWWPP]: Received a write with poisoned payload\n"
        "    radm_rcvd_wreq_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<30 /* ecrc_e */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<30 /* ecrc_e */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[ECRC_E]: Received a ECRC error.\n"
        "    radm_ecrc_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<31 /* rtry_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<31 /* rtry_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[RTRY_PE]: Detected a Retry RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<32 /* hdrq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<32 /* hdrq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[HDRQ_PE]: Detected a Header Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<33 /* datq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<33 /* datq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[DATQ_PE]: Detected a Data Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<34 /* p_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<34 /* tpfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[P_D_SBE]: Detected a TLP Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<35 /* p_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<35 /* tpfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[P_D_DBE]: Detected a TLP Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<36 /* p_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<36 /* tpfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[P_C_SBE]: Detected a TLP Posted Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<37 /* p_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<37 /* tpfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[P_C_DBE]: Detected a TLP Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<38 /* n_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<38 /* tnfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[N_D_SBE]: Detected a TLP Non-Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<39 /* n_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<39 /* tnfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[N_D_DBE]: Detected a TLP Non-Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<40 /* n_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<40 /* tnfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[N_C_SBE]: Detected a TLP Non-Posted fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<41 /* n_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<41 /* tnfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[N_C_DBE]: Detected a TLP Non-Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<42 /* c_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<42 /* tpcsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[C_D_SBE]: Detected a TLP CPL Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<43 /* c_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<43 /* tpcdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[C_D_DBE]: Detected a TLP CPL Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<44 /* c_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<44 /* tpcsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[C_C_SBE]: Detected a TLP CPL Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(0);
    info.status_mask        = 1ull<<45 /* c_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(0);
    info.enable_mask        = 1ull<<45 /* tpcdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(0);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(0)[C_C_DBE]: Detected a TLP CPL Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_INT_SUM(1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<1 /* se */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<1 /* se */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[SE]: System Error, RC DEode Only.\n"
        "    (cfg_sys_err_rc)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<4 /* up_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<4 /* up_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UP_B1]: Received P-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<5 /* up_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<5 /* up_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UP_B2]: Received P-TLP for Bar2 when bar2 is disabeld.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<6 /* up_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<6 /* up_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UP_BX]: Received P-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<7 /* un_b1 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<7 /* un_b1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UN_B1]: Received N-TLP for Bar1 when bar1 index valid\n"
        "    is not set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<8 /* un_b2 */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<8 /* un_b2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UN_B2]: Received N-TLP for Bar2 when bar2 is disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<9 /* un_bx */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<9 /* un_bx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[UN_BX]: Received N-TLP for an unknown Bar.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<11 /* rdlk */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<11 /* rdlk */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[RDLK]: Received Read Lock TLP.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<12 /* crs_er */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<12 /* crs_er */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[CRS_ER]: Had a CRS Timeout when Retries were enabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 1ull<<13 /* crs_dr */;
    info.enable_addr        = CVMX_PEMX_INT_ENB(1);
    info.enable_mask        = 1ull<<13 /* crs_dr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_INT_SUM(1)[CRS_DR]: Had a CRS Timeout when Retries were disabled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_INT_SUM(1);
    info.status_mask        = 0;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<49 /* pem1 */;
    info.func               = __cvmx_error_decode;
    info.user_info          = 0;
    fail |= cvmx_error_add(&info);

    /* CVMX_PEMX_DBG_INFO(1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<0 /* spoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<0 /* spoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[SPOISON]: Poisoned TLP sent\n"
        "    peai__client0_tlp_ep & peai__client0_tlp_hv\n"
        "    peai__client1_tlp_ep & peai__client1_tlp_hv (atomic_op).\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<2 /* rtlplle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<2 /* rtlplle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RTLPLLE]: Received TLP has link layer error\n"
        "    pedc_radm_trgt1_dllp_abort & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<3 /* recrce */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<3 /* recrce */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RECRCE]: Received ECRC Error\n"
        "    pedc_radm_trgt1_ecrc_err & pedc__radm_trgt1_eot\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<4 /* rpoison */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<4 /* rpoison */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RPOISON]: Received Poisoned TLP\n"
        "    pedc__radm_trgt1_poisoned & pedc__radm_trgt1_hv\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<5 /* rcemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<5 /* rcemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RCEMRC]: Received Correctable Error Message (RC Mode only)\n"
        "    pedc_radm_correctable_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<6 /* rnfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<6 /* rnfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RNFEMRC]: Received Non-Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_nonfatal_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<7 /* rfemrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<7 /* rfemrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RFEMRC]: Received Fatal Error Message (RC Mode only)\n"
        "    pedc_radm_fatal_err\n"
        "    Bit set when a message with ERR_FATAL is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<8 /* rpmerc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<8 /* rpmerc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RPMERC]: Received PME Message (RC Mode only)\n"
        "    pedc_radm_pm_pme\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<9 /* rptamrc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<9 /* rptamrc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RPTAMRC]: Received PME Turnoff Acknowledge Message\n"
        "    (RC Mode only)\n"
        "    pedc_radm_pm_to_ack\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<10 /* rumep */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<10 /* rumep */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RUMEP]: Received Unlock Message (EP Mode Only)\n"
        "    pedc_radm_msg_unlock\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<11 /* rvdm */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<11 /* rvdm */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RVDM]: Received Vendor-Defined Message\n"
        "    pedc_radm_vendor_msg\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<12 /* acto */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<12 /* acto */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[ACTO]: A Completion Timeout Occured\n"
        "    pedc_radm_cpl_timeout\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<13 /* rte */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<13 /* rte */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RTE]: Replay Timer Expired\n"
        "    xdlh_replay_timeout_err\n"
        "    This bit is set when the REPLAY_TIMER expires in\n"
        "    the PCIE core. The probability of this bit being\n"
        "    set will increase with the traffic load.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<14 /* mre */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<14 /* mre */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[MRE]: Max Retries Exceeded\n"
        "    xdlh_replay_num_rlover_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<15 /* rdwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<15 /* rdwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RDWDLE]: Received DLLP with DataLink Layer Error\n"
        "    rdlh_bad_dllp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<16 /* rtwdle */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<16 /* rtwdle */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RTWDLE]: Received TLP with DataLink Layer Error\n"
        "    rdlh_bad_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<17 /* dpeoosd */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<17 /* dpeoosd */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[DPEOOSD]: DLLP protocol error (out of sequence DLLP)\n"
        "    rdlh_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<18 /* fcpvwt */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<18 /* fcpvwt */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[FCPVWT]: Flow Control Protocol Violation (Watchdog Timer)\n"
        "    rtlh_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<19 /* rpe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<19 /* rpe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RPE]: When the PHY reports 8B/10B decode error\n"
        "    (RxStatus = 3b100) or disparity error\n"
        "    (RxStatus = 3b111), the signal rmlh_rcvd_err will\n"
        "    be asserted.\n"
        "    rmlh_rcvd_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<20 /* fcuv */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<20 /* fcuv */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[FCUV]: Flow Control Update Violation (opt. checks)\n"
        "    int_xadm_fc_prot_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<21 /* rqo */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<21 /* rqo */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RQO]: Receive queue overflow. Normally happens only when\n"
        "    flow control advertisements are ignored\n"
        "    radm_qoverflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<22 /* rauc */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<22 /* rauc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RAUC]: Received an unexpected completion\n"
        "    radm_unexp_cpl_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<23 /* racur */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<23 /* racur */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RACUR]: Received a completion with UR status\n"
        "    radm_rcvd_cpl_ur\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<24 /* racca */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<24 /* racca */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RACCA]: Received a completion with CA status\n"
        "    radm_rcvd_cpl_ca\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<25 /* caar */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<25 /* caar */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[CAAR]: Completer aborted a request\n"
        "    radm_rcvd_ca_req\n"
        "    This bit will never be set because Octeon does\n"
        "    not generate Completer Aborts.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<26 /* rarwdns */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<26 /* rarwdns */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RARWDNS]: Recieved a request which device does not support\n"
        "    radm_rcvd_ur_req\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<27 /* ramtlp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<27 /* ramtlp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RAMTLP]: Received a malformed TLP\n"
        "    radm_mlf_tlp_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<28 /* racpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<28 /* racpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RACPP]: Received a completion with poisoned payload\n"
        "    radm_rcvd_cpl_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<29 /* rawwpp */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<29 /* rawwpp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RAWWPP]: Received a write with poisoned payload\n"
        "    radm_rcvd_wreq_poisoned\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<30 /* ecrc_e */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<30 /* ecrc_e */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[ECRC_E]: Received a ECRC error.\n"
        "    radm_ecrc_err\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<31 /* rtry_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<31 /* rtry_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[RTRY_PE]: Detected a Retry RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<32 /* hdrq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<32 /* hdrq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[HDRQ_PE]: Detected a Header Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<33 /* datq_pe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<33 /* datq_pe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[DATQ_PE]: Detected a Data Queue RAM parity error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<34 /* p_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<34 /* tpfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[P_D_SBE]: Detected a TLP Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<35 /* p_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<35 /* tpfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[P_D_DBE]: Detected a TLP Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<36 /* p_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<36 /* tpfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[P_C_SBE]: Detected a TLP Posted Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<37 /* p_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<37 /* tpfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[P_C_DBE]: Detected a TLP Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<38 /* n_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<38 /* tnfsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[N_D_SBE]: Detected a TLP Non-Posted Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<39 /* n_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<39 /* tnfdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[N_D_DBE]: Detected a TLP Non-Posted Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<40 /* n_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<40 /* tnfsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[N_C_SBE]: Detected a TLP Non-Posted fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<41 /* n_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<41 /* tnfdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[N_C_DBE]: Detected a TLP Non-Posted Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<42 /* c_d_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<42 /* tpcsbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[C_D_SBE]: Detected a TLP CPL Fifo data single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<43 /* c_d_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<43 /* tpcdbe0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[C_D_DBE]: Detected a TLP CPL Fifo data double bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<44 /* c_c_sbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<44 /* tpcsbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[C_C_SBE]: Detected a TLP CPL Fifo ctrl single bit error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEMX_DBG_INFO(1);
    info.status_mask        = 1ull<<45 /* c_c_dbe */;
    info.enable_addr        = CVMX_PEMX_DBG_INFO_EN(1);
    info.enable_mask        = 1ull<<45 /* tpcdbe1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_PCI;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_PEMX_INT_SUM(1);
    info.parent.status_mask = 1ull<<10 /* exc */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEMX_DBG_INFO(1)[C_C_DBE]: Detected a TLP CPL Fifo ctrl double bit error\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_FPA_INT_SUM */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<0 /* fed0_sbe */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<0 /* fed0_sbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FED0_SBE]: Set when a Single Bit Error is detected in FPF0.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<1 /* fed0_dbe */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<1 /* fed0_dbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FED0_DBE]: Set when a Double Bit Error is detected in FPF0.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<2 /* fed1_sbe */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<2 /* fed1_sbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FED1_SBE]: Set when a Single Bit Error is detected in FPF1.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<3 /* fed1_dbe */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<3 /* fed1_dbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FED1_DBE]: Set when a Double Bit Error is detected in FPF1.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<4 /* q0_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<4 /* q0_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q0_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<5 /* q0_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<5 /* q0_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q0_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<6 /* q0_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<6 /* q0_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q0_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<7 /* q1_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<7 /* q1_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q1_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<8 /* q1_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<8 /* q1_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q1_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<9 /* q1_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<9 /* q1_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q1_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<10 /* q2_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<10 /* q2_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q2_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<11 /* q2_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<11 /* q2_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q2_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<12 /* q2_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<12 /* q2_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q2_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<13 /* q3_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<13 /* q3_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q3_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<14 /* q3_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<14 /* q3_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q3_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<15 /* q3_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<15 /* q3_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q3_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<16 /* q4_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<16 /* q4_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q4_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<17 /* q4_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<17 /* q4_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q4_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<18 /* q4_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<18 /* q4_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q4_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<19 /* q5_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<19 /* q5_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q5_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<20 /* q5_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<20 /* q5_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q5_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<21 /* q5_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<21 /* q5_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q5_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<22 /* q6_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<22 /* q6_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q6_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<23 /* q6_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<23 /* q6_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q6_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<24 /* q6_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<24 /* q6_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q6_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<25 /* q7_und */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<25 /* q7_und */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q7_UND]: Set when a Queue0 page count available goes\n"
        "    negative.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<26 /* q7_coff */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<26 /* q7_coff */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q7_COFF]: Set when a Queue0 stack end tag is present and\n"
        "    the count available is greater than than pointers\n"
        "    present in the FPA.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<27 /* q7_perr */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<27 /* q7_perr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[Q7_PERR]: Set when a Queue0 pointer read from the stack in\n"
        "    the L2C does not have the FPA owner ship bit set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<28 /* pool0th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<28 /* pool0th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL0TH]: Set when FPA_QUE0_AVAILABLE is equal to\n"
        "    FPA_POOL`_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<29 /* pool1th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<29 /* pool1th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL1TH]: Set when FPA_QUE1_AVAILABLE is equal to\n"
        "    FPA_POOL1_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<30 /* pool2th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<30 /* pool2th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL2TH]: Set when FPA_QUE2_AVAILABLE is equal to\n"
        "    FPA_POOL2_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<31 /* pool3th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<31 /* pool3th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL3TH]: Set when FPA_QUE3_AVAILABLE is equal to\n"
        "    FPA_POOL3_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<32 /* pool4th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<32 /* pool4th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL4TH]: Set when FPA_QUE4_AVAILABLE is equal to\n"
        "    FPA_POOL4_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<33 /* pool5th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<33 /* pool5th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL5TH]: Set when FPA_QUE5_AVAILABLE is equal to\n"
        "    FPA_POOL5_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<34 /* pool6th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<34 /* pool6th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL6TH]: Set when FPA_QUE6_AVAILABLE is equal to\n"
        "    FPA_POOL6_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<35 /* pool7th */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<35 /* pool7th */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[POOL7TH]: Set when FPA_QUE7_AVAILABLE is equal to\n"
        "    FPA_POOL7_THRESHOLD[THRESH] and a pointer is\n"
        "    allocated or de-allocated.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<36 /* free0 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<36 /* free0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE0]: When a pointer for POOL0 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<37 /* free1 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<37 /* free1 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE1]: When a pointer for POOL1 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<38 /* free2 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<38 /* free2 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE2]: When a pointer for POOL2 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<39 /* free3 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<39 /* free3 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE3]: When a pointer for POOL3 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<40 /* free4 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<40 /* free4 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE4]: When a pointer for POOL4 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<41 /* free5 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<41 /* free5 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE5]: When a pointer for POOL5 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<42 /* free6 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<42 /* free6 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE6]: When a pointer for POOL6 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<43 /* free7 */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<43 /* free7 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[FREE7]: When a pointer for POOL7 is freed bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_FPA_INT_SUM;
    info.status_mask        = 1ull<<49 /* paddr_e */;
    info.enable_addr        = CVMX_FPA_INT_ENB;
    info.enable_mask        = 1ull<<49 /* paddr_e */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<22 /* fpa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR FPA_INT_SUM[PADDR_E]: Set when a pointer address does not fall in the\n"
        "    address range for a pool specified by\n"
        "    FPA_POOLX_START_ADDR and FPA_POOLX_END_ADDR.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_MIO_BOOT_ERR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_MIO_BOOT_ERR;
    info.status_mask        = 1ull<<0 /* adr_err */;
    info.enable_addr        = CVMX_MIO_BOOT_INT;
    info.enable_mask        = 1ull<<0 /* adr_int */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<20 /* mio */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR MIO_BOOT_ERR[ADR_ERR]: Address decode error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_MIO_BOOT_ERR;
    info.status_mask        = 1ull<<1 /* wait_err */;
    info.enable_addr        = CVMX_MIO_BOOT_INT;
    info.enable_mask        = 1ull<<1 /* wait_int */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<20 /* mio */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR MIO_BOOT_ERR[WAIT_ERR]: Wait mode error\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_DFA_ERROR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DFA_ERROR;
    info.status_mask        = 1ull<<0 /* dblovf */;
    info.enable_addr        = CVMX_DFA_INTMSK;
    info.enable_mask        = 1ull<<0 /* dblina */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<32 /* dfa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DFA_ERROR[DBLOVF]: Doorbell Overflow detected - Status bit\n"
        "    When set, the 20b accumulated doorbell register\n"
        "    had overflowed (SW wrote too many doorbell requests).\n"
        "    If the DBLINA had previously been enabled(set),\n"
        "    an interrupt will be posted. Software can clear\n"
        "    the interrupt by writing a 1 to this register bit.\n"
        "    NOTE: Detection of a Doorbell Register overflow\n"
        "    is a catastrophic error which may leave the DFA\n"
        "    HW in an unrecoverable state.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DFA_ERROR;
    info.status_mask        = 0x7ull<<1 /* dc0perr */;
    info.enable_addr        = CVMX_DFA_INTMSK;
    info.enable_mask        = 0x7ull<<1 /* dc0pena */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<32 /* dfa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DFA_ERROR[DC0PERR]: \"Cluster#0 RAM[3:1] Parity Error Detected\n"
        "    See also DFA_DTCFADR register which contains the\n"
        "    failing addresses for the internal node cache RAMs.\"\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DFA_ERROR;
    info.status_mask        = 1ull<<13 /* dlc0_ovferr */;
    info.enable_addr        = CVMX_DFA_INTMSK;
    info.enable_mask        = 1ull<<13 /* dlc0_ovfena */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<32 /* dfa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DFA_ERROR[DLC0_OVFERR]: DLC0 Fifo Overflow Error Detected\n"
        "    This condition should NEVER architecturally occur, and\n"
        "    is here in case HW credit/debit scheme is not working.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DFA_ERROR;
    info.status_mask        = 1ull<<17 /* dfanxm */;
    info.enable_addr        = CVMX_DFA_INTMSK;
    info.enable_mask        = 1ull<<17 /* dfanxmena */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<32 /* dfa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DFA_ERROR[DFANXM]: DFA Non-existent Memory Access\n"
        "    For o68/o61: DTEs (and backdoor CSR DFA Memory REGION reads)\n"
        "    have access to the following 38bit L2/DRAM address space\n"
        "    which maps to a 37bit physical DDR3 SDRAM address space.\n"
        "    see:\n"
        "    DR0: 0x0 0000 0000 0000 to 0x0 0000 0FFF FFFF\n"
        "    maps to lower 256MB of physical DDR3 SDRAM\n"
        "    DR1: 0x0 0000 2000 0000 to 0x0 0020 0FFF FFFF\n"
        "    maps to upper 127.75GB of DDR3 SDRAM\n"
        "    L2/DRAM address space                     Physical DDR3 SDRAM Address space\n"
        "    (38bit address)                           (37bit address)\n"
        "    +-----------+ 0x0020.0FFF.FFFF\n"
        "    ===   DR1   ===                          +-----------+ 0x001F.FFFF.FFFF\n"
        "    (128GB-256MB)\n"
        "    |           |                     =>    |           |  (128GB-256MB)\n"
        "    +-----------+ 0x0000.1FFF.FFFF          |   DR1\n"
        "    256MB   |   HOLE    |   (DO NOT USE)\n"
        "    +-----------+ 0x0000.0FFF.FFFF          +-----------+ 0x0000.0FFF.FFFF\n"
        "    256MB   |    DR0    |                           |   DR0     |   (256MB)\n"
        "    +-----------+ 0x0000.0000.0000          +-----------+ 0x0000.0000.0000\n"
        "    In the event the DFA generates a reference to the L2/DRAM\n"
        "    address hole (0x0000.0FFF.FFFF - 0x0000.1FFF.FFFF) or to\n"
        "    an address above 0x0020.0FFF.FFFF, the DFANXM programmable\n"
        "    interrupt bit will be set.\n"
        "    SWNOTE: Both the 1) SW DFA Graph compiler and the 2) SW NCB-Direct CSR\n"
        "    accesses to DFA Memory REGION MUST avoid making references\n"
        "    to these non-existent memory regions.\n"
        "    NOTE: If DFANXM is set during a DFA Graph Walk operation,\n"
        "    then the walk will prematurely terminate with RWORD0[REA]=ERR.\n"
        "    If DFANXM is set during a NCB-Direct CSR read access to DFA\n"
        "    Memory REGION, then the CSR read response data is forced to\n"
        "    128'hBADE_FEED_DEAD_BEEF_FACE_CAFE_BEAD_C0DE. (NOTE: the QW\n"
        "    being accessed, either the upper or lower QW will be returned).\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DFA_ERROR;
    info.status_mask        = 1ull<<18 /* replerr */;
    info.enable_addr        = CVMX_DFA_INTMSK;
    info.enable_mask        = 1ull<<18 /* replerrena */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<32 /* dfa */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DFA_ERROR[REPLERR]: DFA Illegal Replication Factor Error\n"
        "    For o68: DFA only supports 1x, 2x, and 4x port replication.\n"
        "    Legal configurations for memory are to support 2 port or\n"
        "    4 port configurations.\n"
        "    The REPLERR interrupt will be set in the following illegal\n"
        "    configuration cases:\n"
        "    1) An 8x replication factor is detected for any memory reference.\n"
        "    2) A 4x replication factor is detected for any memory reference\n"
        "    when only 2 memory ports are enabled.\n"
        "    NOTE: If REPLERR is set during a DFA Graph Walk operation,\n"
        "    then the walk will prematurely terminate with RWORD0[REA]=ERR.\n"
        "    If REPLERR is set during a NCB-Direct CSR read access to DFA\n"
        "    Memory REGION, then the CSR read response data is UNPREDICTABLE.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_TIM_REG_ERROR */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_TIM_REG_ERROR;
    info.status_mask        = 0xffffull<<0 /* mask */;
    info.enable_addr        = CVMX_TIM_REG_INT_MASK;
    info.enable_mask        = 0xffffull<<0 /* mask */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<29 /* tim */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR TIM_REG_ERROR[MASK]: Bit mask indicating the rings in error\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_CIU_CIB_LMCX_RAWX(0,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_LMCX_RAWX(0,0);
    info.status_mask        = 0xfull<<1 /* int_sec_errx */;
    info.enable_addr        = CVMX_CIU_CIB_LMCX_ENX(0,0);
    info.enable_mask        = 0xfull<<1 /* int_sec_errx */;
    info.flags              = CVMX_ERROR_FLAGS_ECC_SINGLE_BIT;
    info.group              = CVMX_ERROR_GROUP_LMC;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<52 /* lmc0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_LMCX_RAWX(0,0)[INT_SEC_ERRX]: Set when LMC0_INT[SEC_ERR<b>] set. Edge-sensitive interrupts, so software should clear\n"
        "    [INT_SEC_ERRX<b>] before clearing LMC0_INT[SEC_ERR<b>].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_LMCX_RAWX(0,0);
    info.status_mask        = 0xfull<<5 /* int_ded_errx */;
    info.enable_addr        = CVMX_CIU_CIB_LMCX_ENX(0,0);
    info.enable_mask        = 0xfull<<5 /* int_ded_errx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_LMC;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<52 /* lmc0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_LMCX_RAWX(0,0)[INT_DED_ERRX]: Set when LMC0_INT[DED_ERR<b>] set. Edge-sensitive interrupts, so software should clear\n"
        "    [INT_DED_ERRX<b>] before clearing LMC0_INT[DED_ERR<b>].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_LMCX_RAWX(0,0);
    info.status_mask        = 1ull<<0 /* int_nxm_wr_err */;
    info.enable_addr        = CVMX_CIU_CIB_LMCX_ENX(0,0);
    info.enable_mask        = 1ull<<0 /* int_nxm_wr_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_LMC;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<52 /* lmc0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_LMCX_RAWX(0,0)[INT_NXM_WR_ERR]: Set when LMC0_INT[NXM_WR_ERR] set. Edge-sensitive interrupt, so software should clear\n"
        "    [INT_NXM_WR_ERR] before clearing LMC0_INT[NXM_WR_ERR].\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_KEY_INT_SUM */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_KEY_INT_SUM;
    info.status_mask        = 1ull<<0 /* key_sbe */;
    info.enable_addr        = CVMX_KEY_INT_ENB;
    info.enable_mask        = 1ull<<0 /* key_sbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<31 /* key */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR KEY_INT_SUM[KEY_SBE]: N/A\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_KEY_INT_SUM;
    info.status_mask        = 1ull<<1 /* key_dbe */;
    info.enable_addr        = CVMX_KEY_INT_ENB;
    info.enable_mask        = 1ull<<1 /* key_dbe */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<31 /* key */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR KEY_INT_SUM[KEY_DBE]: N/A\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_CIU_CIB_RST_RAWX(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_RST_RAWX(0);
    info.status_mask        = 0x7ull<<0 /* int_linkx */;
    info.enable_addr        = CVMX_CIU_CIB_RST_ENX(0);
    info.enable_mask        = 0x7ull<<0 /* int_linkx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<63 /* rst */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_RST_RAWX(0)[INT_LINKX]: Set when RST_INT[RST_LINK<a>] set. Edge-sensitive interrupts, so software should clear\n"
        "    [INT_LINKX<a>] before clearing RST_INT[RST_LINK<a>].\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_CIU_CIB_RST_RAWX(0);
    info.status_mask        = 0x7ull<<3 /* int_perstx */;
    info.enable_addr        = CVMX_CIU_CIB_RST_ENX(0);
    info.enable_mask        = 0x7ull<<3 /* int_perstx */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<63 /* rst */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR CIU_CIB_RST_RAWX(0)[INT_PERSTX]: Set when RST_INT[PERST<a>] set. Edge-sensitive interrupts, so software should clear\n"
        "    [INT_PERSTX<a>] before clearing RST_INT[PERST<a>].\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_IOB_INT_SUM */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<0 /* np_sop */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<0 /* np_sop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[NP_SOP]: Set when a SOP is followed by an SOP for the same\n"
        "    port for a non-passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<1 /* np_eop */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<1 /* np_eop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[NP_EOP]: Set when a EOP is followed by an EOP for the same\n"
        "    port for a non-passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<2 /* p_sop */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<2 /* p_sop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[P_SOP]: Set when a SOP is followed by an SOP for the same\n"
        "    port for a passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<3 /* p_eop */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<3 /* p_eop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[P_EOP]: Set when a EOP is followed by an EOP for the same\n"
        "    port for a passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<4 /* np_dat */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<4 /* np_dat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[NP_DAT]: Set when a data arrives before a SOP for the same\n"
        "    port for a non-passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_IOB_INT_SUM;
    info.status_mask        = 1ull<<5 /* p_dat */;
    info.enable_addr        = CVMX_IOB_INT_ENB;
    info.enable_mask        = 1ull<<5 /* p_dat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<21 /* iob */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR IOB_INT_SUM[P_DAT]: Set when a data arrives before a SOP for the same\n"
        "    port for a passthrough packet.\n"
        "    The first detected error associated with bits [5:0]\n"
        "    of this register will only be set here. A new bit\n"
        "    can be set when the previous reported bit is cleared.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_AGL_GMX_BAD_REG */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_BAD_REG;
    info.status_mask        = 1ull<<32 /* ovrflw */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_BAD_REG[OVRFLW]: RX FIFO overflow (RGMII0)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_BAD_REG;
    info.status_mask        = 1ull<<33 /* txpop */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_BAD_REG[TXPOP]: TX FIFO underflow (RGMII0)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_BAD_REG;
    info.status_mask        = 1ull<<34 /* txpsh */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_BAD_REG[TXPSH]: TX FIFO overflow (RGMII0)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_BAD_REG;
    info.status_mask        = 1ull<<2 /* out_ovr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_BAD_REG[OUT_OVR]: Outbound data FIFO overflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_BAD_REG;
    info.status_mask        = 1ull<<22 /* loststat */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_BAD_REG[LOSTSTAT]: TX Statistics data was over-written\n"
        "    In RGMII, one bit per port\n"
        "    TX Stats are corrupted\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_AGL_GMX_RXX_INT_REG(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_RXX_INT_REG(0);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_AGL_GMX_RXX_INT_EN(0);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_RXX_INT_REG(0)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_RXX_INT_REG(0);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_AGL_GMX_RXX_INT_EN(0);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_RXX_INT_REG(0)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_AGL_GMX_TX_INT_REG */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_TX_INT_REG;
    info.status_mask        = 1ull<<0 /* pko_nxa */;
    info.enable_addr        = CVMX_AGL_GMX_TX_INT_EN;
    info.enable_mask        = 1ull<<0 /* pko_nxa */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_TX_INT_REG[PKO_NXA]: Port address out-of-range from PKO Interface\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_AGL_GMX_TX_INT_REG;
    info.status_mask        = 1ull<<2 /* undflw */;
    info.enable_addr        = CVMX_AGL_GMX_TX_INT_EN;
    info.enable_mask        = 1ull<<2 /* undflw */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_MGMT_PORT;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<46 /* agl */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR AGL_GMX_TX_INT_REG[UNDFLW]: TX Underflow\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_BAD_REG(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(0);
    info.status_mask        = 0xfull<<2 /* out_ovr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(0)[OUT_OVR]: Outbound data FIFO overflow (per port)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(0);
    info.status_mask        = 0xfull<<22 /* loststat */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(0)[LOSTSTAT]: TX Statistics data was over-written\n"
        "    In SGMII, one bit per port\n"
        "    In XAUI, only port0 is used\n"
        "    TX Stats are corrupted\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(0);
    info.status_mask        = 1ull<<26 /* statovr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(0)[STATOVR]: TX Statistics overflow\n"
        "    The common FIFO to SGMII and XAUI had an overflow\n"
        "    TX Stats are corrupted\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(0);
    info.status_mask        = 0xfull<<27 /* inb_nxa */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(0)[INB_NXA]: Inbound port > GMX_RX_PRTS\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(0,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,0);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,0);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,0)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(1,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,0);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,0);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,0)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(2,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,0);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,0);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,0)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(3,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,0);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,0);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,0)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_TX_INT_REG(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(0);
    info.status_mask        = 1ull<<0 /* pko_nxa */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(0);
    info.enable_mask        = 1ull<<0 /* pko_nxa */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(0)[PKO_NXA]: Port address out-of-range from PKO Interface\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(0);
    info.status_mask        = 0xfull<<2 /* undflw */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(0);
    info.enable_mask        = 0xfull<<2 /* undflw */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(0)[UNDFLW]: TX Underflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(0);
    info.status_mask        = 0xfull<<20 /* ptp_lost */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(0);
    info.enable_mask        = 0xfull<<20 /* ptp_lost */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(0)[PTP_LOST]: A packet with a PTP request was not able to be\n"
        "    sent due to XSCOL\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(0,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,0);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,0);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,0)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(1,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,0);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,0);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 1;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,0)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(2,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,0);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,0);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 2;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,0)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(3,0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,0);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,0);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 3;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,0)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSXX_INT_REG(0) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<0 /* txflt */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<0 /* txflt_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[TXFLT]: None defined at this time, always 0x0\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<1 /* rxbad */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<1 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[RXBAD]: Set when RX state machine in bad state\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<2 /* rxsynbad */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<2 /* rxsynbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[RXSYNBAD]: Set when RX code grp sync st machine in bad state\n"
        "    in one of the 4 xaui lanes\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<3 /* bitlckls */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<3 /* bitlckls_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[BITLCKLS]: Set when Bit lock lost on 1 or more xaui lanes\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<4 /* synlos */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<4 /* synlos_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[SYNLOS]: Set when Code group sync lost on 1 or more  lanes\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<5 /* algnlos */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<5 /* algnlos_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[ALGNLOS]: Set when XAUI lanes lose alignment\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSXX_INT_REG(0);
    info.status_mask        = 1ull<<6 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSXX_INT_EN_REG(0);
    info.enable_mask        = 1ull<<6 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<36 /* agx0 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSXX_INT_REG(0)[DBG_SYNC]: Code Group sync failure debug help, see Note below\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_BAD_REG(1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(1);
    info.status_mask        = 0xfull<<2 /* out_ovr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(1)[OUT_OVR]: Outbound data FIFO overflow (per port)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(1);
    info.status_mask        = 0xfull<<22 /* loststat */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(1)[LOSTSTAT]: TX Statistics data was over-written\n"
        "    In SGMII, one bit per port\n"
        "    In XAUI, only port0 is used\n"
        "    TX Stats are corrupted\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(1);
    info.status_mask        = 1ull<<26 /* statovr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(1)[STATOVR]: TX Statistics overflow\n"
        "    The common FIFO to SGMII and XAUI had an overflow\n"
        "    TX Stats are corrupted\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_BAD_REG(1);
    info.status_mask        = 0xfull<<27 /* inb_nxa */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_BAD_REG(1)[INB_NXA]: Inbound port > GMX_RX_PRTS\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(0,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(0,1);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(0,1);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(0,1)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(1,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(1,1);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(1,1);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(1,1)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(2,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(2,1);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(2,1);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(2,1)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_RXX_INT_REG(3,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<1 /* carext */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<1 /* carext */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[CAREXT]: Carrier extend error\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<8 /* skperr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<8 /* skperr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[SKPERR]: Skipper error\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<10 /* ovrerr */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<10 /* ovrerr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[OVRERR]: Internal Data Aggregation Overflow\n"
        "    This interrupt should never assert\n"
        "    (SGMII/1000Base-X only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<20 /* loc_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<20 /* loc_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[LOC_FAULT]: Local Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<21 /* rem_fault */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<21 /* rem_fault */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[REM_FAULT]: Remote Fault Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<22 /* bad_seq */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<22 /* bad_seq */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[BAD_SEQ]: Reserved Sequence Deteted\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<23 /* bad_term */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<23 /* bad_term */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[BAD_TERM]: Frame is terminated by control character other\n"
        "    than /T/.  The error propagation control\n"
        "    character /E/ will be included as part of the\n"
        "    frame and does not cause a frame termination.\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<24 /* unsop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<24 /* unsop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[UNSOP]: Unexpected SOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<25 /* uneop */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<25 /* uneop */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[UNEOP]: Unexpected EOP\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<26 /* undat */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<26 /* undat */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[UNDAT]: Unexpected Data\n"
        "    (XAUI Mode only)\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<27 /* hg2fld */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<27 /* hg2fld */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[HG2FLD]: HiGig2 received message field error, as below\n"
        "    1) MSG_TYPE field not 6'b00_0000\n"
        "    i.e. it is not a FLOW CONTROL message, which\n"
        "    is the only defined type for HiGig2\n"
        "    2) FWD_TYPE field not 2'b00 i.e. Link Level msg\n"
        "    which is the only defined type for HiGig2\n"
        "    3) FC_OBJECT field is neither 4'b0000 for\n"
        "    Physical Link nor 4'b0010 for Logical Link.\n"
        "    Those are the only two defined types in HiGig2\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<28 /* hg2cc */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<28 /* hg2cc */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[HG2CC]: HiGig2 received message CRC or Control char  error\n"
        "    Set when either CRC8 error detected or when\n"
        "    a Control Character is found in the message\n"
        "    bytes after the K.SOM\n"
        "    NOTE: HG2CC has higher priority than HG2FLD\n"
        "    i.e. a HiGig2 message that results in HG2CC\n"
        "    getting set, will never set HG2FLD.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_RXX_INT_REG(3,1);
    info.status_mask        = 1ull<<29 /* wol */;
    info.enable_addr        = CVMX_GMXX_RXX_INT_EN(3,1);
    info.enable_mask        = 1ull<<29 /* wol */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_RXX_INT_REG(3,1)[WOL]: A Wake-on-LAN event has occurred\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_GMXX_TX_INT_REG(1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(1);
    info.status_mask        = 1ull<<0 /* pko_nxa */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(1);
    info.enable_mask        = 1ull<<0 /* pko_nxa */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(1)[PKO_NXA]: Port address out-of-range from PKO Interface\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(1);
    info.status_mask        = 0xfull<<2 /* undflw */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(1);
    info.enable_mask        = 0xfull<<2 /* undflw */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(1)[UNDFLW]: TX Underflow\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_GMXX_TX_INT_REG(1);
    info.status_mask        = 0xfull<<20 /* ptp_lost */;
    info.enable_addr        = CVMX_GMXX_TX_INT_EN(1);
    info.enable_mask        = 0xfull<<20 /* ptp_lost */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR GMXX_TX_INT_REG(1)[PTP_LOST]: A packet with a PTP request was not able to be\n"
        "    sent due to XSCOL\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(0,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(0,1);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(0,1);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 16;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(0,1)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(1,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(1,1);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(1,1);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 17;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(1,1)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(2,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(2,1);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(2,1);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 18;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(2,1)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PCSX_INTX_REG(3,1) */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<2 /* an_err */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<2 /* an_err_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[AN_ERR]: AN Error, AN resolution function failed\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<3 /* txfifu */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<3 /* txfifu_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[TXFIFU]: Set whenever HW detects a TX fifo underflowflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<4 /* txfifo */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<4 /* txfifo_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[TXFIFO]: Set whenever HW detects a TX fifo overflow\n"
        "    condition\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<5 /* txbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<5 /* txbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[TXBAD]: Set by HW whenever tx st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<7 /* rxbad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<7 /* rxbad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[RXBAD]: Set by HW whenever rx st machine reaches a  bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<8 /* rxlock */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<8 /* rxlock_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[RXLOCK]: Set by HW whenever code group Sync or bit lock\n"
        "    failure occurs\n"
        "    Cannot fire in loopback1 mode\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<9 /* an_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<9 /* an_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[AN_BAD]: Set by HW whenever AN st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<10 /* sync_bad */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<10 /* sync_bad_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[SYNC_BAD]: Set by HW whenever rx sync st machine reaches a bad\n"
        "    state. Should never be set during normal operation\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PCSX_INTX_REG(3,1);
    info.status_mask        = 1ull<<12 /* dbg_sync */;
    info.enable_addr        = CVMX_PCSX_INTX_EN_REG(3,1);
    info.enable_mask        = 1ull<<12 /* dbg_sync_en */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_ETHERNET;
    info.group_index        = 19;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<37 /* agx1 */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PCSX_INTX_REG(3,1)[DBG_SYNC]: Code Group sync failure debug help\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_PEXP_SLI_INT_SUM */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<0 /* rml_to */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<0 /* rml_to */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[RML_TO]: A read or write transfer did not complete\n"
        "    within 0xffff core clocks.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<2 /* bar0_to */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<2 /* bar0_to */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[BAR0_TO]: BAR0 R/W to a NCB device did not receive\n"
        "    read-data/commit in 0xffff core clocks.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<3 /* iob2big */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<3 /* iob2big */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[IOB2BIG]: A requested IOBDMA is to large.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<8 /* m0_up_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<8 /* m0_up_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M0_UP_B0]: Received Unsupported P-TLP for Bar0 from MAC 0.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<9 /* m0_up_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<9 /* m0_up_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M0_UP_WI]: Received Unsupported P-TLP for Window Register\n"
        "    from MAC 0. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<10 /* m0_un_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<10 /* m0_un_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M0_UN_B0]: Received Unsupported N-TLP for Bar0 from MAC 0.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<11 /* m0_un_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<11 /* m0_un_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M0_UN_WI]: Received Unsupported N-TLP for Window Register\n"
        "    from MAC 0. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<12 /* m1_up_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<12 /* m1_up_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M1_UP_B0]: Received Unsupported P-TLP for Bar0 from MAC 1.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<13 /* m1_up_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<13 /* m1_up_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M1_UP_WI]: Received Unsupported P-TLP for Window Register\n"
        "    from MAC 1. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<14 /* m1_un_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<14 /* m1_un_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M1_UN_B0]: Received Unsupported N-TLP for Bar0 from MAC 1.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<15 /* m1_un_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<15 /* m1_un_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M1_UN_WI]: Received Unsupported N-TLP for Window Register\n"
        "    from MAC 1. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<20 /* m2_up_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<20 /* m2_up_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M2_UP_B0]: Received Unsupported P-TLP for Bar0 from MAC 0.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<21 /* m2_up_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<21 /* m2_up_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M2_UP_WI]: Received Unsupported P-TLP for Window Register\n"
        "    from MAC 0. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<22 /* m2_un_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<22 /* m2_un_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M2_UN_B0]: Received Unsupported N-TLP for Bar0 from MAC 0.\n"
        "    This occurs when the BAR 0 address space is\n"
        "    disabeled.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<23 /* m2_un_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<23 /* m2_un_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M2_UN_WI]: Received Unsupported N-TLP for Window Register\n"
        "    from MAC 0. This occurs when the window registers\n"
        "    are disabeld and a window register access occurs.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<24 /* m3_up_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<24 /* m3_up_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M3_UP_B0]: Reserved.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<25 /* m3_up_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<25 /* m3_up_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M3_UP_WI]: Reserved.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<26 /* m3_un_b0 */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<26 /* m3_un_b0 */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M3_UN_B0]: Reserved.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<27 /* m3_un_wi */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<27 /* m3_un_wi */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[M3_UN_WI]: Reserved.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<48 /* pidbof */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<48 /* pidbof */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PIDBOF]: Packet Instruction Doorbell count overflowed. Which\n"
        "    doorbell can be found in DPI_PINT_INFO[PIDBOF]\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<49 /* psldbof */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<49 /* psldbof */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PSLDBOF]: Packet Scatterlist Doorbell count overflowed. Which\n"
        "    doorbell can be found in DPI_PINT_INFO[PSLDBOF]\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<50 /* pout_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<50 /* pout_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[POUT_ERR]: Set when PKO sends packet data with the error bit\n"
        "    set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<51 /* pin_bp */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<51 /* pin_bp */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PIN_BP]: Packet input count has exceeded the WMARK.\n"
        "    See SLI_PKT_IN_BP\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<52 /* pgl_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<52 /* pgl_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PGL_ERR]: When a read error occurs on a packet gather list\n"
        "    read this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<53 /* pdi_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<53 /* pdi_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PDI_ERR]: When a read error occurs on a packet data read\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<54 /* pop_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<54 /* pop_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[POP_ERR]: When a read error occurs on a packet scatter\n"
        "    pointer pair this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<55 /* pins_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<55 /* pins_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[PINS_ERR]: When a read error occurs on a packet instruction\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<56 /* sprt0_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<56 /* sprt0_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[SPRT0_ERR]: When an error response received on SLI port 0\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<57 /* sprt1_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<57 /* sprt1_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[SPRT1_ERR]: When an error response received on SLI port 1\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<58 /* sprt2_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<58 /* sprt2_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[SPRT2_ERR]: When an error response received on SLI port 2\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<59 /* sprt3_err */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<59 /* sprt3_err */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[SPRT3_ERR]: Reserved.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_PEXP_SLI_INT_SUM;
    info.status_mask        = 1ull<<60 /* ill_pad */;
    info.enable_addr        = CVMX_PEXP_SLI_INT_ENB_CIU;
    info.enable_mask        = 1ull<<60 /* ill_pad */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<34 /* sli */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR PEXP_SLI_INT_SUM[ILL_PAD]: Set when a BAR0 address R/W falls into theaddress\n"
        "    range of the Packet-CSR, but for an unused\n"
        "    address.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_DPI_INT_REG */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<0 /* nderr */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<0 /* nderr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[NDERR]: NCB Decode Error\n"
        "    DPI received a NCB transaction on the outbound\n"
        "    bus to the DPI deviceID, but the command was not\n"
        "    recognized.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<1 /* nfovr */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<1 /* nfovr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[NFOVR]: CSR Fifo Overflow\n"
        "    DPI can store upto 16 CSR request.  The FIFO will\n"
        "    overflow if that number is exceeded.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 0xffull<<8 /* dmadbo */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 0xffull<<8 /* dmadbo */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[DMADBO]: DMAx doorbell overflow.\n"
        "    DPI has a 32-bit counter for each request's queue\n"
        "    outstanding doorbell counts. Interrupt will fire\n"
        "    if the count overflows.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<16 /* req_badadr */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<16 /* req_badadr */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_BADADR]: DMA instruction fetch with bad pointer\n"
        "    Interrupt will fire if DPI forms an instruction\n"
        "    fetch to the NULL pointer.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<17 /* req_badlen */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<17 /* req_badlen */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_BADLEN]: DMA instruction fetch with length\n"
        "    Interrupt will fire if DPI forms an instruction\n"
        "    fetch with length of zero.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<18 /* req_ovrflw */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<18 /* req_ovrflw */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_OVRFLW]: DMA instruction FIFO overflow\n"
        "    DPI tracks outstanding instructions fetches.\n"
        "    Interrupt will fire when FIFO overflows.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<19 /* req_undflw */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<19 /* req_undflw */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_UNDFLW]: DMA instruction FIFO underflow\n"
        "    DPI tracks outstanding instructions fetches.\n"
        "    Interrupt will fire when FIFO underflows.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<20 /* req_anull */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<20 /* req_anull */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_ANULL]: DMA instruction filled with bad instruction\n"
        "    Fetched instruction word was 0.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<21 /* req_inull */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<21 /* req_inull */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_INULL]: DMA instruction filled with NULL pointer\n"
        "    Next pointer was NULL.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<22 /* req_badfil */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<22 /* req_badfil */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[REQ_BADFIL]: DMA instruction unexpected fill\n"
        "    Instruction fill when none outstanding.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<24 /* sprt0_rst */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<24 /* sprt0_rst */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[SPRT0_RST]: DMA instruction was dropped because the source or\n"
        "    destination port was in reset.\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<25 /* sprt1_rst */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<25 /* sprt1_rst */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[SPRT1_RST]: DMA instruction was dropped because the source or\n"
        "    destination port was in reset.\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<26 /* sprt2_rst */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<26 /* sprt2_rst */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[SPRT2_RST]: DMA instruction was dropped because the source or\n"
        "    destination port was in reset.\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_INT_REG;
    info.status_mask        = 1ull<<27 /* sprt3_rst */;
    info.enable_addr        = CVMX_DPI_INT_EN;
    info.enable_mask        = 1ull<<27 /* sprt3_rst */;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_INT_REG[SPRT3_RST]: DMA instruction was dropped because the source or\n"
        "    destination port was in reset.\n"
        "    this bit is set.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_DPI_PKT_ERR_RSP */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_PKT_ERR_RSP;
    info.status_mask        = 1ull<<0 /* pkterr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_PKT_ERR_RSP[PKTERR]: Indicates that an ErrorResponse was received from\n"
        "    the I/O subsystem.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_DPI_REQ_ERR_RSP */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_REQ_ERR_RSP;
    info.status_mask        = 0xffull<<0 /* qerr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_REQ_ERR_RSP[QERR]: Indicates which instruction queue received an\n"
        "    ErrorResponse from the I/O subsystem.\n"
        "    SW must clear the bit before the the cooresponding\n"
        "    instruction queue will continue processing\n"
        "    instructions if DPI_REQ_ERR_RSP_EN[EN] is set.\n";
    fail |= cvmx_error_add(&info);

    /* CVMX_DPI_REQ_ERR_RST */
    info.reg_type           = CVMX_ERROR_REGISTER_IO64;
    info.status_addr        = CVMX_DPI_REQ_ERR_RST;
    info.status_mask        = 0xffull<<0 /* qerr */;
    info.enable_addr        = 0;
    info.enable_mask        = 0;
    info.flags              = 0;
    info.group              = CVMX_ERROR_GROUP_INTERNAL;
    info.group_index        = 0;
    info.parent.reg_type    = CVMX_ERROR_REGISTER_IO64;
    info.parent.status_addr = CVMX_CIU_INT_SUM1;
    info.parent.status_mask = 1ull<<35 /* dpi */;
    info.func               = __cvmx_error_display;
    info.user_info          = (long)
        "ERROR DPI_REQ_ERR_RST[QERR]: Indicates which instruction queue dropped an\n"
        "    instruction because the source or destination\n"
        "    was in reset.\n"
        "    SW must clear the bit before the the cooresponding\n"
        "    instruction queue will continue processing\n"
        "    instructions if DPI_REQ_ERR_RST_EN[EN] is set.\n";
    fail |= cvmx_error_add(&info);

    return fail;
}

