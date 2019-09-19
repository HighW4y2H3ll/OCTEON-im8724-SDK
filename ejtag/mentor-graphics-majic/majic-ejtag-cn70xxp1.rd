REG = AGL_GMX_BAD_REG 0x80011800E0000518 MEMORY 8
REG_FIELD = AGL_GMX_BAD_REG txpsh 34 34, txpop 33 33, ovrflw 32 32, statovr 26 26, loststat 22 22, out_ovr 2 2

REG = AGL_GMX_BIST 0x80011800E0000400 MEMORY 8
REG_FIELD = AGL_GMX_BIST status 24 0

REG = AGL_GMX_PRT0_CFG 0x80011800E0000010 MEMORY 8
REG_FIELD = AGL_GMX_PRT0_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, burst 6 6, tx_en 5 5, rx_en 4 4, slottime 3 3, duplex 2 2, speed 1 1, en 0 0

REG = AGL_GMX_RX0_ADR_CAM0 0x80011800E0000180 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM0 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM1 0x80011800E0000188 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM1 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM2 0x80011800E0000190 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM2 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM3 0x80011800E0000198 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM3 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM4 0x80011800E00001A0 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM4 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM5 0x80011800E00001A8 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM5 adr 63 0

REG = AGL_GMX_RX0_ADR_CAM_EN 0x80011800E0000108 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CAM_EN en 7 0

REG = AGL_GMX_RX0_ADR_CTL 0x80011800E0000100 MEMORY 8
REG_FIELD = AGL_GMX_RX0_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0

REG = AGL_GMX_RX0_DECISION 0x80011800E0000040 MEMORY 8
REG_FIELD = AGL_GMX_RX0_DECISION cnt 4 0

REG = AGL_GMX_RX0_FRM_CHK 0x80011800E0000020 MEMORY 8
REG_FIELD = AGL_GMX_RX0_FRM_CHK niberr 9 9, skperr 8 8, rcverr 7 7, lenerr 6 6, alnerr 5 5, fcserr 4 4, jabber 3 3, maxerr 2 2, carext 1 1, minerr 0 0

REG = AGL_GMX_RX0_FRM_CTL 0x80011800E0000018 MEMORY 8
REG_FIELD = AGL_GMX_RX0_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pad_len 8 8, vlan_len 7 7, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0

REG = AGL_GMX_RX0_FRM_MAX 0x80011800E0000030 MEMORY 8
REG_FIELD = AGL_GMX_RX0_FRM_MAX len 15 0

REG = AGL_GMX_RX0_FRM_MIN 0x80011800E0000028 MEMORY 8
REG_FIELD = AGL_GMX_RX0_FRM_MIN len 15 0

REG = AGL_GMX_RX0_IFG 0x80011800E0000058 MEMORY 8
REG_FIELD = AGL_GMX_RX0_IFG ifg 3 0

REG = AGL_GMX_RX0_INT_EN 0x80011800E0000008 MEMORY 8
REG_FIELD = AGL_GMX_RX0_INT_EN wol 29 29, pause_drp 19 19, phy_dupx 18 18, phy_spd 17 17, phy_link 16 16, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, niberr 9 9, skperr 8 8, rcverr 7 7, lenerr 6 6, alnerr 5 5, fcserr 4 4, jabber 3 3, maxerr 2 2, carext 1 1, minerr 0 0

REG = AGL_GMX_RX0_INT_REG 0x80011800E0000000 MEMORY 8
REG_FIELD = AGL_GMX_RX0_INT_REG wol 29 29, pause_drp 19 19, phy_dupx 18 18, phy_spd 17 17, phy_link 16 16, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, niberr 9 9, skperr 8 8, rcverr 7 7, lenerr 6 6, alnerr 5 5, fcserr 4 4, jabber 3 3, maxerr 2 2, carext 1 1, minerr 0 0

REG = AGL_GMX_RX0_JABBER 0x80011800E0000038 MEMORY 8
REG_FIELD = AGL_GMX_RX0_JABBER cnt 15 0

REG = AGL_GMX_RX0_PAUSE_DROP_TIME 0x80011800E0000068 MEMORY 8
REG_FIELD = AGL_GMX_RX0_PAUSE_DROP_TIME status 15 0

REG = AGL_GMX_RX0_RX_INBND 0x80011800E0000060 MEMORY 8
REG_FIELD = AGL_GMX_RX0_RX_INBND duplex 3 3, speed 2 1, status 0 0

REG = AGL_GMX_RX0_STATS_CTL 0x80011800E0000050 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_CTL rd_clr 0 0

REG = AGL_GMX_RX0_STATS_OCTS 0x80011800E0000088 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_OCTS cnt 47 0

REG = AGL_GMX_RX0_STATS_OCTS_CTL 0x80011800E0000098 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_OCTS_CTL cnt 47 0

REG = AGL_GMX_RX0_STATS_OCTS_DMAC 0x80011800E00000A8 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_OCTS_DMAC cnt 47 0

REG = AGL_GMX_RX0_STATS_OCTS_DRP 0x80011800E00000B8 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_OCTS_DRP cnt 47 0

REG = AGL_GMX_RX0_STATS_PKTS 0x80011800E0000080 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_PKTS cnt 31 0

REG = AGL_GMX_RX0_STATS_PKTS_BAD 0x80011800E00000C0 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_PKTS_BAD cnt 31 0

REG = AGL_GMX_RX0_STATS_PKTS_CTL 0x80011800E0000090 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_PKTS_CTL cnt 31 0

REG = AGL_GMX_RX0_STATS_PKTS_DMAC 0x80011800E00000A0 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_PKTS_DMAC cnt 31 0

REG = AGL_GMX_RX0_STATS_PKTS_DRP 0x80011800E00000B0 MEMORY 8
REG_FIELD = AGL_GMX_RX0_STATS_PKTS_DRP cnt 31 0

REG = AGL_GMX_RX0_UDD_SKP 0x80011800E0000048 MEMORY 8
REG_FIELD = AGL_GMX_RX0_UDD_SKP fcssel 8 8, len 6 0

REG = AGL_GMX_RX_BP_DROP0 0x80011800E0000420 MEMORY 8
REG_FIELD = AGL_GMX_RX_BP_DROP0 mark 5 0

REG = AGL_GMX_RX_BP_OFF0 0x80011800E0000460 MEMORY 8
REG_FIELD = AGL_GMX_RX_BP_OFF0 mark 5 0

REG = AGL_GMX_RX_BP_ON0 0x80011800E0000440 MEMORY 8
REG_FIELD = AGL_GMX_RX_BP_ON0 mark 8 0

REG = AGL_GMX_RX_PRT_INFO 0x80011800E00004E8 MEMORY 8
REG_FIELD = AGL_GMX_RX_PRT_INFO drop 16 16, commit 0 0

REG = AGL_GMX_RX_TX_STATUS 0x80011800E00007E8 MEMORY 8
REG_FIELD = AGL_GMX_RX_TX_STATUS tx 4 4, rx 0 0

REG = AGL_GMX_SMAC0 0x80011800E0000230 MEMORY 8
REG_FIELD = AGL_GMX_SMAC0 smac 47 0

REG = AGL_GMX_STAT_BP 0x80011800E0000520 MEMORY 8
REG_FIELD = AGL_GMX_STAT_BP bp 16 16, cnt 15 0

REG = AGL_GMX_TX0_APPEND 0x80011800E0000218 MEMORY 8
REG_FIELD = AGL_GMX_TX0_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0

REG = AGL_GMX_TX0_CLK 0x80011800E0000208 MEMORY 8
REG_FIELD = AGL_GMX_TX0_CLK clk_cnt 5 0

REG = AGL_GMX_TX0_CTL 0x80011800E0000270 MEMORY 8
REG_FIELD = AGL_GMX_TX0_CTL xsdef_en 1 1, xscol_en 0 0

REG = AGL_GMX_TX0_MIN_PKT 0x80011800E0000240 MEMORY 8
REG_FIELD = AGL_GMX_TX0_MIN_PKT min_size 7 0

REG = AGL_GMX_TX0_PAUSE_PKT_INTERVAL 0x80011800E0000248 MEMORY 8
REG_FIELD = AGL_GMX_TX0_PAUSE_PKT_INTERVAL interval 15 0

REG = AGL_GMX_TX0_PAUSE_PKT_TIME 0x80011800E0000238 MEMORY 8
REG_FIELD = AGL_GMX_TX0_PAUSE_PKT_TIME time 15 0

REG = AGL_GMX_TX0_PAUSE_TOGO 0x80011800E0000258 MEMORY 8
REG_FIELD = AGL_GMX_TX0_PAUSE_TOGO time 15 0

REG = AGL_GMX_TX0_PAUSE_ZERO 0x80011800E0000260 MEMORY 8
REG_FIELD = AGL_GMX_TX0_PAUSE_ZERO send 0 0

REG = AGL_GMX_TX0_SOFT_PAUSE 0x80011800E0000250 MEMORY 8
REG_FIELD = AGL_GMX_TX0_SOFT_PAUSE time 15 0

REG = AGL_GMX_TX0_STAT0 0x80011800E0000280 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT0 xsdef 63 32, xscol 31 0

REG = AGL_GMX_TX0_STAT1 0x80011800E0000288 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT1 scol 63 32, mcol 31 0

REG = AGL_GMX_TX0_STAT2 0x80011800E0000290 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT2 octs 47 0

REG = AGL_GMX_TX0_STAT3 0x80011800E0000298 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT3 pkts 31 0

REG = AGL_GMX_TX0_STAT4 0x80011800E00002A0 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT4 hist1 63 32, hist0 31 0

REG = AGL_GMX_TX0_STAT5 0x80011800E00002A8 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT5 hist3 63 32, hist2 31 0

REG = AGL_GMX_TX0_STAT6 0x80011800E00002B0 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT6 hist5 63 32, hist4 31 0

REG = AGL_GMX_TX0_STAT7 0x80011800E00002B8 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT7 hist7 63 32, hist6 31 0

REG = AGL_GMX_TX0_STAT8 0x80011800E00002C0 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT8 mcst 63 32, bcst 31 0

REG = AGL_GMX_TX0_STAT9 0x80011800E00002C8 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STAT9 undflw 63 32, ctl 31 0

REG = AGL_GMX_TX0_STATS_CTL 0x80011800E0000268 MEMORY 8
REG_FIELD = AGL_GMX_TX0_STATS_CTL rd_clr 0 0

REG = AGL_GMX_TX0_THRESH 0x80011800E0000210 MEMORY 8
REG_FIELD = AGL_GMX_TX0_THRESH cnt 5 0

REG = AGL_GMX_TX_BP 0x80011800E00004D0 MEMORY 8
REG_FIELD = AGL_GMX_TX_BP bp 0 0

REG = AGL_GMX_TX_COL_ATTEMPT 0x80011800E0000498 MEMORY 8
REG_FIELD = AGL_GMX_TX_COL_ATTEMPT limit 4 0

REG = AGL_GMX_TX_IFG 0x80011800E0000488 MEMORY 8
REG_FIELD = AGL_GMX_TX_IFG ifg2 7 4, ifg1 3 0

REG = AGL_GMX_TX_INT_EN 0x80011800E0000508 MEMORY 8
REG_FIELD = AGL_GMX_TX_INT_EN ptp_lost 20 20, late_col 16 16, xsdef 12 12, xscol 8 8, undflw 2 2, pko_nxa 0 0

REG = AGL_GMX_TX_INT_REG 0x80011800E0000500 MEMORY 8
REG_FIELD = AGL_GMX_TX_INT_REG ptp_lost 20 20, late_col 16 16, xsdef 12 12, xscol 8 8, undflw 2 2, pko_nxa 0 0

REG = AGL_GMX_TX_JAM 0x80011800E0000490 MEMORY 8
REG_FIELD = AGL_GMX_TX_JAM jam 7 0

REG = AGL_GMX_TX_LFSR 0x80011800E00004F8 MEMORY 8
REG_FIELD = AGL_GMX_TX_LFSR lfsr 15 0

REG = AGL_GMX_TX_OVR_BP 0x80011800E00004C8 MEMORY 8
REG_FIELD = AGL_GMX_TX_OVR_BP en 8 8, bp 4 4, ign_full 0 0

REG = AGL_GMX_TX_PAUSE_PKT_DMAC 0x80011800E00004A0 MEMORY 8
REG_FIELD = AGL_GMX_TX_PAUSE_PKT_DMAC dmac 47 0

REG = AGL_GMX_TX_PAUSE_PKT_TYPE 0x80011800E00004A8 MEMORY 8
REG_FIELD = AGL_GMX_TX_PAUSE_PKT_TYPE type 15 0

REG = AGL_GMX_WOL_CTL 0x80011800E0000780 MEMORY 8
REG_FIELD = AGL_GMX_WOL_CTL magic_en 32 32, direct_en 16 16, en 0 0

REG = AGL_PRT0_CTL 0x80011800E0002000 MEMORY 8
REG_FIELD = AGL_PRT0_CTL drv_byp 63 63, cmp_pctl 60 56, cmp_nctl 52 48, drv_pctl 44 40, drv_nctl 36 32, clk_set 30 24, clkrx_byp 23 23, clkrx_set 22 16, clktx_byp 15 15, clktx_set 14 8, refclk_sel 7 6, dllrst 4 4, comp 3 3, enable 2 2, clkrst 1 1, mode 0 0

REG = BCH_BIST_RESULT 0x8001180071000080 MEMORY 8
REG_FIELD = BCH_BIST_RESULT ncb_oub 5 5, ncb_inb 4 4, dat 3 0

REG = BCH_CMD_BUF 0x8001180071000008 MEMORY 8
REG_FIELD = BCH_CMD_BUF dwb 57 49, pool 48 46, size 45 33, ptr 32 0

REG = BCH_CTL 0x8001180071000000 MEMORY 8
REG_FIELD = BCH_CTL free_ena 22 22, early_term 21 18, one_cmd 17 17, erase_disable 16 16, max_read 5 2, store_le 1 1, reset 0 0

REG = BCH_ERR_CFG 0x8001180071000010 MEMORY 8
REG_FIELD = BCH_ERR_CFG dat_flip 17 16, dat_cor_dis 0 0

REG = BCH_GEN_INT 0x8001180071000088 MEMORY 8
REG_FIELD = BCH_GEN_INT dat_dbe 2 2, dat_sbe 1 1, doorbell 0 0

REG = BCH_GEN_INT_EN 0x8001180071000090 MEMORY 8
REG_FIELD = BCH_GEN_INT_EN dat_dbe 2 2, dat_sbe 1 1, doorbell 0 0

REG = CIU_BIST 0x8001070000000730 MEMORY 8
REG_FIELD = CIU_BIST bist 2 0

REG = CIU_CIB_L2C_EN0 0x800107000000E100 MEMORY 8
REG_FIELD = CIU_CIB_L2C_EN0 cbcx_int_ioccmddbe 22 22, cbcx_int_ioccmdsbe 21 21, cbcx_int_rsddbe 20 20, cbcx_int_rsdsbe 19 19, mcix_int_vbfdbe 18 18, mcix_int_vbfsbe 17 17, tadx_int_rtgdbe 16 16, tadx_int_rtgsbe 15 15, tadx_int_rddislmc 14 14, tadx_int_wrdislmc 13 13, tadx_int_bigrd 12 12, tadx_int_bigwr 11 11, tadx_int_holerd 10 10, tadx_int_holewr 9 9, tadx_int_noway 8 8, tadx_int_tagdbe 7 7, tadx_int_tagsbe 6 6, tadx_int_fbfdbe 5 5, tadx_int_fbfsbe 4 4, tadx_int_sbfdbe 3 3, tadx_int_sbfsbe 2 2, tadx_int_l2ddbe 1 1, tadx_int_l2dsbe 0 0

REG = CIU_CIB_L2C_RAW0 0x800107000000E000 MEMORY 8
REG_FIELD = CIU_CIB_L2C_RAW0 cbcx_int_ioccmddbe 22 22, cbcx_int_ioccmdsbe 21 21, cbcx_int_rsddbe 20 20, cbcx_int_rsdsbe 19 19, mcix_int_vbfdbe 18 18, mcix_int_vbfsbe 17 17, tadx_int_rtgdbe 16 16, tadx_int_rtgsbe 15 15, tadx_int_rddislmc 14 14, tadx_int_wrdislmc 13 13, tadx_int_bigrd 12 12, tadx_int_bigwr 11 11, tadx_int_holerd 10 10, tadx_int_holewr 9 9, tadx_int_noway 8 8, tadx_int_tagdbe 7 7, tadx_int_tagsbe 6 6, tadx_int_fbfdbe 5 5, tadx_int_fbfsbe 4 4, tadx_int_sbfdbe 3 3, tadx_int_sbfsbe 2 2, tadx_int_l2ddbe 1 1, tadx_int_l2dsbe 0 0

REG = CIU_CIB_LMC0_EN000 0x800107000000E300 MEMORY 8
REG_FIELD = CIU_CIB_LMC0_EN000 int_ddr_err 11 11, int_dlc_ded 10 10, int_dlc_sec 9 9, int_ded_errx 8 5, int_sec_errx 4 1, int_nxm_wr_err 0 0

REG = CIU_CIB_LMC0_RAW000 0x800107000000E200 MEMORY 8
REG_FIELD = CIU_CIB_LMC0_RAW000 int_ddr_err 11 11, int_dlc_ded 10 10, int_dlc_sec 9 9, int_ded_errx 8 5, int_sec_errx 4 1, int_nxm_wr_err 0 0

REG = CIU_CIB_OCLA0_EN000 0x800107000000EE00 MEMORY 8
REG_FIELD = CIU_CIB_OCLA0_EN000 state_ddrfull 14 14, state_wmark 13 13, state_overfull 12 12, state_trigfull 11 11, state_captured 10 10, state_fsm1_int 9 9, state_fsm0_int 8 8, state_mcdx 7 5, state_trig 4 4, state_ovflx 3 0

REG = CIU_CIB_OCLA0_RAW000 0x800107000000EC00 MEMORY 8
REG_FIELD = CIU_CIB_OCLA0_RAW000 state_ddrfull 14 14, state_wmark 13 13, state_overfull 12 12, state_trigfull 11 11, state_captured 10 10, state_fsm1_int 9 9, state_fsm0_int 8 8, state_mcdx 7 5, state_trig 4 4, state_ovflx 3 0

REG = CIU_CIB_RST_EN0 0x800107000000E500 MEMORY 8
REG_FIELD = CIU_CIB_RST_EN0 int_perstx 5 3, int_linkx 2 0

REG = CIU_CIB_RST_RAW0 0x800107000000E400 MEMORY 8
REG_FIELD = CIU_CIB_RST_RAW0 int_perstx 5 3, int_linkx 2 0

REG = CIU_CIB_SATA_EN0 0x800107000000E700 MEMORY 8
REG_FIELD = CIU_CIB_SATA_EN0 uahc_pme_req_ip 3 3, uahc_intrq_ip 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0

REG = CIU_CIB_SATA_RAW0 0x800107000000E600 MEMORY 8
REG_FIELD = CIU_CIB_SATA_RAW0 uahc_pme_req_ip 3 3, uahc_intrq_ip 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0

REG = CIU_CIB_USBDRD0_EN000 0x800107000000EA00 MEMORY 8
REG_FIELD = CIU_CIB_USBDRD0_EN000 uahc_dev_int 10 10, uahc_imanx_ip 9 9, uahc_usbsts_hse 8 8, intstat_ram2_dbe 7 7, intstat_ram2_sbe 6 6, intstat_ram1_dbe 5 5, intstat_ram1_sbe 4 4, intstat_ram0_dbe 3 3, intstat_ram0_sbe 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0
REG = CIU_CIB_USBDRD1_EN000 0x800107000000EB00 MEMORY 8
REG_FIELD = CIU_CIB_USBDRD1_EN000 uahc_dev_int 10 10, uahc_imanx_ip 9 9, uahc_usbsts_hse 8 8, intstat_ram2_dbe 7 7, intstat_ram2_sbe 6 6, intstat_ram1_dbe 5 5, intstat_ram1_sbe 4 4, intstat_ram0_dbe 3 3, intstat_ram0_sbe 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0

REG = CIU_CIB_USBDRD0_RAW000 0x800107000000E800 MEMORY 8
REG_FIELD = CIU_CIB_USBDRD0_RAW000 uahc_dev_int 10 10, uahc_imanx_ip 9 9, uahc_usbsts_hse 8 8, intstat_ram2_dbe 7 7, intstat_ram2_sbe 6 6, intstat_ram1_dbe 5 5, intstat_ram1_sbe 4 4, intstat_ram0_dbe 3 3, intstat_ram0_sbe 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0
REG = CIU_CIB_USBDRD1_RAW000 0x800107000000E900 MEMORY 8
REG_FIELD = CIU_CIB_USBDRD1_RAW000 uahc_dev_int 10 10, uahc_imanx_ip 9 9, uahc_usbsts_hse 8 8, intstat_ram2_dbe 7 7, intstat_ram2_sbe 6 6, intstat_ram1_dbe 5 5, intstat_ram1_sbe 4 4, intstat_ram0_dbe 3 3, intstat_ram0_sbe 2 2, intstat_xm_bad_dma 1 1, intstat_xs_ncb_oob 0 0

REG = CIU_DINT 0x8001070000000720 MEMORY 8
REG_FIELD = CIU_DINT dint 3 0

REG = CIU_EN2_IO0_INT 0x800107000000A600 MEMORY 8
REG_FIELD = CIU_EN2_IO0_INT bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_IO1_INT 0x800107000000A608 MEMORY 8
REG_FIELD = CIU_EN2_IO1_INT bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_IO0_INT_W1C 0x800107000000CE00 MEMORY 8
REG_FIELD = CIU_EN2_IO0_INT_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_IO1_INT_W1C 0x800107000000CE08 MEMORY 8
REG_FIELD = CIU_EN2_IO1_INT_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_IO0_INT_W1S 0x800107000000AE00 MEMORY 8
REG_FIELD = CIU_EN2_IO0_INT_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_IO1_INT_W1S 0x800107000000AE08 MEMORY 8
REG_FIELD = CIU_EN2_IO1_INT_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP2 0x800107000000A000 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP2 0x800107000000A008 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP2 0x800107000000A010 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP2 0x800107000000A018 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP2_W1C 0x800107000000C800 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP2_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP2_W1C 0x800107000000C808 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP2_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP2_W1C 0x800107000000C810 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP2_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP2_W1C 0x800107000000C818 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP2_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP2_W1S 0x800107000000A800 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP2_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP2_W1S 0x800107000000A808 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP2_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP2_W1S 0x800107000000A810 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP2_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP2_W1S 0x800107000000A818 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP2_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP3 0x800107000000A200 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP3 0x800107000000A208 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP3 0x800107000000A210 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP3 0x800107000000A218 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP3_W1C 0x800107000000CA00 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP3_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP3_W1C 0x800107000000CA08 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP3_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP3_W1C 0x800107000000CA10 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP3_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP3_W1C 0x800107000000CA18 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP3_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP3_W1S 0x800107000000AA00 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP3_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP3_W1S 0x800107000000AA08 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP3_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP3_W1S 0x800107000000AA10 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP3_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP3_W1S 0x800107000000AA18 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP3_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP4 0x800107000000A400 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP4 0x800107000000A408 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP4 0x800107000000A410 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP4 0x800107000000A418 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP4_W1C 0x800107000000CC00 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP4_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP4_W1C 0x800107000000CC08 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP4_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP4_W1C 0x800107000000CC10 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP4_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP4_W1C 0x800107000000CC18 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP4_W1C bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_EN2_PP0_IP4_W1S 0x800107000000AC00 MEMORY 8
REG_FIELD = CIU_EN2_PP0_IP4_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP1_IP4_W1S 0x800107000000AC08 MEMORY 8
REG_FIELD = CIU_EN2_PP1_IP4_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP2_IP4_W1S 0x800107000000AC10 MEMORY 8
REG_FIELD = CIU_EN2_PP2_IP4_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_EN2_PP3_IP4_W1S 0x800107000000AC18 MEMORY 8
REG_FIELD = CIU_EN2_PP3_IP4_W1S bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_FUSE 0x8001070000000728 MEMORY 8
REG_FIELD = CIU_FUSE fuse 3 0

REG = CIU_GSTOP 0x8001070000000710 MEMORY 8
REG_FIELD = CIU_GSTOP gstop 0 0

REG = CIU_INT0_EN0 0x8001070000000200 MEMORY 8
REG_FIELD = CIU_INT0_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN0 0x8001070000000210 MEMORY 8
REG_FIELD = CIU_INT1_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN0 0x8001070000000220 MEMORY 8
REG_FIELD = CIU_INT2_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN0 0x8001070000000230 MEMORY 8
REG_FIELD = CIU_INT3_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT4_EN0 0x8001070000000240 MEMORY 8
REG_FIELD = CIU_INT4_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT5_EN0 0x8001070000000250 MEMORY 8
REG_FIELD = CIU_INT5_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT6_EN0 0x8001070000000260 MEMORY 8
REG_FIELD = CIU_INT6_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT7_EN0 0x8001070000000270 MEMORY 8
REG_FIELD = CIU_INT7_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT32_EN0 0x8001070000000400 MEMORY 8
REG_FIELD = CIU_INT32_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT33_EN0 0x8001070000000410 MEMORY 8
REG_FIELD = CIU_INT33_EN0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN0_W1C 0x8001070000002200 MEMORY 8
REG_FIELD = CIU_INT0_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN0_W1C 0x8001070000002210 MEMORY 8
REG_FIELD = CIU_INT1_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN0_W1C 0x8001070000002220 MEMORY 8
REG_FIELD = CIU_INT2_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN0_W1C 0x8001070000002230 MEMORY 8
REG_FIELD = CIU_INT3_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT4_EN0_W1C 0x8001070000002240 MEMORY 8
REG_FIELD = CIU_INT4_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT5_EN0_W1C 0x8001070000002250 MEMORY 8
REG_FIELD = CIU_INT5_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT6_EN0_W1C 0x8001070000002260 MEMORY 8
REG_FIELD = CIU_INT6_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT7_EN0_W1C 0x8001070000002270 MEMORY 8
REG_FIELD = CIU_INT7_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT32_EN0_W1C 0x8001070000002400 MEMORY 8
REG_FIELD = CIU_INT32_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT33_EN0_W1C 0x8001070000002410 MEMORY 8
REG_FIELD = CIU_INT33_EN0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN0_W1S 0x8001070000006200 MEMORY 8
REG_FIELD = CIU_INT0_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN0_W1S 0x8001070000006210 MEMORY 8
REG_FIELD = CIU_INT1_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN0_W1S 0x8001070000006220 MEMORY 8
REG_FIELD = CIU_INT2_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN0_W1S 0x8001070000006230 MEMORY 8
REG_FIELD = CIU_INT3_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT4_EN0_W1S 0x8001070000006240 MEMORY 8
REG_FIELD = CIU_INT4_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT5_EN0_W1S 0x8001070000006250 MEMORY 8
REG_FIELD = CIU_INT5_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT6_EN0_W1S 0x8001070000006260 MEMORY 8
REG_FIELD = CIU_INT6_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT7_EN0_W1S 0x8001070000006270 MEMORY 8
REG_FIELD = CIU_INT7_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT32_EN0_W1S 0x8001070000006400 MEMORY 8
REG_FIELD = CIU_INT32_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT33_EN0_W1S 0x8001070000006410 MEMORY 8
REG_FIELD = CIU_INT33_EN0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN1 0x8001070000000208 MEMORY 8
REG_FIELD = CIU_INT0_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN1 0x8001070000000218 MEMORY 8
REG_FIELD = CIU_INT1_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN1 0x8001070000000228 MEMORY 8
REG_FIELD = CIU_INT2_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN1 0x8001070000000238 MEMORY 8
REG_FIELD = CIU_INT3_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT4_EN1 0x8001070000000248 MEMORY 8
REG_FIELD = CIU_INT4_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT5_EN1 0x8001070000000258 MEMORY 8
REG_FIELD = CIU_INT5_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT6_EN1 0x8001070000000268 MEMORY 8
REG_FIELD = CIU_INT6_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT7_EN1 0x8001070000000278 MEMORY 8
REG_FIELD = CIU_INT7_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT32_EN1 0x8001070000000408 MEMORY 8
REG_FIELD = CIU_INT32_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT33_EN1 0x8001070000000418 MEMORY 8
REG_FIELD = CIU_INT33_EN1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_EN1_W1C 0x8001070000002208 MEMORY 8
REG_FIELD = CIU_INT0_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN1_W1C 0x8001070000002218 MEMORY 8
REG_FIELD = CIU_INT1_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN1_W1C 0x8001070000002228 MEMORY 8
REG_FIELD = CIU_INT2_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN1_W1C 0x8001070000002238 MEMORY 8
REG_FIELD = CIU_INT3_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT4_EN1_W1C 0x8001070000002248 MEMORY 8
REG_FIELD = CIU_INT4_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT5_EN1_W1C 0x8001070000002258 MEMORY 8
REG_FIELD = CIU_INT5_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT6_EN1_W1C 0x8001070000002268 MEMORY 8
REG_FIELD = CIU_INT6_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT7_EN1_W1C 0x8001070000002278 MEMORY 8
REG_FIELD = CIU_INT7_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT32_EN1_W1C 0x8001070000002408 MEMORY 8
REG_FIELD = CIU_INT32_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT33_EN1_W1C 0x8001070000002418 MEMORY 8
REG_FIELD = CIU_INT33_EN1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_EN1_W1S 0x8001070000006208 MEMORY 8
REG_FIELD = CIU_INT0_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN1_W1S 0x8001070000006218 MEMORY 8
REG_FIELD = CIU_INT1_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN1_W1S 0x8001070000006228 MEMORY 8
REG_FIELD = CIU_INT2_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN1_W1S 0x8001070000006238 MEMORY 8
REG_FIELD = CIU_INT3_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT4_EN1_W1S 0x8001070000006248 MEMORY 8
REG_FIELD = CIU_INT4_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT5_EN1_W1S 0x8001070000006258 MEMORY 8
REG_FIELD = CIU_INT5_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT6_EN1_W1S 0x8001070000006268 MEMORY 8
REG_FIELD = CIU_INT6_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT7_EN1_W1S 0x8001070000006278 MEMORY 8
REG_FIELD = CIU_INT7_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT32_EN1_W1S 0x8001070000006408 MEMORY 8
REG_FIELD = CIU_INT32_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT33_EN1_W1S 0x8001070000006418 MEMORY 8
REG_FIELD = CIU_INT33_EN1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_EN4_0 0x8001070000000C80 MEMORY 8
REG_FIELD = CIU_INT0_EN4_0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN4_0 0x8001070000000C90 MEMORY 8
REG_FIELD = CIU_INT1_EN4_0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN4_0 0x8001070000000CA0 MEMORY 8
REG_FIELD = CIU_INT2_EN4_0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN4_0 0x8001070000000CB0 MEMORY 8
REG_FIELD = CIU_INT3_EN4_0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN4_0_W1C 0x8001070000002C80 MEMORY 8
REG_FIELD = CIU_INT0_EN4_0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN4_0_W1C 0x8001070000002C90 MEMORY 8
REG_FIELD = CIU_INT1_EN4_0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN4_0_W1C 0x8001070000002CA0 MEMORY 8
REG_FIELD = CIU_INT2_EN4_0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN4_0_W1C 0x8001070000002CB0 MEMORY 8
REG_FIELD = CIU_INT3_EN4_0_W1C bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN4_0_W1S 0x8001070000006C80 MEMORY 8
REG_FIELD = CIU_INT0_EN4_0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_EN4_0_W1S 0x8001070000006C90 MEMORY 8
REG_FIELD = CIU_INT1_EN4_0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_EN4_0_W1S 0x8001070000006CA0 MEMORY 8
REG_FIELD = CIU_INT2_EN4_0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_EN4_0_W1S 0x8001070000006CB0 MEMORY 8
REG_FIELD = CIU_INT3_EN4_0_W1S bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_EN4_1 0x8001070000000C88 MEMORY 8
REG_FIELD = CIU_INT0_EN4_1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN4_1 0x8001070000000C98 MEMORY 8
REG_FIELD = CIU_INT1_EN4_1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN4_1 0x8001070000000CA8 MEMORY 8
REG_FIELD = CIU_INT2_EN4_1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN4_1 0x8001070000000CB8 MEMORY 8
REG_FIELD = CIU_INT3_EN4_1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_EN4_1_W1C 0x8001070000002C88 MEMORY 8
REG_FIELD = CIU_INT0_EN4_1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN4_1_W1C 0x8001070000002C98 MEMORY 8
REG_FIELD = CIU_INT1_EN4_1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN4_1_W1C 0x8001070000002CA8 MEMORY 8
REG_FIELD = CIU_INT2_EN4_1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN4_1_W1C 0x8001070000002CB8 MEMORY 8
REG_FIELD = CIU_INT3_EN4_1_W1C rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_EN4_1_W1S 0x8001070000006C88 MEMORY 8
REG_FIELD = CIU_INT0_EN4_1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT1_EN4_1_W1S 0x8001070000006C98 MEMORY 8
REG_FIELD = CIU_INT1_EN4_1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT2_EN4_1_W1S 0x8001070000006CA8 MEMORY 8
REG_FIELD = CIU_INT2_EN4_1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_INT3_EN4_1_W1S 0x8001070000006CB8 MEMORY 8
REG_FIELD = CIU_INT3_EN4_1_W1S rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INT0_SUM0 0x8001070000000000 MEMORY 8
REG_FIELD = CIU_INT0_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_SUM0 0x8001070000000008 MEMORY 8
REG_FIELD = CIU_INT1_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_SUM0 0x8001070000000010 MEMORY 8
REG_FIELD = CIU_INT2_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_SUM0 0x8001070000000018 MEMORY 8
REG_FIELD = CIU_INT3_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT4_SUM0 0x8001070000000020 MEMORY 8
REG_FIELD = CIU_INT4_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT5_SUM0 0x8001070000000028 MEMORY 8
REG_FIELD = CIU_INT5_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT6_SUM0 0x8001070000000030 MEMORY 8
REG_FIELD = CIU_INT6_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT7_SUM0 0x8001070000000038 MEMORY 8
REG_FIELD = CIU_INT7_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT32_SUM0 0x8001070000000100 MEMORY 8
REG_FIELD = CIU_INT32_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT0_SUM4 0x8001070000000C00 MEMORY 8
REG_FIELD = CIU_INT0_SUM4 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT1_SUM4 0x8001070000000C08 MEMORY 8
REG_FIELD = CIU_INT1_SUM4 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT2_SUM4 0x8001070000000C10 MEMORY 8
REG_FIELD = CIU_INT2_SUM4 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0
REG = CIU_INT3_SUM4 0x8001070000000C18 MEMORY 8
REG_FIELD = CIU_INT3_SUM4 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT33_SUM0 0x8001070000000110 MEMORY 8
REG_FIELD = CIU_INT33_SUM0 bootdma 63 63, ipdppthr 61 61, powiq 60 60, twsi2 59 59, mpi 58 58, pcm 57 57, timer 55 52, sum2 51 51, ipd_drp 50 50, gmx_drp 49 48, twsi 45 45, wdog_sum 44 44, pci_msi 43 40, pci_int 39 36, uart 35 34, mbox 33 32, gpio 31 16, workq 15 0

REG = CIU_INT_SUM1 0x8001070000000108 MEMORY 8
REG_FIELD = CIU_INT_SUM1 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_INTR_SLOWDOWN 0x80010700000007D0 MEMORY 8
REG_FIELD = CIU_INTR_SLOWDOWN ctl 2 0

REG = CIU_MBOX_CLR0 0x8001070000000680 MEMORY 8
REG_FIELD = CIU_MBOX_CLR0 bits 31 0
REG = CIU_MBOX_CLR1 0x8001070000000688 MEMORY 8
REG_FIELD = CIU_MBOX_CLR1 bits 31 0
REG = CIU_MBOX_CLR2 0x8001070000000690 MEMORY 8
REG_FIELD = CIU_MBOX_CLR2 bits 31 0
REG = CIU_MBOX_CLR3 0x8001070000000698 MEMORY 8
REG_FIELD = CIU_MBOX_CLR3 bits 31 0

REG = CIU_MBOX_SET0 0x8001070000000600 MEMORY 8
REG_FIELD = CIU_MBOX_SET0 bits 31 0
REG = CIU_MBOX_SET1 0x8001070000000608 MEMORY 8
REG_FIELD = CIU_MBOX_SET1 bits 31 0
REG = CIU_MBOX_SET2 0x8001070000000610 MEMORY 8
REG_FIELD = CIU_MBOX_SET2 bits 31 0
REG = CIU_MBOX_SET3 0x8001070000000618 MEMORY 8
REG_FIELD = CIU_MBOX_SET3 bits 31 0

REG = CIU_NMI 0x8001070000000718 MEMORY 8
REG_FIELD = CIU_NMI nmi 3 0

REG = CIU_PCI_INTA 0x8001070000000750 MEMORY 8
REG_FIELD = CIU_PCI_INTA intr 1 0

REG = CIU_PP_DBG 0x8001070000000708 MEMORY 8
REG_FIELD = CIU_PP_DBG ppdbg 3 0

REG = CIU_PP_POKE0 0x8001070000000580 MEMORY 8
REG_FIELD = CIU_PP_POKE0 poke 63 0
REG = CIU_PP_POKE1 0x8001070000000588 MEMORY 8
REG_FIELD = CIU_PP_POKE1 poke 63 0
REG = CIU_PP_POKE2 0x8001070000000590 MEMORY 8
REG_FIELD = CIU_PP_POKE2 poke 63 0
REG = CIU_PP_POKE3 0x8001070000000598 MEMORY 8
REG_FIELD = CIU_PP_POKE3 poke 63 0

REG = CIU_PP_RST 0x8001070000000700 MEMORY 8
REG_FIELD = CIU_PP_RST rst 3 1, rst0 0 0

REG = CIU_PP_RST_PENDING 0x8001070000000740 MEMORY 8
REG_FIELD = CIU_PP_RST_PENDING pend 47 0

REG = CIU_SOFT_BIST 0x8001070000000738 MEMORY 8
REG_FIELD = CIU_SOFT_BIST soft_bist 0 0

REG = CIU_SUM1_IO0_INT 0x8001070000008600 MEMORY 8
REG_FIELD = CIU_SUM1_IO0_INT rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_IO1_INT 0x8001070000008608 MEMORY 8
REG_FIELD = CIU_SUM1_IO1_INT rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_SUM1_PP0_IP2 0x8001070000008000 MEMORY 8
REG_FIELD = CIU_SUM1_PP0_IP2 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP1_IP2 0x8001070000008008 MEMORY 8
REG_FIELD = CIU_SUM1_PP1_IP2 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP2_IP2 0x8001070000008010 MEMORY 8
REG_FIELD = CIU_SUM1_PP2_IP2 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP3_IP2 0x8001070000008018 MEMORY 8
REG_FIELD = CIU_SUM1_PP3_IP2 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_SUM1_PP0_IP3 0x8001070000008200 MEMORY 8
REG_FIELD = CIU_SUM1_PP0_IP3 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP1_IP3 0x8001070000008208 MEMORY 8
REG_FIELD = CIU_SUM1_PP1_IP3 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP2_IP3 0x8001070000008210 MEMORY 8
REG_FIELD = CIU_SUM1_PP2_IP3 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP3_IP3 0x8001070000008218 MEMORY 8
REG_FIELD = CIU_SUM1_PP3_IP3 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_SUM1_PP0_IP4 0x8001070000008400 MEMORY 8
REG_FIELD = CIU_SUM1_PP0_IP4 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP1_IP4 0x8001070000008408 MEMORY 8
REG_FIELD = CIU_SUM1_PP1_IP4 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP2_IP4 0x8001070000008410 MEMORY 8
REG_FIELD = CIU_SUM1_PP2_IP4 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0
REG = CIU_SUM1_PP3_IP4 0x8001070000008418 MEMORY 8
REG_FIELD = CIU_SUM1_PP3_IP4 rst 63 63, lmc0 52 52, pem2 50 50, pem1 49 49, pem0 48 48, ptp 47 47, agl 46 46, dpi_dma 40 40, agx1 37 37, agx0 36 36, dpi 35 35, sli 34 34, usb 33 33, dfa 32 32, key 31 31, rad 30 30, tim 29 29, pko 27 27, pip 26 26, ipd 25 25, l2c 24 24, pow 23 23, fpa 22 22, iob 21 21, mio 20 20, nand 19 19, usb1 17 17, wdog 3 0

REG = CIU_SUM2_IO0_INT 0x8001070000008E00 MEMORY 8
REG_FIELD = CIU_SUM2_IO0_INT bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_IO1_INT 0x8001070000008E08 MEMORY 8
REG_FIELD = CIU_SUM2_IO1_INT bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_SUM2_PP0_IP2 0x8001070000008800 MEMORY 8
REG_FIELD = CIU_SUM2_PP0_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP1_IP2 0x8001070000008808 MEMORY 8
REG_FIELD = CIU_SUM2_PP1_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP2_IP2 0x8001070000008810 MEMORY 8
REG_FIELD = CIU_SUM2_PP2_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP3_IP2 0x8001070000008818 MEMORY 8
REG_FIELD = CIU_SUM2_PP3_IP2 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_SUM2_PP0_IP3 0x8001070000008A00 MEMORY 8
REG_FIELD = CIU_SUM2_PP0_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP1_IP3 0x8001070000008A08 MEMORY 8
REG_FIELD = CIU_SUM2_PP1_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP2_IP3 0x8001070000008A10 MEMORY 8
REG_FIELD = CIU_SUM2_PP2_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP3_IP3 0x8001070000008A18 MEMORY 8
REG_FIELD = CIU_SUM2_PP3_IP3 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_SUM2_PP0_IP4 0x8001070000008C00 MEMORY 8
REG_FIELD = CIU_SUM2_PP0_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP1_IP4 0x8001070000008C08 MEMORY 8
REG_FIELD = CIU_SUM2_PP1_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP2_IP4 0x8001070000008C10 MEMORY 8
REG_FIELD = CIU_SUM2_PP2_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4
REG = CIU_SUM2_PP3_IP4 0x8001070000008C18 MEMORY 8
REG_FIELD = CIU_SUM2_PP3_IP4 bch 19 19, agl_drp 18 18, ocla 17 17, sata 16 16, timer 9 4

REG = CIU_TIM0 0x8001070000000480 MEMORY 8
REG_FIELD = CIU_TIM0 one_shot 36 36, len 35 0
REG = CIU_TIM1 0x8001070000000488 MEMORY 8
REG_FIELD = CIU_TIM1 one_shot 36 36, len 35 0
REG = CIU_TIM2 0x8001070000000490 MEMORY 8
REG_FIELD = CIU_TIM2 one_shot 36 36, len 35 0
REG = CIU_TIM3 0x8001070000000498 MEMORY 8
REG_FIELD = CIU_TIM3 one_shot 36 36, len 35 0
REG = CIU_TIM4 0x80010700000004A0 MEMORY 8
REG_FIELD = CIU_TIM4 one_shot 36 36, len 35 0
REG = CIU_TIM5 0x80010700000004A8 MEMORY 8
REG_FIELD = CIU_TIM5 one_shot 36 36, len 35 0
REG = CIU_TIM6 0x80010700000004B0 MEMORY 8
REG_FIELD = CIU_TIM6 one_shot 36 36, len 35 0
REG = CIU_TIM7 0x80010700000004B8 MEMORY 8
REG_FIELD = CIU_TIM7 one_shot 36 36, len 35 0
REG = CIU_TIM8 0x80010700000004C0 MEMORY 8
REG_FIELD = CIU_TIM8 one_shot 36 36, len 35 0
REG = CIU_TIM9 0x80010700000004C8 MEMORY 8
REG_FIELD = CIU_TIM9 one_shot 36 36, len 35 0

REG = CIU_TIM_MULTI_CAST 0x80010700000004F0 MEMORY 8
REG_FIELD = CIU_TIM_MULTI_CAST en 0 0

REG = CIU_WDOG0 0x8001070000000500 MEMORY 8
REG_FIELD = CIU_WDOG0 gstopen 45 45, dstop 44 44, cnt 43 20, len 19 4, state 3 2, mode 1 0
REG = CIU_WDOG1 0x8001070000000508 MEMORY 8
REG_FIELD = CIU_WDOG1 gstopen 45 45, dstop 44 44, cnt 43 20, len 19 4, state 3 2, mode 1 0
REG = CIU_WDOG2 0x8001070000000510 MEMORY 8
REG_FIELD = CIU_WDOG2 gstopen 45 45, dstop 44 44, cnt 43 20, len 19 4, state 3 2, mode 1 0
REG = CIU_WDOG3 0x8001070000000518 MEMORY 8
REG_FIELD = CIU_WDOG3 gstopen 45 45, dstop 44 44, cnt 43 20, len 19 4, state 3 2, mode 1 0

REG = DFA_BIST0 0x80011800370007F0 MEMORY 8
REG_FIELD = DFA_BIST0 gfb 24 24, stx 17 16, dtx 9 8, rdf 4 4, pdb 0 0

REG = DFA_BIST1 0x80011800370007F8 MEMORY 8
REG_FIELD = DFA_BIST1 dlc0ram 19 19, ram3 12 12, ram2 11 11, ram1 10 10, crq 9 9, gutv 8 8, gutp 4 4, ncd 3 3, gif 2 2, gib 1 1, gfu 0 0

REG = DFA_CONFIG 0x8001180037000000 MEMORY 8
REG_FIELD = DFA_CONFIG dlcclear_bist 10 10, dlcstart_bist 9 9, repl_ena 8 8, clmskcrip 7 4, cldtecrip 3 1, dteclkdis 0 0

REG = DFA_CONTROL 0x8001180037000020 MEMORY 8
REG_FIELD = DFA_CONTROL sbdnum 9 6, sbdlck 5 5, pmode 2 2, qmode 1 1, imode 0 0

REG = DFA_DBELL 0x8001370000000000 MEMORY 8
REG_FIELD = DFA_DBELL dbell 19 0

REG = DFA_DEBUG0 0x8001180037000040 MEMORY 8
REG_FIELD = DFA_DEBUG0 sbd0 63 0

REG = DFA_DEBUG1 0x8001180037000048 MEMORY 8
REG_FIELD = DFA_DEBUG1 sbd1 63 0

REG = DFA_DEBUG2 0x8001180037000050 MEMORY 8
REG_FIELD = DFA_DEBUG2 sbd2 63 0

REG = DFA_DEBUG3 0x8001180037000058 MEMORY 8
REG_FIELD = DFA_DEBUG3 sbd3 63 0

REG = DFA_DIFCTL 0x8001370600000000 MEMORY 8
REG_FIELD = DFA_DIFCTL msegbase 25 20, dwbcnt 19 12, pool 11 9, size 8 0

REG = DFA_DIFRDPTR 0x8001370200000000 MEMORY 8
REG_FIELD = DFA_DIFRDPTR rdptr 39 5

REG = DFA_DTCFADR 0x8001180037000060 MEMORY 8
REG_FIELD = DFA_DTCFADR ram3fadr 43 32, ram2fadr 24 16, ram1fadr 13 0

REG = DFA_ERROR 0x8001180037000028 MEMORY 8
REG_FIELD = DFA_ERROR replerr 18 18, dfanxm 17 17, cndrd 16 16, dlc0_ovferr 13 13, dc0perr 3 1, dblovf 0 0

REG = DFA_INTMSK 0x8001180037000030 MEMORY 8
REG_FIELD = DFA_INTMSK replerrena 18 18, dfanxmena 17 17, dlc0_ovfena 13 13, dc0pena 3 1, dblina 0 0

REG = DFA_MEMHIDAT 0x8001370700000000 MEMORY 8
REG_FIELD = DFA_MEMHIDAT hidat 63 0

REG = DFA_PFC0_CNT 0x8001180037000090 MEMORY 8
REG_FIELD = DFA_PFC0_CNT pfcnt0 63 0

REG = DFA_PFC0_CTL 0x8001180037000088 MEMORY 8
REG_FIELD = DFA_PFC0_CTL evsel 13 8, cldte 5 2, clnum 1 0

REG = DFA_PFC1_CNT 0x80011800370000A0 MEMORY 8
REG_FIELD = DFA_PFC1_CNT pfcnt1 63 0

REG = DFA_PFC1_CTL 0x8001180037000098 MEMORY 8
REG_FIELD = DFA_PFC1_CTL evsel 13 8, cldte 5 2, clnum 1 0

REG = DFA_PFC2_CNT 0x80011800370000B0 MEMORY 8
REG_FIELD = DFA_PFC2_CNT pfcnt2 63 0

REG = DFA_PFC2_CTL 0x80011800370000A8 MEMORY 8
REG_FIELD = DFA_PFC2_CTL evsel 13 8, cldte 5 2, clnum 1 0

REG = DFA_PFC3_CNT 0x80011800370000C0 MEMORY 8
REG_FIELD = DFA_PFC3_CNT pfcnt3 63 0

REG = DFA_PFC3_CTL 0x80011800370000B8 MEMORY 8
REG_FIELD = DFA_PFC3_CTL evsel 13 8, cldte 5 2, clnum 1 0

REG = DFA_PFC_GCTL 0x8001180037000080 MEMORY 8
REG_FIELD = DFA_PFC_GCTL vgid 28 21, pmode 20 20, ednode 19 18, enode 17 15, snode 14 12, cnt3rclr 11 11, cnt2rclr 10 10, cnt1rclr 9 9, cnt0rclr 8 8, cnt3wclr 7 7, cnt2wclr 6 6, cnt1wclr 5 5, cnt0wclr 4 4, cnt3ena 3 3, cnt2ena 2 2, cnt1ena 1 1, cnt0ena 0 0

REG = DPI_BIST_STATUS 0x8001DF0000000000 MEMORY 8
REG_FIELD = DPI_BIST_STATUS bist 46 0

REG = DPI_CTL 0x8001DF0000000040 MEMORY 8
REG_FIELD = DPI_CTL en 0 0

REG = DPI_DMA0_COUNTS 0x8001DF0000000300 MEMORY 8
REG_FIELD = DPI_DMA0_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA1_COUNTS 0x8001DF0000000308 MEMORY 8
REG_FIELD = DPI_DMA1_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA2_COUNTS 0x8001DF0000000310 MEMORY 8
REG_FIELD = DPI_DMA2_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA3_COUNTS 0x8001DF0000000318 MEMORY 8
REG_FIELD = DPI_DMA3_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA4_COUNTS 0x8001DF0000000320 MEMORY 8
REG_FIELD = DPI_DMA4_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA5_COUNTS 0x8001DF0000000328 MEMORY 8
REG_FIELD = DPI_DMA5_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA6_COUNTS 0x8001DF0000000330 MEMORY 8
REG_FIELD = DPI_DMA6_COUNTS fcnt 38 32, dbell 31 0
REG = DPI_DMA7_COUNTS 0x8001DF0000000338 MEMORY 8
REG_FIELD = DPI_DMA7_COUNTS fcnt 38 32, dbell 31 0

REG = DPI_DMA0_DBELL 0x8001DF0000000200 MEMORY 8
REG_FIELD = DPI_DMA0_DBELL dbell 15 0
REG = DPI_DMA1_DBELL 0x8001DF0000000208 MEMORY 8
REG_FIELD = DPI_DMA1_DBELL dbell 15 0
REG = DPI_DMA2_DBELL 0x8001DF0000000210 MEMORY 8
REG_FIELD = DPI_DMA2_DBELL dbell 15 0
REG = DPI_DMA3_DBELL 0x8001DF0000000218 MEMORY 8
REG_FIELD = DPI_DMA3_DBELL dbell 15 0
REG = DPI_DMA4_DBELL 0x8001DF0000000220 MEMORY 8
REG_FIELD = DPI_DMA4_DBELL dbell 15 0
REG = DPI_DMA5_DBELL 0x8001DF0000000228 MEMORY 8
REG_FIELD = DPI_DMA5_DBELL dbell 15 0
REG = DPI_DMA6_DBELL 0x8001DF0000000230 MEMORY 8
REG_FIELD = DPI_DMA6_DBELL dbell 15 0
REG = DPI_DMA7_DBELL 0x8001DF0000000238 MEMORY 8
REG_FIELD = DPI_DMA7_DBELL dbell 15 0

REG = DPI_DMA0_ERR_RSP_STATUS 0x8001DF0000000A80 MEMORY 8
REG_FIELD = DPI_DMA0_ERR_RSP_STATUS status 5 0
REG = DPI_DMA1_ERR_RSP_STATUS 0x8001DF0000000A88 MEMORY 8
REG_FIELD = DPI_DMA1_ERR_RSP_STATUS status 5 0
REG = DPI_DMA2_ERR_RSP_STATUS 0x8001DF0000000A90 MEMORY 8
REG_FIELD = DPI_DMA2_ERR_RSP_STATUS status 5 0
REG = DPI_DMA3_ERR_RSP_STATUS 0x8001DF0000000A98 MEMORY 8
REG_FIELD = DPI_DMA3_ERR_RSP_STATUS status 5 0
REG = DPI_DMA4_ERR_RSP_STATUS 0x8001DF0000000AA0 MEMORY 8
REG_FIELD = DPI_DMA4_ERR_RSP_STATUS status 5 0
REG = DPI_DMA5_ERR_RSP_STATUS 0x8001DF0000000AA8 MEMORY 8
REG_FIELD = DPI_DMA5_ERR_RSP_STATUS status 5 0
REG = DPI_DMA6_ERR_RSP_STATUS 0x8001DF0000000AB0 MEMORY 8
REG_FIELD = DPI_DMA6_ERR_RSP_STATUS status 5 0
REG = DPI_DMA7_ERR_RSP_STATUS 0x8001DF0000000AB8 MEMORY 8
REG_FIELD = DPI_DMA7_ERR_RSP_STATUS status 5 0

REG = DPI_DMA0_IBUFF_SADDR 0x8001DF0000000280 MEMORY 8
REG_FIELD = DPI_DMA0_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA1_IBUFF_SADDR 0x8001DF0000000288 MEMORY 8
REG_FIELD = DPI_DMA1_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA2_IBUFF_SADDR 0x8001DF0000000290 MEMORY 8
REG_FIELD = DPI_DMA2_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA3_IBUFF_SADDR 0x8001DF0000000298 MEMORY 8
REG_FIELD = DPI_DMA3_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA4_IBUFF_SADDR 0x8001DF00000002A0 MEMORY 8
REG_FIELD = DPI_DMA4_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA5_IBUFF_SADDR 0x8001DF00000002A8 MEMORY 8
REG_FIELD = DPI_DMA5_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA6_IBUFF_SADDR 0x8001DF00000002B0 MEMORY 8
REG_FIELD = DPI_DMA6_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7
REG = DPI_DMA7_IBUFF_SADDR 0x8001DF00000002B8 MEMORY 8
REG_FIELD = DPI_DMA7_IBUFF_SADDR csize 61 48, idle 40 40, saddr 35 7

REG = DPI_DMA0_IFLIGHT 0x8001DF0000000A00 MEMORY 8
REG_FIELD = DPI_DMA0_IFLIGHT cnt 2 0
REG = DPI_DMA1_IFLIGHT 0x8001DF0000000A08 MEMORY 8
REG_FIELD = DPI_DMA1_IFLIGHT cnt 2 0
REG = DPI_DMA2_IFLIGHT 0x8001DF0000000A10 MEMORY 8
REG_FIELD = DPI_DMA2_IFLIGHT cnt 2 0
REG = DPI_DMA3_IFLIGHT 0x8001DF0000000A18 MEMORY 8
REG_FIELD = DPI_DMA3_IFLIGHT cnt 2 0
REG = DPI_DMA4_IFLIGHT 0x8001DF0000000A20 MEMORY 8
REG_FIELD = DPI_DMA4_IFLIGHT cnt 2 0
REG = DPI_DMA5_IFLIGHT 0x8001DF0000000A28 MEMORY 8
REG_FIELD = DPI_DMA5_IFLIGHT cnt 2 0
REG = DPI_DMA6_IFLIGHT 0x8001DF0000000A30 MEMORY 8
REG_FIELD = DPI_DMA6_IFLIGHT cnt 2 0
REG = DPI_DMA7_IFLIGHT 0x8001DF0000000A38 MEMORY 8
REG_FIELD = DPI_DMA7_IFLIGHT cnt 2 0

REG = DPI_DMA0_NADDR 0x8001DF0000000380 MEMORY 8
REG_FIELD = DPI_DMA0_NADDR addr 35 0
REG = DPI_DMA1_NADDR 0x8001DF0000000388 MEMORY 8
REG_FIELD = DPI_DMA1_NADDR addr 35 0
REG = DPI_DMA2_NADDR 0x8001DF0000000390 MEMORY 8
REG_FIELD = DPI_DMA2_NADDR addr 35 0
REG = DPI_DMA3_NADDR 0x8001DF0000000398 MEMORY 8
REG_FIELD = DPI_DMA3_NADDR addr 35 0
REG = DPI_DMA4_NADDR 0x8001DF00000003A0 MEMORY 8
REG_FIELD = DPI_DMA4_NADDR addr 35 0
REG = DPI_DMA5_NADDR 0x8001DF00000003A8 MEMORY 8
REG_FIELD = DPI_DMA5_NADDR addr 35 0
REG = DPI_DMA6_NADDR 0x8001DF00000003B0 MEMORY 8
REG_FIELD = DPI_DMA6_NADDR addr 35 0
REG = DPI_DMA7_NADDR 0x8001DF00000003B8 MEMORY 8
REG_FIELD = DPI_DMA7_NADDR addr 35 0

REG = DPI_DMA0_REQBNK0 0x8001DF0000000400 MEMORY 8
REG_FIELD = DPI_DMA0_REQBNK0 state 63 0
REG = DPI_DMA1_REQBNK0 0x8001DF0000000408 MEMORY 8
REG_FIELD = DPI_DMA1_REQBNK0 state 63 0
REG = DPI_DMA2_REQBNK0 0x8001DF0000000410 MEMORY 8
REG_FIELD = DPI_DMA2_REQBNK0 state 63 0
REG = DPI_DMA3_REQBNK0 0x8001DF0000000418 MEMORY 8
REG_FIELD = DPI_DMA3_REQBNK0 state 63 0
REG = DPI_DMA4_REQBNK0 0x8001DF0000000420 MEMORY 8
REG_FIELD = DPI_DMA4_REQBNK0 state 63 0
REG = DPI_DMA5_REQBNK0 0x8001DF0000000428 MEMORY 8
REG_FIELD = DPI_DMA5_REQBNK0 state 63 0
REG = DPI_DMA6_REQBNK0 0x8001DF0000000430 MEMORY 8
REG_FIELD = DPI_DMA6_REQBNK0 state 63 0
REG = DPI_DMA7_REQBNK0 0x8001DF0000000438 MEMORY 8
REG_FIELD = DPI_DMA7_REQBNK0 state 63 0

REG = DPI_DMA0_REQBNK1 0x8001DF0000000480 MEMORY 8
REG_FIELD = DPI_DMA0_REQBNK1 state 63 0
REG = DPI_DMA1_REQBNK1 0x8001DF0000000488 MEMORY 8
REG_FIELD = DPI_DMA1_REQBNK1 state 63 0
REG = DPI_DMA2_REQBNK1 0x8001DF0000000490 MEMORY 8
REG_FIELD = DPI_DMA2_REQBNK1 state 63 0
REG = DPI_DMA3_REQBNK1 0x8001DF0000000498 MEMORY 8
REG_FIELD = DPI_DMA3_REQBNK1 state 63 0
REG = DPI_DMA4_REQBNK1 0x8001DF00000004A0 MEMORY 8
REG_FIELD = DPI_DMA4_REQBNK1 state 63 0
REG = DPI_DMA5_REQBNK1 0x8001DF00000004A8 MEMORY 8
REG_FIELD = DPI_DMA5_REQBNK1 state 63 0
REG = DPI_DMA6_REQBNK1 0x8001DF00000004B0 MEMORY 8
REG_FIELD = DPI_DMA6_REQBNK1 state 63 0
REG = DPI_DMA7_REQBNK1 0x8001DF00000004B8 MEMORY 8
REG_FIELD = DPI_DMA7_REQBNK1 state 63 0

REG = DPI_DMA_CONTROL 0x8001DF0000000048 MEMORY 8
REG_FIELD = DPI_DMA_CONTROL dici_mode 61 61, pkt_en1 60 60, ffp_dis 59 59, commit_mode 58 58, pkt_hp 57 57, pkt_en 56 56, dma_enb 53 48, b0_lend 33 33, dwb_denb 32 32, dwb_ichk 31 23, fpa_que 22 20, o_add1 19 19, o_ro 18 18, o_ns 17 17, o_es 16 15, o_mode 14 14

REG = DPI_DMA_ENG0_EN 0x8001DF0000000080 MEMORY 8
REG_FIELD = DPI_DMA_ENG0_EN qen 7 0
REG = DPI_DMA_ENG1_EN 0x8001DF0000000088 MEMORY 8
REG_FIELD = DPI_DMA_ENG1_EN qen 7 0
REG = DPI_DMA_ENG2_EN 0x8001DF0000000090 MEMORY 8
REG_FIELD = DPI_DMA_ENG2_EN qen 7 0
REG = DPI_DMA_ENG3_EN 0x8001DF0000000098 MEMORY 8
REG_FIELD = DPI_DMA_ENG3_EN qen 7 0
REG = DPI_DMA_ENG4_EN 0x8001DF00000000A0 MEMORY 8
REG_FIELD = DPI_DMA_ENG4_EN qen 7 0
REG = DPI_DMA_ENG5_EN 0x8001DF00000000A8 MEMORY 8
REG_FIELD = DPI_DMA_ENG5_EN qen 7 0

REG = DPI_DMA_PP0_CNT 0x8001DF0000000B00 MEMORY 8
REG_FIELD = DPI_DMA_PP0_CNT cnt 15 0
REG = DPI_DMA_PP1_CNT 0x8001DF0000000B08 MEMORY 8
REG_FIELD = DPI_DMA_PP1_CNT cnt 15 0
REG = DPI_DMA_PP2_CNT 0x8001DF0000000B10 MEMORY 8
REG_FIELD = DPI_DMA_PP2_CNT cnt 15 0
REG = DPI_DMA_PP3_CNT 0x8001DF0000000B18 MEMORY 8
REG_FIELD = DPI_DMA_PP3_CNT cnt 15 0

REG = DPI_ENG0_BUF 0x8001DF0000000880 MEMORY 8
REG_FIELD = DPI_ENG0_BUF compblks 36 32, base 8 4, blks 3 0
REG = DPI_ENG1_BUF 0x8001DF0000000888 MEMORY 8
REG_FIELD = DPI_ENG1_BUF compblks 36 32, base 8 4, blks 3 0
REG = DPI_ENG2_BUF 0x8001DF0000000890 MEMORY 8
REG_FIELD = DPI_ENG2_BUF compblks 36 32, base 8 4, blks 3 0
REG = DPI_ENG3_BUF 0x8001DF0000000898 MEMORY 8
REG_FIELD = DPI_ENG3_BUF compblks 36 32, base 8 4, blks 3 0
REG = DPI_ENG4_BUF 0x8001DF00000008A0 MEMORY 8
REG_FIELD = DPI_ENG4_BUF compblks 36 32, base 8 4, blks 3 0
REG = DPI_ENG5_BUF 0x8001DF00000008A8 MEMORY 8
REG_FIELD = DPI_ENG5_BUF compblks 36 32, base 8 4, blks 3 0

REG = DPI_INFO_REG 0x8001DF0000000980 MEMORY 8
REG_FIELD = DPI_INFO_REG ffp 7 4, ncb 1 1, rsl 0 0

REG = DPI_INT_EN 0x8001DF0000000010 MEMORY 8
REG_FIELD = DPI_INT_EN sprt3_rst 27 27, sprt2_rst 26 26, sprt1_rst 25 25, sprt0_rst 24 24, req_badfil 22 22, req_inull 21 21, req_anull 20 20, req_undflw 19 19, req_ovrflw 18 18, req_badlen 17 17, req_badadr 16 16, dmadbo 15 8, nfovr 1 1, nderr 0 0

REG = DPI_INT_REG 0x8001DF0000000008 MEMORY 8
REG_FIELD = DPI_INT_REG sprt3_rst 27 27, sprt2_rst 26 26, sprt1_rst 25 25, sprt0_rst 24 24, req_badfil 22 22, req_inull 21 21, req_anull 20 20, req_undflw 19 19, req_ovrflw 18 18, req_badlen 17 17, req_badadr 16 16, dmadbo 15 8, nfovr 1 1, nderr 0 0

REG = DPI_NCB0_CFG 0x8001DF0000000800 MEMORY 8
REG_FIELD = DPI_NCB0_CFG molr 5 0

REG = DPI_PINT_INFO 0x8001DF0000000830 MEMORY 8
REG_FIELD = DPI_PINT_INFO iinfo 13 8, sinfo 5 0

REG = DPI_PKT_ERR_RSP 0x8001DF0000000078 MEMORY 8
REG_FIELD = DPI_PKT_ERR_RSP pkterr 0 0

REG = DPI_REQ_ERR_RSP 0x8001DF0000000058 MEMORY 8
REG_FIELD = DPI_REQ_ERR_RSP qerr 7 0

REG = DPI_REQ_ERR_RSP_EN 0x8001DF0000000068 MEMORY 8
REG_FIELD = DPI_REQ_ERR_RSP_EN en 7 0

REG = DPI_REQ_ERR_RST 0x8001DF0000000060 MEMORY 8
REG_FIELD = DPI_REQ_ERR_RST qerr 7 0

REG = DPI_REQ_ERR_RST_EN 0x8001DF0000000070 MEMORY 8
REG_FIELD = DPI_REQ_ERR_RST_EN en 7 0

REG = DPI_REQ_ERR_SKIP_COMP 0x8001DF0000000838 MEMORY 8
REG_FIELD = DPI_REQ_ERR_SKIP_COMP en_rst 23 16, en_rsp 7 0

REG = DPI_REQ_GBL_EN 0x8001DF0000000050 MEMORY 8
REG_FIELD = DPI_REQ_GBL_EN qen 7 0

REG = DPI_SLI_PRT0_CFG 0x8001DF0000000900 MEMORY 8
REG_FIELD = DPI_SLI_PRT0_CFG halt 24 24, rd_mode 16 16, molr 13 8, mps_lim 7 7, mps 4 4, mrrs_lim 3 3, mrrs 1 0
REG = DPI_SLI_PRT1_CFG 0x8001DF0000000908 MEMORY 8
REG_FIELD = DPI_SLI_PRT1_CFG halt 24 24, rd_mode 16 16, molr 13 8, mps_lim 7 7, mps 4 4, mrrs_lim 3 3, mrrs 1 0
REG = DPI_SLI_PRT2_CFG 0x8001DF0000000910 MEMORY 8
REG_FIELD = DPI_SLI_PRT2_CFG halt 24 24, rd_mode 16 16, molr 13 8, mps_lim 7 7, mps 4 4, mrrs_lim 3 3, mrrs 1 0

REG = DPI_SLI_PRT0_ERR 0x8001DF0000000920 MEMORY 8
REG_FIELD = DPI_SLI_PRT0_ERR addr 63 3
REG = DPI_SLI_PRT1_ERR 0x8001DF0000000928 MEMORY 8
REG_FIELD = DPI_SLI_PRT1_ERR addr 63 3
REG = DPI_SLI_PRT2_ERR 0x8001DF0000000930 MEMORY 8
REG_FIELD = DPI_SLI_PRT2_ERR addr 63 3

REG = DPI_SLI_PRT0_ERR_INFO 0x8001DF0000000940 MEMORY 8
REG_FIELD = DPI_SLI_PRT0_ERR_INFO lock 8 8, type 4 4, reqq 2 0
REG = DPI_SLI_PRT1_ERR_INFO 0x8001DF0000000948 MEMORY 8
REG_FIELD = DPI_SLI_PRT1_ERR_INFO lock 8 8, type 4 4, reqq 2 0
REG = DPI_SLI_PRT2_ERR_INFO 0x8001DF0000000950 MEMORY 8
REG_FIELD = DPI_SLI_PRT2_ERR_INFO lock 8 8, type 4 4, reqq 2 0

REG = DTX_AGL_BCST_RSP 0x80011800FE700080 MEMORY 8
REG_FIELD = DTX_AGL_BCST_RSP ena 0 0

REG = DTX_AGL_CTL 0x80011800FE700060 MEMORY 8
REG_FIELD = DTX_AGL_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_AGL_DAT0 0x80011800FE700040 MEMORY 8
REG_FIELD = DTX_AGL_DAT0 raw 35 0
REG = DTX_AGL_DAT1 0x80011800FE700048 MEMORY 8
REG_FIELD = DTX_AGL_DAT1 raw 35 0

REG = DTX_AGL_ENA0 0x80011800FE700020 MEMORY 8
REG_FIELD = DTX_AGL_ENA0 ena 35 0
REG = DTX_AGL_ENA1 0x80011800FE700028 MEMORY 8
REG_FIELD = DTX_AGL_ENA1 ena 35 0

REG = DTX_AGL_SEL0 0x80011800FE700000 MEMORY 8
REG_FIELD = DTX_AGL_SEL0 value 23 0
REG = DTX_AGL_SEL1 0x80011800FE700008 MEMORY 8
REG_FIELD = DTX_AGL_SEL1 value 23 0

REG = DTX_BROADCAST_CTL 0x80011800FE7F0060 MEMORY 8
REG_FIELD = DTX_BROADCAST_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_BROADCAST_ENA0 0x80011800FE7F0020 MEMORY 8
REG_FIELD = DTX_BROADCAST_ENA0 ena 35 0
REG = DTX_BROADCAST_ENA1 0x80011800FE7F0028 MEMORY 8
REG_FIELD = DTX_BROADCAST_ENA1 ena 35 0

REG = DTX_BROADCAST_SEL0 0x80011800FE7F0000 MEMORY 8
REG_FIELD = DTX_BROADCAST_SEL0 value 23 0
REG = DTX_BROADCAST_SEL1 0x80011800FE7F0008 MEMORY 8
REG_FIELD = DTX_BROADCAST_SEL1 value 23 0

REG = DTX_DFA_BCST_RSP 0x80011800FE1B8080 MEMORY 8
REG_FIELD = DTX_DFA_BCST_RSP ena 0 0

REG = DTX_DFA_CTL 0x80011800FE1B8060 MEMORY 8
REG_FIELD = DTX_DFA_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_DFA_DAT0 0x80011800FE1B8040 MEMORY 8
REG_FIELD = DTX_DFA_DAT0 raw 35 0
REG = DTX_DFA_DAT1 0x80011800FE1B8048 MEMORY 8
REG_FIELD = DTX_DFA_DAT1 raw 35 0

REG = DTX_DFA_ENA0 0x80011800FE1B8020 MEMORY 8
REG_FIELD = DTX_DFA_ENA0 ena 35 0
REG = DTX_DFA_ENA1 0x80011800FE1B8028 MEMORY 8
REG_FIELD = DTX_DFA_ENA1 ena 35 0

REG = DTX_DFA_SEL0 0x80011800FE1B8000 MEMORY 8
REG_FIELD = DTX_DFA_SEL0 value 23 0
REG = DTX_DFA_SEL1 0x80011800FE1B8008 MEMORY 8
REG_FIELD = DTX_DFA_SEL1 value 23 0

REG = DTX_DPI_BCST_RSP 0x80011800FEEF8080 MEMORY 8
REG_FIELD = DTX_DPI_BCST_RSP ena 0 0

REG = DTX_DPI_CTL 0x80011800FEEF8060 MEMORY 8
REG_FIELD = DTX_DPI_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_DPI_DAT0 0x80011800FEEF8040 MEMORY 8
REG_FIELD = DTX_DPI_DAT0 raw 35 0
REG = DTX_DPI_DAT1 0x80011800FEEF8048 MEMORY 8
REG_FIELD = DTX_DPI_DAT1 raw 35 0

REG = DTX_DPI_ENA0 0x80011800FEEF8020 MEMORY 8
REG_FIELD = DTX_DPI_ENA0 ena 35 0
REG = DTX_DPI_ENA1 0x80011800FEEF8028 MEMORY 8
REG_FIELD = DTX_DPI_ENA1 ena 35 0

REG = DTX_DPI_SEL0 0x80011800FEEF8000 MEMORY 8
REG_FIELD = DTX_DPI_SEL0 value 23 0
REG = DTX_DPI_SEL1 0x80011800FEEF8008 MEMORY 8
REG_FIELD = DTX_DPI_SEL1 value 23 0

REG = DTX_FPA_BCST_RSP 0x80011800FE140080 MEMORY 8
REG_FIELD = DTX_FPA_BCST_RSP ena 0 0

REG = DTX_FPA_CTL 0x80011800FE140060 MEMORY 8
REG_FIELD = DTX_FPA_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_FPA_DAT0 0x80011800FE140040 MEMORY 8
REG_FIELD = DTX_FPA_DAT0 raw 35 0
REG = DTX_FPA_DAT1 0x80011800FE140048 MEMORY 8
REG_FIELD = DTX_FPA_DAT1 raw 35 0

REG = DTX_FPA_ENA0 0x80011800FE140020 MEMORY 8
REG_FIELD = DTX_FPA_ENA0 ena 35 0
REG = DTX_FPA_ENA1 0x80011800FE140028 MEMORY 8
REG_FIELD = DTX_FPA_ENA1 ena 35 0

REG = DTX_FPA_SEL0 0x80011800FE140000 MEMORY 8
REG_FIELD = DTX_FPA_SEL0 value 23 0
REG = DTX_FPA_SEL1 0x80011800FE140008 MEMORY 8
REG_FIELD = DTX_FPA_SEL1 value 23 0

REG = DTX_GMX0_BCST_RSP 0x80011800FE040080 MEMORY 8
REG_FIELD = DTX_GMX0_BCST_RSP ena 0 0
REG = DTX_GMX1_BCST_RSP 0x80011800FE080080 MEMORY 8
REG_FIELD = DTX_GMX1_BCST_RSP ena 0 0

REG = DTX_GMX0_CTL 0x80011800FE040060 MEMORY 8
REG_FIELD = DTX_GMX0_CTL active 4 4, echoen 1 1, swap 0 0
REG = DTX_GMX1_CTL 0x80011800FE080060 MEMORY 8
REG_FIELD = DTX_GMX1_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_GMX0_DAT000 0x80011800FE040040 MEMORY 8
REG_FIELD = DTX_GMX0_DAT000 raw 35 0
REG = DTX_GMX0_DAT001 0x80011800FE040048 MEMORY 8
REG_FIELD = DTX_GMX0_DAT001 raw 35 0
REG = DTX_GMX1_DAT000 0x80011800FE080040 MEMORY 8
REG_FIELD = DTX_GMX1_DAT000 raw 35 0
REG = DTX_GMX1_DAT001 0x80011800FE080048 MEMORY 8
REG_FIELD = DTX_GMX1_DAT001 raw 35 0

REG = DTX_GMX0_ENA000 0x80011800FE040020 MEMORY 8
REG_FIELD = DTX_GMX0_ENA000 ena 35 0
REG = DTX_GMX0_ENA001 0x80011800FE040028 MEMORY 8
REG_FIELD = DTX_GMX0_ENA001 ena 35 0
REG = DTX_GMX1_ENA000 0x80011800FE080020 MEMORY 8
REG_FIELD = DTX_GMX1_ENA000 ena 35 0
REG = DTX_GMX1_ENA001 0x80011800FE080028 MEMORY 8
REG_FIELD = DTX_GMX1_ENA001 ena 35 0

REG = DTX_GMX0_SEL000 0x80011800FE040000 MEMORY 8
REG_FIELD = DTX_GMX0_SEL000 value 23 0
REG = DTX_GMX0_SEL001 0x80011800FE040008 MEMORY 8
REG_FIELD = DTX_GMX0_SEL001 value 23 0
REG = DTX_GMX1_SEL000 0x80011800FE080000 MEMORY 8
REG_FIELD = DTX_GMX1_SEL000 value 23 0
REG = DTX_GMX1_SEL001 0x80011800FE080008 MEMORY 8
REG_FIELD = DTX_GMX1_SEL001 value 23 0

REG = DTX_IOB_BCST_RSP 0x80011800FE780080 MEMORY 8
REG_FIELD = DTX_IOB_BCST_RSP ena 0 0

REG = DTX_IOB_CTL 0x80011800FE780060 MEMORY 8
REG_FIELD = DTX_IOB_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_IOB_DAT0 0x80011800FE780040 MEMORY 8
REG_FIELD = DTX_IOB_DAT0 raw 35 0
REG = DTX_IOB_DAT1 0x80011800FE780048 MEMORY 8
REG_FIELD = DTX_IOB_DAT1 raw 35 0

REG = DTX_IOB_ENA0 0x80011800FE780020 MEMORY 8
REG_FIELD = DTX_IOB_ENA0 ena 35 0
REG = DTX_IOB_ENA1 0x80011800FE780028 MEMORY 8
REG_FIELD = DTX_IOB_ENA1 ena 35 0

REG = DTX_IOB_SEL0 0x80011800FE780000 MEMORY 8
REG_FIELD = DTX_IOB_SEL0 value 23 0
REG = DTX_IOB_SEL1 0x80011800FE780008 MEMORY 8
REG_FIELD = DTX_IOB_SEL1 value 23 0

REG = DTX_IPD_BCST_RSP 0x80011800FE278080 MEMORY 8
REG_FIELD = DTX_IPD_BCST_RSP ena 0 0

REG = DTX_IPD_CTL 0x80011800FE278060 MEMORY 8
REG_FIELD = DTX_IPD_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_IPD_DAT0 0x80011800FE278040 MEMORY 8
REG_FIELD = DTX_IPD_DAT0 raw 35 0
REG = DTX_IPD_DAT1 0x80011800FE278048 MEMORY 8
REG_FIELD = DTX_IPD_DAT1 raw 35 0

REG = DTX_IPD_ENA0 0x80011800FE278020 MEMORY 8
REG_FIELD = DTX_IPD_ENA0 ena 35 0
REG = DTX_IPD_ENA1 0x80011800FE278028 MEMORY 8
REG_FIELD = DTX_IPD_ENA1 ena 35 0

REG = DTX_IPD_SEL0 0x80011800FE278000 MEMORY 8
REG_FIELD = DTX_IPD_SEL0 value 23 0
REG = DTX_IPD_SEL1 0x80011800FE278008 MEMORY 8
REG_FIELD = DTX_IPD_SEL1 value 23 0

REG = DTX_KEY_BCST_RSP 0x80011800FE100080 MEMORY 8
REG_FIELD = DTX_KEY_BCST_RSP ena 0 0

REG = DTX_KEY_CTL 0x80011800FE100060 MEMORY 8
REG_FIELD = DTX_KEY_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_KEY_DAT0 0x80011800FE100040 MEMORY 8
REG_FIELD = DTX_KEY_DAT0 raw 35 0
REG = DTX_KEY_DAT1 0x80011800FE100048 MEMORY 8
REG_FIELD = DTX_KEY_DAT1 raw 35 0

REG = DTX_KEY_ENA0 0x80011800FE100020 MEMORY 8
REG_FIELD = DTX_KEY_ENA0 ena 35 0
REG = DTX_KEY_ENA1 0x80011800FE100028 MEMORY 8
REG_FIELD = DTX_KEY_ENA1 ena 35 0

REG = DTX_KEY_SEL0 0x80011800FE100000 MEMORY 8
REG_FIELD = DTX_KEY_SEL0 value 23 0
REG = DTX_KEY_SEL1 0x80011800FE100008 MEMORY 8
REG_FIELD = DTX_KEY_SEL1 value 23 0

REG = DTX_L2C_CBC0_BCST_RSP 0x80011800FE420080 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_BCST_RSP ena 0 0

REG = DTX_L2C_CBC0_CTL 0x80011800FE420060 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_L2C_CBC0_DAT000 0x80011800FE420040 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_DAT000 raw 35 0
REG = DTX_L2C_CBC0_DAT001 0x80011800FE420048 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_DAT001 raw 35 0

REG = DTX_L2C_CBC0_ENA000 0x80011800FE420020 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_ENA000 ena 35 0
REG = DTX_L2C_CBC0_ENA001 0x80011800FE420028 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_ENA001 ena 35 0

REG = DTX_L2C_CBC0_SEL000 0x80011800FE420000 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_SEL000 value 23 0
REG = DTX_L2C_CBC0_SEL001 0x80011800FE420008 MEMORY 8
REG_FIELD = DTX_L2C_CBC0_SEL001 value 23 0

REG = DTX_L2C_MCI0_BCST_RSP 0x80011800FE2E0080 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_BCST_RSP ena 0 0

REG = DTX_L2C_MCI0_CTL 0x80011800FE2E0060 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_L2C_MCI0_DAT000 0x80011800FE2E0040 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_DAT000 raw 35 0
REG = DTX_L2C_MCI0_DAT001 0x80011800FE2E0048 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_DAT001 raw 35 0

REG = DTX_L2C_MCI0_ENA000 0x80011800FE2E0020 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_ENA000 ena 35 0
REG = DTX_L2C_MCI0_ENA001 0x80011800FE2E0028 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_ENA001 ena 35 0

REG = DTX_L2C_MCI0_SEL000 0x80011800FE2E0000 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_SEL000 value 23 0
REG = DTX_L2C_MCI0_SEL001 0x80011800FE2E0008 MEMORY 8
REG_FIELD = DTX_L2C_MCI0_SEL001 value 23 0

REG = DTX_L2C_TAD0_BCST_RSP 0x80011800FE240080 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_BCST_RSP ena 0 0

REG = DTX_L2C_TAD0_CTL 0x80011800FE240060 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_L2C_TAD0_DAT000 0x80011800FE240040 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_DAT000 raw 35 0
REG = DTX_L2C_TAD0_DAT001 0x80011800FE240048 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_DAT001 raw 35 0

REG = DTX_L2C_TAD0_ENA000 0x80011800FE240020 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_ENA000 ena 35 0
REG = DTX_L2C_TAD0_ENA001 0x80011800FE240028 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_ENA001 ena 35 0

REG = DTX_L2C_TAD0_SEL000 0x80011800FE240000 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_SEL000 value 23 0
REG = DTX_L2C_TAD0_SEL001 0x80011800FE240008 MEMORY 8
REG_FIELD = DTX_L2C_TAD0_SEL001 value 23 0

REG = DTX_LMC0_BCST_RSP 0x80011800FE440080 MEMORY 8
REG_FIELD = DTX_LMC0_BCST_RSP ena 0 0

REG = DTX_LMC0_CTL 0x80011800FE440060 MEMORY 8
REG_FIELD = DTX_LMC0_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_LMC0_DAT000 0x80011800FE440040 MEMORY 8
REG_FIELD = DTX_LMC0_DAT000 raw 35 0
REG = DTX_LMC0_DAT001 0x80011800FE440048 MEMORY 8
REG_FIELD = DTX_LMC0_DAT001 raw 35 0

REG = DTX_LMC0_ENA000 0x80011800FE440020 MEMORY 8
REG_FIELD = DTX_LMC0_ENA000 ena 35 0
REG = DTX_LMC0_ENA001 0x80011800FE440028 MEMORY 8
REG_FIELD = DTX_LMC0_ENA001 ena 35 0

REG = DTX_LMC0_SEL000 0x80011800FE440000 MEMORY 8
REG_FIELD = DTX_LMC0_SEL000 value 23 0
REG = DTX_LMC0_SEL001 0x80011800FE440008 MEMORY 8
REG_FIELD = DTX_LMC0_SEL001 value 23 0

REG = DTX_MIO_BCST_RSP 0x80011800FE000080 MEMORY 8
REG_FIELD = DTX_MIO_BCST_RSP ena 0 0

REG = DTX_MIO_CTL 0x80011800FE000060 MEMORY 8
REG_FIELD = DTX_MIO_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_MIO_DAT0 0x80011800FE000040 MEMORY 8
REG_FIELD = DTX_MIO_DAT0 raw 35 0
REG = DTX_MIO_DAT1 0x80011800FE000048 MEMORY 8
REG_FIELD = DTX_MIO_DAT1 raw 35 0

REG = DTX_MIO_ENA0 0x80011800FE000020 MEMORY 8
REG_FIELD = DTX_MIO_ENA0 ena 35 0
REG = DTX_MIO_ENA1 0x80011800FE000028 MEMORY 8
REG_FIELD = DTX_MIO_ENA1 ena 35 0

REG = DTX_MIO_SEL0 0x80011800FE000000 MEMORY 8
REG_FIELD = DTX_MIO_SEL0 value 23 0
REG = DTX_MIO_SEL1 0x80011800FE000008 MEMORY 8
REG_FIELD = DTX_MIO_SEL1 value 23 0

REG = DTX_PCS0_BCST_RSP 0x80011800FE580080 MEMORY 8
REG_FIELD = DTX_PCS0_BCST_RSP ena 0 0
REG = DTX_PCS1_BCST_RSP 0x80011800FE5C0080 MEMORY 8
REG_FIELD = DTX_PCS1_BCST_RSP ena 0 0

REG = DTX_PCS0_CTL 0x80011800FE580060 MEMORY 8
REG_FIELD = DTX_PCS0_CTL active 4 4, echoen 1 1, swap 0 0
REG = DTX_PCS1_CTL 0x80011800FE5C0060 MEMORY 8
REG_FIELD = DTX_PCS1_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_PCS0_DAT000 0x80011800FE580040 MEMORY 8
REG_FIELD = DTX_PCS0_DAT000 raw 35 0
REG = DTX_PCS0_DAT001 0x80011800FE580048 MEMORY 8
REG_FIELD = DTX_PCS0_DAT001 raw 35 0
REG = DTX_PCS1_DAT000 0x80011800FE5C0040 MEMORY 8
REG_FIELD = DTX_PCS1_DAT000 raw 35 0
REG = DTX_PCS1_DAT001 0x80011800FE5C0048 MEMORY 8
REG_FIELD = DTX_PCS1_DAT001 raw 35 0

REG = DTX_PCS0_ENA000 0x80011800FE580020 MEMORY 8
REG_FIELD = DTX_PCS0_ENA000 ena 35 0
REG = DTX_PCS0_ENA001 0x80011800FE580028 MEMORY 8
REG_FIELD = DTX_PCS0_ENA001 ena 35 0
REG = DTX_PCS1_ENA000 0x80011800FE5C0020 MEMORY 8
REG_FIELD = DTX_PCS1_ENA000 ena 35 0
REG = DTX_PCS1_ENA001 0x80011800FE5C0028 MEMORY 8
REG_FIELD = DTX_PCS1_ENA001 ena 35 0

REG = DTX_PCS0_SEL000 0x80011800FE580000 MEMORY 8
REG_FIELD = DTX_PCS0_SEL000 value 23 0
REG = DTX_PCS0_SEL001 0x80011800FE580008 MEMORY 8
REG_FIELD = DTX_PCS0_SEL001 value 23 0
REG = DTX_PCS1_SEL000 0x80011800FE5C0000 MEMORY 8
REG_FIELD = DTX_PCS1_SEL000 value 23 0
REG = DTX_PCS1_SEL001 0x80011800FE5C0008 MEMORY 8
REG_FIELD = DTX_PCS1_SEL001 value 23 0

REG = DTX_PEM0_BCST_RSP 0x80011800FE600080 MEMORY 8
REG_FIELD = DTX_PEM0_BCST_RSP ena 0 0
REG = DTX_PEM1_BCST_RSP 0x80011800FE608080 MEMORY 8
REG_FIELD = DTX_PEM1_BCST_RSP ena 0 0
REG = DTX_PEM2_BCST_RSP 0x80011800FE610080 MEMORY 8
REG_FIELD = DTX_PEM2_BCST_RSP ena 0 0

REG = DTX_PEM0_CTL 0x80011800FE600060 MEMORY 8
REG_FIELD = DTX_PEM0_CTL active 4 4, echoen 1 1, swap 0 0
REG = DTX_PEM1_CTL 0x80011800FE608060 MEMORY 8
REG_FIELD = DTX_PEM1_CTL active 4 4, echoen 1 1, swap 0 0
REG = DTX_PEM2_CTL 0x80011800FE610060 MEMORY 8
REG_FIELD = DTX_PEM2_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_PEM0_DAT000 0x80011800FE600040 MEMORY 8
REG_FIELD = DTX_PEM0_DAT000 raw 35 0
REG = DTX_PEM0_DAT001 0x80011800FE600048 MEMORY 8
REG_FIELD = DTX_PEM0_DAT001 raw 35 0
REG = DTX_PEM1_DAT000 0x80011800FE608040 MEMORY 8
REG_FIELD = DTX_PEM1_DAT000 raw 35 0
REG = DTX_PEM1_DAT001 0x80011800FE608048 MEMORY 8
REG_FIELD = DTX_PEM1_DAT001 raw 35 0
REG = DTX_PEM2_DAT000 0x80011800FE610040 MEMORY 8
REG_FIELD = DTX_PEM2_DAT000 raw 35 0
REG = DTX_PEM2_DAT001 0x80011800FE610048 MEMORY 8
REG_FIELD = DTX_PEM2_DAT001 raw 35 0

REG = DTX_PEM0_ENA000 0x80011800FE600020 MEMORY 8
REG_FIELD = DTX_PEM0_ENA000 ena 35 0
REG = DTX_PEM0_ENA001 0x80011800FE600028 MEMORY 8
REG_FIELD = DTX_PEM0_ENA001 ena 35 0
REG = DTX_PEM1_ENA000 0x80011800FE608020 MEMORY 8
REG_FIELD = DTX_PEM1_ENA000 ena 35 0
REG = DTX_PEM1_ENA001 0x80011800FE608028 MEMORY 8
REG_FIELD = DTX_PEM1_ENA001 ena 35 0
REG = DTX_PEM2_ENA000 0x80011800FE610020 MEMORY 8
REG_FIELD = DTX_PEM2_ENA000 ena 35 0
REG = DTX_PEM2_ENA001 0x80011800FE610028 MEMORY 8
REG_FIELD = DTX_PEM2_ENA001 ena 35 0

REG = DTX_PEM0_SEL000 0x80011800FE600000 MEMORY 8
REG_FIELD = DTX_PEM0_SEL000 value 23 0
REG = DTX_PEM0_SEL001 0x80011800FE600008 MEMORY 8
REG_FIELD = DTX_PEM0_SEL001 value 23 0
REG = DTX_PEM1_SEL000 0x80011800FE608000 MEMORY 8
REG_FIELD = DTX_PEM1_SEL000 value 23 0
REG = DTX_PEM1_SEL001 0x80011800FE608008 MEMORY 8
REG_FIELD = DTX_PEM1_SEL001 value 23 0
REG = DTX_PEM2_SEL000 0x80011800FE610000 MEMORY 8
REG_FIELD = DTX_PEM2_SEL000 value 23 0
REG = DTX_PEM2_SEL001 0x80011800FE610008 MEMORY 8
REG_FIELD = DTX_PEM2_SEL001 value 23 0

REG = DTX_PIP_BCST_RSP 0x80011800FE500080 MEMORY 8
REG_FIELD = DTX_PIP_BCST_RSP ena 0 0

REG = DTX_PIP_CTL 0x80011800FE500060 MEMORY 8
REG_FIELD = DTX_PIP_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_PIP_DAT0 0x80011800FE500040 MEMORY 8
REG_FIELD = DTX_PIP_DAT0 raw 35 0
REG = DTX_PIP_DAT1 0x80011800FE500048 MEMORY 8
REG_FIELD = DTX_PIP_DAT1 raw 35 0

REG = DTX_PIP_ENA0 0x80011800FE500020 MEMORY 8
REG_FIELD = DTX_PIP_ENA0 ena 35 0
REG = DTX_PIP_ENA1 0x80011800FE500028 MEMORY 8
REG_FIELD = DTX_PIP_ENA1 ena 35 0

REG = DTX_PIP_SEL0 0x80011800FE500000 MEMORY 8
REG_FIELD = DTX_PIP_SEL0 value 23 0
REG = DTX_PIP_SEL1 0x80011800FE500008 MEMORY 8
REG_FIELD = DTX_PIP_SEL1 value 23 0

REG = DTX_PKO_BCST_RSP 0x80011800FE280080 MEMORY 8
REG_FIELD = DTX_PKO_BCST_RSP ena 0 0

REG = DTX_PKO_CTL 0x80011800FE280060 MEMORY 8
REG_FIELD = DTX_PKO_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_PKO_DAT0 0x80011800FE280040 MEMORY 8
REG_FIELD = DTX_PKO_DAT0 raw 35 0
REG = DTX_PKO_DAT1 0x80011800FE280048 MEMORY 8
REG_FIELD = DTX_PKO_DAT1 raw 35 0

REG = DTX_PKO_ENA0 0x80011800FE280020 MEMORY 8
REG_FIELD = DTX_PKO_ENA0 ena 35 0
REG = DTX_PKO_ENA1 0x80011800FE280028 MEMORY 8
REG_FIELD = DTX_PKO_ENA1 ena 35 0

REG = DTX_PKO_SEL0 0x80011800FE280000 MEMORY 8
REG_FIELD = DTX_PKO_SEL0 value 23 0
REG = DTX_PKO_SEL1 0x80011800FE280008 MEMORY 8
REG_FIELD = DTX_PKO_SEL1 value 23 0

REG = DTX_POW_BCST_RSP 0x80011800FE338080 MEMORY 8
REG_FIELD = DTX_POW_BCST_RSP ena 0 0

REG = DTX_POW_CTL 0x80011800FE338060 MEMORY 8
REG_FIELD = DTX_POW_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_POW_DAT0 0x80011800FE338040 MEMORY 8
REG_FIELD = DTX_POW_DAT0 raw 35 0
REG = DTX_POW_DAT1 0x80011800FE338048 MEMORY 8
REG_FIELD = DTX_POW_DAT1 raw 35 0

REG = DTX_POW_ENA0 0x80011800FE338020 MEMORY 8
REG_FIELD = DTX_POW_ENA0 ena 35 0
REG = DTX_POW_ENA1 0x80011800FE338028 MEMORY 8
REG_FIELD = DTX_POW_ENA1 ena 35 0

REG = DTX_POW_SEL0 0x80011800FE338000 MEMORY 8
REG_FIELD = DTX_POW_SEL0 value 23 0
REG = DTX_POW_SEL1 0x80011800FE338008 MEMORY 8
REG_FIELD = DTX_POW_SEL1 value 23 0

REG = DTX_RST_BCST_RSP 0x80011800FE030080 MEMORY 8
REG_FIELD = DTX_RST_BCST_RSP ena 0 0

REG = DTX_RST_CTL 0x80011800FE030060 MEMORY 8
REG_FIELD = DTX_RST_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_RST_DAT0 0x80011800FE030040 MEMORY 8
REG_FIELD = DTX_RST_DAT0 raw 35 0
REG = DTX_RST_DAT1 0x80011800FE030048 MEMORY 8
REG_FIELD = DTX_RST_DAT1 raw 35 0

REG = DTX_RST_ENA0 0x80011800FE030020 MEMORY 8
REG_FIELD = DTX_RST_ENA0 ena 35 0
REG = DTX_RST_ENA1 0x80011800FE030028 MEMORY 8
REG_FIELD = DTX_RST_ENA1 ena 35 0

REG = DTX_RST_SEL0 0x80011800FE030000 MEMORY 8
REG_FIELD = DTX_RST_SEL0 value 23 0
REG = DTX_RST_SEL1 0x80011800FE030008 MEMORY 8
REG_FIELD = DTX_RST_SEL1 value 23 0

REG = DTX_SATA_BCST_RSP 0x80011800FE360080 MEMORY 8
REG_FIELD = DTX_SATA_BCST_RSP ena 0 0

REG = DTX_SATA_CTL 0x80011800FE360060 MEMORY 8
REG_FIELD = DTX_SATA_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_SATA_DAT0 0x80011800FE360040 MEMORY 8
REG_FIELD = DTX_SATA_DAT0 raw 35 0
REG = DTX_SATA_DAT1 0x80011800FE360048 MEMORY 8
REG_FIELD = DTX_SATA_DAT1 raw 35 0

REG = DTX_SATA_ENA0 0x80011800FE360020 MEMORY 8
REG_FIELD = DTX_SATA_ENA0 ena 35 0
REG = DTX_SATA_ENA1 0x80011800FE360028 MEMORY 8
REG_FIELD = DTX_SATA_ENA1 ena 35 0

REG = DTX_SATA_SEL0 0x80011800FE360000 MEMORY 8
REG_FIELD = DTX_SATA_SEL0 value 23 0
REG = DTX_SATA_SEL1 0x80011800FE360008 MEMORY 8
REG_FIELD = DTX_SATA_SEL1 value 23 0

REG = DTX_SLI_BCST_RSP 0x80011800FE8F8080 MEMORY 8
REG_FIELD = DTX_SLI_BCST_RSP ena 0 0

REG = DTX_SLI_CTL 0x80011800FE8F8060 MEMORY 8
REG_FIELD = DTX_SLI_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_SLI_DAT0 0x80011800FE8F8040 MEMORY 8
REG_FIELD = DTX_SLI_DAT0 raw 35 0
REG = DTX_SLI_DAT1 0x80011800FE8F8048 MEMORY 8
REG_FIELD = DTX_SLI_DAT1 raw 35 0

REG = DTX_SLI_ENA0 0x80011800FE8F8020 MEMORY 8
REG_FIELD = DTX_SLI_ENA0 ena 35 0
REG = DTX_SLI_ENA1 0x80011800FE8F8028 MEMORY 8
REG_FIELD = DTX_SLI_ENA1 ena 35 0

REG = DTX_SLI_SEL0 0x80011800FE8F8000 MEMORY 8
REG_FIELD = DTX_SLI_SEL0 value 23 0
REG = DTX_SLI_SEL1 0x80011800FE8F8008 MEMORY 8
REG_FIELD = DTX_SLI_SEL1 value 23 0

REG = DTX_TIM_BCST_RSP 0x80011800FE2C0080 MEMORY 8
REG_FIELD = DTX_TIM_BCST_RSP ena 0 0

REG = DTX_TIM_CTL 0x80011800FE2C0060 MEMORY 8
REG_FIELD = DTX_TIM_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_TIM_DAT0 0x80011800FE2C0040 MEMORY 8
REG_FIELD = DTX_TIM_DAT0 raw 35 0
REG = DTX_TIM_DAT1 0x80011800FE2C0048 MEMORY 8
REG_FIELD = DTX_TIM_DAT1 raw 35 0

REG = DTX_TIM_ENA0 0x80011800FE2C0020 MEMORY 8
REG_FIELD = DTX_TIM_ENA0 ena 35 0
REG = DTX_TIM_ENA1 0x80011800FE2C0028 MEMORY 8
REG_FIELD = DTX_TIM_ENA1 ena 35 0

REG = DTX_TIM_SEL0 0x80011800FE2C0000 MEMORY 8
REG_FIELD = DTX_TIM_SEL0 value 23 0
REG = DTX_TIM_SEL1 0x80011800FE2C0008 MEMORY 8
REG_FIELD = DTX_TIM_SEL1 value 23 0

REG = DTX_USBDRD0_BCST_RSP 0x80011800FE340080 MEMORY 8
REG_FIELD = DTX_USBDRD0_BCST_RSP ena 0 0
REG = DTX_USBDRD1_BCST_RSP 0x80011800FE348080 MEMORY 8
REG_FIELD = DTX_USBDRD1_BCST_RSP ena 0 0

REG = DTX_USBDRD0_CTL 0x80011800FE340060 MEMORY 8
REG_FIELD = DTX_USBDRD0_CTL active 4 4, echoen 1 1, swap 0 0
REG = DTX_USBDRD1_CTL 0x80011800FE348060 MEMORY 8
REG_FIELD = DTX_USBDRD1_CTL active 4 4, echoen 1 1, swap 0 0

REG = DTX_USBDRD0_DAT000 0x80011800FE340040 MEMORY 8
REG_FIELD = DTX_USBDRD0_DAT000 raw 35 0
REG = DTX_USBDRD0_DAT001 0x80011800FE340048 MEMORY 8
REG_FIELD = DTX_USBDRD0_DAT001 raw 35 0
REG = DTX_USBDRD1_DAT000 0x80011800FE348040 MEMORY 8
REG_FIELD = DTX_USBDRD1_DAT000 raw 35 0
REG = DTX_USBDRD1_DAT001 0x80011800FE348048 MEMORY 8
REG_FIELD = DTX_USBDRD1_DAT001 raw 35 0

REG = DTX_USBDRD0_ENA000 0x80011800FE340020 MEMORY 8
REG_FIELD = DTX_USBDRD0_ENA000 ena 35 0
REG = DTX_USBDRD0_ENA001 0x80011800FE340028 MEMORY 8
REG_FIELD = DTX_USBDRD0_ENA001 ena 35 0
REG = DTX_USBDRD1_ENA000 0x80011800FE348020 MEMORY 8
REG_FIELD = DTX_USBDRD1_ENA000 ena 35 0
REG = DTX_USBDRD1_ENA001 0x80011800FE348028 MEMORY 8
REG_FIELD = DTX_USBDRD1_ENA001 ena 35 0

REG = DTX_USBDRD0_SEL000 0x80011800FE340000 MEMORY 8
REG_FIELD = DTX_USBDRD0_SEL000 value 23 0
REG = DTX_USBDRD0_SEL001 0x80011800FE340008 MEMORY 8
REG_FIELD = DTX_USBDRD0_SEL001 value 23 0
REG = DTX_USBDRD1_SEL000 0x80011800FE348000 MEMORY 8
REG_FIELD = DTX_USBDRD1_SEL000 value 23 0
REG = DTX_USBDRD1_SEL001 0x80011800FE348008 MEMORY 8
REG_FIELD = DTX_USBDRD1_SEL001 value 23 0

REG = FPA_ADDR_RANGE_ERROR 0x8001180028000458 MEMORY 8
REG_FIELD = FPA_ADDR_RANGE_ERROR pool 37 33, addr 32 0

REG = FPA_BIST_STATUS 0x80011800280000E8 MEMORY 8
REG_FIELD = FPA_BIST_STATUS frd 4 4, fpf0 3 3, fpf1 2 2, ffr 1 1, fdr 0 0

REG = FPA_CTL_STATUS 0x8001180028000050 MEMORY 8
REG_FIELD = FPA_CTL_STATUS free_en 20 20, ret_off 19 19, req_off 18 18, reset 17 17, use_ldt 16 16, use_stt 15 15, enb 14 14, mem1_err 13 7, mem0_err 6 0

REG = FPA_FPF1_MARKS 0x8001180028000008 MEMORY 8
REG_FIELD = FPA_FPF1_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF2_MARKS 0x8001180028000010 MEMORY 8
REG_FIELD = FPA_FPF2_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF3_MARKS 0x8001180028000018 MEMORY 8
REG_FIELD = FPA_FPF3_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF4_MARKS 0x8001180028000020 MEMORY 8
REG_FIELD = FPA_FPF4_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF5_MARKS 0x8001180028000028 MEMORY 8
REG_FIELD = FPA_FPF5_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF6_MARKS 0x8001180028000030 MEMORY 8
REG_FIELD = FPA_FPF6_MARKS fpf_wr 21 11, fpf_rd 10 0
REG = FPA_FPF7_MARKS 0x8001180028000038 MEMORY 8
REG_FIELD = FPA_FPF7_MARKS fpf_wr 21 11, fpf_rd 10 0

REG = FPA_FPF1_SIZE 0x8001180028000060 MEMORY 8
REG_FIELD = FPA_FPF1_SIZE fpf_siz 10 0
REG = FPA_FPF2_SIZE 0x8001180028000068 MEMORY 8
REG_FIELD = FPA_FPF2_SIZE fpf_siz 10 0
REG = FPA_FPF3_SIZE 0x8001180028000070 MEMORY 8
REG_FIELD = FPA_FPF3_SIZE fpf_siz 10 0
REG = FPA_FPF4_SIZE 0x8001180028000078 MEMORY 8
REG_FIELD = FPA_FPF4_SIZE fpf_siz 10 0
REG = FPA_FPF5_SIZE 0x8001180028000080 MEMORY 8
REG_FIELD = FPA_FPF5_SIZE fpf_siz 10 0
REG = FPA_FPF6_SIZE 0x8001180028000088 MEMORY 8
REG_FIELD = FPA_FPF6_SIZE fpf_siz 10 0
REG = FPA_FPF7_SIZE 0x8001180028000090 MEMORY 8
REG_FIELD = FPA_FPF7_SIZE fpf_siz 10 0

REG = FPA_FPF0_MARKS 0x8001180028000000 MEMORY 8
REG_FIELD = FPA_FPF0_MARKS fpf_wr 23 12, fpf_rd 11 0

REG = FPA_FPF0_SIZE 0x8001180028000058 MEMORY 8
REG_FIELD = FPA_FPF0_SIZE fpf_siz 11 0

REG = FPA_INT_ENB 0x8001180028000048 MEMORY 8
REG_FIELD = FPA_INT_ENB paddr_e 49 49, res_44 48 44, free7 43 43, free6 42 42, free5 41 41, free4 40 40, free3 39 39, free2 38 38, free1 37 37, free0 36 36, pool7th 35 35, pool6th 34 34, pool5th 33 33, pool4th 32 32, pool3th 31 31, pool2th 30 30, pool1th 29 29, pool0th 28 28, q7_perr 27 27, q7_coff 26 26, q7_und 25 25, q6_perr 24 24, q6_coff 23 23, q6_und 22 22, q5_perr 21 21, q5_coff 20 20, q5_und 19 19, q4_perr 18 18, q4_coff 17 17, q4_und 16 16, q3_perr 15 15, q3_coff 14 14, q3_und 13 13, q2_perr 12 12, q2_coff 11 11, q2_und 10 10, q1_perr 9 9, q1_coff 8 8, q1_und 7 7, q0_perr 6 6, q0_coff 5 5, q0_und 4 4, fed1_dbe 3 3, fed1_sbe 2 2, fed0_dbe 1 1, fed0_sbe 0 0

REG = FPA_INT_SUM 0x8001180028000040 MEMORY 8
REG_FIELD = FPA_INT_SUM paddr_e 49 49, free7 43 43, free6 42 42, free5 41 41, free4 40 40, free3 39 39, free2 38 38, free1 37 37, free0 36 36, pool7th 35 35, pool6th 34 34, pool5th 33 33, pool4th 32 32, pool3th 31 31, pool2th 30 30, pool1th 29 29, pool0th 28 28, q7_perr 27 27, q7_coff 26 26, q7_und 25 25, q6_perr 24 24, q6_coff 23 23, q6_und 22 22, q5_perr 21 21, q5_coff 20 20, q5_und 19 19, q4_perr 18 18, q4_coff 17 17, q4_und 16 16, q3_perr 15 15, q3_coff 14 14, q3_und 13 13, q2_perr 12 12, q2_coff 11 11, q2_und 10 10, q1_perr 9 9, q1_coff 8 8, q1_und 7 7, q0_perr 6 6, q0_coff 5 5, q0_und 4 4, fed1_dbe 3 3, fed1_sbe 2 2, fed0_dbe 1 1, fed0_sbe 0 0

REG = FPA_PACKET_THRESHOLD 0x8001180028000460 MEMORY 8
REG_FIELD = FPA_PACKET_THRESHOLD thresh 31 0

REG = FPA_POOL0_END_ADDR 0x8001180028000358 MEMORY 8
REG_FIELD = FPA_POOL0_END_ADDR addr 32 0
REG = FPA_POOL1_END_ADDR 0x8001180028000360 MEMORY 8
REG_FIELD = FPA_POOL1_END_ADDR addr 32 0
REG = FPA_POOL2_END_ADDR 0x8001180028000368 MEMORY 8
REG_FIELD = FPA_POOL2_END_ADDR addr 32 0
REG = FPA_POOL3_END_ADDR 0x8001180028000370 MEMORY 8
REG_FIELD = FPA_POOL3_END_ADDR addr 32 0
REG = FPA_POOL4_END_ADDR 0x8001180028000378 MEMORY 8
REG_FIELD = FPA_POOL4_END_ADDR addr 32 0
REG = FPA_POOL5_END_ADDR 0x8001180028000380 MEMORY 8
REG_FIELD = FPA_POOL5_END_ADDR addr 32 0
REG = FPA_POOL6_END_ADDR 0x8001180028000388 MEMORY 8
REG_FIELD = FPA_POOL6_END_ADDR addr 32 0
REG = FPA_POOL7_END_ADDR 0x8001180028000390 MEMORY 8
REG_FIELD = FPA_POOL7_END_ADDR addr 32 0

REG = FPA_POOL0_START_ADDR 0x8001180028000258 MEMORY 8
REG_FIELD = FPA_POOL0_START_ADDR addr 32 0
REG = FPA_POOL1_START_ADDR 0x8001180028000260 MEMORY 8
REG_FIELD = FPA_POOL1_START_ADDR addr 32 0
REG = FPA_POOL2_START_ADDR 0x8001180028000268 MEMORY 8
REG_FIELD = FPA_POOL2_START_ADDR addr 32 0
REG = FPA_POOL3_START_ADDR 0x8001180028000270 MEMORY 8
REG_FIELD = FPA_POOL3_START_ADDR addr 32 0
REG = FPA_POOL4_START_ADDR 0x8001180028000278 MEMORY 8
REG_FIELD = FPA_POOL4_START_ADDR addr 32 0
REG = FPA_POOL5_START_ADDR 0x8001180028000280 MEMORY 8
REG_FIELD = FPA_POOL5_START_ADDR addr 32 0
REG = FPA_POOL6_START_ADDR 0x8001180028000288 MEMORY 8
REG_FIELD = FPA_POOL6_START_ADDR addr 32 0
REG = FPA_POOL7_START_ADDR 0x8001180028000290 MEMORY 8
REG_FIELD = FPA_POOL7_START_ADDR addr 32 0

REG = FPA_POOL0_THRESHOLD 0x8001180028000140 MEMORY 8
REG_FIELD = FPA_POOL0_THRESHOLD thresh 28 0
REG = FPA_POOL1_THRESHOLD 0x8001180028000148 MEMORY 8
REG_FIELD = FPA_POOL1_THRESHOLD thresh 28 0
REG = FPA_POOL2_THRESHOLD 0x8001180028000150 MEMORY 8
REG_FIELD = FPA_POOL2_THRESHOLD thresh 28 0
REG = FPA_POOL3_THRESHOLD 0x8001180028000158 MEMORY 8
REG_FIELD = FPA_POOL3_THRESHOLD thresh 28 0
REG = FPA_POOL4_THRESHOLD 0x8001180028000160 MEMORY 8
REG_FIELD = FPA_POOL4_THRESHOLD thresh 28 0
REG = FPA_POOL5_THRESHOLD 0x8001180028000168 MEMORY 8
REG_FIELD = FPA_POOL5_THRESHOLD thresh 28 0
REG = FPA_POOL6_THRESHOLD 0x8001180028000170 MEMORY 8
REG_FIELD = FPA_POOL6_THRESHOLD thresh 28 0
REG = FPA_POOL7_THRESHOLD 0x8001180028000178 MEMORY 8
REG_FIELD = FPA_POOL7_THRESHOLD thresh 28 0

REG = FPA_QUE0_AVAILABLE 0x8001180028000098 MEMORY 8
REG_FIELD = FPA_QUE0_AVAILABLE que_siz 28 0
REG = FPA_QUE1_AVAILABLE 0x80011800280000A0 MEMORY 8
REG_FIELD = FPA_QUE1_AVAILABLE que_siz 28 0
REG = FPA_QUE2_AVAILABLE 0x80011800280000A8 MEMORY 8
REG_FIELD = FPA_QUE2_AVAILABLE que_siz 28 0
REG = FPA_QUE3_AVAILABLE 0x80011800280000B0 MEMORY 8
REG_FIELD = FPA_QUE3_AVAILABLE que_siz 28 0
REG = FPA_QUE4_AVAILABLE 0x80011800280000B8 MEMORY 8
REG_FIELD = FPA_QUE4_AVAILABLE que_siz 28 0
REG = FPA_QUE5_AVAILABLE 0x80011800280000C0 MEMORY 8
REG_FIELD = FPA_QUE5_AVAILABLE que_siz 28 0
REG = FPA_QUE6_AVAILABLE 0x80011800280000C8 MEMORY 8
REG_FIELD = FPA_QUE6_AVAILABLE que_siz 28 0
REG = FPA_QUE7_AVAILABLE 0x80011800280000D0 MEMORY 8
REG_FIELD = FPA_QUE7_AVAILABLE que_siz 28 0

REG = FPA_QUE0_PAGE_INDEX 0x80011800280000F0 MEMORY 8
REG_FIELD = FPA_QUE0_PAGE_INDEX pg_num 24 0
REG = FPA_QUE1_PAGE_INDEX 0x80011800280000F8 MEMORY 8
REG_FIELD = FPA_QUE1_PAGE_INDEX pg_num 24 0
REG = FPA_QUE2_PAGE_INDEX 0x8001180028000100 MEMORY 8
REG_FIELD = FPA_QUE2_PAGE_INDEX pg_num 24 0
REG = FPA_QUE3_PAGE_INDEX 0x8001180028000108 MEMORY 8
REG_FIELD = FPA_QUE3_PAGE_INDEX pg_num 24 0
REG = FPA_QUE4_PAGE_INDEX 0x8001180028000110 MEMORY 8
REG_FIELD = FPA_QUE4_PAGE_INDEX pg_num 24 0
REG = FPA_QUE5_PAGE_INDEX 0x8001180028000118 MEMORY 8
REG_FIELD = FPA_QUE5_PAGE_INDEX pg_num 24 0
REG = FPA_QUE6_PAGE_INDEX 0x8001180028000120 MEMORY 8
REG_FIELD = FPA_QUE6_PAGE_INDEX pg_num 24 0
REG = FPA_QUE7_PAGE_INDEX 0x8001180028000128 MEMORY 8
REG_FIELD = FPA_QUE7_PAGE_INDEX pg_num 24 0

REG = FPA_QUE_ACT 0x8001180028000138 MEMORY 8
REG_FIELD = FPA_QUE_ACT act_que 28 26, act_indx 25 0

REG = FPA_QUE_EXP 0x8001180028000130 MEMORY 8
REG_FIELD = FPA_QUE_EXP exp_que 28 26, exp_indx 25 0

REG = FPA_WQE_THRESHOLD 0x8001180028000468 MEMORY 8
REG_FIELD = FPA_WQE_THRESHOLD thresh 31 0

REG = GMX0_BAD_REG 0x8001180008000518 MEMORY 8
REG_FIELD = GMX0_BAD_REG inb_nxa 30 27, statovr 26 26, loststat 25 22, out_ovr 5 2
REG = GMX1_BAD_REG 0x8001180010000518 MEMORY 8
REG_FIELD = GMX1_BAD_REG inb_nxa 30 27, statovr 26 26, loststat 25 22, out_ovr 5 2

REG = GMX0_BIST 0x8001180008000400 MEMORY 8
REG_FIELD = GMX0_BIST status 24 0
REG = GMX1_BIST 0x8001180010000400 MEMORY 8
REG_FIELD = GMX1_BIST status 24 0

REG = GMX0_CLK_EN 0x80011800080007F0 MEMORY 8
REG_FIELD = GMX0_CLK_EN clk_en 0 0
REG = GMX1_CLK_EN 0x80011800100007F0 MEMORY 8
REG_FIELD = GMX1_CLK_EN clk_en 0 0

REG = GMX0_HG2_CONTROL 0x8001180008000550 MEMORY 8
REG_FIELD = GMX0_HG2_CONTROL hg2tx_en 18 18, hg2rx_en 17 17, phys_en 16 16, logl_en 15 0
REG = GMX1_HG2_CONTROL 0x8001180010000550 MEMORY 8
REG_FIELD = GMX1_HG2_CONTROL hg2tx_en 18 18, hg2rx_en 17 17, phys_en 16 16, logl_en 15 0

REG = GMX0_INF_MODE 0x80011800080007F8 MEMORY 8
REG_FIELD = GMX0_INF_MODE mode 5 4, en 1 1
REG = GMX1_INF_MODE 0x80011800100007F8 MEMORY 8
REG_FIELD = GMX1_INF_MODE mode 5 4, en 1 1

REG = GMX0_NXA_ADR 0x8001180008000510 MEMORY 8
REG_FIELD = GMX0_NXA_ADR prt 5 0
REG = GMX1_NXA_ADR 0x8001180010000510 MEMORY 8
REG_FIELD = GMX1_NXA_ADR prt 5 0

REG = GMX0_PRT000_CBFC_CTL 0x8001180008000580 MEMORY 8
REG_FIELD = GMX0_PRT000_CBFC_CTL phys_en 63 48, logl_en 47 32, phys_bp 31 16, bck_en 3 3, drp_en 2 2, tx_en 1 1, rx_en 0 0
REG = GMX1_PRT000_CBFC_CTL 0x8001180010000580 MEMORY 8
REG_FIELD = GMX1_PRT000_CBFC_CTL phys_en 63 48, logl_en 47 32, phys_bp 31 16, bck_en 3 3, drp_en 2 2, tx_en 1 1, rx_en 0 0

REG = GMX0_PRT000_CFG 0x8001180008000010 MEMORY 8
REG_FIELD = GMX0_PRT000_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX0_PRT001_CFG 0x8001180008000810 MEMORY 8
REG_FIELD = GMX0_PRT001_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX0_PRT002_CFG 0x8001180008001010 MEMORY 8
REG_FIELD = GMX0_PRT002_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX0_PRT003_CFG 0x8001180008001810 MEMORY 8
REG_FIELD = GMX0_PRT003_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX1_PRT000_CFG 0x8001180010000010 MEMORY 8
REG_FIELD = GMX1_PRT000_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX1_PRT001_CFG 0x8001180010000810 MEMORY 8
REG_FIELD = GMX1_PRT001_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX1_PRT002_CFG 0x8001180010001010 MEMORY 8
REG_FIELD = GMX1_PRT002_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0
REG = GMX1_PRT003_CFG 0x8001180010001810 MEMORY 8
REG_FIELD = GMX1_PRT003_CFG tx_idle 13 13, rx_idle 12 12, speed_msb 8 8, slottime 3 3, duplex 2 2, speed 1 1, en 0 0

REG = GMX0_QSGMII_CTL 0x8001180008000760 MEMORY 8
REG_FIELD = GMX0_QSGMII_CTL disparity 0 0
REG = GMX1_QSGMII_CTL 0x8001180010000760 MEMORY 8
REG_FIELD = GMX1_QSGMII_CTL disparity 0 0

REG = GMX0_RX000_ADR_CAM0 0x8001180008000180 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM0 adr 63 0
REG = GMX0_RX001_ADR_CAM0 0x8001180008000980 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM0 adr 63 0
REG = GMX0_RX002_ADR_CAM0 0x8001180008001180 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM0 adr 63 0
REG = GMX0_RX003_ADR_CAM0 0x8001180008001980 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM0 adr 63 0
REG = GMX1_RX000_ADR_CAM0 0x8001180010000180 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM0 adr 63 0
REG = GMX1_RX001_ADR_CAM0 0x8001180010000980 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM0 adr 63 0
REG = GMX1_RX002_ADR_CAM0 0x8001180010001180 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM0 adr 63 0
REG = GMX1_RX003_ADR_CAM0 0x8001180010001980 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM0 adr 63 0

REG = GMX0_RX000_ADR_CAM1 0x8001180008000188 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM1 adr 63 0
REG = GMX0_RX001_ADR_CAM1 0x8001180008000988 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM1 adr 63 0
REG = GMX0_RX002_ADR_CAM1 0x8001180008001188 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM1 adr 63 0
REG = GMX0_RX003_ADR_CAM1 0x8001180008001988 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM1 adr 63 0
REG = GMX1_RX000_ADR_CAM1 0x8001180010000188 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM1 adr 63 0
REG = GMX1_RX001_ADR_CAM1 0x8001180010000988 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM1 adr 63 0
REG = GMX1_RX002_ADR_CAM1 0x8001180010001188 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM1 adr 63 0
REG = GMX1_RX003_ADR_CAM1 0x8001180010001988 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM1 adr 63 0

REG = GMX0_RX000_ADR_CAM2 0x8001180008000190 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM2 adr 63 0
REG = GMX0_RX001_ADR_CAM2 0x8001180008000990 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM2 adr 63 0
REG = GMX0_RX002_ADR_CAM2 0x8001180008001190 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM2 adr 63 0
REG = GMX0_RX003_ADR_CAM2 0x8001180008001990 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM2 adr 63 0
REG = GMX1_RX000_ADR_CAM2 0x8001180010000190 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM2 adr 63 0
REG = GMX1_RX001_ADR_CAM2 0x8001180010000990 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM2 adr 63 0
REG = GMX1_RX002_ADR_CAM2 0x8001180010001190 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM2 adr 63 0
REG = GMX1_RX003_ADR_CAM2 0x8001180010001990 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM2 adr 63 0

REG = GMX0_RX000_ADR_CAM3 0x8001180008000198 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM3 adr 63 0
REG = GMX0_RX001_ADR_CAM3 0x8001180008000998 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM3 adr 63 0
REG = GMX0_RX002_ADR_CAM3 0x8001180008001198 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM3 adr 63 0
REG = GMX0_RX003_ADR_CAM3 0x8001180008001998 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM3 adr 63 0
REG = GMX1_RX000_ADR_CAM3 0x8001180010000198 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM3 adr 63 0
REG = GMX1_RX001_ADR_CAM3 0x8001180010000998 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM3 adr 63 0
REG = GMX1_RX002_ADR_CAM3 0x8001180010001198 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM3 adr 63 0
REG = GMX1_RX003_ADR_CAM3 0x8001180010001998 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM3 adr 63 0

REG = GMX0_RX000_ADR_CAM4 0x80011800080001A0 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM4 adr 63 0
REG = GMX0_RX001_ADR_CAM4 0x80011800080009A0 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM4 adr 63 0
REG = GMX0_RX002_ADR_CAM4 0x80011800080011A0 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM4 adr 63 0
REG = GMX0_RX003_ADR_CAM4 0x80011800080019A0 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM4 adr 63 0
REG = GMX1_RX000_ADR_CAM4 0x80011800100001A0 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM4 adr 63 0
REG = GMX1_RX001_ADR_CAM4 0x80011800100009A0 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM4 adr 63 0
REG = GMX1_RX002_ADR_CAM4 0x80011800100011A0 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM4 adr 63 0
REG = GMX1_RX003_ADR_CAM4 0x80011800100019A0 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM4 adr 63 0

REG = GMX0_RX000_ADR_CAM5 0x80011800080001A8 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM5 adr 63 0
REG = GMX0_RX001_ADR_CAM5 0x80011800080009A8 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM5 adr 63 0
REG = GMX0_RX002_ADR_CAM5 0x80011800080011A8 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM5 adr 63 0
REG = GMX0_RX003_ADR_CAM5 0x80011800080019A8 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM5 adr 63 0
REG = GMX1_RX000_ADR_CAM5 0x80011800100001A8 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM5 adr 63 0
REG = GMX1_RX001_ADR_CAM5 0x80011800100009A8 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM5 adr 63 0
REG = GMX1_RX002_ADR_CAM5 0x80011800100011A8 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM5 adr 63 0
REG = GMX1_RX003_ADR_CAM5 0x80011800100019A8 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM5 adr 63 0

REG = GMX0_RX000_ADR_CAM_ALL_EN 0x8001180008000110 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM_ALL_EN en 31 0
REG = GMX0_RX001_ADR_CAM_ALL_EN 0x8001180008000910 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM_ALL_EN en 31 0
REG = GMX0_RX002_ADR_CAM_ALL_EN 0x8001180008001110 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM_ALL_EN en 31 0
REG = GMX0_RX003_ADR_CAM_ALL_EN 0x8001180008001910 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM_ALL_EN en 31 0
REG = GMX1_RX000_ADR_CAM_ALL_EN 0x8001180010000110 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM_ALL_EN en 31 0
REG = GMX1_RX001_ADR_CAM_ALL_EN 0x8001180010000910 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM_ALL_EN en 31 0
REG = GMX1_RX002_ADR_CAM_ALL_EN 0x8001180010001110 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM_ALL_EN en 31 0
REG = GMX1_RX003_ADR_CAM_ALL_EN 0x8001180010001910 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM_ALL_EN en 31 0

REG = GMX0_RX000_ADR_CAM_EN 0x8001180008000108 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CAM_EN en 7 0
REG = GMX0_RX001_ADR_CAM_EN 0x8001180008000908 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CAM_EN en 7 0
REG = GMX0_RX002_ADR_CAM_EN 0x8001180008001108 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CAM_EN en 7 0
REG = GMX0_RX003_ADR_CAM_EN 0x8001180008001908 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CAM_EN en 7 0
REG = GMX1_RX000_ADR_CAM_EN 0x8001180010000108 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CAM_EN en 7 0
REG = GMX1_RX001_ADR_CAM_EN 0x8001180010000908 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CAM_EN en 7 0
REG = GMX1_RX002_ADR_CAM_EN 0x8001180010001108 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CAM_EN en 7 0
REG = GMX1_RX003_ADR_CAM_EN 0x8001180010001908 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CAM_EN en 7 0

REG = GMX0_RX000_ADR_CTL 0x8001180008000100 MEMORY 8
REG_FIELD = GMX0_RX000_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX0_RX001_ADR_CTL 0x8001180008000900 MEMORY 8
REG_FIELD = GMX0_RX001_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX0_RX002_ADR_CTL 0x8001180008001100 MEMORY 8
REG_FIELD = GMX0_RX002_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX0_RX003_ADR_CTL 0x8001180008001900 MEMORY 8
REG_FIELD = GMX0_RX003_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX1_RX000_ADR_CTL 0x8001180010000100 MEMORY 8
REG_FIELD = GMX1_RX000_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX1_RX001_ADR_CTL 0x8001180010000900 MEMORY 8
REG_FIELD = GMX1_RX001_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX1_RX002_ADR_CTL 0x8001180010001100 MEMORY 8
REG_FIELD = GMX1_RX002_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0
REG = GMX1_RX003_ADR_CTL 0x8001180010001900 MEMORY 8
REG_FIELD = GMX1_RX003_ADR_CTL cam_mode 3 3, mcst 2 1, bcst 0 0

REG = GMX0_RX000_DECISION 0x8001180008000040 MEMORY 8
REG_FIELD = GMX0_RX000_DECISION cnt 4 0
REG = GMX0_RX001_DECISION 0x8001180008000840 MEMORY 8
REG_FIELD = GMX0_RX001_DECISION cnt 4 0
REG = GMX0_RX002_DECISION 0x8001180008001040 MEMORY 8
REG_FIELD = GMX0_RX002_DECISION cnt 4 0
REG = GMX0_RX003_DECISION 0x8001180008001840 MEMORY 8
REG_FIELD = GMX0_RX003_DECISION cnt 4 0
REG = GMX1_RX000_DECISION 0x8001180010000040 MEMORY 8
REG_FIELD = GMX1_RX000_DECISION cnt 4 0
REG = GMX1_RX001_DECISION 0x8001180010000840 MEMORY 8
REG_FIELD = GMX1_RX001_DECISION cnt 4 0
REG = GMX1_RX002_DECISION 0x8001180010001040 MEMORY 8
REG_FIELD = GMX1_RX002_DECISION cnt 4 0
REG = GMX1_RX003_DECISION 0x8001180010001840 MEMORY 8
REG_FIELD = GMX1_RX003_DECISION cnt 4 0

REG = GMX0_RX000_FRM_CHK 0x8001180008000020 MEMORY 8
REG_FIELD = GMX0_RX000_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX001_FRM_CHK 0x8001180008000820 MEMORY 8
REG_FIELD = GMX0_RX001_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX002_FRM_CHK 0x8001180008001020 MEMORY 8
REG_FIELD = GMX0_RX002_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX003_FRM_CHK 0x8001180008001820 MEMORY 8
REG_FIELD = GMX0_RX003_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX000_FRM_CHK 0x8001180010000020 MEMORY 8
REG_FIELD = GMX1_RX000_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX001_FRM_CHK 0x8001180010000820 MEMORY 8
REG_FIELD = GMX1_RX001_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX002_FRM_CHK 0x8001180010001020 MEMORY 8
REG_FIELD = GMX1_RX002_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX003_FRM_CHK 0x8001180010001820 MEMORY 8
REG_FIELD = GMX1_RX003_FRM_CHK skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0

REG = GMX0_RX000_FRM_CTL 0x8001180008000018 MEMORY 8
REG_FIELD = GMX0_RX000_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX0_RX001_FRM_CTL 0x8001180008000818 MEMORY 8
REG_FIELD = GMX0_RX001_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX0_RX002_FRM_CTL 0x8001180008001018 MEMORY 8
REG_FIELD = GMX0_RX002_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX0_RX003_FRM_CTL 0x8001180008001818 MEMORY 8
REG_FIELD = GMX0_RX003_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX1_RX000_FRM_CTL 0x8001180010000018 MEMORY 8
REG_FIELD = GMX1_RX000_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX1_RX001_FRM_CTL 0x8001180010000818 MEMORY 8
REG_FIELD = GMX1_RX001_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX1_RX002_FRM_CTL 0x8001180010001018 MEMORY 8
REG_FIELD = GMX1_RX002_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0
REG = GMX1_RX003_FRM_CTL 0x8001180010001818 MEMORY 8
REG_FIELD = GMX1_RX003_FRM_CTL ptp_mode 12 12, null_dis 10 10, pre_align 9 9, pre_free 6 6, ctl_smac 5 5, ctl_mcst 4 4, ctl_bck 3 3, ctl_drp 2 2, pre_strp 1 1, pre_chk 0 0

REG = GMX0_RX000_IFG 0x8001180008000058 MEMORY 8
REG_FIELD = GMX0_RX000_IFG ifg 3 0
REG = GMX0_RX001_IFG 0x8001180008000858 MEMORY 8
REG_FIELD = GMX0_RX001_IFG ifg 3 0
REG = GMX0_RX002_IFG 0x8001180008001058 MEMORY 8
REG_FIELD = GMX0_RX002_IFG ifg 3 0
REG = GMX0_RX003_IFG 0x8001180008001858 MEMORY 8
REG_FIELD = GMX0_RX003_IFG ifg 3 0
REG = GMX1_RX000_IFG 0x8001180010000058 MEMORY 8
REG_FIELD = GMX1_RX000_IFG ifg 3 0
REG = GMX1_RX001_IFG 0x8001180010000858 MEMORY 8
REG_FIELD = GMX1_RX001_IFG ifg 3 0
REG = GMX1_RX002_IFG 0x8001180010001058 MEMORY 8
REG_FIELD = GMX1_RX002_IFG ifg 3 0
REG = GMX1_RX003_IFG 0x8001180010001858 MEMORY 8
REG_FIELD = GMX1_RX003_IFG ifg 3 0

REG = GMX0_RX000_INT_EN 0x8001180008000008 MEMORY 8
REG_FIELD = GMX0_RX000_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX001_INT_EN 0x8001180008000808 MEMORY 8
REG_FIELD = GMX0_RX001_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX002_INT_EN 0x8001180008001008 MEMORY 8
REG_FIELD = GMX0_RX002_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX003_INT_EN 0x8001180008001808 MEMORY 8
REG_FIELD = GMX0_RX003_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX000_INT_EN 0x8001180010000008 MEMORY 8
REG_FIELD = GMX1_RX000_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX001_INT_EN 0x8001180010000808 MEMORY 8
REG_FIELD = GMX1_RX001_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX002_INT_EN 0x8001180010001008 MEMORY 8
REG_FIELD = GMX1_RX002_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX003_INT_EN 0x8001180010001808 MEMORY 8
REG_FIELD = GMX1_RX003_INT_EN wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0

REG = GMX0_RX000_INT_REG 0x8001180008000000 MEMORY 8
REG_FIELD = GMX0_RX000_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX001_INT_REG 0x8001180008000800 MEMORY 8
REG_FIELD = GMX0_RX001_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX002_INT_REG 0x8001180008001000 MEMORY 8
REG_FIELD = GMX0_RX002_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX0_RX003_INT_REG 0x8001180008001800 MEMORY 8
REG_FIELD = GMX0_RX003_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX000_INT_REG 0x8001180010000000 MEMORY 8
REG_FIELD = GMX1_RX000_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX001_INT_REG 0x8001180010000800 MEMORY 8
REG_FIELD = GMX1_RX001_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX002_INT_REG 0x8001180010001000 MEMORY 8
REG_FIELD = GMX1_RX002_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0
REG = GMX1_RX003_INT_REG 0x8001180010001800 MEMORY 8
REG_FIELD = GMX1_RX003_INT_REG wol 29 29, hg2cc 28 28, hg2fld 27 27, undat 26 26, uneop 25 25, unsop 24 24, bad_term 23 23, bad_seq 22 22, rem_fault 21 21, loc_fault 20 20, pause_drp 19 19, ifgerr 15 15, coldet 14 14, falerr 13 13, rsverr 12 12, pcterr 11 11, ovrerr 10 10, skperr 8 8, rcverr 7 7, fcserr 4 4, jabber 3 3, carext 1 1, minerr 0 0

REG = GMX0_RX000_JABBER 0x8001180008000038 MEMORY 8
REG_FIELD = GMX0_RX000_JABBER cnt 15 0
REG = GMX0_RX001_JABBER 0x8001180008000838 MEMORY 8
REG_FIELD = GMX0_RX001_JABBER cnt 15 0
REG = GMX0_RX002_JABBER 0x8001180008001038 MEMORY 8
REG_FIELD = GMX0_RX002_JABBER cnt 15 0
REG = GMX0_RX003_JABBER 0x8001180008001838 MEMORY 8
REG_FIELD = GMX0_RX003_JABBER cnt 15 0
REG = GMX1_RX000_JABBER 0x8001180010000038 MEMORY 8
REG_FIELD = GMX1_RX000_JABBER cnt 15 0
REG = GMX1_RX001_JABBER 0x8001180010000838 MEMORY 8
REG_FIELD = GMX1_RX001_JABBER cnt 15 0
REG = GMX1_RX002_JABBER 0x8001180010001038 MEMORY 8
REG_FIELD = GMX1_RX002_JABBER cnt 15 0
REG = GMX1_RX003_JABBER 0x8001180010001838 MEMORY 8
REG_FIELD = GMX1_RX003_JABBER cnt 15 0

REG = GMX0_RX000_PAUSE_DROP_TIME 0x8001180008000068 MEMORY 8
REG_FIELD = GMX0_RX000_PAUSE_DROP_TIME status 15 0
REG = GMX0_RX001_PAUSE_DROP_TIME 0x8001180008000868 MEMORY 8
REG_FIELD = GMX0_RX001_PAUSE_DROP_TIME status 15 0
REG = GMX0_RX002_PAUSE_DROP_TIME 0x8001180008001068 MEMORY 8
REG_FIELD = GMX0_RX002_PAUSE_DROP_TIME status 15 0
REG = GMX0_RX003_PAUSE_DROP_TIME 0x8001180008001868 MEMORY 8
REG_FIELD = GMX0_RX003_PAUSE_DROP_TIME status 15 0
REG = GMX1_RX000_PAUSE_DROP_TIME 0x8001180010000068 MEMORY 8
REG_FIELD = GMX1_RX000_PAUSE_DROP_TIME status 15 0
REG = GMX1_RX001_PAUSE_DROP_TIME 0x8001180010000868 MEMORY 8
REG_FIELD = GMX1_RX001_PAUSE_DROP_TIME status 15 0
REG = GMX1_RX002_PAUSE_DROP_TIME 0x8001180010001068 MEMORY 8
REG_FIELD = GMX1_RX002_PAUSE_DROP_TIME status 15 0
REG = GMX1_RX003_PAUSE_DROP_TIME 0x8001180010001868 MEMORY 8
REG_FIELD = GMX1_RX003_PAUSE_DROP_TIME status 15 0

REG = GMX0_RX000_STATS_CTL 0x8001180008000050 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_CTL rd_clr 0 0
REG = GMX0_RX001_STATS_CTL 0x8001180008000850 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_CTL rd_clr 0 0
REG = GMX0_RX002_STATS_CTL 0x8001180008001050 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_CTL rd_clr 0 0
REG = GMX0_RX003_STATS_CTL 0x8001180008001850 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_CTL rd_clr 0 0
REG = GMX1_RX000_STATS_CTL 0x8001180010000050 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_CTL rd_clr 0 0
REG = GMX1_RX001_STATS_CTL 0x8001180010000850 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_CTL rd_clr 0 0
REG = GMX1_RX002_STATS_CTL 0x8001180010001050 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_CTL rd_clr 0 0
REG = GMX1_RX003_STATS_CTL 0x8001180010001850 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_CTL rd_clr 0 0

REG = GMX0_RX000_STATS_OCTS 0x8001180008000088 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_OCTS cnt 47 0
REG = GMX0_RX001_STATS_OCTS 0x8001180008000888 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_OCTS cnt 47 0
REG = GMX0_RX002_STATS_OCTS 0x8001180008001088 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_OCTS cnt 47 0
REG = GMX0_RX003_STATS_OCTS 0x8001180008001888 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_OCTS cnt 47 0
REG = GMX1_RX000_STATS_OCTS 0x8001180010000088 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_OCTS cnt 47 0
REG = GMX1_RX001_STATS_OCTS 0x8001180010000888 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_OCTS cnt 47 0
REG = GMX1_RX002_STATS_OCTS 0x8001180010001088 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_OCTS cnt 47 0
REG = GMX1_RX003_STATS_OCTS 0x8001180010001888 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_OCTS cnt 47 0

REG = GMX0_RX000_STATS_OCTS_CTL 0x8001180008000098 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_OCTS_CTL cnt 47 0
REG = GMX0_RX001_STATS_OCTS_CTL 0x8001180008000898 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_OCTS_CTL cnt 47 0
REG = GMX0_RX002_STATS_OCTS_CTL 0x8001180008001098 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_OCTS_CTL cnt 47 0
REG = GMX0_RX003_STATS_OCTS_CTL 0x8001180008001898 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_OCTS_CTL cnt 47 0
REG = GMX1_RX000_STATS_OCTS_CTL 0x8001180010000098 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_OCTS_CTL cnt 47 0
REG = GMX1_RX001_STATS_OCTS_CTL 0x8001180010000898 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_OCTS_CTL cnt 47 0
REG = GMX1_RX002_STATS_OCTS_CTL 0x8001180010001098 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_OCTS_CTL cnt 47 0
REG = GMX1_RX003_STATS_OCTS_CTL 0x8001180010001898 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_OCTS_CTL cnt 47 0

REG = GMX0_RX000_STATS_OCTS_DMAC 0x80011800080000A8 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_OCTS_DMAC cnt 47 0
REG = GMX0_RX001_STATS_OCTS_DMAC 0x80011800080008A8 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_OCTS_DMAC cnt 47 0
REG = GMX0_RX002_STATS_OCTS_DMAC 0x80011800080010A8 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_OCTS_DMAC cnt 47 0
REG = GMX0_RX003_STATS_OCTS_DMAC 0x80011800080018A8 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_OCTS_DMAC cnt 47 0
REG = GMX1_RX000_STATS_OCTS_DMAC 0x80011800100000A8 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_OCTS_DMAC cnt 47 0
REG = GMX1_RX001_STATS_OCTS_DMAC 0x80011800100008A8 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_OCTS_DMAC cnt 47 0
REG = GMX1_RX002_STATS_OCTS_DMAC 0x80011800100010A8 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_OCTS_DMAC cnt 47 0
REG = GMX1_RX003_STATS_OCTS_DMAC 0x80011800100018A8 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_OCTS_DMAC cnt 47 0

REG = GMX0_RX000_STATS_OCTS_DRP 0x80011800080000B8 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_OCTS_DRP cnt 47 0
REG = GMX0_RX001_STATS_OCTS_DRP 0x80011800080008B8 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_OCTS_DRP cnt 47 0
REG = GMX0_RX002_STATS_OCTS_DRP 0x80011800080010B8 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_OCTS_DRP cnt 47 0
REG = GMX0_RX003_STATS_OCTS_DRP 0x80011800080018B8 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_OCTS_DRP cnt 47 0
REG = GMX1_RX000_STATS_OCTS_DRP 0x80011800100000B8 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_OCTS_DRP cnt 47 0
REG = GMX1_RX001_STATS_OCTS_DRP 0x80011800100008B8 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_OCTS_DRP cnt 47 0
REG = GMX1_RX002_STATS_OCTS_DRP 0x80011800100010B8 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_OCTS_DRP cnt 47 0
REG = GMX1_RX003_STATS_OCTS_DRP 0x80011800100018B8 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_OCTS_DRP cnt 47 0

REG = GMX0_RX000_STATS_PKTS 0x8001180008000080 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_PKTS cnt 31 0
REG = GMX0_RX001_STATS_PKTS 0x8001180008000880 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_PKTS cnt 31 0
REG = GMX0_RX002_STATS_PKTS 0x8001180008001080 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_PKTS cnt 31 0
REG = GMX0_RX003_STATS_PKTS 0x8001180008001880 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_PKTS cnt 31 0
REG = GMX1_RX000_STATS_PKTS 0x8001180010000080 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_PKTS cnt 31 0
REG = GMX1_RX001_STATS_PKTS 0x8001180010000880 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_PKTS cnt 31 0
REG = GMX1_RX002_STATS_PKTS 0x8001180010001080 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_PKTS cnt 31 0
REG = GMX1_RX003_STATS_PKTS 0x8001180010001880 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_PKTS cnt 31 0

REG = GMX0_RX000_STATS_PKTS_BAD 0x80011800080000C0 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_PKTS_BAD cnt 31 0
REG = GMX0_RX001_STATS_PKTS_BAD 0x80011800080008C0 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_PKTS_BAD cnt 31 0
REG = GMX0_RX002_STATS_PKTS_BAD 0x80011800080010C0 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_PKTS_BAD cnt 31 0
REG = GMX0_RX003_STATS_PKTS_BAD 0x80011800080018C0 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_PKTS_BAD cnt 31 0
REG = GMX1_RX000_STATS_PKTS_BAD 0x80011800100000C0 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_PKTS_BAD cnt 31 0
REG = GMX1_RX001_STATS_PKTS_BAD 0x80011800100008C0 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_PKTS_BAD cnt 31 0
REG = GMX1_RX002_STATS_PKTS_BAD 0x80011800100010C0 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_PKTS_BAD cnt 31 0
REG = GMX1_RX003_STATS_PKTS_BAD 0x80011800100018C0 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_PKTS_BAD cnt 31 0

REG = GMX0_RX000_STATS_PKTS_CTL 0x8001180008000090 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_PKTS_CTL cnt 31 0
REG = GMX0_RX001_STATS_PKTS_CTL 0x8001180008000890 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_PKTS_CTL cnt 31 0
REG = GMX0_RX002_STATS_PKTS_CTL 0x8001180008001090 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_PKTS_CTL cnt 31 0
REG = GMX0_RX003_STATS_PKTS_CTL 0x8001180008001890 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_PKTS_CTL cnt 31 0
REG = GMX1_RX000_STATS_PKTS_CTL 0x8001180010000090 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_PKTS_CTL cnt 31 0
REG = GMX1_RX001_STATS_PKTS_CTL 0x8001180010000890 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_PKTS_CTL cnt 31 0
REG = GMX1_RX002_STATS_PKTS_CTL 0x8001180010001090 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_PKTS_CTL cnt 31 0
REG = GMX1_RX003_STATS_PKTS_CTL 0x8001180010001890 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_PKTS_CTL cnt 31 0

REG = GMX0_RX000_STATS_PKTS_DMAC 0x80011800080000A0 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_PKTS_DMAC cnt 31 0
REG = GMX0_RX001_STATS_PKTS_DMAC 0x80011800080008A0 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_PKTS_DMAC cnt 31 0
REG = GMX0_RX002_STATS_PKTS_DMAC 0x80011800080010A0 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_PKTS_DMAC cnt 31 0
REG = GMX0_RX003_STATS_PKTS_DMAC 0x80011800080018A0 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_PKTS_DMAC cnt 31 0
REG = GMX1_RX000_STATS_PKTS_DMAC 0x80011800100000A0 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_PKTS_DMAC cnt 31 0
REG = GMX1_RX001_STATS_PKTS_DMAC 0x80011800100008A0 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_PKTS_DMAC cnt 31 0
REG = GMX1_RX002_STATS_PKTS_DMAC 0x80011800100010A0 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_PKTS_DMAC cnt 31 0
REG = GMX1_RX003_STATS_PKTS_DMAC 0x80011800100018A0 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_PKTS_DMAC cnt 31 0

REG = GMX0_RX000_STATS_PKTS_DRP 0x80011800080000B0 MEMORY 8
REG_FIELD = GMX0_RX000_STATS_PKTS_DRP cnt 31 0
REG = GMX0_RX001_STATS_PKTS_DRP 0x80011800080008B0 MEMORY 8
REG_FIELD = GMX0_RX001_STATS_PKTS_DRP cnt 31 0
REG = GMX0_RX002_STATS_PKTS_DRP 0x80011800080010B0 MEMORY 8
REG_FIELD = GMX0_RX002_STATS_PKTS_DRP cnt 31 0
REG = GMX0_RX003_STATS_PKTS_DRP 0x80011800080018B0 MEMORY 8
REG_FIELD = GMX0_RX003_STATS_PKTS_DRP cnt 31 0
REG = GMX1_RX000_STATS_PKTS_DRP 0x80011800100000B0 MEMORY 8
REG_FIELD = GMX1_RX000_STATS_PKTS_DRP cnt 31 0
REG = GMX1_RX001_STATS_PKTS_DRP 0x80011800100008B0 MEMORY 8
REG_FIELD = GMX1_RX001_STATS_PKTS_DRP cnt 31 0
REG = GMX1_RX002_STATS_PKTS_DRP 0x80011800100010B0 MEMORY 8
REG_FIELD = GMX1_RX002_STATS_PKTS_DRP cnt 31 0
REG = GMX1_RX003_STATS_PKTS_DRP 0x80011800100018B0 MEMORY 8
REG_FIELD = GMX1_RX003_STATS_PKTS_DRP cnt 31 0

REG = GMX0_RX000_UDD_SKP 0x8001180008000048 MEMORY 8
REG_FIELD = GMX0_RX000_UDD_SKP fcssel 8 8, len 6 0
REG = GMX0_RX001_UDD_SKP 0x8001180008000848 MEMORY 8
REG_FIELD = GMX0_RX001_UDD_SKP fcssel 8 8, len 6 0
REG = GMX0_RX002_UDD_SKP 0x8001180008001048 MEMORY 8
REG_FIELD = GMX0_RX002_UDD_SKP fcssel 8 8, len 6 0
REG = GMX0_RX003_UDD_SKP 0x8001180008001848 MEMORY 8
REG_FIELD = GMX0_RX003_UDD_SKP fcssel 8 8, len 6 0
REG = GMX1_RX000_UDD_SKP 0x8001180010000048 MEMORY 8
REG_FIELD = GMX1_RX000_UDD_SKP fcssel 8 8, len 6 0
REG = GMX1_RX001_UDD_SKP 0x8001180010000848 MEMORY 8
REG_FIELD = GMX1_RX001_UDD_SKP fcssel 8 8, len 6 0
REG = GMX1_RX002_UDD_SKP 0x8001180010001048 MEMORY 8
REG_FIELD = GMX1_RX002_UDD_SKP fcssel 8 8, len 6 0
REG = GMX1_RX003_UDD_SKP 0x8001180010001848 MEMORY 8
REG_FIELD = GMX1_RX003_UDD_SKP fcssel 8 8, len 6 0

REG = GMX0_RX_BP_DROP000 0x8001180008000420 MEMORY 8
REG_FIELD = GMX0_RX_BP_DROP000 mark 5 0
REG = GMX0_RX_BP_DROP001 0x8001180008000428 MEMORY 8
REG_FIELD = GMX0_RX_BP_DROP001 mark 5 0
REG = GMX0_RX_BP_DROP002 0x8001180008000430 MEMORY 8
REG_FIELD = GMX0_RX_BP_DROP002 mark 5 0
REG = GMX0_RX_BP_DROP003 0x8001180008000438 MEMORY 8
REG_FIELD = GMX0_RX_BP_DROP003 mark 5 0
REG = GMX1_RX_BP_DROP000 0x8001180010000420 MEMORY 8
REG_FIELD = GMX1_RX_BP_DROP000 mark 5 0
REG = GMX1_RX_BP_DROP001 0x8001180010000428 MEMORY 8
REG_FIELD = GMX1_RX_BP_DROP001 mark 5 0
REG = GMX1_RX_BP_DROP002 0x8001180010000430 MEMORY 8
REG_FIELD = GMX1_RX_BP_DROP002 mark 5 0
REG = GMX1_RX_BP_DROP003 0x8001180010000438 MEMORY 8
REG_FIELD = GMX1_RX_BP_DROP003 mark 5 0

REG = GMX0_RX_BP_OFF000 0x8001180008000460 MEMORY 8
REG_FIELD = GMX0_RX_BP_OFF000 mark 5 0
REG = GMX0_RX_BP_OFF001 0x8001180008000468 MEMORY 8
REG_FIELD = GMX0_RX_BP_OFF001 mark 5 0
REG = GMX0_RX_BP_OFF002 0x8001180008000470 MEMORY 8
REG_FIELD = GMX0_RX_BP_OFF002 mark 5 0
REG = GMX0_RX_BP_OFF003 0x8001180008000478 MEMORY 8
REG_FIELD = GMX0_RX_BP_OFF003 mark 5 0
REG = GMX1_RX_BP_OFF000 0x8001180010000460 MEMORY 8
REG_FIELD = GMX1_RX_BP_OFF000 mark 5 0
REG = GMX1_RX_BP_OFF001 0x8001180010000468 MEMORY 8
REG_FIELD = GMX1_RX_BP_OFF001 mark 5 0
REG = GMX1_RX_BP_OFF002 0x8001180010000470 MEMORY 8
REG_FIELD = GMX1_RX_BP_OFF002 mark 5 0
REG = GMX1_RX_BP_OFF003 0x8001180010000478 MEMORY 8
REG_FIELD = GMX1_RX_BP_OFF003 mark 5 0

REG = GMX0_RX_BP_ON000 0x8001180008000440 MEMORY 8
REG_FIELD = GMX0_RX_BP_ON000 mark 8 0
REG = GMX0_RX_BP_ON001 0x8001180008000448 MEMORY 8
REG_FIELD = GMX0_RX_BP_ON001 mark 8 0
REG = GMX0_RX_BP_ON002 0x8001180008000450 MEMORY 8
REG_FIELD = GMX0_RX_BP_ON002 mark 8 0
REG = GMX0_RX_BP_ON003 0x8001180008000458 MEMORY 8
REG_FIELD = GMX0_RX_BP_ON003 mark 8 0
REG = GMX1_RX_BP_ON000 0x8001180010000440 MEMORY 8
REG_FIELD = GMX1_RX_BP_ON000 mark 8 0
REG = GMX1_RX_BP_ON001 0x8001180010000448 MEMORY 8
REG_FIELD = GMX1_RX_BP_ON001 mark 8 0
REG = GMX1_RX_BP_ON002 0x8001180010000450 MEMORY 8
REG_FIELD = GMX1_RX_BP_ON002 mark 8 0
REG = GMX1_RX_BP_ON003 0x8001180010000458 MEMORY 8
REG_FIELD = GMX1_RX_BP_ON003 mark 8 0

REG = GMX0_RX_HG2_STATUS 0x8001180008000548 MEMORY 8
REG_FIELD = GMX0_RX_HG2_STATUS phtim2go 47 32, xof 31 16, lgtim2go 15 0
REG = GMX1_RX_HG2_STATUS 0x8001180010000548 MEMORY 8
REG_FIELD = GMX1_RX_HG2_STATUS phtim2go 47 32, xof 31 16, lgtim2go 15 0

REG = GMX0_RX_PRT_INFO 0x80011800080004E8 MEMORY 8
REG_FIELD = GMX0_RX_PRT_INFO drop 19 16, commit 3 0
REG = GMX1_RX_PRT_INFO 0x80011800100004E8 MEMORY 8
REG_FIELD = GMX1_RX_PRT_INFO drop 19 16, commit 3 0

REG = GMX0_RX_PRTS 0x8001180008000410 MEMORY 8
REG_FIELD = GMX0_RX_PRTS prts 2 0
REG = GMX1_RX_PRTS 0x8001180010000410 MEMORY 8
REG_FIELD = GMX1_RX_PRTS prts 2 0

REG = GMX0_RX_XAUI_BAD_COL 0x8001180008000538 MEMORY 8
REG_FIELD = GMX0_RX_XAUI_BAD_COL val 39 39, state 38 36, lane_rxc 35 32, lane_rxd 31 0
REG = GMX1_RX_XAUI_BAD_COL 0x8001180010000538 MEMORY 8
REG_FIELD = GMX1_RX_XAUI_BAD_COL val 39 39, state 38 36, lane_rxc 35 32, lane_rxd 31 0

REG = GMX0_RX_XAUI_CTL 0x8001180008000530 MEMORY 8
REG_FIELD = GMX0_RX_XAUI_CTL status 1 0
REG = GMX1_RX_XAUI_CTL 0x8001180010000530 MEMORY 8
REG_FIELD = GMX1_RX_XAUI_CTL status 1 0

REG = GMX0_RXAUI_CTL 0x8001180008000740 MEMORY 8
REG_FIELD = GMX0_RXAUI_CTL disparity 0 0
REG = GMX1_RXAUI_CTL 0x8001180010000740 MEMORY 8
REG_FIELD = GMX1_RXAUI_CTL disparity 0 0

REG = GMX0_SMAC000 0x8001180008000230 MEMORY 8
REG_FIELD = GMX0_SMAC000 smac 47 0
REG = GMX0_SMAC001 0x8001180008000A30 MEMORY 8
REG_FIELD = GMX0_SMAC001 smac 47 0
REG = GMX0_SMAC002 0x8001180008001230 MEMORY 8
REG_FIELD = GMX0_SMAC002 smac 47 0
REG = GMX0_SMAC003 0x8001180008001A30 MEMORY 8
REG_FIELD = GMX0_SMAC003 smac 47 0
REG = GMX1_SMAC000 0x8001180010000230 MEMORY 8
REG_FIELD = GMX1_SMAC000 smac 47 0
REG = GMX1_SMAC001 0x8001180010000A30 MEMORY 8
REG_FIELD = GMX1_SMAC001 smac 47 0
REG = GMX1_SMAC002 0x8001180010001230 MEMORY 8
REG_FIELD = GMX1_SMAC002 smac 47 0
REG = GMX1_SMAC003 0x8001180010001A30 MEMORY 8
REG_FIELD = GMX1_SMAC003 smac 47 0

REG = GMX0_STAT_BP 0x8001180008000520 MEMORY 8
REG_FIELD = GMX0_STAT_BP bp 16 16, cnt 15 0
REG = GMX1_STAT_BP 0x8001180010000520 MEMORY 8
REG_FIELD = GMX1_STAT_BP bp 16 16, cnt 15 0

REG = GMX0_TB_REG 0x80011800080007E0 MEMORY 8
REG_FIELD = GMX0_TB_REG wr_magic 0 0
REG = GMX1_TB_REG 0x80011800100007E0 MEMORY 8
REG_FIELD = GMX1_TB_REG wr_magic 0 0

REG = GMX0_TX000_APPEND 0x8001180008000218 MEMORY 8
REG_FIELD = GMX0_TX000_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX0_TX001_APPEND 0x8001180008000A18 MEMORY 8
REG_FIELD = GMX0_TX001_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX0_TX002_APPEND 0x8001180008001218 MEMORY 8
REG_FIELD = GMX0_TX002_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX0_TX003_APPEND 0x8001180008001A18 MEMORY 8
REG_FIELD = GMX0_TX003_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX1_TX000_APPEND 0x8001180010000218 MEMORY 8
REG_FIELD = GMX1_TX000_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX1_TX001_APPEND 0x8001180010000A18 MEMORY 8
REG_FIELD = GMX1_TX001_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX1_TX002_APPEND 0x8001180010001218 MEMORY 8
REG_FIELD = GMX1_TX002_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0
REG = GMX1_TX003_APPEND 0x8001180010001A18 MEMORY 8
REG_FIELD = GMX1_TX003_APPEND force_fcs 3 3, fcs 2 2, pad 1 1, preamble 0 0

REG = GMX0_TX000_BCK_CRDT 0x8001180008000388 MEMORY 8
REG_FIELD = GMX0_TX000_BCK_CRDT cnt 3 0
REG = GMX0_TX001_BCK_CRDT 0x8001180008000B88 MEMORY 8
REG_FIELD = GMX0_TX001_BCK_CRDT cnt 3 0
REG = GMX0_TX002_BCK_CRDT 0x8001180008001388 MEMORY 8
REG_FIELD = GMX0_TX002_BCK_CRDT cnt 3 0
REG = GMX0_TX003_BCK_CRDT 0x8001180008001B88 MEMORY 8
REG_FIELD = GMX0_TX003_BCK_CRDT cnt 3 0
REG = GMX1_TX000_BCK_CRDT 0x8001180010000388 MEMORY 8
REG_FIELD = GMX1_TX000_BCK_CRDT cnt 3 0
REG = GMX1_TX001_BCK_CRDT 0x8001180010000B88 MEMORY 8
REG_FIELD = GMX1_TX001_BCK_CRDT cnt 3 0
REG = GMX1_TX002_BCK_CRDT 0x8001180010001388 MEMORY 8
REG_FIELD = GMX1_TX002_BCK_CRDT cnt 3 0
REG = GMX1_TX003_BCK_CRDT 0x8001180010001B88 MEMORY 8
REG_FIELD = GMX1_TX003_BCK_CRDT cnt 3 0

REG = GMX0_TX000_BURST 0x8001180008000228 MEMORY 8
REG_FIELD = GMX0_TX000_BURST burst 15 0
REG = GMX0_TX001_BURST 0x8001180008000A28 MEMORY 8
REG_FIELD = GMX0_TX001_BURST burst 15 0
REG = GMX0_TX002_BURST 0x8001180008001228 MEMORY 8
REG_FIELD = GMX0_TX002_BURST burst 15 0
REG = GMX0_TX003_BURST 0x8001180008001A28 MEMORY 8
REG_FIELD = GMX0_TX003_BURST burst 15 0
REG = GMX1_TX000_BURST 0x8001180010000228 MEMORY 8
REG_FIELD = GMX1_TX000_BURST burst 15 0
REG = GMX1_TX001_BURST 0x8001180010000A28 MEMORY 8
REG_FIELD = GMX1_TX001_BURST burst 15 0
REG = GMX1_TX002_BURST 0x8001180010001228 MEMORY 8
REG_FIELD = GMX1_TX002_BURST burst 15 0
REG = GMX1_TX003_BURST 0x8001180010001A28 MEMORY 8
REG_FIELD = GMX1_TX003_BURST burst 15 0

REG = GMX0_TX000_CBFC_XOFF 0x80011800080005A0 MEMORY 8
REG_FIELD = GMX0_TX000_CBFC_XOFF xoff 15 0
REG = GMX1_TX000_CBFC_XOFF 0x80011800100005A0 MEMORY 8
REG_FIELD = GMX1_TX000_CBFC_XOFF xoff 15 0

REG = GMX0_TX000_CBFC_XON 0x80011800080005C0 MEMORY 8
REG_FIELD = GMX0_TX000_CBFC_XON xon 15 0
REG = GMX1_TX000_CBFC_XON 0x80011800100005C0 MEMORY 8
REG_FIELD = GMX1_TX000_CBFC_XON xon 15 0

REG = GMX0_TX000_CTL 0x8001180008000270 MEMORY 8
REG_FIELD = GMX0_TX000_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX0_TX001_CTL 0x8001180008000A70 MEMORY 8
REG_FIELD = GMX0_TX001_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX0_TX002_CTL 0x8001180008001270 MEMORY 8
REG_FIELD = GMX0_TX002_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX0_TX003_CTL 0x8001180008001A70 MEMORY 8
REG_FIELD = GMX0_TX003_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX1_TX000_CTL 0x8001180010000270 MEMORY 8
REG_FIELD = GMX1_TX000_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX1_TX001_CTL 0x8001180010000A70 MEMORY 8
REG_FIELD = GMX1_TX001_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX1_TX002_CTL 0x8001180010001270 MEMORY 8
REG_FIELD = GMX1_TX002_CTL xsdef_en 1 1, xscol_en 0 0
REG = GMX1_TX003_CTL 0x8001180010001A70 MEMORY 8
REG_FIELD = GMX1_TX003_CTL xsdef_en 1 1, xscol_en 0 0

REG = GMX0_TX000_JAM_MODE 0x8001180008000380 MEMORY 8
REG_FIELD = GMX0_TX000_JAM_MODE mode 0 0
REG = GMX0_TX001_JAM_MODE 0x8001180008000B80 MEMORY 8
REG_FIELD = GMX0_TX001_JAM_MODE mode 0 0
REG = GMX0_TX002_JAM_MODE 0x8001180008001380 MEMORY 8
REG_FIELD = GMX0_TX002_JAM_MODE mode 0 0
REG = GMX0_TX003_JAM_MODE 0x8001180008001B80 MEMORY 8
REG_FIELD = GMX0_TX003_JAM_MODE mode 0 0
REG = GMX1_TX000_JAM_MODE 0x8001180010000380 MEMORY 8
REG_FIELD = GMX1_TX000_JAM_MODE mode 0 0
REG = GMX1_TX001_JAM_MODE 0x8001180010000B80 MEMORY 8
REG_FIELD = GMX1_TX001_JAM_MODE mode 0 0
REG = GMX1_TX002_JAM_MODE 0x8001180010001380 MEMORY 8
REG_FIELD = GMX1_TX002_JAM_MODE mode 0 0
REG = GMX1_TX003_JAM_MODE 0x8001180010001B80 MEMORY 8
REG_FIELD = GMX1_TX003_JAM_MODE mode 0 0

REG = GMX0_TX000_MIN_PKT 0x8001180008000240 MEMORY 8
REG_FIELD = GMX0_TX000_MIN_PKT min_size 7 0
REG = GMX0_TX001_MIN_PKT 0x8001180008000A40 MEMORY 8
REG_FIELD = GMX0_TX001_MIN_PKT min_size 7 0
REG = GMX0_TX002_MIN_PKT 0x8001180008001240 MEMORY 8
REG_FIELD = GMX0_TX002_MIN_PKT min_size 7 0
REG = GMX0_TX003_MIN_PKT 0x8001180008001A40 MEMORY 8
REG_FIELD = GMX0_TX003_MIN_PKT min_size 7 0
REG = GMX1_TX000_MIN_PKT 0x8001180010000240 MEMORY 8
REG_FIELD = GMX1_TX000_MIN_PKT min_size 7 0
REG = GMX1_TX001_MIN_PKT 0x8001180010000A40 MEMORY 8
REG_FIELD = GMX1_TX001_MIN_PKT min_size 7 0
REG = GMX1_TX002_MIN_PKT 0x8001180010001240 MEMORY 8
REG_FIELD = GMX1_TX002_MIN_PKT min_size 7 0
REG = GMX1_TX003_MIN_PKT 0x8001180010001A40 MEMORY 8
REG_FIELD = GMX1_TX003_MIN_PKT min_size 7 0

REG = GMX0_TX000_PAUSE_PKT_INTERVAL 0x8001180008000248 MEMORY 8
REG_FIELD = GMX0_TX000_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX0_TX001_PAUSE_PKT_INTERVAL 0x8001180008000A48 MEMORY 8
REG_FIELD = GMX0_TX001_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX0_TX002_PAUSE_PKT_INTERVAL 0x8001180008001248 MEMORY 8
REG_FIELD = GMX0_TX002_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX0_TX003_PAUSE_PKT_INTERVAL 0x8001180008001A48 MEMORY 8
REG_FIELD = GMX0_TX003_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX1_TX000_PAUSE_PKT_INTERVAL 0x8001180010000248 MEMORY 8
REG_FIELD = GMX1_TX000_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX1_TX001_PAUSE_PKT_INTERVAL 0x8001180010000A48 MEMORY 8
REG_FIELD = GMX1_TX001_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX1_TX002_PAUSE_PKT_INTERVAL 0x8001180010001248 MEMORY 8
REG_FIELD = GMX1_TX002_PAUSE_PKT_INTERVAL interval 15 0
REG = GMX1_TX003_PAUSE_PKT_INTERVAL 0x8001180010001A48 MEMORY 8
REG_FIELD = GMX1_TX003_PAUSE_PKT_INTERVAL interval 15 0

REG = GMX0_TX000_PAUSE_PKT_TIME 0x8001180008000238 MEMORY 8
REG_FIELD = GMX0_TX000_PAUSE_PKT_TIME time 15 0
REG = GMX0_TX001_PAUSE_PKT_TIME 0x8001180008000A38 MEMORY 8
REG_FIELD = GMX0_TX001_PAUSE_PKT_TIME time 15 0
REG = GMX0_TX002_PAUSE_PKT_TIME 0x8001180008001238 MEMORY 8
REG_FIELD = GMX0_TX002_PAUSE_PKT_TIME time 15 0
REG = GMX0_TX003_PAUSE_PKT_TIME 0x8001180008001A38 MEMORY 8
REG_FIELD = GMX0_TX003_PAUSE_PKT_TIME time 15 0
REG = GMX1_TX000_PAUSE_PKT_TIME 0x8001180010000238 MEMORY 8
REG_FIELD = GMX1_TX000_PAUSE_PKT_TIME time 15 0
REG = GMX1_TX001_PAUSE_PKT_TIME 0x8001180010000A38 MEMORY 8
REG_FIELD = GMX1_TX001_PAUSE_PKT_TIME time 15 0
REG = GMX1_TX002_PAUSE_PKT_TIME 0x8001180010001238 MEMORY 8
REG_FIELD = GMX1_TX002_PAUSE_PKT_TIME time 15 0
REG = GMX1_TX003_PAUSE_PKT_TIME 0x8001180010001A38 MEMORY 8
REG_FIELD = GMX1_TX003_PAUSE_PKT_TIME time 15 0

REG = GMX0_TX000_PAUSE_TOGO 0x8001180008000258 MEMORY 8
REG_FIELD = GMX0_TX000_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX0_TX001_PAUSE_TOGO 0x8001180008000A58 MEMORY 8
REG_FIELD = GMX0_TX001_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX0_TX002_PAUSE_TOGO 0x8001180008001258 MEMORY 8
REG_FIELD = GMX0_TX002_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX0_TX003_PAUSE_TOGO 0x8001180008001A58 MEMORY 8
REG_FIELD = GMX0_TX003_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX1_TX000_PAUSE_TOGO 0x8001180010000258 MEMORY 8
REG_FIELD = GMX1_TX000_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX1_TX001_PAUSE_TOGO 0x8001180010000A58 MEMORY 8
REG_FIELD = GMX1_TX001_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX1_TX002_PAUSE_TOGO 0x8001180010001258 MEMORY 8
REG_FIELD = GMX1_TX002_PAUSE_TOGO msg_time 31 16, time 15 0
REG = GMX1_TX003_PAUSE_TOGO 0x8001180010001A58 MEMORY 8
REG_FIELD = GMX1_TX003_PAUSE_TOGO msg_time 31 16, time 15 0

REG = GMX0_TX000_PAUSE_ZERO 0x8001180008000260 MEMORY 8
REG_FIELD = GMX0_TX000_PAUSE_ZERO send 0 0
REG = GMX0_TX001_PAUSE_ZERO 0x8001180008000A60 MEMORY 8
REG_FIELD = GMX0_TX001_PAUSE_ZERO send 0 0
REG = GMX0_TX002_PAUSE_ZERO 0x8001180008001260 MEMORY 8
REG_FIELD = GMX0_TX002_PAUSE_ZERO send 0 0
REG = GMX0_TX003_PAUSE_ZERO 0x8001180008001A60 MEMORY 8
REG_FIELD = GMX0_TX003_PAUSE_ZERO send 0 0
REG = GMX1_TX000_PAUSE_ZERO 0x8001180010000260 MEMORY 8
REG_FIELD = GMX1_TX000_PAUSE_ZERO send 0 0
REG = GMX1_TX001_PAUSE_ZERO 0x8001180010000A60 MEMORY 8
REG_FIELD = GMX1_TX001_PAUSE_ZERO send 0 0
REG = GMX1_TX002_PAUSE_ZERO 0x8001180010001260 MEMORY 8
REG_FIELD = GMX1_TX002_PAUSE_ZERO send 0 0
REG = GMX1_TX003_PAUSE_ZERO 0x8001180010001A60 MEMORY 8
REG_FIELD = GMX1_TX003_PAUSE_ZERO send 0 0

REG = GMX0_TX000_SGMII_CTL 0x8001180008000300 MEMORY 8
REG_FIELD = GMX0_TX000_SGMII_CTL align 0 0
REG = GMX0_TX001_SGMII_CTL 0x8001180008000B00 MEMORY 8
REG_FIELD = GMX0_TX001_SGMII_CTL align 0 0
REG = GMX0_TX002_SGMII_CTL 0x8001180008001300 MEMORY 8
REG_FIELD = GMX0_TX002_SGMII_CTL align 0 0
REG = GMX0_TX003_SGMII_CTL 0x8001180008001B00 MEMORY 8
REG_FIELD = GMX0_TX003_SGMII_CTL align 0 0
REG = GMX1_TX000_SGMII_CTL 0x8001180010000300 MEMORY 8
REG_FIELD = GMX1_TX000_SGMII_CTL align 0 0
REG = GMX1_TX001_SGMII_CTL 0x8001180010000B00 MEMORY 8
REG_FIELD = GMX1_TX001_SGMII_CTL align 0 0
REG = GMX1_TX002_SGMII_CTL 0x8001180010001300 MEMORY 8
REG_FIELD = GMX1_TX002_SGMII_CTL align 0 0
REG = GMX1_TX003_SGMII_CTL 0x8001180010001B00 MEMORY 8
REG_FIELD = GMX1_TX003_SGMII_CTL align 0 0

REG = GMX0_TX000_SLOT 0x8001180008000220 MEMORY 8
REG_FIELD = GMX0_TX000_SLOT slot 9 0
REG = GMX0_TX001_SLOT 0x8001180008000A20 MEMORY 8
REG_FIELD = GMX0_TX001_SLOT slot 9 0
REG = GMX0_TX002_SLOT 0x8001180008001220 MEMORY 8
REG_FIELD = GMX0_TX002_SLOT slot 9 0
REG = GMX0_TX003_SLOT 0x8001180008001A20 MEMORY 8
REG_FIELD = GMX0_TX003_SLOT slot 9 0
REG = GMX1_TX000_SLOT 0x8001180010000220 MEMORY 8
REG_FIELD = GMX1_TX000_SLOT slot 9 0
REG = GMX1_TX001_SLOT 0x8001180010000A20 MEMORY 8
REG_FIELD = GMX1_TX001_SLOT slot 9 0
REG = GMX1_TX002_SLOT 0x8001180010001220 MEMORY 8
REG_FIELD = GMX1_TX002_SLOT slot 9 0
REG = GMX1_TX003_SLOT 0x8001180010001A20 MEMORY 8
REG_FIELD = GMX1_TX003_SLOT slot 9 0

REG = GMX0_TX000_SOFT_PAUSE 0x8001180008000250 MEMORY 8
REG_FIELD = GMX0_TX000_SOFT_PAUSE time 15 0
REG = GMX0_TX001_SOFT_PAUSE 0x8001180008000A50 MEMORY 8
REG_FIELD = GMX0_TX001_SOFT_PAUSE time 15 0
REG = GMX0_TX002_SOFT_PAUSE 0x8001180008001250 MEMORY 8
REG_FIELD = GMX0_TX002_SOFT_PAUSE time 15 0
REG = GMX0_TX003_SOFT_PAUSE 0x8001180008001A50 MEMORY 8
REG_FIELD = GMX0_TX003_SOFT_PAUSE time 15 0
REG = GMX1_TX000_SOFT_PAUSE 0x8001180010000250 MEMORY 8
REG_FIELD = GMX1_TX000_SOFT_PAUSE time 15 0
REG = GMX1_TX001_SOFT_PAUSE 0x8001180010000A50 MEMORY 8
REG_FIELD = GMX1_TX001_SOFT_PAUSE time 15 0
REG = GMX1_TX002_SOFT_PAUSE 0x8001180010001250 MEMORY 8
REG_FIELD = GMX1_TX002_SOFT_PAUSE time 15 0
REG = GMX1_TX003_SOFT_PAUSE 0x8001180010001A50 MEMORY 8
REG_FIELD = GMX1_TX003_SOFT_PAUSE time 15 0

REG = GMX0_TX000_STAT0 0x8001180008000280 MEMORY 8
REG_FIELD = GMX0_TX000_STAT0 xsdef 63 32, xscol 31 0
REG = GMX0_TX001_STAT0 0x8001180008000A80 MEMORY 8
REG_FIELD = GMX0_TX001_STAT0 xsdef 63 32, xscol 31 0
REG = GMX0_TX002_STAT0 0x8001180008001280 MEMORY 8
REG_FIELD = GMX0_TX002_STAT0 xsdef 63 32, xscol 31 0
REG = GMX0_TX003_STAT0 0x8001180008001A80 MEMORY 8
REG_FIELD = GMX0_TX003_STAT0 xsdef 63 32, xscol 31 0
REG = GMX1_TX000_STAT0 0x8001180010000280 MEMORY 8
REG_FIELD = GMX1_TX000_STAT0 xsdef 63 32, xscol 31 0
REG = GMX1_TX001_STAT0 0x8001180010000A80 MEMORY 8
REG_FIELD = GMX1_TX001_STAT0 xsdef 63 32, xscol 31 0
REG = GMX1_TX002_STAT0 0x8001180010001280 MEMORY 8
REG_FIELD = GMX1_TX002_STAT0 xsdef 63 32, xscol 31 0
REG = GMX1_TX003_STAT0 0x8001180010001A80 MEMORY 8
REG_FIELD = GMX1_TX003_STAT0 xsdef 63 32, xscol 31 0

REG = GMX0_TX000_STAT1 0x8001180008000288 MEMORY 8
REG_FIELD = GMX0_TX000_STAT1 scol 63 32, mcol 31 0
REG = GMX0_TX001_STAT1 0x8001180008000A88 MEMORY 8
REG_FIELD = GMX0_TX001_STAT1 scol 63 32, mcol 31 0
REG = GMX0_TX002_STAT1 0x8001180008001288 MEMORY 8
REG_FIELD = GMX0_TX002_STAT1 scol 63 32, mcol 31 0
REG = GMX0_TX003_STAT1 0x8001180008001A88 MEMORY 8
REG_FIELD = GMX0_TX003_STAT1 scol 63 32, mcol 31 0
REG = GMX1_TX000_STAT1 0x8001180010000288 MEMORY 8
REG_FIELD = GMX1_TX000_STAT1 scol 63 32, mcol 31 0
REG = GMX1_TX001_STAT1 0x8001180010000A88 MEMORY 8
REG_FIELD = GMX1_TX001_STAT1 scol 63 32, mcol 31 0
REG = GMX1_TX002_STAT1 0x8001180010001288 MEMORY 8
REG_FIELD = GMX1_TX002_STAT1 scol 63 32, mcol 31 0
REG = GMX1_TX003_STAT1 0x8001180010001A88 MEMORY 8
REG_FIELD = GMX1_TX003_STAT1 scol 63 32, mcol 31 0

REG = GMX0_TX000_STAT2 0x8001180008000290 MEMORY 8
REG_FIELD = GMX0_TX000_STAT2 octs 47 0
REG = GMX0_TX001_STAT2 0x8001180008000A90 MEMORY 8
REG_FIELD = GMX0_TX001_STAT2 octs 47 0
REG = GMX0_TX002_STAT2 0x8001180008001290 MEMORY 8
REG_FIELD = GMX0_TX002_STAT2 octs 47 0
REG = GMX0_TX003_STAT2 0x8001180008001A90 MEMORY 8
REG_FIELD = GMX0_TX003_STAT2 octs 47 0
REG = GMX1_TX000_STAT2 0x8001180010000290 MEMORY 8
REG_FIELD = GMX1_TX000_STAT2 octs 47 0
REG = GMX1_TX001_STAT2 0x8001180010000A90 MEMORY 8
REG_FIELD = GMX1_TX001_STAT2 octs 47 0
REG = GMX1_TX002_STAT2 0x8001180010001290 MEMORY 8
REG_FIELD = GMX1_TX002_STAT2 octs 47 0
REG = GMX1_TX003_STAT2 0x8001180010001A90 MEMORY 8
REG_FIELD = GMX1_TX003_STAT2 octs 47 0

REG = GMX0_TX000_STAT3 0x8001180008000298 MEMORY 8
REG_FIELD = GMX0_TX000_STAT3 pkts 31 0
REG = GMX0_TX001_STAT3 0x8001180008000A98 MEMORY 8
REG_FIELD = GMX0_TX001_STAT3 pkts 31 0
REG = GMX0_TX002_STAT3 0x8001180008001298 MEMORY 8
REG_FIELD = GMX0_TX002_STAT3 pkts 31 0
REG = GMX0_TX003_STAT3 0x8001180008001A98 MEMORY 8
REG_FIELD = GMX0_TX003_STAT3 pkts 31 0
REG = GMX1_TX000_STAT3 0x8001180010000298 MEMORY 8
REG_FIELD = GMX1_TX000_STAT3 pkts 31 0
REG = GMX1_TX001_STAT3 0x8001180010000A98 MEMORY 8
REG_FIELD = GMX1_TX001_STAT3 pkts 31 0
REG = GMX1_TX002_STAT3 0x8001180010001298 MEMORY 8
REG_FIELD = GMX1_TX002_STAT3 pkts 31 0
REG = GMX1_TX003_STAT3 0x8001180010001A98 MEMORY 8
REG_FIELD = GMX1_TX003_STAT3 pkts 31 0

REG = GMX0_TX000_STAT4 0x80011800080002A0 MEMORY 8
REG_FIELD = GMX0_TX000_STAT4 hist1 63 32, hist0 31 0
REG = GMX0_TX001_STAT4 0x8001180008000AA0 MEMORY 8
REG_FIELD = GMX0_TX001_STAT4 hist1 63 32, hist0 31 0
REG = GMX0_TX002_STAT4 0x80011800080012A0 MEMORY 8
REG_FIELD = GMX0_TX002_STAT4 hist1 63 32, hist0 31 0
REG = GMX0_TX003_STAT4 0x8001180008001AA0 MEMORY 8
REG_FIELD = GMX0_TX003_STAT4 hist1 63 32, hist0 31 0
REG = GMX1_TX000_STAT4 0x80011800100002A0 MEMORY 8
REG_FIELD = GMX1_TX000_STAT4 hist1 63 32, hist0 31 0
REG = GMX1_TX001_STAT4 0x8001180010000AA0 MEMORY 8
REG_FIELD = GMX1_TX001_STAT4 hist1 63 32, hist0 31 0
REG = GMX1_TX002_STAT4 0x80011800100012A0 MEMORY 8
REG_FIELD = GMX1_TX002_STAT4 hist1 63 32, hist0 31 0
REG = GMX1_TX003_STAT4 0x8001180010001AA0 MEMORY 8
REG_FIELD = GMX1_TX003_STAT4 hist1 63 32, hist0 31 0

REG = GMX0_TX000_STAT5 0x80011800080002A8 MEMORY 8
REG_FIELD = GMX0_TX000_STAT5 hist3 63 32, hist2 31 0
REG = GMX0_TX001_STAT5 0x8001180008000AA8 MEMORY 8
REG_FIELD = GMX0_TX001_STAT5 hist3 63 32, hist2 31 0
REG = GMX0_TX002_STAT5 0x80011800080012A8 MEMORY 8
REG_FIELD = GMX0_TX002_STAT5 hist3 63 32, hist2 31 0
REG = GMX0_TX003_STAT5 0x8001180008001AA8 MEMORY 8
REG_FIELD = GMX0_TX003_STAT5 hist3 63 32, hist2 31 0
REG = GMX1_TX000_STAT5 0x80011800100002A8 MEMORY 8
REG_FIELD = GMX1_TX000_STAT5 hist3 63 32, hist2 31 0
REG = GMX1_TX001_STAT5 0x8001180010000AA8 MEMORY 8
REG_FIELD = GMX1_TX001_STAT5 hist3 63 32, hist2 31 0
REG = GMX1_TX002_STAT5 0x80011800100012A8 MEMORY 8
REG_FIELD = GMX1_TX002_STAT5 hist3 63 32, hist2 31 0
REG = GMX1_TX003_STAT5 0x8001180010001AA8 MEMORY 8
REG_FIELD = GMX1_TX003_STAT5 hist3 63 32, hist2 31 0

REG = GMX0_TX000_STAT6 0x80011800080002B0 MEMORY 8
REG_FIELD = GMX0_TX000_STAT6 hist5 63 32, hist4 31 0
REG = GMX0_TX001_STAT6 0x8001180008000AB0 MEMORY 8
REG_FIELD = GMX0_TX001_STAT6 hist5 63 32, hist4 31 0
REG = GMX0_TX002_STAT6 0x80011800080012B0 MEMORY 8
REG_FIELD = GMX0_TX002_STAT6 hist5 63 32, hist4 31 0
REG = GMX0_TX003_STAT6 0x8001180008001AB0 MEMORY 8
REG_FIELD = GMX0_TX003_STAT6 hist5 63 32, hist4 31 0
REG = GMX1_TX000_STAT6 0x80011800100002B0 MEMORY 8
REG_FIELD = GMX1_TX000_STAT6 hist5 63 32, hist4 31 0
REG = GMX1_TX001_STAT6 0x8001180010000AB0 MEMORY 8
REG_FIELD = GMX1_TX001_STAT6 hist5 63 32, hist4 31 0
REG = GMX1_TX002_STAT6 0x80011800100012B0 MEMORY 8
REG_FIELD = GMX1_TX002_STAT6 hist5 63 32, hist4 31 0
REG = GMX1_TX003_STAT6 0x8001180010001AB0 MEMORY 8
REG_FIELD = GMX1_TX003_STAT6 hist5 63 32, hist4 31 0

REG = GMX0_TX000_STAT7 0x80011800080002B8 MEMORY 8
REG_FIELD = GMX0_TX000_STAT7 hist7 63 32, hist6 31 0
REG = GMX0_TX001_STAT7 0x8001180008000AB8 MEMORY 8
REG_FIELD = GMX0_TX001_STAT7 hist7 63 32, hist6 31 0
REG = GMX0_TX002_STAT7 0x80011800080012B8 MEMORY 8
REG_FIELD = GMX0_TX002_STAT7 hist7 63 32, hist6 31 0
REG = GMX0_TX003_STAT7 0x8001180008001AB8 MEMORY 8
REG_FIELD = GMX0_TX003_STAT7 hist7 63 32, hist6 31 0
REG = GMX1_TX000_STAT7 0x80011800100002B8 MEMORY 8
REG_FIELD = GMX1_TX000_STAT7 hist7 63 32, hist6 31 0
REG = GMX1_TX001_STAT7 0x8001180010000AB8 MEMORY 8
REG_FIELD = GMX1_TX001_STAT7 hist7 63 32, hist6 31 0
REG = GMX1_TX002_STAT7 0x80011800100012B8 MEMORY 8
REG_FIELD = GMX1_TX002_STAT7 hist7 63 32, hist6 31 0
REG = GMX1_TX003_STAT7 0x8001180010001AB8 MEMORY 8
REG_FIELD = GMX1_TX003_STAT7 hist7 63 32, hist6 31 0

REG = GMX0_TX000_STAT8 0x80011800080002C0 MEMORY 8
REG_FIELD = GMX0_TX000_STAT8 mcst 63 32, bcst 31 0
REG = GMX0_TX001_STAT8 0x8001180008000AC0 MEMORY 8
REG_FIELD = GMX0_TX001_STAT8 mcst 63 32, bcst 31 0
REG = GMX0_TX002_STAT8 0x80011800080012C0 MEMORY 8
REG_FIELD = GMX0_TX002_STAT8 mcst 63 32, bcst 31 0
REG = GMX0_TX003_STAT8 0x8001180008001AC0 MEMORY 8
REG_FIELD = GMX0_TX003_STAT8 mcst 63 32, bcst 31 0
REG = GMX1_TX000_STAT8 0x80011800100002C0 MEMORY 8
REG_FIELD = GMX1_TX000_STAT8 mcst 63 32, bcst 31 0
REG = GMX1_TX001_STAT8 0x8001180010000AC0 MEMORY 8
REG_FIELD = GMX1_TX001_STAT8 mcst 63 32, bcst 31 0
REG = GMX1_TX002_STAT8 0x80011800100012C0 MEMORY 8
REG_FIELD = GMX1_TX002_STAT8 mcst 63 32, bcst 31 0
REG = GMX1_TX003_STAT8 0x8001180010001AC0 MEMORY 8
REG_FIELD = GMX1_TX003_STAT8 mcst 63 32, bcst 31 0

REG = GMX0_TX000_STAT9 0x80011800080002C8 MEMORY 8
REG_FIELD = GMX0_TX000_STAT9 undflw 63 32, ctl 31 0
REG = GMX0_TX001_STAT9 0x8001180008000AC8 MEMORY 8
REG_FIELD = GMX0_TX001_STAT9 undflw 63 32, ctl 31 0
REG = GMX0_TX002_STAT9 0x80011800080012C8 MEMORY 8
REG_FIELD = GMX0_TX002_STAT9 undflw 63 32, ctl 31 0
REG = GMX0_TX003_STAT9 0x8001180008001AC8 MEMORY 8
REG_FIELD = GMX0_TX003_STAT9 undflw 63 32, ctl 31 0
REG = GMX1_TX000_STAT9 0x80011800100002C8 MEMORY 8
REG_FIELD = GMX1_TX000_STAT9 undflw 63 32, ctl 31 0
REG = GMX1_TX001_STAT9 0x8001180010000AC8 MEMORY 8
REG_FIELD = GMX1_TX001_STAT9 undflw 63 32, ctl 31 0
REG = GMX1_TX002_STAT9 0x80011800100012C8 MEMORY 8
REG_FIELD = GMX1_TX002_STAT9 undflw 63 32, ctl 31 0
REG = GMX1_TX003_STAT9 0x8001180010001AC8 MEMORY 8
REG_FIELD = GMX1_TX003_STAT9 undflw 63 32, ctl 31 0

REG = GMX0_TX000_STATS_CTL 0x8001180008000268 MEMORY 8
REG_FIELD = GMX0_TX000_STATS_CTL rd_clr 0 0
REG = GMX0_TX001_STATS_CTL 0x8001180008000A68 MEMORY 8
REG_FIELD = GMX0_TX001_STATS_CTL rd_clr 0 0
REG = GMX0_TX002_STATS_CTL 0x8001180008001268 MEMORY 8
REG_FIELD = GMX0_TX002_STATS_CTL rd_clr 0 0
REG = GMX0_TX003_STATS_CTL 0x8001180008001A68 MEMORY 8
REG_FIELD = GMX0_TX003_STATS_CTL rd_clr 0 0
REG = GMX1_TX000_STATS_CTL 0x8001180010000268 MEMORY 8
REG_FIELD = GMX1_TX000_STATS_CTL rd_clr 0 0
REG = GMX1_TX001_STATS_CTL 0x8001180010000A68 MEMORY 8
REG_FIELD = GMX1_TX001_STATS_CTL rd_clr 0 0
REG = GMX1_TX002_STATS_CTL 0x8001180010001268 MEMORY 8
REG_FIELD = GMX1_TX002_STATS_CTL rd_clr 0 0
REG = GMX1_TX003_STATS_CTL 0x8001180010001A68 MEMORY 8
REG_FIELD = GMX1_TX003_STATS_CTL rd_clr 0 0

REG = GMX0_TX000_THRESH 0x8001180008000210 MEMORY 8
REG_FIELD = GMX0_TX000_THRESH cnt 8 0
REG = GMX0_TX001_THRESH 0x8001180008000A10 MEMORY 8
REG_FIELD = GMX0_TX001_THRESH cnt 8 0
REG = GMX0_TX002_THRESH 0x8001180008001210 MEMORY 8
REG_FIELD = GMX0_TX002_THRESH cnt 8 0
REG = GMX0_TX003_THRESH 0x8001180008001A10 MEMORY 8
REG_FIELD = GMX0_TX003_THRESH cnt 8 0
REG = GMX1_TX000_THRESH 0x8001180010000210 MEMORY 8
REG_FIELD = GMX1_TX000_THRESH cnt 8 0
REG = GMX1_TX001_THRESH 0x8001180010000A10 MEMORY 8
REG_FIELD = GMX1_TX001_THRESH cnt 8 0
REG = GMX1_TX002_THRESH 0x8001180010001210 MEMORY 8
REG_FIELD = GMX1_TX002_THRESH cnt 8 0
REG = GMX1_TX003_THRESH 0x8001180010001A10 MEMORY 8
REG_FIELD = GMX1_TX003_THRESH cnt 8 0

REG = GMX0_TX_BP 0x80011800080004D0 MEMORY 8
REG_FIELD = GMX0_TX_BP bp 3 0
REG = GMX1_TX_BP 0x80011800100004D0 MEMORY 8
REG_FIELD = GMX1_TX_BP bp 3 0

REG = GMX0_TX_COL_ATTEMPT 0x8001180008000498 MEMORY 8
REG_FIELD = GMX0_TX_COL_ATTEMPT limit 4 0
REG = GMX1_TX_COL_ATTEMPT 0x8001180010000498 MEMORY 8
REG_FIELD = GMX1_TX_COL_ATTEMPT limit 4 0

REG = GMX0_TX_CORRUPT 0x80011800080004D8 MEMORY 8
REG_FIELD = GMX0_TX_CORRUPT corrupt 3 0
REG = GMX1_TX_CORRUPT 0x80011800100004D8 MEMORY 8
REG_FIELD = GMX1_TX_CORRUPT corrupt 3 0

REG = GMX0_TX_HG2_REG1 0x8001180008000558 MEMORY 8
REG_FIELD = GMX0_TX_HG2_REG1 tx_xof 15 0
REG = GMX1_TX_HG2_REG1 0x8001180010000558 MEMORY 8
REG_FIELD = GMX1_TX_HG2_REG1 tx_xof 15 0

REG = GMX0_TX_HG2_REG2 0x8001180008000560 MEMORY 8
REG_FIELD = GMX0_TX_HG2_REG2 tx_xon 15 0
REG = GMX1_TX_HG2_REG2 0x8001180010000560 MEMORY 8
REG_FIELD = GMX1_TX_HG2_REG2 tx_xon 15 0

REG = GMX0_TX_IFG 0x8001180008000488 MEMORY 8
REG_FIELD = GMX0_TX_IFG ifg2 7 4, ifg1 3 0
REG = GMX1_TX_IFG 0x8001180010000488 MEMORY 8
REG_FIELD = GMX1_TX_IFG ifg2 7 4, ifg1 3 0

REG = GMX0_TX_INT_EN 0x8001180008000508 MEMORY 8
REG_FIELD = GMX0_TX_INT_EN xchange 24 24, ptp_lost 23 20, late_col 19 16, xsdef 15 12, xscol 11 8, undflw 5 2, pko_nxa 0 0
REG = GMX1_TX_INT_EN 0x8001180010000508 MEMORY 8
REG_FIELD = GMX1_TX_INT_EN xchange 24 24, ptp_lost 23 20, late_col 19 16, xsdef 15 12, xscol 11 8, undflw 5 2, pko_nxa 0 0

REG = GMX0_TX_INT_REG 0x8001180008000500 MEMORY 8
REG_FIELD = GMX0_TX_INT_REG xchange 24 24, ptp_lost 23 20, late_col 19 16, xsdef 15 12, xscol 11 8, undflw 5 2, pko_nxa 0 0
REG = GMX1_TX_INT_REG 0x8001180010000500 MEMORY 8
REG_FIELD = GMX1_TX_INT_REG xchange 24 24, ptp_lost 23 20, late_col 19 16, xsdef 15 12, xscol 11 8, undflw 5 2, pko_nxa 0 0

REG = GMX0_TX_JAM 0x8001180008000490 MEMORY 8
REG_FIELD = GMX0_TX_JAM jam 7 0
REG = GMX1_TX_JAM 0x8001180010000490 MEMORY 8
REG_FIELD = GMX1_TX_JAM jam 7 0

REG = GMX0_TX_LFSR 0x80011800080004F8 MEMORY 8
REG_FIELD = GMX0_TX_LFSR lfsr 15 0
REG = GMX1_TX_LFSR 0x80011800100004F8 MEMORY 8
REG_FIELD = GMX1_TX_LFSR lfsr 15 0

REG = GMX0_TX_OVR_BP 0x80011800080004C8 MEMORY 8
REG_FIELD = GMX0_TX_OVR_BP tx_prt_bp 47 32, en 11 8, bp 7 4, ign_full 3 0
REG = GMX1_TX_OVR_BP 0x80011800100004C8 MEMORY 8
REG_FIELD = GMX1_TX_OVR_BP tx_prt_bp 47 32, en 11 8, bp 7 4, ign_full 3 0

REG = GMX0_TX_PAUSE_PKT_DMAC 0x80011800080004A0 MEMORY 8
REG_FIELD = GMX0_TX_PAUSE_PKT_DMAC dmac 47 0
REG = GMX1_TX_PAUSE_PKT_DMAC 0x80011800100004A0 MEMORY 8
REG_FIELD = GMX1_TX_PAUSE_PKT_DMAC dmac 47 0

REG = GMX0_TX_PAUSE_PKT_TYPE 0x80011800080004A8 MEMORY 8
REG_FIELD = GMX0_TX_PAUSE_PKT_TYPE type 15 0
REG = GMX1_TX_PAUSE_PKT_TYPE 0x80011800100004A8 MEMORY 8
REG_FIELD = GMX1_TX_PAUSE_PKT_TYPE type 15 0

REG = GMX0_TX_PRTS 0x8001180008000480 MEMORY 8
REG_FIELD = GMX0_TX_PRTS prts 4 0
REG = GMX1_TX_PRTS 0x8001180010000480 MEMORY 8
REG_FIELD = GMX1_TX_PRTS prts 4 0

REG = GMX0_TX_XAUI_CTL 0x8001180008000528 MEMORY 8
REG_FIELD = GMX0_TX_XAUI_CTL hg_pause_hgi 10 9, hg_en 8 8, ls_byp 6 6, ls 5 4, uni_en 1 1, dic_en 0 0
REG = GMX1_TX_XAUI_CTL 0x8001180010000528 MEMORY 8
REG_FIELD = GMX1_TX_XAUI_CTL hg_pause_hgi 10 9, hg_en 8 8, ls_byp 6 6, ls 5 4, uni_en 1 1, dic_en 0 0

REG = GMX0_WOL_CTL 0x8001180008000780 MEMORY 8
REG_FIELD = GMX0_WOL_CTL magic_en 35 32, direct_en 19 16, en 0 0
REG = GMX1_WOL_CTL 0x8001180010000780 MEMORY 8
REG_FIELD = GMX1_WOL_CTL magic_en 35 32, direct_en 19 16, en 0 0

REG = GMX0_XAUI_EXT_LOOPBACK 0x8001180008000540 MEMORY 8
REG_FIELD = GMX0_XAUI_EXT_LOOPBACK en 4 4, thresh 3 0
REG = GMX1_XAUI_EXT_LOOPBACK 0x8001180010000540 MEMORY 8
REG_FIELD = GMX1_XAUI_EXT_LOOPBACK en 4 4, thresh 3 0

REG = GPIO_BIT_CFG0 0x8001070000000800 MEMORY 8
REG_FIELD = GPIO_BIT_CFG0 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG1 0x8001070000000808 MEMORY 8
REG_FIELD = GPIO_BIT_CFG1 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG2 0x8001070000000810 MEMORY 8
REG_FIELD = GPIO_BIT_CFG2 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG3 0x8001070000000818 MEMORY 8
REG_FIELD = GPIO_BIT_CFG3 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG4 0x8001070000000820 MEMORY 8
REG_FIELD = GPIO_BIT_CFG4 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG5 0x8001070000000828 MEMORY 8
REG_FIELD = GPIO_BIT_CFG5 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG6 0x8001070000000830 MEMORY 8
REG_FIELD = GPIO_BIT_CFG6 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG7 0x8001070000000838 MEMORY 8
REG_FIELD = GPIO_BIT_CFG7 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG8 0x8001070000000840 MEMORY 8
REG_FIELD = GPIO_BIT_CFG8 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG9 0x8001070000000848 MEMORY 8
REG_FIELD = GPIO_BIT_CFG9 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG10 0x8001070000000850 MEMORY 8
REG_FIELD = GPIO_BIT_CFG10 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG11 0x8001070000000858 MEMORY 8
REG_FIELD = GPIO_BIT_CFG11 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG12 0x8001070000000860 MEMORY 8
REG_FIELD = GPIO_BIT_CFG12 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG13 0x8001070000000868 MEMORY 8
REG_FIELD = GPIO_BIT_CFG13 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG14 0x8001070000000870 MEMORY 8
REG_FIELD = GPIO_BIT_CFG14 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_BIT_CFG15 0x8001070000000878 MEMORY 8
REG_FIELD = GPIO_BIT_CFG15 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0

REG = GPIO_CLK_GEN0 0x80010700000008C0 MEMORY 8
REG_FIELD = GPIO_CLK_GEN0 n 31 0
REG = GPIO_CLK_GEN1 0x80010700000008C8 MEMORY 8
REG_FIELD = GPIO_CLK_GEN1 n 31 0
REG = GPIO_CLK_GEN2 0x80010700000008D0 MEMORY 8
REG_FIELD = GPIO_CLK_GEN2 n 31 0
REG = GPIO_CLK_GEN3 0x80010700000008D8 MEMORY 8
REG_FIELD = GPIO_CLK_GEN3 n 31 0

REG = GPIO_CLK_SYNCE0 0x80010700000008E0 MEMORY 8
REG_FIELD = GPIO_CLK_SYNCE0 dlm_sel 9 8, div 2 2, lane_sel 0 0
REG = GPIO_CLK_SYNCE1 0x80010700000008E8 MEMORY 8
REG_FIELD = GPIO_CLK_SYNCE1 dlm_sel 9 8, div 2 2, lane_sel 0 0

REG = GPIO_INT_CLR 0x8001070000000898 MEMORY 8
REG_FIELD = GPIO_INT_CLR type 15 0

REG = GPIO_MULTI_CAST 0x80010700000008B0 MEMORY 8
REG_FIELD = GPIO_MULTI_CAST en 0 0

REG = GPIO_OCLA_EXTEN_TRIG 0x80010700000008B8 MEMORY 8
REG_FIELD = GPIO_OCLA_EXTEN_TRIG m_trig 5 5, sel 4 0

REG = GPIO_RX_DAT 0x8001070000000880 MEMORY 8
REG_FIELD = GPIO_RX_DAT dat 19 0

REG = GPIO_SATA_CTL 0x80010700000008A8 MEMORY 8
REG_FIELD = GPIO_SATA_CTL sel4 24 20, sel3 19 15, sel2 14 10, sel1 9 5, sel0 4 0

REG = GPIO_TX_CLR 0x8001070000000890 MEMORY 8
REG_FIELD = GPIO_TX_CLR clr 19 0

REG = GPIO_TX_SET 0x8001070000000888 MEMORY 8
REG_FIELD = GPIO_TX_SET set 19 0

REG = GPIO_USBH_CTL 0x80010700000008A0 MEMORY 8
REG_FIELD = GPIO_USBH_CTL sel 12 8, sel1 4 0

REG = GPIO_XBIT_CFG16 0x8001070000000900 MEMORY 8
REG_FIELD = GPIO_XBIT_CFG16 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_XBIT_CFG17 0x8001070000000908 MEMORY 8
REG_FIELD = GPIO_XBIT_CFG17 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_XBIT_CFG18 0x8001070000000910 MEMORY 8
REG_FIELD = GPIO_XBIT_CFG18 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0
REG = GPIO_XBIT_CFG19 0x8001070000000918 MEMORY 8
REG_FIELD = GPIO_XBIT_CFG19 output_sel 20 16, fil_sel 11 8, fil_cnt 7 4, int_type 3 3, int_en 2 2, rx_xor 1 1, tx_oe 0 0

REG = GSER0_DLM000_LOOPBK_EN 0x8001180090001008 MEMORY 8
REG_FIELD = GSER0_DLM000_LOOPBK_EN lane1_loopbk_en 8 8, lane0_loopbk_en 0 0

REG = GSER0_DLM000_LOS_BIAS 0x8001180090001010 MEMORY 8
REG_FIELD = GSER0_DLM000_LOS_BIAS los_bias 2 0
REG = GSER0_DLM001_LOS_BIAS 0x8001180090081010 MEMORY 8
REG_FIELD = GSER0_DLM001_LOS_BIAS los_bias 2 0
REG = GSER0_DLM002_LOS_BIAS 0x8001180090101010 MEMORY 8
REG_FIELD = GSER0_DLM002_LOS_BIAS los_bias 2 0

REG = GSER0_DLM000_LOS_LEVEL 0x8001180090001018 MEMORY 8
REG_FIELD = GSER0_DLM000_LOS_LEVEL los_level 4 0
REG = GSER0_DLM001_LOS_LEVEL 0x8001180090081018 MEMORY 8
REG_FIELD = GSER0_DLM001_LOS_LEVEL los_level 4 0
REG = GSER0_DLM002_LOS_LEVEL 0x8001180090101018 MEMORY 8
REG_FIELD = GSER0_DLM002_LOS_LEVEL los_level 4 0

REG = GSER0_DLM000_MISC_STATUS 0x8001180090000000 MEMORY 8
REG_FIELD = GSER0_DLM000_MISC_STATUS tx1_uflow 8 8, tx0_uflow 0 0

REG = GSER0_DLM000_MPLL_EN 0x8001180090001020 MEMORY 8
REG_FIELD = GSER0_DLM000_MPLL_EN mpll_en 0 0

REG = GSER0_DLM000_MPLL_HALF_RATE 0x8001180090001028 MEMORY 8
REG_FIELD = GSER0_DLM000_MPLL_HALF_RATE mpll_half_rate 0 0
REG = GSER0_DLM001_MPLL_HALF_RATE 0x8001180090081028 MEMORY 8
REG_FIELD = GSER0_DLM001_MPLL_HALF_RATE mpll_half_rate 0 0
REG = GSER0_DLM002_MPLL_HALF_RATE 0x8001180090101028 MEMORY 8
REG_FIELD = GSER0_DLM002_MPLL_HALF_RATE mpll_half_rate 0 0

REG = GSER0_DLM000_MPLL_MULTIPLIER 0x8001180090001030 MEMORY 8
REG_FIELD = GSER0_DLM000_MPLL_MULTIPLIER mpll_multiplier 6 0
REG = GSER0_DLM001_MPLL_MULTIPLIER 0x8001180090081030 MEMORY 8
REG_FIELD = GSER0_DLM001_MPLL_MULTIPLIER mpll_multiplier 6 0
REG = GSER0_DLM002_MPLL_MULTIPLIER 0x8001180090101030 MEMORY 8
REG_FIELD = GSER0_DLM002_MPLL_MULTIPLIER mpll_multiplier 6 0

REG = GSER0_DLM000_MPLL_STATUS 0x8001180090001000 MEMORY 8
REG_FIELD = GSER0_DLM000_MPLL_STATUS mpll_status 0 0
REG = GSER0_DLM001_MPLL_STATUS 0x8001180090081000 MEMORY 8
REG_FIELD = GSER0_DLM001_MPLL_STATUS mpll_status 0 0
REG = GSER0_DLM002_MPLL_STATUS 0x8001180090101000 MEMORY 8
REG_FIELD = GSER0_DLM002_MPLL_STATUS mpll_status 0 0

REG = GSER0_DLM000_PHY_RESET 0x8001180090001038 MEMORY 8
REG_FIELD = GSER0_DLM000_PHY_RESET phy_reset 0 0
REG = GSER0_DLM001_PHY_RESET 0x8001180090081038 MEMORY 8
REG_FIELD = GSER0_DLM001_PHY_RESET phy_reset 0 0
REG = GSER0_DLM002_PHY_RESET 0x8001180090101038 MEMORY 8
REG_FIELD = GSER0_DLM002_PHY_RESET phy_reset 0 0

REG = GSER0_DLM000_REF_CLKDIV2 0x8001180090001040 MEMORY 8
REG_FIELD = GSER0_DLM000_REF_CLKDIV2 ref_clkdiv2 0 0
REG = GSER0_DLM001_REF_CLKDIV2 0x8001180090081040 MEMORY 8
REG_FIELD = GSER0_DLM001_REF_CLKDIV2 ref_clkdiv2 0 0
REG = GSER0_DLM002_REF_CLKDIV2 0x8001180090101040 MEMORY 8
REG_FIELD = GSER0_DLM002_REF_CLKDIV2 ref_clkdiv2 0 0

REG = GSER0_DLM000_REF_SSP_EN 0x8001180090001048 MEMORY 8
REG_FIELD = GSER0_DLM000_REF_SSP_EN ref_ssp_en 0 0

REG = GSER0_DLM000_REF_USE_PAD 0x8001180090001050 MEMORY 8
REG_FIELD = GSER0_DLM000_REF_USE_PAD ref_use_pad 0 0
REG = GSER0_DLM001_REF_USE_PAD 0x8001180090081050 MEMORY 8
REG_FIELD = GSER0_DLM001_REF_USE_PAD ref_use_pad 0 0
REG = GSER0_DLM002_REF_USE_PAD 0x8001180090101050 MEMORY 8
REG_FIELD = GSER0_DLM002_REF_USE_PAD ref_use_pad 0 0

REG = GSER0_DLM000_REFCLK_SEL 0x8001180090000008 MEMORY 8
REG_FIELD = GSER0_DLM000_REFCLK_SEL refclk_sel 0 0
REG = GSER0_DLM001_REFCLK_SEL 0x8001180090080008 MEMORY 8
REG_FIELD = GSER0_DLM001_REFCLK_SEL refclk_sel 0 0
REG = GSER0_DLM002_REFCLK_SEL 0x8001180090100008 MEMORY 8
REG_FIELD = GSER0_DLM002_REFCLK_SEL refclk_sel 0 0

REG = GSER0_DLM000_RX_DATA_EN 0x8001180090002008 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_DATA_EN rx1_data_en 8 8, rx0_data_en 0 0

REG = GSER0_DLM000_RX_EQ 0x8001180090002010 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_EQ rx1_eq 10 8, rx0_eq 2 0
REG = GSER0_DLM001_RX_EQ 0x8001180090082010 MEMORY 8
REG_FIELD = GSER0_DLM001_RX_EQ rx1_eq 10 8, rx0_eq 2 0
REG = GSER0_DLM002_RX_EQ 0x8001180090102010 MEMORY 8
REG_FIELD = GSER0_DLM002_RX_EQ rx1_eq 10 8, rx0_eq 2 0

REG = GSER0_DLM000_RX_LOS_EN 0x8001180090002018 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_LOS_EN rx1_los_en 8 8, rx0_los_en 0 0

REG = GSER0_DLM000_RX_PLL_EN 0x8001180090002020 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_PLL_EN rx1_pll_en 8 8, rx0_pll_en 0 0

REG = GSER0_DLM000_RX_RATE 0x8001180090002028 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_RATE rx1_rate 9 8, rx0_rate 1 0

REG = GSER0_DLM000_RX_RESET 0x8001180090002030 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_RESET rx1_reset 8 8, rx0_reset 0 0

REG = GSER0_DLM000_RX_STATUS 0x8001180090002000 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_STATUS rx1_status 8 8, rx0_status 0 0
REG = GSER0_DLM001_RX_STATUS 0x8001180090082000 MEMORY 8
REG_FIELD = GSER0_DLM001_RX_STATUS rx1_status 8 8, rx0_status 0 0
REG = GSER0_DLM002_RX_STATUS 0x8001180090102000 MEMORY 8
REG_FIELD = GSER0_DLM002_RX_STATUS rx1_status 8 8, rx0_status 0 0

REG = GSER0_DLM000_RX_TERM_EN 0x8001180090002038 MEMORY 8
REG_FIELD = GSER0_DLM000_RX_TERM_EN rx1_term_en 8 8, rx0_term_en 0 0

REG = GSER0_DLM000_TEST_BYPASS 0x8001180090001058 MEMORY 8
REG_FIELD = GSER0_DLM000_TEST_BYPASS test_bypass 0 0
REG = GSER0_DLM001_TEST_BYPASS 0x8001180090081058 MEMORY 8
REG_FIELD = GSER0_DLM001_TEST_BYPASS test_bypass 0 0
REG = GSER0_DLM002_TEST_BYPASS 0x8001180090101058 MEMORY 8
REG_FIELD = GSER0_DLM002_TEST_BYPASS test_bypass 0 0

REG = GSER0_DLM000_TEST_POWERDOWN 0x8001180090001060 MEMORY 8
REG_FIELD = GSER0_DLM000_TEST_POWERDOWN test_powerdown 0 0
REG = GSER0_DLM001_TEST_POWERDOWN 0x8001180090081060 MEMORY 8
REG_FIELD = GSER0_DLM001_TEST_POWERDOWN test_powerdown 0 0
REG = GSER0_DLM002_TEST_POWERDOWN 0x8001180090101060 MEMORY 8
REG_FIELD = GSER0_DLM002_TEST_POWERDOWN test_powerdown 0 0

REG = GSER0_DLM000_TX_AMPLITUDE 0x8001180090003008 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_AMPLITUDE tx1_amplitude 14 8, tx0_amplitude 6 0

REG = GSER0_DLM000_TX_CM_EN 0x8001180090003010 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_CM_EN tx1_cm_en 8 8, tx0_cm_en 0 0

REG = GSER0_DLM000_TX_DATA_EN 0x8001180090003018 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_DATA_EN tx1_data_en 8 8, tx0_data_en 0 0

REG = GSER0_DLM000_TX_EN 0x8001180090003020 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_EN tx1_en 8 8, tx0_en 0 0

REG = GSER0_DLM000_TX_PREEMPH 0x8001180090003028 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_PREEMPH tx1_preemph 14 8, tx0_preemph 6 0

REG = GSER0_DLM000_TX_RATE 0x8001180090003030 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_RATE tx1_rate 9 8, tx0_rate 1 0

REG = GSER0_DLM000_TX_RESET 0x8001180090003038 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_RESET tx1_reset 8 8, tx0_reset 0 0

REG = GSER0_DLM000_TX_STATUS 0x8001180090003000 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_STATUS tx1_cm_status 9 9, tx1_status 8 8, tx0_cm_status 1 1, tx0_status 0 0
REG = GSER0_DLM001_TX_STATUS 0x8001180090083000 MEMORY 8
REG_FIELD = GSER0_DLM001_TX_STATUS tx1_cm_status 9 9, tx1_status 8 8, tx0_cm_status 1 1, tx0_status 0 0
REG = GSER0_DLM002_TX_STATUS 0x8001180090103000 MEMORY 8
REG_FIELD = GSER0_DLM002_TX_STATUS tx1_cm_status 9 9, tx1_status 8 8, tx0_cm_status 1 1, tx0_status 0 0

REG = GSER0_DLM000_TX_TERM_OFFSET 0x8001180090003040 MEMORY 8
REG_FIELD = GSER0_DLM000_TX_TERM_OFFSET tx1_term_offset 12 8, tx0_term_offset 4 0
REG = GSER0_DLM001_TX_TERM_OFFSET 0x8001180090083040 MEMORY 8
REG_FIELD = GSER0_DLM001_TX_TERM_OFFSET tx1_term_offset 12 8, tx0_term_offset 4 0
REG = GSER0_DLM002_TX_TERM_OFFSET 0x8001180090103040 MEMORY 8
REG_FIELD = GSER0_DLM002_TX_TERM_OFFSET tx1_term_offset 12 8, tx0_term_offset 4 0

REG = GSER0_PCIE_PCS_CLK_REQ 0x8001180090080478 MEMORY 8
REG_FIELD = GSER0_PCIE_PCS_CLK_REQ clk_req 0 0

REG = GSER0_PCIE_PIPE000_TXDEEMPH 0x8001180090080480 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE000_TXDEEMPH pipe_txdeemph 0 0
REG = GSER0_PCIE_PIPE001_TXDEEMPH 0x8001180090080488 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE001_TXDEEMPH pipe_txdeemph 0 0
REG = GSER0_PCIE_PIPE002_TXDEEMPH 0x8001180090080490 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE002_TXDEEMPH pipe_txdeemph 0 0
REG = GSER0_PCIE_PIPE003_TXDEEMPH 0x8001180090080498 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE003_TXDEEMPH pipe_txdeemph 0 0

REG = GSER0_PCIE_PIPE_COM_CLK 0x8001180090080470 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_COM_CLK com_clk 0 0

REG = GSER0_PCIE_PIPE_CRST 0x8001180090080458 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_CRST pipe_crst 0 0

REG = GSER0_PCIE_PIPE_PORT_LOOPBK 0x8001180090080468 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_PORT_LOOPBK pipe3_loopbk 3 3, pipe2_loopbk 2 2, pipe1_loopbk 1 1, pipe0_loopbk 0 0

REG = GSER0_PCIE_PIPE_PORT_SEL 0x8001180090080460 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_PORT_SEL cfg_pem1_dlm2 2 2, pipe_port_sel 1 0

REG = GSER0_PCIE_PIPE_RST 0x8001180090080448 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_RST pipe3_rst 3 3, pipe2_rst 2 2, pipe1_rst 1 1, pipe0_rst 0 0

REG = GSER0_PCIE_PIPE_RST_STS 0x8001180090080450 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_RST_STS pipe3_rst 3 3, pipe2_rst 2 2, pipe1_rst 1 1, pipe0_rst 0 0

REG = GSER0_PCIE_PIPE_STATUS 0x8001180090080400 MEMORY 8
REG_FIELD = GSER0_PCIE_PIPE_STATUS pipe3_clkreqn 3 3, pipe2_clkreqn 2 2, pipe1_clkreqn 1 1, pipe0_clkreqn 0 0

REG = GSER0_PCIE_TX_DEEMPH_GEN1 0x8001180090080408 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_DEEMPH_GEN1 tx_deemph_gen1 5 0

REG = GSER0_PCIE_TX_DEEMPH_GEN2_3P5DB 0x8001180090080410 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_DEEMPH_GEN2_3P5DB tx_deemph_gen2_3p5db 5 0

REG = GSER0_PCIE_TX_DEEMPH_GEN2_6DB 0x8001180090080418 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_DEEMPH_GEN2_6DB tx_deemph_gen2_6db 5 0

REG = GSER0_PCIE_TX_SWING_FULL 0x8001180090080420 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_SWING_FULL tx_swing_hi 6 0

REG = GSER0_PCIE_TX_SWING_LOW 0x8001180090080428 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_SWING_LOW tx_swing_lo 6 0

REG = GSER0_PCIE_TX_VBOOST_LVL 0x8001180090080440 MEMORY 8
REG_FIELD = GSER0_PCIE_TX_VBOOST_LVL tx_vboost_lvl 2 0

REG = GSER0_PHY000_IDCODE_HI 0x8001180090400008 MEMORY 8
REG_FIELD = GSER0_PHY000_IDCODE_HI idcode_hi 15 0
REG = GSER0_PHY001_IDCODE_HI 0x8001180090480008 MEMORY 8
REG_FIELD = GSER0_PHY001_IDCODE_HI idcode_hi 15 0
REG = GSER0_PHY002_IDCODE_HI 0x8001180090500008 MEMORY 8
REG_FIELD = GSER0_PHY002_IDCODE_HI idcode_hi 15 0

REG = GSER0_PHY000_IDCODE_LO 0x8001180090400000 MEMORY 8
REG_FIELD = GSER0_PHY000_IDCODE_LO idcode_lo 15 0
REG = GSER0_PHY001_IDCODE_LO 0x8001180090480000 MEMORY 8
REG_FIELD = GSER0_PHY001_IDCODE_LO idcode_lo 15 0
REG = GSER0_PHY002_IDCODE_LO 0x8001180090500000 MEMORY 8
REG_FIELD = GSER0_PHY002_IDCODE_LO idcode_lo 15 0

REG = GSER0_PHY000_LANE0_LOOPBACK 0x8001180090408170 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0
REG = GSER0_PHY001_LANE0_LOOPBACK 0x8001180090488170 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0
REG = GSER0_PHY002_LANE0_LOOPBACK 0x8001180090508170 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0

REG = GSER0_PHY000_LANE0_RX_LBERT_CTL 0x80011800904080B0 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_RX_LBERT_CTL sync 4 4, mode 3 0
REG = GSER0_PHY001_LANE0_RX_LBERT_CTL 0x80011800904880B0 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_RX_LBERT_CTL sync 4 4, mode 3 0
REG = GSER0_PHY002_LANE0_RX_LBERT_CTL 0x80011800905080B0 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_RX_LBERT_CTL sync 4 4, mode 3 0

REG = GSER0_PHY000_LANE0_RX_LBERT_ERR 0x80011800904080B8 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_RX_LBERT_ERR ov14 15 15, count 14 0
REG = GSER0_PHY001_LANE0_RX_LBERT_ERR 0x80011800904880B8 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_RX_LBERT_ERR ov14 15 15, count 14 0
REG = GSER0_PHY002_LANE0_RX_LBERT_ERR 0x80011800905080B8 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_RX_LBERT_ERR ov14 15 15, count 14 0

REG = GSER0_PHY000_LANE0_RX_OVRD_IN_LO 0x8001180090408028 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0
REG = GSER0_PHY001_LANE0_RX_OVRD_IN_LO 0x8001180090488028 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0
REG = GSER0_PHY002_LANE0_RX_OVRD_IN_LO 0x8001180090508028 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0

REG = GSER0_PHY000_LANE0_TX_LBERT_CTL 0x80011800904080A8 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0
REG = GSER0_PHY001_LANE0_TX_LBERT_CTL 0x80011800904880A8 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0
REG = GSER0_PHY002_LANE0_TX_LBERT_CTL 0x80011800905080A8 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0

REG = GSER0_PHY000_LANE0_TX_OVRD_IN_HI 0x8001180090408008 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0
REG = GSER0_PHY001_LANE0_TX_OVRD_IN_HI 0x8001180090488008 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0
REG = GSER0_PHY002_LANE0_TX_OVRD_IN_HI 0x8001180090508008 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0

REG = GSER0_PHY000_LANE0_TX_OVRD_IN_LO 0x8001180090408000 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0
REG = GSER0_PHY001_LANE0_TX_OVRD_IN_LO 0x8001180090488000 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0
REG = GSER0_PHY002_LANE0_TX_OVRD_IN_LO 0x8001180090508000 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0

REG = GSER0_PHY000_LANE0_TXDEBUG 0x8001180090408080 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE0_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0
REG = GSER0_PHY001_LANE0_TXDEBUG 0x8001180090488080 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE0_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0
REG = GSER0_PHY002_LANE0_TXDEBUG 0x8001180090508080 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE0_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0

REG = GSER0_PHY000_LANE1_LOOPBACK 0x8001180090408970 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0
REG = GSER0_PHY001_LANE1_LOOPBACK 0x8001180090488970 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0
REG = GSER0_PHY002_LANE1_LOOPBACK 0x8001180090508970 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_LOOPBACK ovrd_tx_lb 7 7, tx_lb_en_reg 6 6, atb_vptx 5 5, atb_vreg_tx 4 4, atb_vdccp 3 3, atb_vdccm 2 2, sel_pmix_clk 0 0

REG = GSER0_PHY000_LANE1_RX_LBERT_CTL 0x80011800904088B0 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_RX_LBERT_CTL sync 4 4, mode 3 0
REG = GSER0_PHY001_LANE1_RX_LBERT_CTL 0x80011800904888B0 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_RX_LBERT_CTL sync 4 4, mode 3 0
REG = GSER0_PHY002_LANE1_RX_LBERT_CTL 0x80011800905088B0 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_RX_LBERT_CTL sync 4 4, mode 3 0

REG = GSER0_PHY000_LANE1_RX_LBERT_ERR 0x80011800904088B8 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_RX_LBERT_ERR ov14 15 15, count 14 0
REG = GSER0_PHY001_LANE1_RX_LBERT_ERR 0x80011800904888B8 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_RX_LBERT_ERR ov14 15 15, count 14 0
REG = GSER0_PHY002_LANE1_RX_LBERT_ERR 0x80011800905088B8 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_RX_LBERT_ERR ov14 15 15, count 14 0

REG = GSER0_PHY000_LANE1_RX_OVRD_IN_LO 0x8001180090408828 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0
REG = GSER0_PHY001_LANE1_RX_OVRD_IN_LO 0x8001180090488828 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0
REG = GSER0_PHY002_LANE1_RX_OVRD_IN_LO 0x8001180090508828 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_RX_OVRD_IN_LO rx_los_en_ovrd 13 13, rx_los_en 12 12, rx_term_en_ovrd 11 11, rx_term_en 10 10, rx_bit_shift_ovrd 9 9, rx_bit_shift_en 8 8, rx_align_en_ovrd 7 7, rx_align_en 6 6, rx_data_en_ovrd 5 5, rx_data_en 4 4, rx_pll_en_ovrd 3 3, rx_pll_en 2 2, rx_invert_ovrd 1 1, rx_invert 0 0

REG = GSER0_PHY000_LANE1_TX_LBERT_CTL 0x80011800904088A8 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0
REG = GSER0_PHY001_LANE1_TX_LBERT_CTL 0x80011800904888A8 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0
REG = GSER0_PHY002_LANE1_TX_LBERT_CTL 0x80011800905088A8 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_TX_LBERT_CTL pat0 14 5, trig_err 4 4, mode 3 0

REG = GSER0_PHY000_LANE1_TX_OVRD_IN_HI 0x8001180090408808 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0
REG = GSER0_PHY001_LANE1_TX_OVRD_IN_HI 0x8001180090488808 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0
REG = GSER0_PHY002_LANE1_TX_OVRD_IN_HI 0x8001180090508808 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_TX_OVRD_IN_HI tx_vboost_en_ovrd 9 9, tx_vboost_en 8 8, tx_reset_ovrd 7 7, tx_reset 6 6, tx_nyquist_data 5 5, tx_clk_out_en_ovrd 4 4, tx_clk_out_en 3 3, tx_rate_ovrd 2 2, tx_rate 1 0

REG = GSER0_PHY000_LANE1_TX_OVRD_IN_LO 0x8001180090408800 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0
REG = GSER0_PHY001_LANE1_TX_OVRD_IN_LO 0x8001180090488800 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0
REG = GSER0_PHY002_LANE1_TX_OVRD_IN_LO 0x8001180090508800 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_TX_OVRD_IN_LO tx_beacon_en_ovrd 11 11, tx_beacon_en 10 10, tx_cm_en_ovrd 9 9, tx_cm_en 8 8, tx_en_ovrd 7 7, tx_en 6 6, tx_data_en_ovrd 5 5, tx_data_en 4 4, tx_invert_ovrd 3 3, tx_invert 2 2, loopbk_en_ovrd 1 1, loopbk_en 0 0

REG = GSER0_PHY000_LANE1_TXDEBUG 0x8001180090408880 MEMORY 8
REG_FIELD = GSER0_PHY000_LANE1_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0
REG = GSER0_PHY001_LANE1_TXDEBUG 0x8001180090488880 MEMORY 8
REG_FIELD = GSER0_PHY001_LANE1_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0
REG = GSER0_PHY002_LANE1_TXDEBUG 0x8001180090508880 MEMORY 8
REG_FIELD = GSER0_PHY002_LANE1_TXDEBUG rxdet_meas_time 11 4, detrx_always 3 3, dtb_sel 2 0

REG = GSER0_PHY000_OVRD_IN_LO 0x8001180090400088 MEMORY 8
REG_FIELD = GSER0_PHY000_OVRD_IN_LO res_ack_in_ovrd 15 15, res_ack_in 14 14, res_req_in_ovrd 13 13, res_req_in 12 12, rtune_req_ovrd 11 11, rtune_req 10 10, mpll_multiplier_ovrd 9 9, mpll_multiplier 8 2, mpll_en_ovrd 1 1, mpll_en 0 0
REG = GSER0_PHY001_OVRD_IN_LO 0x8001180090480088 MEMORY 8
REG_FIELD = GSER0_PHY001_OVRD_IN_LO res_ack_in_ovrd 15 15, res_ack_in 14 14, res_req_in_ovrd 13 13, res_req_in 12 12, rtune_req_ovrd 11 11, rtune_req 10 10, mpll_multiplier_ovrd 9 9, mpll_multiplier 8 2, mpll_en_ovrd 1 1, mpll_en 0 0
REG = GSER0_PHY002_OVRD_IN_LO 0x8001180090500088 MEMORY 8
REG_FIELD = GSER0_PHY002_OVRD_IN_LO res_ack_in_ovrd 15 15, res_ack_in 14 14, res_req_in_ovrd 13 13, res_req_in 12 12, rtune_req_ovrd 11 11, rtune_req 10 10, mpll_multiplier_ovrd 9 9, mpll_multiplier 8 2, mpll_en_ovrd 1 1, mpll_en 0 0

REG = GSER0_SATA_CFG 0x8001180090100208 MEMORY 8
REG_FIELD = GSER0_SATA_CFG sata_en 0 0

REG = GSER0_SATA_LANE_RST 0x8001180090100210 MEMORY 8
REG_FIELD = GSER0_SATA_LANE_RST l1_rst 1 1, l0_rst 0 0

REG = GSER0_SATA_P0_TX_AMP_GEN001 0x8001180090100488 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_AMP_GEN001 tx_amp_gen 6 0
REG = GSER0_SATA_P0_TX_AMP_GEN002 0x8001180090100490 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_AMP_GEN002 tx_amp_gen 6 0
REG = GSER0_SATA_P0_TX_AMP_GEN003 0x8001180090100498 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_AMP_GEN003 tx_amp_gen 6 0

REG = GSER0_SATA_P0_TX_PREEMPH_GEN001 0x8001180090100408 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_PREEMPH_GEN001 tx_preemph 5 0
REG = GSER0_SATA_P0_TX_PREEMPH_GEN002 0x8001180090100410 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_PREEMPH_GEN002 tx_preemph 5 0
REG = GSER0_SATA_P0_TX_PREEMPH_GEN003 0x8001180090100418 MEMORY 8
REG_FIELD = GSER0_SATA_P0_TX_PREEMPH_GEN003 tx_preemph 5 0

REG = GSER0_SATA_P1_TX_AMP_GEN001 0x80011800901004A8 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_AMP_GEN001 tx_amp_gen 6 0
REG = GSER0_SATA_P1_TX_AMP_GEN002 0x80011800901004B0 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_AMP_GEN002 tx_amp_gen 6 0
REG = GSER0_SATA_P1_TX_AMP_GEN003 0x80011800901004B8 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_AMP_GEN003 tx_amp_gen 6 0

REG = GSER0_SATA_P1_TX_PREEMPH_GEN001 0x8001180090100428 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_PREEMPH_GEN001 tx_preemph 5 0
REG = GSER0_SATA_P1_TX_PREEMPH_GEN002 0x8001180090100430 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_PREEMPH_GEN002 tx_preemph 5 0
REG = GSER0_SATA_P1_TX_PREEMPH_GEN003 0x8001180090100438 MEMORY 8
REG_FIELD = GSER0_SATA_P1_TX_PREEMPH_GEN003 tx_preemph 5 0

REG = GSER0_SATA_REF_SSP_EN 0x8001180090100600 MEMORY 8
REG_FIELD = GSER0_SATA_REF_SSP_EN ref_ssp_en 0 0

REG = GSER0_SATA_RX_INVERT 0x8001180090100218 MEMORY 8
REG_FIELD = GSER0_SATA_RX_INVERT rx1_invert 1 1, rx0_invert 0 0

REG = GSER0_SATA_SSC_CLK_SEL 0x8001180090100238 MEMORY 8
REG_FIELD = GSER0_SATA_SSC_CLK_SEL ssc_clk_sel 8 0

REG = GSER0_SATA_SSC_EN 0x8001180090100228 MEMORY 8
REG_FIELD = GSER0_SATA_SSC_EN ssc_en 0 0

REG = GSER0_SATA_SSC_RANGE 0x8001180090100230 MEMORY 8
REG_FIELD = GSER0_SATA_SSC_RANGE ssc_range 2 0

REG = GSER0_SATA_STATUS 0x8001180090100200 MEMORY 8
REG_FIELD = GSER0_SATA_STATUS p1_rdy 1 1, p0_rdy 0 0

REG = GSER0_SATA_TX_INVERT 0x8001180090100220 MEMORY 8
REG_FIELD = GSER0_SATA_TX_INVERT tx1_invert 1 1, tx0_invert 0 0

REG = IOB_BIST_STATUS 0x80011800F00007F8 MEMORY 8
REG_FIELD = IOB_BIST_STATUS xmdfif 22 22, xmcfif 21 21, iorfif 20 20, rsdfif 19 19, iocfif 18 18, icnrcb 17 17, icr0 16 16, icr1 15 15, icnr1 14 14, icnr0 13 13, ibdr0 12 12, ibdr1 11 11, ibr0 10 10, ibr1 9 9, icnrt 8 8, ibrq0 7 7, ibrq1 6 6, icrn0 5 5, icrn1 4 4, icrp0 3 3, icrp1 2 2, ibd 1 1, icd 0 0

REG = IOB_CHIP_CUR_PWR 0x80011800F0000828 MEMORY 8
REG_FIELD = IOB_CHIP_CUR_PWR current_power_settin 7 0

REG = IOB_CHIP_GLB_PWR_THROTTLE 0x80011800F0000808 MEMORY 8
REG_FIELD = IOB_CHIP_GLB_PWR_THROTTLE pwr_bw 33 32, pwr_max 31 24, pwr_min 23 16, pwr_setting 15 0

REG = IOB_CHIP_PWR_OUT 0x80011800F0000818 MEMORY 8
REG_FIELD = IOB_CHIP_PWR_OUT cpu_pwr 63 48, chip_power 47 32, coproc_power 31 16, avg_chip_power 15 0

REG = IOB_CTL_STATUS 0x80011800F0000050 MEMORY 8
REG_FIELD = IOB_CTL_STATUS xmc_per 9 6, rr_mode 5 5, rsv4 4 4, rsv3 3 3, pko_enb 2 2, dwb_enb 1 1, fau_end 0 0

REG = IOB_DWB_PRI_CNT 0x80011800F0000028 MEMORY 8
REG_FIELD = IOB_DWB_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_FAU_TIMEOUT 0x80011800F0000000 MEMORY 8
REG_FIELD = IOB_FAU_TIMEOUT tout_enb 12 12, tout_val 11 0

REG = IOB_I2C_PRI_CNT 0x80011800F0000010 MEMORY 8
REG_FIELD = IOB_I2C_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_INB_CONTROL_MATCH 0x80011800F0000078 MEMORY 8
REG_FIELD = IOB_INB_CONTROL_MATCH mask 28 21, opc 20 17, dst 16 8, src 7 0

REG = IOB_INB_CONTROL_MATCH_ENB 0x80011800F0000088 MEMORY 8
REG_FIELD = IOB_INB_CONTROL_MATCH_ENB mask 28 21, opc 20 17, dst 16 8, src 7 0

REG = IOB_INB_DATA_MATCH 0x80011800F0000070 MEMORY 8
REG_FIELD = IOB_INB_DATA_MATCH data 63 0

REG = IOB_INB_DATA_MATCH_ENB 0x80011800F0000080 MEMORY 8
REG_FIELD = IOB_INB_DATA_MATCH_ENB data 63 0

REG = IOB_INT_ENB 0x80011800F0000060 MEMORY 8
REG_FIELD = IOB_INT_ENB outb_mat 7 7, inb_mat 6 6, p_dat 5 5, np_dat 4 4, p_eop 3 3, p_sop 2 2, np_eop 1 1, np_sop 0 0

REG = IOB_INT_SUM 0x80011800F0000058 MEMORY 8
REG_FIELD = IOB_INT_SUM outb_mat 7 7, inb_mat 6 6, p_dat 5 5, np_dat 4 4, p_eop 3 3, p_sop 2 2, np_eop 1 1, np_sop 0 0

REG = IOB_N2C_L2C_PRI_CNT 0x80011800F0000020 MEMORY 8
REG_FIELD = IOB_N2C_L2C_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_N2C_RSP_PRI_CNT 0x80011800F0000008 MEMORY 8
REG_FIELD = IOB_N2C_RSP_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_OUTB_COM_PRI_CNT 0x80011800F0000040 MEMORY 8
REG_FIELD = IOB_OUTB_COM_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_OUTB_CONTROL_MATCH 0x80011800F0000098 MEMORY 8
REG_FIELD = IOB_OUTB_CONTROL_MATCH mask 25 18, eot 17 17, dst 16 9, src 8 0

REG = IOB_OUTB_CONTROL_MATCH_ENB 0x80011800F00000A8 MEMORY 8
REG_FIELD = IOB_OUTB_CONTROL_MATCH_ENB mask 25 18, eot 17 17, dst 16 9, src 8 0

REG = IOB_OUTB_DATA_MATCH 0x80011800F0000090 MEMORY 8
REG_FIELD = IOB_OUTB_DATA_MATCH data 63 0

REG = IOB_OUTB_DATA_MATCH_ENB 0x80011800F00000A0 MEMORY 8
REG_FIELD = IOB_OUTB_DATA_MATCH_ENB data 63 0

REG = IOB_OUTB_FPA_PRI_CNT 0x80011800F0000048 MEMORY 8
REG_FIELD = IOB_OUTB_FPA_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_OUTB_REQ_PRI_CNT 0x80011800F0000038 MEMORY 8
REG_FIELD = IOB_OUTB_REQ_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_P2C_REQ_PRI_CNT 0x80011800F0000018 MEMORY 8
REG_FIELD = IOB_P2C_REQ_PRI_CNT cnt_enb 15 15, cnt_val 14 0

REG = IOB_PKT_ERR 0x80011800F0000068 MEMORY 8
REG_FIELD = IOB_PKT_ERR vport 11 6, port 5 0

REG = IOB_PP_BIST_STATUS 0x80011800F0000700 MEMORY 8
REG_FIELD = IOB_PP_BIST_STATUS pp_bstat 3 0

REG = IOB_TO_CMB_CREDITS 0x80011800F00000B0 MEMORY 8
REG_FIELD = IOB_TO_CMB_CREDITS pko_rd 8 6, ncb_rd 5 3, ncb_wr 2 0

REG = IPD_1ST_MBUFF_SKIP 0x80014F0000000000 MEMORY 8
REG_FIELD = IPD_1ST_MBUFF_SKIP skip_sz 5 0

REG = IPD_1ST_NEXT_PTR_BACK 0x80014F0000000150 MEMORY 8
REG_FIELD = IPD_1ST_NEXT_PTR_BACK back 3 0

REG = IPD_2ND_NEXT_PTR_BACK 0x80014F0000000158 MEMORY 8
REG_FIELD = IPD_2ND_NEXT_PTR_BACK back 3 0

REG = IPD_BIST_STATUS 0x80014F00000007F8 MEMORY 8
REG_FIELD = IPD_BIST_STATUS csr_mem 17 17, csr_ncmd 16 16, pwq_wqed 15 15, pwq_wp1 14 14, pwq_pow 13 13, ipq_pbe1 12 12, ipq_pbe0 11 11, pbm3 10 10, pbm2 9 9, pbm1 8 8, pbm0 7 7, pbm_word 6 6, pwq1 5 5, pwq0 4 4, prc_off 3 3, ipd_old 2 2, ipd_new 1 1, pwp 0 0

REG = IPD_BP_PRT_RED_END 0x80014F0000000328 MEMORY 8
REG_FIELD = IPD_BP_PRT_RED_END prt_enb 47 0

REG = IPD_CLK_COUNT 0x80014F0000000338 MEMORY 8
REG_FIELD = IPD_CLK_COUNT clk_cnt 63 0

REG = IPD_CTL_STATUS 0x80014F0000000018 MEMORY 8
REG_FIELD = IPD_CTL_STATUS use_sop 17 17, rst_done 16 16, clken 15 15, no_wptr 14 14, pq_apkt 13 13, pq_nabuf 12 12, ipd_full 11 11, pkt_off 10 10, len_m8 9 9, reset 8 8, addpkt 7 7, naddbuf 6 6, pkt_lend 5 5, wqe_lend 4 4, pbp_en 3 3, opc_mode 2 1, ipd_en 0 0

REG = IPD_INT_ENB 0x80014F0000000160 MEMORY 8
REG_FIELD = IPD_INT_ENB pq_sub 11 11, pq_add 10 10, bc_ovr 9 9, d_coll 8 8, c_coll 7 7, cc_ovr 6 6, dc_ovr 5 5, bp_sub 4 4, prc_par3 3 3, prc_par2 2 2, prc_par1 1 1, prc_par0 0 0

REG = IPD_INT_SUM 0x80014F0000000168 MEMORY 8
REG_FIELD = IPD_INT_SUM pq_sub 11 11, pq_add 10 10, bc_ovr 9 9, d_coll 8 8, c_coll 7 7, cc_ovr 6 6, dc_ovr 5 5, bp_sub 4 4, prc_par3 3 3, prc_par2 2 2, prc_par1 1 1, prc_par0 0 0

REG = IPD_NOT_1ST_MBUFF_SKIP 0x80014F0000000008 MEMORY 8
REG_FIELD = IPD_NOT_1ST_MBUFF_SKIP skip_sz 5 0

REG = IPD_PACKET_MBUFF_SIZE 0x80014F0000000010 MEMORY 8
REG_FIELD = IPD_PACKET_MBUFF_SIZE mb_size 11 0

REG = IPD_PKT_PTR_VALID 0x80014F0000000358 MEMORY 8
REG_FIELD = IPD_PKT_PTR_VALID ptr 28 0

REG = IPD_PORT0_BP_PAGE_CNT 0x80014F0000000028 MEMORY 8
REG_FIELD = IPD_PORT0_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT1_BP_PAGE_CNT 0x80014F0000000030 MEMORY 8
REG_FIELD = IPD_PORT1_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT2_BP_PAGE_CNT 0x80014F0000000038 MEMORY 8
REG_FIELD = IPD_PORT2_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT3_BP_PAGE_CNT 0x80014F0000000040 MEMORY 8
REG_FIELD = IPD_PORT3_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT16_BP_PAGE_CNT 0x80014F00000000A8 MEMORY 8
REG_FIELD = IPD_PORT16_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT17_BP_PAGE_CNT 0x80014F00000000B0 MEMORY 8
REG_FIELD = IPD_PORT17_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT18_BP_PAGE_CNT 0x80014F00000000B8 MEMORY 8
REG_FIELD = IPD_PORT18_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT19_BP_PAGE_CNT 0x80014F00000000C0 MEMORY 8
REG_FIELD = IPD_PORT19_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT24_BP_PAGE_CNT 0x80014F00000000E8 MEMORY 8
REG_FIELD = IPD_PORT24_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT32_BP_PAGE_CNT 0x80014F0000000128 MEMORY 8
REG_FIELD = IPD_PORT32_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT33_BP_PAGE_CNT 0x80014F0000000130 MEMORY 8
REG_FIELD = IPD_PORT33_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT34_BP_PAGE_CNT 0x80014F0000000138 MEMORY 8
REG_FIELD = IPD_PORT34_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT35_BP_PAGE_CNT 0x80014F0000000140 MEMORY 8
REG_FIELD = IPD_PORT35_BP_PAGE_CNT bp_enb 17 17, page_cnt 16 0

REG = IPD_PORT36_BP_PAGE_CNT2 0x80014F0000000368 MEMORY 8
REG_FIELD = IPD_PORT36_BP_PAGE_CNT2 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT37_BP_PAGE_CNT2 0x80014F0000000370 MEMORY 8
REG_FIELD = IPD_PORT37_BP_PAGE_CNT2 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT38_BP_PAGE_CNT2 0x80014F0000000378 MEMORY 8
REG_FIELD = IPD_PORT38_BP_PAGE_CNT2 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT39_BP_PAGE_CNT2 0x80014F0000000380 MEMORY 8
REG_FIELD = IPD_PORT39_BP_PAGE_CNT2 bp_enb 17 17, page_cnt 16 0

REG = IPD_PORT40_BP_PAGE_CNT3 0x80014F00000003D0 MEMORY 8
REG_FIELD = IPD_PORT40_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT41_BP_PAGE_CNT3 0x80014F00000003D8 MEMORY 8
REG_FIELD = IPD_PORT41_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT42_BP_PAGE_CNT3 0x80014F00000003E0 MEMORY 8
REG_FIELD = IPD_PORT42_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT43_BP_PAGE_CNT3 0x80014F00000003E8 MEMORY 8
REG_FIELD = IPD_PORT43_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT44_BP_PAGE_CNT3 0x80014F00000003F0 MEMORY 8
REG_FIELD = IPD_PORT44_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT45_BP_PAGE_CNT3 0x80014F00000003F8 MEMORY 8
REG_FIELD = IPD_PORT45_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT46_BP_PAGE_CNT3 0x80014F0000000400 MEMORY 8
REG_FIELD = IPD_PORT46_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0
REG = IPD_PORT47_BP_PAGE_CNT3 0x80014F0000000408 MEMORY 8
REG_FIELD = IPD_PORT47_BP_PAGE_CNT3 bp_enb 17 17, page_cnt 16 0

REG = IPD_PORT_BP_COUNTERS2_PAIR36 0x80014F0000000388 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS2_PAIR36 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS2_PAIR37 0x80014F0000000390 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS2_PAIR37 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS2_PAIR38 0x80014F0000000398 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS2_PAIR38 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS2_PAIR39 0x80014F00000003A0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS2_PAIR39 cnt_val 24 0

REG = IPD_PORT_BP_COUNTERS3_PAIR40 0x80014F00000003B0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS3_PAIR40 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS3_PAIR41 0x80014F00000003B8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS3_PAIR41 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS3_PAIR42 0x80014F00000003C0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS3_PAIR42 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS3_PAIR43 0x80014F00000003C8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS3_PAIR43 cnt_val 24 0

REG = IPD_PORT_BP_COUNTERS4_PAIR44 0x80014F0000000410 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS4_PAIR44 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS4_PAIR45 0x80014F0000000418 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS4_PAIR45 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS4_PAIR46 0x80014F0000000420 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS4_PAIR46 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS4_PAIR47 0x80014F0000000428 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS4_PAIR47 cnt_val 24 0

REG = IPD_PORT_BP_COUNTERS_PAIR0 0x80014F00000001B8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR0 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR1 0x80014F00000001C0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR1 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR2 0x80014F00000001C8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR2 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR3 0x80014F00000001D0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR3 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR16 0x80014F0000000238 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR16 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR17 0x80014F0000000240 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR17 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR18 0x80014F0000000248 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR18 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR19 0x80014F0000000250 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR19 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR24 0x80014F0000000278 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR24 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR32 0x80014F00000002B8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR32 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR33 0x80014F00000002C0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR33 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR34 0x80014F00000002C8 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR34 cnt_val 24 0
REG = IPD_PORT_BP_COUNTERS_PAIR35 0x80014F00000002D0 MEMORY 8
REG_FIELD = IPD_PORT_BP_COUNTERS_PAIR35 cnt_val 24 0

REG = IPD_PORT_QOS_0_CNT 0x80014F0000000888 MEMORY 8
REG_FIELD = IPD_PORT_QOS_0_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_1_CNT 0x80014F0000000890 MEMORY 8
REG_FIELD = IPD_PORT_QOS_1_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_2_CNT 0x80014F0000000898 MEMORY 8
REG_FIELD = IPD_PORT_QOS_2_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_3_CNT 0x80014F00000008A0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_3_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_4_CNT 0x80014F00000008A8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_4_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_5_CNT 0x80014F00000008B0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_5_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_6_CNT 0x80014F00000008B8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_6_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_7_CNT 0x80014F00000008C0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_7_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_8_CNT 0x80014F00000008C8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_8_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_9_CNT 0x80014F00000008D0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_9_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_10_CNT 0x80014F00000008D8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_10_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_11_CNT 0x80014F00000008E0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_11_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_12_CNT 0x80014F00000008E8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_12_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_13_CNT 0x80014F00000008F0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_13_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_14_CNT 0x80014F00000008F8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_14_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_15_CNT 0x80014F0000000900 MEMORY 8
REG_FIELD = IPD_PORT_QOS_15_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_16_CNT 0x80014F0000000908 MEMORY 8
REG_FIELD = IPD_PORT_QOS_16_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_17_CNT 0x80014F0000000910 MEMORY 8
REG_FIELD = IPD_PORT_QOS_17_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_18_CNT 0x80014F0000000918 MEMORY 8
REG_FIELD = IPD_PORT_QOS_18_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_19_CNT 0x80014F0000000920 MEMORY 8
REG_FIELD = IPD_PORT_QOS_19_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_20_CNT 0x80014F0000000928 MEMORY 8
REG_FIELD = IPD_PORT_QOS_20_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_21_CNT 0x80014F0000000930 MEMORY 8
REG_FIELD = IPD_PORT_QOS_21_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_22_CNT 0x80014F0000000938 MEMORY 8
REG_FIELD = IPD_PORT_QOS_22_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_23_CNT 0x80014F0000000940 MEMORY 8
REG_FIELD = IPD_PORT_QOS_23_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_24_CNT 0x80014F0000000948 MEMORY 8
REG_FIELD = IPD_PORT_QOS_24_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_25_CNT 0x80014F0000000950 MEMORY 8
REG_FIELD = IPD_PORT_QOS_25_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_26_CNT 0x80014F0000000958 MEMORY 8
REG_FIELD = IPD_PORT_QOS_26_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_27_CNT 0x80014F0000000960 MEMORY 8
REG_FIELD = IPD_PORT_QOS_27_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_28_CNT 0x80014F0000000968 MEMORY 8
REG_FIELD = IPD_PORT_QOS_28_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_29_CNT 0x80014F0000000970 MEMORY 8
REG_FIELD = IPD_PORT_QOS_29_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_30_CNT 0x80014F0000000978 MEMORY 8
REG_FIELD = IPD_PORT_QOS_30_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_31_CNT 0x80014F0000000980 MEMORY 8
REG_FIELD = IPD_PORT_QOS_31_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_128_CNT 0x80014F0000000C88 MEMORY 8
REG_FIELD = IPD_PORT_QOS_128_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_129_CNT 0x80014F0000000C90 MEMORY 8
REG_FIELD = IPD_PORT_QOS_129_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_130_CNT 0x80014F0000000C98 MEMORY 8
REG_FIELD = IPD_PORT_QOS_130_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_131_CNT 0x80014F0000000CA0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_131_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_132_CNT 0x80014F0000000CA8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_132_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_133_CNT 0x80014F0000000CB0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_133_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_134_CNT 0x80014F0000000CB8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_134_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_135_CNT 0x80014F0000000CC0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_135_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_136_CNT 0x80014F0000000CC8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_136_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_137_CNT 0x80014F0000000CD0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_137_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_138_CNT 0x80014F0000000CD8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_138_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_139_CNT 0x80014F0000000CE0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_139_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_140_CNT 0x80014F0000000CE8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_140_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_141_CNT 0x80014F0000000CF0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_141_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_142_CNT 0x80014F0000000CF8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_142_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_143_CNT 0x80014F0000000D00 MEMORY 8
REG_FIELD = IPD_PORT_QOS_143_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_144_CNT 0x80014F0000000D08 MEMORY 8
REG_FIELD = IPD_PORT_QOS_144_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_145_CNT 0x80014F0000000D10 MEMORY 8
REG_FIELD = IPD_PORT_QOS_145_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_146_CNT 0x80014F0000000D18 MEMORY 8
REG_FIELD = IPD_PORT_QOS_146_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_147_CNT 0x80014F0000000D20 MEMORY 8
REG_FIELD = IPD_PORT_QOS_147_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_148_CNT 0x80014F0000000D28 MEMORY 8
REG_FIELD = IPD_PORT_QOS_148_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_149_CNT 0x80014F0000000D30 MEMORY 8
REG_FIELD = IPD_PORT_QOS_149_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_150_CNT 0x80014F0000000D38 MEMORY 8
REG_FIELD = IPD_PORT_QOS_150_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_151_CNT 0x80014F0000000D40 MEMORY 8
REG_FIELD = IPD_PORT_QOS_151_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_152_CNT 0x80014F0000000D48 MEMORY 8
REG_FIELD = IPD_PORT_QOS_152_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_153_CNT 0x80014F0000000D50 MEMORY 8
REG_FIELD = IPD_PORT_QOS_153_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_154_CNT 0x80014F0000000D58 MEMORY 8
REG_FIELD = IPD_PORT_QOS_154_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_155_CNT 0x80014F0000000D60 MEMORY 8
REG_FIELD = IPD_PORT_QOS_155_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_156_CNT 0x80014F0000000D68 MEMORY 8
REG_FIELD = IPD_PORT_QOS_156_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_157_CNT 0x80014F0000000D70 MEMORY 8
REG_FIELD = IPD_PORT_QOS_157_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_158_CNT 0x80014F0000000D78 MEMORY 8
REG_FIELD = IPD_PORT_QOS_158_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_159_CNT 0x80014F0000000D80 MEMORY 8
REG_FIELD = IPD_PORT_QOS_159_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_192_CNT 0x80014F0000000E88 MEMORY 8
REG_FIELD = IPD_PORT_QOS_192_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_193_CNT 0x80014F0000000E90 MEMORY 8
REG_FIELD = IPD_PORT_QOS_193_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_194_CNT 0x80014F0000000E98 MEMORY 8
REG_FIELD = IPD_PORT_QOS_194_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_195_CNT 0x80014F0000000EA0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_195_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_196_CNT 0x80014F0000000EA8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_196_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_197_CNT 0x80014F0000000EB0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_197_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_198_CNT 0x80014F0000000EB8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_198_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_199_CNT 0x80014F0000000EC0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_199_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_256_CNT 0x80014F0000001088 MEMORY 8
REG_FIELD = IPD_PORT_QOS_256_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_257_CNT 0x80014F0000001090 MEMORY 8
REG_FIELD = IPD_PORT_QOS_257_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_258_CNT 0x80014F0000001098 MEMORY 8
REG_FIELD = IPD_PORT_QOS_258_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_259_CNT 0x80014F00000010A0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_259_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_260_CNT 0x80014F00000010A8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_260_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_261_CNT 0x80014F00000010B0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_261_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_262_CNT 0x80014F00000010B8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_262_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_263_CNT 0x80014F00000010C0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_263_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_264_CNT 0x80014F00000010C8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_264_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_265_CNT 0x80014F00000010D0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_265_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_266_CNT 0x80014F00000010D8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_266_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_267_CNT 0x80014F00000010E0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_267_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_268_CNT 0x80014F00000010E8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_268_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_269_CNT 0x80014F00000010F0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_269_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_270_CNT 0x80014F00000010F8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_270_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_271_CNT 0x80014F0000001100 MEMORY 8
REG_FIELD = IPD_PORT_QOS_271_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_272_CNT 0x80014F0000001108 MEMORY 8
REG_FIELD = IPD_PORT_QOS_272_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_273_CNT 0x80014F0000001110 MEMORY 8
REG_FIELD = IPD_PORT_QOS_273_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_274_CNT 0x80014F0000001118 MEMORY 8
REG_FIELD = IPD_PORT_QOS_274_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_275_CNT 0x80014F0000001120 MEMORY 8
REG_FIELD = IPD_PORT_QOS_275_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_276_CNT 0x80014F0000001128 MEMORY 8
REG_FIELD = IPD_PORT_QOS_276_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_277_CNT 0x80014F0000001130 MEMORY 8
REG_FIELD = IPD_PORT_QOS_277_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_278_CNT 0x80014F0000001138 MEMORY 8
REG_FIELD = IPD_PORT_QOS_278_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_279_CNT 0x80014F0000001140 MEMORY 8
REG_FIELD = IPD_PORT_QOS_279_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_280_CNT 0x80014F0000001148 MEMORY 8
REG_FIELD = IPD_PORT_QOS_280_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_281_CNT 0x80014F0000001150 MEMORY 8
REG_FIELD = IPD_PORT_QOS_281_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_282_CNT 0x80014F0000001158 MEMORY 8
REG_FIELD = IPD_PORT_QOS_282_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_283_CNT 0x80014F0000001160 MEMORY 8
REG_FIELD = IPD_PORT_QOS_283_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_284_CNT 0x80014F0000001168 MEMORY 8
REG_FIELD = IPD_PORT_QOS_284_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_285_CNT 0x80014F0000001170 MEMORY 8
REG_FIELD = IPD_PORT_QOS_285_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_286_CNT 0x80014F0000001178 MEMORY 8
REG_FIELD = IPD_PORT_QOS_286_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_287_CNT 0x80014F0000001180 MEMORY 8
REG_FIELD = IPD_PORT_QOS_287_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_288_CNT 0x80014F0000001188 MEMORY 8
REG_FIELD = IPD_PORT_QOS_288_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_289_CNT 0x80014F0000001190 MEMORY 8
REG_FIELD = IPD_PORT_QOS_289_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_290_CNT 0x80014F0000001198 MEMORY 8
REG_FIELD = IPD_PORT_QOS_290_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_291_CNT 0x80014F00000011A0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_291_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_292_CNT 0x80014F00000011A8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_292_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_293_CNT 0x80014F00000011B0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_293_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_294_CNT 0x80014F00000011B8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_294_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_295_CNT 0x80014F00000011C0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_295_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_296_CNT 0x80014F00000011C8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_296_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_297_CNT 0x80014F00000011D0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_297_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_298_CNT 0x80014F00000011D8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_298_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_299_CNT 0x80014F00000011E0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_299_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_300_CNT 0x80014F00000011E8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_300_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_301_CNT 0x80014F00000011F0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_301_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_302_CNT 0x80014F00000011F8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_302_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_303_CNT 0x80014F0000001200 MEMORY 8
REG_FIELD = IPD_PORT_QOS_303_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_304_CNT 0x80014F0000001208 MEMORY 8
REG_FIELD = IPD_PORT_QOS_304_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_305_CNT 0x80014F0000001210 MEMORY 8
REG_FIELD = IPD_PORT_QOS_305_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_306_CNT 0x80014F0000001218 MEMORY 8
REG_FIELD = IPD_PORT_QOS_306_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_307_CNT 0x80014F0000001220 MEMORY 8
REG_FIELD = IPD_PORT_QOS_307_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_308_CNT 0x80014F0000001228 MEMORY 8
REG_FIELD = IPD_PORT_QOS_308_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_309_CNT 0x80014F0000001230 MEMORY 8
REG_FIELD = IPD_PORT_QOS_309_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_310_CNT 0x80014F0000001238 MEMORY 8
REG_FIELD = IPD_PORT_QOS_310_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_311_CNT 0x80014F0000001240 MEMORY 8
REG_FIELD = IPD_PORT_QOS_311_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_312_CNT 0x80014F0000001248 MEMORY 8
REG_FIELD = IPD_PORT_QOS_312_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_313_CNT 0x80014F0000001250 MEMORY 8
REG_FIELD = IPD_PORT_QOS_313_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_314_CNT 0x80014F0000001258 MEMORY 8
REG_FIELD = IPD_PORT_QOS_314_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_315_CNT 0x80014F0000001260 MEMORY 8
REG_FIELD = IPD_PORT_QOS_315_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_316_CNT 0x80014F0000001268 MEMORY 8
REG_FIELD = IPD_PORT_QOS_316_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_317_CNT 0x80014F0000001270 MEMORY 8
REG_FIELD = IPD_PORT_QOS_317_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_318_CNT 0x80014F0000001278 MEMORY 8
REG_FIELD = IPD_PORT_QOS_318_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_319_CNT 0x80014F0000001280 MEMORY 8
REG_FIELD = IPD_PORT_QOS_319_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_320_CNT 0x80014F0000001288 MEMORY 8
REG_FIELD = IPD_PORT_QOS_320_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_321_CNT 0x80014F0000001290 MEMORY 8
REG_FIELD = IPD_PORT_QOS_321_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_322_CNT 0x80014F0000001298 MEMORY 8
REG_FIELD = IPD_PORT_QOS_322_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_323_CNT 0x80014F00000012A0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_323_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_324_CNT 0x80014F00000012A8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_324_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_325_CNT 0x80014F00000012B0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_325_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_326_CNT 0x80014F00000012B8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_326_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_327_CNT 0x80014F00000012C0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_327_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_328_CNT 0x80014F00000012C8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_328_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_329_CNT 0x80014F00000012D0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_329_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_330_CNT 0x80014F00000012D8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_330_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_331_CNT 0x80014F00000012E0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_331_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_332_CNT 0x80014F00000012E8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_332_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_333_CNT 0x80014F00000012F0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_333_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_334_CNT 0x80014F00000012F8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_334_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_335_CNT 0x80014F0000001300 MEMORY 8
REG_FIELD = IPD_PORT_QOS_335_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_336_CNT 0x80014F0000001308 MEMORY 8
REG_FIELD = IPD_PORT_QOS_336_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_337_CNT 0x80014F0000001310 MEMORY 8
REG_FIELD = IPD_PORT_QOS_337_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_338_CNT 0x80014F0000001318 MEMORY 8
REG_FIELD = IPD_PORT_QOS_338_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_339_CNT 0x80014F0000001320 MEMORY 8
REG_FIELD = IPD_PORT_QOS_339_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_340_CNT 0x80014F0000001328 MEMORY 8
REG_FIELD = IPD_PORT_QOS_340_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_341_CNT 0x80014F0000001330 MEMORY 8
REG_FIELD = IPD_PORT_QOS_341_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_342_CNT 0x80014F0000001338 MEMORY 8
REG_FIELD = IPD_PORT_QOS_342_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_343_CNT 0x80014F0000001340 MEMORY 8
REG_FIELD = IPD_PORT_QOS_343_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_344_CNT 0x80014F0000001348 MEMORY 8
REG_FIELD = IPD_PORT_QOS_344_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_345_CNT 0x80014F0000001350 MEMORY 8
REG_FIELD = IPD_PORT_QOS_345_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_346_CNT 0x80014F0000001358 MEMORY 8
REG_FIELD = IPD_PORT_QOS_346_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_347_CNT 0x80014F0000001360 MEMORY 8
REG_FIELD = IPD_PORT_QOS_347_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_348_CNT 0x80014F0000001368 MEMORY 8
REG_FIELD = IPD_PORT_QOS_348_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_349_CNT 0x80014F0000001370 MEMORY 8
REG_FIELD = IPD_PORT_QOS_349_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_350_CNT 0x80014F0000001378 MEMORY 8
REG_FIELD = IPD_PORT_QOS_350_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_351_CNT 0x80014F0000001380 MEMORY 8
REG_FIELD = IPD_PORT_QOS_351_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_352_CNT 0x80014F0000001388 MEMORY 8
REG_FIELD = IPD_PORT_QOS_352_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_353_CNT 0x80014F0000001390 MEMORY 8
REG_FIELD = IPD_PORT_QOS_353_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_354_CNT 0x80014F0000001398 MEMORY 8
REG_FIELD = IPD_PORT_QOS_354_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_355_CNT 0x80014F00000013A0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_355_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_356_CNT 0x80014F00000013A8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_356_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_357_CNT 0x80014F00000013B0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_357_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_358_CNT 0x80014F00000013B8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_358_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_359_CNT 0x80014F00000013C0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_359_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_360_CNT 0x80014F00000013C8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_360_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_361_CNT 0x80014F00000013D0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_361_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_362_CNT 0x80014F00000013D8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_362_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_363_CNT 0x80014F00000013E0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_363_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_364_CNT 0x80014F00000013E8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_364_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_365_CNT 0x80014F00000013F0 MEMORY 8
REG_FIELD = IPD_PORT_QOS_365_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_366_CNT 0x80014F00000013F8 MEMORY 8
REG_FIELD = IPD_PORT_QOS_366_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_367_CNT 0x80014F0000001400 MEMORY 8
REG_FIELD = IPD_PORT_QOS_367_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_368_CNT 0x80014F0000001408 MEMORY 8
REG_FIELD = IPD_PORT_QOS_368_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_369_CNT 0x80014F0000001410 MEMORY 8
REG_FIELD = IPD_PORT_QOS_369_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_370_CNT 0x80014F0000001418 MEMORY 8
REG_FIELD = IPD_PORT_QOS_370_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_371_CNT 0x80014F0000001420 MEMORY 8
REG_FIELD = IPD_PORT_QOS_371_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_372_CNT 0x80014F0000001428 MEMORY 8
REG_FIELD = IPD_PORT_QOS_372_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_373_CNT 0x80014F0000001430 MEMORY 8
REG_FIELD = IPD_PORT_QOS_373_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_374_CNT 0x80014F0000001438 MEMORY 8
REG_FIELD = IPD_PORT_QOS_374_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_375_CNT 0x80014F0000001440 MEMORY 8
REG_FIELD = IPD_PORT_QOS_375_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_376_CNT 0x80014F0000001448 MEMORY 8
REG_FIELD = IPD_PORT_QOS_376_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_377_CNT 0x80014F0000001450 MEMORY 8
REG_FIELD = IPD_PORT_QOS_377_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_378_CNT 0x80014F0000001458 MEMORY 8
REG_FIELD = IPD_PORT_QOS_378_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_379_CNT 0x80014F0000001460 MEMORY 8
REG_FIELD = IPD_PORT_QOS_379_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_380_CNT 0x80014F0000001468 MEMORY 8
REG_FIELD = IPD_PORT_QOS_380_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_381_CNT 0x80014F0000001470 MEMORY 8
REG_FIELD = IPD_PORT_QOS_381_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_382_CNT 0x80014F0000001478 MEMORY 8
REG_FIELD = IPD_PORT_QOS_382_CNT wmark 63 32, cnt 31 0
REG = IPD_PORT_QOS_383_CNT 0x80014F0000001480 MEMORY 8
REG_FIELD = IPD_PORT_QOS_383_CNT wmark 63 32, cnt 31 0

REG = IPD_PORT_QOS_INT0 0x80014F0000000808 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT0 intr 63 0
REG = IPD_PORT_QOS_INT2 0x80014F0000000818 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT2 intr 63 0
REG = IPD_PORT_QOS_INT3 0x80014F0000000820 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT3 intr 63 0
REG = IPD_PORT_QOS_INT4 0x80014F0000000828 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT4 intr 63 0

REG = IPD_PORT_QOS_INT_ENB0 0x80014F0000000848 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT_ENB0 enb 63 0
REG = IPD_PORT_QOS_INT_ENB2 0x80014F0000000858 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT_ENB2 enb 63 0
REG = IPD_PORT_QOS_INT_ENB3 0x80014F0000000860 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT_ENB3 enb 63 0
REG = IPD_PORT_QOS_INT_ENB4 0x80014F0000000868 MEMORY 8
REG_FIELD = IPD_PORT_QOS_INT_ENB4 enb 63 0

REG = IPD_PRC_HOLD_PTR_FIFO_CTL 0x80014F0000000348 MEMORY 8
REG_FIELD = IPD_PRC_HOLD_PTR_FIFO_CTL max_pkt 38 36, praddr 35 33, ptr 32 4, cena 3 3, raddr 2 0

REG = IPD_PRC_PORT_PTR_FIFO_CTL 0x80014F0000000350 MEMORY 8
REG_FIELD = IPD_PRC_PORT_PTR_FIFO_CTL max_pkt 43 37, ptr 36 8, cena 7 7, raddr 6 0

REG = IPD_PTR_COUNT 0x80014F0000000320 MEMORY 8
REG_FIELD = IPD_PTR_COUNT pktv_cnt 18 18, wqev_cnt 17 17, pfif_cnt 16 14, pkt_pcnt 13 7, wqe_pcnt 6 0

REG = IPD_PWP_PTR_FIFO_CTL 0x80014F0000000340 MEMORY 8
REG_FIELD = IPD_PWP_PTR_FIFO_CTL max_cnts 60 54, wraddr 53 46, praddr 45 38, ptr 37 9, cena 8 8, raddr 7 0

REG = IPD_QOS0_RED_MARKS 0x80014F0000000178 MEMORY 8
REG_FIELD = IPD_QOS0_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS1_RED_MARKS 0x80014F0000000180 MEMORY 8
REG_FIELD = IPD_QOS1_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS2_RED_MARKS 0x80014F0000000188 MEMORY 8
REG_FIELD = IPD_QOS2_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS3_RED_MARKS 0x80014F0000000190 MEMORY 8
REG_FIELD = IPD_QOS3_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS4_RED_MARKS 0x80014F0000000198 MEMORY 8
REG_FIELD = IPD_QOS4_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS5_RED_MARKS 0x80014F00000001A0 MEMORY 8
REG_FIELD = IPD_QOS5_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS6_RED_MARKS 0x80014F00000001A8 MEMORY 8
REG_FIELD = IPD_QOS6_RED_MARKS drop 63 32, pass 31 0
REG = IPD_QOS7_RED_MARKS 0x80014F00000001B0 MEMORY 8
REG_FIELD = IPD_QOS7_RED_MARKS drop 63 32, pass 31 0

REG = IPD_QUE0_FREE_PAGE_CNT 0x80014F0000000330 MEMORY 8
REG_FIELD = IPD_QUE0_FREE_PAGE_CNT q0_pcnt 31 0

REG = IPD_RED_PORT_ENABLE 0x80014F00000002D8 MEMORY 8
REG_FIELD = IPD_RED_PORT_ENABLE prb_dly 63 50, avg_dly 49 36, prt_enb 35 0

REG = IPD_RED_PORT_ENABLE2 0x80014F00000003A8 MEMORY 8
REG_FIELD = IPD_RED_PORT_ENABLE2 prt_enb 11 0

REG = IPD_RED_QUE0_PARAM 0x80014F00000002E0 MEMORY 8
REG_FIELD = IPD_RED_QUE0_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE1_PARAM 0x80014F00000002E8 MEMORY 8
REG_FIELD = IPD_RED_QUE1_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE2_PARAM 0x80014F00000002F0 MEMORY 8
REG_FIELD = IPD_RED_QUE2_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE3_PARAM 0x80014F00000002F8 MEMORY 8
REG_FIELD = IPD_RED_QUE3_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE4_PARAM 0x80014F0000000300 MEMORY 8
REG_FIELD = IPD_RED_QUE4_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE5_PARAM 0x80014F0000000308 MEMORY 8
REG_FIELD = IPD_RED_QUE5_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE6_PARAM 0x80014F0000000310 MEMORY 8
REG_FIELD = IPD_RED_QUE6_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0
REG = IPD_RED_QUE7_PARAM 0x80014F0000000318 MEMORY 8
REG_FIELD = IPD_RED_QUE7_PARAM use_pcnt 48 48, new_con 47 40, avg_con 39 32, prb_con 31 0

REG = IPD_SUB_PORT_BP_PAGE_CNT 0x80014F0000000148 MEMORY 8
REG_FIELD = IPD_SUB_PORT_BP_PAGE_CNT port 30 25, page_cnt 24 0

REG = IPD_SUB_PORT_FCS 0x80014F0000000170 MEMORY 8
REG_FIELD = IPD_SUB_PORT_FCS port_bit2 39 36, port_bit 31 0

REG = IPD_SUB_PORT_QOS_CNT 0x80014F0000000800 MEMORY 8
REG_FIELD = IPD_SUB_PORT_QOS_CNT port_qos 40 32, cnt 31 0

REG = IPD_WQE_FPA_QUEUE 0x80014F0000000020 MEMORY 8
REG_FIELD = IPD_WQE_FPA_QUEUE wqe_pool 2 0

REG = IPD_WQE_PTR_VALID 0x80014F0000000360 MEMORY 8
REG_FIELD = IPD_WQE_PTR_VALID ptr 28 0

REG = KEY_BIST_REG 0x8001180020000018 MEMORY 8
REG_FIELD = KEY_BIST_REG rrc 1 1, mem 0 0

REG = KEY_CTL_STATUS 0x8001180020000010 MEMORY 8
REG_FIELD = KEY_CTL_STATUS key_fsyn 2 1, key_cdis 0 0

REG = KEY_INT_ENB 0x8001180020000008 MEMORY 8
REG_FIELD = KEY_INT_ENB key_dbe 1 1, key_sbe 0 0

REG = KEY_INT_SUM 0x8001180020000000 MEMORY 8
REG_FIELD = KEY_INT_SUM key_dbe 1 1, key_sbe 0 0

REG = L2C_BIG_CTL 0x8001180080800030 MEMORY 8
REG_FIELD = L2C_BIG_CTL maxdram 7 4, disbig 0 0

REG = L2C_CBC0_BIST_STATUS 0x8001180080E007F8 MEMORY 8
REG_FIELD = L2C_CBC0_BIST_STATUS ioccmdfl 33 32, rsdfl 31 0

REG = L2C_CBC0_INT 0x8001180080E00028 MEMORY 8
REG_FIELD = L2C_CBC0_INT ioccmddbe 3 3, ioccmdsbe 2 2, rsddbe 1 1, rsdsbe 0 0

REG = L2C_CBC0_IOCERR 0x8001180080E007E8 MEMORY 8
REG_FIELD = L2C_CBC0_IOCERR cmddbe 63 63, cmdsbe 62 62, syn 39 32, xmcnum 2 0

REG = L2C_CBC0_RSDERR 0x8001180080E007F0 MEMORY 8
REG_FIELD = L2C_CBC0_RSDERR rsddbe 63 63, rsdsbe 62 62, syn 39 32, tadnum 8 6, qwnum 5 4, rsdnum 3 0

REG = L2C_COP0_ADR 0x8001180080800038 MEMORY 8
REG_FIELD = L2C_COP0_ADR ppid 23 16, mbz 14 9, root 8 8, rd 7 3, sel 2 0

REG = L2C_COP0_DAT 0x8001180080800040 MEMORY 8
REG_FIELD = L2C_COP0_DAT data 63 0

REG = L2C_CTL 0x8001180080800000 MEMORY 8
REG_FIELD = L2C_CTL ocla_qos 31 29, disstgl2i 27 27, discclk 24 24, rsp_arb_mode 15 15, xmc_arb_mode 14 14, rdf_cnt 13 6, disecc 1 1, disidxalias 0 0

REG = L2C_ECC_CTL 0x8001180080800010 MEMORY 8
REG_FIELD = L2C_ECC_CTL l2dflip 9 8, l2tflip 7 6, rdfflip 5 4, xmdflip 3 2, ioccmdflip 1 0

REG = L2C_INV0_PFC 0x8001180080800820 MEMORY 8
REG_FIELD = L2C_INV0_PFC count 63 0

REG = L2C_IOC0_PFC 0x8001180080800828 MEMORY 8
REG_FIELD = L2C_IOC0_PFC count 63 0

REG = L2C_IOR0_PFC 0x8001180080800830 MEMORY 8
REG_FIELD = L2C_IOR0_PFC count 63 0

REG = L2C_MCI0_BIST_STATUS 0x8001180080C007F8 MEMORY 8
REG_FIELD = L2C_MCI0_BIST_STATUS start_bist 63 63, clear_bist 62 62, vbffl 1 0

REG = L2C_MCI0_ERR 0x8001180080C007F0 MEMORY 8
REG_FIELD = L2C_MCI0_ERR vbfdbe1 63 63, vbfdbe0 62 62, vbfsbe1 61 61, vbfsbe0 60 60, syn1 47 40, syn0 39 32, vbf4 11 11, index 10 4

REG = L2C_MCI0_INT 0x8001180080C00028 MEMORY 8
REG_FIELD = L2C_MCI0_INT vbfdbe 1 1, vbfsbe 0 0

REG = L2C_QOS_IOB0 0x8001180080880200 MEMORY 8
REG_FIELD = L2C_QOS_IOB0 dwblvl 6 4, lvl 2 0

REG = L2C_QOS_PP0 0x8001180080880000 MEMORY 8
REG_FIELD = L2C_QOS_PP0 lvl 2 0
REG = L2C_QOS_PP1 0x8001180080880008 MEMORY 8
REG_FIELD = L2C_QOS_PP1 lvl 2 0
REG = L2C_QOS_PP2 0x8001180080880010 MEMORY 8
REG_FIELD = L2C_QOS_PP2 lvl 2 0
REG = L2C_QOS_PP3 0x8001180080880018 MEMORY 8
REG_FIELD = L2C_QOS_PP3 lvl 2 0

REG = L2C_QOS_WGT 0x8001180080800008 MEMORY 8
REG_FIELD = L2C_QOS_WGT wgt7 63 56, wgt6 55 48, wgt5 47 40, wgt4 39 32, wgt3 31 24, wgt2 23 16, wgt1 15 8, wgt0 7 0

REG = L2C_RSC0_PFC 0x8001180080800810 MEMORY 8
REG_FIELD = L2C_RSC0_PFC count 63 0

REG = L2C_RSD0_PFC 0x8001180080800818 MEMORY 8
REG_FIELD = L2C_RSD0_PFC count 63 0

REG = L2C_TAD0_DLL 0x8001180080A00018 MEMORY 8
REG_FIELD = L2C_TAD0_DLL dll_setting 15 4, dll_state 3 1, dll_lock 0 0

REG = L2C_TAD0_ERR 0x8001180080A007D0 MEMORY 8
REG_FIELD = L2C_TAD0_ERR bigrd 63 63, bigwr 62 62, holerd 61 61, holewr 60 60, cmd 57 51, source 50 44, node 43 40, addr 39 0

REG = L2C_TAD0_INT 0x8001180080A00028 MEMORY 8
REG_FIELD = L2C_TAD0_INT rtgdbe 33 33, rtgsbe 32 32, wrdislmc 16 16, rddislmc 15 15, bigrd 14 14, bigwr 13 13, holerd 12 12, holewr 11 11, noway 10 10, tagdbe 9 9, tagsbe 8 8, fbfdbe 5 5, fbfsbe 4 4, sbfdbe 3 3, sbfsbe 2 2, l2ddbe 1 1, l2dsbe 0 0

REG = L2C_TAD0_PFC000 0x8001180080A00400 MEMORY 8
REG_FIELD = L2C_TAD0_PFC000 count 63 0
REG = L2C_TAD0_PFC001 0x8001180080A00408 MEMORY 8
REG_FIELD = L2C_TAD0_PFC001 count 63 0
REG = L2C_TAD0_PFC002 0x8001180080A00410 MEMORY 8
REG_FIELD = L2C_TAD0_PFC002 count 63 0
REG = L2C_TAD0_PFC003 0x8001180080A00418 MEMORY 8
REG_FIELD = L2C_TAD0_PFC003 count 63 0

REG = L2C_TAD0_PRF 0x8001180080A00008 MEMORY 8
REG_FIELD = L2C_TAD0_PRF cnt3sel 31 24, cnt2sel 23 16, cnt1sel 15 8, cnt0sel 7 0

REG = L2C_TAD0_TAG 0x8001180080A00010 MEMORY 8
REG_FIELD = L2C_TAD0_TAG sblkdty 63 60, businfo 55 48, ecc 46 40, tag 39 17, used 3 3, valid 2 2, dirty 1 1, lock 0 0

REG = L2C_TAD_CTL 0x8001180080800018 MEMORY 8
REG_FIELD = L2C_TAD_CTL vbf_thresh 10 8, maxvbf 6 4, maxlfb 2 0

REG = L2C_TBF0_BIST_STATUS 0x8001180080A007E8 MEMORY 8
REG_FIELD = L2C_TBF0_BIST_STATUS vbffl 63 48, sbffl 47 32, fbfrspfl 31 16, fbfwrpfl 15 0

REG = L2C_TDT0_BIST_STATUS 0x8001180080A007F0 MEMORY 8
REG_FIELD = L2C_TDT0_BIST_STATUS l2dfl 15 0

REG = L2C_TQD0_ERR 0x8001180080A007D8 MEMORY 8
REG_FIELD = L2C_TQD0_ERR l2ddbe 63 63, sbfdbe 62 62, fbfdbe 61 61, l2dsbe 60 60, sbfsbe 59 59, fbfsbe 58 58, syn 39 32, qdnum 17 15, qdhlf 14 14, l2didx 13 0

REG = L2C_TTG0_BIST_STATUS 0x8001180080A007F8 MEMORY 8
REG_FIELD = L2C_TTG0_BIST_STATUS rtgfl 47 32, lrulfbfl 17 17, lrufl 16 16, tagfl 15 0

REG = L2C_TTG0_ERR 0x8001180080A007E0 MEMORY 8
REG_FIELD = L2C_TTG0_ERR tagdbe 63 63, tagsbe 62 62, noway 61 61, syn 38 32, way 18 17, l2idx 16 7

REG = L2C_WPAR_IOB0 0x8001180080840200 MEMORY 8
REG_FIELD = L2C_WPAR_IOB0 mask 3 0

REG = L2C_WPAR_PP0 0x8001180080840000 MEMORY 8
REG_FIELD = L2C_WPAR_PP0 mask 3 0
REG = L2C_WPAR_PP1 0x8001180080840008 MEMORY 8
REG_FIELD = L2C_WPAR_PP1 mask 3 0
REG = L2C_WPAR_PP2 0x8001180080840010 MEMORY 8
REG_FIELD = L2C_WPAR_PP2 mask 3 0
REG = L2C_WPAR_PP3 0x8001180080840018 MEMORY 8
REG_FIELD = L2C_WPAR_PP3 mask 3 0

REG = L2C_XMC0_PFC 0x8001180080800800 MEMORY 8
REG_FIELD = L2C_XMC0_PFC count 63 0

REG = L2C_XMC_CMD 0x8001180080800028 MEMORY 8
REG_FIELD = L2C_XMC_CMD inuse 63 63, cmd 62 56, qos 46 44, node 43 40, addr 39 0

REG = L2C_XMD0_PFC 0x8001180080800808 MEMORY 8
REG_FIELD = L2C_XMD0_PFC count 63 0

REG = LMC0_BIST_CTL 0x8001180088000100 MEMORY 8
REG_FIELD = LMC0_BIST_CTL dlcram_bist_status 3 3, dlcram_bist_done 2 2, start_bist 1 1, clear_bist 0 0

REG = LMC0_CHAR_CTL 0x8001180088000220 MEMORY 8
REG_FIELD = LMC0_CHAR_CTL dq_char_check_lock 52 52, dq_char_check_enable 51 51, dq_char_bit_sel 50 48, dq_char_byte_sel 47 44, dr 43 43, skew_on 42 42, en 41 41, sel 40 40, prog 39 32, prbs 31 0

REG = LMC0_CHAR_DQ_ERR_COUNT 0x8001180088000040 MEMORY 8
REG_FIELD = LMC0_CHAR_DQ_ERR_COUNT dq_err_count 39 0

REG = LMC0_CHAR_MASK0 0x8001180088000228 MEMORY 8
REG_FIELD = LMC0_CHAR_MASK0 mask 63 0

REG = LMC0_CHAR_MASK1 0x8001180088000230 MEMORY 8
REG_FIELD = LMC0_CHAR_MASK1 mask 7 0

REG = LMC0_CHAR_MASK2 0x8001180088000238 MEMORY 8
REG_FIELD = LMC0_CHAR_MASK2 mask 63 0

REG = LMC0_CHAR_MASK3 0x8001180088000240 MEMORY 8
REG_FIELD = LMC0_CHAR_MASK3 mask 7 0

REG = LMC0_CHAR_MASK4 0x8001180088000318 MEMORY 8
REG_FIELD = LMC0_CHAR_MASK4 dbi_mask 44 36, par_mask 35 35, act_n_mask 34 34, a17_mask 33 33, reset_n_mask 32 32, a_mask 31 16, ba_mask 15 13, we_n_mask 12 12, cas_n_mask 11 11, ras_n_mask 10 10, odt1_mask 9 8, odt0_mask 7 6, cs1_n_mask 5 4, cs0_n_mask 3 2, cke_mask 1 0

REG = LMC0_COMP_CTL2 0x80011800880001B8 MEMORY 8
REG_FIELD = LMC0_COMP_CTL2 rclk_char_mode 50 50, ddr__ptune 49 45, ddr__ntune 44 40, ptune_offset 39 36, ntune_offset 35 32, m180 31 31, byp 30 30, ptune 29 25, ntune 24 20, rodt_ctl 19 16, control_ctl 15 12, cmd_ctl 11 8, ck_ctl 7 4, dqx_ctl 3 0

REG = LMC0_CONFIG 0x8001180088000188 MEMORY 8
REG_FIELD = LMC0_CONFIG bg2_enable 62 62, mode_x4dev 61 61, mode32b 60 60, scrz 59 59, early_unload_d1_r1 58 58, early_unload_d1_r0 57 57, early_unload_d0_r1 56 56, early_unload_d0_r0 55 55, init_status 54 51, mirrmask 50 47, rankmask 46 43, rank_ena 42 42, sref_with_dll 41 41, early_dqx 40 40, ref_zqcs_int 39 18, reset 17 17, ecc_adr 16 16, forcewrite 15 12, idlepower 11 9, pbank_lsb 8 5, row_lsb 4 2, ecc_ena 1 1

REG = LMC0_CONTROL 0x8001180088000190 MEMORY 8
REG_FIELD = LMC0_CONTROL scramble_ena 63 63, thrcnt 62 51, persub 50 43, thrmax 42 39, crm_cnt 38 34, crm_thr 33 29, crm_max 28 24, rodt_bprch 23 23, wodt_bprch 22 22, bprch 21 20, ext_zqcs_dis 19 19, int_zqcs_dis 18 18, auto_dclkdis 17 17, xor_bank 16 16, max_write_batch 15 12, nxm_write_en 11 11, elev_prio_dis 10 10, inorder_wr 9 9, inorder_rd 8 8, throttle_wr 7 7, throttle_rd 6 6, fprch2 5 4, pocas 3 3, ddr2t 2 2, bwcnt 1 1, rdimm_ena 0 0

REG = LMC0_DCLK_CNT 0x80011800880001E0 MEMORY 8
REG_FIELD = LMC0_DCLK_CNT dclkcnt 63 0

REG = LMC0_DDR4_DIMM_CTL 0x80011800880003F0 MEMORY 8
REG_FIELD = LMC0_DDR4_DIMM_CTL ddr4_dimm1_wmask 21 11, ddr4_dimm0_wmask 10 0

REG = LMC0_DDR_PLL_CTL 0x8001180088000258 MEMORY 8
REG_FIELD = LMC0_DDR_PLL_CTL phy_dcok 30 30, ddr4_mode 29 29, pll_fbslip 28 28, pll_lock 27 27, pll_rfslip 26 26, clkr 25 24, jtg_test_mode 23 23, ddr_div_reset 22 22, ddr_ps_en 21 18, reset_n 7 7, clkf 6 0

REG = LMC0_DIMM000_DDR4_PARAMS0 0x80011800880000D0 MEMORY 8
REG_FIELD = LMC0_DIMM000_DDR4_PARAMS0 rc8x 63 56, rc7x 55 48, rc6x 47 40, rc5x 39 32, rc4x 31 24, rc3x 23 16, rc2x 15 8, rc1x 7 0
REG = LMC0_DIMM001_DDR4_PARAMS0 0x80011800880000D8 MEMORY 8
REG_FIELD = LMC0_DIMM001_DDR4_PARAMS0 rc8x 63 56, rc7x 55 48, rc6x 47 40, rc5x 39 32, rc4x 31 24, rc3x 23 16, rc2x 15 8, rc1x 7 0

REG = LMC0_DIMM000_DDR4_PARAMS1 0x8001180088000140 MEMORY 8
REG_FIELD = LMC0_DIMM000_DDR4_PARAMS1 rcbx 23 16, rcax 15 8, rc9x 7 0
REG = LMC0_DIMM001_DDR4_PARAMS1 0x8001180088000148 MEMORY 8
REG_FIELD = LMC0_DIMM001_DDR4_PARAMS1 rcbx 23 16, rcax 15 8, rc9x 7 0

REG = LMC0_DIMM000_PARAMS 0x8001180088000270 MEMORY 8
REG_FIELD = LMC0_DIMM000_PARAMS rc15 63 60, rc14 59 56, rc13 55 52, rc12 51 48, rc11 47 44, rc10 43 40, rc9 39 36, rc8 35 32, rc7 31 28, rc6 27 24, rc5 23 20, rc4 19 16, rc3 15 12, rc2 11 8, rc1 7 4, rc0 3 0
REG = LMC0_DIMM001_PARAMS 0x8001180088000278 MEMORY 8
REG_FIELD = LMC0_DIMM001_PARAMS rc15 63 60, rc14 59 56, rc13 55 52, rc12 51 48, rc11 47 44, rc10 43 40, rc9 39 36, rc8 35 32, rc7 31 28, rc6 27 24, rc5 23 20, rc4 19 16, rc3 15 12, rc2 11 8, rc1 7 4, rc0 3 0

REG = LMC0_DIMM_CTL 0x8001180088000310 MEMORY 8
REG_FIELD = LMC0_DIMM_CTL parity 45 45, tcws 44 32, dimm1_wmask 31 16, dimm0_wmask 15 0

REG = LMC0_DLL_CTL2 0x80011800880001C8 MEMORY 8
REG_FIELD = LMC0_DLL_CTL2 intf_en 16 16, dll_bringup 15 15, dreset 14 14, quad_dll_ena 13 13, byp_sel 12 9, byp_setting 8 0

REG = LMC0_DLL_CTL3 0x8001180088000218 MEMORY 8
REG_FIELD = LMC0_DLL_CTL3 dclk90_fwd 43 43, ddr_90_dly_byp 42 42, dclk90_recal_dis 41 41, dclk90_byp_sel 40 40, dclk90_byp_setting 39 31, dll_fast 30 30, dll90_setting 29 21, fine_tune_mode 20 20, dll_mode 19 19, dll90_byte_sel 18 15, offset_ena 14 14, load_offset 13 13, mode_sel 12 11, byte_sel 10 7, offset 6 0

REG = LMC0_DUAL_MEMCFG 0x8001180088000098 MEMORY 8
REG_FIELD = LMC0_DUAL_MEMCFG row_lsb 18 16, cs_mask 3 0

REG = LMC0_ECC_SYND 0x8001180088000038 MEMORY 8
REG_FIELD = LMC0_ECC_SYND mrdsyn3 31 24, mrdsyn2 23 16, mrdsyn1 15 8, mrdsyn0 7 0

REG = LMC0_EXT_CONFIG 0x8001180088000030 MEMORY 8
REG_FIELD = LMC0_EXT_CONFIG vrefint_seq_deskew 20 20, read_ena_bprch 19 19, read_ena_fprch 18 18, slot_ctl_reset_force 17 17, ref_int_lsbs 16 8, drive_ena_bprch 7 7, drive_ena_fprch 6 6, dlcram_flip_synd 5 4, dlcram_cor_dis 3 3, dlc_nxm_rd 2 2, l2c_nxm_rd 1 1, l2c_nxm_wr 0 0

REG = LMC0_FADR 0x8001180088000020 MEMORY 8
REG_FIELD = LMC0_FADR fill_order 39 38, fdimm 37 37, fbunk 36 36, fbank 35 32, frow 31 14, fcol 13 0

REG = LMC0_IFB_CNT 0x80011800880001D0 MEMORY 8
REG_FIELD = LMC0_IFB_CNT ifbcnt 63 0

REG = LMC0_INT 0x80011800880001F0 MEMORY 8
REG_FIELD = LMC0_INT ddr_err 11 11, dlcram_ded_err 10 10, dlcram_sec_err 9 9, ded_err 8 5, sec_err 4 1, nxm_wr_err 0 0

REG = LMC0_INT_EN 0x80011800880001E8 MEMORY 8
REG_FIELD = LMC0_INT_EN ddr_error_alert_ena 5 5, dlcram_ded_ena 4 4, dlcram_sec_ena 3 3, intr_ded_ena 2 2, intr_sec_ena 1 1, intr_nxm_wr_ena 0 0

REG = LMC0_MODEREG_PARAMS0 0x80011800880001A8 MEMORY 8
REG_FIELD = LMC0_MODEREG_PARAMS0 ppd 24 24, wrp 23 21, dllr 20 20, tm 19 19, rbt 18 18, cl 17 14, bl 13 12, qoff 11 11, tdqs 10 10, wlev 9 9, al 8 7, dll 6 6, mpr 5 5, mprloc 4 3, cwl 2 0

REG = LMC0_MODEREG_PARAMS1 0x8001180088000260 MEMORY 8
REG_FIELD = LMC0_MODEREG_PARAMS1 rtt_nom_11 47 45, dic_11 44 43, rtt_wr_11 42 41, srt_11 40 40, asr_11 39 39, pasr_11 38 36, rtt_nom_10 35 33, dic_10 32 31, rtt_wr_10 30 29, srt_10 28 28, asr_10 27 27, pasr_10 26 24, rtt_nom_01 23 21, dic_01 20 19, rtt_wr_01 18 17, srt_01 16 16, asr_01 15 15, pasr_01 14 12, rtt_nom_00 11 9, dic_00 8 7, rtt_wr_00 6 5, srt_00 4 4, asr_00 3 3, pasr_00 2 0

REG = LMC0_MODEREG_PARAMS2 0x8001180088000050 MEMORY 8
REG_FIELD = LMC0_MODEREG_PARAMS2 vref_range_11 39 39, vref_value_11 38 33, rtt_park_11 32 30, vref_range_10 29 29, vref_value_10 28 23, rtt_park_10 22 20, vref_range_01 19 19, vref_value_01 18 13, rtt_park_01 12 10, vref_range_00 9 9, vref_value_00 8 3, rtt_park_00 2 0

REG = LMC0_MODEREG_PARAMS3 0x8001180088000058 MEMORY 8
REG_FIELD = LMC0_MODEREG_PARAMS3 mpr_fmt 32 31, wr_cmd_lat 30 29, fgrm 28 26, temp_sense 25 25, pda 24 24, gd 23 23, crc 22 22, lpasr 21 20, tccd_l 19 17, rd_dbi 16 16, wr_dbi 15 15, dm 14 14, ca_par_pers 13 13, odt_pd 12 12, par_lat_mode 11 9, wr_preamble 8 8, rd_preamble 7 7, sre_abort 6 6, cal 5 3, vref_mon 2 2, tc_ref 1 1, max_pd 0 0

REG = LMC0_MPR_DATA0 0x8001180088000070 MEMORY 8
REG_FIELD = LMC0_MPR_DATA0 mpr_data 63 0

REG = LMC0_MPR_DATA1 0x8001180088000078 MEMORY 8
REG_FIELD = LMC0_MPR_DATA1 mpr_data 63 0

REG = LMC0_MPR_DATA2 0x8001180088000080 MEMORY 8
REG_FIELD = LMC0_MPR_DATA2 mpr_data 15 0

REG = LMC0_MR_MPR_CTL 0x8001180088000068 MEMORY 8
REG_FIELD = LMC0_MR_MPR_CTL mpr_whole_byte_enabl 51 51, mpr_byte_select 50 47, mpr_bit_select 46 45, mpr_wr 44 44, mpr_loc 43 42, mr_wr_pda_enable 41 41, mr_wr_pda_mask 40 23, mr_wr_rank 22 21, mr_wr_sel 20 18, mr_wr_addr 17 0

REG = LMC0_NXM 0x80011800880000C8 MEMORY 8
REG_FIELD = LMC0_NXM mem_msb_d1_r1 23 20, mem_msb_d1_r0 19 16, mem_msb_d0_r1 15 12, mem_msb_d0_r0 11 8, cs_mask 3 0

REG = LMC0_NXM_FADR 0x8001180088000028 MEMORY 8
REG_FIELD = LMC0_NXM_FADR nxm_src 38 38, nxm_type 37 37, nxm_faddr 36 0

REG = LMC0_OPS_CNT 0x80011800880001D8 MEMORY 8
REG_FIELD = LMC0_OPS_CNT opscnt 63 0

REG = LMC0_PHY_CTL 0x8001180088000210 MEMORY 8
REG_FIELD = LMC0_PHY_CTL phy_reset 50 50, dsk_dbg_rd_complete 49 49, dsk_dbg_rd_data 48 39, dsk_dbg_rd_start 38 38, dsk_dbg_clk_scaler 37 36, dsk_dbg_offset 35 34, dsk_dbg_num_bits_sel 33 33, dsk_dbg_byte_sel 32 29, dsk_dbg_bit_sel 28 25, dbi_mode_ena 24 24, ddr_error_n_ena 23 23, ref_pin_on 22 22, dac_on 21 21, int_pad_loopback_ena 20 20, int_phy_loopback_ena 19 19, phy_dsk_reset 18 18, phy_dsk_byp 17 17, phy_pwr_save_disable 16 16, ten 15 15, rx_always_on 14 14, lv_mode 13 13, ck_tune1 12 12, ck_dlyout1 11 8, ck_tune0 7 7, ck_dlyout0 6 3, loopback 2 2, loopback_pos 1 1, ts_stagger 0 0

REG = LMC0_RESET_CTL 0x8001180088000180 MEMORY 8
REG_FIELD = LMC0_RESET_CTL ddr3psv 3 3, ddr3psoft 2 2, ddr3pwarm 1 1, ddr3rst 0 0

REG = LMC0_RLEVEL_CTL 0x80011800880002A0 MEMORY 8
REG_FIELD = LMC0_RLEVEL_CTL pattern 31 24, delay_unload_3 21 21, delay_unload_2 20 20, delay_unload_1 19 19, delay_unload_0 18 18, bitmask 17 10, or_dis 9 9, offset_en 8 8, offset 7 4, byte 3 0

REG = LMC0_RLEVEL_DBG 0x80011800880002A8 MEMORY 8
REG_FIELD = LMC0_RLEVEL_DBG bitmask 63 0

REG = LMC0_RLEVEL_RANK000 0x8001180088000280 MEMORY 8
REG_FIELD = LMC0_RLEVEL_RANK000 status 55 54, byte8 53 48, byte7 47 42, byte6 41 36, byte5 35 30, byte4 29 24, byte3 23 18, byte2 17 12, byte1 11 6, byte0 5 0
REG = LMC0_RLEVEL_RANK001 0x8001180088000288 MEMORY 8
REG_FIELD = LMC0_RLEVEL_RANK001 status 55 54, byte8 53 48, byte7 47 42, byte6 41 36, byte5 35 30, byte4 29 24, byte3 23 18, byte2 17 12, byte1 11 6, byte0 5 0
REG = LMC0_RLEVEL_RANK002 0x8001180088000290 MEMORY 8
REG_FIELD = LMC0_RLEVEL_RANK002 status 55 54, byte8 53 48, byte7 47 42, byte6 41 36, byte5 35 30, byte4 29 24, byte3 23 18, byte2 17 12, byte1 11 6, byte0 5 0
REG = LMC0_RLEVEL_RANK003 0x8001180088000298 MEMORY 8
REG_FIELD = LMC0_RLEVEL_RANK003 status 55 54, byte8 53 48, byte7 47 42, byte6 41 36, byte5 35 30, byte4 29 24, byte3 23 18, byte2 17 12, byte1 11 6, byte0 5 0

REG = LMC0_RODT_MASK 0x8001180088000268 MEMORY 8
REG_FIELD = LMC0_RODT_MASK rodt_d1_r1 27 24, rodt_d1_r0 19 16, rodt_d0_r1 11 8, rodt_d0_r0 3 0

REG = LMC0_SCRAMBLE_CFG0 0x8001180088000320 MEMORY 8
REG_FIELD = LMC0_SCRAMBLE_CFG0 key 63 0

REG = LMC0_SCRAMBLE_CFG1 0x8001180088000328 MEMORY 8
REG_FIELD = LMC0_SCRAMBLE_CFG1 key 63 0

REG = LMC0_SCRAMBLED_FADR 0x8001180088000330 MEMORY 8
REG_FIELD = LMC0_SCRAMBLED_FADR fill_order 39 38, fdimm 37 37, fbunk 36 36, fbank 35 32, frow 31 14, fcol 13 0

REG = LMC0_SEQ_CTL 0x8001180088000048 MEMORY 8
REG_FIELD = LMC0_SEQ_CTL seq_complete 5 5, seq_sel 4 1, init_start 0 0

REG = LMC0_SLOT_CTL0 0x80011800880001F8 MEMORY 8
REG_FIELD = LMC0_SLOT_CTL0 w2w_l_init 47 42, w2r_l_init 41 36, r2w_l_init 35 30, r2r_l_init 29 24, w2w_init 23 18, w2r_init 17 12, r2w_init 11 6, r2r_init 5 0

REG = LMC0_SLOT_CTL1 0x8001180088000200 MEMORY 8
REG_FIELD = LMC0_SLOT_CTL1 w2w_xrank_init 23 18, w2r_xrank_init 17 12, r2w_xrank_init 11 6, r2r_xrank_init 5 0

REG = LMC0_SLOT_CTL2 0x8001180088000208 MEMORY 8
REG_FIELD = LMC0_SLOT_CTL2 w2w_xdimm_init 23 18, w2r_xdimm_init 17 12, r2w_xdimm_init 11 6, r2r_xdimm_init 5 0

REG = LMC0_TIMING_PARAMS0 0x8001180088000198 MEMORY 8
REG_FIELD = LMC0_TIMING_PARAMS0 tcksre 47 44, trp 43 39, tzqinit 38 35, tdllk 34 31, tmod 30 26, tmrd 25 22, txpr 21 16, tcke 15 12, tzqcs 11 8

REG = LMC0_TIMING_PARAMS1 0x80011800880001A0 MEMORY 8
REG_FIELD = LMC0_TIMING_PARAMS1 txpdll 48 44, tfaw 43 39, twldqsen 38 35, twlmrd 34 31, txp 30 28, trrd 27 25, trfc 24 18, twtr 17 14, trcd 13 10, tras 9 4, tmprr 3 0

REG = LMC0_TIMING_PARAMS2 0x8001180088000060 MEMORY 8
REG_FIELD = LMC0_TIMING_PARAMS2 trtp 14 11, t_rw_op_max 10 7, twtr_l 6 3, trrd_l 2 0

REG = LMC0_WLEVEL_CTL 0x8001180088000300 MEMORY 8
REG_FIELD = LMC0_WLEVEL_CTL rtt_nom 21 19, bitmask 18 11, or_dis 10 10, sset 9 9, lanemask 8 0

REG = LMC0_WLEVEL_DBG 0x8001180088000308 MEMORY 8
REG_FIELD = LMC0_WLEVEL_DBG bitmask 11 4, byte 3 0

REG = LMC0_WLEVEL_RANK000 0x80011800880002C0 MEMORY 8
REG_FIELD = LMC0_WLEVEL_RANK000 status 46 45, byte8 44 40, byte7 39 35, byte6 34 30, byte5 29 25, byte4 24 20, byte3 19 15, byte2 14 10, byte1 9 5, byte0 4 0
REG = LMC0_WLEVEL_RANK001 0x80011800880002C8 MEMORY 8
REG_FIELD = LMC0_WLEVEL_RANK001 status 46 45, byte8 44 40, byte7 39 35, byte6 34 30, byte5 29 25, byte4 24 20, byte3 19 15, byte2 14 10, byte1 9 5, byte0 4 0
REG = LMC0_WLEVEL_RANK002 0x80011800880002D0 MEMORY 8
REG_FIELD = LMC0_WLEVEL_RANK002 status 46 45, byte8 44 40, byte7 39 35, byte6 34 30, byte5 29 25, byte4 24 20, byte3 19 15, byte2 14 10, byte1 9 5, byte0 4 0
REG = LMC0_WLEVEL_RANK003 0x80011800880002D8 MEMORY 8
REG_FIELD = LMC0_WLEVEL_RANK003 status 46 45, byte8 44 40, byte7 39 35, byte6 34 30, byte5 29 25, byte4 24 20, byte3 19 15, byte2 14 10, byte1 9 5, byte0 4 0

REG = LMC0_WODT_MASK 0x80011800880001B0 MEMORY 8
REG_FIELD = LMC0_WODT_MASK wodt_d1_r1 27 24, wodt_d1_r0 19 16, wodt_d0_r1 11 8, wodt_d0_r0 3 0

REG = MIO_BOOT_BIST_STAT 0x80011800000000F8 MEMORY 8
REG_FIELD = MIO_BOOT_BIST_STAT stat 13 0

REG = MIO_BOOT_COMP 0x80011800000000B8 MEMORY 8
REG_FIELD = MIO_BOOT_COMP pctl 10 8, nctl 2 0

REG = MIO_BOOT_CTL 0x80011800000000D0 MEMORY 8
REG_FIELD = MIO_BOOT_CTL sel 0 0

REG = MIO_BOOT_ERR 0x80011800000000A0 MEMORY 8
REG_FIELD = MIO_BOOT_ERR wait_err 1 1, adr_err 0 0

REG = MIO_BOOT_INT 0x80011800000000A8 MEMORY 8
REG_FIELD = MIO_BOOT_INT wait_int 1 1, adr_int 0 0

REG = MIO_BOOT_LOC_ADR 0x8001180000000090 MEMORY 8
REG_FIELD = MIO_BOOT_LOC_ADR adr 7 3

REG = MIO_BOOT_LOC_CFG0 0x8001180000000080 MEMORY 8
REG_FIELD = MIO_BOOT_LOC_CFG0 en 31 31, base 27 3
REG = MIO_BOOT_LOC_CFG1 0x8001180000000088 MEMORY 8
REG_FIELD = MIO_BOOT_LOC_CFG1 en 31 31, base 27 3

REG = MIO_BOOT_LOC_DAT 0x8001180000000098 MEMORY 8
REG_FIELD = MIO_BOOT_LOC_DAT data 63 0

REG = MIO_BOOT_PIN_DEFS 0x80011800000000C0 MEMORY 8
REG_FIELD = MIO_BOOT_PIN_DEFS dlm_supply 32 32, rgm_supply 31 30, smi_supply 29 27, io_supply 26 24, ref_sel 16 16, ale 15 15, width 14 14, vrm_disable 13 13, device 12 10, term 9 8, user0 7 0

REG = MIO_BOOT_REG_CFG0 0x8001180000000000 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG0 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG1 0x8001180000000008 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG1 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG2 0x8001180000000010 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG2 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG3 0x8001180000000018 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG3 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG4 0x8001180000000020 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG4 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG5 0x8001180000000028 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG5 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG6 0x8001180000000030 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG6 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0
REG = MIO_BOOT_REG_CFG7 0x8001180000000038 MEMORY 8
REG_FIELD = MIO_BOOT_REG_CFG7 dmack 43 42, tim_mult 41 40, rd_dly 39 37, sam 36 36, we_ext 35 34, oe_ext 33 32, en 31 31, orbit 30 30, ale 29 29, width 28 28, size 27 16, base 15 0

REG = MIO_BOOT_REG_TIM0 0x8001180000000040 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM0 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM1 0x8001180000000048 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM1 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM2 0x8001180000000050 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM2 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM3 0x8001180000000058 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM3 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM4 0x8001180000000060 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM4 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM5 0x8001180000000068 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM5 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM6 0x8001180000000070 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM6 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0
REG = MIO_BOOT_REG_TIM7 0x8001180000000078 MEMORY 8
REG_FIELD = MIO_BOOT_REG_TIM7 pagem 63 63, waitm 62 62, pages 61 60, ale 59 54, page 53 48, wait 47 42, pause 41 36, wr_hld 35 30, rd_hld 29 24, we 23 18, oe 17 12, ce 11 6, adr 5 0

REG = MIO_BOOT_THR 0x80011800000000B0 MEMORY 8
REG_FIELD = MIO_BOOT_THR dma_thr 21 16, fif_cnt 13 8, fif_thr 5 0

REG = MIO_EMM_BUF_DAT 0x80011800000020E8 MEMORY 8
REG_FIELD = MIO_EMM_BUF_DAT dat 63 0

REG = MIO_EMM_BUF_IDX 0x80011800000020E0 MEMORY 8
REG_FIELD = MIO_EMM_BUF_IDX inc 16 16, buf_num 6 6, offset 5 0

REG = MIO_EMM_CFG 0x8001180000002000 MEMORY 8
REG_FIELD = MIO_EMM_CFG boot_fail 16 16, bus_ena 3 0

REG = MIO_EMM_CMD 0x8001180000002058 MEMORY 8
REG_FIELD = MIO_EMM_CMD bus_id 61 60, cmd_val 59 59, dbuf 55 55, offset 54 49, ctype_xor 42 41, rtype_xor 40 38, cmd_idx 37 32, arg 31 0

REG = MIO_EMM_DMA 0x8001180000002050 MEMORY 8
REG_FIELD = MIO_EMM_DMA bus_id 61 60, dma_val 59 59, sector 58 58, dat_null 57 57, thres 56 51, rel_wr 50 50, rw 49 49, multi 48 48, block_cnt 47 32, card_addr 31 0

REG = MIO_EMM_INT 0x8001180000002078 MEMORY 8
REG_FIELD = MIO_EMM_INT switch_err 6 6, switch_done 5 5, dma_err 4 4, cmd_err 3 3, dma_done 2 2, cmd_done 1 1, buf_done 0 0

REG = MIO_EMM_INT_EN 0x8001180000002080 MEMORY 8
REG_FIELD = MIO_EMM_INT_EN switch_err 6 6, switch_done 5 5, dma_err 4 4, cmd_err 3 3, dma_done 2 2, cmd_done 1 1, buf_done 0 0

REG = MIO_EMM_MODE0 0x8001180000002008 MEMORY 8
REG_FIELD = MIO_EMM_MODE0 hs_timing 48 48, bus_width 42 40, power_class 35 32, clk_hi 31 16, clk_lo 15 0
REG = MIO_EMM_MODE1 0x8001180000002010 MEMORY 8
REG_FIELD = MIO_EMM_MODE1 hs_timing 48 48, bus_width 42 40, power_class 35 32, clk_hi 31 16, clk_lo 15 0
REG = MIO_EMM_MODE2 0x8001180000002018 MEMORY 8
REG_FIELD = MIO_EMM_MODE2 hs_timing 48 48, bus_width 42 40, power_class 35 32, clk_hi 31 16, clk_lo 15 0
REG = MIO_EMM_MODE3 0x8001180000002020 MEMORY 8
REG_FIELD = MIO_EMM_MODE3 hs_timing 48 48, bus_width 42 40, power_class 35 32, clk_hi 31 16, clk_lo 15 0

REG = MIO_EMM_RCA 0x80011800000020A0 MEMORY 8
REG_FIELD = MIO_EMM_RCA card_rca 15 0

REG = MIO_EMM_RSP_HI 0x8001180000002070 MEMORY 8
REG_FIELD = MIO_EMM_RSP_HI dat 63 0

REG = MIO_EMM_RSP_LO 0x8001180000002068 MEMORY 8
REG_FIELD = MIO_EMM_RSP_LO dat 63 0

REG = MIO_EMM_RSP_STS 0x8001180000002060 MEMORY 8
REG_FIELD = MIO_EMM_RSP_STS bus_id 61 60, cmd_val 59 59, switch_val 58 58, dma_val 57 57, dma_pend 56 56, dbuf_err 28 28, dbuf 23 23, blk_timeout 22 22, blk_crc_err 21 21, rsp_busybit 20 20, stp_timeout 19 19, stp_crc_err 18 18, stp_bad_sts 17 17, stp_val 16 16, rsp_timeout 15 15, rsp_crc_err 14 14, rsp_bad_sts 13 13, rsp_val 12 12, rsp_type 11 9, cmd_type 8 7, cmd_idx 6 1, cmd_done 0 0

REG = MIO_EMM_SAMPLE 0x8001180000002090 MEMORY 8
REG_FIELD = MIO_EMM_SAMPLE cmd_cnt 25 16, dat_cnt 9 0

REG = MIO_EMM_STS_MASK 0x8001180000002098 MEMORY 8
REG_FIELD = MIO_EMM_STS_MASK sts_msk 31 0

REG = MIO_EMM_SWITCH 0x8001180000002048 MEMORY 8
REG_FIELD = MIO_EMM_SWITCH bus_id 61 60, switch_exe 59 59, switch_err0 58 58, switch_err1 57 57, switch_err2 56 56, hs_timing 48 48, bus_width 42 40, power_class 35 32, clk_hi 31 16, clk_lo 15 0

REG = MIO_EMM_WDOG 0x8001180000002088 MEMORY 8
REG_FIELD = MIO_EMM_WDOG clk_cnt 25 0

REG = MIO_FUS_BNK_DAT0 0x8001180000001520 MEMORY 8
REG_FIELD = MIO_FUS_BNK_DAT0 dat 63 0
REG = MIO_FUS_BNK_DAT1 0x8001180000001528 MEMORY 8
REG_FIELD = MIO_FUS_BNK_DAT1 dat 63 0

REG = MIO_FUS_DAT0 0x8001180000001400 MEMORY 8
REG_FIELD = MIO_FUS_DAT0 man_info 31 0

REG = MIO_FUS_DAT1 0x8001180000001408 MEMORY 8
REG_FIELD = MIO_FUS_DAT1 man_info 31 0

REG = MIO_FUS_DAT2 0x8001180000001410 MEMORY 8
REG_FIELD = MIO_FUS_DAT2 fus118 47 47, rom_info 46 37, power_limit 36 35, dorm_crypto 34 34, fus318 33 33, raid_en 32 32, nodfa_cp2 28 28, nomul 27 27, nocrypto 26 26, chip_id 23 16

REG = MIO_FUS_DAT3 0x8001180000001418 MEMORY 8
REG_FIELD = MIO_FUS_DAT3 ema0 63 58, pll_ctl 57 48, dfa_info_dte 47 45, dfa_info_clm 44 41, efus_lck_rsv 37 37, efus_lck_man 36 36, pll_half_dis 35 35, l2c_crip 34 32, zip_info 30 29, bar2_sz_conf 28 28, efus_lck 27 27, efus_ign 26 26, nozip 25 25, nodfa_dte 24 24, ema1 23 18

REG = MIO_FUS_DAT4 0x8001180000001420 MEMORY 8
REG_FIELD = MIO_FUS_DAT4 global_rclk_byp_sele 63 63, global_rclk_byp_sett 62 52, east_rclk_byp_select 51 51, east_rclk_byp_settin 50 39, cmb_rclk_byp_select 38 38, cmb_rclk_byp_setting 37 26, pp_rclk_byp_select 25 25, pp_rclk_byp_setting 24 13, tad_rclk_byp_select 12 12, tad_rclk_byp_setting 11 0

REG = MIO_FUS_INT 0x8001180000001548 MEMORY 8
REG_FIELD = MIO_FUS_INT rpr_dbe 1 1, rpr_sbe 0 0

REG = MIO_FUS_PDF 0x8001180000001428 MEMORY 8
REG_FIELD = MIO_FUS_PDF pdf 63 0

REG = MIO_FUS_PLL 0x8001180000001580 MEMORY 8
REG_FIELD = MIO_FUS_PLL core_status 14 12, pnr_status 10 8, c_cout_rst 7 7, c_cout_sel 6 5, pnr_cout_rst 4 4, pnr_cout_sel 3 2

REG = MIO_FUS_PROG 0x8001180000001510 MEMORY 8
REG_FIELD = MIO_FUS_PROG sft 1 1, prog 0 0

REG = MIO_FUS_PROG_TIMES 0x8001180000001518 MEMORY 8
REG_FIELD = MIO_FUS_PROG_TIMES vgate_pin 34 34, fsrc_pin 33 33, prog_pin 32 32

REG = MIO_FUS_RCMD 0x8001180000001500 MEMORY 8
REG_FIELD = MIO_FUS_RCMD dat 23 16, pend 12 12, addr_hi 10 9, efuse 8 8, addr 7 0

REG = MIO_FUS_READ_TIMES 0x8001180000001570 MEMORY 8
REG_FIELD = MIO_FUS_READ_TIMES done 31 28, ahd 27 24, wrstb_wh 23 12, rdstb_wh 11 8, asu 7 4, setup 3 0

REG = MIO_FUS_RPR_DAT0 0x8001180000001530 MEMORY 8
REG_FIELD = MIO_FUS_RPR_DAT0 dat 63 0
REG = MIO_FUS_RPR_DAT1 0x8001180000001538 MEMORY 8
REG_FIELD = MIO_FUS_RPR_DAT1 dat 63 0

REG = MIO_FUS_SOFT_REPAIR 0x8001180000001540 MEMORY 8
REG_FIELD = MIO_FUS_SOFT_REPAIR rpr_flip_synd 19 18, autoblow 17 17, too_many 16 16, numdefects 15 8, numrepairs 7 0

REG = MIO_FUS_TGG 0x8001180000001430 MEMORY 8
REG_FIELD = MIO_FUS_TGG val 63 63, dat 62 0

REG = MIO_FUS_WADR 0x8001180000001508 MEMORY 8
REG_FIELD = MIO_FUS_WADR addr 5 0

REG = MIO_GPIO_COMP 0x80011800000000C8 MEMORY 8
REG_FIELD = MIO_GPIO_COMP pctl 10 8, nctl 2 0

REG = MIO_NDF_DMA_CFG 0x8001180000000168 MEMORY 8
REG_FIELD = MIO_NDF_DMA_CFG en 63 63, rw 62 62, clr 61 61, swap32 59 59, swap16 58 58, swap8 57 57, endian 56 56, size 55 36, adr 35 0

REG = MIO_NDF_DMA_INT 0x8001180000000170 MEMORY 8
REG_FIELD = MIO_NDF_DMA_INT done 0 0

REG = MIO_NDF_DMA_INT_EN 0x8001180000000178 MEMORY 8
REG_FIELD = MIO_NDF_DMA_INT_EN done 0 0

REG = MIO_PTP_CKOUT_HI_INCR 0x8001070000000F40 MEMORY 8
REG_FIELD = MIO_PTP_CKOUT_HI_INCR nanosec 63 32, frnanosec 31 0

REG = MIO_PTP_CKOUT_LO_INCR 0x8001070000000F48 MEMORY 8
REG_FIELD = MIO_PTP_CKOUT_LO_INCR nanosec 63 32, frnanosec 31 0

REG = MIO_PTP_CKOUT_THRESH_HI 0x8001070000000F38 MEMORY 8
REG_FIELD = MIO_PTP_CKOUT_THRESH_HI nanosec 63 0

REG = MIO_PTP_CKOUT_THRESH_LO 0x8001070000000F30 MEMORY 8
REG_FIELD = MIO_PTP_CKOUT_THRESH_LO frnanosec 31 0

REG = MIO_PTP_CLOCK_CFG 0x8001070000000F00 MEMORY 8
REG_FIELD = MIO_PTP_CLOCK_CFG ckout 41 41, pps 40 40, ext_clk_edge 39 38, pps_inv 31 31, pps_en 30 30, ckout_inv 25 25, ckout_en 24 24, evcnt_in 23 18, evcnt_edge 17 17, evcnt_en 16 16, tstmp_in 15 10, tstmp_edge 9 9, tstmp_en 8 8, ext_clk_in 7 2, ext_clk_en 1 1, ptp_en 0 0

REG = MIO_PTP_CLOCK_COMP 0x8001070000000F18 MEMORY 8
REG_FIELD = MIO_PTP_CLOCK_COMP nanosec 63 32, frnanosec 31 0

REG = MIO_PTP_CLOCK_HI 0x8001070000000F10 MEMORY 8
REG_FIELD = MIO_PTP_CLOCK_HI nanosec 63 0

REG = MIO_PTP_CLOCK_LO 0x8001070000000F08 MEMORY 8
REG_FIELD = MIO_PTP_CLOCK_LO frnanosec 31 0

REG = MIO_PTP_EVT_CNT 0x8001070000000F28 MEMORY 8
REG_FIELD = MIO_PTP_EVT_CNT cntr 63 0

REG = MIO_PTP_PPS_HI_INCR 0x8001070000000F60 MEMORY 8
REG_FIELD = MIO_PTP_PPS_HI_INCR nanosec 63 32, frnanosec 31 0

REG = MIO_PTP_PPS_LO_INCR 0x8001070000000F68 MEMORY 8
REG_FIELD = MIO_PTP_PPS_LO_INCR nanosec 63 32, frnanosec 31 0

REG = MIO_PTP_PPS_THRESH_HI 0x8001070000000F58 MEMORY 8
REG_FIELD = MIO_PTP_PPS_THRESH_HI nanosec 63 0

REG = MIO_PTP_PPS_THRESH_LO 0x8001070000000F50 MEMORY 8
REG_FIELD = MIO_PTP_PPS_THRESH_LO frnanosec 31 0

REG = MIO_PTP_TIMESTAMP 0x8001070000000F20 MEMORY 8
REG_FIELD = MIO_PTP_TIMESTAMP nanosec 63 0

REG = MIO_TWS0_INT 0x8001180000001010 MEMORY 8
REG_FIELD = MIO_TWS0_INT scl 11 11, sda 10 10, scl_ovr 9 9, sda_ovr 8 8, core_en 6 6, ts_en 5 5, st_en 4 4, core_int 2 2, ts_int 1 1, st_int 0 0
REG = MIO_TWS1_INT 0x8001180000001210 MEMORY 8
REG_FIELD = MIO_TWS1_INT scl 11 11, sda 10 10, scl_ovr 9 9, sda_ovr 8 8, core_en 6 6, ts_en 5 5, st_en 4 4, core_int 2 2, ts_int 1 1, st_int 0 0

REG = MIO_TWS0_SW_TWSI 0x8001180000001000 MEMORY 8
REG_FIELD = MIO_TWS0_SW_TWSI v 63 63, slonly 62 62, eia 61 61, op 60 57, r 56 56, sovr 55 55, size 54 52, scr 51 50, a 49 40, ia 39 35, eop_ia 34 32, d 31 0
REG = MIO_TWS1_SW_TWSI 0x8001180000001200 MEMORY 8
REG_FIELD = MIO_TWS1_SW_TWSI v 63 63, slonly 62 62, eia 61 61, op 60 57, r 56 56, sovr 55 55, size 54 52, scr 51 50, a 49 40, ia 39 35, eop_ia 34 32, d 31 0

REG = MIO_TWS0_SW_TWSI_EXT 0x8001180000001018 MEMORY 8
REG_FIELD = MIO_TWS0_SW_TWSI_EXT ia 39 32, d 31 0
REG = MIO_TWS1_SW_TWSI_EXT 0x8001180000001218 MEMORY 8
REG_FIELD = MIO_TWS1_SW_TWSI_EXT ia 39 32, d 31 0

REG = MIO_TWS0_TWSI_SW 0x8001180000001008 MEMORY 8
REG_FIELD = MIO_TWS0_TWSI_SW v 63 62, d 31 0
REG = MIO_TWS1_TWSI_SW 0x8001180000001208 MEMORY 8
REG_FIELD = MIO_TWS1_TWSI_SW v 63 62, d 31 0

REG = MIO_UART0_DLH 0x8001180000000888 MEMORY 8
REG_FIELD = MIO_UART0_DLH dlh 7 0
REG = MIO_UART1_DLH 0x8001180000000C88 MEMORY 8
REG_FIELD = MIO_UART1_DLH dlh 7 0

REG = MIO_UART0_DLL 0x8001180000000880 MEMORY 8
REG_FIELD = MIO_UART0_DLL dll 7 0
REG = MIO_UART1_DLL 0x8001180000000C80 MEMORY 8
REG_FIELD = MIO_UART1_DLL dll 7 0

REG = MIO_UART0_FAR 0x8001180000000920 MEMORY 8
REG_FIELD = MIO_UART0_FAR far 0 0
REG = MIO_UART1_FAR 0x8001180000000D20 MEMORY 8
REG_FIELD = MIO_UART1_FAR far 0 0

REG = MIO_UART0_FCR 0x8001180000000850 MEMORY 8
REG_FIELD = MIO_UART0_FCR rxtrig 7 6, txtrig 5 4, txfr 2 2, rxfr 1 1, en 0 0
REG = MIO_UART1_FCR 0x8001180000000C50 MEMORY 8
REG_FIELD = MIO_UART1_FCR rxtrig 7 6, txtrig 5 4, txfr 2 2, rxfr 1 1, en 0 0

REG = MIO_UART0_HTX 0x8001180000000B08 MEMORY 8
REG_FIELD = MIO_UART0_HTX htx 0 0
REG = MIO_UART1_HTX 0x8001180000000F08 MEMORY 8
REG_FIELD = MIO_UART1_HTX htx 0 0

REG = MIO_UART0_IER 0x8001180000000808 MEMORY 8
REG_FIELD = MIO_UART0_IER ptime 7 7, edssi 3 3, elsi 2 2, etbei 1 1, erbfi 0 0
REG = MIO_UART1_IER 0x8001180000000C08 MEMORY 8
REG_FIELD = MIO_UART1_IER ptime 7 7, edssi 3 3, elsi 2 2, etbei 1 1, erbfi 0 0

REG = MIO_UART0_IIR 0x8001180000000810 MEMORY 8
REG_FIELD = MIO_UART0_IIR fen 7 6, iid 3 0
REG = MIO_UART1_IIR 0x8001180000000C10 MEMORY 8
REG_FIELD = MIO_UART1_IIR fen 7 6, iid 3 0

REG = MIO_UART0_LCR 0x8001180000000818 MEMORY 8
REG_FIELD = MIO_UART0_LCR dlab 7 7, brk 6 6, eps 4 4, pen 3 3, stop 2 2, cls 1 0
REG = MIO_UART1_LCR 0x8001180000000C18 MEMORY 8
REG_FIELD = MIO_UART1_LCR dlab 7 7, brk 6 6, eps 4 4, pen 3 3, stop 2 2, cls 1 0

REG = MIO_UART0_LSR 0x8001180000000828 MEMORY 8
REG_FIELD = MIO_UART0_LSR ferr 7 7, temt 6 6, thre 5 5, bi 4 4, fe 3 3, pe 2 2, oe 1 1, dr 0 0
REG = MIO_UART1_LSR 0x8001180000000C28 MEMORY 8
REG_FIELD = MIO_UART1_LSR ferr 7 7, temt 6 6, thre 5 5, bi 4 4, fe 3 3, pe 2 2, oe 1 1, dr 0 0

REG = MIO_UART0_MCR 0x8001180000000820 MEMORY 8
REG_FIELD = MIO_UART0_MCR afce 5 5, loop 4 4, out2 3 3, out1 2 2, rts 1 1, dtr 0 0
REG = MIO_UART1_MCR 0x8001180000000C20 MEMORY 8
REG_FIELD = MIO_UART1_MCR afce 5 5, loop 4 4, out2 3 3, out1 2 2, rts 1 1, dtr 0 0

REG = MIO_UART0_MSR 0x8001180000000830 MEMORY 8
REG_FIELD = MIO_UART0_MSR dcd 7 7, ri 6 6, dsr 5 5, cts 4 4, ddcd 3 3, teri 2 2, ddsr 1 1, dcts 0 0
REG = MIO_UART1_MSR 0x8001180000000C30 MEMORY 8
REG_FIELD = MIO_UART1_MSR dcd 7 7, ri 6 6, dsr 5 5, cts 4 4, ddcd 3 3, teri 2 2, ddsr 1 1, dcts 0 0

REG = MIO_UART0_RBR 0x8001180000000800 MEMORY 8
REG_FIELD = MIO_UART0_RBR rbr 7 0
REG = MIO_UART1_RBR 0x8001180000000C00 MEMORY 8
REG_FIELD = MIO_UART1_RBR rbr 7 0

REG = MIO_UART0_RFL 0x8001180000000A08 MEMORY 8
REG_FIELD = MIO_UART0_RFL rfl 6 0
REG = MIO_UART1_RFL 0x8001180000000E08 MEMORY 8
REG_FIELD = MIO_UART1_RFL rfl 6 0

REG = MIO_UART0_RFW 0x8001180000000930 MEMORY 8
REG_FIELD = MIO_UART0_RFW rffe 9 9, rfpe 8 8, rfwd 7 0
REG = MIO_UART1_RFW 0x8001180000000D30 MEMORY 8
REG_FIELD = MIO_UART1_RFW rffe 9 9, rfpe 8 8, rfwd 7 0

REG = MIO_UART0_SBCR 0x8001180000000A20 MEMORY 8
REG_FIELD = MIO_UART0_SBCR sbcr 0 0
REG = MIO_UART1_SBCR 0x8001180000000E20 MEMORY 8
REG_FIELD = MIO_UART1_SBCR sbcr 0 0

REG = MIO_UART0_SCR 0x8001180000000838 MEMORY 8
REG_FIELD = MIO_UART0_SCR scr 7 0
REG = MIO_UART1_SCR 0x8001180000000C38 MEMORY 8
REG_FIELD = MIO_UART1_SCR scr 7 0

REG = MIO_UART0_SFE 0x8001180000000A30 MEMORY 8
REG_FIELD = MIO_UART0_SFE sfe 0 0
REG = MIO_UART1_SFE 0x8001180000000E30 MEMORY 8
REG_FIELD = MIO_UART1_SFE sfe 0 0

REG = MIO_UART0_SRR 0x8001180000000A10 MEMORY 8
REG_FIELD = MIO_UART0_SRR stfr 2 2, srfr 1 1, usr 0 0
REG = MIO_UART1_SRR 0x8001180000000E10 MEMORY 8
REG_FIELD = MIO_UART1_SRR stfr 2 2, srfr 1 1, usr 0 0

REG = MIO_UART0_SRT 0x8001180000000A38 MEMORY 8
REG_FIELD = MIO_UART0_SRT srt 1 0
REG = MIO_UART1_SRT 0x8001180000000E38 MEMORY 8
REG_FIELD = MIO_UART1_SRT srt 1 0

REG = MIO_UART0_SRTS 0x8001180000000A18 MEMORY 8
REG_FIELD = MIO_UART0_SRTS srts 0 0
REG = MIO_UART1_SRTS 0x8001180000000E18 MEMORY 8
REG_FIELD = MIO_UART1_SRTS srts 0 0

REG = MIO_UART0_STT 0x8001180000000B00 MEMORY 8
REG_FIELD = MIO_UART0_STT stt 1 0
REG = MIO_UART1_STT 0x8001180000000F00 MEMORY 8
REG_FIELD = MIO_UART1_STT stt 1 0

REG = MIO_UART0_TFL 0x8001180000000A00 MEMORY 8
REG_FIELD = MIO_UART0_TFL tfl 6 0
REG = MIO_UART1_TFL 0x8001180000000E00 MEMORY 8
REG_FIELD = MIO_UART1_TFL tfl 6 0

REG = MIO_UART0_TFR 0x8001180000000928 MEMORY 8
REG_FIELD = MIO_UART0_TFR tfr 7 0
REG = MIO_UART1_TFR 0x8001180000000D28 MEMORY 8
REG_FIELD = MIO_UART1_TFR tfr 7 0

REG = MIO_UART0_THR 0x8001180000000840 MEMORY 8
REG_FIELD = MIO_UART0_THR thr 7 0
REG = MIO_UART1_THR 0x8001180000000C40 MEMORY 8
REG_FIELD = MIO_UART1_THR thr 7 0

REG = MIO_UART0_USR 0x8001180000000938 MEMORY 8
REG_FIELD = MIO_UART0_USR rff 4 4, rfne 3 3, tfe 2 2, tfnf 1 1, busy 0 0
REG = MIO_UART1_USR 0x8001180000000D38 MEMORY 8
REG_FIELD = MIO_UART1_USR rff 4 4, rfne 3 3, tfe 2 2, tfnf 1 1, busy 0 0

REG = MPI_CFG 0x8001070000001000 MEMORY 8
REG_FIELD = MPI_CFG clkdiv 28 16, csena3 15 15, csena2 14 14, csena1 13 13, csena0 12 12, cslate 11 11, tritx 10 10, idleclks 9 8, cshi 7 7, int_ena 5 5, lsbfirst 4 4, wireor 3 3, clk_cont 2 2, idlelo 1 1, enable 0 0

REG = MPI_DAT0 0x8001070000001080 MEMORY 8
REG_FIELD = MPI_DAT0 data 7 0
REG = MPI_DAT1 0x8001070000001088 MEMORY 8
REG_FIELD = MPI_DAT1 data 7 0
REG = MPI_DAT2 0x8001070000001090 MEMORY 8
REG_FIELD = MPI_DAT2 data 7 0
REG = MPI_DAT3 0x8001070000001098 MEMORY 8
REG_FIELD = MPI_DAT3 data 7 0
REG = MPI_DAT4 0x80010700000010A0 MEMORY 8
REG_FIELD = MPI_DAT4 data 7 0
REG = MPI_DAT5 0x80010700000010A8 MEMORY 8
REG_FIELD = MPI_DAT5 data 7 0
REG = MPI_DAT6 0x80010700000010B0 MEMORY 8
REG_FIELD = MPI_DAT6 data 7 0
REG = MPI_DAT7 0x80010700000010B8 MEMORY 8
REG_FIELD = MPI_DAT7 data 7 0
REG = MPI_DAT8 0x80010700000010C0 MEMORY 8
REG_FIELD = MPI_DAT8 data 7 0

REG = MPI_STS 0x8001070000001008 MEMORY 8
REG_FIELD = MPI_STS rxnum 12 8, busy 0 0

REG = MPI_TX 0x8001070000001010 MEMORY 8
REG_FIELD = MPI_TX csid 21 20, leavecs 16 16, txnum 12 8, totnum 4 0

REG = MPI_WIDE_DAT 0x8001070000001040 MEMORY 8
REG_FIELD = MPI_WIDE_DAT data 63 0

REG = NDF_BT_PG_INFO 0x8001070001000018 MEMORY 8
REG_FIELD = NDF_BT_PG_INFO t_mult 10 7, adr_cyc 6 3, size 2 0

REG = NDF_CMD 0x8001070001000000 MEMORY 8
REG_FIELD = NDF_CMD nf_cmd 63 0

REG = NDF_DRBELL 0x8001070001000030 MEMORY 8
REG_FIELD = NDF_DRBELL cnt 7 0

REG = NDF_ECC_CNT 0x8001070001000010 MEMORY 8
REG_FIELD = NDF_ECC_CNT xor_ecc 31 8, ecc_err 7 0

REG = NDF_INT 0x8001070001000020 MEMORY 8
REG_FIELD = NDF_INT ovrf 6 6, ecc_mult 5 5, ecc_1bit 4 4, sm_bad 3 3, wdog 2 2, full 1 1, empty 0 0

REG = NDF_INT_EN 0x8001070001000028 MEMORY 8
REG_FIELD = NDF_INT_EN ovrf 6 6, ecc_mult 5 5, ecc_1bit 4 4, sm_bad 3 3, wdog 2 2, full 1 1, empty 0 0

REG = NDF_MISC 0x8001070001000008 MEMORY 8
REG_FIELD = NDF_MISC mb_dis 27 27, nbr_hwm 26 24, wait_cnt 23 18, fr_byt 17 7, rd_done 6 6, rd_val 5 5, rd_cmd 4 4, bt_dma 3 3, bt_dis 2 2, ex_dis 1 1, rst_ff 0 0

REG = NDF_ST_REG 0x8001070001000038 MEMORY 8
REG_FIELD = NDF_ST_REG exe_idle 15 15, exe_sm 14 11, bt_sm 10 7, rd_ff_bad 6 6, rd_ff 5 4, main_bad 3 3, main_sm 2 0

REG = OCLA0_BIST_RESULT 0x80011800A8000040 MEMORY 8
REG_FIELD = OCLA0_BIST_RESULT dat 0 0

REG = OCLA0_CDH000_CTL 0x80011800A8000600 MEMORY 8
REG_FIELD = OCLA0_CDH000_CTL dup 5 5, dis_stamp 4 4, cap_ctl 3 0
REG = OCLA0_CDH001_CTL 0x80011800A8000608 MEMORY 8
REG_FIELD = OCLA0_CDH001_CTL dup 5 5, dis_stamp 4 4, cap_ctl 3 0

REG = OCLA0_CONST 0x80011800A8000000 MEMORY 8
REG_FIELD = OCLA0_CONST dat_size 15 0

REG = OCLA0_DAT000 0x80011800A8400000 MEMORY 8
REG_FIELD = OCLA0_DAT000 entry 37 0
REG = OCLA0_DAT001 0x80011800A8400008 MEMORY 8
REG_FIELD = OCLA0_DAT001 entry 37 0
REG = OCLA0_DAT002 0x80011800A8400010 MEMORY 8
REG_FIELD = OCLA0_DAT002 entry 37 0
REG = OCLA0_DAT003 0x80011800A8400018 MEMORY 8
REG_FIELD = OCLA0_DAT003 entry 37 0
REG = OCLA0_DAT004 0x80011800A8400020 MEMORY 8
REG_FIELD = OCLA0_DAT004 entry 37 0
REG = OCLA0_DAT005 0x80011800A8400028 MEMORY 8
REG_FIELD = OCLA0_DAT005 entry 37 0
REG = OCLA0_DAT006 0x80011800A8400030 MEMORY 8
REG_FIELD = OCLA0_DAT006 entry 37 0
REG = OCLA0_DAT007 0x80011800A8400038 MEMORY 8
REG_FIELD = OCLA0_DAT007 entry 37 0
REG = OCLA0_DAT008 0x80011800A8400040 MEMORY 8
REG_FIELD = OCLA0_DAT008 entry 37 0
REG = OCLA0_DAT009 0x80011800A8400048 MEMORY 8
REG_FIELD = OCLA0_DAT009 entry 37 0
REG = OCLA0_DAT010 0x80011800A8400050 MEMORY 8
REG_FIELD = OCLA0_DAT010 entry 37 0
REG = OCLA0_DAT011 0x80011800A8400058 MEMORY 8
REG_FIELD = OCLA0_DAT011 entry 37 0
REG = OCLA0_DAT012 0x80011800A8400060 MEMORY 8
REG_FIELD = OCLA0_DAT012 entry 37 0
REG = OCLA0_DAT013 0x80011800A8400068 MEMORY 8
REG_FIELD = OCLA0_DAT013 entry 37 0
REG = OCLA0_DAT014 0x80011800A8400070 MEMORY 8
REG_FIELD = OCLA0_DAT014 entry 37 0
REG = OCLA0_DAT015 0x80011800A8400078 MEMORY 8
REG_FIELD = OCLA0_DAT015 entry 37 0
REG = OCLA0_DAT016 0x80011800A8400080 MEMORY 8
REG_FIELD = OCLA0_DAT016 entry 37 0
REG = OCLA0_DAT017 0x80011800A8400088 MEMORY 8
REG_FIELD = OCLA0_DAT017 entry 37 0
REG = OCLA0_DAT018 0x80011800A8400090 MEMORY 8
REG_FIELD = OCLA0_DAT018 entry 37 0
REG = OCLA0_DAT019 0x80011800A8400098 MEMORY 8
REG_FIELD = OCLA0_DAT019 entry 37 0
REG = OCLA0_DAT020 0x80011800A84000A0 MEMORY 8
REG_FIELD = OCLA0_DAT020 entry 37 0
REG = OCLA0_DAT021 0x80011800A84000A8 MEMORY 8
REG_FIELD = OCLA0_DAT021 entry 37 0
REG = OCLA0_DAT022 0x80011800A84000B0 MEMORY 8
REG_FIELD = OCLA0_DAT022 entry 37 0
REG = OCLA0_DAT023 0x80011800A84000B8 MEMORY 8
REG_FIELD = OCLA0_DAT023 entry 37 0
REG = OCLA0_DAT024 0x80011800A84000C0 MEMORY 8
REG_FIELD = OCLA0_DAT024 entry 37 0
REG = OCLA0_DAT025 0x80011800A84000C8 MEMORY 8
REG_FIELD = OCLA0_DAT025 entry 37 0
REG = OCLA0_DAT026 0x80011800A84000D0 MEMORY 8
REG_FIELD = OCLA0_DAT026 entry 37 0
REG = OCLA0_DAT027 0x80011800A84000D8 MEMORY 8
REG_FIELD = OCLA0_DAT027 entry 37 0
REG = OCLA0_DAT028 0x80011800A84000E0 MEMORY 8
REG_FIELD = OCLA0_DAT028 entry 37 0
REG = OCLA0_DAT029 0x80011800A84000E8 MEMORY 8
REG_FIELD = OCLA0_DAT029 entry 37 0
REG = OCLA0_DAT030 0x80011800A84000F0 MEMORY 8
REG_FIELD = OCLA0_DAT030 entry 37 0
REG = OCLA0_DAT031 0x80011800A84000F8 MEMORY 8
REG_FIELD = OCLA0_DAT031 entry 37 0
REG = OCLA0_DAT032 0x80011800A8400100 MEMORY 8
REG_FIELD = OCLA0_DAT032 entry 37 0
REG = OCLA0_DAT033 0x80011800A8400108 MEMORY 8
REG_FIELD = OCLA0_DAT033 entry 37 0
REG = OCLA0_DAT034 0x80011800A8400110 MEMORY 8
REG_FIELD = OCLA0_DAT034 entry 37 0
REG = OCLA0_DAT035 0x80011800A8400118 MEMORY 8
REG_FIELD = OCLA0_DAT035 entry 37 0
REG = OCLA0_DAT036 0x80011800A8400120 MEMORY 8
REG_FIELD = OCLA0_DAT036 entry 37 0
REG = OCLA0_DAT037 0x80011800A8400128 MEMORY 8
REG_FIELD = OCLA0_DAT037 entry 37 0
REG = OCLA0_DAT038 0x80011800A8400130 MEMORY 8
REG_FIELD = OCLA0_DAT038 entry 37 0
REG = OCLA0_DAT039 0x80011800A8400138 MEMORY 8
REG_FIELD = OCLA0_DAT039 entry 37 0
REG = OCLA0_DAT040 0x80011800A8400140 MEMORY 8
REG_FIELD = OCLA0_DAT040 entry 37 0
REG = OCLA0_DAT041 0x80011800A8400148 MEMORY 8
REG_FIELD = OCLA0_DAT041 entry 37 0
REG = OCLA0_DAT042 0x80011800A8400150 MEMORY 8
REG_FIELD = OCLA0_DAT042 entry 37 0
REG = OCLA0_DAT043 0x80011800A8400158 MEMORY 8
REG_FIELD = OCLA0_DAT043 entry 37 0
REG = OCLA0_DAT044 0x80011800A8400160 MEMORY 8
REG_FIELD = OCLA0_DAT044 entry 37 0
REG = OCLA0_DAT045 0x80011800A8400168 MEMORY 8
REG_FIELD = OCLA0_DAT045 entry 37 0
REG = OCLA0_DAT046 0x80011800A8400170 MEMORY 8
REG_FIELD = OCLA0_DAT046 entry 37 0
REG = OCLA0_DAT047 0x80011800A8400178 MEMORY 8
REG_FIELD = OCLA0_DAT047 entry 37 0
REG = OCLA0_DAT048 0x80011800A8400180 MEMORY 8
REG_FIELD = OCLA0_DAT048 entry 37 0
REG = OCLA0_DAT049 0x80011800A8400188 MEMORY 8
REG_FIELD = OCLA0_DAT049 entry 37 0
REG = OCLA0_DAT050 0x80011800A8400190 MEMORY 8
REG_FIELD = OCLA0_DAT050 entry 37 0
REG = OCLA0_DAT051 0x80011800A8400198 MEMORY 8
REG_FIELD = OCLA0_DAT051 entry 37 0
REG = OCLA0_DAT052 0x80011800A84001A0 MEMORY 8
REG_FIELD = OCLA0_DAT052 entry 37 0
REG = OCLA0_DAT053 0x80011800A84001A8 MEMORY 8
REG_FIELD = OCLA0_DAT053 entry 37 0
REG = OCLA0_DAT054 0x80011800A84001B0 MEMORY 8
REG_FIELD = OCLA0_DAT054 entry 37 0
REG = OCLA0_DAT055 0x80011800A84001B8 MEMORY 8
REG_FIELD = OCLA0_DAT055 entry 37 0
REG = OCLA0_DAT056 0x80011800A84001C0 MEMORY 8
REG_FIELD = OCLA0_DAT056 entry 37 0
REG = OCLA0_DAT057 0x80011800A84001C8 MEMORY 8
REG_FIELD = OCLA0_DAT057 entry 37 0
REG = OCLA0_DAT058 0x80011800A84001D0 MEMORY 8
REG_FIELD = OCLA0_DAT058 entry 37 0
REG = OCLA0_DAT059 0x80011800A84001D8 MEMORY 8
REG_FIELD = OCLA0_DAT059 entry 37 0
REG = OCLA0_DAT060 0x80011800A84001E0 MEMORY 8
REG_FIELD = OCLA0_DAT060 entry 37 0
REG = OCLA0_DAT061 0x80011800A84001E8 MEMORY 8
REG_FIELD = OCLA0_DAT061 entry 37 0
REG = OCLA0_DAT062 0x80011800A84001F0 MEMORY 8
REG_FIELD = OCLA0_DAT062 entry 37 0
REG = OCLA0_DAT063 0x80011800A84001F8 MEMORY 8
REG_FIELD = OCLA0_DAT063 entry 37 0
REG = OCLA0_DAT064 0x80011800A8400200 MEMORY 8
REG_FIELD = OCLA0_DAT064 entry 37 0
REG = OCLA0_DAT065 0x80011800A8400208 MEMORY 8
REG_FIELD = OCLA0_DAT065 entry 37 0
REG = OCLA0_DAT066 0x80011800A8400210 MEMORY 8
REG_FIELD = OCLA0_DAT066 entry 37 0
REG = OCLA0_DAT067 0x80011800A8400218 MEMORY 8
REG_FIELD = OCLA0_DAT067 entry 37 0
REG = OCLA0_DAT068 0x80011800A8400220 MEMORY 8
REG_FIELD = OCLA0_DAT068 entry 37 0
REG = OCLA0_DAT069 0x80011800A8400228 MEMORY 8
REG_FIELD = OCLA0_DAT069 entry 37 0
REG = OCLA0_DAT070 0x80011800A8400230 MEMORY 8
REG_FIELD = OCLA0_DAT070 entry 37 0
REG = OCLA0_DAT071 0x80011800A8400238 MEMORY 8
REG_FIELD = OCLA0_DAT071 entry 37 0
REG = OCLA0_DAT072 0x80011800A8400240 MEMORY 8
REG_FIELD = OCLA0_DAT072 entry 37 0
REG = OCLA0_DAT073 0x80011800A8400248 MEMORY 8
REG_FIELD = OCLA0_DAT073 entry 37 0
REG = OCLA0_DAT074 0x80011800A8400250 MEMORY 8
REG_FIELD = OCLA0_DAT074 entry 37 0
REG = OCLA0_DAT075 0x80011800A8400258 MEMORY 8
REG_FIELD = OCLA0_DAT075 entry 37 0
REG = OCLA0_DAT076 0x80011800A8400260 MEMORY 8
REG_FIELD = OCLA0_DAT076 entry 37 0
REG = OCLA0_DAT077 0x80011800A8400268 MEMORY 8
REG_FIELD = OCLA0_DAT077 entry 37 0
REG = OCLA0_DAT078 0x80011800A8400270 MEMORY 8
REG_FIELD = OCLA0_DAT078 entry 37 0
REG = OCLA0_DAT079 0x80011800A8400278 MEMORY 8
REG_FIELD = OCLA0_DAT079 entry 37 0
REG = OCLA0_DAT080 0x80011800A8400280 MEMORY 8
REG_FIELD = OCLA0_DAT080 entry 37 0
REG = OCLA0_DAT081 0x80011800A8400288 MEMORY 8
REG_FIELD = OCLA0_DAT081 entry 37 0
REG = OCLA0_DAT082 0x80011800A8400290 MEMORY 8
REG_FIELD = OCLA0_DAT082 entry 37 0
REG = OCLA0_DAT083 0x80011800A8400298 MEMORY 8
REG_FIELD = OCLA0_DAT083 entry 37 0
REG = OCLA0_DAT084 0x80011800A84002A0 MEMORY 8
REG_FIELD = OCLA0_DAT084 entry 37 0
REG = OCLA0_DAT085 0x80011800A84002A8 MEMORY 8
REG_FIELD = OCLA0_DAT085 entry 37 0
REG = OCLA0_DAT086 0x80011800A84002B0 MEMORY 8
REG_FIELD = OCLA0_DAT086 entry 37 0
REG = OCLA0_DAT087 0x80011800A84002B8 MEMORY 8
REG_FIELD = OCLA0_DAT087 entry 37 0
REG = OCLA0_DAT088 0x80011800A84002C0 MEMORY 8
REG_FIELD = OCLA0_DAT088 entry 37 0
REG = OCLA0_DAT089 0x80011800A84002C8 MEMORY 8
REG_FIELD = OCLA0_DAT089 entry 37 0
REG = OCLA0_DAT090 0x80011800A84002D0 MEMORY 8
REG_FIELD = OCLA0_DAT090 entry 37 0
REG = OCLA0_DAT091 0x80011800A84002D8 MEMORY 8
REG_FIELD = OCLA0_DAT091 entry 37 0
REG = OCLA0_DAT092 0x80011800A84002E0 MEMORY 8
REG_FIELD = OCLA0_DAT092 entry 37 0
REG = OCLA0_DAT093 0x80011800A84002E8 MEMORY 8
REG_FIELD = OCLA0_DAT093 entry 37 0
REG = OCLA0_DAT094 0x80011800A84002F0 MEMORY 8
REG_FIELD = OCLA0_DAT094 entry 37 0
REG = OCLA0_DAT095 0x80011800A84002F8 MEMORY 8
REG_FIELD = OCLA0_DAT095 entry 37 0
REG = OCLA0_DAT096 0x80011800A8400300 MEMORY 8
REG_FIELD = OCLA0_DAT096 entry 37 0
REG = OCLA0_DAT097 0x80011800A8400308 MEMORY 8
REG_FIELD = OCLA0_DAT097 entry 37 0
REG = OCLA0_DAT098 0x80011800A8400310 MEMORY 8
REG_FIELD = OCLA0_DAT098 entry 37 0
REG = OCLA0_DAT099 0x80011800A8400318 MEMORY 8
REG_FIELD = OCLA0_DAT099 entry 37 0
REG = OCLA0_DAT100 0x80011800A8400320 MEMORY 8
REG_FIELD = OCLA0_DAT100 entry 37 0
REG = OCLA0_DAT101 0x80011800A8400328 MEMORY 8
REG_FIELD = OCLA0_DAT101 entry 37 0
REG = OCLA0_DAT102 0x80011800A8400330 MEMORY 8
REG_FIELD = OCLA0_DAT102 entry 37 0
REG = OCLA0_DAT103 0x80011800A8400338 MEMORY 8
REG_FIELD = OCLA0_DAT103 entry 37 0
REG = OCLA0_DAT104 0x80011800A8400340 MEMORY 8
REG_FIELD = OCLA0_DAT104 entry 37 0
REG = OCLA0_DAT105 0x80011800A8400348 MEMORY 8
REG_FIELD = OCLA0_DAT105 entry 37 0
REG = OCLA0_DAT106 0x80011800A8400350 MEMORY 8
REG_FIELD = OCLA0_DAT106 entry 37 0
REG = OCLA0_DAT107 0x80011800A8400358 MEMORY 8
REG_FIELD = OCLA0_DAT107 entry 37 0
REG = OCLA0_DAT108 0x80011800A8400360 MEMORY 8
REG_FIELD = OCLA0_DAT108 entry 37 0
REG = OCLA0_DAT109 0x80011800A8400368 MEMORY 8
REG_FIELD = OCLA0_DAT109 entry 37 0
REG = OCLA0_DAT110 0x80011800A8400370 MEMORY 8
REG_FIELD = OCLA0_DAT110 entry 37 0
REG = OCLA0_DAT111 0x80011800A8400378 MEMORY 8
REG_FIELD = OCLA0_DAT111 entry 37 0
REG = OCLA0_DAT112 0x80011800A8400380 MEMORY 8
REG_FIELD = OCLA0_DAT112 entry 37 0
REG = OCLA0_DAT113 0x80011800A8400388 MEMORY 8
REG_FIELD = OCLA0_DAT113 entry 37 0
REG = OCLA0_DAT114 0x80011800A8400390 MEMORY 8
REG_FIELD = OCLA0_DAT114 entry 37 0
REG = OCLA0_DAT115 0x80011800A8400398 MEMORY 8
REG_FIELD = OCLA0_DAT115 entry 37 0
REG = OCLA0_DAT116 0x80011800A84003A0 MEMORY 8
REG_FIELD = OCLA0_DAT116 entry 37 0
REG = OCLA0_DAT117 0x80011800A84003A8 MEMORY 8
REG_FIELD = OCLA0_DAT117 entry 37 0
REG = OCLA0_DAT118 0x80011800A84003B0 MEMORY 8
REG_FIELD = OCLA0_DAT118 entry 37 0
REG = OCLA0_DAT119 0x80011800A84003B8 MEMORY 8
REG_FIELD = OCLA0_DAT119 entry 37 0
REG = OCLA0_DAT120 0x80011800A84003C0 MEMORY 8
REG_FIELD = OCLA0_DAT120 entry 37 0
REG = OCLA0_DAT121 0x80011800A84003C8 MEMORY 8
REG_FIELD = OCLA0_DAT121 entry 37 0
REG = OCLA0_DAT122 0x80011800A84003D0 MEMORY 8
REG_FIELD = OCLA0_DAT122 entry 37 0
REG = OCLA0_DAT123 0x80011800A84003D8 MEMORY 8
REG_FIELD = OCLA0_DAT123 entry 37 0
REG = OCLA0_DAT124 0x80011800A84003E0 MEMORY 8
REG_FIELD = OCLA0_DAT124 entry 37 0
REG = OCLA0_DAT125 0x80011800A84003E8 MEMORY 8
REG_FIELD = OCLA0_DAT125 entry 37 0
REG = OCLA0_DAT126 0x80011800A84003F0 MEMORY 8
REG_FIELD = OCLA0_DAT126 entry 37 0
REG = OCLA0_DAT127 0x80011800A84003F8 MEMORY 8
REG_FIELD = OCLA0_DAT127 entry 37 0
REG = OCLA0_DAT128 0x80011800A8400400 MEMORY 8
REG_FIELD = OCLA0_DAT128 entry 37 0
REG = OCLA0_DAT129 0x80011800A8400408 MEMORY 8
REG_FIELD = OCLA0_DAT129 entry 37 0
REG = OCLA0_DAT130 0x80011800A8400410 MEMORY 8
REG_FIELD = OCLA0_DAT130 entry 37 0
REG = OCLA0_DAT131 0x80011800A8400418 MEMORY 8
REG_FIELD = OCLA0_DAT131 entry 37 0
REG = OCLA0_DAT132 0x80011800A8400420 MEMORY 8
REG_FIELD = OCLA0_DAT132 entry 37 0
REG = OCLA0_DAT133 0x80011800A8400428 MEMORY 8
REG_FIELD = OCLA0_DAT133 entry 37 0
REG = OCLA0_DAT134 0x80011800A8400430 MEMORY 8
REG_FIELD = OCLA0_DAT134 entry 37 0
REG = OCLA0_DAT135 0x80011800A8400438 MEMORY 8
REG_FIELD = OCLA0_DAT135 entry 37 0
REG = OCLA0_DAT136 0x80011800A8400440 MEMORY 8
REG_FIELD = OCLA0_DAT136 entry 37 0
REG = OCLA0_DAT137 0x80011800A8400448 MEMORY 8
REG_FIELD = OCLA0_DAT137 entry 37 0
REG = OCLA0_DAT138 0x80011800A8400450 MEMORY 8
REG_FIELD = OCLA0_DAT138 entry 37 0
REG = OCLA0_DAT139 0x80011800A8400458 MEMORY 8
REG_FIELD = OCLA0_DAT139 entry 37 0
REG = OCLA0_DAT140 0x80011800A8400460 MEMORY 8
REG_FIELD = OCLA0_DAT140 entry 37 0
REG = OCLA0_DAT141 0x80011800A8400468 MEMORY 8
REG_FIELD = OCLA0_DAT141 entry 37 0
REG = OCLA0_DAT142 0x80011800A8400470 MEMORY 8
REG_FIELD = OCLA0_DAT142 entry 37 0
REG = OCLA0_DAT143 0x80011800A8400478 MEMORY 8
REG_FIELD = OCLA0_DAT143 entry 37 0
REG = OCLA0_DAT144 0x80011800A8400480 MEMORY 8
REG_FIELD = OCLA0_DAT144 entry 37 0
REG = OCLA0_DAT145 0x80011800A8400488 MEMORY 8
REG_FIELD = OCLA0_DAT145 entry 37 0
REG = OCLA0_DAT146 0x80011800A8400490 MEMORY 8
REG_FIELD = OCLA0_DAT146 entry 37 0
REG = OCLA0_DAT147 0x80011800A8400498 MEMORY 8
REG_FIELD = OCLA0_DAT147 entry 37 0
REG = OCLA0_DAT148 0x80011800A84004A0 MEMORY 8
REG_FIELD = OCLA0_DAT148 entry 37 0
REG = OCLA0_DAT149 0x80011800A84004A8 MEMORY 8
REG_FIELD = OCLA0_DAT149 entry 37 0
REG = OCLA0_DAT150 0x80011800A84004B0 MEMORY 8
REG_FIELD = OCLA0_DAT150 entry 37 0
REG = OCLA0_DAT151 0x80011800A84004B8 MEMORY 8
REG_FIELD = OCLA0_DAT151 entry 37 0
REG = OCLA0_DAT152 0x80011800A84004C0 MEMORY 8
REG_FIELD = OCLA0_DAT152 entry 37 0
REG = OCLA0_DAT153 0x80011800A84004C8 MEMORY 8
REG_FIELD = OCLA0_DAT153 entry 37 0
REG = OCLA0_DAT154 0x80011800A84004D0 MEMORY 8
REG_FIELD = OCLA0_DAT154 entry 37 0
REG = OCLA0_DAT155 0x80011800A84004D8 MEMORY 8
REG_FIELD = OCLA0_DAT155 entry 37 0
REG = OCLA0_DAT156 0x80011800A84004E0 MEMORY 8
REG_FIELD = OCLA0_DAT156 entry 37 0
REG = OCLA0_DAT157 0x80011800A84004E8 MEMORY 8
REG_FIELD = OCLA0_DAT157 entry 37 0
REG = OCLA0_DAT158 0x80011800A84004F0 MEMORY 8
REG_FIELD = OCLA0_DAT158 entry 37 0
REG = OCLA0_DAT159 0x80011800A84004F8 MEMORY 8
REG_FIELD = OCLA0_DAT159 entry 37 0
REG = OCLA0_DAT160 0x80011800A8400500 MEMORY 8
REG_FIELD = OCLA0_DAT160 entry 37 0
REG = OCLA0_DAT161 0x80011800A8400508 MEMORY 8
REG_FIELD = OCLA0_DAT161 entry 37 0
REG = OCLA0_DAT162 0x80011800A8400510 MEMORY 8
REG_FIELD = OCLA0_DAT162 entry 37 0
REG = OCLA0_DAT163 0x80011800A8400518 MEMORY 8
REG_FIELD = OCLA0_DAT163 entry 37 0
REG = OCLA0_DAT164 0x80011800A8400520 MEMORY 8
REG_FIELD = OCLA0_DAT164 entry 37 0
REG = OCLA0_DAT165 0x80011800A8400528 MEMORY 8
REG_FIELD = OCLA0_DAT165 entry 37 0
REG = OCLA0_DAT166 0x80011800A8400530 MEMORY 8
REG_FIELD = OCLA0_DAT166 entry 37 0
REG = OCLA0_DAT167 0x80011800A8400538 MEMORY 8
REG_FIELD = OCLA0_DAT167 entry 37 0
REG = OCLA0_DAT168 0x80011800A8400540 MEMORY 8
REG_FIELD = OCLA0_DAT168 entry 37 0
REG = OCLA0_DAT169 0x80011800A8400548 MEMORY 8
REG_FIELD = OCLA0_DAT169 entry 37 0
REG = OCLA0_DAT170 0x80011800A8400550 MEMORY 8
REG_FIELD = OCLA0_DAT170 entry 37 0
REG = OCLA0_DAT171 0x80011800A8400558 MEMORY 8
REG_FIELD = OCLA0_DAT171 entry 37 0
REG = OCLA0_DAT172 0x80011800A8400560 MEMORY 8
REG_FIELD = OCLA0_DAT172 entry 37 0
REG = OCLA0_DAT173 0x80011800A8400568 MEMORY 8
REG_FIELD = OCLA0_DAT173 entry 37 0
REG = OCLA0_DAT174 0x80011800A8400570 MEMORY 8
REG_FIELD = OCLA0_DAT174 entry 37 0
REG = OCLA0_DAT175 0x80011800A8400578 MEMORY 8
REG_FIELD = OCLA0_DAT175 entry 37 0
REG = OCLA0_DAT176 0x80011800A8400580 MEMORY 8
REG_FIELD = OCLA0_DAT176 entry 37 0
REG = OCLA0_DAT177 0x80011800A8400588 MEMORY 8
REG_FIELD = OCLA0_DAT177 entry 37 0
REG = OCLA0_DAT178 0x80011800A8400590 MEMORY 8
REG_FIELD = OCLA0_DAT178 entry 37 0
REG = OCLA0_DAT179 0x80011800A8400598 MEMORY 8
REG_FIELD = OCLA0_DAT179 entry 37 0
REG = OCLA0_DAT180 0x80011800A84005A0 MEMORY 8
REG_FIELD = OCLA0_DAT180 entry 37 0
REG = OCLA0_DAT181 0x80011800A84005A8 MEMORY 8
REG_FIELD = OCLA0_DAT181 entry 37 0
REG = OCLA0_DAT182 0x80011800A84005B0 MEMORY 8
REG_FIELD = OCLA0_DAT182 entry 37 0
REG = OCLA0_DAT183 0x80011800A84005B8 MEMORY 8
REG_FIELD = OCLA0_DAT183 entry 37 0
REG = OCLA0_DAT184 0x80011800A84005C0 MEMORY 8
REG_FIELD = OCLA0_DAT184 entry 37 0
REG = OCLA0_DAT185 0x80011800A84005C8 MEMORY 8
REG_FIELD = OCLA0_DAT185 entry 37 0
REG = OCLA0_DAT186 0x80011800A84005D0 MEMORY 8
REG_FIELD = OCLA0_DAT186 entry 37 0
REG = OCLA0_DAT187 0x80011800A84005D8 MEMORY 8
REG_FIELD = OCLA0_DAT187 entry 37 0
REG = OCLA0_DAT188 0x80011800A84005E0 MEMORY 8
REG_FIELD = OCLA0_DAT188 entry 37 0
REG = OCLA0_DAT189 0x80011800A84005E8 MEMORY 8
REG_FIELD = OCLA0_DAT189 entry 37 0
REG = OCLA0_DAT190 0x80011800A84005F0 MEMORY 8
REG_FIELD = OCLA0_DAT190 entry 37 0
REG = OCLA0_DAT191 0x80011800A84005F8 MEMORY 8
REG_FIELD = OCLA0_DAT191 entry 37 0
REG = OCLA0_DAT192 0x80011800A8400600 MEMORY 8
REG_FIELD = OCLA0_DAT192 entry 37 0
REG = OCLA0_DAT193 0x80011800A8400608 MEMORY 8
REG_FIELD = OCLA0_DAT193 entry 37 0
REG = OCLA0_DAT194 0x80011800A8400610 MEMORY 8
REG_FIELD = OCLA0_DAT194 entry 37 0
REG = OCLA0_DAT195 0x80011800A8400618 MEMORY 8
REG_FIELD = OCLA0_DAT195 entry 37 0
REG = OCLA0_DAT196 0x80011800A8400620 MEMORY 8
REG_FIELD = OCLA0_DAT196 entry 37 0
REG = OCLA0_DAT197 0x80011800A8400628 MEMORY 8
REG_FIELD = OCLA0_DAT197 entry 37 0
REG = OCLA0_DAT198 0x80011800A8400630 MEMORY 8
REG_FIELD = OCLA0_DAT198 entry 37 0
REG = OCLA0_DAT199 0x80011800A8400638 MEMORY 8
REG_FIELD = OCLA0_DAT199 entry 37 0
REG = OCLA0_DAT200 0x80011800A8400640 MEMORY 8
REG_FIELD = OCLA0_DAT200 entry 37 0
REG = OCLA0_DAT201 0x80011800A8400648 MEMORY 8
REG_FIELD = OCLA0_DAT201 entry 37 0
REG = OCLA0_DAT202 0x80011800A8400650 MEMORY 8
REG_FIELD = OCLA0_DAT202 entry 37 0
REG = OCLA0_DAT203 0x80011800A8400658 MEMORY 8
REG_FIELD = OCLA0_DAT203 entry 37 0
REG = OCLA0_DAT204 0x80011800A8400660 MEMORY 8
REG_FIELD = OCLA0_DAT204 entry 37 0
REG = OCLA0_DAT205 0x80011800A8400668 MEMORY 8
REG_FIELD = OCLA0_DAT205 entry 37 0
REG = OCLA0_DAT206 0x80011800A8400670 MEMORY 8
REG_FIELD = OCLA0_DAT206 entry 37 0
REG = OCLA0_DAT207 0x80011800A8400678 MEMORY 8
REG_FIELD = OCLA0_DAT207 entry 37 0
REG = OCLA0_DAT208 0x80011800A8400680 MEMORY 8
REG_FIELD = OCLA0_DAT208 entry 37 0
REG = OCLA0_DAT209 0x80011800A8400688 MEMORY 8
REG_FIELD = OCLA0_DAT209 entry 37 0
REG = OCLA0_DAT210 0x80011800A8400690 MEMORY 8
REG_FIELD = OCLA0_DAT210 entry 37 0
REG = OCLA0_DAT211 0x80011800A8400698 MEMORY 8
REG_FIELD = OCLA0_DAT211 entry 37 0
REG = OCLA0_DAT212 0x80011800A84006A0 MEMORY 8
REG_FIELD = OCLA0_DAT212 entry 37 0
REG = OCLA0_DAT213 0x80011800A84006A8 MEMORY 8
REG_FIELD = OCLA0_DAT213 entry 37 0
REG = OCLA0_DAT214 0x80011800A84006B0 MEMORY 8
REG_FIELD = OCLA0_DAT214 entry 37 0
REG = OCLA0_DAT215 0x80011800A84006B8 MEMORY 8
REG_FIELD = OCLA0_DAT215 entry 37 0
REG = OCLA0_DAT216 0x80011800A84006C0 MEMORY 8
REG_FIELD = OCLA0_DAT216 entry 37 0
REG = OCLA0_DAT217 0x80011800A84006C8 MEMORY 8
REG_FIELD = OCLA0_DAT217 entry 37 0
REG = OCLA0_DAT218 0x80011800A84006D0 MEMORY 8
REG_FIELD = OCLA0_DAT218 entry 37 0
REG = OCLA0_DAT219 0x80011800A84006D8 MEMORY 8
REG_FIELD = OCLA0_DAT219 entry 37 0
REG = OCLA0_DAT220 0x80011800A84006E0 MEMORY 8
REG_FIELD = OCLA0_DAT220 entry 37 0
REG = OCLA0_DAT221 0x80011800A84006E8 MEMORY 8
REG_FIELD = OCLA0_DAT221 entry 37 0
REG = OCLA0_DAT222 0x80011800A84006F0 MEMORY 8
REG_FIELD = OCLA0_DAT222 entry 37 0
REG = OCLA0_DAT223 0x80011800A84006F8 MEMORY 8
REG_FIELD = OCLA0_DAT223 entry 37 0
REG = OCLA0_DAT224 0x80011800A8400700 MEMORY 8
REG_FIELD = OCLA0_DAT224 entry 37 0
REG = OCLA0_DAT225 0x80011800A8400708 MEMORY 8
REG_FIELD = OCLA0_DAT225 entry 37 0
REG = OCLA0_DAT226 0x80011800A8400710 MEMORY 8
REG_FIELD = OCLA0_DAT226 entry 37 0
REG = OCLA0_DAT227 0x80011800A8400718 MEMORY 8
REG_FIELD = OCLA0_DAT227 entry 37 0
REG = OCLA0_DAT228 0x80011800A8400720 MEMORY 8
REG_FIELD = OCLA0_DAT228 entry 37 0
REG = OCLA0_DAT229 0x80011800A8400728 MEMORY 8
REG_FIELD = OCLA0_DAT229 entry 37 0
REG = OCLA0_DAT230 0x80011800A8400730 MEMORY 8
REG_FIELD = OCLA0_DAT230 entry 37 0
REG = OCLA0_DAT231 0x80011800A8400738 MEMORY 8
REG_FIELD = OCLA0_DAT231 entry 37 0
REG = OCLA0_DAT232 0x80011800A8400740 MEMORY 8
REG_FIELD = OCLA0_DAT232 entry 37 0
REG = OCLA0_DAT233 0x80011800A8400748 MEMORY 8
REG_FIELD = OCLA0_DAT233 entry 37 0
REG = OCLA0_DAT234 0x80011800A8400750 MEMORY 8
REG_FIELD = OCLA0_DAT234 entry 37 0
REG = OCLA0_DAT235 0x80011800A8400758 MEMORY 8
REG_FIELD = OCLA0_DAT235 entry 37 0
REG = OCLA0_DAT236 0x80011800A8400760 MEMORY 8
REG_FIELD = OCLA0_DAT236 entry 37 0
REG = OCLA0_DAT237 0x80011800A8400768 MEMORY 8
REG_FIELD = OCLA0_DAT237 entry 37 0
REG = OCLA0_DAT238 0x80011800A8400770 MEMORY 8
REG_FIELD = OCLA0_DAT238 entry 37 0
REG = OCLA0_DAT239 0x80011800A8400778 MEMORY 8
REG_FIELD = OCLA0_DAT239 entry 37 0
REG = OCLA0_DAT240 0x80011800A8400780 MEMORY 8
REG_FIELD = OCLA0_DAT240 entry 37 0
REG = OCLA0_DAT241 0x80011800A8400788 MEMORY 8
REG_FIELD = OCLA0_DAT241 entry 37 0
REG = OCLA0_DAT242 0x80011800A8400790 MEMORY 8
REG_FIELD = OCLA0_DAT242 entry 37 0
REG = OCLA0_DAT243 0x80011800A8400798 MEMORY 8
REG_FIELD = OCLA0_DAT243 entry 37 0
REG = OCLA0_DAT244 0x80011800A84007A0 MEMORY 8
REG_FIELD = OCLA0_DAT244 entry 37 0
REG = OCLA0_DAT245 0x80011800A84007A8 MEMORY 8
REG_FIELD = OCLA0_DAT245 entry 37 0
REG = OCLA0_DAT246 0x80011800A84007B0 MEMORY 8
REG_FIELD = OCLA0_DAT246 entry 37 0
REG = OCLA0_DAT247 0x80011800A84007B8 MEMORY 8
REG_FIELD = OCLA0_DAT247 entry 37 0
REG = OCLA0_DAT248 0x80011800A84007C0 MEMORY 8
REG_FIELD = OCLA0_DAT248 entry 37 0
REG = OCLA0_DAT249 0x80011800A84007C8 MEMORY 8
REG_FIELD = OCLA0_DAT249 entry 37 0
REG = OCLA0_DAT250 0x80011800A84007D0 MEMORY 8
REG_FIELD = OCLA0_DAT250 entry 37 0
REG = OCLA0_DAT251 0x80011800A84007D8 MEMORY 8
REG_FIELD = OCLA0_DAT251 entry 37 0
REG = OCLA0_DAT252 0x80011800A84007E0 MEMORY 8
REG_FIELD = OCLA0_DAT252 entry 37 0
REG = OCLA0_DAT253 0x80011800A84007E8 MEMORY 8
REG_FIELD = OCLA0_DAT253 entry 37 0
REG = OCLA0_DAT254 0x80011800A84007F0 MEMORY 8
REG_FIELD = OCLA0_DAT254 entry 37 0
REG = OCLA0_DAT255 0x80011800A84007F8 MEMORY 8
REG_FIELD = OCLA0_DAT255 entry 37 0
REG = OCLA0_DAT256 0x80011800A8400800 MEMORY 8
REG_FIELD = OCLA0_DAT256 entry 37 0
REG = OCLA0_DAT257 0x80011800A8400808 MEMORY 8
REG_FIELD = OCLA0_DAT257 entry 37 0
REG = OCLA0_DAT258 0x80011800A8400810 MEMORY 8
REG_FIELD = OCLA0_DAT258 entry 37 0
REG = OCLA0_DAT259 0x80011800A8400818 MEMORY 8
REG_FIELD = OCLA0_DAT259 entry 37 0
REG = OCLA0_DAT260 0x80011800A8400820 MEMORY 8
REG_FIELD = OCLA0_DAT260 entry 37 0
REG = OCLA0_DAT261 0x80011800A8400828 MEMORY 8
REG_FIELD = OCLA0_DAT261 entry 37 0
REG = OCLA0_DAT262 0x80011800A8400830 MEMORY 8
REG_FIELD = OCLA0_DAT262 entry 37 0
REG = OCLA0_DAT263 0x80011800A8400838 MEMORY 8
REG_FIELD = OCLA0_DAT263 entry 37 0
REG = OCLA0_DAT264 0x80011800A8400840 MEMORY 8
REG_FIELD = OCLA0_DAT264 entry 37 0
REG = OCLA0_DAT265 0x80011800A8400848 MEMORY 8
REG_FIELD = OCLA0_DAT265 entry 37 0
REG = OCLA0_DAT266 0x80011800A8400850 MEMORY 8
REG_FIELD = OCLA0_DAT266 entry 37 0
REG = OCLA0_DAT267 0x80011800A8400858 MEMORY 8
REG_FIELD = OCLA0_DAT267 entry 37 0
REG = OCLA0_DAT268 0x80011800A8400860 MEMORY 8
REG_FIELD = OCLA0_DAT268 entry 37 0
REG = OCLA0_DAT269 0x80011800A8400868 MEMORY 8
REG_FIELD = OCLA0_DAT269 entry 37 0
REG = OCLA0_DAT270 0x80011800A8400870 MEMORY 8
REG_FIELD = OCLA0_DAT270 entry 37 0
REG = OCLA0_DAT271 0x80011800A8400878 MEMORY 8
REG_FIELD = OCLA0_DAT271 entry 37 0
REG = OCLA0_DAT272 0x80011800A8400880 MEMORY 8
REG_FIELD = OCLA0_DAT272 entry 37 0
REG = OCLA0_DAT273 0x80011800A8400888 MEMORY 8
REG_FIELD = OCLA0_DAT273 entry 37 0
REG = OCLA0_DAT274 0x80011800A8400890 MEMORY 8
REG_FIELD = OCLA0_DAT274 entry 37 0
REG = OCLA0_DAT275 0x80011800A8400898 MEMORY 8
REG_FIELD = OCLA0_DAT275 entry 37 0
REG = OCLA0_DAT276 0x80011800A84008A0 MEMORY 8
REG_FIELD = OCLA0_DAT276 entry 37 0
REG = OCLA0_DAT277 0x80011800A84008A8 MEMORY 8
REG_FIELD = OCLA0_DAT277 entry 37 0
REG = OCLA0_DAT278 0x80011800A84008B0 MEMORY 8
REG_FIELD = OCLA0_DAT278 entry 37 0
REG = OCLA0_DAT279 0x80011800A84008B8 MEMORY 8
REG_FIELD = OCLA0_DAT279 entry 37 0
REG = OCLA0_DAT280 0x80011800A84008C0 MEMORY 8
REG_FIELD = OCLA0_DAT280 entry 37 0
REG = OCLA0_DAT281 0x80011800A84008C8 MEMORY 8
REG_FIELD = OCLA0_DAT281 entry 37 0
REG = OCLA0_DAT282 0x80011800A84008D0 MEMORY 8
REG_FIELD = OCLA0_DAT282 entry 37 0
REG = OCLA0_DAT283 0x80011800A84008D8 MEMORY 8
REG_FIELD = OCLA0_DAT283 entry 37 0
REG = OCLA0_DAT284 0x80011800A84008E0 MEMORY 8
REG_FIELD = OCLA0_DAT284 entry 37 0
REG = OCLA0_DAT285 0x80011800A84008E8 MEMORY 8
REG_FIELD = OCLA0_DAT285 entry 37 0
REG = OCLA0_DAT286 0x80011800A84008F0 MEMORY 8
REG_FIELD = OCLA0_DAT286 entry 37 0
REG = OCLA0_DAT287 0x80011800A84008F8 MEMORY 8
REG_FIELD = OCLA0_DAT287 entry 37 0
REG = OCLA0_DAT288 0x80011800A8400900 MEMORY 8
REG_FIELD = OCLA0_DAT288 entry 37 0
REG = OCLA0_DAT289 0x80011800A8400908 MEMORY 8
REG_FIELD = OCLA0_DAT289 entry 37 0
REG = OCLA0_DAT290 0x80011800A8400910 MEMORY 8
REG_FIELD = OCLA0_DAT290 entry 37 0
REG = OCLA0_DAT291 0x80011800A8400918 MEMORY 8
REG_FIELD = OCLA0_DAT291 entry 37 0
REG = OCLA0_DAT292 0x80011800A8400920 MEMORY 8
REG_FIELD = OCLA0_DAT292 entry 37 0
REG = OCLA0_DAT293 0x80011800A8400928 MEMORY 8
REG_FIELD = OCLA0_DAT293 entry 37 0
REG = OCLA0_DAT294 0x80011800A8400930 MEMORY 8
REG_FIELD = OCLA0_DAT294 entry 37 0
REG = OCLA0_DAT295 0x80011800A8400938 MEMORY 8
REG_FIELD = OCLA0_DAT295 entry 37 0
REG = OCLA0_DAT296 0x80011800A8400940 MEMORY 8
REG_FIELD = OCLA0_DAT296 entry 37 0
REG = OCLA0_DAT297 0x80011800A8400948 MEMORY 8
REG_FIELD = OCLA0_DAT297 entry 37 0
REG = OCLA0_DAT298 0x80011800A8400950 MEMORY 8
REG_FIELD = OCLA0_DAT298 entry 37 0
REG = OCLA0_DAT299 0x80011800A8400958 MEMORY 8
REG_FIELD = OCLA0_DAT299 entry 37 0
REG = OCLA0_DAT300 0x80011800A8400960 MEMORY 8
REG_FIELD = OCLA0_DAT300 entry 37 0
REG = OCLA0_DAT301 0x80011800A8400968 MEMORY 8
REG_FIELD = OCLA0_DAT301 entry 37 0
REG = OCLA0_DAT302 0x80011800A8400970 MEMORY 8
REG_FIELD = OCLA0_DAT302 entry 37 0
REG = OCLA0_DAT303 0x80011800A8400978 MEMORY 8
REG_FIELD = OCLA0_DAT303 entry 37 0
REG = OCLA0_DAT304 0x80011800A8400980 MEMORY 8
REG_FIELD = OCLA0_DAT304 entry 37 0
REG = OCLA0_DAT305 0x80011800A8400988 MEMORY 8
REG_FIELD = OCLA0_DAT305 entry 37 0
REG = OCLA0_DAT306 0x80011800A8400990 MEMORY 8
REG_FIELD = OCLA0_DAT306 entry 37 0
REG = OCLA0_DAT307 0x80011800A8400998 MEMORY 8
REG_FIELD = OCLA0_DAT307 entry 37 0
REG = OCLA0_DAT308 0x80011800A84009A0 MEMORY 8
REG_FIELD = OCLA0_DAT308 entry 37 0
REG = OCLA0_DAT309 0x80011800A84009A8 MEMORY 8
REG_FIELD = OCLA0_DAT309 entry 37 0
REG = OCLA0_DAT310 0x80011800A84009B0 MEMORY 8
REG_FIELD = OCLA0_DAT310 entry 37 0
REG = OCLA0_DAT311 0x80011800A84009B8 MEMORY 8
REG_FIELD = OCLA0_DAT311 entry 37 0
REG = OCLA0_DAT312 0x80011800A84009C0 MEMORY 8
REG_FIELD = OCLA0_DAT312 entry 37 0
REG = OCLA0_DAT313 0x80011800A84009C8 MEMORY 8
REG_FIELD = OCLA0_DAT313 entry 37 0
REG = OCLA0_DAT314 0x80011800A84009D0 MEMORY 8
REG_FIELD = OCLA0_DAT314 entry 37 0
REG = OCLA0_DAT315 0x80011800A84009D8 MEMORY 8
REG_FIELD = OCLA0_DAT315 entry 37 0
REG = OCLA0_DAT316 0x80011800A84009E0 MEMORY 8
REG_FIELD = OCLA0_DAT316 entry 37 0
REG = OCLA0_DAT317 0x80011800A84009E8 MEMORY 8
REG_FIELD = OCLA0_DAT317 entry 37 0
REG = OCLA0_DAT318 0x80011800A84009F0 MEMORY 8
REG_FIELD = OCLA0_DAT318 entry 37 0
REG = OCLA0_DAT319 0x80011800A84009F8 MEMORY 8
REG_FIELD = OCLA0_DAT319 entry 37 0
REG = OCLA0_DAT320 0x80011800A8400A00 MEMORY 8
REG_FIELD = OCLA0_DAT320 entry 37 0
REG = OCLA0_DAT321 0x80011800A8400A08 MEMORY 8
REG_FIELD = OCLA0_DAT321 entry 37 0
REG = OCLA0_DAT322 0x80011800A8400A10 MEMORY 8
REG_FIELD = OCLA0_DAT322 entry 37 0
REG = OCLA0_DAT323 0x80011800A8400A18 MEMORY 8
REG_FIELD = OCLA0_DAT323 entry 37 0
REG = OCLA0_DAT324 0x80011800A8400A20 MEMORY 8
REG_FIELD = OCLA0_DAT324 entry 37 0
REG = OCLA0_DAT325 0x80011800A8400A28 MEMORY 8
REG_FIELD = OCLA0_DAT325 entry 37 0
REG = OCLA0_DAT326 0x80011800A8400A30 MEMORY 8
REG_FIELD = OCLA0_DAT326 entry 37 0
REG = OCLA0_DAT327 0x80011800A8400A38 MEMORY 8
REG_FIELD = OCLA0_DAT327 entry 37 0
REG = OCLA0_DAT328 0x80011800A8400A40 MEMORY 8
REG_FIELD = OCLA0_DAT328 entry 37 0
REG = OCLA0_DAT329 0x80011800A8400A48 MEMORY 8
REG_FIELD = OCLA0_DAT329 entry 37 0
REG = OCLA0_DAT330 0x80011800A8400A50 MEMORY 8
REG_FIELD = OCLA0_DAT330 entry 37 0
REG = OCLA0_DAT331 0x80011800A8400A58 MEMORY 8
REG_FIELD = OCLA0_DAT331 entry 37 0
REG = OCLA0_DAT332 0x80011800A8400A60 MEMORY 8
REG_FIELD = OCLA0_DAT332 entry 37 0
REG = OCLA0_DAT333 0x80011800A8400A68 MEMORY 8
REG_FIELD = OCLA0_DAT333 entry 37 0
REG = OCLA0_DAT334 0x80011800A8400A70 MEMORY 8
REG_FIELD = OCLA0_DAT334 entry 37 0
REG = OCLA0_DAT335 0x80011800A8400A78 MEMORY 8
REG_FIELD = OCLA0_DAT335 entry 37 0
REG = OCLA0_DAT336 0x80011800A8400A80 MEMORY 8
REG_FIELD = OCLA0_DAT336 entry 37 0
REG = OCLA0_DAT337 0x80011800A8400A88 MEMORY 8
REG_FIELD = OCLA0_DAT337 entry 37 0
REG = OCLA0_DAT338 0x80011800A8400A90 MEMORY 8
REG_FIELD = OCLA0_DAT338 entry 37 0
REG = OCLA0_DAT339 0x80011800A8400A98 MEMORY 8
REG_FIELD = OCLA0_DAT339 entry 37 0
REG = OCLA0_DAT340 0x80011800A8400AA0 MEMORY 8
REG_FIELD = OCLA0_DAT340 entry 37 0
REG = OCLA0_DAT341 0x80011800A8400AA8 MEMORY 8
REG_FIELD = OCLA0_DAT341 entry 37 0
REG = OCLA0_DAT342 0x80011800A8400AB0 MEMORY 8
REG_FIELD = OCLA0_DAT342 entry 37 0
REG = OCLA0_DAT343 0x80011800A8400AB8 MEMORY 8
REG_FIELD = OCLA0_DAT343 entry 37 0
REG = OCLA0_DAT344 0x80011800A8400AC0 MEMORY 8
REG_FIELD = OCLA0_DAT344 entry 37 0
REG = OCLA0_DAT345 0x80011800A8400AC8 MEMORY 8
REG_FIELD = OCLA0_DAT345 entry 37 0
REG = OCLA0_DAT346 0x80011800A8400AD0 MEMORY 8
REG_FIELD = OCLA0_DAT346 entry 37 0
REG = OCLA0_DAT347 0x80011800A8400AD8 MEMORY 8
REG_FIELD = OCLA0_DAT347 entry 37 0
REG = OCLA0_DAT348 0x80011800A8400AE0 MEMORY 8
REG_FIELD = OCLA0_DAT348 entry 37 0
REG = OCLA0_DAT349 0x80011800A8400AE8 MEMORY 8
REG_FIELD = OCLA0_DAT349 entry 37 0
REG = OCLA0_DAT350 0x80011800A8400AF0 MEMORY 8
REG_FIELD = OCLA0_DAT350 entry 37 0
REG = OCLA0_DAT351 0x80011800A8400AF8 MEMORY 8
REG_FIELD = OCLA0_DAT351 entry 37 0
REG = OCLA0_DAT352 0x80011800A8400B00 MEMORY 8
REG_FIELD = OCLA0_DAT352 entry 37 0
REG = OCLA0_DAT353 0x80011800A8400B08 MEMORY 8
REG_FIELD = OCLA0_DAT353 entry 37 0
REG = OCLA0_DAT354 0x80011800A8400B10 MEMORY 8
REG_FIELD = OCLA0_DAT354 entry 37 0
REG = OCLA0_DAT355 0x80011800A8400B18 MEMORY 8
REG_FIELD = OCLA0_DAT355 entry 37 0
REG = OCLA0_DAT356 0x80011800A8400B20 MEMORY 8
REG_FIELD = OCLA0_DAT356 entry 37 0
REG = OCLA0_DAT357 0x80011800A8400B28 MEMORY 8
REG_FIELD = OCLA0_DAT357 entry 37 0
REG = OCLA0_DAT358 0x80011800A8400B30 MEMORY 8
REG_FIELD = OCLA0_DAT358 entry 37 0
REG = OCLA0_DAT359 0x80011800A8400B38 MEMORY 8
REG_FIELD = OCLA0_DAT359 entry 37 0
REG = OCLA0_DAT360 0x80011800A8400B40 MEMORY 8
REG_FIELD = OCLA0_DAT360 entry 37 0
REG = OCLA0_DAT361 0x80011800A8400B48 MEMORY 8
REG_FIELD = OCLA0_DAT361 entry 37 0
REG = OCLA0_DAT362 0x80011800A8400B50 MEMORY 8
REG_FIELD = OCLA0_DAT362 entry 37 0
REG = OCLA0_DAT363 0x80011800A8400B58 MEMORY 8
REG_FIELD = OCLA0_DAT363 entry 37 0
REG = OCLA0_DAT364 0x80011800A8400B60 MEMORY 8
REG_FIELD = OCLA0_DAT364 entry 37 0
REG = OCLA0_DAT365 0x80011800A8400B68 MEMORY 8
REG_FIELD = OCLA0_DAT365 entry 37 0
REG = OCLA0_DAT366 0x80011800A8400B70 MEMORY 8
REG_FIELD = OCLA0_DAT366 entry 37 0
REG = OCLA0_DAT367 0x80011800A8400B78 MEMORY 8
REG_FIELD = OCLA0_DAT367 entry 37 0
REG = OCLA0_DAT368 0x80011800A8400B80 MEMORY 8
REG_FIELD = OCLA0_DAT368 entry 37 0
REG = OCLA0_DAT369 0x80011800A8400B88 MEMORY 8
REG_FIELD = OCLA0_DAT369 entry 37 0
REG = OCLA0_DAT370 0x80011800A8400B90 MEMORY 8
REG_FIELD = OCLA0_DAT370 entry 37 0
REG = OCLA0_DAT371 0x80011800A8400B98 MEMORY 8
REG_FIELD = OCLA0_DAT371 entry 37 0
REG = OCLA0_DAT372 0x80011800A8400BA0 MEMORY 8
REG_FIELD = OCLA0_DAT372 entry 37 0
REG = OCLA0_DAT373 0x80011800A8400BA8 MEMORY 8
REG_FIELD = OCLA0_DAT373 entry 37 0
REG = OCLA0_DAT374 0x80011800A8400BB0 MEMORY 8
REG_FIELD = OCLA0_DAT374 entry 37 0
REG = OCLA0_DAT375 0x80011800A8400BB8 MEMORY 8
REG_FIELD = OCLA0_DAT375 entry 37 0
REG = OCLA0_DAT376 0x80011800A8400BC0 MEMORY 8
REG_FIELD = OCLA0_DAT376 entry 37 0
REG = OCLA0_DAT377 0x80011800A8400BC8 MEMORY 8
REG_FIELD = OCLA0_DAT377 entry 37 0
REG = OCLA0_DAT378 0x80011800A8400BD0 MEMORY 8
REG_FIELD = OCLA0_DAT378 entry 37 0
REG = OCLA0_DAT379 0x80011800A8400BD8 MEMORY 8
REG_FIELD = OCLA0_DAT379 entry 37 0
REG = OCLA0_DAT380 0x80011800A8400BE0 MEMORY 8
REG_FIELD = OCLA0_DAT380 entry 37 0
REG = OCLA0_DAT381 0x80011800A8400BE8 MEMORY 8
REG_FIELD = OCLA0_DAT381 entry 37 0
REG = OCLA0_DAT382 0x80011800A8400BF0 MEMORY 8
REG_FIELD = OCLA0_DAT382 entry 37 0
REG = OCLA0_DAT383 0x80011800A8400BF8 MEMORY 8
REG_FIELD = OCLA0_DAT383 entry 37 0
REG = OCLA0_DAT384 0x80011800A8400C00 MEMORY 8
REG_FIELD = OCLA0_DAT384 entry 37 0
REG = OCLA0_DAT385 0x80011800A8400C08 MEMORY 8
REG_FIELD = OCLA0_DAT385 entry 37 0
REG = OCLA0_DAT386 0x80011800A8400C10 MEMORY 8
REG_FIELD = OCLA0_DAT386 entry 37 0
REG = OCLA0_DAT387 0x80011800A8400C18 MEMORY 8
REG_FIELD = OCLA0_DAT387 entry 37 0
REG = OCLA0_DAT388 0x80011800A8400C20 MEMORY 8
REG_FIELD = OCLA0_DAT388 entry 37 0
REG = OCLA0_DAT389 0x80011800A8400C28 MEMORY 8
REG_FIELD = OCLA0_DAT389 entry 37 0
REG = OCLA0_DAT390 0x80011800A8400C30 MEMORY 8
REG_FIELD = OCLA0_DAT390 entry 37 0
REG = OCLA0_DAT391 0x80011800A8400C38 MEMORY 8
REG_FIELD = OCLA0_DAT391 entry 37 0
REG = OCLA0_DAT392 0x80011800A8400C40 MEMORY 8
REG_FIELD = OCLA0_DAT392 entry 37 0
REG = OCLA0_DAT393 0x80011800A8400C48 MEMORY 8
REG_FIELD = OCLA0_DAT393 entry 37 0
REG = OCLA0_DAT394 0x80011800A8400C50 MEMORY 8
REG_FIELD = OCLA0_DAT394 entry 37 0
REG = OCLA0_DAT395 0x80011800A8400C58 MEMORY 8
REG_FIELD = OCLA0_DAT395 entry 37 0
REG = OCLA0_DAT396 0x80011800A8400C60 MEMORY 8
REG_FIELD = OCLA0_DAT396 entry 37 0
REG = OCLA0_DAT397 0x80011800A8400C68 MEMORY 8
REG_FIELD = OCLA0_DAT397 entry 37 0
REG = OCLA0_DAT398 0x80011800A8400C70 MEMORY 8
REG_FIELD = OCLA0_DAT398 entry 37 0
REG = OCLA0_DAT399 0x80011800A8400C78 MEMORY 8
REG_FIELD = OCLA0_DAT399 entry 37 0
REG = OCLA0_DAT400 0x80011800A8400C80 MEMORY 8
REG_FIELD = OCLA0_DAT400 entry 37 0
REG = OCLA0_DAT401 0x80011800A8400C88 MEMORY 8
REG_FIELD = OCLA0_DAT401 entry 37 0
REG = OCLA0_DAT402 0x80011800A8400C90 MEMORY 8
REG_FIELD = OCLA0_DAT402 entry 37 0
REG = OCLA0_DAT403 0x80011800A8400C98 MEMORY 8
REG_FIELD = OCLA0_DAT403 entry 37 0
REG = OCLA0_DAT404 0x80011800A8400CA0 MEMORY 8
REG_FIELD = OCLA0_DAT404 entry 37 0
REG = OCLA0_DAT405 0x80011800A8400CA8 MEMORY 8
REG_FIELD = OCLA0_DAT405 entry 37 0
REG = OCLA0_DAT406 0x80011800A8400CB0 MEMORY 8
REG_FIELD = OCLA0_DAT406 entry 37 0
REG = OCLA0_DAT407 0x80011800A8400CB8 MEMORY 8
REG_FIELD = OCLA0_DAT407 entry 37 0
REG = OCLA0_DAT408 0x80011800A8400CC0 MEMORY 8
REG_FIELD = OCLA0_DAT408 entry 37 0
REG = OCLA0_DAT409 0x80011800A8400CC8 MEMORY 8
REG_FIELD = OCLA0_DAT409 entry 37 0
REG = OCLA0_DAT410 0x80011800A8400CD0 MEMORY 8
REG_FIELD = OCLA0_DAT410 entry 37 0
REG = OCLA0_DAT411 0x80011800A8400CD8 MEMORY 8
REG_FIELD = OCLA0_DAT411 entry 37 0
REG = OCLA0_DAT412 0x80011800A8400CE0 MEMORY 8
REG_FIELD = OCLA0_DAT412 entry 37 0
REG = OCLA0_DAT413 0x80011800A8400CE8 MEMORY 8
REG_FIELD = OCLA0_DAT413 entry 37 0
REG = OCLA0_DAT414 0x80011800A8400CF0 MEMORY 8
REG_FIELD = OCLA0_DAT414 entry 37 0
REG = OCLA0_DAT415 0x80011800A8400CF8 MEMORY 8
REG_FIELD = OCLA0_DAT415 entry 37 0
REG = OCLA0_DAT416 0x80011800A8400D00 MEMORY 8
REG_FIELD = OCLA0_DAT416 entry 37 0
REG = OCLA0_DAT417 0x80011800A8400D08 MEMORY 8
REG_FIELD = OCLA0_DAT417 entry 37 0
REG = OCLA0_DAT418 0x80011800A8400D10 MEMORY 8
REG_FIELD = OCLA0_DAT418 entry 37 0
REG = OCLA0_DAT419 0x80011800A8400D18 MEMORY 8
REG_FIELD = OCLA0_DAT419 entry 37 0
REG = OCLA0_DAT420 0x80011800A8400D20 MEMORY 8
REG_FIELD = OCLA0_DAT420 entry 37 0
REG = OCLA0_DAT421 0x80011800A8400D28 MEMORY 8
REG_FIELD = OCLA0_DAT421 entry 37 0
REG = OCLA0_DAT422 0x80011800A8400D30 MEMORY 8
REG_FIELD = OCLA0_DAT422 entry 37 0
REG = OCLA0_DAT423 0x80011800A8400D38 MEMORY 8
REG_FIELD = OCLA0_DAT423 entry 37 0
REG = OCLA0_DAT424 0x80011800A8400D40 MEMORY 8
REG_FIELD = OCLA0_DAT424 entry 37 0
REG = OCLA0_DAT425 0x80011800A8400D48 MEMORY 8
REG_FIELD = OCLA0_DAT425 entry 37 0
REG = OCLA0_DAT426 0x80011800A8400D50 MEMORY 8
REG_FIELD = OCLA0_DAT426 entry 37 0
REG = OCLA0_DAT427 0x80011800A8400D58 MEMORY 8
REG_FIELD = OCLA0_DAT427 entry 37 0
REG = OCLA0_DAT428 0x80011800A8400D60 MEMORY 8
REG_FIELD = OCLA0_DAT428 entry 37 0
REG = OCLA0_DAT429 0x80011800A8400D68 MEMORY 8
REG_FIELD = OCLA0_DAT429 entry 37 0
REG = OCLA0_DAT430 0x80011800A8400D70 MEMORY 8
REG_FIELD = OCLA0_DAT430 entry 37 0
REG = OCLA0_DAT431 0x80011800A8400D78 MEMORY 8
REG_FIELD = OCLA0_DAT431 entry 37 0
REG = OCLA0_DAT432 0x80011800A8400D80 MEMORY 8
REG_FIELD = OCLA0_DAT432 entry 37 0
REG = OCLA0_DAT433 0x80011800A8400D88 MEMORY 8
REG_FIELD = OCLA0_DAT433 entry 37 0
REG = OCLA0_DAT434 0x80011800A8400D90 MEMORY 8
REG_FIELD = OCLA0_DAT434 entry 37 0
REG = OCLA0_DAT435 0x80011800A8400D98 MEMORY 8
REG_FIELD = OCLA0_DAT435 entry 37 0
REG = OCLA0_DAT436 0x80011800A8400DA0 MEMORY 8
REG_FIELD = OCLA0_DAT436 entry 37 0
REG = OCLA0_DAT437 0x80011800A8400DA8 MEMORY 8
REG_FIELD = OCLA0_DAT437 entry 37 0
REG = OCLA0_DAT438 0x80011800A8400DB0 MEMORY 8
REG_FIELD = OCLA0_DAT438 entry 37 0
REG = OCLA0_DAT439 0x80011800A8400DB8 MEMORY 8
REG_FIELD = OCLA0_DAT439 entry 37 0
REG = OCLA0_DAT440 0x80011800A8400DC0 MEMORY 8
REG_FIELD = OCLA0_DAT440 entry 37 0
REG = OCLA0_DAT441 0x80011800A8400DC8 MEMORY 8
REG_FIELD = OCLA0_DAT441 entry 37 0
REG = OCLA0_DAT442 0x80011800A8400DD0 MEMORY 8
REG_FIELD = OCLA0_DAT442 entry 37 0
REG = OCLA0_DAT443 0x80011800A8400DD8 MEMORY 8
REG_FIELD = OCLA0_DAT443 entry 37 0
REG = OCLA0_DAT444 0x80011800A8400DE0 MEMORY 8
REG_FIELD = OCLA0_DAT444 entry 37 0
REG = OCLA0_DAT445 0x80011800A8400DE8 MEMORY 8
REG_FIELD = OCLA0_DAT445 entry 37 0
REG = OCLA0_DAT446 0x80011800A8400DF0 MEMORY 8
REG_FIELD = OCLA0_DAT446 entry 37 0
REG = OCLA0_DAT447 0x80011800A8400DF8 MEMORY 8
REG_FIELD = OCLA0_DAT447 entry 37 0
REG = OCLA0_DAT448 0x80011800A8400E00 MEMORY 8
REG_FIELD = OCLA0_DAT448 entry 37 0
REG = OCLA0_DAT449 0x80011800A8400E08 MEMORY 8
REG_FIELD = OCLA0_DAT449 entry 37 0
REG = OCLA0_DAT450 0x80011800A8400E10 MEMORY 8
REG_FIELD = OCLA0_DAT450 entry 37 0
REG = OCLA0_DAT451 0x80011800A8400E18 MEMORY 8
REG_FIELD = OCLA0_DAT451 entry 37 0
REG = OCLA0_DAT452 0x80011800A8400E20 MEMORY 8
REG_FIELD = OCLA0_DAT452 entry 37 0
REG = OCLA0_DAT453 0x80011800A8400E28 MEMORY 8
REG_FIELD = OCLA0_DAT453 entry 37 0
REG = OCLA0_DAT454 0x80011800A8400E30 MEMORY 8
REG_FIELD = OCLA0_DAT454 entry 37 0
REG = OCLA0_DAT455 0x80011800A8400E38 MEMORY 8
REG_FIELD = OCLA0_DAT455 entry 37 0
REG = OCLA0_DAT456 0x80011800A8400E40 MEMORY 8
REG_FIELD = OCLA0_DAT456 entry 37 0
REG = OCLA0_DAT457 0x80011800A8400E48 MEMORY 8
REG_FIELD = OCLA0_DAT457 entry 37 0
REG = OCLA0_DAT458 0x80011800A8400E50 MEMORY 8
REG_FIELD = OCLA0_DAT458 entry 37 0
REG = OCLA0_DAT459 0x80011800A8400E58 MEMORY 8
REG_FIELD = OCLA0_DAT459 entry 37 0
REG = OCLA0_DAT460 0x80011800A8400E60 MEMORY 8
REG_FIELD = OCLA0_DAT460 entry 37 0
REG = OCLA0_DAT461 0x80011800A8400E68 MEMORY 8
REG_FIELD = OCLA0_DAT461 entry 37 0
REG = OCLA0_DAT462 0x80011800A8400E70 MEMORY 8
REG_FIELD = OCLA0_DAT462 entry 37 0
REG = OCLA0_DAT463 0x80011800A8400E78 MEMORY 8
REG_FIELD = OCLA0_DAT463 entry 37 0
REG = OCLA0_DAT464 0x80011800A8400E80 MEMORY 8
REG_FIELD = OCLA0_DAT464 entry 37 0
REG = OCLA0_DAT465 0x80011800A8400E88 MEMORY 8
REG_FIELD = OCLA0_DAT465 entry 37 0
REG = OCLA0_DAT466 0x80011800A8400E90 MEMORY 8
REG_FIELD = OCLA0_DAT466 entry 37 0
REG = OCLA0_DAT467 0x80011800A8400E98 MEMORY 8
REG_FIELD = OCLA0_DAT467 entry 37 0
REG = OCLA0_DAT468 0x80011800A8400EA0 MEMORY 8
REG_FIELD = OCLA0_DAT468 entry 37 0
REG = OCLA0_DAT469 0x80011800A8400EA8 MEMORY 8
REG_FIELD = OCLA0_DAT469 entry 37 0
REG = OCLA0_DAT470 0x80011800A8400EB0 MEMORY 8
REG_FIELD = OCLA0_DAT470 entry 37 0
REG = OCLA0_DAT471 0x80011800A8400EB8 MEMORY 8
REG_FIELD = OCLA0_DAT471 entry 37 0
REG = OCLA0_DAT472 0x80011800A8400EC0 MEMORY 8
REG_FIELD = OCLA0_DAT472 entry 37 0
REG = OCLA0_DAT473 0x80011800A8400EC8 MEMORY 8
REG_FIELD = OCLA0_DAT473 entry 37 0
REG = OCLA0_DAT474 0x80011800A8400ED0 MEMORY 8
REG_FIELD = OCLA0_DAT474 entry 37 0
REG = OCLA0_DAT475 0x80011800A8400ED8 MEMORY 8
REG_FIELD = OCLA0_DAT475 entry 37 0
REG = OCLA0_DAT476 0x80011800A8400EE0 MEMORY 8
REG_FIELD = OCLA0_DAT476 entry 37 0
REG = OCLA0_DAT477 0x80011800A8400EE8 MEMORY 8
REG_FIELD = OCLA0_DAT477 entry 37 0
REG = OCLA0_DAT478 0x80011800A8400EF0 MEMORY 8
REG_FIELD = OCLA0_DAT478 entry 37 0
REG = OCLA0_DAT479 0x80011800A8400EF8 MEMORY 8
REG_FIELD = OCLA0_DAT479 entry 37 0
REG = OCLA0_DAT480 0x80011800A8400F00 MEMORY 8
REG_FIELD = OCLA0_DAT480 entry 37 0
REG = OCLA0_DAT481 0x80011800A8400F08 MEMORY 8
REG_FIELD = OCLA0_DAT481 entry 37 0
REG = OCLA0_DAT482 0x80011800A8400F10 MEMORY 8
REG_FIELD = OCLA0_DAT482 entry 37 0
REG = OCLA0_DAT483 0x80011800A8400F18 MEMORY 8
REG_FIELD = OCLA0_DAT483 entry 37 0
REG = OCLA0_DAT484 0x80011800A8400F20 MEMORY 8
REG_FIELD = OCLA0_DAT484 entry 37 0
REG = OCLA0_DAT485 0x80011800A8400F28 MEMORY 8
REG_FIELD = OCLA0_DAT485 entry 37 0
REG = OCLA0_DAT486 0x80011800A8400F30 MEMORY 8
REG_FIELD = OCLA0_DAT486 entry 37 0
REG = OCLA0_DAT487 0x80011800A8400F38 MEMORY 8
REG_FIELD = OCLA0_DAT487 entry 37 0
REG = OCLA0_DAT488 0x80011800A8400F40 MEMORY 8
REG_FIELD = OCLA0_DAT488 entry 37 0
REG = OCLA0_DAT489 0x80011800A8400F48 MEMORY 8
REG_FIELD = OCLA0_DAT489 entry 37 0
REG = OCLA0_DAT490 0x80011800A8400F50 MEMORY 8
REG_FIELD = OCLA0_DAT490 entry 37 0
REG = OCLA0_DAT491 0x80011800A8400F58 MEMORY 8
REG_FIELD = OCLA0_DAT491 entry 37 0
REG = OCLA0_DAT492 0x80011800A8400F60 MEMORY 8
REG_FIELD = OCLA0_DAT492 entry 37 0
REG = OCLA0_DAT493 0x80011800A8400F68 MEMORY 8
REG_FIELD = OCLA0_DAT493 entry 37 0
REG = OCLA0_DAT494 0x80011800A8400F70 MEMORY 8
REG_FIELD = OCLA0_DAT494 entry 37 0
REG = OCLA0_DAT495 0x80011800A8400F78 MEMORY 8
REG_FIELD = OCLA0_DAT495 entry 37 0
REG = OCLA0_DAT496 0x80011800A8400F80 MEMORY 8
REG_FIELD = OCLA0_DAT496 entry 37 0
REG = OCLA0_DAT497 0x80011800A8400F88 MEMORY 8
REG_FIELD = OCLA0_DAT497 entry 37 0
REG = OCLA0_DAT498 0x80011800A8400F90 MEMORY 8
REG_FIELD = OCLA0_DAT498 entry 37 0
REG = OCLA0_DAT499 0x80011800A8400F98 MEMORY 8
REG_FIELD = OCLA0_DAT499 entry 37 0
REG = OCLA0_DAT500 0x80011800A8400FA0 MEMORY 8
REG_FIELD = OCLA0_DAT500 entry 37 0
REG = OCLA0_DAT501 0x80011800A8400FA8 MEMORY 8
REG_FIELD = OCLA0_DAT501 entry 37 0
REG = OCLA0_DAT502 0x80011800A8400FB0 MEMORY 8
REG_FIELD = OCLA0_DAT502 entry 37 0
REG = OCLA0_DAT503 0x80011800A8400FB8 MEMORY 8
REG_FIELD = OCLA0_DAT503 entry 37 0
REG = OCLA0_DAT504 0x80011800A8400FC0 MEMORY 8
REG_FIELD = OCLA0_DAT504 entry 37 0
REG = OCLA0_DAT505 0x80011800A8400FC8 MEMORY 8
REG_FIELD = OCLA0_DAT505 entry 37 0
REG = OCLA0_DAT506 0x80011800A8400FD0 MEMORY 8
REG_FIELD = OCLA0_DAT506 entry 37 0
REG = OCLA0_DAT507 0x80011800A8400FD8 MEMORY 8
REG_FIELD = OCLA0_DAT507 entry 37 0
REG = OCLA0_DAT508 0x80011800A8400FE0 MEMORY 8
REG_FIELD = OCLA0_DAT508 entry 37 0
REG = OCLA0_DAT509 0x80011800A8400FE8 MEMORY 8
REG_FIELD = OCLA0_DAT509 entry 37 0
REG = OCLA0_DAT510 0x80011800A8400FF0 MEMORY 8
REG_FIELD = OCLA0_DAT510 entry 37 0
REG = OCLA0_DAT511 0x80011800A8400FF8 MEMORY 8
REG_FIELD = OCLA0_DAT511 entry 37 0
REG = OCLA0_DAT512 0x80011800A8401000 MEMORY 8
REG_FIELD = OCLA0_DAT512 entry 37 0
REG = OCLA0_DAT513 0x80011800A8401008 MEMORY 8
REG_FIELD = OCLA0_DAT513 entry 37 0
REG = OCLA0_DAT514 0x80011800A8401010 MEMORY 8
REG_FIELD = OCLA0_DAT514 entry 37 0
REG = OCLA0_DAT515 0x80011800A8401018 MEMORY 8
REG_FIELD = OCLA0_DAT515 entry 37 0
REG = OCLA0_DAT516 0x80011800A8401020 MEMORY 8
REG_FIELD = OCLA0_DAT516 entry 37 0
REG = OCLA0_DAT517 0x80011800A8401028 MEMORY 8
REG_FIELD = OCLA0_DAT517 entry 37 0
REG = OCLA0_DAT518 0x80011800A8401030 MEMORY 8
REG_FIELD = OCLA0_DAT518 entry 37 0
REG = OCLA0_DAT519 0x80011800A8401038 MEMORY 8
REG_FIELD = OCLA0_DAT519 entry 37 0
REG = OCLA0_DAT520 0x80011800A8401040 MEMORY 8
REG_FIELD = OCLA0_DAT520 entry 37 0
REG = OCLA0_DAT521 0x80011800A8401048 MEMORY 8
REG_FIELD = OCLA0_DAT521 entry 37 0
REG = OCLA0_DAT522 0x80011800A8401050 MEMORY 8
REG_FIELD = OCLA0_DAT522 entry 37 0
REG = OCLA0_DAT523 0x80011800A8401058 MEMORY 8
REG_FIELD = OCLA0_DAT523 entry 37 0
REG = OCLA0_DAT524 0x80011800A8401060 MEMORY 8
REG_FIELD = OCLA0_DAT524 entry 37 0
REG = OCLA0_DAT525 0x80011800A8401068 MEMORY 8
REG_FIELD = OCLA0_DAT525 entry 37 0
REG = OCLA0_DAT526 0x80011800A8401070 MEMORY 8
REG_FIELD = OCLA0_DAT526 entry 37 0
REG = OCLA0_DAT527 0x80011800A8401078 MEMORY 8
REG_FIELD = OCLA0_DAT527 entry 37 0
REG = OCLA0_DAT528 0x80011800A8401080 MEMORY 8
REG_FIELD = OCLA0_DAT528 entry 37 0
REG = OCLA0_DAT529 0x80011800A8401088 MEMORY 8
REG_FIELD = OCLA0_DAT529 entry 37 0
REG = OCLA0_DAT530 0x80011800A8401090 MEMORY 8
REG_FIELD = OCLA0_DAT530 entry 37 0
REG = OCLA0_DAT531 0x80011800A8401098 MEMORY 8
REG_FIELD = OCLA0_DAT531 entry 37 0
REG = OCLA0_DAT532 0x80011800A84010A0 MEMORY 8
REG_FIELD = OCLA0_DAT532 entry 37 0
REG = OCLA0_DAT533 0x80011800A84010A8 MEMORY 8
REG_FIELD = OCLA0_DAT533 entry 37 0
REG = OCLA0_DAT534 0x80011800A84010B0 MEMORY 8
REG_FIELD = OCLA0_DAT534 entry 37 0
REG = OCLA0_DAT535 0x80011800A84010B8 MEMORY 8
REG_FIELD = OCLA0_DAT535 entry 37 0
REG = OCLA0_DAT536 0x80011800A84010C0 MEMORY 8
REG_FIELD = OCLA0_DAT536 entry 37 0
REG = OCLA0_DAT537 0x80011800A84010C8 MEMORY 8
REG_FIELD = OCLA0_DAT537 entry 37 0
REG = OCLA0_DAT538 0x80011800A84010D0 MEMORY 8
REG_FIELD = OCLA0_DAT538 entry 37 0
REG = OCLA0_DAT539 0x80011800A84010D8 MEMORY 8
REG_FIELD = OCLA0_DAT539 entry 37 0
REG = OCLA0_DAT540 0x80011800A84010E0 MEMORY 8
REG_FIELD = OCLA0_DAT540 entry 37 0
REG = OCLA0_DAT541 0x80011800A84010E8 MEMORY 8
REG_FIELD = OCLA0_DAT541 entry 37 0
REG = OCLA0_DAT542 0x80011800A84010F0 MEMORY 8
REG_FIELD = OCLA0_DAT542 entry 37 0
REG = OCLA0_DAT543 0x80011800A84010F8 MEMORY 8
REG_FIELD = OCLA0_DAT543 entry 37 0
REG = OCLA0_DAT544 0x80011800A8401100 MEMORY 8
REG_FIELD = OCLA0_DAT544 entry 37 0
REG = OCLA0_DAT545 0x80011800A8401108 MEMORY 8
REG_FIELD = OCLA0_DAT545 entry 37 0
REG = OCLA0_DAT546 0x80011800A8401110 MEMORY 8
REG_FIELD = OCLA0_DAT546 entry 37 0
REG = OCLA0_DAT547 0x80011800A8401118 MEMORY 8
REG_FIELD = OCLA0_DAT547 entry 37 0
REG = OCLA0_DAT548 0x80011800A8401120 MEMORY 8
REG_FIELD = OCLA0_DAT548 entry 37 0
REG = OCLA0_DAT549 0x80011800A8401128 MEMORY 8
REG_FIELD = OCLA0_DAT549 entry 37 0
REG = OCLA0_DAT550 0x80011800A8401130 MEMORY 8
REG_FIELD = OCLA0_DAT550 entry 37 0
REG = OCLA0_DAT551 0x80011800A8401138 MEMORY 8
REG_FIELD = OCLA0_DAT551 entry 37 0
REG = OCLA0_DAT552 0x80011800A8401140 MEMORY 8
REG_FIELD = OCLA0_DAT552 entry 37 0
REG = OCLA0_DAT553 0x80011800A8401148 MEMORY 8
REG_FIELD = OCLA0_DAT553 entry 37 0
REG = OCLA0_DAT554 0x80011800A8401150 MEMORY 8
REG_FIELD = OCLA0_DAT554 entry 37 0
REG = OCLA0_DAT555 0x80011800A8401158 MEMORY 8
REG_FIELD = OCLA0_DAT555 entry 37 0
REG = OCLA0_DAT556 0x80011800A8401160 MEMORY 8
REG_FIELD = OCLA0_DAT556 entry 37 0
REG = OCLA0_DAT557 0x80011800A8401168 MEMORY 8
REG_FIELD = OCLA0_DAT557 entry 37 0
REG = OCLA0_DAT558 0x80011800A8401170 MEMORY 8
REG_FIELD = OCLA0_DAT558 entry 37 0
REG = OCLA0_DAT559 0x80011800A8401178 MEMORY 8
REG_FIELD = OCLA0_DAT559 entry 37 0
REG = OCLA0_DAT560 0x80011800A8401180 MEMORY 8
REG_FIELD = OCLA0_DAT560 entry 37 0
REG = OCLA0_DAT561 0x80011800A8401188 MEMORY 8
REG_FIELD = OCLA0_DAT561 entry 37 0
REG = OCLA0_DAT562 0x80011800A8401190 MEMORY 8
REG_FIELD = OCLA0_DAT562 entry 37 0
REG = OCLA0_DAT563 0x80011800A8401198 MEMORY 8
REG_FIELD = OCLA0_DAT563 entry 37 0
REG = OCLA0_DAT564 0x80011800A84011A0 MEMORY 8
REG_FIELD = OCLA0_DAT564 entry 37 0
REG = OCLA0_DAT565 0x80011800A84011A8 MEMORY 8
REG_FIELD = OCLA0_DAT565 entry 37 0
REG = OCLA0_DAT566 0x80011800A84011B0 MEMORY 8
REG_FIELD = OCLA0_DAT566 entry 37 0
REG = OCLA0_DAT567 0x80011800A84011B8 MEMORY 8
REG_FIELD = OCLA0_DAT567 entry 37 0
REG = OCLA0_DAT568 0x80011800A84011C0 MEMORY 8
REG_FIELD = OCLA0_DAT568 entry 37 0
REG = OCLA0_DAT569 0x80011800A84011C8 MEMORY 8
REG_FIELD = OCLA0_DAT569 entry 37 0
REG = OCLA0_DAT570 0x80011800A84011D0 MEMORY 8
REG_FIELD = OCLA0_DAT570 entry 37 0
REG = OCLA0_DAT571 0x80011800A84011D8 MEMORY 8
REG_FIELD = OCLA0_DAT571 entry 37 0
REG = OCLA0_DAT572 0x80011800A84011E0 MEMORY 8
REG_FIELD = OCLA0_DAT572 entry 37 0
REG = OCLA0_DAT573 0x80011800A84011E8 MEMORY 8
REG_FIELD = OCLA0_DAT573 entry 37 0
REG = OCLA0_DAT574 0x80011800A84011F0 MEMORY 8
REG_FIELD = OCLA0_DAT574 entry 37 0
REG = OCLA0_DAT575 0x80011800A84011F8 MEMORY 8
REG_FIELD = OCLA0_DAT575 entry 37 0
REG = OCLA0_DAT576 0x80011800A8401200 MEMORY 8
REG_FIELD = OCLA0_DAT576 entry 37 0
REG = OCLA0_DAT577 0x80011800A8401208 MEMORY 8
REG_FIELD = OCLA0_DAT577 entry 37 0
REG = OCLA0_DAT578 0x80011800A8401210 MEMORY 8
REG_FIELD = OCLA0_DAT578 entry 37 0
REG = OCLA0_DAT579 0x80011800A8401218 MEMORY 8
REG_FIELD = OCLA0_DAT579 entry 37 0
REG = OCLA0_DAT580 0x80011800A8401220 MEMORY 8
REG_FIELD = OCLA0_DAT580 entry 37 0
REG = OCLA0_DAT581 0x80011800A8401228 MEMORY 8
REG_FIELD = OCLA0_DAT581 entry 37 0
REG = OCLA0_DAT582 0x80011800A8401230 MEMORY 8
REG_FIELD = OCLA0_DAT582 entry 37 0
REG = OCLA0_DAT583 0x80011800A8401238 MEMORY 8
REG_FIELD = OCLA0_DAT583 entry 37 0
REG = OCLA0_DAT584 0x80011800A8401240 MEMORY 8
REG_FIELD = OCLA0_DAT584 entry 37 0
REG = OCLA0_DAT585 0x80011800A8401248 MEMORY 8
REG_FIELD = OCLA0_DAT585 entry 37 0
REG = OCLA0_DAT586 0x80011800A8401250 MEMORY 8
REG_FIELD = OCLA0_DAT586 entry 37 0
REG = OCLA0_DAT587 0x80011800A8401258 MEMORY 8
REG_FIELD = OCLA0_DAT587 entry 37 0
REG = OCLA0_DAT588 0x80011800A8401260 MEMORY 8
REG_FIELD = OCLA0_DAT588 entry 37 0
REG = OCLA0_DAT589 0x80011800A8401268 MEMORY 8
REG_FIELD = OCLA0_DAT589 entry 37 0
REG = OCLA0_DAT590 0x80011800A8401270 MEMORY 8
REG_FIELD = OCLA0_DAT590 entry 37 0
REG = OCLA0_DAT591 0x80011800A8401278 MEMORY 8
REG_FIELD = OCLA0_DAT591 entry 37 0
REG = OCLA0_DAT592 0x80011800A8401280 MEMORY 8
REG_FIELD = OCLA0_DAT592 entry 37 0
REG = OCLA0_DAT593 0x80011800A8401288 MEMORY 8
REG_FIELD = OCLA0_DAT593 entry 37 0
REG = OCLA0_DAT594 0x80011800A8401290 MEMORY 8
REG_FIELD = OCLA0_DAT594 entry 37 0
REG = OCLA0_DAT595 0x80011800A8401298 MEMORY 8
REG_FIELD = OCLA0_DAT595 entry 37 0
REG = OCLA0_DAT596 0x80011800A84012A0 MEMORY 8
REG_FIELD = OCLA0_DAT596 entry 37 0
REG = OCLA0_DAT597 0x80011800A84012A8 MEMORY 8
REG_FIELD = OCLA0_DAT597 entry 37 0
REG = OCLA0_DAT598 0x80011800A84012B0 MEMORY 8
REG_FIELD = OCLA0_DAT598 entry 37 0
REG = OCLA0_DAT599 0x80011800A84012B8 MEMORY 8
REG_FIELD = OCLA0_DAT599 entry 37 0
REG = OCLA0_DAT600 0x80011800A84012C0 MEMORY 8
REG_FIELD = OCLA0_DAT600 entry 37 0
REG = OCLA0_DAT601 0x80011800A84012C8 MEMORY 8
REG_FIELD = OCLA0_DAT601 entry 37 0
REG = OCLA0_DAT602 0x80011800A84012D0 MEMORY 8
REG_FIELD = OCLA0_DAT602 entry 37 0
REG = OCLA0_DAT603 0x80011800A84012D8 MEMORY 8
REG_FIELD = OCLA0_DAT603 entry 37 0
REG = OCLA0_DAT604 0x80011800A84012E0 MEMORY 8
REG_FIELD = OCLA0_DAT604 entry 37 0
REG = OCLA0_DAT605 0x80011800A84012E8 MEMORY 8
REG_FIELD = OCLA0_DAT605 entry 37 0
REG = OCLA0_DAT606 0x80011800A84012F0 MEMORY 8
REG_FIELD = OCLA0_DAT606 entry 37 0
REG = OCLA0_DAT607 0x80011800A84012F8 MEMORY 8
REG_FIELD = OCLA0_DAT607 entry 37 0
REG = OCLA0_DAT608 0x80011800A8401300 MEMORY 8
REG_FIELD = OCLA0_DAT608 entry 37 0
REG = OCLA0_DAT609 0x80011800A8401308 MEMORY 8
REG_FIELD = OCLA0_DAT609 entry 37 0
REG = OCLA0_DAT610 0x80011800A8401310 MEMORY 8
REG_FIELD = OCLA0_DAT610 entry 37 0
REG = OCLA0_DAT611 0x80011800A8401318 MEMORY 8
REG_FIELD = OCLA0_DAT611 entry 37 0
REG = OCLA0_DAT612 0x80011800A8401320 MEMORY 8
REG_FIELD = OCLA0_DAT612 entry 37 0
REG = OCLA0_DAT613 0x80011800A8401328 MEMORY 8
REG_FIELD = OCLA0_DAT613 entry 37 0
REG = OCLA0_DAT614 0x80011800A8401330 MEMORY 8
REG_FIELD = OCLA0_DAT614 entry 37 0
REG = OCLA0_DAT615 0x80011800A8401338 MEMORY 8
REG_FIELD = OCLA0_DAT615 entry 37 0
REG = OCLA0_DAT616 0x80011800A8401340 MEMORY 8
REG_FIELD = OCLA0_DAT616 entry 37 0
REG = OCLA0_DAT617 0x80011800A8401348 MEMORY 8
REG_FIELD = OCLA0_DAT617 entry 37 0
REG = OCLA0_DAT618 0x80011800A8401350 MEMORY 8
REG_FIELD = OCLA0_DAT618 entry 37 0
REG = OCLA0_DAT619 0x80011800A8401358 MEMORY 8
REG_FIELD = OCLA0_DAT619 entry 37 0
REG = OCLA0_DAT620 0x80011800A8401360 MEMORY 8
REG_FIELD = OCLA0_DAT620 entry 37 0
REG = OCLA0_DAT621 0x80011800A8401368 MEMORY 8
REG_FIELD = OCLA0_DAT621 entry 37 0
REG = OCLA0_DAT622 0x80011800A8401370 MEMORY 8
REG_FIELD = OCLA0_DAT622 entry 37 0
REG = OCLA0_DAT623 0x80011800A8401378 MEMORY 8
REG_FIELD = OCLA0_DAT623 entry 37 0
REG = OCLA0_DAT624 0x80011800A8401380 MEMORY 8
REG_FIELD = OCLA0_DAT624 entry 37 0
REG = OCLA0_DAT625 0x80011800A8401388 MEMORY 8
REG_FIELD = OCLA0_DAT625 entry 37 0
REG = OCLA0_DAT626 0x80011800A8401390 MEMORY 8
REG_FIELD = OCLA0_DAT626 entry 37 0
REG = OCLA0_DAT627 0x80011800A8401398 MEMORY 8
REG_FIELD = OCLA0_DAT627 entry 37 0
REG = OCLA0_DAT628 0x80011800A84013A0 MEMORY 8
REG_FIELD = OCLA0_DAT628 entry 37 0
REG = OCLA0_DAT629 0x80011800A84013A8 MEMORY 8
REG_FIELD = OCLA0_DAT629 entry 37 0
REG = OCLA0_DAT630 0x80011800A84013B0 MEMORY 8
REG_FIELD = OCLA0_DAT630 entry 37 0
REG = OCLA0_DAT631 0x80011800A84013B8 MEMORY 8
REG_FIELD = OCLA0_DAT631 entry 37 0
REG = OCLA0_DAT632 0x80011800A84013C0 MEMORY 8
REG_FIELD = OCLA0_DAT632 entry 37 0
REG = OCLA0_DAT633 0x80011800A84013C8 MEMORY 8
REG_FIELD = OCLA0_DAT633 entry 37 0
REG = OCLA0_DAT634 0x80011800A84013D0 MEMORY 8
REG_FIELD = OCLA0_DAT634 entry 37 0
REG = OCLA0_DAT635 0x80011800A84013D8 MEMORY 8
REG_FIELD = OCLA0_DAT635 entry 37 0
REG = OCLA0_DAT636 0x80011800A84013E0 MEMORY 8
REG_FIELD = OCLA0_DAT636 entry 37 0
REG = OCLA0_DAT637 0x80011800A84013E8 MEMORY 8
REG_FIELD = OCLA0_DAT637 entry 37 0
REG = OCLA0_DAT638 0x80011800A84013F0 MEMORY 8
REG_FIELD = OCLA0_DAT638 entry 37 0
REG = OCLA0_DAT639 0x80011800A84013F8 MEMORY 8
REG_FIELD = OCLA0_DAT639 entry 37 0
REG = OCLA0_DAT640 0x80011800A8401400 MEMORY 8
REG_FIELD = OCLA0_DAT640 entry 37 0
REG = OCLA0_DAT641 0x80011800A8401408 MEMORY 8
REG_FIELD = OCLA0_DAT641 entry 37 0
REG = OCLA0_DAT642 0x80011800A8401410 MEMORY 8
REG_FIELD = OCLA0_DAT642 entry 37 0
REG = OCLA0_DAT643 0x80011800A8401418 MEMORY 8
REG_FIELD = OCLA0_DAT643 entry 37 0
REG = OCLA0_DAT644 0x80011800A8401420 MEMORY 8
REG_FIELD = OCLA0_DAT644 entry 37 0
REG = OCLA0_DAT645 0x80011800A8401428 MEMORY 8
REG_FIELD = OCLA0_DAT645 entry 37 0
REG = OCLA0_DAT646 0x80011800A8401430 MEMORY 8
REG_FIELD = OCLA0_DAT646 entry 37 0
REG = OCLA0_DAT647 0x80011800A8401438 MEMORY 8
REG_FIELD = OCLA0_DAT647 entry 37 0
REG = OCLA0_DAT648 0x80011800A8401440 MEMORY 8
REG_FIELD = OCLA0_DAT648 entry 37 0
REG = OCLA0_DAT649 0x80011800A8401448 MEMORY 8
REG_FIELD = OCLA0_DAT649 entry 37 0
REG = OCLA0_DAT650 0x80011800A8401450 MEMORY 8
REG_FIELD = OCLA0_DAT650 entry 37 0
REG = OCLA0_DAT651 0x80011800A8401458 MEMORY 8
REG_FIELD = OCLA0_DAT651 entry 37 0
REG = OCLA0_DAT652 0x80011800A8401460 MEMORY 8
REG_FIELD = OCLA0_DAT652 entry 37 0
REG = OCLA0_DAT653 0x80011800A8401468 MEMORY 8
REG_FIELD = OCLA0_DAT653 entry 37 0
REG = OCLA0_DAT654 0x80011800A8401470 MEMORY 8
REG_FIELD = OCLA0_DAT654 entry 37 0
REG = OCLA0_DAT655 0x80011800A8401478 MEMORY 8
REG_FIELD = OCLA0_DAT655 entry 37 0
REG = OCLA0_DAT656 0x80011800A8401480 MEMORY 8
REG_FIELD = OCLA0_DAT656 entry 37 0
REG = OCLA0_DAT657 0x80011800A8401488 MEMORY 8
REG_FIELD = OCLA0_DAT657 entry 37 0
REG = OCLA0_DAT658 0x80011800A8401490 MEMORY 8
REG_FIELD = OCLA0_DAT658 entry 37 0
REG = OCLA0_DAT659 0x80011800A8401498 MEMORY 8
REG_FIELD = OCLA0_DAT659 entry 37 0
REG = OCLA0_DAT660 0x80011800A84014A0 MEMORY 8
REG_FIELD = OCLA0_DAT660 entry 37 0
REG = OCLA0_DAT661 0x80011800A84014A8 MEMORY 8
REG_FIELD = OCLA0_DAT661 entry 37 0
REG = OCLA0_DAT662 0x80011800A84014B0 MEMORY 8
REG_FIELD = OCLA0_DAT662 entry 37 0
REG = OCLA0_DAT663 0x80011800A84014B8 MEMORY 8
REG_FIELD = OCLA0_DAT663 entry 37 0
REG = OCLA0_DAT664 0x80011800A84014C0 MEMORY 8
REG_FIELD = OCLA0_DAT664 entry 37 0
REG = OCLA0_DAT665 0x80011800A84014C8 MEMORY 8
REG_FIELD = OCLA0_DAT665 entry 37 0
REG = OCLA0_DAT666 0x80011800A84014D0 MEMORY 8
REG_FIELD = OCLA0_DAT666 entry 37 0
REG = OCLA0_DAT667 0x80011800A84014D8 MEMORY 8
REG_FIELD = OCLA0_DAT667 entry 37 0
REG = OCLA0_DAT668 0x80011800A84014E0 MEMORY 8
REG_FIELD = OCLA0_DAT668 entry 37 0
REG = OCLA0_DAT669 0x80011800A84014E8 MEMORY 8
REG_FIELD = OCLA0_DAT669 entry 37 0
REG = OCLA0_DAT670 0x80011800A84014F0 MEMORY 8
REG_FIELD = OCLA0_DAT670 entry 37 0
REG = OCLA0_DAT671 0x80011800A84014F8 MEMORY 8
REG_FIELD = OCLA0_DAT671 entry 37 0
REG = OCLA0_DAT672 0x80011800A8401500 MEMORY 8
REG_FIELD = OCLA0_DAT672 entry 37 0
REG = OCLA0_DAT673 0x80011800A8401508 MEMORY 8
REG_FIELD = OCLA0_DAT673 entry 37 0
REG = OCLA0_DAT674 0x80011800A8401510 MEMORY 8
REG_FIELD = OCLA0_DAT674 entry 37 0
REG = OCLA0_DAT675 0x80011800A8401518 MEMORY 8
REG_FIELD = OCLA0_DAT675 entry 37 0
REG = OCLA0_DAT676 0x80011800A8401520 MEMORY 8
REG_FIELD = OCLA0_DAT676 entry 37 0
REG = OCLA0_DAT677 0x80011800A8401528 MEMORY 8
REG_FIELD = OCLA0_DAT677 entry 37 0
REG = OCLA0_DAT678 0x80011800A8401530 MEMORY 8
REG_FIELD = OCLA0_DAT678 entry 37 0
REG = OCLA0_DAT679 0x80011800A8401538 MEMORY 8
REG_FIELD = OCLA0_DAT679 entry 37 0
REG = OCLA0_DAT680 0x80011800A8401540 MEMORY 8
REG_FIELD = OCLA0_DAT680 entry 37 0
REG = OCLA0_DAT681 0x80011800A8401548 MEMORY 8
REG_FIELD = OCLA0_DAT681 entry 37 0
REG = OCLA0_DAT682 0x80011800A8401550 MEMORY 8
REG_FIELD = OCLA0_DAT682 entry 37 0
REG = OCLA0_DAT683 0x80011800A8401558 MEMORY 8
REG_FIELD = OCLA0_DAT683 entry 37 0
REG = OCLA0_DAT684 0x80011800A8401560 MEMORY 8
REG_FIELD = OCLA0_DAT684 entry 37 0
REG = OCLA0_DAT685 0x80011800A8401568 MEMORY 8
REG_FIELD = OCLA0_DAT685 entry 37 0
REG = OCLA0_DAT686 0x80011800A8401570 MEMORY 8
REG_FIELD = OCLA0_DAT686 entry 37 0
REG = OCLA0_DAT687 0x80011800A8401578 MEMORY 8
REG_FIELD = OCLA0_DAT687 entry 37 0
REG = OCLA0_DAT688 0x80011800A8401580 MEMORY 8
REG_FIELD = OCLA0_DAT688 entry 37 0
REG = OCLA0_DAT689 0x80011800A8401588 MEMORY 8
REG_FIELD = OCLA0_DAT689 entry 37 0
REG = OCLA0_DAT690 0x80011800A8401590 MEMORY 8
REG_FIELD = OCLA0_DAT690 entry 37 0
REG = OCLA0_DAT691 0x80011800A8401598 MEMORY 8
REG_FIELD = OCLA0_DAT691 entry 37 0
REG = OCLA0_DAT692 0x80011800A84015A0 MEMORY 8
REG_FIELD = OCLA0_DAT692 entry 37 0
REG = OCLA0_DAT693 0x80011800A84015A8 MEMORY 8
REG_FIELD = OCLA0_DAT693 entry 37 0
REG = OCLA0_DAT694 0x80011800A84015B0 MEMORY 8
REG_FIELD = OCLA0_DAT694 entry 37 0
REG = OCLA0_DAT695 0x80011800A84015B8 MEMORY 8
REG_FIELD = OCLA0_DAT695 entry 37 0
REG = OCLA0_DAT696 0x80011800A84015C0 MEMORY 8
REG_FIELD = OCLA0_DAT696 entry 37 0
REG = OCLA0_DAT697 0x80011800A84015C8 MEMORY 8
REG_FIELD = OCLA0_DAT697 entry 37 0
REG = OCLA0_DAT698 0x80011800A84015D0 MEMORY 8
REG_FIELD = OCLA0_DAT698 entry 37 0
REG = OCLA0_DAT699 0x80011800A84015D8 MEMORY 8
REG_FIELD = OCLA0_DAT699 entry 37 0
REG = OCLA0_DAT700 0x80011800A84015E0 MEMORY 8
REG_FIELD = OCLA0_DAT700 entry 37 0
REG = OCLA0_DAT701 0x80011800A84015E8 MEMORY 8
REG_FIELD = OCLA0_DAT701 entry 37 0
REG = OCLA0_DAT702 0x80011800A84015F0 MEMORY 8
REG_FIELD = OCLA0_DAT702 entry 37 0
REG = OCLA0_DAT703 0x80011800A84015F8 MEMORY 8
REG_FIELD = OCLA0_DAT703 entry 37 0
REG = OCLA0_DAT704 0x80011800A8401600 MEMORY 8
REG_FIELD = OCLA0_DAT704 entry 37 0
REG = OCLA0_DAT705 0x80011800A8401608 MEMORY 8
REG_FIELD = OCLA0_DAT705 entry 37 0
REG = OCLA0_DAT706 0x80011800A8401610 MEMORY 8
REG_FIELD = OCLA0_DAT706 entry 37 0
REG = OCLA0_DAT707 0x80011800A8401618 MEMORY 8
REG_FIELD = OCLA0_DAT707 entry 37 0
REG = OCLA0_DAT708 0x80011800A8401620 MEMORY 8
REG_FIELD = OCLA0_DAT708 entry 37 0
REG = OCLA0_DAT709 0x80011800A8401628 MEMORY 8
REG_FIELD = OCLA0_DAT709 entry 37 0
REG = OCLA0_DAT710 0x80011800A8401630 MEMORY 8
REG_FIELD = OCLA0_DAT710 entry 37 0
REG = OCLA0_DAT711 0x80011800A8401638 MEMORY 8
REG_FIELD = OCLA0_DAT711 entry 37 0
REG = OCLA0_DAT712 0x80011800A8401640 MEMORY 8
REG_FIELD = OCLA0_DAT712 entry 37 0
REG = OCLA0_DAT713 0x80011800A8401648 MEMORY 8
REG_FIELD = OCLA0_DAT713 entry 37 0
REG = OCLA0_DAT714 0x80011800A8401650 MEMORY 8
REG_FIELD = OCLA0_DAT714 entry 37 0
REG = OCLA0_DAT715 0x80011800A8401658 MEMORY 8
REG_FIELD = OCLA0_DAT715 entry 37 0
REG = OCLA0_DAT716 0x80011800A8401660 MEMORY 8
REG_FIELD = OCLA0_DAT716 entry 37 0
REG = OCLA0_DAT717 0x80011800A8401668 MEMORY 8
REG_FIELD = OCLA0_DAT717 entry 37 0
REG = OCLA0_DAT718 0x80011800A8401670 MEMORY 8
REG_FIELD = OCLA0_DAT718 entry 37 0
REG = OCLA0_DAT719 0x80011800A8401678 MEMORY 8
REG_FIELD = OCLA0_DAT719 entry 37 0
REG = OCLA0_DAT720 0x80011800A8401680 MEMORY 8
REG_FIELD = OCLA0_DAT720 entry 37 0
REG = OCLA0_DAT721 0x80011800A8401688 MEMORY 8
REG_FIELD = OCLA0_DAT721 entry 37 0
REG = OCLA0_DAT722 0x80011800A8401690 MEMORY 8
REG_FIELD = OCLA0_DAT722 entry 37 0
REG = OCLA0_DAT723 0x80011800A8401698 MEMORY 8
REG_FIELD = OCLA0_DAT723 entry 37 0
REG = OCLA0_DAT724 0x80011800A84016A0 MEMORY 8
REG_FIELD = OCLA0_DAT724 entry 37 0
REG = OCLA0_DAT725 0x80011800A84016A8 MEMORY 8
REG_FIELD = OCLA0_DAT725 entry 37 0
REG = OCLA0_DAT726 0x80011800A84016B0 MEMORY 8
REG_FIELD = OCLA0_DAT726 entry 37 0
REG = OCLA0_DAT727 0x80011800A84016B8 MEMORY 8
REG_FIELD = OCLA0_DAT727 entry 37 0
REG = OCLA0_DAT728 0x80011800A84016C0 MEMORY 8
REG_FIELD = OCLA0_DAT728 entry 37 0
REG = OCLA0_DAT729 0x80011800A84016C8 MEMORY 8
REG_FIELD = OCLA0_DAT729 entry 37 0
REG = OCLA0_DAT730 0x80011800A84016D0 MEMORY 8
REG_FIELD = OCLA0_DAT730 entry 37 0
REG = OCLA0_DAT731 0x80011800A84016D8 MEMORY 8
REG_FIELD = OCLA0_DAT731 entry 37 0
REG = OCLA0_DAT732 0x80011800A84016E0 MEMORY 8
REG_FIELD = OCLA0_DAT732 entry 37 0
REG = OCLA0_DAT733 0x80011800A84016E8 MEMORY 8
REG_FIELD = OCLA0_DAT733 entry 37 0
REG = OCLA0_DAT734 0x80011800A84016F0 MEMORY 8
REG_FIELD = OCLA0_DAT734 entry 37 0
REG = OCLA0_DAT735 0x80011800A84016F8 MEMORY 8
REG_FIELD = OCLA0_DAT735 entry 37 0
REG = OCLA0_DAT736 0x80011800A8401700 MEMORY 8
REG_FIELD = OCLA0_DAT736 entry 37 0
REG = OCLA0_DAT737 0x80011800A8401708 MEMORY 8
REG_FIELD = OCLA0_DAT737 entry 37 0
REG = OCLA0_DAT738 0x80011800A8401710 MEMORY 8
REG_FIELD = OCLA0_DAT738 entry 37 0
REG = OCLA0_DAT739 0x80011800A8401718 MEMORY 8
REG_FIELD = OCLA0_DAT739 entry 37 0
REG = OCLA0_DAT740 0x80011800A8401720 MEMORY 8
REG_FIELD = OCLA0_DAT740 entry 37 0
REG = OCLA0_DAT741 0x80011800A8401728 MEMORY 8
REG_FIELD = OCLA0_DAT741 entry 37 0
REG = OCLA0_DAT742 0x80011800A8401730 MEMORY 8
REG_FIELD = OCLA0_DAT742 entry 37 0
REG = OCLA0_DAT743 0x80011800A8401738 MEMORY 8
REG_FIELD = OCLA0_DAT743 entry 37 0
REG = OCLA0_DAT744 0x80011800A8401740 MEMORY 8
REG_FIELD = OCLA0_DAT744 entry 37 0
REG = OCLA0_DAT745 0x80011800A8401748 MEMORY 8
REG_FIELD = OCLA0_DAT745 entry 37 0
REG = OCLA0_DAT746 0x80011800A8401750 MEMORY 8
REG_FIELD = OCLA0_DAT746 entry 37 0
REG = OCLA0_DAT747 0x80011800A8401758 MEMORY 8
REG_FIELD = OCLA0_DAT747 entry 37 0
REG = OCLA0_DAT748 0x80011800A8401760 MEMORY 8
REG_FIELD = OCLA0_DAT748 entry 37 0
REG = OCLA0_DAT749 0x80011800A8401768 MEMORY 8
REG_FIELD = OCLA0_DAT749 entry 37 0
REG = OCLA0_DAT750 0x80011800A8401770 MEMORY 8
REG_FIELD = OCLA0_DAT750 entry 37 0
REG = OCLA0_DAT751 0x80011800A8401778 MEMORY 8
REG_FIELD = OCLA0_DAT751 entry 37 0
REG = OCLA0_DAT752 0x80011800A8401780 MEMORY 8
REG_FIELD = OCLA0_DAT752 entry 37 0
REG = OCLA0_DAT753 0x80011800A8401788 MEMORY 8
REG_FIELD = OCLA0_DAT753 entry 37 0
REG = OCLA0_DAT754 0x80011800A8401790 MEMORY 8
REG_FIELD = OCLA0_DAT754 entry 37 0
REG = OCLA0_DAT755 0x80011800A8401798 MEMORY 8
REG_FIELD = OCLA0_DAT755 entry 37 0
REG = OCLA0_DAT756 0x80011800A84017A0 MEMORY 8
REG_FIELD = OCLA0_DAT756 entry 37 0
REG = OCLA0_DAT757 0x80011800A84017A8 MEMORY 8
REG_FIELD = OCLA0_DAT757 entry 37 0
REG = OCLA0_DAT758 0x80011800A84017B0 MEMORY 8
REG_FIELD = OCLA0_DAT758 entry 37 0
REG = OCLA0_DAT759 0x80011800A84017B8 MEMORY 8
REG_FIELD = OCLA0_DAT759 entry 37 0
REG = OCLA0_DAT760 0x80011800A84017C0 MEMORY 8
REG_FIELD = OCLA0_DAT760 entry 37 0
REG = OCLA0_DAT761 0x80011800A84017C8 MEMORY 8
REG_FIELD = OCLA0_DAT761 entry 37 0
REG = OCLA0_DAT762 0x80011800A84017D0 MEMORY 8
REG_FIELD = OCLA0_DAT762 entry 37 0
REG = OCLA0_DAT763 0x80011800A84017D8 MEMORY 8
REG_FIELD = OCLA0_DAT763 entry 37 0
REG = OCLA0_DAT764 0x80011800A84017E0 MEMORY 8
REG_FIELD = OCLA0_DAT764 entry 37 0
REG = OCLA0_DAT765 0x80011800A84017E8 MEMORY 8
REG_FIELD = OCLA0_DAT765 entry 37 0
REG = OCLA0_DAT766 0x80011800A84017F0 MEMORY 8
REG_FIELD = OCLA0_DAT766 entry 37 0
REG = OCLA0_DAT767 0x80011800A84017F8 MEMORY 8
REG_FIELD = OCLA0_DAT767 entry 37 0
REG = OCLA0_DAT768 0x80011800A8401800 MEMORY 8
REG_FIELD = OCLA0_DAT768 entry 37 0
REG = OCLA0_DAT769 0x80011800A8401808 MEMORY 8
REG_FIELD = OCLA0_DAT769 entry 37 0
REG = OCLA0_DAT770 0x80011800A8401810 MEMORY 8
REG_FIELD = OCLA0_DAT770 entry 37 0
REG = OCLA0_DAT771 0x80011800A8401818 MEMORY 8
REG_FIELD = OCLA0_DAT771 entry 37 0
REG = OCLA0_DAT772 0x80011800A8401820 MEMORY 8
REG_FIELD = OCLA0_DAT772 entry 37 0
REG = OCLA0_DAT773 0x80011800A8401828 MEMORY 8
REG_FIELD = OCLA0_DAT773 entry 37 0
REG = OCLA0_DAT774 0x80011800A8401830 MEMORY 8
REG_FIELD = OCLA0_DAT774 entry 37 0
REG = OCLA0_DAT775 0x80011800A8401838 MEMORY 8
REG_FIELD = OCLA0_DAT775 entry 37 0
REG = OCLA0_DAT776 0x80011800A8401840 MEMORY 8
REG_FIELD = OCLA0_DAT776 entry 37 0
REG = OCLA0_DAT777 0x80011800A8401848 MEMORY 8
REG_FIELD = OCLA0_DAT777 entry 37 0
REG = OCLA0_DAT778 0x80011800A8401850 MEMORY 8
REG_FIELD = OCLA0_DAT778 entry 37 0
REG = OCLA0_DAT779 0x80011800A8401858 MEMORY 8
REG_FIELD = OCLA0_DAT779 entry 37 0
REG = OCLA0_DAT780 0x80011800A8401860 MEMORY 8
REG_FIELD = OCLA0_DAT780 entry 37 0
REG = OCLA0_DAT781 0x80011800A8401868 MEMORY 8
REG_FIELD = OCLA0_DAT781 entry 37 0
REG = OCLA0_DAT782 0x80011800A8401870 MEMORY 8
REG_FIELD = OCLA0_DAT782 entry 37 0
REG = OCLA0_DAT783 0x80011800A8401878 MEMORY 8
REG_FIELD = OCLA0_DAT783 entry 37 0
REG = OCLA0_DAT784 0x80011800A8401880 MEMORY 8
REG_FIELD = OCLA0_DAT784 entry 37 0
REG = OCLA0_DAT785 0x80011800A8401888 MEMORY 8
REG_FIELD = OCLA0_DAT785 entry 37 0
REG = OCLA0_DAT786 0x80011800A8401890 MEMORY 8
REG_FIELD = OCLA0_DAT786 entry 37 0
REG = OCLA0_DAT787 0x80011800A8401898 MEMORY 8
REG_FIELD = OCLA0_DAT787 entry 37 0
REG = OCLA0_DAT788 0x80011800A84018A0 MEMORY 8
REG_FIELD = OCLA0_DAT788 entry 37 0
REG = OCLA0_DAT789 0x80011800A84018A8 MEMORY 8
REG_FIELD = OCLA0_DAT789 entry 37 0
REG = OCLA0_DAT790 0x80011800A84018B0 MEMORY 8
REG_FIELD = OCLA0_DAT790 entry 37 0
REG = OCLA0_DAT791 0x80011800A84018B8 MEMORY 8
REG_FIELD = OCLA0_DAT791 entry 37 0
REG = OCLA0_DAT792 0x80011800A84018C0 MEMORY 8
REG_FIELD = OCLA0_DAT792 entry 37 0
REG = OCLA0_DAT793 0x80011800A84018C8 MEMORY 8
REG_FIELD = OCLA0_DAT793 entry 37 0
REG = OCLA0_DAT794 0x80011800A84018D0 MEMORY 8
REG_FIELD = OCLA0_DAT794 entry 37 0
REG = OCLA0_DAT795 0x80011800A84018D8 MEMORY 8
REG_FIELD = OCLA0_DAT795 entry 37 0
REG = OCLA0_DAT796 0x80011800A84018E0 MEMORY 8
REG_FIELD = OCLA0_DAT796 entry 37 0
REG = OCLA0_DAT797 0x80011800A84018E8 MEMORY 8
REG_FIELD = OCLA0_DAT797 entry 37 0
REG = OCLA0_DAT798 0x80011800A84018F0 MEMORY 8
REG_FIELD = OCLA0_DAT798 entry 37 0
REG = OCLA0_DAT799 0x80011800A84018F8 MEMORY 8
REG_FIELD = OCLA0_DAT799 entry 37 0
REG = OCLA0_DAT800 0x80011800A8401900 MEMORY 8
REG_FIELD = OCLA0_DAT800 entry 37 0
REG = OCLA0_DAT801 0x80011800A8401908 MEMORY 8
REG_FIELD = OCLA0_DAT801 entry 37 0
REG = OCLA0_DAT802 0x80011800A8401910 MEMORY 8
REG_FIELD = OCLA0_DAT802 entry 37 0
REG = OCLA0_DAT803 0x80011800A8401918 MEMORY 8
REG_FIELD = OCLA0_DAT803 entry 37 0
REG = OCLA0_DAT804 0x80011800A8401920 MEMORY 8
REG_FIELD = OCLA0_DAT804 entry 37 0
REG = OCLA0_DAT805 0x80011800A8401928 MEMORY 8
REG_FIELD = OCLA0_DAT805 entry 37 0
REG = OCLA0_DAT806 0x80011800A8401930 MEMORY 8
REG_FIELD = OCLA0_DAT806 entry 37 0
REG = OCLA0_DAT807 0x80011800A8401938 MEMORY 8
REG_FIELD = OCLA0_DAT807 entry 37 0
REG = OCLA0_DAT808 0x80011800A8401940 MEMORY 8
REG_FIELD = OCLA0_DAT808 entry 37 0
REG = OCLA0_DAT809 0x80011800A8401948 MEMORY 8
REG_FIELD = OCLA0_DAT809 entry 37 0
REG = OCLA0_DAT810 0x80011800A8401950 MEMORY 8
REG_FIELD = OCLA0_DAT810 entry 37 0
REG = OCLA0_DAT811 0x80011800A8401958 MEMORY 8
REG_FIELD = OCLA0_DAT811 entry 37 0
REG = OCLA0_DAT812 0x80011800A8401960 MEMORY 8
REG_FIELD = OCLA0_DAT812 entry 37 0
REG = OCLA0_DAT813 0x80011800A8401968 MEMORY 8
REG_FIELD = OCLA0_DAT813 entry 37 0
REG = OCLA0_DAT814 0x80011800A8401970 MEMORY 8
REG_FIELD = OCLA0_DAT814 entry 37 0
REG = OCLA0_DAT815 0x80011800A8401978 MEMORY 8
REG_FIELD = OCLA0_DAT815 entry 37 0
REG = OCLA0_DAT816 0x80011800A8401980 MEMORY 8
REG_FIELD = OCLA0_DAT816 entry 37 0
REG = OCLA0_DAT817 0x80011800A8401988 MEMORY 8
REG_FIELD = OCLA0_DAT817 entry 37 0
REG = OCLA0_DAT818 0x80011800A8401990 MEMORY 8
REG_FIELD = OCLA0_DAT818 entry 37 0
REG = OCLA0_DAT819 0x80011800A8401998 MEMORY 8
REG_FIELD = OCLA0_DAT819 entry 37 0
REG = OCLA0_DAT820 0x80011800A84019A0 MEMORY 8
REG_FIELD = OCLA0_DAT820 entry 37 0
REG = OCLA0_DAT821 0x80011800A84019A8 MEMORY 8
REG_FIELD = OCLA0_DAT821 entry 37 0
REG = OCLA0_DAT822 0x80011800A84019B0 MEMORY 8
REG_FIELD = OCLA0_DAT822 entry 37 0
REG = OCLA0_DAT823 0x80011800A84019B8 MEMORY 8
REG_FIELD = OCLA0_DAT823 entry 37 0
REG = OCLA0_DAT824 0x80011800A84019C0 MEMORY 8
REG_FIELD = OCLA0_DAT824 entry 37 0
REG = OCLA0_DAT825 0x80011800A84019C8 MEMORY 8
REG_FIELD = OCLA0_DAT825 entry 37 0
REG = OCLA0_DAT826 0x80011800A84019D0 MEMORY 8
REG_FIELD = OCLA0_DAT826 entry 37 0
REG = OCLA0_DAT827 0x80011800A84019D8 MEMORY 8
REG_FIELD = OCLA0_DAT827 entry 37 0
REG = OCLA0_DAT828 0x80011800A84019E0 MEMORY 8
REG_FIELD = OCLA0_DAT828 entry 37 0
REG = OCLA0_DAT829 0x80011800A84019E8 MEMORY 8
REG_FIELD = OCLA0_DAT829 entry 37 0
REG = OCLA0_DAT830 0x80011800A84019F0 MEMORY 8
REG_FIELD = OCLA0_DAT830 entry 37 0
REG = OCLA0_DAT831 0x80011800A84019F8 MEMORY 8
REG_FIELD = OCLA0_DAT831 entry 37 0
REG = OCLA0_DAT832 0x80011800A8401A00 MEMORY 8
REG_FIELD = OCLA0_DAT832 entry 37 0
REG = OCLA0_DAT833 0x80011800A8401A08 MEMORY 8
REG_FIELD = OCLA0_DAT833 entry 37 0
REG = OCLA0_DAT834 0x80011800A8401A10 MEMORY 8
REG_FIELD = OCLA0_DAT834 entry 37 0
REG = OCLA0_DAT835 0x80011800A8401A18 MEMORY 8
REG_FIELD = OCLA0_DAT835 entry 37 0
REG = OCLA0_DAT836 0x80011800A8401A20 MEMORY 8
REG_FIELD = OCLA0_DAT836 entry 37 0
REG = OCLA0_DAT837 0x80011800A8401A28 MEMORY 8
REG_FIELD = OCLA0_DAT837 entry 37 0
REG = OCLA0_DAT838 0x80011800A8401A30 MEMORY 8
REG_FIELD = OCLA0_DAT838 entry 37 0
REG = OCLA0_DAT839 0x80011800A8401A38 MEMORY 8
REG_FIELD = OCLA0_DAT839 entry 37 0
REG = OCLA0_DAT840 0x80011800A8401A40 MEMORY 8
REG_FIELD = OCLA0_DAT840 entry 37 0
REG = OCLA0_DAT841 0x80011800A8401A48 MEMORY 8
REG_FIELD = OCLA0_DAT841 entry 37 0
REG = OCLA0_DAT842 0x80011800A8401A50 MEMORY 8
REG_FIELD = OCLA0_DAT842 entry 37 0
REG = OCLA0_DAT843 0x80011800A8401A58 MEMORY 8
REG_FIELD = OCLA0_DAT843 entry 37 0
REG = OCLA0_DAT844 0x80011800A8401A60 MEMORY 8
REG_FIELD = OCLA0_DAT844 entry 37 0
REG = OCLA0_DAT845 0x80011800A8401A68 MEMORY 8
REG_FIELD = OCLA0_DAT845 entry 37 0
REG = OCLA0_DAT846 0x80011800A8401A70 MEMORY 8
REG_FIELD = OCLA0_DAT846 entry 37 0
REG = OCLA0_DAT847 0x80011800A8401A78 MEMORY 8
REG_FIELD = OCLA0_DAT847 entry 37 0
REG = OCLA0_DAT848 0x80011800A8401A80 MEMORY 8
REG_FIELD = OCLA0_DAT848 entry 37 0
REG = OCLA0_DAT849 0x80011800A8401A88 MEMORY 8
REG_FIELD = OCLA0_DAT849 entry 37 0
REG = OCLA0_DAT850 0x80011800A8401A90 MEMORY 8
REG_FIELD = OCLA0_DAT850 entry 37 0
REG = OCLA0_DAT851 0x80011800A8401A98 MEMORY 8
REG_FIELD = OCLA0_DAT851 entry 37 0
REG = OCLA0_DAT852 0x80011800A8401AA0 MEMORY 8
REG_FIELD = OCLA0_DAT852 entry 37 0
REG = OCLA0_DAT853 0x80011800A8401AA8 MEMORY 8
REG_FIELD = OCLA0_DAT853 entry 37 0
REG = OCLA0_DAT854 0x80011800A8401AB0 MEMORY 8
REG_FIELD = OCLA0_DAT854 entry 37 0
REG = OCLA0_DAT855 0x80011800A8401AB8 MEMORY 8
REG_FIELD = OCLA0_DAT855 entry 37 0
REG = OCLA0_DAT856 0x80011800A8401AC0 MEMORY 8
REG_FIELD = OCLA0_DAT856 entry 37 0
REG = OCLA0_DAT857 0x80011800A8401AC8 MEMORY 8
REG_FIELD = OCLA0_DAT857 entry 37 0
REG = OCLA0_DAT858 0x80011800A8401AD0 MEMORY 8
REG_FIELD = OCLA0_DAT858 entry 37 0
REG = OCLA0_DAT859 0x80011800A8401AD8 MEMORY 8
REG_FIELD = OCLA0_DAT859 entry 37 0
REG = OCLA0_DAT860 0x80011800A8401AE0 MEMORY 8
REG_FIELD = OCLA0_DAT860 entry 37 0
REG = OCLA0_DAT861 0x80011800A8401AE8 MEMORY 8
REG_FIELD = OCLA0_DAT861 entry 37 0
REG = OCLA0_DAT862 0x80011800A8401AF0 MEMORY 8
REG_FIELD = OCLA0_DAT862 entry 37 0
REG = OCLA0_DAT863 0x80011800A8401AF8 MEMORY 8
REG_FIELD = OCLA0_DAT863 entry 37 0
REG = OCLA0_DAT864 0x80011800A8401B00 MEMORY 8
REG_FIELD = OCLA0_DAT864 entry 37 0
REG = OCLA0_DAT865 0x80011800A8401B08 MEMORY 8
REG_FIELD = OCLA0_DAT865 entry 37 0
REG = OCLA0_DAT866 0x80011800A8401B10 MEMORY 8
REG_FIELD = OCLA0_DAT866 entry 37 0
REG = OCLA0_DAT867 0x80011800A8401B18 MEMORY 8
REG_FIELD = OCLA0_DAT867 entry 37 0
REG = OCLA0_DAT868 0x80011800A8401B20 MEMORY 8
REG_FIELD = OCLA0_DAT868 entry 37 0
REG = OCLA0_DAT869 0x80011800A8401B28 MEMORY 8
REG_FIELD = OCLA0_DAT869 entry 37 0
REG = OCLA0_DAT870 0x80011800A8401B30 MEMORY 8
REG_FIELD = OCLA0_DAT870 entry 37 0
REG = OCLA0_DAT871 0x80011800A8401B38 MEMORY 8
REG_FIELD = OCLA0_DAT871 entry 37 0
REG = OCLA0_DAT872 0x80011800A8401B40 MEMORY 8
REG_FIELD = OCLA0_DAT872 entry 37 0
REG = OCLA0_DAT873 0x80011800A8401B48 MEMORY 8
REG_FIELD = OCLA0_DAT873 entry 37 0
REG = OCLA0_DAT874 0x80011800A8401B50 MEMORY 8
REG_FIELD = OCLA0_DAT874 entry 37 0
REG = OCLA0_DAT875 0x80011800A8401B58 MEMORY 8
REG_FIELD = OCLA0_DAT875 entry 37 0
REG = OCLA0_DAT876 0x80011800A8401B60 MEMORY 8
REG_FIELD = OCLA0_DAT876 entry 37 0
REG = OCLA0_DAT877 0x80011800A8401B68 MEMORY 8
REG_FIELD = OCLA0_DAT877 entry 37 0
REG = OCLA0_DAT878 0x80011800A8401B70 MEMORY 8
REG_FIELD = OCLA0_DAT878 entry 37 0
REG = OCLA0_DAT879 0x80011800A8401B78 MEMORY 8
REG_FIELD = OCLA0_DAT879 entry 37 0
REG = OCLA0_DAT880 0x80011800A8401B80 MEMORY 8
REG_FIELD = OCLA0_DAT880 entry 37 0
REG = OCLA0_DAT881 0x80011800A8401B88 MEMORY 8
REG_FIELD = OCLA0_DAT881 entry 37 0
REG = OCLA0_DAT882 0x80011800A8401B90 MEMORY 8
REG_FIELD = OCLA0_DAT882 entry 37 0
REG = OCLA0_DAT883 0x80011800A8401B98 MEMORY 8
REG_FIELD = OCLA0_DAT883 entry 37 0
REG = OCLA0_DAT884 0x80011800A8401BA0 MEMORY 8
REG_FIELD = OCLA0_DAT884 entry 37 0
REG = OCLA0_DAT885 0x80011800A8401BA8 MEMORY 8
REG_FIELD = OCLA0_DAT885 entry 37 0
REG = OCLA0_DAT886 0x80011800A8401BB0 MEMORY 8
REG_FIELD = OCLA0_DAT886 entry 37 0
REG = OCLA0_DAT887 0x80011800A8401BB8 MEMORY 8
REG_FIELD = OCLA0_DAT887 entry 37 0
REG = OCLA0_DAT888 0x80011800A8401BC0 MEMORY 8
REG_FIELD = OCLA0_DAT888 entry 37 0
REG = OCLA0_DAT889 0x80011800A8401BC8 MEMORY 8
REG_FIELD = OCLA0_DAT889 entry 37 0
REG = OCLA0_DAT890 0x80011800A8401BD0 MEMORY 8
REG_FIELD = OCLA0_DAT890 entry 37 0
REG = OCLA0_DAT891 0x80011800A8401BD8 MEMORY 8
REG_FIELD = OCLA0_DAT891 entry 37 0
REG = OCLA0_DAT892 0x80011800A8401BE0 MEMORY 8
REG_FIELD = OCLA0_DAT892 entry 37 0
REG = OCLA0_DAT893 0x80011800A8401BE8 MEMORY 8
REG_FIELD = OCLA0_DAT893 entry 37 0
REG = OCLA0_DAT894 0x80011800A8401BF0 MEMORY 8
REG_FIELD = OCLA0_DAT894 entry 37 0
REG = OCLA0_DAT895 0x80011800A8401BF8 MEMORY 8
REG_FIELD = OCLA0_DAT895 entry 37 0
REG = OCLA0_DAT896 0x80011800A8401C00 MEMORY 8
REG_FIELD = OCLA0_DAT896 entry 37 0
REG = OCLA0_DAT897 0x80011800A8401C08 MEMORY 8
REG_FIELD = OCLA0_DAT897 entry 37 0
REG = OCLA0_DAT898 0x80011800A8401C10 MEMORY 8
REG_FIELD = OCLA0_DAT898 entry 37 0
REG = OCLA0_DAT899 0x80011800A8401C18 MEMORY 8
REG_FIELD = OCLA0_DAT899 entry 37 0
REG = OCLA0_DAT900 0x80011800A8401C20 MEMORY 8
REG_FIELD = OCLA0_DAT900 entry 37 0
REG = OCLA0_DAT901 0x80011800A8401C28 MEMORY 8
REG_FIELD = OCLA0_DAT901 entry 37 0
REG = OCLA0_DAT902 0x80011800A8401C30 MEMORY 8
REG_FIELD = OCLA0_DAT902 entry 37 0
REG = OCLA0_DAT903 0x80011800A8401C38 MEMORY 8
REG_FIELD = OCLA0_DAT903 entry 37 0
REG = OCLA0_DAT904 0x80011800A8401C40 MEMORY 8
REG_FIELD = OCLA0_DAT904 entry 37 0
REG = OCLA0_DAT905 0x80011800A8401C48 MEMORY 8
REG_FIELD = OCLA0_DAT905 entry 37 0
REG = OCLA0_DAT906 0x80011800A8401C50 MEMORY 8
REG_FIELD = OCLA0_DAT906 entry 37 0
REG = OCLA0_DAT907 0x80011800A8401C58 MEMORY 8
REG_FIELD = OCLA0_DAT907 entry 37 0
REG = OCLA0_DAT908 0x80011800A8401C60 MEMORY 8
REG_FIELD = OCLA0_DAT908 entry 37 0
REG = OCLA0_DAT909 0x80011800A8401C68 MEMORY 8
REG_FIELD = OCLA0_DAT909 entry 37 0
REG = OCLA0_DAT910 0x80011800A8401C70 MEMORY 8
REG_FIELD = OCLA0_DAT910 entry 37 0
REG = OCLA0_DAT911 0x80011800A8401C78 MEMORY 8
REG_FIELD = OCLA0_DAT911 entry 37 0
REG = OCLA0_DAT912 0x80011800A8401C80 MEMORY 8
REG_FIELD = OCLA0_DAT912 entry 37 0
REG = OCLA0_DAT913 0x80011800A8401C88 MEMORY 8
REG_FIELD = OCLA0_DAT913 entry 37 0
REG = OCLA0_DAT914 0x80011800A8401C90 MEMORY 8
REG_FIELD = OCLA0_DAT914 entry 37 0
REG = OCLA0_DAT915 0x80011800A8401C98 MEMORY 8
REG_FIELD = OCLA0_DAT915 entry 37 0
REG = OCLA0_DAT916 0x80011800A8401CA0 MEMORY 8
REG_FIELD = OCLA0_DAT916 entry 37 0
REG = OCLA0_DAT917 0x80011800A8401CA8 MEMORY 8
REG_FIELD = OCLA0_DAT917 entry 37 0
REG = OCLA0_DAT918 0x80011800A8401CB0 MEMORY 8
REG_FIELD = OCLA0_DAT918 entry 37 0
REG = OCLA0_DAT919 0x80011800A8401CB8 MEMORY 8
REG_FIELD = OCLA0_DAT919 entry 37 0
REG = OCLA0_DAT920 0x80011800A8401CC0 MEMORY 8
REG_FIELD = OCLA0_DAT920 entry 37 0
REG = OCLA0_DAT921 0x80011800A8401CC8 MEMORY 8
REG_FIELD = OCLA0_DAT921 entry 37 0
REG = OCLA0_DAT922 0x80011800A8401CD0 MEMORY 8
REG_FIELD = OCLA0_DAT922 entry 37 0
REG = OCLA0_DAT923 0x80011800A8401CD8 MEMORY 8
REG_FIELD = OCLA0_DAT923 entry 37 0
REG = OCLA0_DAT924 0x80011800A8401CE0 MEMORY 8
REG_FIELD = OCLA0_DAT924 entry 37 0
REG = OCLA0_DAT925 0x80011800A8401CE8 MEMORY 8
REG_FIELD = OCLA0_DAT925 entry 37 0
REG = OCLA0_DAT926 0x80011800A8401CF0 MEMORY 8
REG_FIELD = OCLA0_DAT926 entry 37 0
REG = OCLA0_DAT927 0x80011800A8401CF8 MEMORY 8
REG_FIELD = OCLA0_DAT927 entry 37 0
REG = OCLA0_DAT928 0x80011800A8401D00 MEMORY 8
REG_FIELD = OCLA0_DAT928 entry 37 0
REG = OCLA0_DAT929 0x80011800A8401D08 MEMORY 8
REG_FIELD = OCLA0_DAT929 entry 37 0
REG = OCLA0_DAT930 0x80011800A8401D10 MEMORY 8
REG_FIELD = OCLA0_DAT930 entry 37 0
REG = OCLA0_DAT931 0x80011800A8401D18 MEMORY 8
REG_FIELD = OCLA0_DAT931 entry 37 0
REG = OCLA0_DAT932 0x80011800A8401D20 MEMORY 8
REG_FIELD = OCLA0_DAT932 entry 37 0
REG = OCLA0_DAT933 0x80011800A8401D28 MEMORY 8
REG_FIELD = OCLA0_DAT933 entry 37 0
REG = OCLA0_DAT934 0x80011800A8401D30 MEMORY 8
REG_FIELD = OCLA0_DAT934 entry 37 0
REG = OCLA0_DAT935 0x80011800A8401D38 MEMORY 8
REG_FIELD = OCLA0_DAT935 entry 37 0
REG = OCLA0_DAT936 0x80011800A8401D40 MEMORY 8
REG_FIELD = OCLA0_DAT936 entry 37 0
REG = OCLA0_DAT937 0x80011800A8401D48 MEMORY 8
REG_FIELD = OCLA0_DAT937 entry 37 0
REG = OCLA0_DAT938 0x80011800A8401D50 MEMORY 8
REG_FIELD = OCLA0_DAT938 entry 37 0
REG = OCLA0_DAT939 0x80011800A8401D58 MEMORY 8
REG_FIELD = OCLA0_DAT939 entry 37 0
REG = OCLA0_DAT940 0x80011800A8401D60 MEMORY 8
REG_FIELD = OCLA0_DAT940 entry 37 0
REG = OCLA0_DAT941 0x80011800A8401D68 MEMORY 8
REG_FIELD = OCLA0_DAT941 entry 37 0
REG = OCLA0_DAT942 0x80011800A8401D70 MEMORY 8
REG_FIELD = OCLA0_DAT942 entry 37 0
REG = OCLA0_DAT943 0x80011800A8401D78 MEMORY 8
REG_FIELD = OCLA0_DAT943 entry 37 0
REG = OCLA0_DAT944 0x80011800A8401D80 MEMORY 8
REG_FIELD = OCLA0_DAT944 entry 37 0
REG = OCLA0_DAT945 0x80011800A8401D88 MEMORY 8
REG_FIELD = OCLA0_DAT945 entry 37 0
REG = OCLA0_DAT946 0x80011800A8401D90 MEMORY 8
REG_FIELD = OCLA0_DAT946 entry 37 0
REG = OCLA0_DAT947 0x80011800A8401D98 MEMORY 8
REG_FIELD = OCLA0_DAT947 entry 37 0
REG = OCLA0_DAT948 0x80011800A8401DA0 MEMORY 8
REG_FIELD = OCLA0_DAT948 entry 37 0
REG = OCLA0_DAT949 0x80011800A8401DA8 MEMORY 8
REG_FIELD = OCLA0_DAT949 entry 37 0
REG = OCLA0_DAT950 0x80011800A8401DB0 MEMORY 8
REG_FIELD = OCLA0_DAT950 entry 37 0
REG = OCLA0_DAT951 0x80011800A8401DB8 MEMORY 8
REG_FIELD = OCLA0_DAT951 entry 37 0
REG = OCLA0_DAT952 0x80011800A8401DC0 MEMORY 8
REG_FIELD = OCLA0_DAT952 entry 37 0
REG = OCLA0_DAT953 0x80011800A8401DC8 MEMORY 8
REG_FIELD = OCLA0_DAT953 entry 37 0
REG = OCLA0_DAT954 0x80011800A8401DD0 MEMORY 8
REG_FIELD = OCLA0_DAT954 entry 37 0
REG = OCLA0_DAT955 0x80011800A8401DD8 MEMORY 8
REG_FIELD = OCLA0_DAT955 entry 37 0
REG = OCLA0_DAT956 0x80011800A8401DE0 MEMORY 8
REG_FIELD = OCLA0_DAT956 entry 37 0
REG = OCLA0_DAT957 0x80011800A8401DE8 MEMORY 8
REG_FIELD = OCLA0_DAT957 entry 37 0
REG = OCLA0_DAT958 0x80011800A8401DF0 MEMORY 8
REG_FIELD = OCLA0_DAT958 entry 37 0
REG = OCLA0_DAT959 0x80011800A8401DF8 MEMORY 8
REG_FIELD = OCLA0_DAT959 entry 37 0
REG = OCLA0_DAT960 0x80011800A8401E00 MEMORY 8
REG_FIELD = OCLA0_DAT960 entry 37 0
REG = OCLA0_DAT961 0x80011800A8401E08 MEMORY 8
REG_FIELD = OCLA0_DAT961 entry 37 0
REG = OCLA0_DAT962 0x80011800A8401E10 MEMORY 8
REG_FIELD = OCLA0_DAT962 entry 37 0
REG = OCLA0_DAT963 0x80011800A8401E18 MEMORY 8
REG_FIELD = OCLA0_DAT963 entry 37 0
REG = OCLA0_DAT964 0x80011800A8401E20 MEMORY 8
REG_FIELD = OCLA0_DAT964 entry 37 0
REG = OCLA0_DAT965 0x80011800A8401E28 MEMORY 8
REG_FIELD = OCLA0_DAT965 entry 37 0
REG = OCLA0_DAT966 0x80011800A8401E30 MEMORY 8
REG_FIELD = OCLA0_DAT966 entry 37 0
REG = OCLA0_DAT967 0x80011800A8401E38 MEMORY 8
REG_FIELD = OCLA0_DAT967 entry 37 0
REG = OCLA0_DAT968 0x80011800A8401E40 MEMORY 8
REG_FIELD = OCLA0_DAT968 entry 37 0
REG = OCLA0_DAT969 0x80011800A8401E48 MEMORY 8
REG_FIELD = OCLA0_DAT969 entry 37 0
REG = OCLA0_DAT970 0x80011800A8401E50 MEMORY 8
REG_FIELD = OCLA0_DAT970 entry 37 0
REG = OCLA0_DAT971 0x80011800A8401E58 MEMORY 8
REG_FIELD = OCLA0_DAT971 entry 37 0
REG = OCLA0_DAT972 0x80011800A8401E60 MEMORY 8
REG_FIELD = OCLA0_DAT972 entry 37 0
REG = OCLA0_DAT973 0x80011800A8401E68 MEMORY 8
REG_FIELD = OCLA0_DAT973 entry 37 0
REG = OCLA0_DAT974 0x80011800A8401E70 MEMORY 8
REG_FIELD = OCLA0_DAT974 entry 37 0
REG = OCLA0_DAT975 0x80011800A8401E78 MEMORY 8
REG_FIELD = OCLA0_DAT975 entry 37 0
REG = OCLA0_DAT976 0x80011800A8401E80 MEMORY 8
REG_FIELD = OCLA0_DAT976 entry 37 0
REG = OCLA0_DAT977 0x80011800A8401E88 MEMORY 8
REG_FIELD = OCLA0_DAT977 entry 37 0
REG = OCLA0_DAT978 0x80011800A8401E90 MEMORY 8
REG_FIELD = OCLA0_DAT978 entry 37 0
REG = OCLA0_DAT979 0x80011800A8401E98 MEMORY 8
REG_FIELD = OCLA0_DAT979 entry 37 0
REG = OCLA0_DAT980 0x80011800A8401EA0 MEMORY 8
REG_FIELD = OCLA0_DAT980 entry 37 0
REG = OCLA0_DAT981 0x80011800A8401EA8 MEMORY 8
REG_FIELD = OCLA0_DAT981 entry 37 0
REG = OCLA0_DAT982 0x80011800A8401EB0 MEMORY 8
REG_FIELD = OCLA0_DAT982 entry 37 0
REG = OCLA0_DAT983 0x80011800A8401EB8 MEMORY 8
REG_FIELD = OCLA0_DAT983 entry 37 0
REG = OCLA0_DAT984 0x80011800A8401EC0 MEMORY 8
REG_FIELD = OCLA0_DAT984 entry 37 0
REG = OCLA0_DAT985 0x80011800A8401EC8 MEMORY 8
REG_FIELD = OCLA0_DAT985 entry 37 0
REG = OCLA0_DAT986 0x80011800A8401ED0 MEMORY 8
REG_FIELD = OCLA0_DAT986 entry 37 0
REG = OCLA0_DAT987 0x80011800A8401ED8 MEMORY 8
REG_FIELD = OCLA0_DAT987 entry 37 0
REG = OCLA0_DAT988 0x80011800A8401EE0 MEMORY 8
REG_FIELD = OCLA0_DAT988 entry 37 0
REG = OCLA0_DAT989 0x80011800A8401EE8 MEMORY 8
REG_FIELD = OCLA0_DAT989 entry 37 0
REG = OCLA0_DAT990 0x80011800A8401EF0 MEMORY 8
REG_FIELD = OCLA0_DAT990 entry 37 0
REG = OCLA0_DAT991 0x80011800A8401EF8 MEMORY 8
REG_FIELD = OCLA0_DAT991 entry 37 0
REG = OCLA0_DAT992 0x80011800A8401F00 MEMORY 8
REG_FIELD = OCLA0_DAT992 entry 37 0
REG = OCLA0_DAT993 0x80011800A8401F08 MEMORY 8
REG_FIELD = OCLA0_DAT993 entry 37 0
REG = OCLA0_DAT994 0x80011800A8401F10 MEMORY 8
REG_FIELD = OCLA0_DAT994 entry 37 0
REG = OCLA0_DAT995 0x80011800A8401F18 MEMORY 8
REG_FIELD = OCLA0_DAT995 entry 37 0
REG = OCLA0_DAT996 0x80011800A8401F20 MEMORY 8
REG_FIELD = OCLA0_DAT996 entry 37 0
REG = OCLA0_DAT997 0x80011800A8401F28 MEMORY 8
REG_FIELD = OCLA0_DAT997 entry 37 0
REG = OCLA0_DAT998 0x80011800A8401F30 MEMORY 8
REG_FIELD = OCLA0_DAT998 entry 37 0
REG = OCLA0_DAT999 0x80011800A8401F38 MEMORY 8
REG_FIELD = OCLA0_DAT999 entry 37 0
REG = OCLA0_DAT1000 0x80011800A8401F40 MEMORY 8
REG_FIELD = OCLA0_DAT1000 entry 37 0
REG = OCLA0_DAT1001 0x80011800A8401F48 MEMORY 8
REG_FIELD = OCLA0_DAT1001 entry 37 0
REG = OCLA0_DAT1002 0x80011800A8401F50 MEMORY 8
REG_FIELD = OCLA0_DAT1002 entry 37 0
REG = OCLA0_DAT1003 0x80011800A8401F58 MEMORY 8
REG_FIELD = OCLA0_DAT1003 entry 37 0
REG = OCLA0_DAT1004 0x80011800A8401F60 MEMORY 8
REG_FIELD = OCLA0_DAT1004 entry 37 0
REG = OCLA0_DAT1005 0x80011800A8401F68 MEMORY 8
REG_FIELD = OCLA0_DAT1005 entry 37 0
REG = OCLA0_DAT1006 0x80011800A8401F70 MEMORY 8
REG_FIELD = OCLA0_DAT1006 entry 37 0
REG = OCLA0_DAT1007 0x80011800A8401F78 MEMORY 8
REG_FIELD = OCLA0_DAT1007 entry 37 0
REG = OCLA0_DAT1008 0x80011800A8401F80 MEMORY 8
REG_FIELD = OCLA0_DAT1008 entry 37 0
REG = OCLA0_DAT1009 0x80011800A8401F88 MEMORY 8
REG_FIELD = OCLA0_DAT1009 entry 37 0
REG = OCLA0_DAT1010 0x80011800A8401F90 MEMORY 8
REG_FIELD = OCLA0_DAT1010 entry 37 0
REG = OCLA0_DAT1011 0x80011800A8401F98 MEMORY 8
REG_FIELD = OCLA0_DAT1011 entry 37 0
REG = OCLA0_DAT1012 0x80011800A8401FA0 MEMORY 8
REG_FIELD = OCLA0_DAT1012 entry 37 0
REG = OCLA0_DAT1013 0x80011800A8401FA8 MEMORY 8
REG_FIELD = OCLA0_DAT1013 entry 37 0
REG = OCLA0_DAT1014 0x80011800A8401FB0 MEMORY 8
REG_FIELD = OCLA0_DAT1014 entry 37 0
REG = OCLA0_DAT1015 0x80011800A8401FB8 MEMORY 8
REG_FIELD = OCLA0_DAT1015 entry 37 0
REG = OCLA0_DAT1016 0x80011800A8401FC0 MEMORY 8
REG_FIELD = OCLA0_DAT1016 entry 37 0
REG = OCLA0_DAT1017 0x80011800A8401FC8 MEMORY 8
REG_FIELD = OCLA0_DAT1017 entry 37 0
REG = OCLA0_DAT1018 0x80011800A8401FD0 MEMORY 8
REG_FIELD = OCLA0_DAT1018 entry 37 0
REG = OCLA0_DAT1019 0x80011800A8401FD8 MEMORY 8
REG_FIELD = OCLA0_DAT1019 entry 37 0
REG = OCLA0_DAT1020 0x80011800A8401FE0 MEMORY 8
REG_FIELD = OCLA0_DAT1020 entry 37 0
REG = OCLA0_DAT1021 0x80011800A8401FE8 MEMORY 8
REG_FIELD = OCLA0_DAT1021 entry 37 0
REG = OCLA0_DAT1022 0x80011800A8401FF0 MEMORY 8
REG_FIELD = OCLA0_DAT1022 entry 37 0
REG = OCLA0_DAT1023 0x80011800A8401FF8 MEMORY 8
REG_FIELD = OCLA0_DAT1023 entry 37 0

REG = OCLA0_DAT_POP 0x80011800A8000800 MEMORY 8
REG_FIELD = OCLA0_DAT_POP valid 63 63, trig 62 62, wmark 61 61, entry 37 0

REG = OCLA0_FIFO_DEPTH 0x80011800A8000200 MEMORY 8
REG_FIELD = OCLA0_FIFO_DEPTH depth 15 0

REG = OCLA0_FIFO_LIMIT 0x80011800A8000240 MEMORY 8
REG_FIELD = OCLA0_FIFO_LIMIT overfull 63 48, ddr 47 32, bp 31 16, wmark 15 0

REG = OCLA0_FIFO_TAIL 0x80011800A8000260 MEMORY 8
REG_FIELD = OCLA0_FIFO_TAIL tail 15 0

REG = OCLA0_FIFO_TRIG 0x80011800A80002A0 MEMORY 8
REG_FIELD = OCLA0_FIFO_TRIG limit 31 16, cnt 15 0

REG = OCLA0_FIFO_WRAP 0x80011800A8000280 MEMORY 8
REG_FIELD = OCLA0_FIFO_WRAP wraps 31 0




REG = OCLA0_GEN_CTL 0x80011800A8000060 MEMORY 8
REG_FIELD = OCLA0_GEN_CTL mcdtrig 6 4, exten 3 3, den 2 2, stt 1 1

REG = OCLA0_MAT000_COUNT 0x80011800A8230000 MEMORY 8
REG_FIELD = OCLA0_MAT000_COUNT count 31 0
REG = OCLA0_MAT001_COUNT 0x80011800A8231000 MEMORY 8
REG_FIELD = OCLA0_MAT001_COUNT count 31 0
REG = OCLA0_MAT002_COUNT 0x80011800A8232000 MEMORY 8
REG_FIELD = OCLA0_MAT002_COUNT count 31 0
REG = OCLA0_MAT003_COUNT 0x80011800A8233000 MEMORY 8
REG_FIELD = OCLA0_MAT003_COUNT count 31 0

REG = OCLA0_MAT000_CTL 0x80011800A8200000 MEMORY 8
REG_FIELD = OCLA0_MAT000_CTL fsm_ctr 7 7, inc_match 6 6, shift 5 0
REG = OCLA0_MAT001_CTL 0x80011800A8201000 MEMORY 8
REG_FIELD = OCLA0_MAT001_CTL fsm_ctr 7 7, inc_match 6 6, shift 5 0
REG = OCLA0_MAT002_CTL 0x80011800A8202000 MEMORY 8
REG_FIELD = OCLA0_MAT002_CTL fsm_ctr 7 7, inc_match 6 6, shift 5 0
REG = OCLA0_MAT003_CTL 0x80011800A8203000 MEMORY 8
REG_FIELD = OCLA0_MAT003_CTL fsm_ctr 7 7, inc_match 6 6, shift 5 0


REG = OCLA0_MAT000_THRESH 0x80011800A8240000 MEMORY 8
REG_FIELD = OCLA0_MAT000_THRESH thresh 31 0
REG = OCLA0_MAT001_THRESH 0x80011800A8241000 MEMORY 8
REG_FIELD = OCLA0_MAT001_THRESH thresh 31 0
REG = OCLA0_MAT002_THRESH 0x80011800A8242000 MEMORY 8
REG_FIELD = OCLA0_MAT002_THRESH thresh 31 0
REG = OCLA0_MAT003_THRESH 0x80011800A8243000 MEMORY 8
REG_FIELD = OCLA0_MAT003_THRESH thresh 31 0


REG = OCLA0_RAW000 0x80011800A8000100 MEMORY 8
REG_FIELD = OCLA0_RAW000 raw 35 0
REG = OCLA0_RAW001 0x80011800A8000108 MEMORY 8
REG_FIELD = OCLA0_RAW001 raw 35 0

REG = OCLA0_SFT_RST 0x80011800A8000020 MEMORY 8
REG_FIELD = OCLA0_SFT_RST reset 0 0

REG = OCLA0_STACK_BASE 0x80011800A8000400 MEMORY 8
REG_FIELD = OCLA0_STACK_BASE ptr 41 7

REG = OCLA0_STACK_CUR 0x80011800A8000480 MEMORY 8
REG_FIELD = OCLA0_STACK_CUR ptr 41 7

REG = OCLA0_STACK_STORE_CNT 0x80011800A8000460 MEMORY 8
REG_FIELD = OCLA0_STACK_STORE_CNT stores 31 0

REG = OCLA0_STACK_TOP 0x80011800A8000420 MEMORY 8
REG_FIELD = OCLA0_STACK_TOP ptr 41 7

REG = OCLA0_STACK_WRAP 0x80011800A8000440 MEMORY 8
REG_FIELD = OCLA0_STACK_WRAP wraps 31 0

REG = OCLA0_STAGE000 0x80011800A8100000 MEMORY 8
REG_FIELD = OCLA0_STAGE000 dly 2 0
REG = OCLA0_STAGE001 0x80011800A8100008 MEMORY 8
REG_FIELD = OCLA0_STAGE001 dly 2 0
REG = OCLA0_STAGE002 0x80011800A8100010 MEMORY 8
REG_FIELD = OCLA0_STAGE002 dly 2 0
REG = OCLA0_STAGE003 0x80011800A8100018 MEMORY 8
REG_FIELD = OCLA0_STAGE003 dly 2 0
REG = OCLA0_STAGE004 0x80011800A8100020 MEMORY 8
REG_FIELD = OCLA0_STAGE004 dly 2 0
REG = OCLA0_STAGE005 0x80011800A8100028 MEMORY 8
REG_FIELD = OCLA0_STAGE005 dly 2 0
REG = OCLA0_STAGE006 0x80011800A8100030 MEMORY 8
REG_FIELD = OCLA0_STAGE006 dly 2 0
REG = OCLA0_STAGE007 0x80011800A8100038 MEMORY 8
REG_FIELD = OCLA0_STAGE007 dly 2 0
REG = OCLA0_STAGE008 0x80011800A8100040 MEMORY 8
REG_FIELD = OCLA0_STAGE008 dly 2 0
REG = OCLA0_STAGE009 0x80011800A8100048 MEMORY 8
REG_FIELD = OCLA0_STAGE009 dly 2 0
REG = OCLA0_STAGE010 0x80011800A8100050 MEMORY 8
REG_FIELD = OCLA0_STAGE010 dly 2 0
REG = OCLA0_STAGE011 0x80011800A8100058 MEMORY 8
REG_FIELD = OCLA0_STAGE011 dly 2 0
REG = OCLA0_STAGE012 0x80011800A8100060 MEMORY 8
REG_FIELD = OCLA0_STAGE012 dly 2 0
REG = OCLA0_STAGE013 0x80011800A8100068 MEMORY 8
REG_FIELD = OCLA0_STAGE013 dly 2 0
REG = OCLA0_STAGE014 0x80011800A8100070 MEMORY 8
REG_FIELD = OCLA0_STAGE014 dly 2 0
REG = OCLA0_STAGE015 0x80011800A8100078 MEMORY 8
REG_FIELD = OCLA0_STAGE015 dly 2 0
REG = OCLA0_STAGE016 0x80011800A8100080 MEMORY 8
REG_FIELD = OCLA0_STAGE016 dly 2 0
REG = OCLA0_STAGE017 0x80011800A8100088 MEMORY 8
REG_FIELD = OCLA0_STAGE017 dly 2 0
REG = OCLA0_STAGE018 0x80011800A8100090 MEMORY 8
REG_FIELD = OCLA0_STAGE018 dly 2 0
REG = OCLA0_STAGE019 0x80011800A8100098 MEMORY 8
REG_FIELD = OCLA0_STAGE019 dly 2 0
REG = OCLA0_STAGE020 0x80011800A81000A0 MEMORY 8
REG_FIELD = OCLA0_STAGE020 dly 2 0
REG = OCLA0_STAGE021 0x80011800A81000A8 MEMORY 8
REG_FIELD = OCLA0_STAGE021 dly 2 0
REG = OCLA0_STAGE022 0x80011800A81000B0 MEMORY 8
REG_FIELD = OCLA0_STAGE022 dly 2 0
REG = OCLA0_STAGE023 0x80011800A81000B8 MEMORY 8
REG_FIELD = OCLA0_STAGE023 dly 2 0
REG = OCLA0_STAGE024 0x80011800A81000C0 MEMORY 8
REG_FIELD = OCLA0_STAGE024 dly 2 0
REG = OCLA0_STAGE025 0x80011800A81000C8 MEMORY 8
REG_FIELD = OCLA0_STAGE025 dly 2 0
REG = OCLA0_STAGE026 0x80011800A81000D0 MEMORY 8
REG_FIELD = OCLA0_STAGE026 dly 2 0
REG = OCLA0_STAGE027 0x80011800A81000D8 MEMORY 8
REG_FIELD = OCLA0_STAGE027 dly 2 0
REG = OCLA0_STAGE028 0x80011800A81000E0 MEMORY 8
REG_FIELD = OCLA0_STAGE028 dly 2 0
REG = OCLA0_STAGE029 0x80011800A81000E8 MEMORY 8
REG_FIELD = OCLA0_STAGE029 dly 2 0
REG = OCLA0_STAGE030 0x80011800A81000F0 MEMORY 8
REG_FIELD = OCLA0_STAGE030 dly 2 0
REG = OCLA0_STAGE031 0x80011800A81000F8 MEMORY 8
REG_FIELD = OCLA0_STAGE031 dly 2 0
REG = OCLA0_STAGE032 0x80011800A8100100 MEMORY 8
REG_FIELD = OCLA0_STAGE032 dly 2 0
REG = OCLA0_STAGE033 0x80011800A8100108 MEMORY 8
REG_FIELD = OCLA0_STAGE033 dly 2 0
REG = OCLA0_STAGE034 0x80011800A8100110 MEMORY 8
REG_FIELD = OCLA0_STAGE034 dly 2 0
REG = OCLA0_STAGE035 0x80011800A8100118 MEMORY 8
REG_FIELD = OCLA0_STAGE035 dly 2 0
REG = OCLA0_STAGE036 0x80011800A8100120 MEMORY 8
REG_FIELD = OCLA0_STAGE036 dly 2 0
REG = OCLA0_STAGE037 0x80011800A8100128 MEMORY 8
REG_FIELD = OCLA0_STAGE037 dly 2 0
REG = OCLA0_STAGE038 0x80011800A8100130 MEMORY 8
REG_FIELD = OCLA0_STAGE038 dly 2 0
REG = OCLA0_STAGE039 0x80011800A8100138 MEMORY 8
REG_FIELD = OCLA0_STAGE039 dly 2 0
REG = OCLA0_STAGE040 0x80011800A8100140 MEMORY 8
REG_FIELD = OCLA0_STAGE040 dly 2 0
REG = OCLA0_STAGE041 0x80011800A8100148 MEMORY 8
REG_FIELD = OCLA0_STAGE041 dly 2 0
REG = OCLA0_STAGE042 0x80011800A8100150 MEMORY 8
REG_FIELD = OCLA0_STAGE042 dly 2 0
REG = OCLA0_STAGE043 0x80011800A8100158 MEMORY 8
REG_FIELD = OCLA0_STAGE043 dly 2 0
REG = OCLA0_STAGE044 0x80011800A8100160 MEMORY 8
REG_FIELD = OCLA0_STAGE044 dly 2 0
REG = OCLA0_STAGE045 0x80011800A8100168 MEMORY 8
REG_FIELD = OCLA0_STAGE045 dly 2 0
REG = OCLA0_STAGE046 0x80011800A8100170 MEMORY 8
REG_FIELD = OCLA0_STAGE046 dly 2 0
REG = OCLA0_STAGE047 0x80011800A8100178 MEMORY 8
REG_FIELD = OCLA0_STAGE047 dly 2 0
REG = OCLA0_STAGE048 0x80011800A8100180 MEMORY 8
REG_FIELD = OCLA0_STAGE048 dly 2 0
REG = OCLA0_STAGE049 0x80011800A8100188 MEMORY 8
REG_FIELD = OCLA0_STAGE049 dly 2 0
REG = OCLA0_STAGE050 0x80011800A8100190 MEMORY 8
REG_FIELD = OCLA0_STAGE050 dly 2 0
REG = OCLA0_STAGE051 0x80011800A8100198 MEMORY 8
REG_FIELD = OCLA0_STAGE051 dly 2 0
REG = OCLA0_STAGE052 0x80011800A81001A0 MEMORY 8
REG_FIELD = OCLA0_STAGE052 dly 2 0
REG = OCLA0_STAGE053 0x80011800A81001A8 MEMORY 8
REG_FIELD = OCLA0_STAGE053 dly 2 0
REG = OCLA0_STAGE054 0x80011800A81001B0 MEMORY 8
REG_FIELD = OCLA0_STAGE054 dly 2 0
REG = OCLA0_STAGE055 0x80011800A81001B8 MEMORY 8
REG_FIELD = OCLA0_STAGE055 dly 2 0
REG = OCLA0_STAGE056 0x80011800A81001C0 MEMORY 8
REG_FIELD = OCLA0_STAGE056 dly 2 0
REG = OCLA0_STAGE057 0x80011800A81001C8 MEMORY 8
REG_FIELD = OCLA0_STAGE057 dly 2 0
REG = OCLA0_STAGE058 0x80011800A81001D0 MEMORY 8
REG_FIELD = OCLA0_STAGE058 dly 2 0
REG = OCLA0_STAGE059 0x80011800A81001D8 MEMORY 8
REG_FIELD = OCLA0_STAGE059 dly 2 0
REG = OCLA0_STAGE060 0x80011800A81001E0 MEMORY 8
REG_FIELD = OCLA0_STAGE060 dly 2 0
REG = OCLA0_STAGE061 0x80011800A81001E8 MEMORY 8
REG_FIELD = OCLA0_STAGE061 dly 2 0
REG = OCLA0_STAGE062 0x80011800A81001F0 MEMORY 8
REG_FIELD = OCLA0_STAGE062 dly 2 0
REG = OCLA0_STAGE063 0x80011800A81001F8 MEMORY 8
REG_FIELD = OCLA0_STAGE063 dly 2 0
REG = OCLA0_STAGE064 0x80011800A8100200 MEMORY 8
REG_FIELD = OCLA0_STAGE064 dly 2 0
REG = OCLA0_STAGE065 0x80011800A8100208 MEMORY 8
REG_FIELD = OCLA0_STAGE065 dly 2 0
REG = OCLA0_STAGE066 0x80011800A8100210 MEMORY 8
REG_FIELD = OCLA0_STAGE066 dly 2 0
REG = OCLA0_STAGE067 0x80011800A8100218 MEMORY 8
REG_FIELD = OCLA0_STAGE067 dly 2 0
REG = OCLA0_STAGE068 0x80011800A8100220 MEMORY 8
REG_FIELD = OCLA0_STAGE068 dly 2 0
REG = OCLA0_STAGE069 0x80011800A8100228 MEMORY 8
REG_FIELD = OCLA0_STAGE069 dly 2 0
REG = OCLA0_STAGE070 0x80011800A8100230 MEMORY 8
REG_FIELD = OCLA0_STAGE070 dly 2 0
REG = OCLA0_STAGE071 0x80011800A8100238 MEMORY 8
REG_FIELD = OCLA0_STAGE071 dly 2 0

REG = OCLA0_STATE_INT 0x80011800A8000080 MEMORY 8
REG_FIELD = OCLA0_STATE_INT fsm1_state 63 60, fsm0_state 59 56, fsm1_rst 35 35, fsm0_rst 34 34, fsm1_ena 33 33, fsm0_ena 32 32, ddrfull 18 18, wmark 17 17, overfull 16 16, trigfull 15 15, captured 14 14, fsm1_int 13 13, fsm0_int 12 12, mcd 11 9, trig 8 8, ovfl 3 0

REG = OCLA0_STATE_SET 0x80011800A80000A0 MEMORY 8
REG_FIELD = OCLA0_STATE_SET fsm1_state 63 60, fsm0_state 59 56, fsm1_rst 35 35, fsm0_rst 34 34, fsm1_ena 33 33, fsm0_ena 32 32, ddrfull 18 18, wmark 17 17, overfull 16 16, trigfull 15 15, captured 14 14, fsm1_int 13 13, fsm0_int 12 12, mcd 11 9, trig 8 8, ovfl 3 0

REG = OCLA0_TIME 0x80011800A80000C0 MEMORY 8
REG_FIELD = OCLA0_TIME cycle 31 0





























































































































































REG = PCM0_DMA_CFG 0x8001070000010018 MEMORY 8
REG_FIELD = PCM0_DMA_CFG rdpend 63 63, rxslots 53 44, txslots 41 32, rxst 29 20, useldt 18 18, txrd 17 8, fetchsiz 7 4, thresh 3 0
REG = PCM1_DMA_CFG 0x8001070000014018 MEMORY 8
REG_FIELD = PCM1_DMA_CFG rdpend 63 63, rxslots 53 44, txslots 41 32, rxst 29 20, useldt 18 18, txrd 17 8, fetchsiz 7 4, thresh 3 0
REG = PCM2_DMA_CFG 0x8001070000018018 MEMORY 8
REG_FIELD = PCM2_DMA_CFG rdpend 63 63, rxslots 53 44, txslots 41 32, rxst 29 20, useldt 18 18, txrd 17 8, fetchsiz 7 4, thresh 3 0
REG = PCM3_DMA_CFG 0x800107000001C018 MEMORY 8
REG_FIELD = PCM3_DMA_CFG rdpend 63 63, rxslots 53 44, txslots 41 32, rxst 29 20, useldt 18 18, txrd 17 8, fetchsiz 7 4, thresh 3 0

REG = PCM0_INT_ENA 0x8001070000010020 MEMORY 8
REG_FIELD = PCM0_INT_ENA rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM1_INT_ENA 0x8001070000014020 MEMORY 8
REG_FIELD = PCM1_INT_ENA rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM2_INT_ENA 0x8001070000018020 MEMORY 8
REG_FIELD = PCM2_INT_ENA rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM3_INT_ENA 0x800107000001C020 MEMORY 8
REG_FIELD = PCM3_INT_ENA rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0

REG = PCM0_INT_SUM 0x8001070000010028 MEMORY 8
REG_FIELD = PCM0_INT_SUM rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM1_INT_SUM 0x8001070000014028 MEMORY 8
REG_FIELD = PCM1_INT_SUM rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM2_INT_SUM 0x8001070000018028 MEMORY 8
REG_FIELD = PCM2_INT_SUM rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0
REG = PCM3_INT_SUM 0x800107000001C028 MEMORY 8
REG_FIELD = PCM3_INT_SUM rxovf 7 7, txempty 6 6, txrd 5 5, txwrap 4 4, rxst 3 3, rxwrap 2 2, fsyncextra 1 1, fsyncmissed 0 0

REG = PCM0_RXADDR 0x8001070000010068 MEMORY 8
REG_FIELD = PCM0_RXADDR addr 35 0
REG = PCM1_RXADDR 0x8001070000014068 MEMORY 8
REG_FIELD = PCM1_RXADDR addr 35 0
REG = PCM2_RXADDR 0x8001070000018068 MEMORY 8
REG_FIELD = PCM2_RXADDR addr 35 0
REG = PCM3_RXADDR 0x800107000001C068 MEMORY 8
REG_FIELD = PCM3_RXADDR addr 35 0

REG = PCM0_RXCNT 0x8001070000010060 MEMORY 8
REG_FIELD = PCM0_RXCNT cnt 15 0
REG = PCM1_RXCNT 0x8001070000014060 MEMORY 8
REG_FIELD = PCM1_RXCNT cnt 15 0
REG = PCM2_RXCNT 0x8001070000018060 MEMORY 8
REG_FIELD = PCM2_RXCNT cnt 15 0
REG = PCM3_RXCNT 0x800107000001C060 MEMORY 8
REG_FIELD = PCM3_RXCNT cnt 15 0

REG = PCM0_RXMSK000 0x80010700000100C0 MEMORY 8
REG_FIELD = PCM0_RXMSK000 mask 63 0
REG = PCM0_RXMSK001 0x80010700000100C8 MEMORY 8
REG_FIELD = PCM0_RXMSK001 mask 63 0
REG = PCM0_RXMSK002 0x80010700000100D0 MEMORY 8
REG_FIELD = PCM0_RXMSK002 mask 63 0
REG = PCM0_RXMSK003 0x80010700000100D8 MEMORY 8
REG_FIELD = PCM0_RXMSK003 mask 63 0
REG = PCM0_RXMSK004 0x80010700000100E0 MEMORY 8
REG_FIELD = PCM0_RXMSK004 mask 63 0
REG = PCM0_RXMSK005 0x80010700000100E8 MEMORY 8
REG_FIELD = PCM0_RXMSK005 mask 63 0
REG = PCM0_RXMSK006 0x80010700000100F0 MEMORY 8
REG_FIELD = PCM0_RXMSK006 mask 63 0
REG = PCM0_RXMSK007 0x80010700000100F8 MEMORY 8
REG_FIELD = PCM0_RXMSK007 mask 63 0
REG = PCM1_RXMSK000 0x80010700000140C0 MEMORY 8
REG_FIELD = PCM1_RXMSK000 mask 63 0
REG = PCM1_RXMSK001 0x80010700000140C8 MEMORY 8
REG_FIELD = PCM1_RXMSK001 mask 63 0
REG = PCM1_RXMSK002 0x80010700000140D0 MEMORY 8
REG_FIELD = PCM1_RXMSK002 mask 63 0
REG = PCM1_RXMSK003 0x80010700000140D8 MEMORY 8
REG_FIELD = PCM1_RXMSK003 mask 63 0
REG = PCM1_RXMSK004 0x80010700000140E0 MEMORY 8
REG_FIELD = PCM1_RXMSK004 mask 63 0
REG = PCM1_RXMSK005 0x80010700000140E8 MEMORY 8
REG_FIELD = PCM1_RXMSK005 mask 63 0
REG = PCM1_RXMSK006 0x80010700000140F0 MEMORY 8
REG_FIELD = PCM1_RXMSK006 mask 63 0
REG = PCM1_RXMSK007 0x80010700000140F8 MEMORY 8
REG_FIELD = PCM1_RXMSK007 mask 63 0
REG = PCM2_RXMSK000 0x80010700000180C0 MEMORY 8
REG_FIELD = PCM2_RXMSK000 mask 63 0
REG = PCM2_RXMSK001 0x80010700000180C8 MEMORY 8
REG_FIELD = PCM2_RXMSK001 mask 63 0
REG = PCM2_RXMSK002 0x80010700000180D0 MEMORY 8
REG_FIELD = PCM2_RXMSK002 mask 63 0
REG = PCM2_RXMSK003 0x80010700000180D8 MEMORY 8
REG_FIELD = PCM2_RXMSK003 mask 63 0
REG = PCM2_RXMSK004 0x80010700000180E0 MEMORY 8
REG_FIELD = PCM2_RXMSK004 mask 63 0
REG = PCM2_RXMSK005 0x80010700000180E8 MEMORY 8
REG_FIELD = PCM2_RXMSK005 mask 63 0
REG = PCM2_RXMSK006 0x80010700000180F0 MEMORY 8
REG_FIELD = PCM2_RXMSK006 mask 63 0
REG = PCM2_RXMSK007 0x80010700000180F8 MEMORY 8
REG_FIELD = PCM2_RXMSK007 mask 63 0
REG = PCM3_RXMSK000 0x800107000001C0C0 MEMORY 8
REG_FIELD = PCM3_RXMSK000 mask 63 0
REG = PCM3_RXMSK001 0x800107000001C0C8 MEMORY 8
REG_FIELD = PCM3_RXMSK001 mask 63 0
REG = PCM3_RXMSK002 0x800107000001C0D0 MEMORY 8
REG_FIELD = PCM3_RXMSK002 mask 63 0
REG = PCM3_RXMSK003 0x800107000001C0D8 MEMORY 8
REG_FIELD = PCM3_RXMSK003 mask 63 0
REG = PCM3_RXMSK004 0x800107000001C0E0 MEMORY 8
REG_FIELD = PCM3_RXMSK004 mask 63 0
REG = PCM3_RXMSK005 0x800107000001C0E8 MEMORY 8
REG_FIELD = PCM3_RXMSK005 mask 63 0
REG = PCM3_RXMSK006 0x800107000001C0F0 MEMORY 8
REG_FIELD = PCM3_RXMSK006 mask 63 0
REG = PCM3_RXMSK007 0x800107000001C0F8 MEMORY 8
REG_FIELD = PCM3_RXMSK007 mask 63 0

REG = PCM0_RXSTART 0x8001070000010058 MEMORY 8
REG_FIELD = PCM0_RXSTART addr 35 3
REG = PCM1_RXSTART 0x8001070000014058 MEMORY 8
REG_FIELD = PCM1_RXSTART addr 35 3
REG = PCM2_RXSTART 0x8001070000018058 MEMORY 8
REG_FIELD = PCM2_RXSTART addr 35 3
REG = PCM3_RXSTART 0x800107000001C058 MEMORY 8
REG_FIELD = PCM3_RXSTART addr 35 3

REG = PCM0_TDM_CFG 0x8001070000010010 MEMORY 8
REG_FIELD = PCM0_TDM_CFG drvtim 63 48, samppt 47 32, lsbfirst 2 2, useclk1 1 1, enable 0 0
REG = PCM1_TDM_CFG 0x8001070000014010 MEMORY 8
REG_FIELD = PCM1_TDM_CFG drvtim 63 48, samppt 47 32, lsbfirst 2 2, useclk1 1 1, enable 0 0
REG = PCM2_TDM_CFG 0x8001070000018010 MEMORY 8
REG_FIELD = PCM2_TDM_CFG drvtim 63 48, samppt 47 32, lsbfirst 2 2, useclk1 1 1, enable 0 0
REG = PCM3_TDM_CFG 0x800107000001C010 MEMORY 8
REG_FIELD = PCM3_TDM_CFG drvtim 63 48, samppt 47 32, lsbfirst 2 2, useclk1 1 1, enable 0 0

REG = PCM0_TDM_DBG 0x8001070000010030 MEMORY 8
REG_FIELD = PCM0_TDM_DBG debuginfo 63 0
REG = PCM1_TDM_DBG 0x8001070000014030 MEMORY 8
REG_FIELD = PCM1_TDM_DBG debuginfo 63 0
REG = PCM2_TDM_DBG 0x8001070000018030 MEMORY 8
REG_FIELD = PCM2_TDM_DBG debuginfo 63 0
REG = PCM3_TDM_DBG 0x800107000001C030 MEMORY 8
REG_FIELD = PCM3_TDM_DBG debuginfo 63 0

REG = PCM0_TXADDR 0x8001070000010050 MEMORY 8
REG_FIELD = PCM0_TXADDR addr 35 3, fram 2 0
REG = PCM1_TXADDR 0x8001070000014050 MEMORY 8
REG_FIELD = PCM1_TXADDR addr 35 3, fram 2 0
REG = PCM2_TXADDR 0x8001070000018050 MEMORY 8
REG_FIELD = PCM2_TXADDR addr 35 3, fram 2 0
REG = PCM3_TXADDR 0x800107000001C050 MEMORY 8
REG_FIELD = PCM3_TXADDR addr 35 3, fram 2 0

REG = PCM0_TXCNT 0x8001070000010048 MEMORY 8
REG_FIELD = PCM0_TXCNT cnt 15 0
REG = PCM1_TXCNT 0x8001070000014048 MEMORY 8
REG_FIELD = PCM1_TXCNT cnt 15 0
REG = PCM2_TXCNT 0x8001070000018048 MEMORY 8
REG_FIELD = PCM2_TXCNT cnt 15 0
REG = PCM3_TXCNT 0x800107000001C048 MEMORY 8
REG_FIELD = PCM3_TXCNT cnt 15 0

REG = PCM0_TXMSK000 0x8001070000010080 MEMORY 8
REG_FIELD = PCM0_TXMSK000 mask 63 0
REG = PCM0_TXMSK001 0x8001070000010088 MEMORY 8
REG_FIELD = PCM0_TXMSK001 mask 63 0
REG = PCM0_TXMSK002 0x8001070000010090 MEMORY 8
REG_FIELD = PCM0_TXMSK002 mask 63 0
REG = PCM0_TXMSK003 0x8001070000010098 MEMORY 8
REG_FIELD = PCM0_TXMSK003 mask 63 0
REG = PCM0_TXMSK004 0x80010700000100A0 MEMORY 8
REG_FIELD = PCM0_TXMSK004 mask 63 0
REG = PCM0_TXMSK005 0x80010700000100A8 MEMORY 8
REG_FIELD = PCM0_TXMSK005 mask 63 0
REG = PCM0_TXMSK006 0x80010700000100B0 MEMORY 8
REG_FIELD = PCM0_TXMSK006 mask 63 0
REG = PCM0_TXMSK007 0x80010700000100B8 MEMORY 8
REG_FIELD = PCM0_TXMSK007 mask 63 0
REG = PCM1_TXMSK000 0x8001070000014080 MEMORY 8
REG_FIELD = PCM1_TXMSK000 mask 63 0
REG = PCM1_TXMSK001 0x8001070000014088 MEMORY 8
REG_FIELD = PCM1_TXMSK001 mask 63 0
REG = PCM1_TXMSK002 0x8001070000014090 MEMORY 8
REG_FIELD = PCM1_TXMSK002 mask 63 0
REG = PCM1_TXMSK003 0x8001070000014098 MEMORY 8
REG_FIELD = PCM1_TXMSK003 mask 63 0
REG = PCM1_TXMSK004 0x80010700000140A0 MEMORY 8
REG_FIELD = PCM1_TXMSK004 mask 63 0
REG = PCM1_TXMSK005 0x80010700000140A8 MEMORY 8
REG_FIELD = PCM1_TXMSK005 mask 63 0
REG = PCM1_TXMSK006 0x80010700000140B0 MEMORY 8
REG_FIELD = PCM1_TXMSK006 mask 63 0
REG = PCM1_TXMSK007 0x80010700000140B8 MEMORY 8
REG_FIELD = PCM1_TXMSK007 mask 63 0
REG = PCM2_TXMSK000 0x8001070000018080 MEMORY 8
REG_FIELD = PCM2_TXMSK000 mask 63 0
REG = PCM2_TXMSK001 0x8001070000018088 MEMORY 8
REG_FIELD = PCM2_TXMSK001 mask 63 0
REG = PCM2_TXMSK002 0x8001070000018090 MEMORY 8
REG_FIELD = PCM2_TXMSK002 mask 63 0
REG = PCM2_TXMSK003 0x8001070000018098 MEMORY 8
REG_FIELD = PCM2_TXMSK003 mask 63 0
REG = PCM2_TXMSK004 0x80010700000180A0 MEMORY 8
REG_FIELD = PCM2_TXMSK004 mask 63 0
REG = PCM2_TXMSK005 0x80010700000180A8 MEMORY 8
REG_FIELD = PCM2_TXMSK005 mask 63 0
REG = PCM2_TXMSK006 0x80010700000180B0 MEMORY 8
REG_FIELD = PCM2_TXMSK006 mask 63 0
REG = PCM2_TXMSK007 0x80010700000180B8 MEMORY 8
REG_FIELD = PCM2_TXMSK007 mask 63 0
REG = PCM3_TXMSK000 0x800107000001C080 MEMORY 8
REG_FIELD = PCM3_TXMSK000 mask 63 0
REG = PCM3_TXMSK001 0x800107000001C088 MEMORY 8
REG_FIELD = PCM3_TXMSK001 mask 63 0
REG = PCM3_TXMSK002 0x800107000001C090 MEMORY 8
REG_FIELD = PCM3_TXMSK002 mask 63 0
REG = PCM3_TXMSK003 0x800107000001C098 MEMORY 8
REG_FIELD = PCM3_TXMSK003 mask 63 0
REG = PCM3_TXMSK004 0x800107000001C0A0 MEMORY 8
REG_FIELD = PCM3_TXMSK004 mask 63 0
REG = PCM3_TXMSK005 0x800107000001C0A8 MEMORY 8
REG_FIELD = PCM3_TXMSK005 mask 63 0
REG = PCM3_TXMSK006 0x800107000001C0B0 MEMORY 8
REG_FIELD = PCM3_TXMSK006 mask 63 0
REG = PCM3_TXMSK007 0x800107000001C0B8 MEMORY 8
REG_FIELD = PCM3_TXMSK007 mask 63 0

REG = PCM0_TXSTART 0x8001070000010040 MEMORY 8
REG_FIELD = PCM0_TXSTART addr 35 3
REG = PCM1_TXSTART 0x8001070000014040 MEMORY 8
REG_FIELD = PCM1_TXSTART addr 35 3
REG = PCM2_TXSTART 0x8001070000018040 MEMORY 8
REG_FIELD = PCM2_TXSTART addr 35 3
REG = PCM3_TXSTART 0x800107000001C040 MEMORY 8
REG_FIELD = PCM3_TXSTART addr 35 3

REG = PCM_CLK0_CFG 0x8001070000010000 MEMORY 8
REG_FIELD = PCM_CLK0_CFG fsyncgood 63 63, fsyncsamp 47 32, fsynclen 25 21, fsyncloc 20 16, numslots 15 6, extrabit 5 5, bitlen 4 3, bclkpol 2 2, fsyncpol 1 1, ena 0 0
REG = PCM_CLK1_CFG 0x8001070000014000 MEMORY 8
REG_FIELD = PCM_CLK1_CFG fsyncgood 63 63, fsyncsamp 47 32, fsynclen 25 21, fsyncloc 20 16, numslots 15 6, extrabit 5 5, bitlen 4 3, bclkpol 2 2, fsyncpol 1 1, ena 0 0

REG = PCM_CLK0_DBG 0x8001070000010038 MEMORY 8
REG_FIELD = PCM_CLK0_DBG debuginfo 63 0
REG = PCM_CLK1_DBG 0x8001070000014038 MEMORY 8
REG_FIELD = PCM_CLK1_DBG debuginfo 63 0

REG = PCM_CLK0_GEN 0x8001070000010008 MEMORY 8
REG_FIELD = PCM_CLK0_GEN deltasamp 63 48, numsamp 47 32, n 31 0
REG = PCM_CLK1_GEN 0x8001070000014008 MEMORY 8
REG_FIELD = PCM_CLK1_GEN deltasamp 63 48, numsamp 47 32, n 31 0

REG = PCS0_AN000_ADV_REG 0x80011800B0001010 MEMORY 8
REG_FIELD = PCS0_AN000_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN001_ADV_REG 0x80011800B0001410 MEMORY 8
REG_FIELD = PCS0_AN001_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN002_ADV_REG 0x80011800B0001810 MEMORY 8
REG_FIELD = PCS0_AN002_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN003_ADV_REG 0x80011800B0001C10 MEMORY 8
REG_FIELD = PCS0_AN003_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN000_ADV_REG 0x80011800B8001010 MEMORY 8
REG_FIELD = PCS1_AN000_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN001_ADV_REG 0x80011800B8001410 MEMORY 8
REG_FIELD = PCS1_AN001_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN002_ADV_REG 0x80011800B8001810 MEMORY 8
REG_FIELD = PCS1_AN002_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN003_ADV_REG 0x80011800B8001C10 MEMORY 8
REG_FIELD = PCS1_AN003_ADV_REG np 15 15, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5

REG = PCS0_AN000_EXT_ST_REG 0x80011800B0001028 MEMORY 8
REG_FIELD = PCS0_AN000_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS0_AN001_EXT_ST_REG 0x80011800B0001428 MEMORY 8
REG_FIELD = PCS0_AN001_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS0_AN002_EXT_ST_REG 0x80011800B0001828 MEMORY 8
REG_FIELD = PCS0_AN002_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS0_AN003_EXT_ST_REG 0x80011800B0001C28 MEMORY 8
REG_FIELD = PCS0_AN003_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS1_AN000_EXT_ST_REG 0x80011800B8001028 MEMORY 8
REG_FIELD = PCS1_AN000_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS1_AN001_EXT_ST_REG 0x80011800B8001428 MEMORY 8
REG_FIELD = PCS1_AN001_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS1_AN002_EXT_ST_REG 0x80011800B8001828 MEMORY 8
REG_FIELD = PCS1_AN002_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12
REG = PCS1_AN003_EXT_ST_REG 0x80011800B8001C28 MEMORY 8
REG_FIELD = PCS1_AN003_EXT_ST_REG thou_xfd 15 15, thou_xhd 14 14, thou_tfd 13 13, thou_thd 12 12

REG = PCS0_AN000_LP_ABIL_REG 0x80011800B0001018 MEMORY 8
REG_FIELD = PCS0_AN000_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN001_LP_ABIL_REG 0x80011800B0001418 MEMORY 8
REG_FIELD = PCS0_AN001_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN002_LP_ABIL_REG 0x80011800B0001818 MEMORY 8
REG_FIELD = PCS0_AN002_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS0_AN003_LP_ABIL_REG 0x80011800B0001C18 MEMORY 8
REG_FIELD = PCS0_AN003_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN000_LP_ABIL_REG 0x80011800B8001018 MEMORY 8
REG_FIELD = PCS1_AN000_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN001_LP_ABIL_REG 0x80011800B8001418 MEMORY 8
REG_FIELD = PCS1_AN001_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN002_LP_ABIL_REG 0x80011800B8001818 MEMORY 8
REG_FIELD = PCS1_AN002_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5
REG = PCS1_AN003_LP_ABIL_REG 0x80011800B8001C18 MEMORY 8
REG_FIELD = PCS1_AN003_LP_ABIL_REG np 15 15, ack 14 14, rem_flt 13 12, pause 8 7, hfd 6 6, fd 5 5

REG = PCS0_AN000_RESULTS_REG 0x80011800B0001020 MEMORY 8
REG_FIELD = PCS0_AN000_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS0_AN001_RESULTS_REG 0x80011800B0001420 MEMORY 8
REG_FIELD = PCS0_AN001_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS0_AN002_RESULTS_REG 0x80011800B0001820 MEMORY 8
REG_FIELD = PCS0_AN002_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS0_AN003_RESULTS_REG 0x80011800B0001C20 MEMORY 8
REG_FIELD = PCS0_AN003_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS1_AN000_RESULTS_REG 0x80011800B8001020 MEMORY 8
REG_FIELD = PCS1_AN000_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS1_AN001_RESULTS_REG 0x80011800B8001420 MEMORY 8
REG_FIELD = PCS1_AN001_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS1_AN002_RESULTS_REG 0x80011800B8001820 MEMORY 8
REG_FIELD = PCS1_AN002_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0
REG = PCS1_AN003_RESULTS_REG 0x80011800B8001C20 MEMORY 8
REG_FIELD = PCS1_AN003_RESULTS_REG pause 6 5, spd 4 3, an_cpt 2 2, dup 1 1, link_ok 0 0

REG = PCS0_INT000_EN_REG 0x80011800B0001088 MEMORY 8
REG_FIELD = PCS0_INT000_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS0_INT001_EN_REG 0x80011800B0001488 MEMORY 8
REG_FIELD = PCS0_INT001_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS0_INT002_EN_REG 0x80011800B0001888 MEMORY 8
REG_FIELD = PCS0_INT002_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS0_INT003_EN_REG 0x80011800B0001C88 MEMORY 8
REG_FIELD = PCS0_INT003_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS1_INT000_EN_REG 0x80011800B8001088 MEMORY 8
REG_FIELD = PCS1_INT000_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS1_INT001_EN_REG 0x80011800B8001488 MEMORY 8
REG_FIELD = PCS1_INT001_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS1_INT002_EN_REG 0x80011800B8001888 MEMORY 8
REG_FIELD = PCS1_INT002_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0
REG = PCS1_INT003_EN_REG 0x80011800B8001C88 MEMORY 8
REG_FIELD = PCS1_INT003_EN_REG dbg_sync_en 12 12, dup 11 11, sync_bad_en 10 10, an_bad_en 9 9, rxlock_en 8 8, rxbad_en 7 7, rxerr_en 6 6, txbad_en 5 5, txfifo_en 4 4, txfifu_en 3 3, an_err_en 2 2, xmit_en 1 1, lnkspd_en 0 0

REG = PCS0_INT000_REG 0x80011800B0001080 MEMORY 8
REG_FIELD = PCS0_INT000_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS0_INT001_REG 0x80011800B0001480 MEMORY 8
REG_FIELD = PCS0_INT001_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS0_INT002_REG 0x80011800B0001880 MEMORY 8
REG_FIELD = PCS0_INT002_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS0_INT003_REG 0x80011800B0001C80 MEMORY 8
REG_FIELD = PCS0_INT003_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS1_INT000_REG 0x80011800B8001080 MEMORY 8
REG_FIELD = PCS1_INT000_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS1_INT001_REG 0x80011800B8001480 MEMORY 8
REG_FIELD = PCS1_INT001_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS1_INT002_REG 0x80011800B8001880 MEMORY 8
REG_FIELD = PCS1_INT002_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0
REG = PCS1_INT003_REG 0x80011800B8001C80 MEMORY 8
REG_FIELD = PCS1_INT003_REG dbg_sync 12 12, dup 11 11, sync_bad 10 10, an_bad 9 9, rxlock 8 8, rxbad 7 7, rxerr 6 6, txbad 5 5, txfifo 4 4, txfifu 3 3, an_err 2 2, xmit 1 1, lnkspd 0 0

REG = PCS0_LINK000_TIMER_COUNT_REG 0x80011800B0001040 MEMORY 8
REG_FIELD = PCS0_LINK000_TIMER_COUNT_REG count 15 0
REG = PCS0_LINK001_TIMER_COUNT_REG 0x80011800B0001440 MEMORY 8
REG_FIELD = PCS0_LINK001_TIMER_COUNT_REG count 15 0
REG = PCS0_LINK002_TIMER_COUNT_REG 0x80011800B0001840 MEMORY 8
REG_FIELD = PCS0_LINK002_TIMER_COUNT_REG count 15 0
REG = PCS0_LINK003_TIMER_COUNT_REG 0x80011800B0001C40 MEMORY 8
REG_FIELD = PCS0_LINK003_TIMER_COUNT_REG count 15 0
REG = PCS1_LINK000_TIMER_COUNT_REG 0x80011800B8001040 MEMORY 8
REG_FIELD = PCS1_LINK000_TIMER_COUNT_REG count 15 0
REG = PCS1_LINK001_TIMER_COUNT_REG 0x80011800B8001440 MEMORY 8
REG_FIELD = PCS1_LINK001_TIMER_COUNT_REG count 15 0
REG = PCS1_LINK002_TIMER_COUNT_REG 0x80011800B8001840 MEMORY 8
REG_FIELD = PCS1_LINK002_TIMER_COUNT_REG count 15 0
REG = PCS1_LINK003_TIMER_COUNT_REG 0x80011800B8001C40 MEMORY 8
REG_FIELD = PCS1_LINK003_TIMER_COUNT_REG count 15 0

REG = PCS0_LOG_ANL000_REG 0x80011800B0001090 MEMORY 8
REG_FIELD = PCS0_LOG_ANL000_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS0_LOG_ANL001_REG 0x80011800B0001490 MEMORY 8
REG_FIELD = PCS0_LOG_ANL001_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS0_LOG_ANL002_REG 0x80011800B0001890 MEMORY 8
REG_FIELD = PCS0_LOG_ANL002_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS0_LOG_ANL003_REG 0x80011800B0001C90 MEMORY 8
REG_FIELD = PCS0_LOG_ANL003_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS1_LOG_ANL000_REG 0x80011800B8001090 MEMORY 8
REG_FIELD = PCS1_LOG_ANL000_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS1_LOG_ANL001_REG 0x80011800B8001490 MEMORY 8
REG_FIELD = PCS1_LOG_ANL001_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS1_LOG_ANL002_REG 0x80011800B8001890 MEMORY 8
REG_FIELD = PCS1_LOG_ANL002_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0
REG = PCS1_LOG_ANL003_REG 0x80011800B8001C90 MEMORY 8
REG_FIELD = PCS1_LOG_ANL003_REG lafifovfl 3 3, la_en 2 2, pkt_sz 1 0

REG = PCS0_MAC_CRDT_CNT000_REG 0x80011800B00010B0 MEMORY 8
REG_FIELD = PCS0_MAC_CRDT_CNT000_REG cnt 4 0
REG = PCS0_MAC_CRDT_CNT001_REG 0x80011800B00014B0 MEMORY 8
REG_FIELD = PCS0_MAC_CRDT_CNT001_REG cnt 4 0
REG = PCS0_MAC_CRDT_CNT002_REG 0x80011800B00018B0 MEMORY 8
REG_FIELD = PCS0_MAC_CRDT_CNT002_REG cnt 4 0
REG = PCS0_MAC_CRDT_CNT003_REG 0x80011800B0001CB0 MEMORY 8
REG_FIELD = PCS0_MAC_CRDT_CNT003_REG cnt 4 0
REG = PCS1_MAC_CRDT_CNT000_REG 0x80011800B80010B0 MEMORY 8
REG_FIELD = PCS1_MAC_CRDT_CNT000_REG cnt 4 0
REG = PCS1_MAC_CRDT_CNT001_REG 0x80011800B80014B0 MEMORY 8
REG_FIELD = PCS1_MAC_CRDT_CNT001_REG cnt 4 0
REG = PCS1_MAC_CRDT_CNT002_REG 0x80011800B80018B0 MEMORY 8
REG_FIELD = PCS1_MAC_CRDT_CNT002_REG cnt 4 0
REG = PCS1_MAC_CRDT_CNT003_REG 0x80011800B8001CB0 MEMORY 8
REG_FIELD = PCS1_MAC_CRDT_CNT003_REG cnt 4 0

REG = PCS0_MISC000_CTL_REG 0x80011800B0001078 MEMORY 8
REG_FIELD = PCS0_MISC000_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS0_MISC001_CTL_REG 0x80011800B0001478 MEMORY 8
REG_FIELD = PCS0_MISC001_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS0_MISC002_CTL_REG 0x80011800B0001878 MEMORY 8
REG_FIELD = PCS0_MISC002_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS0_MISC003_CTL_REG 0x80011800B0001C78 MEMORY 8
REG_FIELD = PCS0_MISC003_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS1_MISC000_CTL_REG 0x80011800B8001078 MEMORY 8
REG_FIELD = PCS1_MISC000_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS1_MISC001_CTL_REG 0x80011800B8001478 MEMORY 8
REG_FIELD = PCS1_MISC001_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS1_MISC002_CTL_REG 0x80011800B8001878 MEMORY 8
REG_FIELD = PCS1_MISC002_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0
REG = PCS1_MISC003_CTL_REG 0x80011800B8001C78 MEMORY 8
REG_FIELD = PCS1_MISC003_CTL_REG gmxeno 11 11, loopbck2 10 10, mac_phy 9 9, mode 8 8, an_ovrd 7 7, samp_pt 6 0

REG = PCS0_MR000_CONTROL_REG 0x80011800B0001000 MEMORY 8
REG_FIELD = PCS0_MR000_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS0_MR001_CONTROL_REG 0x80011800B0001400 MEMORY 8
REG_FIELD = PCS0_MR001_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS0_MR002_CONTROL_REG 0x80011800B0001800 MEMORY 8
REG_FIELD = PCS0_MR002_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS0_MR003_CONTROL_REG 0x80011800B0001C00 MEMORY 8
REG_FIELD = PCS0_MR003_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS1_MR000_CONTROL_REG 0x80011800B8001000 MEMORY 8
REG_FIELD = PCS1_MR000_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS1_MR001_CONTROL_REG 0x80011800B8001400 MEMORY 8
REG_FIELD = PCS1_MR001_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS1_MR002_CONTROL_REG 0x80011800B8001800 MEMORY 8
REG_FIELD = PCS1_MR002_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5
REG = PCS1_MR003_CONTROL_REG 0x80011800B8001C00 MEMORY 8
REG_FIELD = PCS1_MR003_CONTROL_REG reset 15 15, loopbck1 14 14, spdlsb 13 13, an_en 12 12, pwr_dn 11 11, rst_an 9 9, dup 8 8, coltst 7 7, spdmsb 6 6, uni 5 5

REG = PCS0_MR000_STATUS_REG 0x80011800B0001008 MEMORY 8
REG_FIELD = PCS0_MR000_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS0_MR001_STATUS_REG 0x80011800B0001408 MEMORY 8
REG_FIELD = PCS0_MR001_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS0_MR002_STATUS_REG 0x80011800B0001808 MEMORY 8
REG_FIELD = PCS0_MR002_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS0_MR003_STATUS_REG 0x80011800B0001C08 MEMORY 8
REG_FIELD = PCS0_MR003_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS1_MR000_STATUS_REG 0x80011800B8001008 MEMORY 8
REG_FIELD = PCS1_MR000_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS1_MR001_STATUS_REG 0x80011800B8001408 MEMORY 8
REG_FIELD = PCS1_MR001_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS1_MR002_STATUS_REG 0x80011800B8001808 MEMORY 8
REG_FIELD = PCS1_MR002_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0
REG = PCS1_MR003_STATUS_REG 0x80011800B8001C08 MEMORY 8
REG_FIELD = PCS1_MR003_STATUS_REG hun_t4 15 15, hun_xfd 14 14, hun_xhd 13 13, ten_fd 12 12, ten_hd 11 11, hun_t2fd 10 10, hun_t2hd 9 9, ext_st 8 8, prb_sup 6 6, an_cpt 5 5, rm_flt 4 4, an_abil 3 3, lnk_st 2 2, extnd 0 0

REG = PCS0_RX000_STATES_REG 0x80011800B0001058 MEMORY 8
REG_FIELD = PCS0_RX000_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS0_RX001_STATES_REG 0x80011800B0001458 MEMORY 8
REG_FIELD = PCS0_RX001_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS0_RX002_STATES_REG 0x80011800B0001858 MEMORY 8
REG_FIELD = PCS0_RX002_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS0_RX003_STATES_REG 0x80011800B0001C58 MEMORY 8
REG_FIELD = PCS0_RX003_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS1_RX000_STATES_REG 0x80011800B8001058 MEMORY 8
REG_FIELD = PCS1_RX000_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS1_RX001_STATES_REG 0x80011800B8001458 MEMORY 8
REG_FIELD = PCS1_RX001_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS1_RX002_STATES_REG 0x80011800B8001858 MEMORY 8
REG_FIELD = PCS1_RX002_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0
REG = PCS1_RX003_STATES_REG 0x80011800B8001C58 MEMORY 8
REG_FIELD = PCS1_RX003_STATES_REG rx_bad 15 15, rx_st 14 10, sync_bad 9 9, sync 8 5, an_bad 4 4, an_st 3 0

REG = PCS0_RX000_SYNC_REG 0x80011800B0001050 MEMORY 8
REG_FIELD = PCS0_RX000_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS0_RX001_SYNC_REG 0x80011800B0001450 MEMORY 8
REG_FIELD = PCS0_RX001_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS0_RX002_SYNC_REG 0x80011800B0001850 MEMORY 8
REG_FIELD = PCS0_RX002_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS0_RX003_SYNC_REG 0x80011800B0001C50 MEMORY 8
REG_FIELD = PCS0_RX003_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS1_RX000_SYNC_REG 0x80011800B8001050 MEMORY 8
REG_FIELD = PCS1_RX000_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS1_RX001_SYNC_REG 0x80011800B8001450 MEMORY 8
REG_FIELD = PCS1_RX001_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS1_RX002_SYNC_REG 0x80011800B8001850 MEMORY 8
REG_FIELD = PCS1_RX002_SYNC_REG sync 1 1, bit_lock 0 0
REG = PCS1_RX003_SYNC_REG 0x80011800B8001C50 MEMORY 8
REG_FIELD = PCS1_RX003_SYNC_REG sync 1 1, bit_lock 0 0

REG = PCS0_SERDES_CRDT_CNT000_REG 0x80011800B00010A0 MEMORY 8
REG_FIELD = PCS0_SERDES_CRDT_CNT000_REG cnt 4 0
REG = PCS0_SERDES_CRDT_CNT001_REG 0x80011800B00014A0 MEMORY 8
REG_FIELD = PCS0_SERDES_CRDT_CNT001_REG cnt 4 0
REG = PCS0_SERDES_CRDT_CNT002_REG 0x80011800B00018A0 MEMORY 8
REG_FIELD = PCS0_SERDES_CRDT_CNT002_REG cnt 4 0
REG = PCS0_SERDES_CRDT_CNT003_REG 0x80011800B0001CA0 MEMORY 8
REG_FIELD = PCS0_SERDES_CRDT_CNT003_REG cnt 4 0
REG = PCS1_SERDES_CRDT_CNT000_REG 0x80011800B80010A0 MEMORY 8
REG_FIELD = PCS1_SERDES_CRDT_CNT000_REG cnt 4 0
REG = PCS1_SERDES_CRDT_CNT001_REG 0x80011800B80014A0 MEMORY 8
REG_FIELD = PCS1_SERDES_CRDT_CNT001_REG cnt 4 0
REG = PCS1_SERDES_CRDT_CNT002_REG 0x80011800B80018A0 MEMORY 8
REG_FIELD = PCS1_SERDES_CRDT_CNT002_REG cnt 4 0
REG = PCS1_SERDES_CRDT_CNT003_REG 0x80011800B8001CA0 MEMORY 8
REG_FIELD = PCS1_SERDES_CRDT_CNT003_REG cnt 4 0

REG = PCS0_SGM000_AN_ADV_REG 0x80011800B0001068 MEMORY 8
REG_FIELD = PCS0_SGM000_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM001_AN_ADV_REG 0x80011800B0001468 MEMORY 8
REG_FIELD = PCS0_SGM001_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM002_AN_ADV_REG 0x80011800B0001868 MEMORY 8
REG_FIELD = PCS0_SGM002_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM003_AN_ADV_REG 0x80011800B0001C68 MEMORY 8
REG_FIELD = PCS0_SGM003_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM000_AN_ADV_REG 0x80011800B8001068 MEMORY 8
REG_FIELD = PCS1_SGM000_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM001_AN_ADV_REG 0x80011800B8001468 MEMORY 8
REG_FIELD = PCS1_SGM001_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM002_AN_ADV_REG 0x80011800B8001868 MEMORY 8
REG_FIELD = PCS1_SGM002_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM003_AN_ADV_REG 0x80011800B8001C68 MEMORY 8
REG_FIELD = PCS1_SGM003_AN_ADV_REG link 15 15, ack 14 14, dup 12 12, speed 11 10, one 0 0

REG = PCS0_SGM000_LP_ADV_REG 0x80011800B0001070 MEMORY 8
REG_FIELD = PCS0_SGM000_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM001_LP_ADV_REG 0x80011800B0001470 MEMORY 8
REG_FIELD = PCS0_SGM001_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM002_LP_ADV_REG 0x80011800B0001870 MEMORY 8
REG_FIELD = PCS0_SGM002_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS0_SGM003_LP_ADV_REG 0x80011800B0001C70 MEMORY 8
REG_FIELD = PCS0_SGM003_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM000_LP_ADV_REG 0x80011800B8001070 MEMORY 8
REG_FIELD = PCS1_SGM000_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM001_LP_ADV_REG 0x80011800B8001470 MEMORY 8
REG_FIELD = PCS1_SGM001_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM002_LP_ADV_REG 0x80011800B8001870 MEMORY 8
REG_FIELD = PCS1_SGM002_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0
REG = PCS1_SGM003_LP_ADV_REG 0x80011800B8001C70 MEMORY 8
REG_FIELD = PCS1_SGM003_LP_ADV_REG link 15 15, dup 12 12, speed 11 10, one 0 0

REG = PCS0_TX000_STATES_REG 0x80011800B0001060 MEMORY 8
REG_FIELD = PCS0_TX000_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS0_TX001_STATES_REG 0x80011800B0001460 MEMORY 8
REG_FIELD = PCS0_TX001_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS0_TX002_STATES_REG 0x80011800B0001860 MEMORY 8
REG_FIELD = PCS0_TX002_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS0_TX003_STATES_REG 0x80011800B0001C60 MEMORY 8
REG_FIELD = PCS0_TX003_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS1_TX000_STATES_REG 0x80011800B8001060 MEMORY 8
REG_FIELD = PCS1_TX000_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS1_TX001_STATES_REG 0x80011800B8001460 MEMORY 8
REG_FIELD = PCS1_TX001_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS1_TX002_STATES_REG 0x80011800B8001860 MEMORY 8
REG_FIELD = PCS1_TX002_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0
REG = PCS1_TX003_STATES_REG 0x80011800B8001C60 MEMORY 8
REG_FIELD = PCS1_TX003_STATES_REG xmit 6 5, tx_bad 4 4, ord_st 3 0

REG = PCS0_TX_RX000_POLARITY_REG 0x80011800B0001048 MEMORY 8
REG_FIELD = PCS0_TX_RX000_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS0_TX_RX001_POLARITY_REG 0x80011800B0001448 MEMORY 8
REG_FIELD = PCS0_TX_RX001_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS0_TX_RX002_POLARITY_REG 0x80011800B0001848 MEMORY 8
REG_FIELD = PCS0_TX_RX002_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS0_TX_RX003_POLARITY_REG 0x80011800B0001C48 MEMORY 8
REG_FIELD = PCS0_TX_RX003_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS1_TX_RX000_POLARITY_REG 0x80011800B8001048 MEMORY 8
REG_FIELD = PCS1_TX_RX000_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS1_TX_RX001_POLARITY_REG 0x80011800B8001448 MEMORY 8
REG_FIELD = PCS1_TX_RX001_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS1_TX_RX002_POLARITY_REG 0x80011800B8001848 MEMORY 8
REG_FIELD = PCS1_TX_RX002_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0
REG = PCS1_TX_RX003_POLARITY_REG 0x80011800B8001C48 MEMORY 8
REG_FIELD = PCS1_TX_RX003_POLARITY_REG rxovrd 3 3, autorxpl 2 2, rxplrt 1 1, txplrt 0 0

REG = PCSX0_10GBX_STATUS_REG 0x80011800B0000828 MEMORY 8
REG_FIELD = PCSX0_10GBX_STATUS_REG alignd 12 12, pattst 11 11, l3sync 3 3, l2sync 2 2, l1sync 1 1, l0sync 0 0

REG = PCSX0_BIST_STATUS_REG 0x80011800B0000870 MEMORY 8
REG_FIELD = PCSX0_BIST_STATUS_REG bist_status 0 0

REG = PCSX0_BIT_LOCK_STATUS_REG 0x80011800B0000850 MEMORY 8
REG_FIELD = PCSX0_BIT_LOCK_STATUS_REG bitlck3 3 3, bitlck2 2 2, bitlck1 1 1, bitlck0 0 0

REG = PCSX0_CONTROL1_REG 0x80011800B0000800 MEMORY 8
REG_FIELD = PCSX0_CONTROL1_REG reset 15 15, loopbck1 14 14, spdsel1 13 13, lo_pwr 11 11, spdsel0 6 6, spd 5 2

REG = PCSX0_CONTROL2_REG 0x80011800B0000818 MEMORY 8
REG_FIELD = PCSX0_CONTROL2_REG type 1 0

REG = PCSX0_INT_EN_REG 0x80011800B0000860 MEMORY 8
REG_FIELD = PCSX0_INT_EN_REG dbg_sync_en 6 6, algnlos_en 5 5, synlos_en 4 4, bitlckls_en 3 3, rxsynbad_en 2 2, rxbad_en 1 1, txflt_en 0 0

REG = PCSX0_INT_REG 0x80011800B0000858 MEMORY 8
REG_FIELD = PCSX0_INT_REG dbg_sync 6 6, algnlos 5 5, synlos 4 4, bitlckls 3 3, rxsynbad 2 2, rxbad 1 1, txflt 0 0

REG = PCSX0_LOG_ANL_REG 0x80011800B0000868 MEMORY 8
REG_FIELD = PCSX0_LOG_ANL_REG enc_mode 6 6, drop_ln 5 4, lafifovfl 3 3, la_en 2 2, pkt_sz 1 0

REG = PCSX0_MISC_CTL_REG 0x80011800B0000848 MEMORY 8
REG_FIELD = PCSX0_MISC_CTL_REG tx_swap 3 3, rx_swap 2 2, xaui 1 1, gmxeno 0 0

REG = PCSX0_RX_SYNC_STATES_REG 0x80011800B0000838 MEMORY 8
REG_FIELD = PCSX0_RX_SYNC_STATES_REG sync3st 15 12, sync2st 11 8, sync1st 7 4, sync0st 3 0

REG = PCSX0_SERDES_CRDT_CNT_REG 0x80011800B0000880 MEMORY 8
REG_FIELD = PCSX0_SERDES_CRDT_CNT_REG cnt 4 0

REG = PCSX0_SPD_ABIL_REG 0x80011800B0000810 MEMORY 8
REG_FIELD = PCSX0_SPD_ABIL_REG tenpasst 1 1, tengb 0 0

REG = PCSX0_STATUS1_REG 0x80011800B0000808 MEMORY 8
REG_FIELD = PCSX0_STATUS1_REG flt 7 7, rcv_lnk 2 2, lpable 1 1

REG = PCSX0_STATUS2_REG 0x80011800B0000820 MEMORY 8
REG_FIELD = PCSX0_STATUS2_REG dev 15 14, xmtflt 11 11, rcvflt 10 10, tengb_w 2 2, tengb_x 1 1, tengb_r 0 0

REG = PCSX0_TX_RX_POLARITY_REG 0x80011800B0000840 MEMORY 8
REG_FIELD = PCSX0_TX_RX_POLARITY_REG xor_rxplrt 9 6, xor_txplrt 5 2, rxplrt 1 1, txplrt 0 0

REG = PCSX0_TX_RX_STATES_REG 0x80011800B0000830 MEMORY 8
REG_FIELD = PCSX0_TX_RX_STATES_REG term_err 13 13, syn3bad 12 12, syn2bad 11 11, syn1bad 10 10, syn0bad 9 9, rxbad 8 8, algn_st 7 5, rx_st 4 3, tx_st 2 0

REG = PEM0_BAR1_INDEX000 0x80011800C0000100 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX000 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX001 0x80011800C0000108 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX001 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX002 0x80011800C0000110 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX002 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX003 0x80011800C0000118 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX003 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX004 0x80011800C0000120 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX004 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX005 0x80011800C0000128 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX005 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX006 0x80011800C0000130 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX006 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX007 0x80011800C0000138 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX007 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX008 0x80011800C0000140 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX008 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX009 0x80011800C0000148 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX009 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX010 0x80011800C0000150 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX010 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX011 0x80011800C0000158 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX011 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX012 0x80011800C0000160 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX012 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX013 0x80011800C0000168 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX013 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX014 0x80011800C0000170 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX014 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM0_BAR1_INDEX015 0x80011800C0000178 MEMORY 8
REG_FIELD = PEM0_BAR1_INDEX015 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX000 0x80011800C1000100 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX000 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX001 0x80011800C1000108 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX001 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX002 0x80011800C1000110 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX002 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX003 0x80011800C1000118 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX003 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX004 0x80011800C1000120 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX004 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX005 0x80011800C1000128 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX005 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX006 0x80011800C1000130 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX006 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX007 0x80011800C1000138 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX007 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX008 0x80011800C1000140 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX008 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX009 0x80011800C1000148 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX009 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX010 0x80011800C1000150 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX010 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX011 0x80011800C1000158 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX011 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX012 0x80011800C1000160 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX012 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX013 0x80011800C1000168 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX013 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX014 0x80011800C1000170 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX014 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM1_BAR1_INDEX015 0x80011800C1000178 MEMORY 8
REG_FIELD = PEM1_BAR1_INDEX015 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX000 0x80011800C2000100 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX000 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX001 0x80011800C2000108 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX001 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX002 0x80011800C2000110 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX002 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX003 0x80011800C2000118 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX003 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX004 0x80011800C2000120 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX004 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX005 0x80011800C2000128 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX005 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX006 0x80011800C2000130 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX006 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX007 0x80011800C2000138 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX007 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX008 0x80011800C2000140 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX008 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX009 0x80011800C2000148 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX009 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX010 0x80011800C2000150 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX010 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX011 0x80011800C2000158 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX011 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX012 0x80011800C2000160 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX012 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX013 0x80011800C2000168 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX013 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX014 0x80011800C2000170 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX014 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0
REG = PEM2_BAR1_INDEX015 0x80011800C2000178 MEMORY 8
REG_FIELD = PEM2_BAR1_INDEX015 addr_idx 23 4, ca 3 3, end_swp 2 1, addr_v 0 0

REG = PEM0_BAR2_MASK 0x80011800C00000B0 MEMORY 8
REG_FIELD = PEM0_BAR2_MASK mask 37 3
REG = PEM1_BAR2_MASK 0x80011800C10000B0 MEMORY 8
REG_FIELD = PEM1_BAR2_MASK mask 37 3
REG = PEM2_BAR2_MASK 0x80011800C20000B0 MEMORY 8
REG_FIELD = PEM2_BAR2_MASK mask 37 3

REG = PEM0_BAR_CTL 0x80011800C00000A8 MEMORY 8
REG_FIELD = PEM0_BAR_CTL bar1_siz 6 4, bar2_enb 3 3, bar2_esx 2 1, bar2_cax 0 0
REG = PEM1_BAR_CTL 0x80011800C10000A8 MEMORY 8
REG_FIELD = PEM1_BAR_CTL bar1_siz 6 4, bar2_enb 3 3, bar2_esx 2 1, bar2_cax 0 0
REG = PEM2_BAR_CTL 0x80011800C20000A8 MEMORY 8
REG_FIELD = PEM2_BAR_CTL bar1_siz 6 4, bar2_enb 3 3, bar2_esx 2 1, bar2_cax 0 0

REG = PEM0_BIST_STATUS 0x80011800C0000018 MEMORY 8
REG_FIELD = PEM0_BIST_STATUS retry 5 5, sot 4 4, rqhdr0 3 3, rqhdr1 2 2, rqdata0 1 1, rqdata1 0 0
REG = PEM1_BIST_STATUS 0x80011800C1000018 MEMORY 8
REG_FIELD = PEM1_BIST_STATUS retry 5 5, sot 4 4, rqhdr0 3 3, rqhdr1 2 2, rqdata0 1 1, rqdata1 0 0
REG = PEM2_BIST_STATUS 0x80011800C2000018 MEMORY 8
REG_FIELD = PEM2_BIST_STATUS retry 5 5, sot 4 4, rqhdr0 3 3, rqhdr1 2 2, rqdata0 1 1, rqdata1 0 0

REG = PEM0_BIST_STATUS2 0x80011800C0000440 MEMORY 8
REG_FIELD = PEM0_BIST_STATUS2 peai_p2e 13 13, tlpn_d 12 12, tlpn_ctl 11 11, tlpp_d 10 10, tlpp_ctl 9 9, tlpc_d 8 8, tlpc_ctl 7 7, tlpan_d 6 6, tlpan_ctl 5 5, tlpap_d 4 4, tlpap_ctl 3 3, tlpac_d 2 2, tlpac_ctl 1 1, m2s 0 0
REG = PEM1_BIST_STATUS2 0x80011800C1000440 MEMORY 8
REG_FIELD = PEM1_BIST_STATUS2 peai_p2e 13 13, tlpn_d 12 12, tlpn_ctl 11 11, tlpp_d 10 10, tlpp_ctl 9 9, tlpc_d 8 8, tlpc_ctl 7 7, tlpan_d 6 6, tlpan_ctl 5 5, tlpap_d 4 4, tlpap_ctl 3 3, tlpac_d 2 2, tlpac_ctl 1 1, m2s 0 0
REG = PEM2_BIST_STATUS2 0x80011800C2000440 MEMORY 8
REG_FIELD = PEM2_BIST_STATUS2 peai_p2e 13 13, tlpn_d 12 12, tlpn_ctl 11 11, tlpp_d 10 10, tlpp_ctl 9 9, tlpc_d 8 8, tlpc_ctl 7 7, tlpan_d 6 6, tlpan_ctl 5 5, tlpap_d 4 4, tlpap_ctl 3 3, tlpac_d 2 2, tlpac_ctl 1 1, m2s 0 0

REG = PEM0_CFG 0x80011800C0000410 MEMORY 8
REG_FIELD = PEM0_CFG laneswap 4 4, hostmd 3 3, md 2 0
REG = PEM1_CFG 0x80011800C1000410 MEMORY 8
REG_FIELD = PEM1_CFG laneswap 4 4, hostmd 3 3, md 2 0
REG = PEM2_CFG 0x80011800C2000410 MEMORY 8
REG_FIELD = PEM2_CFG laneswap 4 4, hostmd 3 3, md 2 0

REG = PEM0_CFG_RD 0x80011800C0000030 MEMORY 8
REG_FIELD = PEM0_CFG_RD data 63 32, addr 31 0
REG = PEM1_CFG_RD 0x80011800C1000030 MEMORY 8
REG_FIELD = PEM1_CFG_RD data 63 32, addr 31 0
REG = PEM2_CFG_RD 0x80011800C2000030 MEMORY 8
REG_FIELD = PEM2_CFG_RD data 63 32, addr 31 0

REG = PEM0_CFG_WR 0x80011800C0000028 MEMORY 8
REG_FIELD = PEM0_CFG_WR data 63 32, addr 31 0
REG = PEM1_CFG_WR 0x80011800C1000028 MEMORY 8
REG_FIELD = PEM1_CFG_WR data 63 32, addr 31 0
REG = PEM2_CFG_WR 0x80011800C2000028 MEMORY 8
REG_FIELD = PEM2_CFG_WR data 63 32, addr 31 0

REG = PEM0_CLK_EN 0x80011800C0000400 MEMORY 8
REG_FIELD = PEM0_CLK_EN pceclk_gate 1 1, csclk_gate 0 0
REG = PEM1_CLK_EN 0x80011800C1000400 MEMORY 8
REG_FIELD = PEM1_CLK_EN pceclk_gate 1 1, csclk_gate 0 0
REG = PEM2_CLK_EN 0x80011800C2000400 MEMORY 8
REG_FIELD = PEM2_CLK_EN pceclk_gate 1 1, csclk_gate 0 0

REG = PEM0_CPL_LUT_VALID 0x80011800C0000098 MEMORY 8
REG_FIELD = PEM0_CPL_LUT_VALID tag 31 0
REG = PEM1_CPL_LUT_VALID 0x80011800C1000098 MEMORY 8
REG_FIELD = PEM1_CPL_LUT_VALID tag 31 0
REG = PEM2_CPL_LUT_VALID 0x80011800C2000098 MEMORY 8
REG_FIELD = PEM2_CPL_LUT_VALID tag 31 0

REG = PEM0_CTL_STATUS 0x80011800C0000000 MEMORY 8
REG_FIELD = PEM0_CTL_STATUS inv_dpar 50 50, inv_hpar 49 49, inv_rpar 48 48, auto_sd 47 47, dnum 46 42, pbus 41 34, cfg_rtry 31 16, pm_xtoff 11 11, pm_xpme 10 10, ob_p_cmd 9 9, nf_ecrc 6 6, dly_one 5 5, lnk_enb 4 4, ro_ctlp 3 3, fast_lm 2 2, inv_ecrc 1 1, inv_lcrc 0 0
REG = PEM1_CTL_STATUS 0x80011800C1000000 MEMORY 8
REG_FIELD = PEM1_CTL_STATUS inv_dpar 50 50, inv_hpar 49 49, inv_rpar 48 48, auto_sd 47 47, dnum 46 42, pbus 41 34, cfg_rtry 31 16, pm_xtoff 11 11, pm_xpme 10 10, ob_p_cmd 9 9, nf_ecrc 6 6, dly_one 5 5, lnk_enb 4 4, ro_ctlp 3 3, fast_lm 2 2, inv_ecrc 1 1, inv_lcrc 0 0
REG = PEM2_CTL_STATUS 0x80011800C2000000 MEMORY 8
REG_FIELD = PEM2_CTL_STATUS inv_dpar 50 50, inv_hpar 49 49, inv_rpar 48 48, auto_sd 47 47, dnum 46 42, pbus 41 34, cfg_rtry 31 16, pm_xtoff 11 11, pm_xpme 10 10, ob_p_cmd 9 9, nf_ecrc 6 6, dly_one 5 5, lnk_enb 4 4, ro_ctlp 3 3, fast_lm 2 2, inv_ecrc 1 1, inv_lcrc 0 0

REG = PEM0_DBG_INFO 0x80011800C00000D0 MEMORY 8
REG_FIELD = PEM0_DBG_INFO c_c_dbe 45 45, c_c_sbe 44 44, c_d_dbe 43 43, c_d_sbe 42 42, n_c_dbe 41 41, n_c_sbe 40 40, n_d_dbe 39 39, n_d_sbe 38 38, p_c_dbe 37 37, p_c_sbe 36 36, p_d_dbe 35 35, p_d_sbe 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0
REG = PEM1_DBG_INFO 0x80011800C10000D0 MEMORY 8
REG_FIELD = PEM1_DBG_INFO c_c_dbe 45 45, c_c_sbe 44 44, c_d_dbe 43 43, c_d_sbe 42 42, n_c_dbe 41 41, n_c_sbe 40 40, n_d_dbe 39 39, n_d_sbe 38 38, p_c_dbe 37 37, p_c_sbe 36 36, p_d_dbe 35 35, p_d_sbe 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0
REG = PEM2_DBG_INFO 0x80011800C20000D0 MEMORY 8
REG_FIELD = PEM2_DBG_INFO c_c_dbe 45 45, c_c_sbe 44 44, c_d_dbe 43 43, c_d_sbe 42 42, n_c_dbe 41 41, n_c_sbe 40 40, n_d_dbe 39 39, n_d_sbe 38 38, p_c_dbe 37 37, p_c_sbe 36 36, p_d_dbe 35 35, p_d_sbe 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0

REG = PEM0_DBG_INFO_EN 0x80011800C00000A0 MEMORY 8
REG_FIELD = PEM0_DBG_INFO_EN tpcdbe1 45 45, tpcsbe1 44 44, tpcdbe0 43 43, tpcsbe0 42 42, tnfdbe1 41 41, tnfsbe1 40 40, tnfdbe0 39 39, tnfsbe0 38 38, tpfdbe1 37 37, tpfsbe1 36 36, tpfdbe0 35 35, tpfsbe0 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0
REG = PEM1_DBG_INFO_EN 0x80011800C10000A0 MEMORY 8
REG_FIELD = PEM1_DBG_INFO_EN tpcdbe1 45 45, tpcsbe1 44 44, tpcdbe0 43 43, tpcsbe0 42 42, tnfdbe1 41 41, tnfsbe1 40 40, tnfdbe0 39 39, tnfsbe0 38 38, tpfdbe1 37 37, tpfsbe1 36 36, tpfdbe0 35 35, tpfsbe0 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0
REG = PEM2_DBG_INFO_EN 0x80011800C20000A0 MEMORY 8
REG_FIELD = PEM2_DBG_INFO_EN tpcdbe1 45 45, tpcsbe1 44 44, tpcdbe0 43 43, tpcsbe0 42 42, tnfdbe1 41 41, tnfsbe1 40 40, tnfdbe0 39 39, tnfsbe0 38 38, tpfdbe1 37 37, tpfsbe1 36 36, tpfdbe0 35 35, tpfsbe0 34 34, datq_pe 33 33, hdrq_pe 32 32, rtry_pe 31 31, ecrc_e 30 30, rawwpp 29 29, racpp 28 28, ramtlp 27 27, rarwdns 26 26, caar 25 25, racca 24 24, racur 23 23, rauc 22 22, rqo 21 21, fcuv 20 20, rpe 19 19, fcpvwt 18 18, dpeoosd 17 17, rtwdle 16 16, rdwdle 15 15, mre 14 14, rte 13 13, acto 12 12, rvdm 11 11, rumep 10 10, rptamrc 9 9, rpmerc 8 8, rfemrc 7 7, rnfemrc 6 6, rcemrc 5 5, rpoison 4 4, recrce 3 3, rtlplle 2 2, rtlpmal 1 1, spoison 0 0

REG = PEM0_DIAG_STATUS 0x80011800C0000020 MEMORY 8
REG_FIELD = PEM0_DIAG_STATUS pm_dst 5 3, pm_stat 2 2, pm_en 1 1, aux_en 0 0
REG = PEM1_DIAG_STATUS 0x80011800C1000020 MEMORY 8
REG_FIELD = PEM1_DIAG_STATUS pm_dst 5 3, pm_stat 2 2, pm_en 1 1, aux_en 0 0
REG = PEM2_DIAG_STATUS 0x80011800C2000020 MEMORY 8
REG_FIELD = PEM2_DIAG_STATUS pm_dst 5 3, pm_stat 2 2, pm_en 1 1, aux_en 0 0

REG = PEM0_ECC_ENA 0x80011800C00000C0 MEMORY 8
REG_FIELD = PEM0_ECC_ENA tlp_nc_ena 5 5, tlp_nd_ena 4 4, tlp_pc_ena 3 3, tlp_pd_ena 2 2, tlp_cc_ena 1 1, tlp_cd_ena 0 0
REG = PEM1_ECC_ENA 0x80011800C10000C0 MEMORY 8
REG_FIELD = PEM1_ECC_ENA tlp_nc_ena 5 5, tlp_nd_ena 4 4, tlp_pc_ena 3 3, tlp_pd_ena 2 2, tlp_cc_ena 1 1, tlp_cd_ena 0 0
REG = PEM2_ECC_ENA 0x80011800C20000C0 MEMORY 8
REG_FIELD = PEM2_ECC_ENA tlp_nc_ena 5 5, tlp_nd_ena 4 4, tlp_pc_ena 3 3, tlp_pd_ena 2 2, tlp_cc_ena 1 1, tlp_cd_ena 0 0

REG = PEM0_ECC_SYND_CTRL 0x80011800C00000C8 MEMORY 8
REG_FIELD = PEM0_ECC_SYND_CTRL tlp_nc_syn 11 10, tlp_nd_syn 9 8, tlp_pc_syn 7 6, tlp_pd_syn 5 4, tlp_cc_syn 3 2, tlp_cd_syn 1 0
REG = PEM1_ECC_SYND_CTRL 0x80011800C10000C8 MEMORY 8
REG_FIELD = PEM1_ECC_SYND_CTRL tlp_nc_syn 11 10, tlp_nd_syn 9 8, tlp_pc_syn 7 6, tlp_pd_syn 5 4, tlp_cc_syn 3 2, tlp_cd_syn 1 0
REG = PEM2_ECC_SYND_CTRL 0x80011800C20000C8 MEMORY 8
REG_FIELD = PEM2_ECC_SYND_CTRL tlp_nc_syn 11 10, tlp_nd_syn 9 8, tlp_pc_syn 7 6, tlp_pd_syn 5 4, tlp_cc_syn 3 2, tlp_cd_syn 1 0

REG = PEM0_INB_READ_CREDITS 0x80011800C00000B8 MEMORY 8
REG_FIELD = PEM0_INB_READ_CREDITS num 5 0
REG = PEM1_INB_READ_CREDITS 0x80011800C10000B8 MEMORY 8
REG_FIELD = PEM1_INB_READ_CREDITS num 5 0
REG = PEM2_INB_READ_CREDITS 0x80011800C20000B8 MEMORY 8
REG_FIELD = PEM2_INB_READ_CREDITS num 5 0

REG = PEM0_INT_ENB 0x80011800C0000430 MEMORY 8
REG_FIELD = PEM0_INT_ENB crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM1_INT_ENB 0x80011800C1000430 MEMORY 8
REG_FIELD = PEM1_INT_ENB crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM2_INT_ENB 0x80011800C2000430 MEMORY 8
REG_FIELD = PEM2_INT_ENB crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0

REG = PEM0_INT_ENB_INT 0x80011800C0000438 MEMORY 8
REG_FIELD = PEM0_INT_ENB_INT crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM1_INT_ENB_INT 0x80011800C1000438 MEMORY 8
REG_FIELD = PEM1_INT_ENB_INT crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM2_INT_ENB_INT 0x80011800C2000438 MEMORY 8
REG_FIELD = PEM2_INT_ENB_INT crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0

REG = PEM0_INT_SUM 0x80011800C0000428 MEMORY 8
REG_FIELD = PEM0_INT_SUM crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM1_INT_SUM 0x80011800C1000428 MEMORY 8
REG_FIELD = PEM1_INT_SUM crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0
REG = PEM2_INT_SUM 0x80011800C2000428 MEMORY 8
REG_FIELD = PEM2_INT_SUM crs_dr 13 13, crs_er 12 12, rdlk 11 11, exc 10 10, un_bx 9 9, un_b2 8 8, un_b1 7 7, up_bx 6 6, up_b2 5 5, up_b1 4 4, pmem 3 3, pmei 2 2, se 1 1, aeri 0 0

REG = PEM0_ON 0x80011800C0000420 MEMORY 8
REG_FIELD = PEM0_ON pemoor 1 1, pemon 0 0
REG = PEM1_ON 0x80011800C1000420 MEMORY 8
REG_FIELD = PEM1_ON pemoor 1 1, pemon 0 0
REG = PEM2_ON 0x80011800C2000420 MEMORY 8
REG_FIELD = PEM2_ON pemoor 1 1, pemon 0 0

REG = PEM0_P2N_BAR0_START 0x80011800C0000080 MEMORY 8
REG_FIELD = PEM0_P2N_BAR0_START addr 63 14
REG = PEM1_P2N_BAR0_START 0x80011800C1000080 MEMORY 8
REG_FIELD = PEM1_P2N_BAR0_START addr 63 14
REG = PEM2_P2N_BAR0_START 0x80011800C2000080 MEMORY 8
REG_FIELD = PEM2_P2N_BAR0_START addr 63 14

REG = PEM0_P2N_BAR1_START 0x80011800C0000088 MEMORY 8
REG_FIELD = PEM0_P2N_BAR1_START addr 63 26
REG = PEM1_P2N_BAR1_START 0x80011800C1000088 MEMORY 8
REG_FIELD = PEM1_P2N_BAR1_START addr 63 26
REG = PEM2_P2N_BAR1_START 0x80011800C2000088 MEMORY 8
REG_FIELD = PEM2_P2N_BAR1_START addr 63 26

REG = PEM0_P2N_BAR2_START 0x80011800C0000090 MEMORY 8
REG_FIELD = PEM0_P2N_BAR2_START addr 63 41
REG = PEM1_P2N_BAR2_START 0x80011800C1000090 MEMORY 8
REG_FIELD = PEM1_P2N_BAR2_START addr 63 41
REG = PEM2_P2N_BAR2_START 0x80011800C2000090 MEMORY 8
REG_FIELD = PEM2_P2N_BAR2_START addr 63 41

REG = PEM0_SPI_CTL 0x80011800C0000180 MEMORY 8
REG_FIELD = PEM0_SPI_CTL start_busy 13 13, tvalid 12 12, cmd 11 9, adr 8 0
REG = PEM1_SPI_CTL 0x80011800C1000180 MEMORY 8
REG_FIELD = PEM1_SPI_CTL start_busy 13 13, tvalid 12 12, cmd 11 9, adr 8 0
REG = PEM2_SPI_CTL 0x80011800C2000180 MEMORY 8
REG_FIELD = PEM2_SPI_CTL start_busy 13 13, tvalid 12 12, cmd 11 9, adr 8 0

REG = PEM0_SPI_DATA 0x80011800C0000188 MEMORY 8
REG_FIELD = PEM0_SPI_DATA preamble 63 48, cs2 44 44, adr 43 32, data 31 0
REG = PEM1_SPI_DATA 0x80011800C1000188 MEMORY 8
REG_FIELD = PEM1_SPI_DATA preamble 63 48, cs2 44 44, adr 43 32, data 31 0
REG = PEM2_SPI_DATA 0x80011800C2000188 MEMORY 8
REG_FIELD = PEM2_SPI_DATA preamble 63 48, cs2 44 44, adr 43 32, data 31 0

REG = PEM0_STRAP 0x80011800C0000408 MEMORY 8
REG_FIELD = PEM0_STRAP pilaneswap 3 3, pimode 2 0
REG = PEM1_STRAP 0x80011800C1000408 MEMORY 8
REG_FIELD = PEM1_STRAP pilaneswap 3 3, pimode 2 0
REG = PEM2_STRAP 0x80011800C2000408 MEMORY 8
REG_FIELD = PEM2_STRAP pilaneswap 3 3, pimode 2 0

REG = PEM0_TLP_CREDITS 0x80011800C0000038 MEMORY 8
REG_FIELD = PEM0_TLP_CREDITS peai_ppf 55 48, sli_cpl 23 16, sli_np 15 8, sli_p 7 0
REG = PEM1_TLP_CREDITS 0x80011800C1000038 MEMORY 8
REG_FIELD = PEM1_TLP_CREDITS peai_ppf 55 48, sli_cpl 23 16, sli_np 15 8, sli_p 7 0
REG = PEM2_TLP_CREDITS 0x80011800C2000038 MEMORY 8
REG_FIELD = PEM2_TLP_CREDITS peai_ppf 55 48, sli_cpl 23 16, sli_np 15 8, sli_p 7 0

REG = PIP_ALT_SKIP_CFG0 0x80011800A0002A00 MEMORY 8
REG_FIELD = PIP_ALT_SKIP_CFG0 len 56 56, bit1 45 40, bit0 37 32, skip3 22 16, skip2 14 8, skip1 6 0
REG = PIP_ALT_SKIP_CFG1 0x80011800A0002A08 MEMORY 8
REG_FIELD = PIP_ALT_SKIP_CFG1 len 56 56, bit1 45 40, bit0 37 32, skip3 22 16, skip2 14 8, skip1 6 0
REG = PIP_ALT_SKIP_CFG2 0x80011800A0002A10 MEMORY 8
REG_FIELD = PIP_ALT_SKIP_CFG2 len 56 56, bit1 45 40, bit0 37 32, skip3 22 16, skip2 14 8, skip1 6 0
REG = PIP_ALT_SKIP_CFG3 0x80011800A0002A18 MEMORY 8
REG_FIELD = PIP_ALT_SKIP_CFG3 len 56 56, bit1 45 40, bit0 37 32, skip3 22 16, skip2 14 8, skip1 6 0

REG = PIP_BCK_PRS 0x80011800A0000038 MEMORY 8
REG_FIELD = PIP_BCK_PRS bckprs 63 63, hiwater 12 8, lowater 4 0

REG = PIP_BIST_STATUS 0x80011800A0000000 MEMORY 8
REG_FIELD = PIP_BIST_STATUS bist 19 0

REG = PIP_BSEL_EXT_CFG0 0x80011800A0002800 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_CFG0 upper_tag 55 40, tag 39 32, offset 24 16, skip 6 0
REG = PIP_BSEL_EXT_CFG1 0x80011800A0002810 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_CFG1 upper_tag 55 40, tag 39 32, offset 24 16, skip 6 0
REG = PIP_BSEL_EXT_CFG2 0x80011800A0002820 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_CFG2 upper_tag 55 40, tag 39 32, offset 24 16, skip 6 0
REG = PIP_BSEL_EXT_CFG3 0x80011800A0002830 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_CFG3 upper_tag 55 40, tag 39 32, offset 24 16, skip 6 0

REG = PIP_BSEL_EXT_POS0 0x80011800A0002808 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_POS0 pos7_val 63 63, pos7 62 56, pos6_val 55 55, pos6 54 48, pos5_val 47 47, pos5 46 40, pos4_val 39 39, pos4 38 32, pos3_val 31 31, pos3 30 24, pos2_val 23 23, pos2 22 16, pos1_val 15 15, pos1 14 8, pos0_val 7 7, pos0 6 0
REG = PIP_BSEL_EXT_POS1 0x80011800A0002818 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_POS1 pos7_val 63 63, pos7 62 56, pos6_val 55 55, pos6 54 48, pos5_val 47 47, pos5 46 40, pos4_val 39 39, pos4 38 32, pos3_val 31 31, pos3 30 24, pos2_val 23 23, pos2 22 16, pos1_val 15 15, pos1 14 8, pos0_val 7 7, pos0 6 0
REG = PIP_BSEL_EXT_POS2 0x80011800A0002828 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_POS2 pos7_val 63 63, pos7 62 56, pos6_val 55 55, pos6 54 48, pos5_val 47 47, pos5 46 40, pos4_val 39 39, pos4 38 32, pos3_val 31 31, pos3 30 24, pos2_val 23 23, pos2 22 16, pos1_val 15 15, pos1 14 8, pos0_val 7 7, pos0 6 0
REG = PIP_BSEL_EXT_POS3 0x80011800A0002838 MEMORY 8
REG_FIELD = PIP_BSEL_EXT_POS3 pos7_val 63 63, pos7 62 56, pos6_val 55 55, pos6 54 48, pos5_val 47 47, pos5 46 40, pos4_val 39 39, pos4 38 32, pos3_val 31 31, pos3 30 24, pos2_val 23 23, pos2 22 16, pos1_val 15 15, pos1 14 8, pos0_val 7 7, pos0 6 0

REG = PIP_BSEL_TBL_ENT0 0x80011800A0003000 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT0 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT1 0x80011800A0003008 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT1 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT2 0x80011800A0003010 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT2 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT3 0x80011800A0003018 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT3 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT4 0x80011800A0003020 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT4 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT5 0x80011800A0003028 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT5 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT6 0x80011800A0003030 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT6 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT7 0x80011800A0003038 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT7 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT8 0x80011800A0003040 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT8 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT9 0x80011800A0003048 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT9 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT10 0x80011800A0003050 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT10 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT11 0x80011800A0003058 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT11 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT12 0x80011800A0003060 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT12 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT13 0x80011800A0003068 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT13 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT14 0x80011800A0003070 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT14 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT15 0x80011800A0003078 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT15 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT16 0x80011800A0003080 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT16 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT17 0x80011800A0003088 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT17 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT18 0x80011800A0003090 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT18 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT19 0x80011800A0003098 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT19 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT20 0x80011800A00030A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT20 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT21 0x80011800A00030A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT21 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT22 0x80011800A00030B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT22 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT23 0x80011800A00030B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT23 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT24 0x80011800A00030C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT24 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT25 0x80011800A00030C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT25 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT26 0x80011800A00030D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT26 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT27 0x80011800A00030D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT27 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT28 0x80011800A00030E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT28 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT29 0x80011800A00030E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT29 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT30 0x80011800A00030F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT30 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT31 0x80011800A00030F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT31 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT32 0x80011800A0003100 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT32 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT33 0x80011800A0003108 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT33 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT34 0x80011800A0003110 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT34 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT35 0x80011800A0003118 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT35 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT36 0x80011800A0003120 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT36 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT37 0x80011800A0003128 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT37 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT38 0x80011800A0003130 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT38 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT39 0x80011800A0003138 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT39 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT40 0x80011800A0003140 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT40 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT41 0x80011800A0003148 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT41 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT42 0x80011800A0003150 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT42 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT43 0x80011800A0003158 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT43 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT44 0x80011800A0003160 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT44 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT45 0x80011800A0003168 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT45 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT46 0x80011800A0003170 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT46 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT47 0x80011800A0003178 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT47 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT48 0x80011800A0003180 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT48 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT49 0x80011800A0003188 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT49 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT50 0x80011800A0003190 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT50 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT51 0x80011800A0003198 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT51 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT52 0x80011800A00031A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT52 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT53 0x80011800A00031A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT53 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT54 0x80011800A00031B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT54 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT55 0x80011800A00031B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT55 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT56 0x80011800A00031C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT56 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT57 0x80011800A00031C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT57 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT58 0x80011800A00031D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT58 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT59 0x80011800A00031D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT59 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT60 0x80011800A00031E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT60 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT61 0x80011800A00031E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT61 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT62 0x80011800A00031F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT62 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT63 0x80011800A00031F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT63 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT64 0x80011800A0003200 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT64 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT65 0x80011800A0003208 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT65 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT66 0x80011800A0003210 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT66 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT67 0x80011800A0003218 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT67 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT68 0x80011800A0003220 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT68 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT69 0x80011800A0003228 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT69 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT70 0x80011800A0003230 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT70 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT71 0x80011800A0003238 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT71 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT72 0x80011800A0003240 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT72 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT73 0x80011800A0003248 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT73 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT74 0x80011800A0003250 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT74 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT75 0x80011800A0003258 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT75 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT76 0x80011800A0003260 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT76 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT77 0x80011800A0003268 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT77 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT78 0x80011800A0003270 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT78 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT79 0x80011800A0003278 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT79 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT80 0x80011800A0003280 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT80 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT81 0x80011800A0003288 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT81 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT82 0x80011800A0003290 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT82 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT83 0x80011800A0003298 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT83 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT84 0x80011800A00032A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT84 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT85 0x80011800A00032A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT85 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT86 0x80011800A00032B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT86 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT87 0x80011800A00032B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT87 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT88 0x80011800A00032C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT88 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT89 0x80011800A00032C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT89 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT90 0x80011800A00032D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT90 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT91 0x80011800A00032D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT91 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT92 0x80011800A00032E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT92 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT93 0x80011800A00032E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT93 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT94 0x80011800A00032F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT94 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT95 0x80011800A00032F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT95 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT96 0x80011800A0003300 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT96 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT97 0x80011800A0003308 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT97 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT98 0x80011800A0003310 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT98 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT99 0x80011800A0003318 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT99 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT100 0x80011800A0003320 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT100 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT101 0x80011800A0003328 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT101 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT102 0x80011800A0003330 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT102 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT103 0x80011800A0003338 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT103 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT104 0x80011800A0003340 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT104 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT105 0x80011800A0003348 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT105 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT106 0x80011800A0003350 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT106 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT107 0x80011800A0003358 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT107 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT108 0x80011800A0003360 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT108 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT109 0x80011800A0003368 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT109 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT110 0x80011800A0003370 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT110 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT111 0x80011800A0003378 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT111 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT112 0x80011800A0003380 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT112 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT113 0x80011800A0003388 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT113 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT114 0x80011800A0003390 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT114 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT115 0x80011800A0003398 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT115 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT116 0x80011800A00033A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT116 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT117 0x80011800A00033A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT117 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT118 0x80011800A00033B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT118 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT119 0x80011800A00033B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT119 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT120 0x80011800A00033C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT120 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT121 0x80011800A00033C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT121 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT122 0x80011800A00033D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT122 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT123 0x80011800A00033D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT123 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT124 0x80011800A00033E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT124 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT125 0x80011800A00033E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT125 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT126 0x80011800A00033F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT126 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT127 0x80011800A00033F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT127 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT128 0x80011800A0003400 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT128 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT129 0x80011800A0003408 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT129 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT130 0x80011800A0003410 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT130 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT131 0x80011800A0003418 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT131 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT132 0x80011800A0003420 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT132 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT133 0x80011800A0003428 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT133 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT134 0x80011800A0003430 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT134 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT135 0x80011800A0003438 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT135 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT136 0x80011800A0003440 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT136 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT137 0x80011800A0003448 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT137 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT138 0x80011800A0003450 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT138 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT139 0x80011800A0003458 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT139 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT140 0x80011800A0003460 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT140 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT141 0x80011800A0003468 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT141 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT142 0x80011800A0003470 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT142 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT143 0x80011800A0003478 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT143 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT144 0x80011800A0003480 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT144 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT145 0x80011800A0003488 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT145 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT146 0x80011800A0003490 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT146 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT147 0x80011800A0003498 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT147 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT148 0x80011800A00034A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT148 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT149 0x80011800A00034A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT149 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT150 0x80011800A00034B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT150 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT151 0x80011800A00034B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT151 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT152 0x80011800A00034C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT152 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT153 0x80011800A00034C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT153 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT154 0x80011800A00034D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT154 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT155 0x80011800A00034D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT155 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT156 0x80011800A00034E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT156 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT157 0x80011800A00034E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT157 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT158 0x80011800A00034F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT158 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT159 0x80011800A00034F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT159 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT160 0x80011800A0003500 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT160 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT161 0x80011800A0003508 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT161 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT162 0x80011800A0003510 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT162 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT163 0x80011800A0003518 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT163 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT164 0x80011800A0003520 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT164 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT165 0x80011800A0003528 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT165 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT166 0x80011800A0003530 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT166 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT167 0x80011800A0003538 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT167 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT168 0x80011800A0003540 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT168 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT169 0x80011800A0003548 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT169 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT170 0x80011800A0003550 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT170 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT171 0x80011800A0003558 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT171 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT172 0x80011800A0003560 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT172 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT173 0x80011800A0003568 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT173 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT174 0x80011800A0003570 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT174 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT175 0x80011800A0003578 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT175 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT176 0x80011800A0003580 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT176 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT177 0x80011800A0003588 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT177 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT178 0x80011800A0003590 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT178 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT179 0x80011800A0003598 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT179 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT180 0x80011800A00035A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT180 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT181 0x80011800A00035A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT181 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT182 0x80011800A00035B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT182 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT183 0x80011800A00035B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT183 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT184 0x80011800A00035C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT184 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT185 0x80011800A00035C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT185 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT186 0x80011800A00035D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT186 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT187 0x80011800A00035D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT187 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT188 0x80011800A00035E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT188 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT189 0x80011800A00035E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT189 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT190 0x80011800A00035F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT190 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT191 0x80011800A00035F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT191 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT192 0x80011800A0003600 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT192 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT193 0x80011800A0003608 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT193 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT194 0x80011800A0003610 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT194 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT195 0x80011800A0003618 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT195 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT196 0x80011800A0003620 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT196 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT197 0x80011800A0003628 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT197 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT198 0x80011800A0003630 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT198 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT199 0x80011800A0003638 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT199 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT200 0x80011800A0003640 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT200 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT201 0x80011800A0003648 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT201 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT202 0x80011800A0003650 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT202 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT203 0x80011800A0003658 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT203 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT204 0x80011800A0003660 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT204 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT205 0x80011800A0003668 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT205 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT206 0x80011800A0003670 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT206 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT207 0x80011800A0003678 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT207 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT208 0x80011800A0003680 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT208 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT209 0x80011800A0003688 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT209 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT210 0x80011800A0003690 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT210 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT211 0x80011800A0003698 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT211 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT212 0x80011800A00036A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT212 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT213 0x80011800A00036A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT213 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT214 0x80011800A00036B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT214 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT215 0x80011800A00036B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT215 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT216 0x80011800A00036C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT216 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT217 0x80011800A00036C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT217 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT218 0x80011800A00036D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT218 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT219 0x80011800A00036D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT219 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT220 0x80011800A00036E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT220 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT221 0x80011800A00036E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT221 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT222 0x80011800A00036F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT222 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT223 0x80011800A00036F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT223 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT224 0x80011800A0003700 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT224 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT225 0x80011800A0003708 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT225 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT226 0x80011800A0003710 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT226 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT227 0x80011800A0003718 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT227 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT228 0x80011800A0003720 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT228 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT229 0x80011800A0003728 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT229 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT230 0x80011800A0003730 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT230 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT231 0x80011800A0003738 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT231 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT232 0x80011800A0003740 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT232 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT233 0x80011800A0003748 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT233 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT234 0x80011800A0003750 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT234 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT235 0x80011800A0003758 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT235 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT236 0x80011800A0003760 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT236 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT237 0x80011800A0003768 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT237 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT238 0x80011800A0003770 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT238 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT239 0x80011800A0003778 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT239 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT240 0x80011800A0003780 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT240 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT241 0x80011800A0003788 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT241 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT242 0x80011800A0003790 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT242 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT243 0x80011800A0003798 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT243 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT244 0x80011800A00037A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT244 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT245 0x80011800A00037A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT245 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT246 0x80011800A00037B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT246 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT247 0x80011800A00037B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT247 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT248 0x80011800A00037C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT248 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT249 0x80011800A00037C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT249 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT250 0x80011800A00037D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT250 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT251 0x80011800A00037D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT251 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT252 0x80011800A00037E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT252 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT253 0x80011800A00037E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT253 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT254 0x80011800A00037F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT254 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT255 0x80011800A00037F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT255 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT256 0x80011800A0003800 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT256 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT257 0x80011800A0003808 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT257 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT258 0x80011800A0003810 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT258 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT259 0x80011800A0003818 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT259 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT260 0x80011800A0003820 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT260 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT261 0x80011800A0003828 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT261 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT262 0x80011800A0003830 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT262 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT263 0x80011800A0003838 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT263 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT264 0x80011800A0003840 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT264 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT265 0x80011800A0003848 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT265 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT266 0x80011800A0003850 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT266 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT267 0x80011800A0003858 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT267 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT268 0x80011800A0003860 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT268 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT269 0x80011800A0003868 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT269 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT270 0x80011800A0003870 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT270 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT271 0x80011800A0003878 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT271 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT272 0x80011800A0003880 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT272 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT273 0x80011800A0003888 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT273 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT274 0x80011800A0003890 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT274 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT275 0x80011800A0003898 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT275 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT276 0x80011800A00038A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT276 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT277 0x80011800A00038A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT277 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT278 0x80011800A00038B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT278 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT279 0x80011800A00038B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT279 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT280 0x80011800A00038C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT280 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT281 0x80011800A00038C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT281 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT282 0x80011800A00038D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT282 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT283 0x80011800A00038D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT283 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT284 0x80011800A00038E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT284 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT285 0x80011800A00038E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT285 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT286 0x80011800A00038F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT286 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT287 0x80011800A00038F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT287 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT288 0x80011800A0003900 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT288 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT289 0x80011800A0003908 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT289 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT290 0x80011800A0003910 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT290 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT291 0x80011800A0003918 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT291 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT292 0x80011800A0003920 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT292 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT293 0x80011800A0003928 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT293 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT294 0x80011800A0003930 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT294 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT295 0x80011800A0003938 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT295 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT296 0x80011800A0003940 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT296 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT297 0x80011800A0003948 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT297 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT298 0x80011800A0003950 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT298 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT299 0x80011800A0003958 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT299 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT300 0x80011800A0003960 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT300 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT301 0x80011800A0003968 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT301 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT302 0x80011800A0003970 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT302 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT303 0x80011800A0003978 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT303 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT304 0x80011800A0003980 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT304 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT305 0x80011800A0003988 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT305 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT306 0x80011800A0003990 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT306 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT307 0x80011800A0003998 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT307 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT308 0x80011800A00039A0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT308 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT309 0x80011800A00039A8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT309 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT310 0x80011800A00039B0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT310 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT311 0x80011800A00039B8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT311 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT312 0x80011800A00039C0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT312 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT313 0x80011800A00039C8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT313 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT314 0x80011800A00039D0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT314 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT315 0x80011800A00039D8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT315 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT316 0x80011800A00039E0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT316 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT317 0x80011800A00039E8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT317 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT318 0x80011800A00039F0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT318 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT319 0x80011800A00039F8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT319 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT320 0x80011800A0003A00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT320 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT321 0x80011800A0003A08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT321 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT322 0x80011800A0003A10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT322 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT323 0x80011800A0003A18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT323 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT324 0x80011800A0003A20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT324 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT325 0x80011800A0003A28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT325 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT326 0x80011800A0003A30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT326 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT327 0x80011800A0003A38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT327 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT328 0x80011800A0003A40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT328 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT329 0x80011800A0003A48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT329 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT330 0x80011800A0003A50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT330 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT331 0x80011800A0003A58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT331 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT332 0x80011800A0003A60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT332 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT333 0x80011800A0003A68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT333 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT334 0x80011800A0003A70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT334 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT335 0x80011800A0003A78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT335 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT336 0x80011800A0003A80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT336 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT337 0x80011800A0003A88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT337 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT338 0x80011800A0003A90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT338 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT339 0x80011800A0003A98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT339 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT340 0x80011800A0003AA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT340 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT341 0x80011800A0003AA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT341 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT342 0x80011800A0003AB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT342 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT343 0x80011800A0003AB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT343 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT344 0x80011800A0003AC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT344 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT345 0x80011800A0003AC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT345 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT346 0x80011800A0003AD0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT346 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT347 0x80011800A0003AD8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT347 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT348 0x80011800A0003AE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT348 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT349 0x80011800A0003AE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT349 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT350 0x80011800A0003AF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT350 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT351 0x80011800A0003AF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT351 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT352 0x80011800A0003B00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT352 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT353 0x80011800A0003B08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT353 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT354 0x80011800A0003B10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT354 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT355 0x80011800A0003B18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT355 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT356 0x80011800A0003B20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT356 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT357 0x80011800A0003B28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT357 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT358 0x80011800A0003B30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT358 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT359 0x80011800A0003B38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT359 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT360 0x80011800A0003B40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT360 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT361 0x80011800A0003B48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT361 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT362 0x80011800A0003B50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT362 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT363 0x80011800A0003B58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT363 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT364 0x80011800A0003B60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT364 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT365 0x80011800A0003B68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT365 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT366 0x80011800A0003B70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT366 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT367 0x80011800A0003B78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT367 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT368 0x80011800A0003B80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT368 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT369 0x80011800A0003B88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT369 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT370 0x80011800A0003B90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT370 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT371 0x80011800A0003B98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT371 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT372 0x80011800A0003BA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT372 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT373 0x80011800A0003BA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT373 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT374 0x80011800A0003BB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT374 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT375 0x80011800A0003BB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT375 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT376 0x80011800A0003BC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT376 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT377 0x80011800A0003BC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT377 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT378 0x80011800A0003BD0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT378 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT379 0x80011800A0003BD8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT379 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT380 0x80011800A0003BE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT380 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT381 0x80011800A0003BE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT381 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT382 0x80011800A0003BF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT382 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT383 0x80011800A0003BF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT383 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT384 0x80011800A0003C00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT384 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT385 0x80011800A0003C08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT385 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT386 0x80011800A0003C10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT386 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT387 0x80011800A0003C18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT387 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT388 0x80011800A0003C20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT388 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT389 0x80011800A0003C28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT389 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT390 0x80011800A0003C30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT390 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT391 0x80011800A0003C38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT391 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT392 0x80011800A0003C40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT392 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT393 0x80011800A0003C48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT393 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT394 0x80011800A0003C50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT394 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT395 0x80011800A0003C58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT395 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT396 0x80011800A0003C60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT396 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT397 0x80011800A0003C68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT397 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT398 0x80011800A0003C70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT398 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT399 0x80011800A0003C78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT399 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT400 0x80011800A0003C80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT400 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT401 0x80011800A0003C88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT401 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT402 0x80011800A0003C90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT402 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT403 0x80011800A0003C98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT403 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT404 0x80011800A0003CA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT404 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT405 0x80011800A0003CA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT405 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT406 0x80011800A0003CB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT406 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT407 0x80011800A0003CB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT407 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT408 0x80011800A0003CC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT408 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT409 0x80011800A0003CC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT409 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT410 0x80011800A0003CD0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT410 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT411 0x80011800A0003CD8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT411 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT412 0x80011800A0003CE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT412 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT413 0x80011800A0003CE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT413 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT414 0x80011800A0003CF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT414 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT415 0x80011800A0003CF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT415 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT416 0x80011800A0003D00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT416 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT417 0x80011800A0003D08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT417 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT418 0x80011800A0003D10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT418 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT419 0x80011800A0003D18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT419 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT420 0x80011800A0003D20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT420 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT421 0x80011800A0003D28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT421 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT422 0x80011800A0003D30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT422 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT423 0x80011800A0003D38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT423 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT424 0x80011800A0003D40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT424 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT425 0x80011800A0003D48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT425 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT426 0x80011800A0003D50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT426 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT427 0x80011800A0003D58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT427 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT428 0x80011800A0003D60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT428 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT429 0x80011800A0003D68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT429 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT430 0x80011800A0003D70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT430 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT431 0x80011800A0003D78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT431 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT432 0x80011800A0003D80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT432 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT433 0x80011800A0003D88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT433 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT434 0x80011800A0003D90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT434 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT435 0x80011800A0003D98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT435 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT436 0x80011800A0003DA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT436 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT437 0x80011800A0003DA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT437 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT438 0x80011800A0003DB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT438 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT439 0x80011800A0003DB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT439 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT440 0x80011800A0003DC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT440 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT441 0x80011800A0003DC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT441 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT442 0x80011800A0003DD0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT442 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT443 0x80011800A0003DD8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT443 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT444 0x80011800A0003DE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT444 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT445 0x80011800A0003DE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT445 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT446 0x80011800A0003DF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT446 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT447 0x80011800A0003DF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT447 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT448 0x80011800A0003E00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT448 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT449 0x80011800A0003E08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT449 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT450 0x80011800A0003E10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT450 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT451 0x80011800A0003E18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT451 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT452 0x80011800A0003E20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT452 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT453 0x80011800A0003E28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT453 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT454 0x80011800A0003E30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT454 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT455 0x80011800A0003E38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT455 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT456 0x80011800A0003E40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT456 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT457 0x80011800A0003E48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT457 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT458 0x80011800A0003E50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT458 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT459 0x80011800A0003E58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT459 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT460 0x80011800A0003E60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT460 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT461 0x80011800A0003E68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT461 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT462 0x80011800A0003E70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT462 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT463 0x80011800A0003E78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT463 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT464 0x80011800A0003E80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT464 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT465 0x80011800A0003E88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT465 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT466 0x80011800A0003E90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT466 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT467 0x80011800A0003E98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT467 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT468 0x80011800A0003EA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT468 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT469 0x80011800A0003EA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT469 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT470 0x80011800A0003EB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT470 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT471 0x80011800A0003EB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT471 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT472 0x80011800A0003EC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT472 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT473 0x80011800A0003EC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT473 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT474 0x80011800A0003ED0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT474 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT475 0x80011800A0003ED8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT475 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT476 0x80011800A0003EE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT476 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT477 0x80011800A0003EE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT477 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT478 0x80011800A0003EF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT478 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT479 0x80011800A0003EF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT479 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT480 0x80011800A0003F00 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT480 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT481 0x80011800A0003F08 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT481 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT482 0x80011800A0003F10 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT482 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT483 0x80011800A0003F18 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT483 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT484 0x80011800A0003F20 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT484 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT485 0x80011800A0003F28 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT485 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT486 0x80011800A0003F30 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT486 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT487 0x80011800A0003F38 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT487 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT488 0x80011800A0003F40 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT488 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT489 0x80011800A0003F48 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT489 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT490 0x80011800A0003F50 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT490 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT491 0x80011800A0003F58 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT491 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT492 0x80011800A0003F60 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT492 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT493 0x80011800A0003F68 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT493 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT494 0x80011800A0003F70 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT494 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT495 0x80011800A0003F78 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT495 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT496 0x80011800A0003F80 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT496 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT497 0x80011800A0003F88 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT497 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT498 0x80011800A0003F90 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT498 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT499 0x80011800A0003F98 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT499 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT500 0x80011800A0003FA0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT500 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT501 0x80011800A0003FA8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT501 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT502 0x80011800A0003FB0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT502 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT503 0x80011800A0003FB8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT503 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT504 0x80011800A0003FC0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT504 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT505 0x80011800A0003FC8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT505 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT506 0x80011800A0003FD0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT506 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT507 0x80011800A0003FD8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT507 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT508 0x80011800A0003FE0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT508 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT509 0x80011800A0003FE8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT509 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT510 0x80011800A0003FF0 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT510 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0
REG = PIP_BSEL_TBL_ENT511 0x80011800A0003FF8 MEMORY 8
REG_FIELD = PIP_BSEL_TBL_ENT511 tag_en 63 63, grp_en 62 62, tt_en 61 61, qos_en 60 60, tag 39 32, grp 19 16, tt 9 8, qos 2 0

REG = PIP_CLKEN 0x80011800A0000040 MEMORY 8
REG_FIELD = PIP_CLKEN clken 0 0

REG = PIP_DEC_IPSEC0 0x80011800A0000080 MEMORY 8
REG_FIELD = PIP_DEC_IPSEC0 tcp 17 17, udp 16 16, dprt 15 0
REG = PIP_DEC_IPSEC1 0x80011800A0000088 MEMORY 8
REG_FIELD = PIP_DEC_IPSEC1 tcp 17 17, udp 16 16, dprt 15 0
REG = PIP_DEC_IPSEC2 0x80011800A0000090 MEMORY 8
REG_FIELD = PIP_DEC_IPSEC2 tcp 17 17, udp 16 16, dprt 15 0
REG = PIP_DEC_IPSEC3 0x80011800A0000098 MEMORY 8
REG_FIELD = PIP_DEC_IPSEC3 tcp 17 17, udp 16 16, dprt 15 0

REG = PIP_DSA_SRC_GRP 0x80011800A0000190 MEMORY 8
REG_FIELD = PIP_DSA_SRC_GRP map15 63 60, map14 59 56, map13 55 52, map12 51 48, map11 47 44, map10 43 40, map9 39 36, map8 35 32, map7 31 28, map6 27 24, map5 23 20, map4 19 16, map3 15 12, map2 11 8, map1 7 4, map0 3 0

REG = PIP_DSA_VID_GRP 0x80011800A0000198 MEMORY 8
REG_FIELD = PIP_DSA_VID_GRP map15 63 60, map14 59 56, map13 55 52, map12 51 48, map11 47 44, map10 43 40, map9 39 36, map8 35 32, map7 31 28, map6 27 24, map5 23 20, map4 19 16, map3 15 12, map2 11 8, map1 7 4, map0 3 0

REG = PIP_FRM_LEN_CHK0 0x80011800A0000180 MEMORY 8
REG_FIELD = PIP_FRM_LEN_CHK0 maxlen 31 16, minlen 15 0

REG = PIP_GBL_CFG 0x80011800A0000028 MEMORY 8
REG_FIELD = PIP_GBL_CFG tag_syn 18 18, ip6_udp 17 17, max_l2 16 16, raw_shf 10 8, nip_shf 2 0

REG = PIP_GBL_CTL 0x80011800A0000020 MEMORY 8
REG_FIELD = PIP_GBL_CTL ihmsk_dis 27 27, dsa_grp_tvid 26 26, dsa_grp_scmd 25 25, dsa_grp_sid 24 24, ring_en 20 20, ignrs 16 16, vs_wqe 15 15, vs_qos 14 14, l2_mal 13 13, tcp_flag 12 12, l4_len 11 11, l4_chk 10 10, l4_prt 9 9, l4_mal 8 8, ip6_eext 5 4, ip4_opts 3 3, ip_hop 2 2, ip_mal 1 1, ip_chk 0 0

REG = PIP_HG_PRI_QOS 0x80011800A00001A0 MEMORY 8
REG_FIELD = PIP_HG_PRI_QOS up_qos 12 12, qos 10 8, pri 5 0

REG = PIP_INT_EN 0x80011800A0000010 MEMORY 8
REG_FIELD = PIP_INT_EN punyerr 12 12, lenerr 11 11, maxerr 10 10, minerr 9 9, beperr 8 8, feperr 7 7, todoovr 6 6, skprunt 5 5, badtag 4 4, prtnxa 3 3, bckprs 2 2, crcerr 1 1, pktdrp 0 0

REG = PIP_INT_REG 0x80011800A0000008 MEMORY 8
REG_FIELD = PIP_INT_REG punyerr 12 12, lenerr 11 11, maxerr 10 10, minerr 9 9, beperr 8 8, feperr 7 7, todoovr 6 6, skprunt 5 5, badtag 4 4, prtnxa 3 3, bckprs 2 2, crcerr 1 1, pktdrp 0 0

REG = PIP_IP_OFFSET 0x80011800A0000060 MEMORY 8
REG_FIELD = PIP_IP_OFFSET offset 2 0

REG = PIP_PRT_CFG0 0x80011800A0000200 MEMORY 8
REG_FIELD = PIP_PRT_CFG0 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG1 0x80011800A0000208 MEMORY 8
REG_FIELD = PIP_PRT_CFG1 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG2 0x80011800A0000210 MEMORY 8
REG_FIELD = PIP_PRT_CFG2 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG3 0x80011800A0000218 MEMORY 8
REG_FIELD = PIP_PRT_CFG3 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG16 0x80011800A0000280 MEMORY 8
REG_FIELD = PIP_PRT_CFG16 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG17 0x80011800A0000288 MEMORY 8
REG_FIELD = PIP_PRT_CFG17 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG18 0x80011800A0000290 MEMORY 8
REG_FIELD = PIP_PRT_CFG18 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG19 0x80011800A0000298 MEMORY 8
REG_FIELD = PIP_PRT_CFG19 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG24 0x80011800A00002C0 MEMORY 8
REG_FIELD = PIP_PRT_CFG24 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG32 0x80011800A0000300 MEMORY 8
REG_FIELD = PIP_PRT_CFG32 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG33 0x80011800A0000308 MEMORY 8
REG_FIELD = PIP_PRT_CFG33 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG34 0x80011800A0000310 MEMORY 8
REG_FIELD = PIP_PRT_CFG34 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG35 0x80011800A0000318 MEMORY 8
REG_FIELD = PIP_PRT_CFG35 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG36 0x80011800A0000320 MEMORY 8
REG_FIELD = PIP_PRT_CFG36 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG37 0x80011800A0000328 MEMORY 8
REG_FIELD = PIP_PRT_CFG37 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG38 0x80011800A0000330 MEMORY 8
REG_FIELD = PIP_PRT_CFG38 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0
REG = PIP_PRT_CFG39 0x80011800A0000338 MEMORY 8
REG_FIELD = PIP_PRT_CFG39 pad_len 52 52, vlan_len 51 51, lenerr_en 50 50, maxerr_en 49 49, minerr_en 48 48, grp_wat_47 47 44, qos_wat_47 43 40, rawdrp 36 36, tag_inc 35 34, dyn_rs 33 33, inst_hdr 32 32, grp_wat 31 28, hg_qos 27 27, qos 26 24, qos_wat 23 20, qos_vsel 19 19, qos_vod 18 18, qos_diff 17 17, qos_vlan 16 16, crc_en 12 12, higig_en 11 11, dsa_en 10 10, mode 9 8, skip 6 0

REG = PIP_PRT_CFGB0 0x80011800A0008000 MEMORY 8
REG_FIELD = PIP_PRT_CFGB0 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB1 0x80011800A0008008 MEMORY 8
REG_FIELD = PIP_PRT_CFGB1 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB2 0x80011800A0008010 MEMORY 8
REG_FIELD = PIP_PRT_CFGB2 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB3 0x80011800A0008018 MEMORY 8
REG_FIELD = PIP_PRT_CFGB3 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB16 0x80011800A0008080 MEMORY 8
REG_FIELD = PIP_PRT_CFGB16 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB17 0x80011800A0008088 MEMORY 8
REG_FIELD = PIP_PRT_CFGB17 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB18 0x80011800A0008090 MEMORY 8
REG_FIELD = PIP_PRT_CFGB18 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB19 0x80011800A0008098 MEMORY 8
REG_FIELD = PIP_PRT_CFGB19 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB24 0x80011800A00080C0 MEMORY 8
REG_FIELD = PIP_PRT_CFGB24 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB32 0x80011800A0008100 MEMORY 8
REG_FIELD = PIP_PRT_CFGB32 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB33 0x80011800A0008108 MEMORY 8
REG_FIELD = PIP_PRT_CFGB33 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB34 0x80011800A0008110 MEMORY 8
REG_FIELD = PIP_PRT_CFGB34 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB35 0x80011800A0008118 MEMORY 8
REG_FIELD = PIP_PRT_CFGB35 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB36 0x80011800A0008120 MEMORY 8
REG_FIELD = PIP_PRT_CFGB36 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB37 0x80011800A0008128 MEMORY 8
REG_FIELD = PIP_PRT_CFGB37 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB38 0x80011800A0008130 MEMORY 8
REG_FIELD = PIP_PRT_CFGB38 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32
REG = PIP_PRT_CFGB39 0x80011800A0008138 MEMORY 8
REG_FIELD = PIP_PRT_CFGB39 alt_skp_sel 38 37, alt_skp_en 36 36, bsel_num 34 33, bsel_en 32 32

REG = PIP_PRT_TAG0 0x80011800A0000400 MEMORY 8
REG_FIELD = PIP_PRT_TAG0 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG1 0x80011800A0000408 MEMORY 8
REG_FIELD = PIP_PRT_TAG1 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG2 0x80011800A0000410 MEMORY 8
REG_FIELD = PIP_PRT_TAG2 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG3 0x80011800A0000418 MEMORY 8
REG_FIELD = PIP_PRT_TAG3 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG16 0x80011800A0000480 MEMORY 8
REG_FIELD = PIP_PRT_TAG16 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG17 0x80011800A0000488 MEMORY 8
REG_FIELD = PIP_PRT_TAG17 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG18 0x80011800A0000490 MEMORY 8
REG_FIELD = PIP_PRT_TAG18 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG19 0x80011800A0000498 MEMORY 8
REG_FIELD = PIP_PRT_TAG19 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG24 0x80011800A00004C0 MEMORY 8
REG_FIELD = PIP_PRT_TAG24 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG32 0x80011800A0000500 MEMORY 8
REG_FIELD = PIP_PRT_TAG32 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG33 0x80011800A0000508 MEMORY 8
REG_FIELD = PIP_PRT_TAG33 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG34 0x80011800A0000510 MEMORY 8
REG_FIELD = PIP_PRT_TAG34 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG35 0x80011800A0000518 MEMORY 8
REG_FIELD = PIP_PRT_TAG35 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG36 0x80011800A0000520 MEMORY 8
REG_FIELD = PIP_PRT_TAG36 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG37 0x80011800A0000528 MEMORY 8
REG_FIELD = PIP_PRT_TAG37 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG38 0x80011800A0000530 MEMORY 8
REG_FIELD = PIP_PRT_TAG38 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0
REG = PIP_PRT_TAG39 0x80011800A0000538 MEMORY 8
REG_FIELD = PIP_PRT_TAG39 grptagbase 39 36, grptagmask 35 32, grptag 31 31, grptag_mskip 30 30, tag_mode 29 28, inc_vs 27 26, inc_vlan 25 25, inc_prt_flag 24 24, ip6_dprt_flag 23 23, ip4_dprt_flag 22 22, ip6_sprt_flag 21 21, ip4_sprt_flag 20 20, ip6_nxth_flag 19 19, ip4_pctl_flag 18 18, ip6_dst_flag 17 17, ip4_dst_flag 16 16, ip6_src_flag 15 15, ip4_src_flag 14 14, tcp6_tag_type 13 12, tcp4_tag_type 11 10, ip6_tag_type 9 8, ip4_tag_type 7 6, non_tag_type 5 4, grp 3 0

REG = PIP_QOS_DIFF0 0x80011800A0000600 MEMORY 8
REG_FIELD = PIP_QOS_DIFF0 qos 2 0
REG = PIP_QOS_DIFF1 0x80011800A0000608 MEMORY 8
REG_FIELD = PIP_QOS_DIFF1 qos 2 0
REG = PIP_QOS_DIFF2 0x80011800A0000610 MEMORY 8
REG_FIELD = PIP_QOS_DIFF2 qos 2 0
REG = PIP_QOS_DIFF3 0x80011800A0000618 MEMORY 8
REG_FIELD = PIP_QOS_DIFF3 qos 2 0
REG = PIP_QOS_DIFF4 0x80011800A0000620 MEMORY 8
REG_FIELD = PIP_QOS_DIFF4 qos 2 0
REG = PIP_QOS_DIFF5 0x80011800A0000628 MEMORY 8
REG_FIELD = PIP_QOS_DIFF5 qos 2 0
REG = PIP_QOS_DIFF6 0x80011800A0000630 MEMORY 8
REG_FIELD = PIP_QOS_DIFF6 qos 2 0
REG = PIP_QOS_DIFF7 0x80011800A0000638 MEMORY 8
REG_FIELD = PIP_QOS_DIFF7 qos 2 0
REG = PIP_QOS_DIFF8 0x80011800A0000640 MEMORY 8
REG_FIELD = PIP_QOS_DIFF8 qos 2 0
REG = PIP_QOS_DIFF9 0x80011800A0000648 MEMORY 8
REG_FIELD = PIP_QOS_DIFF9 qos 2 0
REG = PIP_QOS_DIFF10 0x80011800A0000650 MEMORY 8
REG_FIELD = PIP_QOS_DIFF10 qos 2 0
REG = PIP_QOS_DIFF11 0x80011800A0000658 MEMORY 8
REG_FIELD = PIP_QOS_DIFF11 qos 2 0
REG = PIP_QOS_DIFF12 0x80011800A0000660 MEMORY 8
REG_FIELD = PIP_QOS_DIFF12 qos 2 0
REG = PIP_QOS_DIFF13 0x80011800A0000668 MEMORY 8
REG_FIELD = PIP_QOS_DIFF13 qos 2 0
REG = PIP_QOS_DIFF14 0x80011800A0000670 MEMORY 8
REG_FIELD = PIP_QOS_DIFF14 qos 2 0
REG = PIP_QOS_DIFF15 0x80011800A0000678 MEMORY 8
REG_FIELD = PIP_QOS_DIFF15 qos 2 0
REG = PIP_QOS_DIFF16 0x80011800A0000680 MEMORY 8
REG_FIELD = PIP_QOS_DIFF16 qos 2 0
REG = PIP_QOS_DIFF17 0x80011800A0000688 MEMORY 8
REG_FIELD = PIP_QOS_DIFF17 qos 2 0
REG = PIP_QOS_DIFF18 0x80011800A0000690 MEMORY 8
REG_FIELD = PIP_QOS_DIFF18 qos 2 0
REG = PIP_QOS_DIFF19 0x80011800A0000698 MEMORY 8
REG_FIELD = PIP_QOS_DIFF19 qos 2 0
REG = PIP_QOS_DIFF20 0x80011800A00006A0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF20 qos 2 0
REG = PIP_QOS_DIFF21 0x80011800A00006A8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF21 qos 2 0
REG = PIP_QOS_DIFF22 0x80011800A00006B0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF22 qos 2 0
REG = PIP_QOS_DIFF23 0x80011800A00006B8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF23 qos 2 0
REG = PIP_QOS_DIFF24 0x80011800A00006C0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF24 qos 2 0
REG = PIP_QOS_DIFF25 0x80011800A00006C8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF25 qos 2 0
REG = PIP_QOS_DIFF26 0x80011800A00006D0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF26 qos 2 0
REG = PIP_QOS_DIFF27 0x80011800A00006D8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF27 qos 2 0
REG = PIP_QOS_DIFF28 0x80011800A00006E0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF28 qos 2 0
REG = PIP_QOS_DIFF29 0x80011800A00006E8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF29 qos 2 0
REG = PIP_QOS_DIFF30 0x80011800A00006F0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF30 qos 2 0
REG = PIP_QOS_DIFF31 0x80011800A00006F8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF31 qos 2 0
REG = PIP_QOS_DIFF32 0x80011800A0000700 MEMORY 8
REG_FIELD = PIP_QOS_DIFF32 qos 2 0
REG = PIP_QOS_DIFF33 0x80011800A0000708 MEMORY 8
REG_FIELD = PIP_QOS_DIFF33 qos 2 0
REG = PIP_QOS_DIFF34 0x80011800A0000710 MEMORY 8
REG_FIELD = PIP_QOS_DIFF34 qos 2 0
REG = PIP_QOS_DIFF35 0x80011800A0000718 MEMORY 8
REG_FIELD = PIP_QOS_DIFF35 qos 2 0
REG = PIP_QOS_DIFF36 0x80011800A0000720 MEMORY 8
REG_FIELD = PIP_QOS_DIFF36 qos 2 0
REG = PIP_QOS_DIFF37 0x80011800A0000728 MEMORY 8
REG_FIELD = PIP_QOS_DIFF37 qos 2 0
REG = PIP_QOS_DIFF38 0x80011800A0000730 MEMORY 8
REG_FIELD = PIP_QOS_DIFF38 qos 2 0
REG = PIP_QOS_DIFF39 0x80011800A0000738 MEMORY 8
REG_FIELD = PIP_QOS_DIFF39 qos 2 0
REG = PIP_QOS_DIFF40 0x80011800A0000740 MEMORY 8
REG_FIELD = PIP_QOS_DIFF40 qos 2 0
REG = PIP_QOS_DIFF41 0x80011800A0000748 MEMORY 8
REG_FIELD = PIP_QOS_DIFF41 qos 2 0
REG = PIP_QOS_DIFF42 0x80011800A0000750 MEMORY 8
REG_FIELD = PIP_QOS_DIFF42 qos 2 0
REG = PIP_QOS_DIFF43 0x80011800A0000758 MEMORY 8
REG_FIELD = PIP_QOS_DIFF43 qos 2 0
REG = PIP_QOS_DIFF44 0x80011800A0000760 MEMORY 8
REG_FIELD = PIP_QOS_DIFF44 qos 2 0
REG = PIP_QOS_DIFF45 0x80011800A0000768 MEMORY 8
REG_FIELD = PIP_QOS_DIFF45 qos 2 0
REG = PIP_QOS_DIFF46 0x80011800A0000770 MEMORY 8
REG_FIELD = PIP_QOS_DIFF46 qos 2 0
REG = PIP_QOS_DIFF47 0x80011800A0000778 MEMORY 8
REG_FIELD = PIP_QOS_DIFF47 qos 2 0
REG = PIP_QOS_DIFF48 0x80011800A0000780 MEMORY 8
REG_FIELD = PIP_QOS_DIFF48 qos 2 0
REG = PIP_QOS_DIFF49 0x80011800A0000788 MEMORY 8
REG_FIELD = PIP_QOS_DIFF49 qos 2 0
REG = PIP_QOS_DIFF50 0x80011800A0000790 MEMORY 8
REG_FIELD = PIP_QOS_DIFF50 qos 2 0
REG = PIP_QOS_DIFF51 0x80011800A0000798 MEMORY 8
REG_FIELD = PIP_QOS_DIFF51 qos 2 0
REG = PIP_QOS_DIFF52 0x80011800A00007A0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF52 qos 2 0
REG = PIP_QOS_DIFF53 0x80011800A00007A8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF53 qos 2 0
REG = PIP_QOS_DIFF54 0x80011800A00007B0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF54 qos 2 0
REG = PIP_QOS_DIFF55 0x80011800A00007B8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF55 qos 2 0
REG = PIP_QOS_DIFF56 0x80011800A00007C0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF56 qos 2 0
REG = PIP_QOS_DIFF57 0x80011800A00007C8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF57 qos 2 0
REG = PIP_QOS_DIFF58 0x80011800A00007D0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF58 qos 2 0
REG = PIP_QOS_DIFF59 0x80011800A00007D8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF59 qos 2 0
REG = PIP_QOS_DIFF60 0x80011800A00007E0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF60 qos 2 0
REG = PIP_QOS_DIFF61 0x80011800A00007E8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF61 qos 2 0
REG = PIP_QOS_DIFF62 0x80011800A00007F0 MEMORY 8
REG_FIELD = PIP_QOS_DIFF62 qos 2 0
REG = PIP_QOS_DIFF63 0x80011800A00007F8 MEMORY 8
REG_FIELD = PIP_QOS_DIFF63 qos 2 0

REG = PIP_QOS_VLAN0 0x80011800A00000C0 MEMORY 8
REG_FIELD = PIP_QOS_VLAN0 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN1 0x80011800A00000C8 MEMORY 8
REG_FIELD = PIP_QOS_VLAN1 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN2 0x80011800A00000D0 MEMORY 8
REG_FIELD = PIP_QOS_VLAN2 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN3 0x80011800A00000D8 MEMORY 8
REG_FIELD = PIP_QOS_VLAN3 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN4 0x80011800A00000E0 MEMORY 8
REG_FIELD = PIP_QOS_VLAN4 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN5 0x80011800A00000E8 MEMORY 8
REG_FIELD = PIP_QOS_VLAN5 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN6 0x80011800A00000F0 MEMORY 8
REG_FIELD = PIP_QOS_VLAN6 qos1 6 4, qos 2 0
REG = PIP_QOS_VLAN7 0x80011800A00000F8 MEMORY 8
REG_FIELD = PIP_QOS_VLAN7 qos1 6 4, qos 2 0

REG = PIP_QOS_WATCH0 0x80011800A0000100 MEMORY 8
REG_FIELD = PIP_QOS_WATCH0 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH1 0x80011800A0000108 MEMORY 8
REG_FIELD = PIP_QOS_WATCH1 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH2 0x80011800A0000110 MEMORY 8
REG_FIELD = PIP_QOS_WATCH2 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH3 0x80011800A0000118 MEMORY 8
REG_FIELD = PIP_QOS_WATCH3 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH4 0x80011800A0000120 MEMORY 8
REG_FIELD = PIP_QOS_WATCH4 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH5 0x80011800A0000128 MEMORY 8
REG_FIELD = PIP_QOS_WATCH5 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH6 0x80011800A0000130 MEMORY 8
REG_FIELD = PIP_QOS_WATCH6 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0
REG = PIP_QOS_WATCH7 0x80011800A0000138 MEMORY 8
REG_FIELD = PIP_QOS_WATCH7 mask 47 32, grp 27 24, qos 22 20, typ 18 16, match_value 15 0

REG = PIP_RAW_WORD 0x80011800A00000B0 MEMORY 8
REG_FIELD = PIP_RAW_WORD word 55 0

REG = PIP_SFT_RST 0x80011800A0000030 MEMORY 8
REG_FIELD = PIP_SFT_RST rst 0 0

REG = PIP_STAT0_PRT0 0x80011800A0000800 MEMORY 8
REG_FIELD = PIP_STAT0_PRT0 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT1 0x80011800A0000850 MEMORY 8
REG_FIELD = PIP_STAT0_PRT1 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT2 0x80011800A00008A0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT2 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT3 0x80011800A00008F0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT3 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT16 0x80011800A0000D00 MEMORY 8
REG_FIELD = PIP_STAT0_PRT16 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT17 0x80011800A0000D50 MEMORY 8
REG_FIELD = PIP_STAT0_PRT17 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT18 0x80011800A0000DA0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT18 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT19 0x80011800A0000DF0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT19 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT24 0x80011800A0000F80 MEMORY 8
REG_FIELD = PIP_STAT0_PRT24 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT32 0x80011800A0001200 MEMORY 8
REG_FIELD = PIP_STAT0_PRT32 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT33 0x80011800A0001250 MEMORY 8
REG_FIELD = PIP_STAT0_PRT33 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT34 0x80011800A00012A0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT34 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT35 0x80011800A00012F0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT35 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT36 0x80011800A0001340 MEMORY 8
REG_FIELD = PIP_STAT0_PRT36 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT37 0x80011800A0001390 MEMORY 8
REG_FIELD = PIP_STAT0_PRT37 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT38 0x80011800A00013E0 MEMORY 8
REG_FIELD = PIP_STAT0_PRT38 drp_pkts 63 32, drp_octs 31 0
REG = PIP_STAT0_PRT39 0x80011800A0001430 MEMORY 8
REG_FIELD = PIP_STAT0_PRT39 drp_pkts 63 32, drp_octs 31 0

REG = PIP_STAT10_PRT0 0x80011800A0001480 MEMORY 8
REG_FIELD = PIP_STAT10_PRT0 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT1 0x80011800A0001490 MEMORY 8
REG_FIELD = PIP_STAT10_PRT1 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT2 0x80011800A00014A0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT2 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT3 0x80011800A00014B0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT3 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT16 0x80011800A0001580 MEMORY 8
REG_FIELD = PIP_STAT10_PRT16 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT17 0x80011800A0001590 MEMORY 8
REG_FIELD = PIP_STAT10_PRT17 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT18 0x80011800A00015A0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT18 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT19 0x80011800A00015B0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT19 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT24 0x80011800A0001600 MEMORY 8
REG_FIELD = PIP_STAT10_PRT24 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT32 0x80011800A0001680 MEMORY 8
REG_FIELD = PIP_STAT10_PRT32 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT33 0x80011800A0001690 MEMORY 8
REG_FIELD = PIP_STAT10_PRT33 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT34 0x80011800A00016A0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT34 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT35 0x80011800A00016B0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT35 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT36 0x80011800A00016C0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT36 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT37 0x80011800A00016D0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT37 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT38 0x80011800A00016E0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT38 bcast 63 32, mcast 31 0
REG = PIP_STAT10_PRT39 0x80011800A00016F0 MEMORY 8
REG_FIELD = PIP_STAT10_PRT39 bcast 63 32, mcast 31 0

REG = PIP_STAT11_PRT0 0x80011800A0001488 MEMORY 8
REG_FIELD = PIP_STAT11_PRT0 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT1 0x80011800A0001498 MEMORY 8
REG_FIELD = PIP_STAT11_PRT1 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT2 0x80011800A00014A8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT2 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT3 0x80011800A00014B8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT3 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT16 0x80011800A0001588 MEMORY 8
REG_FIELD = PIP_STAT11_PRT16 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT17 0x80011800A0001598 MEMORY 8
REG_FIELD = PIP_STAT11_PRT17 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT18 0x80011800A00015A8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT18 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT19 0x80011800A00015B8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT19 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT24 0x80011800A0001608 MEMORY 8
REG_FIELD = PIP_STAT11_PRT24 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT32 0x80011800A0001688 MEMORY 8
REG_FIELD = PIP_STAT11_PRT32 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT33 0x80011800A0001698 MEMORY 8
REG_FIELD = PIP_STAT11_PRT33 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT34 0x80011800A00016A8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT34 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT35 0x80011800A00016B8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT35 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT36 0x80011800A00016C8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT36 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT37 0x80011800A00016D8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT37 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT38 0x80011800A00016E8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT38 bcast 63 32, mcast 31 0
REG = PIP_STAT11_PRT39 0x80011800A00016F8 MEMORY 8
REG_FIELD = PIP_STAT11_PRT39 bcast 63 32, mcast 31 0

REG = PIP_STAT1_PRT0 0x80011800A0000808 MEMORY 8
REG_FIELD = PIP_STAT1_PRT0 octs 47 0
REG = PIP_STAT1_PRT1 0x80011800A0000858 MEMORY 8
REG_FIELD = PIP_STAT1_PRT1 octs 47 0
REG = PIP_STAT1_PRT2 0x80011800A00008A8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT2 octs 47 0
REG = PIP_STAT1_PRT3 0x80011800A00008F8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT3 octs 47 0
REG = PIP_STAT1_PRT16 0x80011800A0000D08 MEMORY 8
REG_FIELD = PIP_STAT1_PRT16 octs 47 0
REG = PIP_STAT1_PRT17 0x80011800A0000D58 MEMORY 8
REG_FIELD = PIP_STAT1_PRT17 octs 47 0
REG = PIP_STAT1_PRT18 0x80011800A0000DA8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT18 octs 47 0
REG = PIP_STAT1_PRT19 0x80011800A0000DF8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT19 octs 47 0
REG = PIP_STAT1_PRT24 0x80011800A0000F88 MEMORY 8
REG_FIELD = PIP_STAT1_PRT24 octs 47 0
REG = PIP_STAT1_PRT32 0x80011800A0001208 MEMORY 8
REG_FIELD = PIP_STAT1_PRT32 octs 47 0
REG = PIP_STAT1_PRT33 0x80011800A0001258 MEMORY 8
REG_FIELD = PIP_STAT1_PRT33 octs 47 0
REG = PIP_STAT1_PRT34 0x80011800A00012A8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT34 octs 47 0
REG = PIP_STAT1_PRT35 0x80011800A00012F8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT35 octs 47 0
REG = PIP_STAT1_PRT36 0x80011800A0001348 MEMORY 8
REG_FIELD = PIP_STAT1_PRT36 octs 47 0
REG = PIP_STAT1_PRT37 0x80011800A0001398 MEMORY 8
REG_FIELD = PIP_STAT1_PRT37 octs 47 0
REG = PIP_STAT1_PRT38 0x80011800A00013E8 MEMORY 8
REG_FIELD = PIP_STAT1_PRT38 octs 47 0
REG = PIP_STAT1_PRT39 0x80011800A0001438 MEMORY 8
REG_FIELD = PIP_STAT1_PRT39 octs 47 0

REG = PIP_STAT2_PRT0 0x80011800A0000810 MEMORY 8
REG_FIELD = PIP_STAT2_PRT0 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT1 0x80011800A0000860 MEMORY 8
REG_FIELD = PIP_STAT2_PRT1 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT2 0x80011800A00008B0 MEMORY 8
REG_FIELD = PIP_STAT2_PRT2 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT3 0x80011800A0000900 MEMORY 8
REG_FIELD = PIP_STAT2_PRT3 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT16 0x80011800A0000D10 MEMORY 8
REG_FIELD = PIP_STAT2_PRT16 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT17 0x80011800A0000D60 MEMORY 8
REG_FIELD = PIP_STAT2_PRT17 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT18 0x80011800A0000DB0 MEMORY 8
REG_FIELD = PIP_STAT2_PRT18 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT19 0x80011800A0000E00 MEMORY 8
REG_FIELD = PIP_STAT2_PRT19 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT24 0x80011800A0000F90 MEMORY 8
REG_FIELD = PIP_STAT2_PRT24 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT32 0x80011800A0001210 MEMORY 8
REG_FIELD = PIP_STAT2_PRT32 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT33 0x80011800A0001260 MEMORY 8
REG_FIELD = PIP_STAT2_PRT33 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT34 0x80011800A00012B0 MEMORY 8
REG_FIELD = PIP_STAT2_PRT34 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT35 0x80011800A0001300 MEMORY 8
REG_FIELD = PIP_STAT2_PRT35 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT36 0x80011800A0001350 MEMORY 8
REG_FIELD = PIP_STAT2_PRT36 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT37 0x80011800A00013A0 MEMORY 8
REG_FIELD = PIP_STAT2_PRT37 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT38 0x80011800A00013F0 MEMORY 8
REG_FIELD = PIP_STAT2_PRT38 pkts 63 32, raw 31 0
REG = PIP_STAT2_PRT39 0x80011800A0001440 MEMORY 8
REG_FIELD = PIP_STAT2_PRT39 pkts 63 32, raw 31 0

REG = PIP_STAT3_PRT0 0x80011800A0000818 MEMORY 8
REG_FIELD = PIP_STAT3_PRT0 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT1 0x80011800A0000868 MEMORY 8
REG_FIELD = PIP_STAT3_PRT1 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT2 0x80011800A00008B8 MEMORY 8
REG_FIELD = PIP_STAT3_PRT2 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT3 0x80011800A0000908 MEMORY 8
REG_FIELD = PIP_STAT3_PRT3 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT16 0x80011800A0000D18 MEMORY 8
REG_FIELD = PIP_STAT3_PRT16 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT17 0x80011800A0000D68 MEMORY 8
REG_FIELD = PIP_STAT3_PRT17 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT18 0x80011800A0000DB8 MEMORY 8
REG_FIELD = PIP_STAT3_PRT18 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT19 0x80011800A0000E08 MEMORY 8
REG_FIELD = PIP_STAT3_PRT19 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT24 0x80011800A0000F98 MEMORY 8
REG_FIELD = PIP_STAT3_PRT24 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT32 0x80011800A0001218 MEMORY 8
REG_FIELD = PIP_STAT3_PRT32 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT33 0x80011800A0001268 MEMORY 8
REG_FIELD = PIP_STAT3_PRT33 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT34 0x80011800A00012B8 MEMORY 8
REG_FIELD = PIP_STAT3_PRT34 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT35 0x80011800A0001308 MEMORY 8
REG_FIELD = PIP_STAT3_PRT35 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT36 0x80011800A0001358 MEMORY 8
REG_FIELD = PIP_STAT3_PRT36 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT37 0x80011800A00013A8 MEMORY 8
REG_FIELD = PIP_STAT3_PRT37 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT38 0x80011800A00013F8 MEMORY 8
REG_FIELD = PIP_STAT3_PRT38 bcst 63 32, mcst 31 0
REG = PIP_STAT3_PRT39 0x80011800A0001448 MEMORY 8
REG_FIELD = PIP_STAT3_PRT39 bcst 63 32, mcst 31 0

REG = PIP_STAT4_PRT0 0x80011800A0000820 MEMORY 8
REG_FIELD = PIP_STAT4_PRT0 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT1 0x80011800A0000870 MEMORY 8
REG_FIELD = PIP_STAT4_PRT1 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT2 0x80011800A00008C0 MEMORY 8
REG_FIELD = PIP_STAT4_PRT2 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT3 0x80011800A0000910 MEMORY 8
REG_FIELD = PIP_STAT4_PRT3 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT16 0x80011800A0000D20 MEMORY 8
REG_FIELD = PIP_STAT4_PRT16 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT17 0x80011800A0000D70 MEMORY 8
REG_FIELD = PIP_STAT4_PRT17 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT18 0x80011800A0000DC0 MEMORY 8
REG_FIELD = PIP_STAT4_PRT18 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT19 0x80011800A0000E10 MEMORY 8
REG_FIELD = PIP_STAT4_PRT19 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT24 0x80011800A0000FA0 MEMORY 8
REG_FIELD = PIP_STAT4_PRT24 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT32 0x80011800A0001220 MEMORY 8
REG_FIELD = PIP_STAT4_PRT32 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT33 0x80011800A0001270 MEMORY 8
REG_FIELD = PIP_STAT4_PRT33 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT34 0x80011800A00012C0 MEMORY 8
REG_FIELD = PIP_STAT4_PRT34 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT35 0x80011800A0001310 MEMORY 8
REG_FIELD = PIP_STAT4_PRT35 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT36 0x80011800A0001360 MEMORY 8
REG_FIELD = PIP_STAT4_PRT36 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT37 0x80011800A00013B0 MEMORY 8
REG_FIELD = PIP_STAT4_PRT37 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT38 0x80011800A0001400 MEMORY 8
REG_FIELD = PIP_STAT4_PRT38 h65to127 63 32, h64 31 0
REG = PIP_STAT4_PRT39 0x80011800A0001450 MEMORY 8
REG_FIELD = PIP_STAT4_PRT39 h65to127 63 32, h64 31 0

REG = PIP_STAT5_PRT0 0x80011800A0000828 MEMORY 8
REG_FIELD = PIP_STAT5_PRT0 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT1 0x80011800A0000878 MEMORY 8
REG_FIELD = PIP_STAT5_PRT1 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT2 0x80011800A00008C8 MEMORY 8
REG_FIELD = PIP_STAT5_PRT2 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT3 0x80011800A0000918 MEMORY 8
REG_FIELD = PIP_STAT5_PRT3 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT16 0x80011800A0000D28 MEMORY 8
REG_FIELD = PIP_STAT5_PRT16 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT17 0x80011800A0000D78 MEMORY 8
REG_FIELD = PIP_STAT5_PRT17 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT18 0x80011800A0000DC8 MEMORY 8
REG_FIELD = PIP_STAT5_PRT18 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT19 0x80011800A0000E18 MEMORY 8
REG_FIELD = PIP_STAT5_PRT19 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT24 0x80011800A0000FA8 MEMORY 8
REG_FIELD = PIP_STAT5_PRT24 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT32 0x80011800A0001228 MEMORY 8
REG_FIELD = PIP_STAT5_PRT32 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT33 0x80011800A0001278 MEMORY 8
REG_FIELD = PIP_STAT5_PRT33 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT34 0x80011800A00012C8 MEMORY 8
REG_FIELD = PIP_STAT5_PRT34 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT35 0x80011800A0001318 MEMORY 8
REG_FIELD = PIP_STAT5_PRT35 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT36 0x80011800A0001368 MEMORY 8
REG_FIELD = PIP_STAT5_PRT36 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT37 0x80011800A00013B8 MEMORY 8
REG_FIELD = PIP_STAT5_PRT37 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT38 0x80011800A0001408 MEMORY 8
REG_FIELD = PIP_STAT5_PRT38 h256to511 63 32, h128to255 31 0
REG = PIP_STAT5_PRT39 0x80011800A0001458 MEMORY 8
REG_FIELD = PIP_STAT5_PRT39 h256to511 63 32, h128to255 31 0

REG = PIP_STAT6_PRT0 0x80011800A0000830 MEMORY 8
REG_FIELD = PIP_STAT6_PRT0 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT1 0x80011800A0000880 MEMORY 8
REG_FIELD = PIP_STAT6_PRT1 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT2 0x80011800A00008D0 MEMORY 8
REG_FIELD = PIP_STAT6_PRT2 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT3 0x80011800A0000920 MEMORY 8
REG_FIELD = PIP_STAT6_PRT3 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT16 0x80011800A0000D30 MEMORY 8
REG_FIELD = PIP_STAT6_PRT16 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT17 0x80011800A0000D80 MEMORY 8
REG_FIELD = PIP_STAT6_PRT17 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT18 0x80011800A0000DD0 MEMORY 8
REG_FIELD = PIP_STAT6_PRT18 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT19 0x80011800A0000E20 MEMORY 8
REG_FIELD = PIP_STAT6_PRT19 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT24 0x80011800A0000FB0 MEMORY 8
REG_FIELD = PIP_STAT6_PRT24 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT32 0x80011800A0001230 MEMORY 8
REG_FIELD = PIP_STAT6_PRT32 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT33 0x80011800A0001280 MEMORY 8
REG_FIELD = PIP_STAT6_PRT33 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT34 0x80011800A00012D0 MEMORY 8
REG_FIELD = PIP_STAT6_PRT34 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT35 0x80011800A0001320 MEMORY 8
REG_FIELD = PIP_STAT6_PRT35 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT36 0x80011800A0001370 MEMORY 8
REG_FIELD = PIP_STAT6_PRT36 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT37 0x80011800A00013C0 MEMORY 8
REG_FIELD = PIP_STAT6_PRT37 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT38 0x80011800A0001410 MEMORY 8
REG_FIELD = PIP_STAT6_PRT38 h1024to1518 63 32, h512to1023 31 0
REG = PIP_STAT6_PRT39 0x80011800A0001460 MEMORY 8
REG_FIELD = PIP_STAT6_PRT39 h1024to1518 63 32, h512to1023 31 0

REG = PIP_STAT7_PRT0 0x80011800A0000838 MEMORY 8
REG_FIELD = PIP_STAT7_PRT0 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT1 0x80011800A0000888 MEMORY 8
REG_FIELD = PIP_STAT7_PRT1 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT2 0x80011800A00008D8 MEMORY 8
REG_FIELD = PIP_STAT7_PRT2 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT3 0x80011800A0000928 MEMORY 8
REG_FIELD = PIP_STAT7_PRT3 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT16 0x80011800A0000D38 MEMORY 8
REG_FIELD = PIP_STAT7_PRT16 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT17 0x80011800A0000D88 MEMORY 8
REG_FIELD = PIP_STAT7_PRT17 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT18 0x80011800A0000DD8 MEMORY 8
REG_FIELD = PIP_STAT7_PRT18 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT19 0x80011800A0000E28 MEMORY 8
REG_FIELD = PIP_STAT7_PRT19 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT24 0x80011800A0000FB8 MEMORY 8
REG_FIELD = PIP_STAT7_PRT24 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT32 0x80011800A0001238 MEMORY 8
REG_FIELD = PIP_STAT7_PRT32 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT33 0x80011800A0001288 MEMORY 8
REG_FIELD = PIP_STAT7_PRT33 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT34 0x80011800A00012D8 MEMORY 8
REG_FIELD = PIP_STAT7_PRT34 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT35 0x80011800A0001328 MEMORY 8
REG_FIELD = PIP_STAT7_PRT35 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT36 0x80011800A0001378 MEMORY 8
REG_FIELD = PIP_STAT7_PRT36 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT37 0x80011800A00013C8 MEMORY 8
REG_FIELD = PIP_STAT7_PRT37 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT38 0x80011800A0001418 MEMORY 8
REG_FIELD = PIP_STAT7_PRT38 fcs 63 32, h1519 31 0
REG = PIP_STAT7_PRT39 0x80011800A0001468 MEMORY 8
REG_FIELD = PIP_STAT7_PRT39 fcs 63 32, h1519 31 0

REG = PIP_STAT8_PRT0 0x80011800A0000840 MEMORY 8
REG_FIELD = PIP_STAT8_PRT0 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT1 0x80011800A0000890 MEMORY 8
REG_FIELD = PIP_STAT8_PRT1 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT2 0x80011800A00008E0 MEMORY 8
REG_FIELD = PIP_STAT8_PRT2 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT3 0x80011800A0000930 MEMORY 8
REG_FIELD = PIP_STAT8_PRT3 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT16 0x80011800A0000D40 MEMORY 8
REG_FIELD = PIP_STAT8_PRT16 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT17 0x80011800A0000D90 MEMORY 8
REG_FIELD = PIP_STAT8_PRT17 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT18 0x80011800A0000DE0 MEMORY 8
REG_FIELD = PIP_STAT8_PRT18 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT19 0x80011800A0000E30 MEMORY 8
REG_FIELD = PIP_STAT8_PRT19 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT24 0x80011800A0000FC0 MEMORY 8
REG_FIELD = PIP_STAT8_PRT24 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT32 0x80011800A0001240 MEMORY 8
REG_FIELD = PIP_STAT8_PRT32 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT33 0x80011800A0001290 MEMORY 8
REG_FIELD = PIP_STAT8_PRT33 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT34 0x80011800A00012E0 MEMORY 8
REG_FIELD = PIP_STAT8_PRT34 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT35 0x80011800A0001330 MEMORY 8
REG_FIELD = PIP_STAT8_PRT35 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT36 0x80011800A0001380 MEMORY 8
REG_FIELD = PIP_STAT8_PRT36 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT37 0x80011800A00013D0 MEMORY 8
REG_FIELD = PIP_STAT8_PRT37 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT38 0x80011800A0001420 MEMORY 8
REG_FIELD = PIP_STAT8_PRT38 frag 63 32, undersz 31 0
REG = PIP_STAT8_PRT39 0x80011800A0001470 MEMORY 8
REG_FIELD = PIP_STAT8_PRT39 frag 63 32, undersz 31 0

REG = PIP_STAT9_PRT0 0x80011800A0000848 MEMORY 8
REG_FIELD = PIP_STAT9_PRT0 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT1 0x80011800A0000898 MEMORY 8
REG_FIELD = PIP_STAT9_PRT1 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT2 0x80011800A00008E8 MEMORY 8
REG_FIELD = PIP_STAT9_PRT2 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT3 0x80011800A0000938 MEMORY 8
REG_FIELD = PIP_STAT9_PRT3 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT16 0x80011800A0000D48 MEMORY 8
REG_FIELD = PIP_STAT9_PRT16 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT17 0x80011800A0000D98 MEMORY 8
REG_FIELD = PIP_STAT9_PRT17 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT18 0x80011800A0000DE8 MEMORY 8
REG_FIELD = PIP_STAT9_PRT18 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT19 0x80011800A0000E38 MEMORY 8
REG_FIELD = PIP_STAT9_PRT19 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT24 0x80011800A0000FC8 MEMORY 8
REG_FIELD = PIP_STAT9_PRT24 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT32 0x80011800A0001248 MEMORY 8
REG_FIELD = PIP_STAT9_PRT32 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT33 0x80011800A0001298 MEMORY 8
REG_FIELD = PIP_STAT9_PRT33 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT34 0x80011800A00012E8 MEMORY 8
REG_FIELD = PIP_STAT9_PRT34 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT35 0x80011800A0001338 MEMORY 8
REG_FIELD = PIP_STAT9_PRT35 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT36 0x80011800A0001388 MEMORY 8
REG_FIELD = PIP_STAT9_PRT36 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT37 0x80011800A00013D8 MEMORY 8
REG_FIELD = PIP_STAT9_PRT37 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT38 0x80011800A0001428 MEMORY 8
REG_FIELD = PIP_STAT9_PRT38 jabber 63 32, oversz 31 0
REG = PIP_STAT9_PRT39 0x80011800A0001478 MEMORY 8
REG_FIELD = PIP_STAT9_PRT39 jabber 63 32, oversz 31 0

REG = PIP_STAT_CTL 0x80011800A0000018 MEMORY 8
REG_FIELD = PIP_STAT_CTL rdclr 0 0

REG = PIP_STAT_INB_ERRS0 0x80011800A0001A10 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS0 errs 15 0
REG = PIP_STAT_INB_ERRS1 0x80011800A0001A30 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS1 errs 15 0
REG = PIP_STAT_INB_ERRS2 0x80011800A0001A50 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS2 errs 15 0
REG = PIP_STAT_INB_ERRS3 0x80011800A0001A70 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS3 errs 15 0
REG = PIP_STAT_INB_ERRS16 0x80011800A0001C10 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS16 errs 15 0
REG = PIP_STAT_INB_ERRS17 0x80011800A0001C30 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS17 errs 15 0
REG = PIP_STAT_INB_ERRS18 0x80011800A0001C50 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS18 errs 15 0
REG = PIP_STAT_INB_ERRS19 0x80011800A0001C70 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS19 errs 15 0
REG = PIP_STAT_INB_ERRS24 0x80011800A0001D10 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS24 errs 15 0
REG = PIP_STAT_INB_ERRS32 0x80011800A0001E10 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS32 errs 15 0
REG = PIP_STAT_INB_ERRS33 0x80011800A0001E30 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS33 errs 15 0
REG = PIP_STAT_INB_ERRS34 0x80011800A0001E50 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS34 errs 15 0
REG = PIP_STAT_INB_ERRS35 0x80011800A0001E70 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS35 errs 15 0
REG = PIP_STAT_INB_ERRS36 0x80011800A0001E90 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS36 errs 15 0
REG = PIP_STAT_INB_ERRS37 0x80011800A0001EB0 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS37 errs 15 0
REG = PIP_STAT_INB_ERRS38 0x80011800A0001ED0 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS38 errs 15 0
REG = PIP_STAT_INB_ERRS39 0x80011800A0001EF0 MEMORY 8
REG_FIELD = PIP_STAT_INB_ERRS39 errs 15 0

REG = PIP_STAT_INB_OCTS0 0x80011800A0001A08 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS0 octs 47 0
REG = PIP_STAT_INB_OCTS1 0x80011800A0001A28 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS1 octs 47 0
REG = PIP_STAT_INB_OCTS2 0x80011800A0001A48 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS2 octs 47 0
REG = PIP_STAT_INB_OCTS3 0x80011800A0001A68 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS3 octs 47 0
REG = PIP_STAT_INB_OCTS16 0x80011800A0001C08 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS16 octs 47 0
REG = PIP_STAT_INB_OCTS17 0x80011800A0001C28 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS17 octs 47 0
REG = PIP_STAT_INB_OCTS18 0x80011800A0001C48 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS18 octs 47 0
REG = PIP_STAT_INB_OCTS19 0x80011800A0001C68 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS19 octs 47 0
REG = PIP_STAT_INB_OCTS24 0x80011800A0001D08 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS24 octs 47 0
REG = PIP_STAT_INB_OCTS32 0x80011800A0001E08 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS32 octs 47 0
REG = PIP_STAT_INB_OCTS33 0x80011800A0001E28 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS33 octs 47 0
REG = PIP_STAT_INB_OCTS34 0x80011800A0001E48 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS34 octs 47 0
REG = PIP_STAT_INB_OCTS35 0x80011800A0001E68 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS35 octs 47 0
REG = PIP_STAT_INB_OCTS36 0x80011800A0001E88 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS36 octs 47 0
REG = PIP_STAT_INB_OCTS37 0x80011800A0001EA8 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS37 octs 47 0
REG = PIP_STAT_INB_OCTS38 0x80011800A0001EC8 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS38 octs 47 0
REG = PIP_STAT_INB_OCTS39 0x80011800A0001EE8 MEMORY 8
REG_FIELD = PIP_STAT_INB_OCTS39 octs 47 0

REG = PIP_STAT_INB_PKTS0 0x80011800A0001A00 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS0 pkts 31 0
REG = PIP_STAT_INB_PKTS1 0x80011800A0001A20 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS1 pkts 31 0
REG = PIP_STAT_INB_PKTS2 0x80011800A0001A40 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS2 pkts 31 0
REG = PIP_STAT_INB_PKTS3 0x80011800A0001A60 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS3 pkts 31 0
REG = PIP_STAT_INB_PKTS16 0x80011800A0001C00 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS16 pkts 31 0
REG = PIP_STAT_INB_PKTS17 0x80011800A0001C20 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS17 pkts 31 0
REG = PIP_STAT_INB_PKTS18 0x80011800A0001C40 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS18 pkts 31 0
REG = PIP_STAT_INB_PKTS19 0x80011800A0001C60 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS19 pkts 31 0
REG = PIP_STAT_INB_PKTS24 0x80011800A0001D00 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS24 pkts 31 0
REG = PIP_STAT_INB_PKTS32 0x80011800A0001E00 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS32 pkts 31 0
REG = PIP_STAT_INB_PKTS33 0x80011800A0001E20 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS33 pkts 31 0
REG = PIP_STAT_INB_PKTS34 0x80011800A0001E40 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS34 pkts 31 0
REG = PIP_STAT_INB_PKTS35 0x80011800A0001E60 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS35 pkts 31 0
REG = PIP_STAT_INB_PKTS36 0x80011800A0001E80 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS36 pkts 31 0
REG = PIP_STAT_INB_PKTS37 0x80011800A0001EA0 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS37 pkts 31 0
REG = PIP_STAT_INB_PKTS38 0x80011800A0001EC0 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS38 pkts 31 0
REG = PIP_STAT_INB_PKTS39 0x80011800A0001EE0 MEMORY 8
REG_FIELD = PIP_STAT_INB_PKTS39 pkts 31 0

REG = PIP_TAG_INC0 0x80011800A0001800 MEMORY 8
REG_FIELD = PIP_TAG_INC0 en 7 0
REG = PIP_TAG_INC1 0x80011800A0001808 MEMORY 8
REG_FIELD = PIP_TAG_INC1 en 7 0
REG = PIP_TAG_INC2 0x80011800A0001810 MEMORY 8
REG_FIELD = PIP_TAG_INC2 en 7 0
REG = PIP_TAG_INC3 0x80011800A0001818 MEMORY 8
REG_FIELD = PIP_TAG_INC3 en 7 0
REG = PIP_TAG_INC4 0x80011800A0001820 MEMORY 8
REG_FIELD = PIP_TAG_INC4 en 7 0
REG = PIP_TAG_INC5 0x80011800A0001828 MEMORY 8
REG_FIELD = PIP_TAG_INC5 en 7 0
REG = PIP_TAG_INC6 0x80011800A0001830 MEMORY 8
REG_FIELD = PIP_TAG_INC6 en 7 0
REG = PIP_TAG_INC7 0x80011800A0001838 MEMORY 8
REG_FIELD = PIP_TAG_INC7 en 7 0
REG = PIP_TAG_INC8 0x80011800A0001840 MEMORY 8
REG_FIELD = PIP_TAG_INC8 en 7 0
REG = PIP_TAG_INC9 0x80011800A0001848 MEMORY 8
REG_FIELD = PIP_TAG_INC9 en 7 0
REG = PIP_TAG_INC10 0x80011800A0001850 MEMORY 8
REG_FIELD = PIP_TAG_INC10 en 7 0
REG = PIP_TAG_INC11 0x80011800A0001858 MEMORY 8
REG_FIELD = PIP_TAG_INC11 en 7 0
REG = PIP_TAG_INC12 0x80011800A0001860 MEMORY 8
REG_FIELD = PIP_TAG_INC12 en 7 0
REG = PIP_TAG_INC13 0x80011800A0001868 MEMORY 8
REG_FIELD = PIP_TAG_INC13 en 7 0
REG = PIP_TAG_INC14 0x80011800A0001870 MEMORY 8
REG_FIELD = PIP_TAG_INC14 en 7 0
REG = PIP_TAG_INC15 0x80011800A0001878 MEMORY 8
REG_FIELD = PIP_TAG_INC15 en 7 0
REG = PIP_TAG_INC16 0x80011800A0001880 MEMORY 8
REG_FIELD = PIP_TAG_INC16 en 7 0
REG = PIP_TAG_INC17 0x80011800A0001888 MEMORY 8
REG_FIELD = PIP_TAG_INC17 en 7 0
REG = PIP_TAG_INC18 0x80011800A0001890 MEMORY 8
REG_FIELD = PIP_TAG_INC18 en 7 0
REG = PIP_TAG_INC19 0x80011800A0001898 MEMORY 8
REG_FIELD = PIP_TAG_INC19 en 7 0
REG = PIP_TAG_INC20 0x80011800A00018A0 MEMORY 8
REG_FIELD = PIP_TAG_INC20 en 7 0
REG = PIP_TAG_INC21 0x80011800A00018A8 MEMORY 8
REG_FIELD = PIP_TAG_INC21 en 7 0
REG = PIP_TAG_INC22 0x80011800A00018B0 MEMORY 8
REG_FIELD = PIP_TAG_INC22 en 7 0
REG = PIP_TAG_INC23 0x80011800A00018B8 MEMORY 8
REG_FIELD = PIP_TAG_INC23 en 7 0
REG = PIP_TAG_INC24 0x80011800A00018C0 MEMORY 8
REG_FIELD = PIP_TAG_INC24 en 7 0
REG = PIP_TAG_INC25 0x80011800A00018C8 MEMORY 8
REG_FIELD = PIP_TAG_INC25 en 7 0
REG = PIP_TAG_INC26 0x80011800A00018D0 MEMORY 8
REG_FIELD = PIP_TAG_INC26 en 7 0
REG = PIP_TAG_INC27 0x80011800A00018D8 MEMORY 8
REG_FIELD = PIP_TAG_INC27 en 7 0
REG = PIP_TAG_INC28 0x80011800A00018E0 MEMORY 8
REG_FIELD = PIP_TAG_INC28 en 7 0
REG = PIP_TAG_INC29 0x80011800A00018E8 MEMORY 8
REG_FIELD = PIP_TAG_INC29 en 7 0
REG = PIP_TAG_INC30 0x80011800A00018F0 MEMORY 8
REG_FIELD = PIP_TAG_INC30 en 7 0
REG = PIP_TAG_INC31 0x80011800A00018F8 MEMORY 8
REG_FIELD = PIP_TAG_INC31 en 7 0
REG = PIP_TAG_INC32 0x80011800A0001900 MEMORY 8
REG_FIELD = PIP_TAG_INC32 en 7 0
REG = PIP_TAG_INC33 0x80011800A0001908 MEMORY 8
REG_FIELD = PIP_TAG_INC33 en 7 0
REG = PIP_TAG_INC34 0x80011800A0001910 MEMORY 8
REG_FIELD = PIP_TAG_INC34 en 7 0
REG = PIP_TAG_INC35 0x80011800A0001918 MEMORY 8
REG_FIELD = PIP_TAG_INC35 en 7 0
REG = PIP_TAG_INC36 0x80011800A0001920 MEMORY 8
REG_FIELD = PIP_TAG_INC36 en 7 0
REG = PIP_TAG_INC37 0x80011800A0001928 MEMORY 8
REG_FIELD = PIP_TAG_INC37 en 7 0
REG = PIP_TAG_INC38 0x80011800A0001930 MEMORY 8
REG_FIELD = PIP_TAG_INC38 en 7 0
REG = PIP_TAG_INC39 0x80011800A0001938 MEMORY 8
REG_FIELD = PIP_TAG_INC39 en 7 0
REG = PIP_TAG_INC40 0x80011800A0001940 MEMORY 8
REG_FIELD = PIP_TAG_INC40 en 7 0
REG = PIP_TAG_INC41 0x80011800A0001948 MEMORY 8
REG_FIELD = PIP_TAG_INC41 en 7 0
REG = PIP_TAG_INC42 0x80011800A0001950 MEMORY 8
REG_FIELD = PIP_TAG_INC42 en 7 0
REG = PIP_TAG_INC43 0x80011800A0001958 MEMORY 8
REG_FIELD = PIP_TAG_INC43 en 7 0
REG = PIP_TAG_INC44 0x80011800A0001960 MEMORY 8
REG_FIELD = PIP_TAG_INC44 en 7 0
REG = PIP_TAG_INC45 0x80011800A0001968 MEMORY 8
REG_FIELD = PIP_TAG_INC45 en 7 0
REG = PIP_TAG_INC46 0x80011800A0001970 MEMORY 8
REG_FIELD = PIP_TAG_INC46 en 7 0
REG = PIP_TAG_INC47 0x80011800A0001978 MEMORY 8
REG_FIELD = PIP_TAG_INC47 en 7 0
REG = PIP_TAG_INC48 0x80011800A0001980 MEMORY 8
REG_FIELD = PIP_TAG_INC48 en 7 0
REG = PIP_TAG_INC49 0x80011800A0001988 MEMORY 8
REG_FIELD = PIP_TAG_INC49 en 7 0
REG = PIP_TAG_INC50 0x80011800A0001990 MEMORY 8
REG_FIELD = PIP_TAG_INC50 en 7 0
REG = PIP_TAG_INC51 0x80011800A0001998 MEMORY 8
REG_FIELD = PIP_TAG_INC51 en 7 0
REG = PIP_TAG_INC52 0x80011800A00019A0 MEMORY 8
REG_FIELD = PIP_TAG_INC52 en 7 0
REG = PIP_TAG_INC53 0x80011800A00019A8 MEMORY 8
REG_FIELD = PIP_TAG_INC53 en 7 0
REG = PIP_TAG_INC54 0x80011800A00019B0 MEMORY 8
REG_FIELD = PIP_TAG_INC54 en 7 0
REG = PIP_TAG_INC55 0x80011800A00019B8 MEMORY 8
REG_FIELD = PIP_TAG_INC55 en 7 0
REG = PIP_TAG_INC56 0x80011800A00019C0 MEMORY 8
REG_FIELD = PIP_TAG_INC56 en 7 0
REG = PIP_TAG_INC57 0x80011800A00019C8 MEMORY 8
REG_FIELD = PIP_TAG_INC57 en 7 0
REG = PIP_TAG_INC58 0x80011800A00019D0 MEMORY 8
REG_FIELD = PIP_TAG_INC58 en 7 0
REG = PIP_TAG_INC59 0x80011800A00019D8 MEMORY 8
REG_FIELD = PIP_TAG_INC59 en 7 0
REG = PIP_TAG_INC60 0x80011800A00019E0 MEMORY 8
REG_FIELD = PIP_TAG_INC60 en 7 0
REG = PIP_TAG_INC61 0x80011800A00019E8 MEMORY 8
REG_FIELD = PIP_TAG_INC61 en 7 0
REG = PIP_TAG_INC62 0x80011800A00019F0 MEMORY 8
REG_FIELD = PIP_TAG_INC62 en 7 0
REG = PIP_TAG_INC63 0x80011800A00019F8 MEMORY 8
REG_FIELD = PIP_TAG_INC63 en 7 0

REG = PIP_TAG_MASK 0x80011800A0000070 MEMORY 8
REG_FIELD = PIP_TAG_MASK mask 15 0

REG = PIP_TAG_SECRET 0x80011800A0000068 MEMORY 8
REG_FIELD = PIP_TAG_SECRET dst 31 16, src 15 0

REG = PIP_TODO_ENTRY 0x80011800A0000078 MEMORY 8
REG_FIELD = PIP_TODO_ENTRY val 63 63, entry 61 0

REG = PIP_VLAN_ETYPES0 0x80011800A00001C0 MEMORY 8
REG_FIELD = PIP_VLAN_ETYPES0 type3 63 48, type2 47 32, type1 31 16, type0 15 0
REG = PIP_VLAN_ETYPES1 0x80011800A00001C8 MEMORY 8
REG_FIELD = PIP_VLAN_ETYPES1 type3 63 48, type2 47 32, type1 31 16, type0 15 0

REG = PKO_MEM_COUNT0 0x8001180050001080 MEMORY 8
REG_FIELD = PKO_MEM_COUNT0 count 31 0

REG = PKO_MEM_COUNT1 0x8001180050001088 MEMORY 8
REG_FIELD = PKO_MEM_COUNT1 count 47 0

REG = PKO_MEM_DEBUG0 0x8001180050001100 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG0 fau 63 36, cmd 35 22, segs 21 16, size 15 0

REG = PKO_MEM_DEBUG1 0x8001180050001108 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG1 i 63 63, back 62 59, pool 58 56, size 55 40, ptr 39 0

REG = PKO_MEM_DEBUG10 0x8001180050001150 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG10 ptrs1 48 32, ptrs2 16 0

REG = PKO_MEM_DEBUG11 0x8001180050001158 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG11 maj 22 22, uid 21 19, sop 18 18, len 17 17, chk 16 16, cnt 15 3, mod 2 0

REG = PKO_MEM_DEBUG12 0x8001180050001160 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG12 fau 63 36, cmd 35 22, segs 21 16, size 15 0

REG = PKO_MEM_DEBUG13 0x8001180050001168 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG13 i 63 63, back 62 59, pool 58 56, size 55 40, ptr 39 0

REG = PKO_MEM_DEBUG14 0x8001180050001170 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG14 data 63 0

REG = PKO_MEM_DEBUG2 0x8001180050001110 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG2 i 63 63, back 62 59, pool 58 56, size 55 40, ptr 39 0

REG = PKO_MEM_DEBUG3 0x8001180050001118 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG3 data 63 0

REG = PKO_MEM_DEBUG4 0x8001180050001120 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG4 curr_siz 63 56, curr_off 55 40, cmnd_segs 39 34, cmnd_siz 33 18, cmnd_off 17 12, uid 11 10, dread_sop 9 9, init_dwrite 8 8, chk_once 7 7, chk_mode 6 6, wait 5 5, minor 4 3, major 2 0

REG = PKO_MEM_DEBUG5 0x8001180050001128 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG5 ptp 55 55, major_3 54 54, nxt_inflt 53 48, curr_ptr 47 8, curr_siz 7 0

REG = PKO_MEM_DEBUG6 0x8001180050001130 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG6 qid_offres 36 33, qid_offths 32 29, preempter 28 28, preemptee 27 27, preempted 26 26, active 25 25, staticb 24 24, qos 23 21, qcb_ridx 20 16, qid_offmax 15 12, qid_off 11 8, qid_base 7 0

REG = PKO_MEM_DEBUG7 0x8001180050001138 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG7 qos 63 59, tail 58 58, buf_siz 57 45, buf_ptr 44 12, qcb_widx 11 6, qcb_ridx 5 0

REG = PKO_MEM_DEBUG8 0x8001180050001140 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG8 qid_qqos 41 34, qid_idx 32 29, preempter 28 28, doorbell 27 8, preemptee 6 6, static_p 5 5, s_tail 4 4, static_q 3 3, qos 2 0

REG = PKO_MEM_DEBUG9 0x8001180050001148 MEMORY 8
REG_FIELD = PKO_MEM_DEBUG9 ptrs0 48 32, ptrs3 16 0

REG = PKO_MEM_PORT_PTRS 0x8001180050001010 MEMORY 8
REG_FIELD = PKO_MEM_PORT_PTRS static_p 61 61, qos_mask 60 53, bp_port 15 10, eid 9 6, pid 5 0

REG = PKO_MEM_PORT_QOS 0x8001180050001018 MEMORY 8
REG_FIELD = PKO_MEM_PORT_QOS qos_mask 60 53, eid 9 6, pid 5 0

REG = PKO_MEM_PORT_RATE0 0x8001180050001020 MEMORY 8
REG_FIELD = PKO_MEM_PORT_RATE0 rate_word 50 32, rate_pkt 31 8, pid 5 0

REG = PKO_MEM_PORT_RATE1 0x8001180050001028 MEMORY 8
REG_FIELD = PKO_MEM_PORT_RATE1 rate_lim 31 8, pid 5 0

REG = PKO_MEM_QUEUE_PTRS 0x8001180050001000 MEMORY 8
REG_FIELD = PKO_MEM_QUEUE_PTRS s_tail 63 63, static_p 62 62, static_q 61 61, qos_mask 60 53, buf_ptr 52 17, tail 16 16, index 15 13, port 12 7, queue 6 0

REG = PKO_MEM_QUEUE_QOS 0x8001180050001008 MEMORY 8
REG_FIELD = PKO_MEM_QUEUE_QOS qos_mask 60 53, pid 12 7, qid 6 0

REG = PKO_REG_BIST_RESULT 0x8001180050000080 MEMORY 8
REG_FIELD = PKO_REG_BIST_RESULT csr 29 29, iob 28 28, out_dat 27 27, out_ctl 26 26, out_sta 25 25, out_wif 24 24, prt_chk 23 21, prt_nxt 20 20, prt_psb 19 12, ncb_inb 11 11, prt_qcb 10 10, prt_qsb 9 8, prt_ctl 7 6, dat_dat 5 4, dat_ptr 3 0

REG = PKO_REG_CMD_BUF 0x8001180050000010 MEMORY 8
REG_FIELD = PKO_REG_CMD_BUF pool 22 20, size 12 0

REG = PKO_REG_DEBUG0 0x8001180050000098 MEMORY 8
REG_FIELD = PKO_REG_DEBUG0 asserts 63 0

REG = PKO_REG_DEBUG1 0x80011800500000A0 MEMORY 8
REG_FIELD = PKO_REG_DEBUG1 asserts 63 0

REG = PKO_REG_DEBUG2 0x80011800500000A8 MEMORY 8
REG_FIELD = PKO_REG_DEBUG2 asserts 63 0

REG = PKO_REG_DEBUG3 0x80011800500000B0 MEMORY 8
REG_FIELD = PKO_REG_DEBUG3 asserts 63 0

REG = PKO_REG_ENGINE_INFLIGHT 0x8001180050000050 MEMORY 8
REG_FIELD = PKO_REG_ENGINE_INFLIGHT engine13 55 52, engine12 51 48, engine11 47 44, engine10 43 40, engine9 39 36, engine8 35 32, engine7 31 28, engine6 27 24, engine5 23 20, engine4 19 16, engine3 15 12, engine2 11 8, engine1 7 4, engine0 3 0

REG = PKO_REG_ENGINE_THRESH 0x8001180050000058 MEMORY 8
REG_FIELD = PKO_REG_ENGINE_THRESH mask 13 0

REG = PKO_REG_ERROR 0x8001180050000088 MEMORY 8
REG_FIELD = PKO_REG_ERROR currzero 2 2, doorbell 1 1, parity 0 0

REG = PKO_REG_FLAGS 0x8001180050000000 MEMORY 8
REG_FIELD = PKO_REG_FLAGS dis_perf3 8 8, dis_perf2 7 7, reset 3 3, store_be 2 2, ena_dwb 1 1, ena_pko 0 0

REG = PKO_REG_GMX_PORT_MODE 0x8001180050000018 MEMORY 8
REG_FIELD = PKO_REG_GMX_PORT_MODE mode1 5 3, mode0 2 0

REG = PKO_REG_INT_MASK 0x8001180050000090 MEMORY 8
REG_FIELD = PKO_REG_INT_MASK currzero 2 2, doorbell 1 1, parity 0 0

REG = PKO_REG_PREEMPT 0x8001180050000110 MEMORY 8
REG_FIELD = PKO_REG_PREEMPT min_size 15 0

REG = PKO_REG_QUEUE_MODE 0x8001180050000048 MEMORY 8
REG_FIELD = PKO_REG_QUEUE_MODE mode 1 0

REG = PKO_REG_QUEUE_PREEMPT 0x8001180050000108 MEMORY 8
REG_FIELD = PKO_REG_QUEUE_PREEMPT preemptee 1 1, preempter 0 0

REG = PKO_REG_QUEUE_PTRS1 0x8001180050000100 MEMORY 8
REG_FIELD = PKO_REG_QUEUE_PTRS1 idx3 1 1, qid7 0 0

REG = PKO_REG_READ_IDX 0x8001180050000008 MEMORY 8
REG_FIELD = PKO_REG_READ_IDX inc 15 8, index 7 0

REG = PKO_REG_TIMESTAMP 0x8001180050000060 MEMORY 8
REG_FIELD = PKO_REG_TIMESTAMP wqe_word 3 0

REG = POW_BIST_STAT 0x80016700000003F8 MEMORY 8
REG_FIELD = POW_BIST_STAT cam 11 11, nbr 9 8, nbt 5 4, index 3 3, fidx 2 2, pend 1 1, adr 0 0

REG = POW_DS_PC 0x8001670000000398 MEMORY 8
REG_FIELD = POW_DS_PC ds_pc 31 0

REG = POW_ECC_ERR 0x8001670000000218 MEMORY 8
REG_FIELD = POW_ECC_ERR iop_ie 44 32, iop 28 16, rpe_ie 13 13, rpe 12 12, syn 8 4, dbe_ie 3 3, sbe_ie 2 2, dbe 1 1, sbe 0 0

REG = POW_INT_CTL 0x8001670000000220 MEMORY 8
REG_FIELD = POW_INT_CTL pfr_dis 5 5, nbr_thr 4 0

REG = POW_IQ_CNT0 0x8001670000000340 MEMORY 8
REG_FIELD = POW_IQ_CNT0 iq_cnt 31 0
REG = POW_IQ_CNT1 0x8001670000000348 MEMORY 8
REG_FIELD = POW_IQ_CNT1 iq_cnt 31 0
REG = POW_IQ_CNT2 0x8001670000000350 MEMORY 8
REG_FIELD = POW_IQ_CNT2 iq_cnt 31 0
REG = POW_IQ_CNT3 0x8001670000000358 MEMORY 8
REG_FIELD = POW_IQ_CNT3 iq_cnt 31 0
REG = POW_IQ_CNT4 0x8001670000000360 MEMORY 8
REG_FIELD = POW_IQ_CNT4 iq_cnt 31 0
REG = POW_IQ_CNT5 0x8001670000000368 MEMORY 8
REG_FIELD = POW_IQ_CNT5 iq_cnt 31 0
REG = POW_IQ_CNT6 0x8001670000000370 MEMORY 8
REG_FIELD = POW_IQ_CNT6 iq_cnt 31 0
REG = POW_IQ_CNT7 0x8001670000000378 MEMORY 8
REG_FIELD = POW_IQ_CNT7 iq_cnt 31 0

REG = POW_IQ_COM_CNT 0x8001670000000388 MEMORY 8
REG_FIELD = POW_IQ_COM_CNT iq_cnt 31 0

REG = POW_IQ_INT 0x8001670000000238 MEMORY 8
REG_FIELD = POW_IQ_INT iq_int 7 0

REG = POW_IQ_INT_EN 0x8001670000000240 MEMORY 8
REG_FIELD = POW_IQ_INT_EN int_en 7 0

REG = POW_IQ_THR0 0x80016700000003A0 MEMORY 8
REG_FIELD = POW_IQ_THR0 iq_thr 31 0
REG = POW_IQ_THR1 0x80016700000003A8 MEMORY 8
REG_FIELD = POW_IQ_THR1 iq_thr 31 0
REG = POW_IQ_THR2 0x80016700000003B0 MEMORY 8
REG_FIELD = POW_IQ_THR2 iq_thr 31 0
REG = POW_IQ_THR3 0x80016700000003B8 MEMORY 8
REG_FIELD = POW_IQ_THR3 iq_thr 31 0
REG = POW_IQ_THR4 0x80016700000003C0 MEMORY 8
REG_FIELD = POW_IQ_THR4 iq_thr 31 0
REG = POW_IQ_THR5 0x80016700000003C8 MEMORY 8
REG_FIELD = POW_IQ_THR5 iq_thr 31 0
REG = POW_IQ_THR6 0x80016700000003D0 MEMORY 8
REG_FIELD = POW_IQ_THR6 iq_thr 31 0
REG = POW_IQ_THR7 0x80016700000003D8 MEMORY 8
REG_FIELD = POW_IQ_THR7 iq_thr 31 0

REG = POW_NOS_CNT 0x8001670000000228 MEMORY 8
REG_FIELD = POW_NOS_CNT nos_cnt 9 0

REG = POW_NW_TIM 0x8001670000000210 MEMORY 8
REG_FIELD = POW_NW_TIM nw_tim 9 0

REG = POW_PF_RST_MSK 0x8001670000000230 MEMORY 8
REG_FIELD = POW_PF_RST_MSK rst_msk 7 0

REG = POW_PP_GRP_MSK0 0x8001670000000000 MEMORY 8
REG_FIELD = POW_PP_GRP_MSK0 qos7_pri 47 44, qos6_pri 43 40, qos5_pri 39 36, qos4_pri 35 32, qos3_pri 31 28, qos2_pri 27 24, qos1_pri 23 20, qos0_pri 19 16, grp_msk 15 0
REG = POW_PP_GRP_MSK1 0x8001670000000008 MEMORY 8
REG_FIELD = POW_PP_GRP_MSK1 qos7_pri 47 44, qos6_pri 43 40, qos5_pri 39 36, qos4_pri 35 32, qos3_pri 31 28, qos2_pri 27 24, qos1_pri 23 20, qos0_pri 19 16, grp_msk 15 0
REG = POW_PP_GRP_MSK2 0x8001670000000010 MEMORY 8
REG_FIELD = POW_PP_GRP_MSK2 qos7_pri 47 44, qos6_pri 43 40, qos5_pri 39 36, qos4_pri 35 32, qos3_pri 31 28, qos2_pri 27 24, qos1_pri 23 20, qos0_pri 19 16, grp_msk 15 0
REG = POW_PP_GRP_MSK3 0x8001670000000018 MEMORY 8
REG_FIELD = POW_PP_GRP_MSK3 qos7_pri 47 44, qos6_pri 43 40, qos5_pri 39 36, qos4_pri 35 32, qos3_pri 31 28, qos2_pri 27 24, qos1_pri 23 20, qos0_pri 19 16, grp_msk 15 0

REG = POW_QOS_RND0 0x80016700000001C0 MEMORY 8
REG_FIELD = POW_QOS_RND0 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND1 0x80016700000001C8 MEMORY 8
REG_FIELD = POW_QOS_RND1 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND2 0x80016700000001D0 MEMORY 8
REG_FIELD = POW_QOS_RND2 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND3 0x80016700000001D8 MEMORY 8
REG_FIELD = POW_QOS_RND3 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND4 0x80016700000001E0 MEMORY 8
REG_FIELD = POW_QOS_RND4 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND5 0x80016700000001E8 MEMORY 8
REG_FIELD = POW_QOS_RND5 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND6 0x80016700000001F0 MEMORY 8
REG_FIELD = POW_QOS_RND6 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0
REG = POW_QOS_RND7 0x80016700000001F8 MEMORY 8
REG_FIELD = POW_QOS_RND7 rnd_p3 31 24, rnd_p2 23 16, rnd_p1 15 8, rnd 7 0

REG = POW_QOS_THR0 0x8001670000000180 MEMORY 8
REG_FIELD = POW_QOS_THR0 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR1 0x8001670000000188 MEMORY 8
REG_FIELD = POW_QOS_THR1 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR2 0x8001670000000190 MEMORY 8
REG_FIELD = POW_QOS_THR2 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR3 0x8001670000000198 MEMORY 8
REG_FIELD = POW_QOS_THR3 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR4 0x80016700000001A0 MEMORY 8
REG_FIELD = POW_QOS_THR4 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR5 0x80016700000001A8 MEMORY 8
REG_FIELD = POW_QOS_THR5 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR6 0x80016700000001B0 MEMORY 8
REG_FIELD = POW_QOS_THR6 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0
REG = POW_QOS_THR7 0x80016700000001B8 MEMORY 8
REG_FIELD = POW_QOS_THR7 des_cnt 57 48, buf_cnt 45 36, free_cnt 33 24, max_thr 20 12, min_thr 8 0

REG = POW_TS_PC 0x8001670000000390 MEMORY 8
REG_FIELD = POW_TS_PC ts_pc 31 0

REG = POW_WA_COM_PC 0x8001670000000380 MEMORY 8
REG_FIELD = POW_WA_COM_PC wa_pc 31 0

REG = POW_WA_PC0 0x8001670000000300 MEMORY 8
REG_FIELD = POW_WA_PC0 wa_pc 31 0
REG = POW_WA_PC1 0x8001670000000308 MEMORY 8
REG_FIELD = POW_WA_PC1 wa_pc 31 0
REG = POW_WA_PC2 0x8001670000000310 MEMORY 8
REG_FIELD = POW_WA_PC2 wa_pc 31 0
REG = POW_WA_PC3 0x8001670000000318 MEMORY 8
REG_FIELD = POW_WA_PC3 wa_pc 31 0
REG = POW_WA_PC4 0x8001670000000320 MEMORY 8
REG_FIELD = POW_WA_PC4 wa_pc 31 0
REG = POW_WA_PC5 0x8001670000000328 MEMORY 8
REG_FIELD = POW_WA_PC5 wa_pc 31 0
REG = POW_WA_PC6 0x8001670000000330 MEMORY 8
REG_FIELD = POW_WA_PC6 wa_pc 31 0
REG = POW_WA_PC7 0x8001670000000338 MEMORY 8
REG_FIELD = POW_WA_PC7 wa_pc 31 0

REG = POW_WQ_INT 0x8001670000000200 MEMORY 8
REG_FIELD = POW_WQ_INT iq_dis 31 16, wq_int 15 0

REG = POW_WQ_INT_CNT0 0x8001670000000100 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT0 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT1 0x8001670000000108 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT1 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT2 0x8001670000000110 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT2 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT3 0x8001670000000118 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT3 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT4 0x8001670000000120 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT4 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT5 0x8001670000000128 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT5 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT6 0x8001670000000130 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT6 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT7 0x8001670000000138 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT7 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT8 0x8001670000000140 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT8 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT9 0x8001670000000148 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT9 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT10 0x8001670000000150 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT10 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT11 0x8001670000000158 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT11 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT12 0x8001670000000160 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT12 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT13 0x8001670000000168 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT13 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT14 0x8001670000000170 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT14 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0
REG = POW_WQ_INT_CNT15 0x8001670000000178 MEMORY 8
REG_FIELD = POW_WQ_INT_CNT15 tc_cnt 27 24, ds_cnt 21 12, iq_cnt 9 0

REG = POW_WQ_INT_PC 0x8001670000000208 MEMORY 8
REG_FIELD = POW_WQ_INT_PC pc 59 32, pc_thr 27 8

REG = POW_WQ_INT_THR0 0x8001670000000080 MEMORY 8
REG_FIELD = POW_WQ_INT_THR0 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR1 0x8001670000000088 MEMORY 8
REG_FIELD = POW_WQ_INT_THR1 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR2 0x8001670000000090 MEMORY 8
REG_FIELD = POW_WQ_INT_THR2 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR3 0x8001670000000098 MEMORY 8
REG_FIELD = POW_WQ_INT_THR3 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR4 0x80016700000000A0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR4 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR5 0x80016700000000A8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR5 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR6 0x80016700000000B0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR6 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR7 0x80016700000000B8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR7 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR8 0x80016700000000C0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR8 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR9 0x80016700000000C8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR9 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR10 0x80016700000000D0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR10 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR11 0x80016700000000D8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR11 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR12 0x80016700000000E0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR12 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR13 0x80016700000000E8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR13 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR14 0x80016700000000F0 MEMORY 8
REG_FIELD = POW_WQ_INT_THR14 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0
REG = POW_WQ_INT_THR15 0x80016700000000F8 MEMORY 8
REG_FIELD = POW_WQ_INT_THR15 tc_en 28 28, tc_thr 27 24, ds_thr 20 12, iq_thr 8 0

REG = POW_WS_PC0 0x8001670000000280 MEMORY 8
REG_FIELD = POW_WS_PC0 ws_pc 31 0
REG = POW_WS_PC1 0x8001670000000288 MEMORY 8
REG_FIELD = POW_WS_PC1 ws_pc 31 0
REG = POW_WS_PC2 0x8001670000000290 MEMORY 8
REG_FIELD = POW_WS_PC2 ws_pc 31 0
REG = POW_WS_PC3 0x8001670000000298 MEMORY 8
REG_FIELD = POW_WS_PC3 ws_pc 31 0
REG = POW_WS_PC4 0x80016700000002A0 MEMORY 8
REG_FIELD = POW_WS_PC4 ws_pc 31 0
REG = POW_WS_PC5 0x80016700000002A8 MEMORY 8
REG_FIELD = POW_WS_PC5 ws_pc 31 0
REG = POW_WS_PC6 0x80016700000002B0 MEMORY 8
REG_FIELD = POW_WS_PC6 ws_pc 31 0
REG = POW_WS_PC7 0x80016700000002B8 MEMORY 8
REG_FIELD = POW_WS_PC7 ws_pc 31 0
REG = POW_WS_PC8 0x80016700000002C0 MEMORY 8
REG_FIELD = POW_WS_PC8 ws_pc 31 0
REG = POW_WS_PC9 0x80016700000002C8 MEMORY 8
REG_FIELD = POW_WS_PC9 ws_pc 31 0
REG = POW_WS_PC10 0x80016700000002D0 MEMORY 8
REG_FIELD = POW_WS_PC10 ws_pc 31 0
REG = POW_WS_PC11 0x80016700000002D8 MEMORY 8
REG_FIELD = POW_WS_PC11 ws_pc 31 0
REG = POW_WS_PC12 0x80016700000002E0 MEMORY 8
REG_FIELD = POW_WS_PC12 ws_pc 31 0
REG = POW_WS_PC13 0x80016700000002E8 MEMORY 8
REG_FIELD = POW_WS_PC13 ws_pc 31 0
REG = POW_WS_PC14 0x80016700000002F0 MEMORY 8
REG_FIELD = POW_WS_PC14 ws_pc 31 0
REG = POW_WS_PC15 0x80016700000002F8 MEMORY 8
REG_FIELD = POW_WS_PC15 ws_pc 31 0

REG = RAD_MEM_DEBUG0 0x8001180070001000 MEMORY 8
REG_FIELD = RAD_MEM_DEBUG0 iword 63 0

REG = RAD_MEM_DEBUG1 0x8001180070001008 MEMORY 8
REG_FIELD = RAD_MEM_DEBUG1 p_dat 63 0

REG = RAD_MEM_DEBUG2 0x8001180070001010 MEMORY 8
REG_FIELD = RAD_MEM_DEBUG2 q_dat 63 0

REG = RAD_REG_BIST_RESULT 0x8001180070000080 MEMORY 8
REG_FIELD = RAD_REG_BIST_RESULT sta 7 7, ncb_oub 6 6, ncb_inb 5 4, dat 3 0

REG = RAD_REG_CMD_BUF 0x8001180070000008 MEMORY 8
REG_FIELD = RAD_REG_CMD_BUF dwb 57 49, pool 48 46, size 45 33, ptr 32 0

REG = RAD_REG_CTL 0x8001180070000000 MEMORY 8
REG_FIELD = RAD_REG_CTL wc_dis 6 6, max_read 5 2, store_le 1 1, reset 0 0

REG = RAD_REG_DEBUG0 0x8001180070000100 MEMORY 8
REG_FIELD = RAD_REG_DEBUG0 loop 56 32, iridx 21 16, iwidx 13 8, owordqv 7 7, owordpv 6 6, commit 5 5, state 4 0

REG = RAD_REG_DEBUG1 0x8001180070000108 MEMORY 8
REG_FIELD = RAD_REG_DEBUG1 cword 63 0

REG = RAD_REG_DEBUG10 0x8001180070000150 MEMORY 8
REG_FIELD = RAD_REG_DEBUG10 flags 63 56, size 55 40, ptr 39 0

REG = RAD_REG_DEBUG11 0x8001180070000158 MEMORY 8
REG_FIELD = RAD_REG_DEBUG11 q 12 12, p 11 11, wc 10 10, eod 9 9, sod 8 8, index 7 0

REG = RAD_REG_DEBUG12 0x8001180070000160 MEMORY 8
REG_FIELD = RAD_REG_DEBUG12 asserts 14 0

REG = RAD_REG_DEBUG2 0x8001180070000110 MEMORY 8
REG_FIELD = RAD_REG_DEBUG2 owordp 63 0

REG = RAD_REG_DEBUG3 0x8001180070000118 MEMORY 8
REG_FIELD = RAD_REG_DEBUG3 owordq 63 0

REG = RAD_REG_DEBUG4 0x8001180070000120 MEMORY 8
REG_FIELD = RAD_REG_DEBUG4 rword 63 0

REG = RAD_REG_DEBUG5 0x8001180070000128 MEMORY 8
REG_FIELD = RAD_REG_DEBUG5 niropc7 52 50, nirque7 49 48, nirval7 47 43, niropc6 42 40, nirque6 39 38, nirarb6 37 37, nirval6 36 32, niridx1 31 28, niwidx1 27 24, niridx0 23 20, niwidx0 19 16, wccreds 15 14, fpacreds 13 12, powcreds 9 8, n1creds 7 4, n0creds 3 0

REG = RAD_REG_DEBUG6 0x8001180070000130 MEMORY 8
REG_FIELD = RAD_REG_DEBUG6 cnt 63 56, size 55 40, ptr 39 0

REG = RAD_REG_DEBUG7 0x8001180070000138 MEMORY 8
REG_FIELD = RAD_REG_DEBUG7 cnt 14 0

REG = RAD_REG_DEBUG8 0x8001180070000140 MEMORY 8
REG_FIELD = RAD_REG_DEBUG8 flags 63 56, size 55 40, ptr 39 0

REG = RAD_REG_DEBUG9 0x8001180070000148 MEMORY 8
REG_FIELD = RAD_REG_DEBUG9 eod 19 19, ini 18 18, q 17 17, p 16 16, mul 15 8, index 7 0

REG = RAD_REG_ERROR 0x8001180070000088 MEMORY 8
REG_FIELD = RAD_REG_ERROR doorbell 0 0

REG = RAD_REG_INT_MASK 0x8001180070000090 MEMORY 8
REG_FIELD = RAD_REG_INT_MASK doorbell 0 0

REG = RAD_REG_POLYNOMIAL 0x8001180070000010 MEMORY 8
REG_FIELD = RAD_REG_POLYNOMIAL coeffs 7 0

REG = RAD_REG_READ_IDX 0x8001180070000018 MEMORY 8
REG_FIELD = RAD_REG_READ_IDX inc 15 8, index 7 0

REG = RNM_BIST_STATUS 0x8001180040000008 MEMORY 8
REG_FIELD = RNM_BIST_STATUS rrc 1 1, mem 0 0

REG = RNM_CTL_STATUS 0x8001180040000000 MEMORY 8
REG_FIELD = RNM_CTL_STATUS dis_mak 11 11, eer_lck 10 10, eer_val 9 9, ent_sel 8 5, exp_ent 4 4, rng_rst 3 3, rnm_rst 2 2, rng_en 1 1, ent_en 0 0

REG = RNM_EER_DBG 0x8001180040000018 MEMORY 8
REG_FIELD = RNM_EER_DBG dat 63 0

REG = RNM_EER_KEY 0x8001180040000010 MEMORY 8
REG_FIELD = RNM_EER_KEY key 63 0

REG = RNM_SERIAL_NUM 0x8001180040000020 MEMORY 8
REG_FIELD = RNM_SERIAL_NUM dat 63 0

REG = RST_BOOT 0x8001180006001600 MEMORY 8
REG_FIELD = RST_BOOT chipkill 63 63, jtcsrdis 62 62, ejtagdis 61 61, romen 60 60, ckill_ppdis 59 59, jt_tstmode 58 58, vrm_err 57 57, c_mul 36 30, pnr_mul 29 24, lboot_oci 20 18, lboot_ext 17 12, lboot 11 2, rboot 1 1, rboot_pin 0 0

REG = RST_CFG 0x8001180006001610 MEMORY 8
REG_FIELD = RST_CFG bist_delay 63 6, cntl_clr_bist 2 2, warm_clr_bist 1 1, soft_clr_bist 0 0

REG = RST_CKILL 0x8001180006001638 MEMORY 8
REG_FIELD = RST_CKILL timer 46 0

REG = RST_CTL0 0x8001180006001640 MEMORY 8
REG_FIELD = RST_CTL0 prst_link 9 9, rst_done 8 8, rst_link 7 7, host_mode 6 6, rst_drv 3 3, rst_rcv 2 2, rst_chip 1 1, rst_val 0 0
REG = RST_CTL1 0x8001180006001648 MEMORY 8
REG_FIELD = RST_CTL1 prst_link 9 9, rst_done 8 8, rst_link 7 7, host_mode 6 6, rst_drv 3 3, rst_rcv 2 2, rst_chip 1 1, rst_val 0 0
REG = RST_CTL2 0x8001180006001650 MEMORY 8
REG_FIELD = RST_CTL2 prst_link 9 9, rst_done 8 8, rst_link 7 7, host_mode 6 6, rst_drv 3 3, rst_rcv 2 2, rst_chip 1 1, rst_val 0 0

REG = RST_DELAY 0x8001180006001608 MEMORY 8
REG_FIELD = RST_DELAY warm_rst_dly 31 16, soft_rst_dly 15 0

REG = RST_INT 0x8001180006001628 MEMORY 8
REG_FIELD = RST_INT perst 10 8, rst_link 2 0

REG = RST_PP_POWER 0x8001180006001700 MEMORY 8
REG_FIELD = RST_PP_POWER gate 3 0

REG = RST_SOFT_PRST0 0x80011800060016C0 MEMORY 8
REG_FIELD = RST_SOFT_PRST0 soft_prst 0 0
REG = RST_SOFT_PRST1 0x80011800060016C8 MEMORY 8
REG_FIELD = RST_SOFT_PRST1 soft_prst 0 0
REG = RST_SOFT_PRST2 0x80011800060016D0 MEMORY 8
REG_FIELD = RST_SOFT_PRST2 soft_prst 0 0

REG = RST_SOFT_RST 0x8001180006001680 MEMORY 8
REG_FIELD = RST_SOFT_RST soft_rst 0 0

REG = SATA_UAHC_GBL_BISTAFR 0x80016C00000000A0 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_BISTAFR ncp 15 8, pd 7 0

REG = SATA_UAHC_GBL_BISTCR 0x80016C00000000A4 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_BISTCR old_phy_ready 25 25, late_phy_ready 24 24, ferlib 20 20, txo 18 18, cntclr 17 17, nealb 16 16, llb 15 15, errlossen 13 13, sdfe 12 12, llc 10 8, erren 6 6, flip 5 5, pv 4 4, pattern 3 0

REG = SATA_UAHC_GBL_BISTDECR 0x80016C00000000B0 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_BISTDECR dwerr 31 0

REG = SATA_UAHC_GBL_BISTFCTR 0x80016C00000000A8 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_BISTFCTR count 31 0

REG = SATA_UAHC_GBL_BISTSR 0x80016C00000000AC MEMORY 4
REG_FIELD = SATA_UAHC_GBL_BISTSR brsterr 23 16, framerr 15 0

REG = SATA_UAHC_GBL_CAP 0x80016C0000000000 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_CAP s64a 31 31, sncq 30 30, ssntf 29 29, smps 28 28, sss 27 27, salp 26 26, sal 25 25, sclo 24 24, iss 23 20, snzo 19 19, sam 18 18, spm 17 17, fbss 16 16, pmd 15 15, ssc 14 14, psc 13 13, ncs 12 8, cccs 7 7, ems 6 6, sxs 5 5, np 4 0

REG = SATA_UAHC_GBL_CAP2 0x80016C0000000024 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_CAP2 deso 5 5, sadm 4 4, sds 3 3, apst 2 2, nvmp 1 1, boh 0 0

REG = SATA_UAHC_GBL_CCC_CTL 0x80016C0000000014 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_CCC_CTL tv 31 16, cc 15 8, intr 7 3, en 0 0

REG = SATA_UAHC_GBL_CCC_PORTS 0x80016C0000000018 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_CCC_PORTS prt 1 0

REG = SATA_UAHC_GBL_GHC 0x80016C0000000004 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_GHC ae 31 31, ie 1 1, hr 0 0

REG = SATA_UAHC_GBL_GPARAM1R 0x80016C00000000E8 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_GPARAM1R align_m 31 31, rx_buffer 30 30, phy_data 29 28, phy_rst 27 27, phy_ctrl 26 21, phy_stat 20 15, latch_m 14 14, phy_type 13 11, return_err 10 10, ahb_endian 9 8, s_haddr 7 7, m_haddr 6 6, s_hdata 5 3, m_hdata 2 0

REG = SATA_UAHC_GBL_GPARAM2R 0x80016C00000000EC MEMORY 4
REG_FIELD = SATA_UAHC_GBL_GPARAM2R bist_m 19 19, fbs_mem_s 18 18, fbs_pmpn 17 16, fbs_support 15 15, dev_cp 14 14, dev_mp 13 13, encode_m 12 12, rxoob_clk_m 11 11, rx_oob_m 10 10, tx_oob_m 9 9, rxoob_clk 8 0

REG = SATA_UAHC_GBL_IDR 0x80016C00000000FC MEMORY 4
REG_FIELD = SATA_UAHC_GBL_IDR id 31 0

REG = SATA_UAHC_GBL_IS 0x80016C0000000008 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_IS ips 2 0

REG = SATA_UAHC_GBL_OOBR 0x80016C00000000BC MEMORY 4
REG_FIELD = SATA_UAHC_GBL_OOBR we 31 31, cwmin 30 24, cwmax 23 16, cimin 15 8, cimax 7 0

REG = SATA_UAHC_GBL_PI 0x80016C000000000C MEMORY 4
REG_FIELD = SATA_UAHC_GBL_PI pi 1 0

REG = SATA_UAHC_GBL_PPARAMR 0x80016C00000000F0 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_PPARAMR tx_mem_m 11 11, tx_mem_s 10 10, rx_mem_m 9 9, rx_mem_s 8 8, txfifo_depth 7 4, rxfifo_depth 3 0

REG = SATA_UAHC_GBL_TESTR 0x80016C00000000F4 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_TESTR psel 18 16, test_if 0 0

REG = SATA_UAHC_GBL_TIMER1MS 0x80016C00000000E0 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_TIMER1MS timv 19 0

REG = SATA_UAHC_GBL_VERSIONR 0x80016C00000000F8 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_VERSIONR ver 31 0

REG = SATA_UAHC_GBL_VS 0x80016C0000000010 MEMORY 4
REG_FIELD = SATA_UAHC_GBL_VS mjr 31 16, mnr 15 0

REG = SATA_UAHC_P0_CI 0x80016C0000000138 MEMORY 4
REG_FIELD = SATA_UAHC_P0_CI ci 31 0
REG = SATA_UAHC_P1_CI 0x80016C00000001B8 MEMORY 4
REG_FIELD = SATA_UAHC_P1_CI ci 31 0

REG = SATA_UAHC_P0_CLB 0x80016C0000000100 MEMORY 8
REG_FIELD = SATA_UAHC_P0_CLB clb 63 10
REG = SATA_UAHC_P1_CLB 0x80016C0000000180 MEMORY 8
REG_FIELD = SATA_UAHC_P1_CLB clb 63 10

REG = SATA_UAHC_P0_CMD 0x80016C0000000118 MEMORY 4
REG_FIELD = SATA_UAHC_P0_CMD icc 31 28, asp 27 27, alpe 26 26, dlae 25 25, atapi 24 24, apste 23 23, fbscp 22 22, esp 21 21, cpd 20 20, mpsp 19 19, hpcp 18 18, pma 17 17, cps 16 16, cr 15 15, fr 14 14, mpss 13 13, ccs 12 8, fre 4 4, clo 3 3, pod 2 2, sud 1 1, st 0 0
REG = SATA_UAHC_P1_CMD 0x80016C0000000198 MEMORY 4
REG_FIELD = SATA_UAHC_P1_CMD icc 31 28, asp 27 27, alpe 26 26, dlae 25 25, atapi 24 24, apste 23 23, fbscp 22 22, esp 21 21, cpd 20 20, mpsp 19 19, hpcp 18 18, pma 17 17, cps 16 16, cr 15 15, fr 14 14, mpss 13 13, ccs 12 8, fre 4 4, clo 3 3, pod 2 2, sud 1 1, st 0 0

REG = SATA_UAHC_P0_DMACR 0x80016C0000000170 MEMORY 4
REG_FIELD = SATA_UAHC_P0_DMACR rxts 7 4, txts 3 0
REG = SATA_UAHC_P1_DMACR 0x80016C00000001F0 MEMORY 4
REG_FIELD = SATA_UAHC_P1_DMACR rxts 7 4, txts 3 0

REG = SATA_UAHC_P0_FB 0x80016C0000000108 MEMORY 8
REG_FIELD = SATA_UAHC_P0_FB fb 63 8
REG = SATA_UAHC_P1_FB 0x80016C0000000188 MEMORY 8
REG_FIELD = SATA_UAHC_P1_FB fb 63 8

REG = SATA_UAHC_P0_FBS 0x80016C0000000140 MEMORY 4
REG_FIELD = SATA_UAHC_P0_FBS dwe 19 16, ado 15 12, dev 11 8, sde 2 2, dec 1 1, en 0 0
REG = SATA_UAHC_P1_FBS 0x80016C00000001C0 MEMORY 4
REG_FIELD = SATA_UAHC_P1_FBS dwe 19 16, ado 15 12, dev 11 8, sde 2 2, dec 1 1, en 0 0

REG = SATA_UAHC_P0_IE 0x80016C0000000114 MEMORY 4
REG_FIELD = SATA_UAHC_P0_IE cpde 31 31, tfee 30 30, hbfe 29 29, hbde 28 28, ife 27 27, infe 26 26, ofe 24 24, impe 23 23, prce 22 22, dmpe 7 7, pce 6 6, dpe 5 5, ufe 4 4, sdbe 3 3, dse 2 2, pse 1 1, dhre 0 0
REG = SATA_UAHC_P1_IE 0x80016C0000000194 MEMORY 4
REG_FIELD = SATA_UAHC_P1_IE cpde 31 31, tfee 30 30, hbfe 29 29, hbde 28 28, ife 27 27, infe 26 26, ofe 24 24, impe 23 23, prce 22 22, dmpe 7 7, pce 6 6, dpe 5 5, ufe 4 4, sdbe 3 3, dse 2 2, pse 1 1, dhre 0 0

REG = SATA_UAHC_P0_IS 0x80016C0000000110 MEMORY 4
REG_FIELD = SATA_UAHC_P0_IS cpds 31 31, tfes 30 30, hbfs 29 29, hbds 28 28, ifs 27 27, infs 26 26, ofs 24 24, imps 23 23, prcs 22 22, dmps 7 7, pcs 6 6, dps 5 5, ufs 4 4, sdbs 3 3, dss 2 2, pss 1 1, dhrs 0 0
REG = SATA_UAHC_P1_IS 0x80016C0000000190 MEMORY 4
REG_FIELD = SATA_UAHC_P1_IS cpds 31 31, tfes 30 30, hbfs 29 29, hbds 28 28, ifs 27 27, infs 26 26, ofs 24 24, imps 23 23, prcs 22 22, dmps 7 7, pcs 6 6, dps 5 5, ufs 4 4, sdbs 3 3, dss 2 2, pss 1 1, dhrs 0 0

REG = SATA_UAHC_P0_PHYCR 0x80016C0000000178 MEMORY 4
REG_FIELD = SATA_UAHC_P0_PHYCR ctrl 31 0
REG = SATA_UAHC_P1_PHYCR 0x80016C00000001F8 MEMORY 4
REG_FIELD = SATA_UAHC_P1_PHYCR ctrl 31 0

REG = SATA_UAHC_P0_PHYSR 0x80016C000000017C MEMORY 4
REG_FIELD = SATA_UAHC_P0_PHYSR stat 31 0
REG = SATA_UAHC_P1_PHYSR 0x80016C00000001FC MEMORY 4
REG_FIELD = SATA_UAHC_P1_PHYSR stat 31 0

REG = SATA_UAHC_P0_SACT 0x80016C0000000134 MEMORY 4
REG_FIELD = SATA_UAHC_P0_SACT ds 31 0
REG = SATA_UAHC_P1_SACT 0x80016C00000001B4 MEMORY 4
REG_FIELD = SATA_UAHC_P1_SACT ds 31 0

REG = SATA_UAHC_P0_SCTL 0x80016C000000012C MEMORY 4
REG_FIELD = SATA_UAHC_P0_SCTL ipm 9 8, spd 5 4, det 2 0
REG = SATA_UAHC_P1_SCTL 0x80016C00000001AC MEMORY 4
REG_FIELD = SATA_UAHC_P1_SCTL ipm 9 8, spd 5 4, det 2 0

REG = SATA_UAHC_P0_SERR 0x80016C0000000130 MEMORY 4
REG_FIELD = SATA_UAHC_P0_SERR diag_x 26 26, diag_f 25 25, diag_t 24 24, diag_s 23 23, diag_h 22 22, diag_c 21 21, diag_d 20 20, diag_b 19 19, diag_w 18 18, diag_i 17 17, diag_n 16 16, err_e 11 11, err_p 10 10, err_c 9 9, err_t 8 8, err_m 1 1, err_i 0 0
REG = SATA_UAHC_P1_SERR 0x80016C00000001B0 MEMORY 4
REG_FIELD = SATA_UAHC_P1_SERR diag_x 26 26, diag_f 25 25, diag_t 24 24, diag_s 23 23, diag_h 22 22, diag_c 21 21, diag_d 20 20, diag_b 19 19, diag_w 18 18, diag_i 17 17, diag_n 16 16, err_e 11 11, err_p 10 10, err_c 9 9, err_t 8 8, err_m 1 1, err_i 0 0

REG = SATA_UAHC_P0_SIG 0x80016C0000000124 MEMORY 4
REG_FIELD = SATA_UAHC_P0_SIG sig 31 0
REG = SATA_UAHC_P1_SIG 0x80016C00000001A4 MEMORY 4
REG_FIELD = SATA_UAHC_P1_SIG sig 31 0

REG = SATA_UAHC_P0_SNTF 0x80016C000000013C MEMORY 4
REG_FIELD = SATA_UAHC_P0_SNTF pmn 15 0
REG = SATA_UAHC_P1_SNTF 0x80016C00000001BC MEMORY 4
REG_FIELD = SATA_UAHC_P1_SNTF pmn 15 0

REG = SATA_UAHC_P0_SSTS 0x80016C0000000128 MEMORY 4
REG_FIELD = SATA_UAHC_P0_SSTS ipm 11 8, spd 7 4, det 3 0
REG = SATA_UAHC_P1_SSTS 0x80016C00000001A8 MEMORY 4
REG_FIELD = SATA_UAHC_P1_SSTS ipm 11 8, spd 7 4, det 3 0

REG = SATA_UAHC_P0_TFD 0x80016C0000000120 MEMORY 4
REG_FIELD = SATA_UAHC_P0_TFD tferr 15 8, sts 7 0
REG = SATA_UAHC_P1_TFD 0x80016C00000001A0 MEMORY 4
REG_FIELD = SATA_UAHC_P1_TFD tferr 15 8, sts 7 0

REG = SATA_UCTL_BIST_STATUS 0x800118006C000008 MEMORY 8
REG_FIELD = SATA_UCTL_BIST_STATUS uctl_xm_r_bist_ndone 41 41, uctl_xm_w_bist_ndone 40 40, uahc_p0_rxram_bist_n 35 35, uahc_p1_rxram_bist_n 34 34, uahc_p0_txram_bist_n 33 33, uahc_p1_txram_bist_n 32 32, uctl_xm_r_bist_statu 9 9, uctl_xm_w_bist_statu 8 8, uahc_p0_rxram_bist_s 3 3, uahc_p1_rxram_bist_s 2 2, uahc_p0_txram_bist_s 1 1, uahc_p1_txram_bist_s 0 0

REG = SATA_UCTL_CTL 0x800118006C000000 MEMORY 8
REG_FIELD = SATA_UCTL_CTL clear_bist 63 63, start_bist 62 62, a_clk_en 30 30, a_clk_byp_sel 29 29, a_clkdiv_rst 28 28, a_clkdiv_sel 26 24, csclk_en 4 4, sata_uahc_rst 1 1, sata_uctl_rst 0 0

REG = SATA_UCTL_INTSTAT 0x800118006C000030 MEMORY 8
REG_FIELD = SATA_UCTL_INTSTAT xm_bad_dma 2 2, xs_ncb_oob 1 1

REG = SATA_UCTL_SHIM_CFG 0x800118006C0000E8 MEMORY 8
REG_FIELD = SATA_UCTL_SHIM_CFG xs_ncb_oob_wrn 63 63, xs_ncb_oob_osrc 56 48, xm_bad_dma_wrn 47 47, xm_bad_dma_type 43 40, dma_read_cmd 12 12, dma_endian_mode 9 8, csr_endian_mode 1 0

REG = SATA_UCTL_SPARE0 0x800118006C000010 MEMORY 8
REG_FIELD = SATA_UCTL_SPARE0 reserved_0_63 0 63

REG = SATA_UCTL_SPARE1 0x800118006C0000F8 MEMORY 8
REG_FIELD = SATA_UCTL_SPARE1 reserved_0_63 0 63

REG = SLI_BIST_STATUS 0x80011F0000010580 MEMORY 8
REG_FIELD = SLI_BIST_STATUS n2p0_c 30 30, n2p0_o 29 29, cpl_p0 26 26, cpl_p1 25 25, p2n0_c0 18 18, p2n0_n 16 16, p2n0_p0 15 15, p2n1_c0 13 13, p2n1_n 11 11, p2n1_p0 10 10, dsi1_1 5 5, dsi1_0 4 4, dsi0_1 3 3, dsi0_0 2 2, msi 1 1, ncb_cmd 0 0

REG = SLI_CTL_PORT0 0x80011F0000010050 MEMORY 8
REG_FIELD = SLI_CTL_PORT0 intd 21 21, intc 20 20, intb 19 19, inta 18 18, dis_port 17 17, waitl_com 16 16, intd_map 15 14, intc_map 13 12, intb_map 11 10, inta_map 9 8, ctlp_ro 7 7, ptlp_ro 5 5, wait_com 0 0
REG = SLI_CTL_PORT1 0x80011F0000010060 MEMORY 8
REG_FIELD = SLI_CTL_PORT1 intd 21 21, intc 20 20, intb 19 19, inta 18 18, dis_port 17 17, waitl_com 16 16, intd_map 15 14, intc_map 13 12, intb_map 11 10, inta_map 9 8, ctlp_ro 7 7, ptlp_ro 5 5, wait_com 0 0
REG = SLI_CTL_PORT2 0x80011F0000010070 MEMORY 8
REG_FIELD = SLI_CTL_PORT2 intd 21 21, intc 20 20, intb 19 19, inta 18 18, dis_port 17 17, waitl_com 16 16, intd_map 15 14, intc_map 13 12, intb_map 11 10, inta_map 9 8, ctlp_ro 7 7, ptlp_ro 5 5, wait_com 0 0

REG = SLI_CTL_STATUS 0x80011F0000010570 MEMORY 8
REG_FIELD = SLI_CTL_STATUS p1_ntags 19 14, p0_ntags 13 8, chip_rev 7 0

REG = SLI_DATA_OUT_CNT 0x80011F00000105F0 MEMORY 8
REG_FIELD = SLI_DATA_OUT_CNT p1_ucnt 43 28, p1_fcnt 27 22, p0_ucnt 21 6, p0_fcnt 5 0

REG = SLI_DMA0_CNT 0x80011F0000010400 MEMORY 8
REG_FIELD = SLI_DMA0_CNT cnt 31 0
REG = SLI_DMA1_CNT 0x80011F0000010410 MEMORY 8
REG_FIELD = SLI_DMA1_CNT cnt 31 0

REG = SLI_DMA0_INT_LEVEL 0x80011F00000103E0 MEMORY 8
REG_FIELD = SLI_DMA0_INT_LEVEL time 63 32, cnt 31 0
REG = SLI_DMA1_INT_LEVEL 0x80011F00000103F0 MEMORY 8
REG_FIELD = SLI_DMA1_INT_LEVEL time 63 32, cnt 31 0

REG = SLI_DMA0_TIM 0x80011F0000010420 MEMORY 8
REG_FIELD = SLI_DMA0_TIM tim 31 0
REG = SLI_DMA1_TIM 0x80011F0000010430 MEMORY 8
REG_FIELD = SLI_DMA1_TIM tim 31 0

REG = SLI_INT_ENB_CIU 0x80011F0000013CD0 MEMORY 8
REG_FIELD = SLI_INT_ENB_CIU ill_pad 60 60, sprt3_err 59 59, sprt2_err 58 58, sprt1_err 57 57, sprt0_err 56 56, pins_err 55 55, pop_err 54 54, pdi_err 53 53, pgl_err 52 52, pin_bp 51 51, pout_err 50 50, psldbof 49 49, pidbof 48 48, dtime 37 36, dcnt 35 34, dmafi 33 32, mio_int2 28 28, m3_un_wi 27 27, m3_un_b0 26 26, m3_up_wi 25 25, m3_up_b0 24 24, m2_un_wi 23 23, m2_un_b0 22 22, m2_up_wi 21 21, m2_up_b0 20 20, mio_int1 17 17, mio_int0 16 16, m1_un_wi 15 15, m1_un_b0 14 14, m1_up_wi 13 13, m1_up_b0 12 12, m0_un_wi 11 11, m0_un_b0 10 10, m0_up_wi 9 9, m0_up_b0 8 8, ptime 5 5, pcnt 4 4, iob2big 3 3, bar0_to 2 2, rml_to 0 0

REG = SLI_INT_ENB_PORT0 0x80011F0000010340 MEMORY 8
REG_FIELD = SLI_INT_ENB_PORT0 ill_pad 60 60, sprt3_err 59 59, sprt2_err 58 58, sprt1_err 57 57, sprt0_err 56 56, pins_err 55 55, pop_err 54 54, pdi_err 53 53, pgl_err 52 52, pin_bp 51 51, pout_err 50 50, psldbof 49 49, pidbof 48 48, dtime 37 36, dcnt 35 34, dmafi 33 32, mac2_int 29 29, mio_int2 28 28, m3_un_wi 27 27, m3_un_b0 26 26, m3_up_wi 25 25, m3_up_b0 24 24, m2_un_wi 23 23, m2_un_b0 22 22, m2_up_wi 21 21, m2_up_b0 20 20, mac1_int 19 19, mac0_int 18 18, mio_int1 17 17, mio_int0 16 16, m1_un_wi 15 15, m1_un_b0 14 14, m1_up_wi 13 13, m1_up_b0 12 12, m0_un_wi 11 11, m0_un_b0 10 10, m0_up_wi 9 9, m0_up_b0 8 8, ptime 5 5, pcnt 4 4, iob2big 3 3, bar0_to 2 2, rml_to 0 0
REG = SLI_INT_ENB_PORT1 0x80011F0000010350 MEMORY 8
REG_FIELD = SLI_INT_ENB_PORT1 ill_pad 60 60, sprt3_err 59 59, sprt2_err 58 58, sprt1_err 57 57, sprt0_err 56 56, pins_err 55 55, pop_err 54 54, pdi_err 53 53, pgl_err 52 52, pin_bp 51 51, pout_err 50 50, psldbof 49 49, pidbof 48 48, dtime 37 36, dcnt 35 34, dmafi 33 32, mac2_int 29 29, mio_int2 28 28, m3_un_wi 27 27, m3_un_b0 26 26, m3_up_wi 25 25, m3_up_b0 24 24, m2_un_wi 23 23, m2_un_b0 22 22, m2_up_wi 21 21, m2_up_b0 20 20, mac1_int 19 19, mac0_int 18 18, mio_int1 17 17, mio_int0 16 16, m1_un_wi 15 15, m1_un_b0 14 14, m1_up_wi 13 13, m1_up_b0 12 12, m0_un_wi 11 11, m0_un_b0 10 10, m0_up_wi 9 9, m0_up_b0 8 8, ptime 5 5, pcnt 4 4, iob2big 3 3, bar0_to 2 2, rml_to 0 0
REG = SLI_INT_ENB_PORT2 0x80011F0000010360 MEMORY 8
REG_FIELD = SLI_INT_ENB_PORT2 ill_pad 60 60, sprt3_err 59 59, sprt2_err 58 58, sprt1_err 57 57, sprt0_err 56 56, pins_err 55 55, pop_err 54 54, pdi_err 53 53, pgl_err 52 52, pin_bp 51 51, pout_err 50 50, psldbof 49 49, pidbof 48 48, dtime 37 36, dcnt 35 34, dmafi 33 32, mac2_int 29 29, mio_int2 28 28, m3_un_wi 27 27, m3_un_b0 26 26, m3_up_wi 25 25, m3_up_b0 24 24, m2_un_wi 23 23, m2_un_b0 22 22, m2_up_wi 21 21, m2_up_b0 20 20, mac1_int 19 19, mac0_int 18 18, mio_int1 17 17, mio_int0 16 16, m1_un_wi 15 15, m1_un_b0 14 14, m1_up_wi 13 13, m1_up_b0 12 12, m0_un_wi 11 11, m0_un_b0 10 10, m0_up_wi 9 9, m0_up_b0 8 8, ptime 5 5, pcnt 4 4, iob2big 3 3, bar0_to 2 2, rml_to 0 0

REG = SLI_INT_SUM 0x80011F0000010330 MEMORY 8
REG_FIELD = SLI_INT_SUM ill_pad 60 60, sprt3_err 59 59, sprt2_err 58 58, sprt1_err 57 57, sprt0_err 56 56, pins_err 55 55, pop_err 54 54, pdi_err 53 53, pgl_err 52 52, pin_bp 51 51, pout_err 50 50, psldbof 49 49, pidbof 48 48, dtime 37 36, dcnt 35 34, dmafi 33 32, mac2_int 29 29, mio_int2 28 28, m3_un_wi 27 27, m3_un_b0 26 26, m3_up_wi 25 25, m3_up_b0 24 24, m2_un_wi 23 23, m2_un_b0 22 22, m2_up_wi 21 21, m2_up_b0 20 20, mac1_int 19 19, mac0_int 18 18, mio_int1 17 17, mio_int0 16 16, m1_un_wi 15 15, m1_un_b0 14 14, m1_up_wi 13 13, m1_up_b0 12 12, m0_un_wi 11 11, m0_un_b0 10 10, m0_up_wi 9 9, m0_up_b0 8 8, ptime 5 5, pcnt 4 4, iob2big 3 3, bar0_to 2 2, rml_to 0 0

REG = SLI_LAST_WIN_RDATA0 0x80011F0000010600 MEMORY 8
REG_FIELD = SLI_LAST_WIN_RDATA0 data 63 0

REG = SLI_LAST_WIN_RDATA1 0x80011F0000010610 MEMORY 8
REG_FIELD = SLI_LAST_WIN_RDATA1 data 63 0

REG = SLI_LAST_WIN_RDATA2 0x80011F00000106C0 MEMORY 8
REG_FIELD = SLI_LAST_WIN_RDATA2 data 63 0

REG = SLI_LAST_WIN_RDATA3 0x80011F00000106D0 MEMORY 8
REG_FIELD = SLI_LAST_WIN_RDATA3 data 63 0

REG = SLI_MAC_CREDIT_CNT 0x80011F0000013D70 MEMORY 8
REG_FIELD = SLI_MAC_CREDIT_CNT p1_c_d 53 53, p1_n_d 52 52, p1_p_d 51 51, p0_c_d 50 50, p0_n_d 49 49, p0_p_d 48 48, p1_ccnt 47 40, p1_ncnt 39 32, p1_pcnt 31 24, p0_ccnt 23 16, p0_ncnt 15 8, p0_pcnt 7 0

REG = SLI_MAC_CREDIT_CNT2 0x80011F0000013E10 MEMORY 8
REG_FIELD = SLI_MAC_CREDIT_CNT2 p3_c_d 53 53, p3_n_d 52 52, p3_p_d 51 51, p2_c_d 50 50, p2_n_d 49 49, p2_p_d 48 48, p3_ccnt 47 40, p3_ncnt 39 32, p3_pcnt 31 24, p2_ccnt 23 16, p2_ncnt 15 8, p2_pcnt 7 0


REG = SLI_MEM_ACCESS_CTL 0x80011F00000102F0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_CTL max_word 13 10, timer 9 0

REG = SLI_MEM_ACCESS_SUBID12 0x80011F00000100E0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID12 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID13 0x80011F00000100F0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID13 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID14 0x80011F0000010100 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID14 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID15 0x80011F0000010110 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID15 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID16 0x80011F0000010120 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID16 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID17 0x80011F0000010130 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID17 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID18 0x80011F0000010140 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID18 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID19 0x80011F0000010150 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID19 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID20 0x80011F0000010160 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID20 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID21 0x80011F0000010170 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID21 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID22 0x80011F0000010180 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID22 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID23 0x80011F0000010190 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID23 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID24 0x80011F00000101A0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID24 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID25 0x80011F00000101B0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID25 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID26 0x80011F00000101C0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID26 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0
REG = SLI_MEM_ACCESS_SUBID27 0x80011F00000101D0 MEMORY 8
REG_FIELD = SLI_MEM_ACCESS_SUBID27 zero 42 42, port 41 39, nmerge 38 38, esr 37 36, esw 35 34, wtype 33 32, rtype 31 30, ba 29 0

REG = SLI_MSI_ENB0 0x80011F0000013C50 MEMORY 8
REG_FIELD = SLI_MSI_ENB0 enb 63 0

REG = SLI_MSI_ENB1 0x80011F0000013C60 MEMORY 8
REG_FIELD = SLI_MSI_ENB1 enb 63 0

REG = SLI_MSI_ENB2 0x80011F0000013C70 MEMORY 8
REG_FIELD = SLI_MSI_ENB2 enb 63 0

REG = SLI_MSI_ENB3 0x80011F0000013C80 MEMORY 8
REG_FIELD = SLI_MSI_ENB3 enb 63 0

REG = SLI_MSI_RCV0 0x80011F0000013C10 MEMORY 8
REG_FIELD = SLI_MSI_RCV0 intr 63 0

REG = SLI_MSI_RCV1 0x80011F0000013C20 MEMORY 8
REG_FIELD = SLI_MSI_RCV1 intr 63 0

REG = SLI_MSI_RCV2 0x80011F0000013C30 MEMORY 8
REG_FIELD = SLI_MSI_RCV2 intr 63 0

REG = SLI_MSI_RCV3 0x80011F0000013C40 MEMORY 8
REG_FIELD = SLI_MSI_RCV3 intr 63 0

REG = SLI_MSI_RD_MAP 0x80011F0000013CA0 MEMORY 8
REG_FIELD = SLI_MSI_RD_MAP rd_int 15 8, msi_int 7 0

REG = SLI_MSI_W1C_ENB0 0x80011F0000013CF0 MEMORY 8
REG_FIELD = SLI_MSI_W1C_ENB0 clr 63 0

REG = SLI_MSI_W1C_ENB1 0x80011F0000013D00 MEMORY 8
REG_FIELD = SLI_MSI_W1C_ENB1 clr 63 0

REG = SLI_MSI_W1C_ENB2 0x80011F0000013D10 MEMORY 8
REG_FIELD = SLI_MSI_W1C_ENB2 clr 63 0

REG = SLI_MSI_W1C_ENB3 0x80011F0000013D20 MEMORY 8
REG_FIELD = SLI_MSI_W1C_ENB3 clr 63 0

REG = SLI_MSI_W1S_ENB0 0x80011F0000013D30 MEMORY 8
REG_FIELD = SLI_MSI_W1S_ENB0 set 63 0

REG = SLI_MSI_W1S_ENB1 0x80011F0000013D40 MEMORY 8
REG_FIELD = SLI_MSI_W1S_ENB1 set 63 0

REG = SLI_MSI_W1S_ENB2 0x80011F0000013D50 MEMORY 8
REG_FIELD = SLI_MSI_W1S_ENB2 set 63 0

REG = SLI_MSI_W1S_ENB3 0x80011F0000013D60 MEMORY 8
REG_FIELD = SLI_MSI_W1S_ENB3 set 63 0

REG = SLI_MSI_WR_MAP 0x80011F0000013C90 MEMORY 8
REG_FIELD = SLI_MSI_WR_MAP ciu_int 15 8, msi_int 7 0

REG = SLI_PCIE_MSI_RCV 0x80011F0000013CB0 MEMORY 8
REG_FIELD = SLI_PCIE_MSI_RCV intr 7 0

REG = SLI_PCIE_MSI_RCV_B1 0x80011F0000010650 MEMORY 8
REG_FIELD = SLI_PCIE_MSI_RCV_B1 intr 15 8

REG = SLI_PCIE_MSI_RCV_B2 0x80011F0000010660 MEMORY 8
REG_FIELD = SLI_PCIE_MSI_RCV_B2 intr 23 16

REG = SLI_PCIE_MSI_RCV_B3 0x80011F0000010670 MEMORY 8
REG_FIELD = SLI_PCIE_MSI_RCV_B3 intr 31 24

REG = SLI_PKT0_CNTS 0x80011F0000012400 MEMORY 8
REG_FIELD = SLI_PKT0_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT1_CNTS 0x80011F0000012410 MEMORY 8
REG_FIELD = SLI_PKT1_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT2_CNTS 0x80011F0000012420 MEMORY 8
REG_FIELD = SLI_PKT2_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT3_CNTS 0x80011F0000012430 MEMORY 8
REG_FIELD = SLI_PKT3_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT4_CNTS 0x80011F0000012440 MEMORY 8
REG_FIELD = SLI_PKT4_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT5_CNTS 0x80011F0000012450 MEMORY 8
REG_FIELD = SLI_PKT5_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT6_CNTS 0x80011F0000012460 MEMORY 8
REG_FIELD = SLI_PKT6_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT7_CNTS 0x80011F0000012470 MEMORY 8
REG_FIELD = SLI_PKT7_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT8_CNTS 0x80011F0000012480 MEMORY 8
REG_FIELD = SLI_PKT8_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT9_CNTS 0x80011F0000012490 MEMORY 8
REG_FIELD = SLI_PKT9_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT10_CNTS 0x80011F00000124A0 MEMORY 8
REG_FIELD = SLI_PKT10_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT11_CNTS 0x80011F00000124B0 MEMORY 8
REG_FIELD = SLI_PKT11_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT12_CNTS 0x80011F00000124C0 MEMORY 8
REG_FIELD = SLI_PKT12_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT13_CNTS 0x80011F00000124D0 MEMORY 8
REG_FIELD = SLI_PKT13_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT14_CNTS 0x80011F00000124E0 MEMORY 8
REG_FIELD = SLI_PKT14_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT15_CNTS 0x80011F00000124F0 MEMORY 8
REG_FIELD = SLI_PKT15_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT16_CNTS 0x80011F0000012500 MEMORY 8
REG_FIELD = SLI_PKT16_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT17_CNTS 0x80011F0000012510 MEMORY 8
REG_FIELD = SLI_PKT17_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT18_CNTS 0x80011F0000012520 MEMORY 8
REG_FIELD = SLI_PKT18_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT19_CNTS 0x80011F0000012530 MEMORY 8
REG_FIELD = SLI_PKT19_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT20_CNTS 0x80011F0000012540 MEMORY 8
REG_FIELD = SLI_PKT20_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT21_CNTS 0x80011F0000012550 MEMORY 8
REG_FIELD = SLI_PKT21_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT22_CNTS 0x80011F0000012560 MEMORY 8
REG_FIELD = SLI_PKT22_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT23_CNTS 0x80011F0000012570 MEMORY 8
REG_FIELD = SLI_PKT23_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT24_CNTS 0x80011F0000012580 MEMORY 8
REG_FIELD = SLI_PKT24_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT25_CNTS 0x80011F0000012590 MEMORY 8
REG_FIELD = SLI_PKT25_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT26_CNTS 0x80011F00000125A0 MEMORY 8
REG_FIELD = SLI_PKT26_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT27_CNTS 0x80011F00000125B0 MEMORY 8
REG_FIELD = SLI_PKT27_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT28_CNTS 0x80011F00000125C0 MEMORY 8
REG_FIELD = SLI_PKT28_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT29_CNTS 0x80011F00000125D0 MEMORY 8
REG_FIELD = SLI_PKT29_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT30_CNTS 0x80011F00000125E0 MEMORY 8
REG_FIELD = SLI_PKT30_CNTS timer 53 32, cnt 31 0
REG = SLI_PKT31_CNTS 0x80011F00000125F0 MEMORY 8
REG_FIELD = SLI_PKT31_CNTS timer 53 32, cnt 31 0

REG = SLI_PKT0_IN_BP 0x80011F0000013800 MEMORY 8
REG_FIELD = SLI_PKT0_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT1_IN_BP 0x80011F0000013810 MEMORY 8
REG_FIELD = SLI_PKT1_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT2_IN_BP 0x80011F0000013820 MEMORY 8
REG_FIELD = SLI_PKT2_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT3_IN_BP 0x80011F0000013830 MEMORY 8
REG_FIELD = SLI_PKT3_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT4_IN_BP 0x80011F0000013840 MEMORY 8
REG_FIELD = SLI_PKT4_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT5_IN_BP 0x80011F0000013850 MEMORY 8
REG_FIELD = SLI_PKT5_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT6_IN_BP 0x80011F0000013860 MEMORY 8
REG_FIELD = SLI_PKT6_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT7_IN_BP 0x80011F0000013870 MEMORY 8
REG_FIELD = SLI_PKT7_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT8_IN_BP 0x80011F0000013880 MEMORY 8
REG_FIELD = SLI_PKT8_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT9_IN_BP 0x80011F0000013890 MEMORY 8
REG_FIELD = SLI_PKT9_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT10_IN_BP 0x80011F00000138A0 MEMORY 8
REG_FIELD = SLI_PKT10_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT11_IN_BP 0x80011F00000138B0 MEMORY 8
REG_FIELD = SLI_PKT11_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT12_IN_BP 0x80011F00000138C0 MEMORY 8
REG_FIELD = SLI_PKT12_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT13_IN_BP 0x80011F00000138D0 MEMORY 8
REG_FIELD = SLI_PKT13_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT14_IN_BP 0x80011F00000138E0 MEMORY 8
REG_FIELD = SLI_PKT14_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT15_IN_BP 0x80011F00000138F0 MEMORY 8
REG_FIELD = SLI_PKT15_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT16_IN_BP 0x80011F0000013900 MEMORY 8
REG_FIELD = SLI_PKT16_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT17_IN_BP 0x80011F0000013910 MEMORY 8
REG_FIELD = SLI_PKT17_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT18_IN_BP 0x80011F0000013920 MEMORY 8
REG_FIELD = SLI_PKT18_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT19_IN_BP 0x80011F0000013930 MEMORY 8
REG_FIELD = SLI_PKT19_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT20_IN_BP 0x80011F0000013940 MEMORY 8
REG_FIELD = SLI_PKT20_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT21_IN_BP 0x80011F0000013950 MEMORY 8
REG_FIELD = SLI_PKT21_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT22_IN_BP 0x80011F0000013960 MEMORY 8
REG_FIELD = SLI_PKT22_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT23_IN_BP 0x80011F0000013970 MEMORY 8
REG_FIELD = SLI_PKT23_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT24_IN_BP 0x80011F0000013980 MEMORY 8
REG_FIELD = SLI_PKT24_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT25_IN_BP 0x80011F0000013990 MEMORY 8
REG_FIELD = SLI_PKT25_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT26_IN_BP 0x80011F00000139A0 MEMORY 8
REG_FIELD = SLI_PKT26_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT27_IN_BP 0x80011F00000139B0 MEMORY 8
REG_FIELD = SLI_PKT27_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT28_IN_BP 0x80011F00000139C0 MEMORY 8
REG_FIELD = SLI_PKT28_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT29_IN_BP 0x80011F00000139D0 MEMORY 8
REG_FIELD = SLI_PKT29_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT30_IN_BP 0x80011F00000139E0 MEMORY 8
REG_FIELD = SLI_PKT30_IN_BP wmark 63 32, cnt 31 0
REG = SLI_PKT31_IN_BP 0x80011F00000139F0 MEMORY 8
REG_FIELD = SLI_PKT31_IN_BP wmark 63 32, cnt 31 0

REG = SLI_PKT0_INSTR_BADDR 0x80011F0000012800 MEMORY 8
REG_FIELD = SLI_PKT0_INSTR_BADDR addr 63 3
REG = SLI_PKT1_INSTR_BADDR 0x80011F0000012810 MEMORY 8
REG_FIELD = SLI_PKT1_INSTR_BADDR addr 63 3
REG = SLI_PKT2_INSTR_BADDR 0x80011F0000012820 MEMORY 8
REG_FIELD = SLI_PKT2_INSTR_BADDR addr 63 3
REG = SLI_PKT3_INSTR_BADDR 0x80011F0000012830 MEMORY 8
REG_FIELD = SLI_PKT3_INSTR_BADDR addr 63 3
REG = SLI_PKT4_INSTR_BADDR 0x80011F0000012840 MEMORY 8
REG_FIELD = SLI_PKT4_INSTR_BADDR addr 63 3
REG = SLI_PKT5_INSTR_BADDR 0x80011F0000012850 MEMORY 8
REG_FIELD = SLI_PKT5_INSTR_BADDR addr 63 3
REG = SLI_PKT6_INSTR_BADDR 0x80011F0000012860 MEMORY 8
REG_FIELD = SLI_PKT6_INSTR_BADDR addr 63 3
REG = SLI_PKT7_INSTR_BADDR 0x80011F0000012870 MEMORY 8
REG_FIELD = SLI_PKT7_INSTR_BADDR addr 63 3
REG = SLI_PKT8_INSTR_BADDR 0x80011F0000012880 MEMORY 8
REG_FIELD = SLI_PKT8_INSTR_BADDR addr 63 3
REG = SLI_PKT9_INSTR_BADDR 0x80011F0000012890 MEMORY 8
REG_FIELD = SLI_PKT9_INSTR_BADDR addr 63 3
REG = SLI_PKT10_INSTR_BADDR 0x80011F00000128A0 MEMORY 8
REG_FIELD = SLI_PKT10_INSTR_BADDR addr 63 3
REG = SLI_PKT11_INSTR_BADDR 0x80011F00000128B0 MEMORY 8
REG_FIELD = SLI_PKT11_INSTR_BADDR addr 63 3
REG = SLI_PKT12_INSTR_BADDR 0x80011F00000128C0 MEMORY 8
REG_FIELD = SLI_PKT12_INSTR_BADDR addr 63 3
REG = SLI_PKT13_INSTR_BADDR 0x80011F00000128D0 MEMORY 8
REG_FIELD = SLI_PKT13_INSTR_BADDR addr 63 3
REG = SLI_PKT14_INSTR_BADDR 0x80011F00000128E0 MEMORY 8
REG_FIELD = SLI_PKT14_INSTR_BADDR addr 63 3
REG = SLI_PKT15_INSTR_BADDR 0x80011F00000128F0 MEMORY 8
REG_FIELD = SLI_PKT15_INSTR_BADDR addr 63 3
REG = SLI_PKT16_INSTR_BADDR 0x80011F0000012900 MEMORY 8
REG_FIELD = SLI_PKT16_INSTR_BADDR addr 63 3
REG = SLI_PKT17_INSTR_BADDR 0x80011F0000012910 MEMORY 8
REG_FIELD = SLI_PKT17_INSTR_BADDR addr 63 3
REG = SLI_PKT18_INSTR_BADDR 0x80011F0000012920 MEMORY 8
REG_FIELD = SLI_PKT18_INSTR_BADDR addr 63 3
REG = SLI_PKT19_INSTR_BADDR 0x80011F0000012930 MEMORY 8
REG_FIELD = SLI_PKT19_INSTR_BADDR addr 63 3
REG = SLI_PKT20_INSTR_BADDR 0x80011F0000012940 MEMORY 8
REG_FIELD = SLI_PKT20_INSTR_BADDR addr 63 3
REG = SLI_PKT21_INSTR_BADDR 0x80011F0000012950 MEMORY 8
REG_FIELD = SLI_PKT21_INSTR_BADDR addr 63 3
REG = SLI_PKT22_INSTR_BADDR 0x80011F0000012960 MEMORY 8
REG_FIELD = SLI_PKT22_INSTR_BADDR addr 63 3
REG = SLI_PKT23_INSTR_BADDR 0x80011F0000012970 MEMORY 8
REG_FIELD = SLI_PKT23_INSTR_BADDR addr 63 3
REG = SLI_PKT24_INSTR_BADDR 0x80011F0000012980 MEMORY 8
REG_FIELD = SLI_PKT24_INSTR_BADDR addr 63 3
REG = SLI_PKT25_INSTR_BADDR 0x80011F0000012990 MEMORY 8
REG_FIELD = SLI_PKT25_INSTR_BADDR addr 63 3
REG = SLI_PKT26_INSTR_BADDR 0x80011F00000129A0 MEMORY 8
REG_FIELD = SLI_PKT26_INSTR_BADDR addr 63 3
REG = SLI_PKT27_INSTR_BADDR 0x80011F00000129B0 MEMORY 8
REG_FIELD = SLI_PKT27_INSTR_BADDR addr 63 3
REG = SLI_PKT28_INSTR_BADDR 0x80011F00000129C0 MEMORY 8
REG_FIELD = SLI_PKT28_INSTR_BADDR addr 63 3
REG = SLI_PKT29_INSTR_BADDR 0x80011F00000129D0 MEMORY 8
REG_FIELD = SLI_PKT29_INSTR_BADDR addr 63 3
REG = SLI_PKT30_INSTR_BADDR 0x80011F00000129E0 MEMORY 8
REG_FIELD = SLI_PKT30_INSTR_BADDR addr 63 3
REG = SLI_PKT31_INSTR_BADDR 0x80011F00000129F0 MEMORY 8
REG_FIELD = SLI_PKT31_INSTR_BADDR addr 63 3

REG = SLI_PKT0_INSTR_BAOFF_DBELL 0x80011F0000012C00 MEMORY 8
REG_FIELD = SLI_PKT0_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT1_INSTR_BAOFF_DBELL 0x80011F0000012C10 MEMORY 8
REG_FIELD = SLI_PKT1_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT2_INSTR_BAOFF_DBELL 0x80011F0000012C20 MEMORY 8
REG_FIELD = SLI_PKT2_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT3_INSTR_BAOFF_DBELL 0x80011F0000012C30 MEMORY 8
REG_FIELD = SLI_PKT3_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT4_INSTR_BAOFF_DBELL 0x80011F0000012C40 MEMORY 8
REG_FIELD = SLI_PKT4_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT5_INSTR_BAOFF_DBELL 0x80011F0000012C50 MEMORY 8
REG_FIELD = SLI_PKT5_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT6_INSTR_BAOFF_DBELL 0x80011F0000012C60 MEMORY 8
REG_FIELD = SLI_PKT6_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT7_INSTR_BAOFF_DBELL 0x80011F0000012C70 MEMORY 8
REG_FIELD = SLI_PKT7_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT8_INSTR_BAOFF_DBELL 0x80011F0000012C80 MEMORY 8
REG_FIELD = SLI_PKT8_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT9_INSTR_BAOFF_DBELL 0x80011F0000012C90 MEMORY 8
REG_FIELD = SLI_PKT9_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT10_INSTR_BAOFF_DBELL 0x80011F0000012CA0 MEMORY 8
REG_FIELD = SLI_PKT10_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT11_INSTR_BAOFF_DBELL 0x80011F0000012CB0 MEMORY 8
REG_FIELD = SLI_PKT11_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT12_INSTR_BAOFF_DBELL 0x80011F0000012CC0 MEMORY 8
REG_FIELD = SLI_PKT12_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT13_INSTR_BAOFF_DBELL 0x80011F0000012CD0 MEMORY 8
REG_FIELD = SLI_PKT13_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT14_INSTR_BAOFF_DBELL 0x80011F0000012CE0 MEMORY 8
REG_FIELD = SLI_PKT14_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT15_INSTR_BAOFF_DBELL 0x80011F0000012CF0 MEMORY 8
REG_FIELD = SLI_PKT15_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT16_INSTR_BAOFF_DBELL 0x80011F0000012D00 MEMORY 8
REG_FIELD = SLI_PKT16_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT17_INSTR_BAOFF_DBELL 0x80011F0000012D10 MEMORY 8
REG_FIELD = SLI_PKT17_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT18_INSTR_BAOFF_DBELL 0x80011F0000012D20 MEMORY 8
REG_FIELD = SLI_PKT18_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT19_INSTR_BAOFF_DBELL 0x80011F0000012D30 MEMORY 8
REG_FIELD = SLI_PKT19_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT20_INSTR_BAOFF_DBELL 0x80011F0000012D40 MEMORY 8
REG_FIELD = SLI_PKT20_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT21_INSTR_BAOFF_DBELL 0x80011F0000012D50 MEMORY 8
REG_FIELD = SLI_PKT21_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT22_INSTR_BAOFF_DBELL 0x80011F0000012D60 MEMORY 8
REG_FIELD = SLI_PKT22_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT23_INSTR_BAOFF_DBELL 0x80011F0000012D70 MEMORY 8
REG_FIELD = SLI_PKT23_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT24_INSTR_BAOFF_DBELL 0x80011F0000012D80 MEMORY 8
REG_FIELD = SLI_PKT24_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT25_INSTR_BAOFF_DBELL 0x80011F0000012D90 MEMORY 8
REG_FIELD = SLI_PKT25_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT26_INSTR_BAOFF_DBELL 0x80011F0000012DA0 MEMORY 8
REG_FIELD = SLI_PKT26_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT27_INSTR_BAOFF_DBELL 0x80011F0000012DB0 MEMORY 8
REG_FIELD = SLI_PKT27_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT28_INSTR_BAOFF_DBELL 0x80011F0000012DC0 MEMORY 8
REG_FIELD = SLI_PKT28_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT29_INSTR_BAOFF_DBELL 0x80011F0000012DD0 MEMORY 8
REG_FIELD = SLI_PKT29_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT30_INSTR_BAOFF_DBELL 0x80011F0000012DE0 MEMORY 8
REG_FIELD = SLI_PKT30_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT31_INSTR_BAOFF_DBELL 0x80011F0000012DF0 MEMORY 8
REG_FIELD = SLI_PKT31_INSTR_BAOFF_DBELL aoff 63 32, dbell 31 0

REG = SLI_PKT0_INSTR_FIFO_RSIZE 0x80011F0000013000 MEMORY 8
REG_FIELD = SLI_PKT0_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT1_INSTR_FIFO_RSIZE 0x80011F0000013010 MEMORY 8
REG_FIELD = SLI_PKT1_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT2_INSTR_FIFO_RSIZE 0x80011F0000013020 MEMORY 8
REG_FIELD = SLI_PKT2_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT3_INSTR_FIFO_RSIZE 0x80011F0000013030 MEMORY 8
REG_FIELD = SLI_PKT3_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT4_INSTR_FIFO_RSIZE 0x80011F0000013040 MEMORY 8
REG_FIELD = SLI_PKT4_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT5_INSTR_FIFO_RSIZE 0x80011F0000013050 MEMORY 8
REG_FIELD = SLI_PKT5_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT6_INSTR_FIFO_RSIZE 0x80011F0000013060 MEMORY 8
REG_FIELD = SLI_PKT6_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT7_INSTR_FIFO_RSIZE 0x80011F0000013070 MEMORY 8
REG_FIELD = SLI_PKT7_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT8_INSTR_FIFO_RSIZE 0x80011F0000013080 MEMORY 8
REG_FIELD = SLI_PKT8_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT9_INSTR_FIFO_RSIZE 0x80011F0000013090 MEMORY 8
REG_FIELD = SLI_PKT9_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT10_INSTR_FIFO_RSIZE 0x80011F00000130A0 MEMORY 8
REG_FIELD = SLI_PKT10_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT11_INSTR_FIFO_RSIZE 0x80011F00000130B0 MEMORY 8
REG_FIELD = SLI_PKT11_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT12_INSTR_FIFO_RSIZE 0x80011F00000130C0 MEMORY 8
REG_FIELD = SLI_PKT12_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT13_INSTR_FIFO_RSIZE 0x80011F00000130D0 MEMORY 8
REG_FIELD = SLI_PKT13_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT14_INSTR_FIFO_RSIZE 0x80011F00000130E0 MEMORY 8
REG_FIELD = SLI_PKT14_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT15_INSTR_FIFO_RSIZE 0x80011F00000130F0 MEMORY 8
REG_FIELD = SLI_PKT15_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT16_INSTR_FIFO_RSIZE 0x80011F0000013100 MEMORY 8
REG_FIELD = SLI_PKT16_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT17_INSTR_FIFO_RSIZE 0x80011F0000013110 MEMORY 8
REG_FIELD = SLI_PKT17_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT18_INSTR_FIFO_RSIZE 0x80011F0000013120 MEMORY 8
REG_FIELD = SLI_PKT18_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT19_INSTR_FIFO_RSIZE 0x80011F0000013130 MEMORY 8
REG_FIELD = SLI_PKT19_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT20_INSTR_FIFO_RSIZE 0x80011F0000013140 MEMORY 8
REG_FIELD = SLI_PKT20_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT21_INSTR_FIFO_RSIZE 0x80011F0000013150 MEMORY 8
REG_FIELD = SLI_PKT21_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT22_INSTR_FIFO_RSIZE 0x80011F0000013160 MEMORY 8
REG_FIELD = SLI_PKT22_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT23_INSTR_FIFO_RSIZE 0x80011F0000013170 MEMORY 8
REG_FIELD = SLI_PKT23_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT24_INSTR_FIFO_RSIZE 0x80011F0000013180 MEMORY 8
REG_FIELD = SLI_PKT24_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT25_INSTR_FIFO_RSIZE 0x80011F0000013190 MEMORY 8
REG_FIELD = SLI_PKT25_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT26_INSTR_FIFO_RSIZE 0x80011F00000131A0 MEMORY 8
REG_FIELD = SLI_PKT26_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT27_INSTR_FIFO_RSIZE 0x80011F00000131B0 MEMORY 8
REG_FIELD = SLI_PKT27_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT28_INSTR_FIFO_RSIZE 0x80011F00000131C0 MEMORY 8
REG_FIELD = SLI_PKT28_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT29_INSTR_FIFO_RSIZE 0x80011F00000131D0 MEMORY 8
REG_FIELD = SLI_PKT29_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT30_INSTR_FIFO_RSIZE 0x80011F00000131E0 MEMORY 8
REG_FIELD = SLI_PKT30_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0
REG = SLI_PKT31_INSTR_FIFO_RSIZE 0x80011F00000131F0 MEMORY 8
REG_FIELD = SLI_PKT31_INSTR_FIFO_RSIZE max 63 55, rrp 54 46, wrp 45 37, fcnt 36 32, rsize 31 0

REG = SLI_PKT0_INSTR_HEADER 0x80011F0000013400 MEMORY 8
REG_FIELD = SLI_PKT0_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT1_INSTR_HEADER 0x80011F0000013410 MEMORY 8
REG_FIELD = SLI_PKT1_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT2_INSTR_HEADER 0x80011F0000013420 MEMORY 8
REG_FIELD = SLI_PKT2_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT3_INSTR_HEADER 0x80011F0000013430 MEMORY 8
REG_FIELD = SLI_PKT3_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT4_INSTR_HEADER 0x80011F0000013440 MEMORY 8
REG_FIELD = SLI_PKT4_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT5_INSTR_HEADER 0x80011F0000013450 MEMORY 8
REG_FIELD = SLI_PKT5_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT6_INSTR_HEADER 0x80011F0000013460 MEMORY 8
REG_FIELD = SLI_PKT6_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT7_INSTR_HEADER 0x80011F0000013470 MEMORY 8
REG_FIELD = SLI_PKT7_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT8_INSTR_HEADER 0x80011F0000013480 MEMORY 8
REG_FIELD = SLI_PKT8_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT9_INSTR_HEADER 0x80011F0000013490 MEMORY 8
REG_FIELD = SLI_PKT9_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT10_INSTR_HEADER 0x80011F00000134A0 MEMORY 8
REG_FIELD = SLI_PKT10_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT11_INSTR_HEADER 0x80011F00000134B0 MEMORY 8
REG_FIELD = SLI_PKT11_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT12_INSTR_HEADER 0x80011F00000134C0 MEMORY 8
REG_FIELD = SLI_PKT12_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT13_INSTR_HEADER 0x80011F00000134D0 MEMORY 8
REG_FIELD = SLI_PKT13_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT14_INSTR_HEADER 0x80011F00000134E0 MEMORY 8
REG_FIELD = SLI_PKT14_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT15_INSTR_HEADER 0x80011F00000134F0 MEMORY 8
REG_FIELD = SLI_PKT15_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT16_INSTR_HEADER 0x80011F0000013500 MEMORY 8
REG_FIELD = SLI_PKT16_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT17_INSTR_HEADER 0x80011F0000013510 MEMORY 8
REG_FIELD = SLI_PKT17_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT18_INSTR_HEADER 0x80011F0000013520 MEMORY 8
REG_FIELD = SLI_PKT18_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT19_INSTR_HEADER 0x80011F0000013530 MEMORY 8
REG_FIELD = SLI_PKT19_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT20_INSTR_HEADER 0x80011F0000013540 MEMORY 8
REG_FIELD = SLI_PKT20_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT21_INSTR_HEADER 0x80011F0000013550 MEMORY 8
REG_FIELD = SLI_PKT21_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT22_INSTR_HEADER 0x80011F0000013560 MEMORY 8
REG_FIELD = SLI_PKT22_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT23_INSTR_HEADER 0x80011F0000013570 MEMORY 8
REG_FIELD = SLI_PKT23_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT24_INSTR_HEADER 0x80011F0000013580 MEMORY 8
REG_FIELD = SLI_PKT24_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT25_INSTR_HEADER 0x80011F0000013590 MEMORY 8
REG_FIELD = SLI_PKT25_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT26_INSTR_HEADER 0x80011F00000135A0 MEMORY 8
REG_FIELD = SLI_PKT26_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT27_INSTR_HEADER 0x80011F00000135B0 MEMORY 8
REG_FIELD = SLI_PKT27_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT28_INSTR_HEADER 0x80011F00000135C0 MEMORY 8
REG_FIELD = SLI_PKT28_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT29_INSTR_HEADER 0x80011F00000135D0 MEMORY 8
REG_FIELD = SLI_PKT29_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT30_INSTR_HEADER 0x80011F00000135E0 MEMORY 8
REG_FIELD = SLI_PKT30_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0
REG = SLI_PKT31_INSTR_HEADER 0x80011F00000135F0 MEMORY 8
REG_FIELD = SLI_PKT31_INSTR_HEADER pbp 43 43, rparmode 37 36, rskp_len 34 28, rnqos 25 25, rngrp 24 24, rntt 23 23, rntag 22 22, use_ihdr 21 21, par_mode 15 14, skp_len 12 6, nqos 3 3, ngrp 2 2, ntt 1 1, ntag 0 0

REG = SLI_PKT0_OUT_SIZE 0x80011F0000010C00 MEMORY 8
REG_FIELD = SLI_PKT0_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT1_OUT_SIZE 0x80011F0000010C10 MEMORY 8
REG_FIELD = SLI_PKT1_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT2_OUT_SIZE 0x80011F0000010C20 MEMORY 8
REG_FIELD = SLI_PKT2_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT3_OUT_SIZE 0x80011F0000010C30 MEMORY 8
REG_FIELD = SLI_PKT3_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT4_OUT_SIZE 0x80011F0000010C40 MEMORY 8
REG_FIELD = SLI_PKT4_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT5_OUT_SIZE 0x80011F0000010C50 MEMORY 8
REG_FIELD = SLI_PKT5_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT6_OUT_SIZE 0x80011F0000010C60 MEMORY 8
REG_FIELD = SLI_PKT6_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT7_OUT_SIZE 0x80011F0000010C70 MEMORY 8
REG_FIELD = SLI_PKT7_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT8_OUT_SIZE 0x80011F0000010C80 MEMORY 8
REG_FIELD = SLI_PKT8_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT9_OUT_SIZE 0x80011F0000010C90 MEMORY 8
REG_FIELD = SLI_PKT9_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT10_OUT_SIZE 0x80011F0000010CA0 MEMORY 8
REG_FIELD = SLI_PKT10_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT11_OUT_SIZE 0x80011F0000010CB0 MEMORY 8
REG_FIELD = SLI_PKT11_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT12_OUT_SIZE 0x80011F0000010CC0 MEMORY 8
REG_FIELD = SLI_PKT12_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT13_OUT_SIZE 0x80011F0000010CD0 MEMORY 8
REG_FIELD = SLI_PKT13_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT14_OUT_SIZE 0x80011F0000010CE0 MEMORY 8
REG_FIELD = SLI_PKT14_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT15_OUT_SIZE 0x80011F0000010CF0 MEMORY 8
REG_FIELD = SLI_PKT15_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT16_OUT_SIZE 0x80011F0000010D00 MEMORY 8
REG_FIELD = SLI_PKT16_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT17_OUT_SIZE 0x80011F0000010D10 MEMORY 8
REG_FIELD = SLI_PKT17_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT18_OUT_SIZE 0x80011F0000010D20 MEMORY 8
REG_FIELD = SLI_PKT18_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT19_OUT_SIZE 0x80011F0000010D30 MEMORY 8
REG_FIELD = SLI_PKT19_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT20_OUT_SIZE 0x80011F0000010D40 MEMORY 8
REG_FIELD = SLI_PKT20_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT21_OUT_SIZE 0x80011F0000010D50 MEMORY 8
REG_FIELD = SLI_PKT21_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT22_OUT_SIZE 0x80011F0000010D60 MEMORY 8
REG_FIELD = SLI_PKT22_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT23_OUT_SIZE 0x80011F0000010D70 MEMORY 8
REG_FIELD = SLI_PKT23_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT24_OUT_SIZE 0x80011F0000010D80 MEMORY 8
REG_FIELD = SLI_PKT24_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT25_OUT_SIZE 0x80011F0000010D90 MEMORY 8
REG_FIELD = SLI_PKT25_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT26_OUT_SIZE 0x80011F0000010DA0 MEMORY 8
REG_FIELD = SLI_PKT26_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT27_OUT_SIZE 0x80011F0000010DB0 MEMORY 8
REG_FIELD = SLI_PKT27_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT28_OUT_SIZE 0x80011F0000010DC0 MEMORY 8
REG_FIELD = SLI_PKT28_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT29_OUT_SIZE 0x80011F0000010DD0 MEMORY 8
REG_FIELD = SLI_PKT29_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT30_OUT_SIZE 0x80011F0000010DE0 MEMORY 8
REG_FIELD = SLI_PKT30_OUT_SIZE isize 22 16, bsize 15 0
REG = SLI_PKT31_OUT_SIZE 0x80011F0000010DF0 MEMORY 8
REG_FIELD = SLI_PKT31_OUT_SIZE isize 22 16, bsize 15 0

REG = SLI_PKT0_SLIST_BADDR 0x80011F0000011400 MEMORY 8
REG_FIELD = SLI_PKT0_SLIST_BADDR addr 63 4
REG = SLI_PKT1_SLIST_BADDR 0x80011F0000011410 MEMORY 8
REG_FIELD = SLI_PKT1_SLIST_BADDR addr 63 4
REG = SLI_PKT2_SLIST_BADDR 0x80011F0000011420 MEMORY 8
REG_FIELD = SLI_PKT2_SLIST_BADDR addr 63 4
REG = SLI_PKT3_SLIST_BADDR 0x80011F0000011430 MEMORY 8
REG_FIELD = SLI_PKT3_SLIST_BADDR addr 63 4
REG = SLI_PKT4_SLIST_BADDR 0x80011F0000011440 MEMORY 8
REG_FIELD = SLI_PKT4_SLIST_BADDR addr 63 4
REG = SLI_PKT5_SLIST_BADDR 0x80011F0000011450 MEMORY 8
REG_FIELD = SLI_PKT5_SLIST_BADDR addr 63 4
REG = SLI_PKT6_SLIST_BADDR 0x80011F0000011460 MEMORY 8
REG_FIELD = SLI_PKT6_SLIST_BADDR addr 63 4
REG = SLI_PKT7_SLIST_BADDR 0x80011F0000011470 MEMORY 8
REG_FIELD = SLI_PKT7_SLIST_BADDR addr 63 4
REG = SLI_PKT8_SLIST_BADDR 0x80011F0000011480 MEMORY 8
REG_FIELD = SLI_PKT8_SLIST_BADDR addr 63 4
REG = SLI_PKT9_SLIST_BADDR 0x80011F0000011490 MEMORY 8
REG_FIELD = SLI_PKT9_SLIST_BADDR addr 63 4
REG = SLI_PKT10_SLIST_BADDR 0x80011F00000114A0 MEMORY 8
REG_FIELD = SLI_PKT10_SLIST_BADDR addr 63 4
REG = SLI_PKT11_SLIST_BADDR 0x80011F00000114B0 MEMORY 8
REG_FIELD = SLI_PKT11_SLIST_BADDR addr 63 4
REG = SLI_PKT12_SLIST_BADDR 0x80011F00000114C0 MEMORY 8
REG_FIELD = SLI_PKT12_SLIST_BADDR addr 63 4
REG = SLI_PKT13_SLIST_BADDR 0x80011F00000114D0 MEMORY 8
REG_FIELD = SLI_PKT13_SLIST_BADDR addr 63 4
REG = SLI_PKT14_SLIST_BADDR 0x80011F00000114E0 MEMORY 8
REG_FIELD = SLI_PKT14_SLIST_BADDR addr 63 4
REG = SLI_PKT15_SLIST_BADDR 0x80011F00000114F0 MEMORY 8
REG_FIELD = SLI_PKT15_SLIST_BADDR addr 63 4
REG = SLI_PKT16_SLIST_BADDR 0x80011F0000011500 MEMORY 8
REG_FIELD = SLI_PKT16_SLIST_BADDR addr 63 4
REG = SLI_PKT17_SLIST_BADDR 0x80011F0000011510 MEMORY 8
REG_FIELD = SLI_PKT17_SLIST_BADDR addr 63 4
REG = SLI_PKT18_SLIST_BADDR 0x80011F0000011520 MEMORY 8
REG_FIELD = SLI_PKT18_SLIST_BADDR addr 63 4
REG = SLI_PKT19_SLIST_BADDR 0x80011F0000011530 MEMORY 8
REG_FIELD = SLI_PKT19_SLIST_BADDR addr 63 4
REG = SLI_PKT20_SLIST_BADDR 0x80011F0000011540 MEMORY 8
REG_FIELD = SLI_PKT20_SLIST_BADDR addr 63 4
REG = SLI_PKT21_SLIST_BADDR 0x80011F0000011550 MEMORY 8
REG_FIELD = SLI_PKT21_SLIST_BADDR addr 63 4
REG = SLI_PKT22_SLIST_BADDR 0x80011F0000011560 MEMORY 8
REG_FIELD = SLI_PKT22_SLIST_BADDR addr 63 4
REG = SLI_PKT23_SLIST_BADDR 0x80011F0000011570 MEMORY 8
REG_FIELD = SLI_PKT23_SLIST_BADDR addr 63 4
REG = SLI_PKT24_SLIST_BADDR 0x80011F0000011580 MEMORY 8
REG_FIELD = SLI_PKT24_SLIST_BADDR addr 63 4
REG = SLI_PKT25_SLIST_BADDR 0x80011F0000011590 MEMORY 8
REG_FIELD = SLI_PKT25_SLIST_BADDR addr 63 4
REG = SLI_PKT26_SLIST_BADDR 0x80011F00000115A0 MEMORY 8
REG_FIELD = SLI_PKT26_SLIST_BADDR addr 63 4
REG = SLI_PKT27_SLIST_BADDR 0x80011F00000115B0 MEMORY 8
REG_FIELD = SLI_PKT27_SLIST_BADDR addr 63 4
REG = SLI_PKT28_SLIST_BADDR 0x80011F00000115C0 MEMORY 8
REG_FIELD = SLI_PKT28_SLIST_BADDR addr 63 4
REG = SLI_PKT29_SLIST_BADDR 0x80011F00000115D0 MEMORY 8
REG_FIELD = SLI_PKT29_SLIST_BADDR addr 63 4
REG = SLI_PKT30_SLIST_BADDR 0x80011F00000115E0 MEMORY 8
REG_FIELD = SLI_PKT30_SLIST_BADDR addr 63 4
REG = SLI_PKT31_SLIST_BADDR 0x80011F00000115F0 MEMORY 8
REG_FIELD = SLI_PKT31_SLIST_BADDR addr 63 4

REG = SLI_PKT0_SLIST_BAOFF_DBELL 0x80011F0000011800 MEMORY 8
REG_FIELD = SLI_PKT0_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT1_SLIST_BAOFF_DBELL 0x80011F0000011810 MEMORY 8
REG_FIELD = SLI_PKT1_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT2_SLIST_BAOFF_DBELL 0x80011F0000011820 MEMORY 8
REG_FIELD = SLI_PKT2_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT3_SLIST_BAOFF_DBELL 0x80011F0000011830 MEMORY 8
REG_FIELD = SLI_PKT3_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT4_SLIST_BAOFF_DBELL 0x80011F0000011840 MEMORY 8
REG_FIELD = SLI_PKT4_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT5_SLIST_BAOFF_DBELL 0x80011F0000011850 MEMORY 8
REG_FIELD = SLI_PKT5_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT6_SLIST_BAOFF_DBELL 0x80011F0000011860 MEMORY 8
REG_FIELD = SLI_PKT6_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT7_SLIST_BAOFF_DBELL 0x80011F0000011870 MEMORY 8
REG_FIELD = SLI_PKT7_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT8_SLIST_BAOFF_DBELL 0x80011F0000011880 MEMORY 8
REG_FIELD = SLI_PKT8_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT9_SLIST_BAOFF_DBELL 0x80011F0000011890 MEMORY 8
REG_FIELD = SLI_PKT9_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT10_SLIST_BAOFF_DBELL 0x80011F00000118A0 MEMORY 8
REG_FIELD = SLI_PKT10_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT11_SLIST_BAOFF_DBELL 0x80011F00000118B0 MEMORY 8
REG_FIELD = SLI_PKT11_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT12_SLIST_BAOFF_DBELL 0x80011F00000118C0 MEMORY 8
REG_FIELD = SLI_PKT12_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT13_SLIST_BAOFF_DBELL 0x80011F00000118D0 MEMORY 8
REG_FIELD = SLI_PKT13_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT14_SLIST_BAOFF_DBELL 0x80011F00000118E0 MEMORY 8
REG_FIELD = SLI_PKT14_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT15_SLIST_BAOFF_DBELL 0x80011F00000118F0 MEMORY 8
REG_FIELD = SLI_PKT15_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT16_SLIST_BAOFF_DBELL 0x80011F0000011900 MEMORY 8
REG_FIELD = SLI_PKT16_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT17_SLIST_BAOFF_DBELL 0x80011F0000011910 MEMORY 8
REG_FIELD = SLI_PKT17_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT18_SLIST_BAOFF_DBELL 0x80011F0000011920 MEMORY 8
REG_FIELD = SLI_PKT18_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT19_SLIST_BAOFF_DBELL 0x80011F0000011930 MEMORY 8
REG_FIELD = SLI_PKT19_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT20_SLIST_BAOFF_DBELL 0x80011F0000011940 MEMORY 8
REG_FIELD = SLI_PKT20_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT21_SLIST_BAOFF_DBELL 0x80011F0000011950 MEMORY 8
REG_FIELD = SLI_PKT21_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT22_SLIST_BAOFF_DBELL 0x80011F0000011960 MEMORY 8
REG_FIELD = SLI_PKT22_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT23_SLIST_BAOFF_DBELL 0x80011F0000011970 MEMORY 8
REG_FIELD = SLI_PKT23_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT24_SLIST_BAOFF_DBELL 0x80011F0000011980 MEMORY 8
REG_FIELD = SLI_PKT24_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT25_SLIST_BAOFF_DBELL 0x80011F0000011990 MEMORY 8
REG_FIELD = SLI_PKT25_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT26_SLIST_BAOFF_DBELL 0x80011F00000119A0 MEMORY 8
REG_FIELD = SLI_PKT26_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT27_SLIST_BAOFF_DBELL 0x80011F00000119B0 MEMORY 8
REG_FIELD = SLI_PKT27_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT28_SLIST_BAOFF_DBELL 0x80011F00000119C0 MEMORY 8
REG_FIELD = SLI_PKT28_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT29_SLIST_BAOFF_DBELL 0x80011F00000119D0 MEMORY 8
REG_FIELD = SLI_PKT29_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT30_SLIST_BAOFF_DBELL 0x80011F00000119E0 MEMORY 8
REG_FIELD = SLI_PKT30_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0
REG = SLI_PKT31_SLIST_BAOFF_DBELL 0x80011F00000119F0 MEMORY 8
REG_FIELD = SLI_PKT31_SLIST_BAOFF_DBELL aoff 63 32, dbell 31 0

REG = SLI_PKT0_SLIST_FIFO_RSIZE 0x80011F0000011C00 MEMORY 8
REG_FIELD = SLI_PKT0_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT1_SLIST_FIFO_RSIZE 0x80011F0000011C10 MEMORY 8
REG_FIELD = SLI_PKT1_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT2_SLIST_FIFO_RSIZE 0x80011F0000011C20 MEMORY 8
REG_FIELD = SLI_PKT2_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT3_SLIST_FIFO_RSIZE 0x80011F0000011C30 MEMORY 8
REG_FIELD = SLI_PKT3_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT4_SLIST_FIFO_RSIZE 0x80011F0000011C40 MEMORY 8
REG_FIELD = SLI_PKT4_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT5_SLIST_FIFO_RSIZE 0x80011F0000011C50 MEMORY 8
REG_FIELD = SLI_PKT5_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT6_SLIST_FIFO_RSIZE 0x80011F0000011C60 MEMORY 8
REG_FIELD = SLI_PKT6_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT7_SLIST_FIFO_RSIZE 0x80011F0000011C70 MEMORY 8
REG_FIELD = SLI_PKT7_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT8_SLIST_FIFO_RSIZE 0x80011F0000011C80 MEMORY 8
REG_FIELD = SLI_PKT8_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT9_SLIST_FIFO_RSIZE 0x80011F0000011C90 MEMORY 8
REG_FIELD = SLI_PKT9_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT10_SLIST_FIFO_RSIZE 0x80011F0000011CA0 MEMORY 8
REG_FIELD = SLI_PKT10_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT11_SLIST_FIFO_RSIZE 0x80011F0000011CB0 MEMORY 8
REG_FIELD = SLI_PKT11_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT12_SLIST_FIFO_RSIZE 0x80011F0000011CC0 MEMORY 8
REG_FIELD = SLI_PKT12_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT13_SLIST_FIFO_RSIZE 0x80011F0000011CD0 MEMORY 8
REG_FIELD = SLI_PKT13_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT14_SLIST_FIFO_RSIZE 0x80011F0000011CE0 MEMORY 8
REG_FIELD = SLI_PKT14_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT15_SLIST_FIFO_RSIZE 0x80011F0000011CF0 MEMORY 8
REG_FIELD = SLI_PKT15_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT16_SLIST_FIFO_RSIZE 0x80011F0000011D00 MEMORY 8
REG_FIELD = SLI_PKT16_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT17_SLIST_FIFO_RSIZE 0x80011F0000011D10 MEMORY 8
REG_FIELD = SLI_PKT17_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT18_SLIST_FIFO_RSIZE 0x80011F0000011D20 MEMORY 8
REG_FIELD = SLI_PKT18_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT19_SLIST_FIFO_RSIZE 0x80011F0000011D30 MEMORY 8
REG_FIELD = SLI_PKT19_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT20_SLIST_FIFO_RSIZE 0x80011F0000011D40 MEMORY 8
REG_FIELD = SLI_PKT20_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT21_SLIST_FIFO_RSIZE 0x80011F0000011D50 MEMORY 8
REG_FIELD = SLI_PKT21_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT22_SLIST_FIFO_RSIZE 0x80011F0000011D60 MEMORY 8
REG_FIELD = SLI_PKT22_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT23_SLIST_FIFO_RSIZE 0x80011F0000011D70 MEMORY 8
REG_FIELD = SLI_PKT23_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT24_SLIST_FIFO_RSIZE 0x80011F0000011D80 MEMORY 8
REG_FIELD = SLI_PKT24_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT25_SLIST_FIFO_RSIZE 0x80011F0000011D90 MEMORY 8
REG_FIELD = SLI_PKT25_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT26_SLIST_FIFO_RSIZE 0x80011F0000011DA0 MEMORY 8
REG_FIELD = SLI_PKT26_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT27_SLIST_FIFO_RSIZE 0x80011F0000011DB0 MEMORY 8
REG_FIELD = SLI_PKT27_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT28_SLIST_FIFO_RSIZE 0x80011F0000011DC0 MEMORY 8
REG_FIELD = SLI_PKT28_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT29_SLIST_FIFO_RSIZE 0x80011F0000011DD0 MEMORY 8
REG_FIELD = SLI_PKT29_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT30_SLIST_FIFO_RSIZE 0x80011F0000011DE0 MEMORY 8
REG_FIELD = SLI_PKT30_SLIST_FIFO_RSIZE rsize 31 0
REG = SLI_PKT31_SLIST_FIFO_RSIZE 0x80011F0000011DF0 MEMORY 8
REG_FIELD = SLI_PKT31_SLIST_FIFO_RSIZE rsize 31 0

REG = SLI_PKT_CNT_INT 0x80011F0000011130 MEMORY 8
REG_FIELD = SLI_PKT_CNT_INT port 31 0

REG = SLI_PKT_CNT_INT_ENB 0x80011F0000011150 MEMORY 8
REG_FIELD = SLI_PKT_CNT_INT_ENB port 31 0

REG = SLI_PKT_CTL 0x80011F0000011220 MEMORY 8
REG_FIELD = SLI_PKT_CTL ring_en 4 4, pkt_bp 3 0

REG = SLI_PKT_DATA_OUT_ES 0x80011F00000110B0 MEMORY 8
REG_FIELD = SLI_PKT_DATA_OUT_ES es 63 0

REG = SLI_PKT_DATA_OUT_NS 0x80011F00000110A0 MEMORY 8
REG_FIELD = SLI_PKT_DATA_OUT_NS nsr 31 0

REG = SLI_PKT_DATA_OUT_ROR 0x80011F0000011090 MEMORY 8
REG_FIELD = SLI_PKT_DATA_OUT_ROR ror 31 0

REG = SLI_PKT_DPADDR 0x80011F0000011080 MEMORY 8
REG_FIELD = SLI_PKT_DPADDR dptr 31 0

REG = SLI_PKT_IN_BP 0x80011F0000011210 MEMORY 8
REG_FIELD = SLI_PKT_IN_BP bp 31 0

REG = SLI_PKT_IN_DONE0_CNTS 0x80011F0000012000 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE0_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE1_CNTS 0x80011F0000012010 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE1_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE2_CNTS 0x80011F0000012020 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE2_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE3_CNTS 0x80011F0000012030 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE3_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE4_CNTS 0x80011F0000012040 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE4_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE5_CNTS 0x80011F0000012050 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE5_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE6_CNTS 0x80011F0000012060 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE6_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE7_CNTS 0x80011F0000012070 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE7_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE8_CNTS 0x80011F0000012080 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE8_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE9_CNTS 0x80011F0000012090 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE9_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE10_CNTS 0x80011F00000120A0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE10_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE11_CNTS 0x80011F00000120B0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE11_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE12_CNTS 0x80011F00000120C0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE12_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE13_CNTS 0x80011F00000120D0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE13_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE14_CNTS 0x80011F00000120E0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE14_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE15_CNTS 0x80011F00000120F0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE15_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE16_CNTS 0x80011F0000012100 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE16_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE17_CNTS 0x80011F0000012110 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE17_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE18_CNTS 0x80011F0000012120 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE18_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE19_CNTS 0x80011F0000012130 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE19_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE20_CNTS 0x80011F0000012140 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE20_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE21_CNTS 0x80011F0000012150 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE21_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE22_CNTS 0x80011F0000012160 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE22_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE23_CNTS 0x80011F0000012170 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE23_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE24_CNTS 0x80011F0000012180 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE24_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE25_CNTS 0x80011F0000012190 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE25_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE26_CNTS 0x80011F00000121A0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE26_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE27_CNTS 0x80011F00000121B0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE27_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE28_CNTS 0x80011F00000121C0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE28_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE29_CNTS 0x80011F00000121D0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE29_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE30_CNTS 0x80011F00000121E0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE30_CNTS cnt 31 0
REG = SLI_PKT_IN_DONE31_CNTS 0x80011F00000121F0 MEMORY 8
REG_FIELD = SLI_PKT_IN_DONE31_CNTS cnt 31 0

REG = SLI_PKT_IN_INSTR_COUNTS 0x80011F0000011200 MEMORY 8
REG_FIELD = SLI_PKT_IN_INSTR_COUNTS wr_cnt 63 32, rd_cnt 31 0

REG = SLI_PKT_IN_PCIE_PORT 0x80011F00000111B0 MEMORY 8
REG_FIELD = SLI_PKT_IN_PCIE_PORT pp 63 0

REG = SLI_PKT_INPUT_CONTROL 0x80011F0000011170 MEMORY 8
REG_FIELD = SLI_PKT_INPUT_CONTROL prd_erst 63 63, prd_rds 62 56, gii_erst 55 55, gii_rds 54 48, prc_idle 40 40, pin_rst 23 23, pkt_rr 22 22, pbp_dhi 21 9, d_nsr 8 8, d_esr 7 6, d_ror 5 5, use_csr 4 4, nsr 3 3, esr 2 1, ror 0 0

REG = SLI_PKT_INSTR_ENB 0x80011F0000011000 MEMORY 8
REG_FIELD = SLI_PKT_INSTR_ENB enb 31 0

REG = SLI_PKT_INSTR_RD_SIZE 0x80011F00000111A0 MEMORY 8
REG_FIELD = SLI_PKT_INSTR_RD_SIZE rdsize 63 0

REG = SLI_PKT_INSTR_SIZE 0x80011F0000011020 MEMORY 8
REG_FIELD = SLI_PKT_INSTR_SIZE is_64b 31 0

REG = SLI_PKT_INT_LEVELS 0x80011F0000011120 MEMORY 8
REG_FIELD = SLI_PKT_INT_LEVELS time 53 32, cnt 31 0

REG = SLI_PKT_IPTR 0x80011F0000011070 MEMORY 8
REG_FIELD = SLI_PKT_IPTR iptr 31 0

REG = SLI_PKT_OUT_BMODE 0x80011F00000110D0 MEMORY 8
REG_FIELD = SLI_PKT_OUT_BMODE bmode 31 0

REG = SLI_PKT_OUT_ENB 0x80011F0000011010 MEMORY 8
REG_FIELD = SLI_PKT_OUT_ENB enb 31 0

REG = SLI_PKT_OUTPUT_WMARK 0x80011F0000011180 MEMORY 8
REG_FIELD = SLI_PKT_OUTPUT_WMARK wmark 31 0

REG = SLI_PKT_PCIE_PORT 0x80011F00000110E0 MEMORY 8
REG_FIELD = SLI_PKT_PCIE_PORT pp 63 0

REG = SLI_PKT_PORT_IN_RST 0x80011F00000111F0 MEMORY 8
REG_FIELD = SLI_PKT_PORT_IN_RST in_rst 63 32, out_rst 31 0

REG = SLI_PKT_SLIST_ES 0x80011F0000011050 MEMORY 8
REG_FIELD = SLI_PKT_SLIST_ES es 63 0

REG = SLI_PKT_SLIST_NS 0x80011F0000011040 MEMORY 8
REG_FIELD = SLI_PKT_SLIST_NS nsr 31 0

REG = SLI_PKT_SLIST_ROR 0x80011F0000011030 MEMORY 8
REG_FIELD = SLI_PKT_SLIST_ROR ror 31 0

REG = SLI_PKT_TIME_INT 0x80011F0000011140 MEMORY 8
REG_FIELD = SLI_PKT_TIME_INT port 31 0

REG = SLI_PKT_TIME_INT_ENB 0x80011F0000011160 MEMORY 8
REG_FIELD = SLI_PKT_TIME_INT_ENB port 31 0

REG = SLI_S2M_PORT0_CTL 0x80011F0000013D80 MEMORY 8
REG_FIELD = SLI_S2M_PORT0_CTL wind_d 4 4, bar0_d 3 3, mrrs 2 0
REG = SLI_S2M_PORT1_CTL 0x80011F0000013D90 MEMORY 8
REG_FIELD = SLI_S2M_PORT1_CTL wind_d 4 4, bar0_d 3 3, mrrs 2 0
REG = SLI_S2M_PORT2_CTL 0x80011F0000013DA0 MEMORY 8
REG_FIELD = SLI_S2M_PORT2_CTL wind_d 4 4, bar0_d 3 3, mrrs 2 0

REG = SLI_SCRATCH_1 0x80011F00000103C0 MEMORY 8
REG_FIELD = SLI_SCRATCH_1 data 63 0

REG = SLI_SCRATCH_2 0x80011F00000103D0 MEMORY 8
REG_FIELD = SLI_SCRATCH_2 data 63 0

REG = SLI_STATE1 0x80011F0000010620 MEMORY 8
REG_FIELD = SLI_STATE1 cpl1 63 52, cpl0 51 40, arb 39 39, csr 38 0

REG = SLI_STATE2 0x80011F0000010630 MEMORY 8
REG_FIELD = SLI_STATE2 nnp1 55 48, rac 46 46, csm1 45 31, csm0 30 16, nnp0 15 8, nnd 7 0

REG = SLI_STATE3 0x80011F0000010640 MEMORY 8
REG_FIELD = SLI_STATE3 psm1 55 41, psm0 40 26, nsm1 25 13, nsm0 12 0






REG = SLI_WINDOW_CTL 0x80011F00000102E0 MEMORY 8
REG_FIELD = SLI_WINDOW_CTL time 31 0

REG = SMI0_CLK 0x8001180000001818 MEMORY 8
REG_FIELD = SMI0_CLK mode 24 24, sample_hi 20 16, sample_mode 15 15, clk_idle 13 13, preamble 12 12, sample 11 8, phase 7 0
REG = SMI1_CLK 0x8001180000001918 MEMORY 8
REG_FIELD = SMI1_CLK mode 24 24, sample_hi 20 16, sample_mode 15 15, clk_idle 13 13, preamble 12 12, sample 11 8, phase 7 0

REG = SMI0_CMD 0x8001180000001800 MEMORY 8
REG_FIELD = SMI0_CMD phy_op 17 16, phy_adr 12 8, reg_adr 4 0
REG = SMI1_CMD 0x8001180000001900 MEMORY 8
REG_FIELD = SMI1_CMD phy_op 17 16, phy_adr 12 8, reg_adr 4 0

REG = SMI0_EN 0x8001180000001820 MEMORY 8
REG_FIELD = SMI0_EN en 0 0
REG = SMI1_EN 0x8001180000001920 MEMORY 8
REG_FIELD = SMI1_EN en 0 0

REG = SMI0_RD_DAT 0x8001180000001810 MEMORY 8
REG_FIELD = SMI0_RD_DAT pending 17 17, val 16 16, dat 15 0
REG = SMI1_RD_DAT 0x8001180000001910 MEMORY 8
REG_FIELD = SMI1_RD_DAT pending 17 17, val 16 16, dat 15 0

REG = SMI0_WR_DAT 0x8001180000001808 MEMORY 8
REG_FIELD = SMI0_WR_DAT pending 17 17, val 16 16, dat 15 0
REG = SMI1_WR_DAT 0x8001180000001908 MEMORY 8
REG_FIELD = SMI1_WR_DAT pending 17 17, val 16 16, dat 15 0

REG = SMI_DRV_CTL 0x8001180000001828 MEMORY 8
REG_FIELD = SMI_DRV_CTL pctl 10 8, nctl 2 0

REG = TIM_MEM_DEBUG0 0x8001180058001100 MEMORY 8
REG_FIELD = TIM_MEM_DEBUG0 ena 47 47, count 45 24, interval 21 0

REG = TIM_MEM_DEBUG1 0x8001180058001108 MEMORY 8
REG_FIELD = TIM_MEM_DEBUG1 bucket 63 51, base 50 20, bsize 19 0

REG = TIM_MEM_DEBUG2 0x8001180058001110 MEMORY 8
REG_FIELD = TIM_MEM_DEBUG2 cpool 23 21, csize 20 8, bucket 6 0

REG = TIM_MEM_RING0 0x8001180058001000 MEMORY 8
REG_FIELD = TIM_MEM_RING0 first_bucket 54 24, num_buckets 23 4, ring 3 0

REG = TIM_MEM_RING1 0x8001180058001008 MEMORY 8
REG_FIELD = TIM_MEM_RING1 enable 42 42, pool 41 39, words_per_chunk 38 26, interval 25 4, ring 3 0

REG = TIM_REG_BIST_RESULT 0x8001180058000080 MEMORY 8
REG_FIELD = TIM_REG_BIST_RESULT sta 3 2, ncb 1 1, ctl 0 0

REG = TIM_REG_ERROR 0x8001180058000088 MEMORY 8
REG_FIELD = TIM_REG_ERROR mask 15 0

REG = TIM_REG_FLAGS 0x8001180058000000 MEMORY 8
REG_FIELD = TIM_REG_FLAGS reset 2 2, enable_dwb 1 1, enable_timers 0 0

REG = TIM_REG_INT_MASK 0x8001180058000090 MEMORY 8
REG_FIELD = TIM_REG_INT_MASK mask 15 0

REG = TIM_REG_READ_IDX 0x8001180058000008 MEMORY 8
REG_FIELD = TIM_REG_READ_IDX inc 15 8, index 7 0

REG = USBDRD0_UAHC_CAPLENGTH 0x8001680000000000 MEMORY 4
REG_FIELD = USBDRD0_UAHC_CAPLENGTH hciversion 31 16, caplength 7 0
REG = USBDRD1_UAHC_CAPLENGTH 0x8001690000000000 MEMORY 4
REG_FIELD = USBDRD1_UAHC_CAPLENGTH hciversion 31 16, caplength 7 0

REG = USBDRD0_UAHC_CONFIG 0x8001680000000058 MEMORY 4
REG_FIELD = USBDRD0_UAHC_CONFIG maxslotsen 7 0
REG = USBDRD1_UAHC_CONFIG 0x8001690000000058 MEMORY 4
REG_FIELD = USBDRD1_UAHC_CONFIG maxslotsen 7 0

REG = USBDRD0_UAHC_CRCR 0x8001680000000038 MEMORY 8
REG_FIELD = USBDRD0_UAHC_CRCR cmd_ring_ptr 63 6, crr 3 3, ca 2 2, cs 1 1, rcs 0 0
REG = USBDRD1_UAHC_CRCR 0x8001690000000038 MEMORY 8
REG_FIELD = USBDRD1_UAHC_CRCR cmd_ring_ptr 63 6, crr 3 3, ca 2 2, cs 1 1, rcs 0 0

REG = USBDRD0_UAHC_DALEPENA 0x800168000000C720 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DALEPENA usbactep 31 0
REG = USBDRD1_UAHC_DALEPENA 0x800169000000C720 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DALEPENA usbactep 31 0

REG = USBDRD0_UAHC_DB000 0x8001680000000480 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB000 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB001 0x8001680000000484 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB001 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB002 0x8001680000000488 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB002 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB003 0x800168000000048C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB003 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB004 0x8001680000000490 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB004 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB005 0x8001680000000494 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB005 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB006 0x8001680000000498 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB006 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB007 0x800168000000049C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB007 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB008 0x80016800000004A0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB008 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB009 0x80016800000004A4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB009 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB010 0x80016800000004A8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB010 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB011 0x80016800000004AC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB011 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB012 0x80016800000004B0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB012 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB013 0x80016800000004B4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB013 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB014 0x80016800000004B8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB014 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB015 0x80016800000004BC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB015 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB016 0x80016800000004C0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB016 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB017 0x80016800000004C4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB017 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB018 0x80016800000004C8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB018 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB019 0x80016800000004CC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB019 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB020 0x80016800000004D0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB020 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB021 0x80016800000004D4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB021 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB022 0x80016800000004D8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB022 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB023 0x80016800000004DC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB023 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB024 0x80016800000004E0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB024 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB025 0x80016800000004E4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB025 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB026 0x80016800000004E8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB026 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB027 0x80016800000004EC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB027 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB028 0x80016800000004F0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB028 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB029 0x80016800000004F4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB029 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB030 0x80016800000004F8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB030 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB031 0x80016800000004FC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB031 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB032 0x8001680000000500 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB032 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB033 0x8001680000000504 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB033 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB034 0x8001680000000508 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB034 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB035 0x800168000000050C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB035 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB036 0x8001680000000510 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB036 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB037 0x8001680000000514 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB037 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB038 0x8001680000000518 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB038 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB039 0x800168000000051C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB039 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB040 0x8001680000000520 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB040 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB041 0x8001680000000524 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB041 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB042 0x8001680000000528 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB042 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB043 0x800168000000052C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB043 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB044 0x8001680000000530 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB044 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB045 0x8001680000000534 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB045 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB046 0x8001680000000538 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB046 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB047 0x800168000000053C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB047 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB048 0x8001680000000540 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB048 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB049 0x8001680000000544 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB049 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB050 0x8001680000000548 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB050 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB051 0x800168000000054C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB051 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB052 0x8001680000000550 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB052 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB053 0x8001680000000554 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB053 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB054 0x8001680000000558 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB054 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB055 0x800168000000055C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB055 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB056 0x8001680000000560 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB056 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB057 0x8001680000000564 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB057 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB058 0x8001680000000568 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB058 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB059 0x800168000000056C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB059 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB060 0x8001680000000570 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB060 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB061 0x8001680000000574 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB061 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB062 0x8001680000000578 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB062 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB063 0x800168000000057C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB063 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD0_UAHC_DB064 0x8001680000000580 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DB064 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB000 0x8001690000000480 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB000 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB001 0x8001690000000484 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB001 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB002 0x8001690000000488 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB002 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB003 0x800169000000048C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB003 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB004 0x8001690000000490 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB004 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB005 0x8001690000000494 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB005 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB006 0x8001690000000498 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB006 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB007 0x800169000000049C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB007 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB008 0x80016900000004A0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB008 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB009 0x80016900000004A4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB009 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB010 0x80016900000004A8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB010 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB011 0x80016900000004AC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB011 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB012 0x80016900000004B0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB012 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB013 0x80016900000004B4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB013 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB014 0x80016900000004B8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB014 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB015 0x80016900000004BC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB015 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB016 0x80016900000004C0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB016 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB017 0x80016900000004C4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB017 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB018 0x80016900000004C8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB018 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB019 0x80016900000004CC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB019 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB020 0x80016900000004D0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB020 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB021 0x80016900000004D4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB021 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB022 0x80016900000004D8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB022 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB023 0x80016900000004DC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB023 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB024 0x80016900000004E0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB024 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB025 0x80016900000004E4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB025 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB026 0x80016900000004E8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB026 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB027 0x80016900000004EC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB027 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB028 0x80016900000004F0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB028 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB029 0x80016900000004F4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB029 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB030 0x80016900000004F8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB030 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB031 0x80016900000004FC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB031 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB032 0x8001690000000500 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB032 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB033 0x8001690000000504 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB033 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB034 0x8001690000000508 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB034 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB035 0x800169000000050C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB035 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB036 0x8001690000000510 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB036 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB037 0x8001690000000514 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB037 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB038 0x8001690000000518 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB038 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB039 0x800169000000051C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB039 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB040 0x8001690000000520 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB040 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB041 0x8001690000000524 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB041 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB042 0x8001690000000528 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB042 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB043 0x800169000000052C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB043 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB044 0x8001690000000530 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB044 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB045 0x8001690000000534 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB045 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB046 0x8001690000000538 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB046 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB047 0x800169000000053C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB047 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB048 0x8001690000000540 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB048 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB049 0x8001690000000544 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB049 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB050 0x8001690000000548 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB050 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB051 0x800169000000054C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB051 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB052 0x8001690000000550 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB052 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB053 0x8001690000000554 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB053 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB054 0x8001690000000558 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB054 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB055 0x800169000000055C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB055 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB056 0x8001690000000560 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB056 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB057 0x8001690000000564 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB057 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB058 0x8001690000000568 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB058 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB059 0x800169000000056C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB059 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB060 0x8001690000000570 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB060 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB061 0x8001690000000574 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB061 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB062 0x8001690000000578 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB062 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB063 0x800169000000057C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB063 dbstreamid 31 16, dbtarget 7 0
REG = USBDRD1_UAHC_DB064 0x8001690000000580 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DB064 dbstreamid 31 16, dbtarget 7 0

REG = USBDRD0_UAHC_DBOFF 0x8001680000000014 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DBOFF dboff 31 2
REG = USBDRD1_UAHC_DBOFF 0x8001690000000014 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DBOFF dboff 31 2

REG = USBDRD0_UAHC_DCBAAP 0x8001680000000050 MEMORY 8
REG_FIELD = USBDRD0_UAHC_DCBAAP dcbaap 63 6
REG = USBDRD1_UAHC_DCBAAP 0x8001690000000050 MEMORY 8
REG_FIELD = USBDRD1_UAHC_DCBAAP dcbaap 63 6

REG = USBDRD0_UAHC_DCFG 0x800168000000C700 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DCFG ignorestreampp 23 23, lpmcap 22 22, nump 21 17, intrnum 16 12, devaddr 9 3, devspd 2 0
REG = USBDRD1_UAHC_DCFG 0x800169000000C700 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DCFG ignorestreampp 23 23, lpmcap 22 22, nump 21 17, intrnum 16 12, devaddr 9 3, devspd 2 0

REG = USBDRD0_UAHC_DCTL 0x800168000000C704 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DCTL rs 31 31, csftrst 30 30, hird_thres 28 24, appl1res 23 23, keepconnect 19 19, l1hibernationen 18 18, crs 17 17, css 16 16, initu2ena 12 12, acceptu2ena 11 11, initu1ena 10 10, acceptu1ena 9 9, ulstchngreq 8 5, tstctl 4 1
REG = USBDRD1_UAHC_DCTL 0x800169000000C704 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DCTL rs 31 31, csftrst 30 30, hird_thres 28 24, appl1res 23 23, keepconnect 19 19, l1hibernationen 18 18, crs 17 17, css 16 16, initu2ena 12 12, acceptu2ena 11 11, initu1ena 10 10, acceptu1ena 9 9, ulstchngreq 8 5, tstctl 4 1

REG = USBDRD0_UAHC_DEPCMD000 0x800168000000C80C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD000 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD001 0x800168000000C81C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD001 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD002 0x800168000000C82C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD002 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD003 0x800168000000C83C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD003 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD004 0x800168000000C84C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD004 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD005 0x800168000000C85C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD005 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD006 0x800168000000C86C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD006 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD007 0x800168000000C87C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD007 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD008 0x800168000000C88C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD008 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD009 0x800168000000C89C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD009 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD010 0x800168000000C8AC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD010 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD011 0x800168000000C8BC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD011 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD012 0x800168000000C8CC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD012 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD013 0x800168000000C8DC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD013 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD014 0x800168000000C8EC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD014 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD0_UAHC_DEPCMD015 0x800168000000C8FC MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMD015 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD000 0x800169000000C80C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD000 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD001 0x800169000000C81C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD001 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD002 0x800169000000C82C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD002 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD003 0x800169000000C83C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD003 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD004 0x800169000000C84C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD004 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD005 0x800169000000C85C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD005 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD006 0x800169000000C86C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD006 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD007 0x800169000000C87C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD007 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD008 0x800169000000C88C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD008 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD009 0x800169000000C89C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD009 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD010 0x800169000000C8AC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD010 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD011 0x800169000000C8BC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD011 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD012 0x800169000000C8CC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD012 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD013 0x800169000000C8DC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD013 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD014 0x800169000000C8EC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD014 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0
REG = USBDRD1_UAHC_DEPCMD015 0x800169000000C8FC MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMD015 commandparam 31 16, cmdstatus 15 12, hipri_forcerm 11 11, cmdact 10 10, cmdioc 8 8, cmdtyp 3 0

REG = USBDRD0_UAHC_DEPCMDPAR0_000 0x800168000000C808 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_000 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_001 0x800168000000C818 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_001 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_002 0x800168000000C828 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_002 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_003 0x800168000000C838 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_003 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_004 0x800168000000C848 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_004 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_005 0x800168000000C858 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_005 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_006 0x800168000000C868 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_006 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_007 0x800168000000C878 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_007 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_008 0x800168000000C888 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_008 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_009 0x800168000000C898 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_009 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_010 0x800168000000C8A8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_010 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_011 0x800168000000C8B8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_011 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_012 0x800168000000C8C8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_012 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_013 0x800168000000C8D8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_013 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_014 0x800168000000C8E8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_014 param0 31 0
REG = USBDRD0_UAHC_DEPCMDPAR0_015 0x800168000000C8F8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR0_015 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_000 0x800169000000C808 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_000 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_001 0x800169000000C818 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_001 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_002 0x800169000000C828 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_002 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_003 0x800169000000C838 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_003 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_004 0x800169000000C848 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_004 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_005 0x800169000000C858 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_005 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_006 0x800169000000C868 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_006 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_007 0x800169000000C878 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_007 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_008 0x800169000000C888 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_008 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_009 0x800169000000C898 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_009 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_010 0x800169000000C8A8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_010 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_011 0x800169000000C8B8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_011 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_012 0x800169000000C8C8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_012 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_013 0x800169000000C8D8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_013 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_014 0x800169000000C8E8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_014 param0 31 0
REG = USBDRD1_UAHC_DEPCMDPAR0_015 0x800169000000C8F8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR0_015 param0 31 0

REG = USBDRD0_UAHC_DEPCMDPAR1_000 0x800168000000C804 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_000 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_001 0x800168000000C814 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_001 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_002 0x800168000000C824 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_002 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_003 0x800168000000C834 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_003 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_004 0x800168000000C844 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_004 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_005 0x800168000000C854 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_005 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_006 0x800168000000C864 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_006 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_007 0x800168000000C874 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_007 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_008 0x800168000000C884 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_008 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_009 0x800168000000C894 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_009 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_010 0x800168000000C8A4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_010 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_011 0x800168000000C8B4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_011 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_012 0x800168000000C8C4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_012 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_013 0x800168000000C8D4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_013 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_014 0x800168000000C8E4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_014 param1 31 0
REG = USBDRD0_UAHC_DEPCMDPAR1_015 0x800168000000C8F4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR1_015 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_000 0x800169000000C804 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_000 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_001 0x800169000000C814 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_001 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_002 0x800169000000C824 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_002 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_003 0x800169000000C834 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_003 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_004 0x800169000000C844 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_004 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_005 0x800169000000C854 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_005 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_006 0x800169000000C864 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_006 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_007 0x800169000000C874 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_007 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_008 0x800169000000C884 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_008 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_009 0x800169000000C894 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_009 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_010 0x800169000000C8A4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_010 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_011 0x800169000000C8B4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_011 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_012 0x800169000000C8C4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_012 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_013 0x800169000000C8D4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_013 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_014 0x800169000000C8E4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_014 param1 31 0
REG = USBDRD1_UAHC_DEPCMDPAR1_015 0x800169000000C8F4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR1_015 param1 31 0

REG = USBDRD0_UAHC_DEPCMDPAR2_000 0x800168000000C800 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_000 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_001 0x800168000000C810 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_001 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_002 0x800168000000C820 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_002 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_003 0x800168000000C830 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_003 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_004 0x800168000000C840 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_004 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_005 0x800168000000C850 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_005 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_006 0x800168000000C860 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_006 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_007 0x800168000000C870 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_007 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_008 0x800168000000C880 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_008 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_009 0x800168000000C890 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_009 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_010 0x800168000000C8A0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_010 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_011 0x800168000000C8B0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_011 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_012 0x800168000000C8C0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_012 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_013 0x800168000000C8D0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_013 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_014 0x800168000000C8E0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_014 param2 31 0
REG = USBDRD0_UAHC_DEPCMDPAR2_015 0x800168000000C8F0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEPCMDPAR2_015 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_000 0x800169000000C800 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_000 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_001 0x800169000000C810 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_001 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_002 0x800169000000C820 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_002 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_003 0x800169000000C830 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_003 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_004 0x800169000000C840 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_004 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_005 0x800169000000C850 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_005 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_006 0x800169000000C860 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_006 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_007 0x800169000000C870 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_007 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_008 0x800169000000C880 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_008 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_009 0x800169000000C890 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_009 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_010 0x800169000000C8A0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_010 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_011 0x800169000000C8B0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_011 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_012 0x800169000000C8C0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_012 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_013 0x800169000000C8D0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_013 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_014 0x800169000000C8E0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_014 param2 31 0
REG = USBDRD1_UAHC_DEPCMDPAR2_015 0x800169000000C8F0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEPCMDPAR2_015 param2 31 0

REG = USBDRD0_UAHC_DEVTEN 0x800168000000C708 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DEVTEN vndrdevtstrcveden 12 12, errticerren 9 9, sofen 7 7, u3l2l1suspen 6 6, hibernationreqevten 5 5, wkupevten 4 4, ulstcngen 3 3, connectdoneen 2 2, usbrsten 1 1, disconnevten 0 0
REG = USBDRD1_UAHC_DEVTEN 0x800169000000C708 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DEVTEN vndrdevtstrcveden 12 12, errticerren 9 9, sofen 7 7, u3l2l1suspen 6 6, hibernationreqevten 5 5, wkupevten 4 4, ulstcngen 3 3, connectdoneen 2 2, usbrsten 1 1, disconnevten 0 0

REG = USBDRD0_UAHC_DGCMD 0x800168000000C714 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DGCMD cmdstatus 15 15, cmdact 10 10, cmdioc 8 8, cmdtyp 7 0
REG = USBDRD1_UAHC_DGCMD 0x800169000000C714 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DGCMD cmdstatus 15 15, cmdact 10 10, cmdioc 8 8, cmdtyp 7 0

REG = USBDRD0_UAHC_DGCMDPAR 0x800168000000C710 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DGCMDPAR param 31 0
REG = USBDRD1_UAHC_DGCMDPAR 0x800169000000C710 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DGCMDPAR param 31 0

REG = USBDRD0_UAHC_DNCTRL 0x8001680000000034 MEMORY 4
REG_FIELD = USBDRD0_UAHC_DNCTRL n 15 0
REG = USBDRD1_UAHC_DNCTRL 0x8001690000000034 MEMORY 4
REG_FIELD = USBDRD1_UAHC_DNCTRL n 15 0

REG = USBDRD0_UAHC_DSTS 0x800168000000C70C MEMORY 4
REG_FIELD = USBDRD0_UAHC_DSTS dcnrd 29 29, sre 28 28, rss 25 25, sss 24 24, coreidle 23 23, devctrlhlt 22 22, usblnkst 21 18, rxfifoempty 17 17, soffn 16 3, connectspd 2 0
REG = USBDRD1_UAHC_DSTS 0x800169000000C70C MEMORY 4
REG_FIELD = USBDRD1_UAHC_DSTS dcnrd 29 29, sre 28 28, rss 25 25, sss 24 24, coreidle 23 23, devctrlhlt 22 22, usblnkst 21 18, rxfifoempty 17 17, soffn 16 3, connectspd 2 0

REG = USBDRD0_UAHC_ERDP000 0x8001680000000478 MEMORY 8
REG_FIELD = USBDRD0_UAHC_ERDP000 erdp 63 4, ehb 3 3, desi 2 0
REG = USBDRD1_UAHC_ERDP000 0x8001690000000478 MEMORY 8
REG_FIELD = USBDRD1_UAHC_ERDP000 erdp 63 4, ehb 3 3, desi 2 0

REG = USBDRD0_UAHC_ERSTBA000 0x8001680000000470 MEMORY 8
REG_FIELD = USBDRD0_UAHC_ERSTBA000 erstba 63 6
REG = USBDRD1_UAHC_ERSTBA000 0x8001690000000470 MEMORY 8
REG_FIELD = USBDRD1_UAHC_ERSTBA000 erstba 63 6

REG = USBDRD0_UAHC_ERSTSZ000 0x8001680000000468 MEMORY 4
REG_FIELD = USBDRD0_UAHC_ERSTSZ000 erstsz 15 0
REG = USBDRD1_UAHC_ERSTSZ000 0x8001690000000468 MEMORY 4
REG_FIELD = USBDRD1_UAHC_ERSTSZ000 erstsz 15 0

REG = USBDRD0_UAHC_GBUSERRADDR 0x800168000000C130 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GBUSERRADDR busaddr 63 0
REG = USBDRD1_UAHC_GBUSERRADDR 0x800169000000C130 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GBUSERRADDR busaddr 63 0

REG = USBDRD0_UAHC_GCTL 0x800168000000C110 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GCTL pwrdnscale 31 19, masterfiltbypass 18 18, bypssetaddr 17 17, u2rstecn 16 16, frmscldwn 15 14, prtcapdir 13 12, coresoftreset 11 11, sofitpsync 10 10, u1u2timerscale 9 9, debugattach 8 8, ramclksel 7 6, scaledown 5 4, disscramble 3 3, u2exit_lfps 2 2, dsblclkgtng 0 0
REG = USBDRD1_UAHC_GCTL 0x800169000000C110 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GCTL pwrdnscale 31 19, masterfiltbypass 18 18, bypssetaddr 17 17, u2rstecn 16 16, frmscldwn 15 14, prtcapdir 13 12, coresoftreset 11 11, sofitpsync 10 10, u1u2timerscale 9 9, debugattach 8 8, ramclksel 7 6, scaledown 5 4, disscramble 3 3, u2exit_lfps 2 2, dsblclkgtng 0 0

REG = USBDRD0_UAHC_GDBGBMU 0x800168000000C16C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGBMU bmu_bcu_dbg 31 8, bmu_dcu_dbg 7 4, bmu_ccu_dbg 3 0
REG = USBDRD1_UAHC_GDBGBMU 0x800169000000C16C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGBMU bmu_bcu_dbg 31 8, bmu_dcu_dbg 7 4, bmu_ccu_dbg 3 0

REG = USBDRD0_UAHC_GDBGEPINFO 0x800168000000C178 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GDBGEPINFO endpt_dbg 63 0
REG = USBDRD1_UAHC_GDBGEPINFO 0x800169000000C178 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GDBGEPINFO endpt_dbg 63 0

REG = USBDRD0_UAHC_GDBGFIFOSPACE 0x800168000000C160 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGFIFOSPACE spaceavailable 31 16, select 7 0
REG = USBDRD1_UAHC_GDBGFIFOSPACE 0x800169000000C160 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGFIFOSPACE spaceavailable 31 16, select 7 0

REG = USBDRD0_UAHC_GDBGLNMCC 0x800168000000C168 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGLNMCC lnmcc_berc 8 0
REG = USBDRD1_UAHC_GDBGLNMCC 0x800169000000C168 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGLNMCC lnmcc_berc 8 0

REG = USBDRD0_UAHC_GDBGLSP 0x800168000000C174 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGLSP lsp_dbg 31 0
REG = USBDRD1_UAHC_GDBGLSP 0x800169000000C174 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGLSP lsp_dbg 31 0

REG = USBDRD0_UAHC_GDBGLSPMUX 0x800168000000C170 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGLSPMUX latraceportmuxselect 23 16, endbc 15 15, hostselect 13 0
REG = USBDRD1_UAHC_GDBGLSPMUX 0x800169000000C170 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGLSPMUX latraceportmuxselect 23 16, endbc 15 15, hostselect 13 0

REG = USBDRD0_UAHC_GDBGLTSSM 0x800168000000C164 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDBGLTSSM ltdbtimeout 26 26, ltdblinkstate 25 22, ltdbsubstate 21 18, debugpipestatus 17 0
REG = USBDRD1_UAHC_GDBGLTSSM 0x800169000000C164 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDBGLTSSM ltdbtimeout 26 26, ltdblinkstate 25 22, ltdbsubstate 21 18, debugpipestatus 17 0

REG = USBDRD0_UAHC_GDMAHLRATIO 0x800168000000C624 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GDMAHLRATIO rx_ratio 12 8, tx_ratio 4 0
REG = USBDRD1_UAHC_GDMAHLRATIO 0x800169000000C624 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GDMAHLRATIO rx_ratio 12 8, tx_ratio 4 0

REG = USBDRD0_UAHC_GEVNTADR000 0x800168000000C400 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GEVNTADR000 evntadr 63 0
REG = USBDRD1_UAHC_GEVNTADR000 0x800169000000C400 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GEVNTADR000 evntadr 63 0

REG = USBDRD0_UAHC_GEVNTCOUNT000 0x800168000000C40C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GEVNTCOUNT000 evntcount 15 0
REG = USBDRD1_UAHC_GEVNTCOUNT000 0x800169000000C40C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GEVNTCOUNT000 evntcount 15 0

REG = USBDRD0_UAHC_GEVNTSIZ000 0x800168000000C408 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GEVNTSIZ000 evntintmask 31 31, evntsiz 15 0
REG = USBDRD1_UAHC_GEVNTSIZ000 0x800169000000C408 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GEVNTSIZ000 evntintmask 31 31, evntsiz 15 0

REG = USBDRD0_UAHC_GFLADJ 0x800168000000C630 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GFLADJ gfladj_refclk_240mhz 31 31, gfladj_refclk_240mhz 30 24, gfladj_refclk_lpm_se 23 23, gfladj_refclk_fladj 21 8, gfladj_30mhz_reg_sel 7 7, gfladj_30mhz 5 0
REG = USBDRD1_UAHC_GFLADJ 0x800169000000C630 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GFLADJ gfladj_refclk_240mhz 31 31, gfladj_refclk_240mhz 30 24, gfladj_refclk_lpm_se 23 23, gfladj_refclk_fladj 21 8, gfladj_30mhz_reg_sel 7 7, gfladj_30mhz 5 0

REG = USBDRD0_UAHC_GGPIO 0x800168000000C124 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GGPIO gpo 31 16, gpi 15 0
REG = USBDRD1_UAHC_GGPIO 0x800169000000C124 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GGPIO gpo 31 16, gpi 15 0

REG = USBDRD0_UAHC_GHWPARAMS0 0x800168000000C140 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS0 awidth 31 24, sdwidth 23 16, mdwidth 15 8, sbus_type 7 6, mbus_type 5 3, mode 2 0
REG = USBDRD1_UAHC_GHWPARAMS0 0x800169000000C140 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS0 awidth 31 24, sdwidth 23 16, mdwidth 15 8, sbus_type 7 6, mbus_type 5 3, mode 2 0

REG = USBDRD0_UAHC_GHWPARAMS1 0x800168000000C144 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS1 en_dbc 31 31, rm_opt_features 30 30, sync_rst 29 29, ram_bus_clks_sync 28 28, mac_ram_clks_sync 27 27, mac_phy_clks_sync 26 26, en_pwropt 25 24, spram_typ 23 23, num_rams 22 21, device_num_int 20 15, aspacewidth 14 12, reqinfowidth 11 9, datainfowidth 8 6, burstwidth_m1 5 3, idwidth_m1 2 0
REG = USBDRD1_UAHC_GHWPARAMS1 0x800169000000C144 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS1 en_dbc 31 31, rm_opt_features 30 30, sync_rst 29 29, ram_bus_clks_sync 28 28, mac_ram_clks_sync 27 27, mac_phy_clks_sync 26 26, en_pwropt 25 24, spram_typ 23 23, num_rams 22 21, device_num_int 20 15, aspacewidth 14 12, reqinfowidth 11 9, datainfowidth 8 6, burstwidth_m1 5 3, idwidth_m1 2 0

REG = USBDRD0_UAHC_GHWPARAMS2 0x800168000000C148 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS2 userid 31 0
REG = USBDRD1_UAHC_GHWPARAMS2 0x800169000000C148 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS2 userid 31 0

REG = USBDRD0_UAHC_GHWPARAMS3 0x800168000000C14C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS3 cache_total_xfer_res 30 23, num_in_eps 22 18, num_eps 17 12, ulpi_carkit 11 11, vendor_ctl_interface 10 10, hsphy_dwidth 7 6, fsphy_interface 5 4, hsphy_interface 3 2, ssphy_interface 1 0
REG = USBDRD1_UAHC_GHWPARAMS3 0x800169000000C14C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS3 cache_total_xfer_res 30 23, num_in_eps 22 18, num_eps 17 12, ulpi_carkit 11 11, vendor_ctl_interface 10 10, hsphy_dwidth 7 6, fsphy_interface 5 4, hsphy_interface 3 2, ssphy_interface 1 0

REG = USBDRD0_UAHC_GHWPARAMS4 0x800168000000C150 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS4 bmu_lsp_depth 31 28, bmu_ptl_depth_m1 27 24, en_isoc_supt 23 23, ext_buff_control 21 21, num_ss_usb_instances 20 17, hiber_scratchbufs 16 13, cache_trbs_per_trans 5 0
REG = USBDRD1_UAHC_GHWPARAMS4 0x800169000000C150 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS4 bmu_lsp_depth 31 28, bmu_ptl_depth_m1 27 24, en_isoc_supt 23 23, ext_buff_control 21 21, num_ss_usb_instances 20 17, hiber_scratchbufs 16 13, cache_trbs_per_trans 5 0

REG = USBDRD0_UAHC_GHWPARAMS5 0x800168000000C154 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS5 dfq_fifo_depth 27 22, dwq_fifo_depth 21 16, txq_fifo_depth 15 10, rxq_fifo_depth 9 4, bmu_busgm_depth 3 0
REG = USBDRD1_UAHC_GHWPARAMS5 0x800169000000C154 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS5 dfq_fifo_depth 27 22, dwq_fifo_depth 21 16, txq_fifo_depth 15 10, rxq_fifo_depth 9 4, bmu_busgm_depth 3 0

REG = USBDRD0_UAHC_GHWPARAMS6 0x800168000000C158 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS6 ram0_depth 31 16, en_bus_filters 15 15, en_bc 14 14, en_otg_ss 13 13, en_adp 12 12, hnp_support 11 11, srp_support 10 10, en_fpga 7 7, en_dbg_ports 6 6, psq_fifo_depth 5 0
REG = USBDRD1_UAHC_GHWPARAMS6 0x800169000000C158 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS6 ram0_depth 31 16, en_bus_filters 15 15, en_bc 14 14, en_otg_ss 13 13, en_adp 12 12, hnp_support 11 11, srp_support 10 10, en_fpga 7 7, en_dbg_ports 6 6, psq_fifo_depth 5 0

REG = USBDRD0_UAHC_GHWPARAMS7 0x800168000000C15C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS7 ram2_depth 31 16, ram1_depth 15 0
REG = USBDRD1_UAHC_GHWPARAMS7 0x800169000000C15C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS7 ram2_depth 31 16, ram1_depth 15 0

REG = USBDRD0_UAHC_GHWPARAMS8 0x800168000000C600 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GHWPARAMS8 dcache_depth_info 31 0
REG = USBDRD1_UAHC_GHWPARAMS8 0x800169000000C600 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GHWPARAMS8 dcache_depth_info 31 0

REG = USBDRD0_UAHC_GPMSTS 0x800168000000C114 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GPMSTS portsel 31 28, u3wakeup 16 12, u2wakeup 9 0
REG = USBDRD1_UAHC_GPMSTS 0x800169000000C114 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GPMSTS portsel 31 28, u3wakeup 16 12, u2wakeup 9 0

REG = USBDRD0_UAHC_GPRTBIMAP 0x800168000000C138 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GPRTBIMAP binum1 3 0
REG = USBDRD1_UAHC_GPRTBIMAP 0x800169000000C138 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GPRTBIMAP binum1 3 0

REG = USBDRD0_UAHC_GPRTBIMAP_FS 0x800168000000C188 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GPRTBIMAP_FS binum1 3 0
REG = USBDRD1_UAHC_GPRTBIMAP_FS 0x800169000000C188 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GPRTBIMAP_FS binum1 3 0

REG = USBDRD0_UAHC_GPRTBIMAP_HS 0x800168000000C180 MEMORY 8
REG_FIELD = USBDRD0_UAHC_GPRTBIMAP_HS binum1 3 0
REG = USBDRD1_UAHC_GPRTBIMAP_HS 0x800169000000C180 MEMORY 8
REG_FIELD = USBDRD1_UAHC_GPRTBIMAP_HS binum1 3 0

REG = USBDRD0_UAHC_GRLSID 0x800168000000C120 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRLSID releaseid 31 0
REG = USBDRD1_UAHC_GRLSID 0x800169000000C120 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRLSID releaseid 31 0

REG = USBDRD0_UAHC_GRXFIFOPRIHST 0x800168000000C61C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRXFIFOPRIHST rx_priority 2 0
REG = USBDRD1_UAHC_GRXFIFOPRIHST 0x800169000000C61C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRXFIFOPRIHST rx_priority 2 0

REG = USBDRD0_UAHC_GRXFIFOSIZ000 0x800168000000C380 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRXFIFOSIZ000 rxfstaddr 31 16, rxfdep 15 0
REG = USBDRD0_UAHC_GRXFIFOSIZ001 0x800168000000C384 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRXFIFOSIZ001 rxfstaddr 31 16, rxfdep 15 0
REG = USBDRD0_UAHC_GRXFIFOSIZ002 0x800168000000C388 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRXFIFOSIZ002 rxfstaddr 31 16, rxfdep 15 0
REG = USBDRD1_UAHC_GRXFIFOSIZ000 0x800169000000C380 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRXFIFOSIZ000 rxfstaddr 31 16, rxfdep 15 0
REG = USBDRD1_UAHC_GRXFIFOSIZ001 0x800169000000C384 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRXFIFOSIZ001 rxfstaddr 31 16, rxfdep 15 0
REG = USBDRD1_UAHC_GRXFIFOSIZ002 0x800169000000C388 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRXFIFOSIZ002 rxfstaddr 31 16, rxfdep 15 0

REG = USBDRD0_UAHC_GRXTHRCFG 0x800168000000C10C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GRXTHRCFG usbrxpktcntsel 29 29, usbrxpktcnt 27 24, usbmaxrxburstsize 23 19
REG = USBDRD1_UAHC_GRXTHRCFG 0x800169000000C10C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GRXTHRCFG usbrxpktcntsel 29 29, usbrxpktcnt 27 24, usbmaxrxburstsize 23 19

REG = USBDRD0_UAHC_GSBUSCFG0 0x800168000000C100 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GSBUSCFG0 datrdreqinfo 31 28, desrdreqinfo 27 24, datwrreqinfo 23 20, deswrreqinfo 19 16, datbigend 11 11, descbigend 10 10, incr256brstena 7 7, incr128brstena 6 6, incr64brstena 5 5, incr32brstena 4 4, incr16brstena 3 3, incr8brstena 2 2, incr4brstena 1 1, incrbrstena 0 0
REG = USBDRD1_UAHC_GSBUSCFG0 0x800169000000C100 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GSBUSCFG0 datrdreqinfo 31 28, desrdreqinfo 27 24, datwrreqinfo 23 20, deswrreqinfo 19 16, datbigend 11 11, descbigend 10 10, incr256brstena 7 7, incr128brstena 6 6, incr64brstena 5 5, incr32brstena 4 4, incr16brstena 3 3, incr8brstena 2 2, incr4brstena 1 1, incrbrstena 0 0

REG = USBDRD0_UAHC_GSBUSCFG1 0x800168000000C104 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GSBUSCFG1 en1kpage 12 12, pipetranslimit 11 8
REG = USBDRD1_UAHC_GSBUSCFG1 0x800169000000C104 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GSBUSCFG1 en1kpage 12 12, pipetranslimit 11 8

REG = USBDRD0_UAHC_GSTS 0x800168000000C118 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GSTS cbelt 31 20, host_ip 7 7, device_ip 6 6, csrtimeout 5 5, buserraddrvld 4 4, curmod 1 0
REG = USBDRD1_UAHC_GSTS 0x800169000000C118 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GSTS cbelt 31 20, host_ip 7 7, device_ip 6 6, csrtimeout 5 5, buserraddrvld 4 4, curmod 1 0

REG = USBDRD0_UAHC_GTXFIFOPRIDEV 0x800168000000C610 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOPRIDEV tx_priority 3 0
REG = USBDRD1_UAHC_GTXFIFOPRIDEV 0x800169000000C610 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOPRIDEV tx_priority 3 0

REG = USBDRD0_UAHC_GTXFIFOPRIHST 0x800168000000C618 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOPRIHST tx_priority 2 0
REG = USBDRD1_UAHC_GTXFIFOPRIHST 0x800169000000C618 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOPRIHST tx_priority 2 0

REG = USBDRD0_UAHC_GTXFIFOSIZ000 0x800168000000C300 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOSIZ000 txfstaddr 31 16, txfdep 15 0
REG = USBDRD0_UAHC_GTXFIFOSIZ001 0x800168000000C304 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOSIZ001 txfstaddr 31 16, txfdep 15 0
REG = USBDRD0_UAHC_GTXFIFOSIZ002 0x800168000000C308 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOSIZ002 txfstaddr 31 16, txfdep 15 0
REG = USBDRD0_UAHC_GTXFIFOSIZ003 0x800168000000C30C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXFIFOSIZ003 txfstaddr 31 16, txfdep 15 0
REG = USBDRD1_UAHC_GTXFIFOSIZ000 0x800169000000C300 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOSIZ000 txfstaddr 31 16, txfdep 15 0
REG = USBDRD1_UAHC_GTXFIFOSIZ001 0x800169000000C304 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOSIZ001 txfstaddr 31 16, txfdep 15 0
REG = USBDRD1_UAHC_GTXFIFOSIZ002 0x800169000000C308 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOSIZ002 txfstaddr 31 16, txfdep 15 0
REG = USBDRD1_UAHC_GTXFIFOSIZ003 0x800169000000C30C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXFIFOSIZ003 txfstaddr 31 16, txfdep 15 0

REG = USBDRD0_UAHC_GTXTHRCFG 0x800168000000C108 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GTXTHRCFG usbtxpktcntsel 29 29, usbtxpktcnt 27 24, usbmaxtxburstsize 23 16
REG = USBDRD1_UAHC_GTXTHRCFG 0x800169000000C108 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GTXTHRCFG usbtxpktcntsel 29 29, usbtxpktcnt 27 24, usbmaxtxburstsize 23 16

REG = USBDRD0_UAHC_GUCTL 0x800168000000C12C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUCTL refclkper 31 22, noextrdl 21 21, psqextrressp 20 18, sprsctrltransen 17 17, resbwhseps 16 16, cmdevaddr 15 15, usbhstinautoretryen 14 14, enoverlapchk 13 13, extcapsupten 12 12, insrtextrfsbodi 11 11, dtct 10 9, dtft 8 0
REG = USBDRD1_UAHC_GUCTL 0x800169000000C12C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUCTL refclkper 31 22, noextrdl 21 21, psqextrressp 20 18, sprsctrltransen 17 17, resbwhseps 16 16, cmdevaddr 15 15, usbhstinautoretryen 14 14, enoverlapchk 13 13, extcapsupten 12 12, insrtextrfsbodi 11 11, dtct 10 9, dtft 8 0

REG = USBDRD0_UAHC_GUCTL1 0x800168000000C11C MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUCTL1 ovrld_l1_susp_com 1 1, loa_filter_en 0 0
REG = USBDRD1_UAHC_GUCTL1 0x800169000000C11C MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUCTL1 ovrld_l1_susp_com 1 1, loa_filter_en 0 0

REG = USBDRD0_UAHC_GUID 0x800168000000C128 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUID userid 31 0
REG = USBDRD1_UAHC_GUID 0x800169000000C128 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUID userid 31 0

REG = USBDRD0_UAHC_GUSB2I2CCTL000 0x800168000000C240 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUSB2I2CCTL000 reserved_0_31 0 31
REG = USBDRD1_UAHC_GUSB2I2CCTL000 0x800169000000C240 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUSB2I2CCTL000 reserved_0_31 0 31

REG = USBDRD0_UAHC_GUSB2PHYCFG000 0x800168000000C200 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUSB2PHYCFG000 physoftrst 31 31, u2_freeclk_exists 30 30, ulpi_lpm_with_opmode 29 29, ulpiextvbusindicator 18 18, ulpiextvbusdrv 17 17, ulpiclksusm 16 16, ulpiautores 15 15, usbtrdtim 13 10, xcvrdly 9 9, enblslpm 8 8, physel 7 7, susphy 6 6, fsintf 5 5, ulpi_utmi_sel 4 4, phyif 3 3, toutcal 2 0
REG = USBDRD1_UAHC_GUSB2PHYCFG000 0x800169000000C200 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUSB2PHYCFG000 physoftrst 31 31, u2_freeclk_exists 30 30, ulpi_lpm_with_opmode 29 29, ulpiextvbusindicator 18 18, ulpiextvbusdrv 17 17, ulpiclksusm 16 16, ulpiautores 15 15, usbtrdtim 13 10, xcvrdly 9 9, enblslpm 8 8, physel 7 7, susphy 6 6, fsintf 5 5, ulpi_utmi_sel 4 4, phyif 3 3, toutcal 2 0

REG = USBDRD0_UAHC_GUSB3PIPECTL000 0x800168000000C2C0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_GUSB3PIPECTL000 physoftrst 31 31, hstprtcmpl 30 30, u2ssinactp3ok 29 29, disrxdetp3 28 28, ux_exit_in_px 27 27, ping_enchance_en 26 26, u1u2exitfail_to_reco 25 25, request_p1p2p3 24 24, startrxdetu3rxdet 23 23, disrxdetu3rxdet 22 22, delaypx 21 19, delaypxtransenterux 18 18, suspend_en 17 17, datwidth 16 15, abortrxdetinu2 14 14, skiprxdet 13 13, lfpsp0algn 12 12, p3p2tranok 11 11, p3exsigp2 10 10, lfpsfilt 9 9, rxdet2polllfpsctrl 8 8, txswing 6 6, txmargin 5 3, txdeemphasis 2 1, elasticbuffermode 0 0
REG = USBDRD1_UAHC_GUSB3PIPECTL000 0x800169000000C2C0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_GUSB3PIPECTL000 physoftrst 31 31, hstprtcmpl 30 30, u2ssinactp3ok 29 29, disrxdetp3 28 28, ux_exit_in_px 27 27, ping_enchance_en 26 26, u1u2exitfail_to_reco 25 25, request_p1p2p3 24 24, startrxdetu3rxdet 23 23, disrxdetu3rxdet 22 22, delaypx 21 19, delaypxtransenterux 18 18, suspend_en 17 17, datwidth 16 15, abortrxdetinu2 14 14, skiprxdet 13 13, lfpsp0algn 12 12, p3p2tranok 11 11, p3exsigp2 10 10, lfpsfilt 9 9, rxdet2polllfpsctrl 8 8, txswing 6 6, txmargin 5 3, txdeemphasis 2 1, elasticbuffermode 0 0

REG = USBDRD0_UAHC_HCCPARAMS 0x8001680000000010 MEMORY 4
REG_FIELD = USBDRD0_UAHC_HCCPARAMS xecp 31 16, maxpsasize 15 12, pae 8 8, nss 7 7, ltc 6 6, lhrc 5 5, pind 4 4, ppc 3 3, csz 2 2, bnc 1 1, ac64 0 0
REG = USBDRD1_UAHC_HCCPARAMS 0x8001690000000010 MEMORY 4
REG_FIELD = USBDRD1_UAHC_HCCPARAMS xecp 31 16, maxpsasize 15 12, pae 8 8, nss 7 7, ltc 6 6, lhrc 5 5, pind 4 4, ppc 3 3, csz 2 2, bnc 1 1, ac64 0 0

REG = USBDRD0_UAHC_HCSPARAMS1 0x8001680000000004 MEMORY 4
REG_FIELD = USBDRD0_UAHC_HCSPARAMS1 maxports 31 24, maxintrs 18 8, maxslots 7 0
REG = USBDRD1_UAHC_HCSPARAMS1 0x8001690000000004 MEMORY 4
REG_FIELD = USBDRD1_UAHC_HCSPARAMS1 maxports 31 24, maxintrs 18 8, maxslots 7 0

REG = USBDRD0_UAHC_HCSPARAMS2 0x8001680000000008 MEMORY 4
REG_FIELD = USBDRD0_UAHC_HCSPARAMS2 maxscratchpadbufs_l 31 27, maxscratchpadbufs_h 25 21, erst_max 7 4, ist 3 0
REG = USBDRD1_UAHC_HCSPARAMS2 0x8001690000000008 MEMORY 4
REG_FIELD = USBDRD1_UAHC_HCSPARAMS2 maxscratchpadbufs_l 31 27, maxscratchpadbufs_h 25 21, erst_max 7 4, ist 3 0

REG = USBDRD0_UAHC_HCSPARAMS3 0x800168000000000C MEMORY 4
REG_FIELD = USBDRD0_UAHC_HCSPARAMS3 u2_device_exit_laten 31 16, u1_device_exit_laten 7 0
REG = USBDRD1_UAHC_HCSPARAMS3 0x800169000000000C MEMORY 4
REG_FIELD = USBDRD1_UAHC_HCSPARAMS3 u2_device_exit_laten 31 16, u1_device_exit_laten 7 0

REG = USBDRD0_UAHC_IMAN000 0x8001680000000460 MEMORY 4
REG_FIELD = USBDRD0_UAHC_IMAN000 ie 1 1, ip 0 0
REG = USBDRD1_UAHC_IMAN000 0x8001690000000460 MEMORY 4
REG_FIELD = USBDRD1_UAHC_IMAN000 ie 1 1, ip 0 0

REG = USBDRD0_UAHC_IMOD000 0x8001680000000464 MEMORY 4
REG_FIELD = USBDRD0_UAHC_IMOD000 imodc 31 16, imodi 15 0
REG = USBDRD1_UAHC_IMOD000 0x8001690000000464 MEMORY 4
REG_FIELD = USBDRD1_UAHC_IMOD000 imodc 31 16, imodi 15 0

REG = USBDRD0_UAHC_MFINDEX 0x8001680000000440 MEMORY 4
REG_FIELD = USBDRD0_UAHC_MFINDEX mfindex 13 0
REG = USBDRD1_UAHC_MFINDEX 0x8001690000000440 MEMORY 4
REG_FIELD = USBDRD1_UAHC_MFINDEX mfindex 13 0

REG = USBDRD0_UAHC_PAGESIZE 0x8001680000000028 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PAGESIZE pagesize 15 0
REG = USBDRD1_UAHC_PAGESIZE 0x8001690000000028 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PAGESIZE pagesize 15 0

REG = USBDRD0_UAHC_PORTHLPMC_20000 0x800168000000042C MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTHLPMC_20000 hirdd 13 10, l1_timeout 9 2, hirdm 1 0
REG = USBDRD1_UAHC_PORTHLPMC_20000 0x800169000000042C MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTHLPMC_20000 hirdd 13 10, l1_timeout 9 2, hirdm 1 0

REG = USBDRD0_UAHC_PORTHLPMC_SS001 0x800168000000043C MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTHLPMC_SS001 reserved_0_31 0 31
REG = USBDRD1_UAHC_PORTHLPMC_SS001 0x800169000000043C MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTHLPMC_SS001 reserved_0_31 0 31

REG = USBDRD0_UAHC_PORTLI_20000 0x8001680000000428 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTLI_20000 reserved_0_31 0 31
REG = USBDRD1_UAHC_PORTLI_20000 0x8001690000000428 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTLI_20000 reserved_0_31 0 31

REG = USBDRD0_UAHC_PORTLI_SS001 0x8001680000000438 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTLI_SS001 linkerrorcount 15 0
REG = USBDRD1_UAHC_PORTLI_SS001 0x8001690000000438 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTLI_SS001 linkerrorcount 15 0

REG = USBDRD0_UAHC_PORTPMSC_20000 0x8001680000000424 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTPMSC_20000 port_test_control 31 28, hle 16 16, l1_device_slot 15 8, hird 7 4, rwe 3 3, l1s 2 0
REG = USBDRD1_UAHC_PORTPMSC_20000 0x8001690000000424 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTPMSC_20000 port_test_control 31 28, hle 16 16, l1_device_slot 15 8, hird 7 4, rwe 3 3, l1s 2 0

REG = USBDRD0_UAHC_PORTPMSC_SS001 0x8001680000000434 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTPMSC_SS001 fla 16 16, u2_timeout 15 8, u1_timeout 7 0
REG = USBDRD1_UAHC_PORTPMSC_SS001 0x8001690000000434 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTPMSC_SS001 fla 16 16, u2_timeout 15 8, u1_timeout 7 0

REG = USBDRD0_UAHC_PORTSC000 0x8001680000000420 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTSC000 wpr 31 31, dr 30 30, woe 27 27, wde 26 26, wce 25 25, cas 24 24, cec 23 23, plc 22 22, prc 21 21, occ 20 20, wrc 19 19, pec 18 18, csc 17 17, lws 16 16, pic 15 14, portspeed 13 10, pp 9 9, pls 8 5, pr 4 4, oca 3 3, ped 1 1, ccs 0 0
REG = USBDRD0_UAHC_PORTSC001 0x8001680000000430 MEMORY 4
REG_FIELD = USBDRD0_UAHC_PORTSC001 wpr 31 31, dr 30 30, woe 27 27, wde 26 26, wce 25 25, cas 24 24, cec 23 23, plc 22 22, prc 21 21, occ 20 20, wrc 19 19, pec 18 18, csc 17 17, lws 16 16, pic 15 14, portspeed 13 10, pp 9 9, pls 8 5, pr 4 4, oca 3 3, ped 1 1, ccs 0 0
REG = USBDRD1_UAHC_PORTSC000 0x8001690000000420 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTSC000 wpr 31 31, dr 30 30, woe 27 27, wde 26 26, wce 25 25, cas 24 24, cec 23 23, plc 22 22, prc 21 21, occ 20 20, wrc 19 19, pec 18 18, csc 17 17, lws 16 16, pic 15 14, portspeed 13 10, pp 9 9, pls 8 5, pr 4 4, oca 3 3, ped 1 1, ccs 0 0
REG = USBDRD1_UAHC_PORTSC001 0x8001690000000430 MEMORY 4
REG_FIELD = USBDRD1_UAHC_PORTSC001 wpr 31 31, dr 30 30, woe 27 27, wde 26 26, wce 25 25, cas 24 24, cec 23 23, plc 22 22, prc 21 21, occ 20 20, wrc 19 19, pec 18 18, csc 17 17, lws 16 16, pic 15 14, portspeed 13 10, pp 9 9, pls 8 5, pr 4 4, oca 3 3, ped 1 1, ccs 0 0

REG = USBDRD0_UAHC_RTSOFF 0x8001680000000018 MEMORY 4
REG_FIELD = USBDRD0_UAHC_RTSOFF rtsoff 31 5
REG = USBDRD1_UAHC_RTSOFF 0x8001690000000018 MEMORY 4
REG_FIELD = USBDRD1_UAHC_RTSOFF rtsoff 31 5

REG = USBDRD0_UAHC_SUPTPRT2_DW0 0x8001680000000890 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT2_DW0 majorrev 31 24, minorrev 23 16, nextcapptr 15 8, capid 7 0
REG = USBDRD1_UAHC_SUPTPRT2_DW0 0x8001690000000890 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT2_DW0 majorrev 31 24, minorrev 23 16, nextcapptr 15 8, capid 7 0

REG = USBDRD0_UAHC_SUPTPRT2_DW1 0x8001680000000894 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT2_DW1 name 31 0
REG = USBDRD1_UAHC_SUPTPRT2_DW1 0x8001690000000894 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT2_DW1 name 31 0

REG = USBDRD0_UAHC_SUPTPRT2_DW2 0x8001680000000898 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT2_DW2 psic 31 28, blc 20 20, hlc 19 19, ihi 18 18, hso 17 17, compatprtcnt 15 8, compatprtoff 7 0
REG = USBDRD1_UAHC_SUPTPRT2_DW2 0x8001690000000898 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT2_DW2 psic 31 28, blc 20 20, hlc 19 19, ihi 18 18, hso 17 17, compatprtcnt 15 8, compatprtoff 7 0

REG = USBDRD0_UAHC_SUPTPRT2_DW3 0x800168000000089C MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT2_DW3 protslottype 4 0
REG = USBDRD1_UAHC_SUPTPRT2_DW3 0x800169000000089C MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT2_DW3 protslottype 4 0

REG = USBDRD0_UAHC_SUPTPRT3_DW0 0x80016800000008A0 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT3_DW0 majorrev 31 24, minorrev 23 16, nextcapptr 15 8, capid 7 0
REG = USBDRD1_UAHC_SUPTPRT3_DW0 0x80016900000008A0 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT3_DW0 majorrev 31 24, minorrev 23 16, nextcapptr 15 8, capid 7 0

REG = USBDRD0_UAHC_SUPTPRT3_DW1 0x80016800000008A4 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT3_DW1 name 31 0
REG = USBDRD1_UAHC_SUPTPRT3_DW1 0x80016900000008A4 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT3_DW1 name 31 0

REG = USBDRD0_UAHC_SUPTPRT3_DW2 0x80016800000008A8 MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT3_DW2 psic 31 28, compatprtcnt 15 8, compatprtoff 7 0
REG = USBDRD1_UAHC_SUPTPRT3_DW2 0x80016900000008A8 MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT3_DW2 psic 31 28, compatprtcnt 15 8, compatprtoff 7 0

REG = USBDRD0_UAHC_SUPTPRT3_DW3 0x80016800000008AC MEMORY 4
REG_FIELD = USBDRD0_UAHC_SUPTPRT3_DW3 protslottype 4 0
REG = USBDRD1_UAHC_SUPTPRT3_DW3 0x80016900000008AC MEMORY 4
REG_FIELD = USBDRD1_UAHC_SUPTPRT3_DW3 protslottype 4 0

REG = USBDRD0_UAHC_USBCMD 0x8001680000000020 MEMORY 4
REG_FIELD = USBDRD0_UAHC_USBCMD eu3s 11 11, ewe 10 10, crs 9 9, css 8 8, lhcrst 7 7, hsee 3 3, inte 2 2, hcrst 1 1, r_s 0 0
REG = USBDRD1_UAHC_USBCMD 0x8001690000000020 MEMORY 4
REG_FIELD = USBDRD1_UAHC_USBCMD eu3s 11 11, ewe 10 10, crs 9 9, css 8 8, lhcrst 7 7, hsee 3 3, inte 2 2, hcrst 1 1, r_s 0 0

REG = USBDRD0_UAHC_USBLEGCTLSTS 0x8001680000000884 MEMORY 4
REG_FIELD = USBDRD0_UAHC_USBLEGCTLSTS smi_on_bar 31 31, smi_on_pci_command 30 30, smi_on_os_ownership 29 29, smi_on_hostsystemerr 20 20, smi_on_event_interru 16 16, smi_on_bar_en 15 15, smi_on_pci_command_e 14 14, smi_on_os_ownership_ 13 13, smi_on_hostsystemerr 4 4, usb_smi_en 0 0
REG = USBDRD1_UAHC_USBLEGCTLSTS 0x8001690000000884 MEMORY 4
REG_FIELD = USBDRD1_UAHC_USBLEGCTLSTS smi_on_bar 31 31, smi_on_pci_command 30 30, smi_on_os_ownership 29 29, smi_on_hostsystemerr 20 20, smi_on_event_interru 16 16, smi_on_bar_en 15 15, smi_on_pci_command_e 14 14, smi_on_os_ownership_ 13 13, smi_on_hostsystemerr 4 4, usb_smi_en 0 0

REG = USBDRD0_UAHC_USBLEGSUP 0x8001680000000880 MEMORY 4
REG_FIELD = USBDRD0_UAHC_USBLEGSUP hc_os_owned_semaphor 24 24, hc_bios_owned_semaph 16 16, nextcapptr 15 8, capid 7 0
REG = USBDRD1_UAHC_USBLEGSUP 0x8001690000000880 MEMORY 4
REG_FIELD = USBDRD1_UAHC_USBLEGSUP hc_os_owned_semaphor 24 24, hc_bios_owned_semaph 16 16, nextcapptr 15 8, capid 7 0

REG = USBDRD0_UAHC_USBSTS 0x8001680000000024 MEMORY 4
REG_FIELD = USBDRD0_UAHC_USBSTS hce 12 12, cnr 11 11, sre 10 10, rss 9 9, sss 8 8, pcd 4 4, eint 3 3, hse 2 2, hch 0 0
REG = USBDRD1_UAHC_USBSTS 0x8001690000000024 MEMORY 4
REG_FIELD = USBDRD1_UAHC_USBSTS hce 12 12, cnr 11 11, sre 10 10, rss 9 9, sss 8 8, pcd 4 4, eint 3 3, hse 2 2, hch 0 0

REG = USBDRD0_UCTL_BIST_STATUS 0x8001180068000008 MEMORY 8
REG_FIELD = USBDRD0_UCTL_BIST_STATUS uctl_xm_r_bist_ndone 41 41, uctl_xm_w_bist_ndone 40 40, uahc_ram2_bist_ndone 34 34, uahc_ram1_bist_ndone 33 33, uahc_ram0_bist_ndone 32 32, uctl_xm_r_bist_statu 9 9, uctl_xm_w_bist_statu 8 8, uahc_ram2_bist_statu 2 2, uahc_ram1_bist_statu 1 1, uahc_ram0_bist_statu 0 0
REG = USBDRD1_UCTL_BIST_STATUS 0x8001180069000008 MEMORY 8
REG_FIELD = USBDRD1_UCTL_BIST_STATUS uctl_xm_r_bist_ndone 41 41, uctl_xm_w_bist_ndone 40 40, uahc_ram2_bist_ndone 34 34, uahc_ram1_bist_ndone 33 33, uahc_ram0_bist_ndone 32 32, uctl_xm_r_bist_statu 9 9, uctl_xm_w_bist_statu 8 8, uahc_ram2_bist_statu 2 2, uahc_ram1_bist_statu 1 1, uahc_ram0_bist_statu 0 0

REG = USBDRD0_UCTL_CTL 0x8001180068000000 MEMORY 8
REG_FIELD = USBDRD0_UCTL_CTL clear_bist 63 63, start_bist 62 62, ref_clk_sel 61 60, ssc_en 59 59, ssc_range 58 56, ssc_ref_clk_sel 55 47, mpll_multiplier 46 40, ref_ssp_en 39 39, ref_clk_div2 38 38, ref_clk_fsel 37 32, h_clk_en 30 30, h_clk_byp_sel 29 29, h_clkdiv_rst 28 28, h_clkdiv_sel 26 24, usb3_port_perm_attac 21 21, usb2_port_perm_attac 20 20, usb3_port_disable 18 18, usb2_port_disable 16 16, ss_power_en 14 14, hs_power_en 12 12, csclk_en 4 4, drd_mode 3 3, uphy_rst 2 2, uahc_rst 1 1, uctl_rst 0 0
REG = USBDRD1_UCTL_CTL 0x8001180069000000 MEMORY 8
REG_FIELD = USBDRD1_UCTL_CTL clear_bist 63 63, start_bist 62 62, ref_clk_sel 61 60, ssc_en 59 59, ssc_range 58 56, ssc_ref_clk_sel 55 47, mpll_multiplier 46 40, ref_ssp_en 39 39, ref_clk_div2 38 38, ref_clk_fsel 37 32, h_clk_en 30 30, h_clk_byp_sel 29 29, h_clkdiv_rst 28 28, h_clkdiv_sel 26 24, usb3_port_perm_attac 21 21, usb2_port_perm_attac 20 20, usb3_port_disable 18 18, usb2_port_disable 16 16, ss_power_en 14 14, hs_power_en 12 12, csclk_en 4 4, drd_mode 3 3, uphy_rst 2 2, uahc_rst 1 1, uctl_rst 0 0

REG = USBDRD0_UCTL_ECC 0x80011800680000F0 MEMORY 8
REG_FIELD = USBDRD0_UCTL_ECC ecc_err_source 59 56, ecc_err_syndrome 55 48, ecc_err_address 47 32, uahc_ram2_ecc_flip_s 8 7, uahc_ram2_ecc_cor_di 6 6, uahc_ram1_ecc_flip_s 5 4, uahc_ram1_ecc_cor_di 3 3, uahc_ram0_ecc_flip_s 2 1, uahc_ram0_ecc_cor_di 0 0
REG = USBDRD1_UCTL_ECC 0x80011800690000F0 MEMORY 8
REG_FIELD = USBDRD1_UCTL_ECC ecc_err_source 59 56, ecc_err_syndrome 55 48, ecc_err_address 47 32, uahc_ram2_ecc_flip_s 8 7, uahc_ram2_ecc_cor_di 6 6, uahc_ram1_ecc_flip_s 5 4, uahc_ram1_ecc_cor_di 3 3, uahc_ram0_ecc_flip_s 2 1, uahc_ram0_ecc_cor_di 0 0

REG = USBDRD0_UCTL_HOST_CFG 0x80011800680000E0 MEMORY 8
REG_FIELD = USBDRD0_UCTL_HOST_CFG host_current_belt 59 48, fla 37 32, bme 28 28, oci_en 27 27, oci_active_high_en 26 26, ppc_en 25 25, ppc_active_high_en 24 24
REG = USBDRD1_UCTL_HOST_CFG 0x80011800690000E0 MEMORY 8
REG_FIELD = USBDRD1_UCTL_HOST_CFG host_current_belt 59 48, fla 37 32, bme 28 28, oci_en 27 27, oci_active_high_en 26 26, ppc_en 25 25, ppc_active_high_en 24 24

REG = USBDRD0_UCTL_INTSTAT 0x8001180068000030 MEMORY 8
REG_FIELD = USBDRD0_UCTL_INTSTAT ram2_dbe 21 21, ram2_sbe 20 20, ram1_dbe 19 19, ram1_sbe 18 18, ram0_dbe 17 17, ram0_sbe 16 16, xm_bad_dma 2 2, xs_ncb_oob 1 1
REG = USBDRD1_UCTL_INTSTAT 0x8001180069000030 MEMORY 8
REG_FIELD = USBDRD1_UCTL_INTSTAT ram2_dbe 21 21, ram2_sbe 20 20, ram1_dbe 19 19, ram1_sbe 18 18, ram0_dbe 17 17, ram0_sbe 16 16, xm_bad_dma 2 2, xs_ncb_oob 1 1

REG = USBDRD0_UCTL_PORT000_CFG_HS 0x8001180068000040 MEMORY 8
REG_FIELD = USBDRD0_UCTL_PORT000_CFG_HS comp_dis_tune 57 55, sq_rx_tune 54 52, tx_fsls_tune 51 48, tx_hs_xv_tune 45 44, tx_preemp_amp_tune 43 42, tx_preemp_pulse_tune 40 40, tx_res_tune 39 38, tx_rise_tune 37 36, tx_vref_tune 35 32, otgtune 6 4, vatest_enable 3 2, loopback_enable 1 1, atereset 0 0
REG = USBDRD1_UCTL_PORT000_CFG_HS 0x8001180069000040 MEMORY 8
REG_FIELD = USBDRD1_UCTL_PORT000_CFG_HS comp_dis_tune 57 55, sq_rx_tune 54 52, tx_fsls_tune 51 48, tx_hs_xv_tune 45 44, tx_preemp_amp_tune 43 42, tx_preemp_pulse_tune 40 40, tx_res_tune 39 38, tx_rise_tune 37 36, tx_vref_tune 35 32, otgtune 6 4, vatest_enable 3 2, loopback_enable 1 1, atereset 0 0

REG = USBDRD0_UCTL_PORT000_CFG_SS 0x8001180068000048 MEMORY 8
REG_FIELD = USBDRD0_UCTL_PORT000_CFG_SS tx_vboost_lvl 63 61, los_bias 60 58, lane0_ext_pclk_req 57 57, lane0_tx2rx_loopbk 56 56, pcs_rx_los_mask_val 41 32, pcs_tx_deemph_3p5db 31 26, pcs_tx_deemph_6db 25 20, pcs_tx_swing_full 19 13, lane0_tx_term_offset 12 8, res_tune_ack 5 5, res_tune_req 4 4
REG = USBDRD1_UCTL_PORT000_CFG_SS 0x8001180069000048 MEMORY 8
REG_FIELD = USBDRD1_UCTL_PORT000_CFG_SS tx_vboost_lvl 63 61, los_bias 60 58, lane0_ext_pclk_req 57 57, lane0_tx2rx_loopbk 56 56, pcs_rx_los_mask_val 41 32, pcs_tx_deemph_3p5db 31 26, pcs_tx_deemph_6db 25 20, pcs_tx_swing_full 19 13, lane0_tx_term_offset 12 8, res_tune_ack 5 5, res_tune_req 4 4

REG = USBDRD0_UCTL_PORT000_CR_DBG_CFG 0x8001180068000050 MEMORY 8
REG_FIELD = USBDRD0_UCTL_PORT000_CR_DBG_CFG data_in 47 32, cap_addr 3 3, cap_data 2 2, read 1 1, write 0 0
REG = USBDRD1_UCTL_PORT000_CR_DBG_CFG 0x8001180069000050 MEMORY 8
REG_FIELD = USBDRD1_UCTL_PORT000_CR_DBG_CFG data_in 47 32, cap_addr 3 3, cap_data 2 2, read 1 1, write 0 0

REG = USBDRD0_UCTL_PORT000_CR_DBG_STATUS 0x8001180068000058 MEMORY 8
REG_FIELD = USBDRD0_UCTL_PORT000_CR_DBG_STATUS data_out 47 32, ack 0 0
REG = USBDRD1_UCTL_PORT000_CR_DBG_STATUS 0x8001180069000058 MEMORY 8
REG_FIELD = USBDRD1_UCTL_PORT000_CR_DBG_STATUS data_out 47 32, ack 0 0

REG = USBDRD0_UCTL_SHIM_CFG 0x80011800680000E8 MEMORY 8
REG_FIELD = USBDRD0_UCTL_SHIM_CFG xs_ncb_oob_wrn 63 63, xs_ncb_oob_osrc 59 48, xm_bad_dma_wrn 47 47, xm_bad_dma_type 43 40, dma_read_cmd 12 12, dma_endian_mode 9 8, csr_endian_mode 1 0
REG = USBDRD1_UCTL_SHIM_CFG 0x80011800690000E8 MEMORY 8
REG_FIELD = USBDRD1_UCTL_SHIM_CFG xs_ncb_oob_wrn 63 63, xs_ncb_oob_osrc 59 48, xm_bad_dma_wrn 47 47, xm_bad_dma_type 43 40, dma_read_cmd 12 12, dma_endian_mode 9 8, csr_endian_mode 1 0

REG = USBDRD0_UCTL_SPARE0 0x8001180068000010 MEMORY 8
REG_FIELD = USBDRD0_UCTL_SPARE0 reserved_0_63 0 63
REG = USBDRD1_UCTL_SPARE0 0x8001180069000010 MEMORY 8
REG_FIELD = USBDRD1_UCTL_SPARE0 reserved_0_63 0 63

REG = USBDRD0_UCTL_SPARE1 0x80011800680000F8 MEMORY 8
REG_FIELD = USBDRD0_UCTL_SPARE1 reserved_0_63 0 63
REG = USBDRD1_UCTL_SPARE1 0x80011800690000F8 MEMORY 8
REG_FIELD = USBDRD1_UCTL_SPARE1 reserved_0_63 0 63

REG = VRM0_ALT_FUSE 0x8001180021000018 MEMORY 8
REG_FIELD = VRM0_ALT_FUSE tran_temp 39 32, max_step 31 24, slope 23 16, v_base 15 8, v_max 7 0

REG = VRM0_FUSE_BYPASS 0x8001180021000098 MEMORY 8
REG_FIELD = VRM0_FUSE_BYPASS ctl_hw_bypass 2 2, ctl_fuse_bypass 1 1, ts_fuse_bypass 0 0

REG = VRM0_MISC_INFO 0x8001180021000048 MEMORY 8
REG_FIELD = VRM0_MISC_INFO vrm_type2 42 40, vrm_ctl_rcv_status_e 39 16, vrm_ctl_cur_state 15 13, vrm_type 12 11, boot_seq 10 10, ts_fuse_sts 9 9, vrm_fuse_sts 8 8, cmnd 7 0

REG = VRM0_TELEMETRY_CMND 0x8001180021000038 MEMORY 8
REG_FIELD = VRM0_TELEMETRY_CMND cmnd 1 0

REG = VRM0_TELEMETRY_READ 0x8001180021000028 MEMORY 8
REG_FIELD = VRM0_TELEMETRY_READ iout 31 16, vout 15 0

REG = VRM0_TS_TEMP_CONV_COEFF_FSM 0x8001180021000078 MEMORY 8
REG_FIELD = VRM0_TS_TEMP_CONV_COEFF_FSM t_fsm 57 48, coeff_a 37 32, coeff_b 24 16, coeff_c 11 0

REG = VRM0_TS_TEMP_CONV_CTL 0x8001180021000058 MEMORY 8
REG_FIELD = VRM0_TS_TEMP_CONV_CTL reset_sm 12 12, sw_access 11 11, ts_switch 10 2, ts_curr2en 1 1, strobe 0 0

REG = VRM0_TS_TEMP_CONV_RESULT 0x8001180021000068 MEMORY 8
REG_FIELD = VRM0_TS_TEMP_CONV_RESULT n_valid 23 23, n_value 22 12, temp_valid 11 11, temp_corrected 10 0

REG = VRM0_TS_TEMP_NOFF_MC 0x8001180021000088 MEMORY 8
REG_FIELD = VRM0_TS_TEMP_NOFF_MC mc 27 16, noff 10 0

REG = VRM0_TWS_TWSI_SW 0x8001180021000008 MEMORY 8
REG_FIELD = VRM0_TWS_TWSI_SW v 63 63, sl_only 62 62, eia 61 61, op 60 57, r 56 56, sovr 55 55, size_b3 54 54, size 53 52, scr 51 50, un_addr 49 47, addr 46 40, ia 39 32, data 31 0

REG_WINDOW = AGL_GMX_BAD_REG AGL_GMX_BAD_REG, AGL_GMX_BIST, AGL_GMX_PRT0_CFG, AGL_GMX_RX0_ADR_CAM0, AGL_GMX_RX0_ADR_CAM1, AGL_GMX_RX0_ADR_CAM2, AGL_GMX_RX0_ADR_CAM3, AGL_GMX_RX0_ADR_CAM4, AGL_GMX_RX0_ADR_CAM5, AGL_GMX_RX0_ADR_CAM_EN, AGL_GMX_RX0_ADR_CTL, AGL_GMX_RX0_DECISION, AGL_GMX_RX0_FRM_CHK, AGL_GMX_RX0_FRM_CTL, AGL_GMX_RX0_FRM_MAX, AGL_GMX_RX0_FRM_MIN, AGL_GMX_RX0_IFG, AGL_GMX_RX0_INT_EN, AGL_GMX_RX0_INT_REG, AGL_GMX_RX0_JABBER, AGL_GMX_RX0_PAUSE_DROP_TIME, AGL_GMX_RX0_RX_INBND, AGL_GMX_RX0_STATS_CTL, AGL_GMX_RX0_STATS_OCTS, AGL_GMX_RX0_STATS_OCTS_CTL, AGL_GMX_RX0_STATS_OCTS_DMAC, AGL_GMX_RX0_STATS_OCTS_DRP, AGL_GMX_RX0_STATS_PKTS, AGL_GMX_RX0_STATS_PKTS_BAD, AGL_GMX_RX0_STATS_PKTS_CTL, AGL_GMX_RX0_STATS_PKTS_DMAC, AGL_GMX_RX0_STATS_PKTS_DRP, AGL_GMX_RX0_UDD_SKP, AGL_GMX_RX_BP_DROP0, AGL_GMX_RX_BP_OFF0, AGL_GMX_RX_BP_ON0, AGL_GMX_RX_PRT_INFO, AGL_GMX_RX_TX_STATUS, AGL_GMX_SMAC0, AGL_GMX_STAT_BP, AGL_GMX_TX0_APPEND, AGL_GMX_TX0_CLK, AGL_GMX_TX0_CTL, AGL_GMX_TX0_MIN_PKT, AGL_GMX_TX0_PAUSE_PKT_INTERVAL, AGL_GMX_TX0_PAUSE_PKT_TIME, AGL_GMX_TX0_PAUSE_TOGO, AGL_GMX_TX0_PAUSE_ZERO, AGL_GMX_TX0_SOFT_PAUSE, AGL_GMX_TX0_STAT0
REG_WINDOW = AGL_GMX_TX0_STAT1 AGL_GMX_TX0_STAT1, AGL_GMX_TX0_STAT2, AGL_GMX_TX0_STAT3, AGL_GMX_TX0_STAT4, AGL_GMX_TX0_STAT5, AGL_GMX_TX0_STAT6, AGL_GMX_TX0_STAT7, AGL_GMX_TX0_STAT8, AGL_GMX_TX0_STAT9, AGL_GMX_TX0_STATS_CTL, AGL_GMX_TX0_THRESH, AGL_GMX_TX_BP, AGL_GMX_TX_COL_ATTEMPT, AGL_GMX_TX_IFG, AGL_GMX_TX_INT_EN, AGL_GMX_TX_INT_REG, AGL_GMX_TX_JAM, AGL_GMX_TX_LFSR, AGL_GMX_TX_OVR_BP, AGL_GMX_TX_PAUSE_PKT_DMAC, AGL_GMX_TX_PAUSE_PKT_TYPE, AGL_GMX_WOL_CTL, AGL_PRT0_CTL
REG_WINDOW = BCH BCH_BIST_RESULT, BCH_CMD_BUF, BCH_CTL, BCH_ERR_CFG, BCH_GEN_INT, BCH_GEN_INT_EN
REG_WINDOW = CIU_BIST CIU_BIST, CIU_CIB_L2C_EN0, CIU_CIB_L2C_RAW0, CIU_CIB_LMC0_EN000, CIU_CIB_LMC0_RAW000, CIU_CIB_OCLA0_EN000, CIU_CIB_OCLA0_RAW000, CIU_CIB_RST_EN0, CIU_CIB_RST_RAW0, CIU_CIB_SATA_EN0, CIU_CIB_SATA_RAW0, CIU_CIB_USBDRD0_EN000, CIU_CIB_USBDRD1_EN000, CIU_CIB_USBDRD0_RAW000, CIU_CIB_USBDRD1_RAW000, CIU_DINT, CIU_EN2_IO0_INT, CIU_EN2_IO1_INT, CIU_EN2_IO0_INT_W1C, CIU_EN2_IO1_INT_W1C, CIU_EN2_IO0_INT_W1S, CIU_EN2_IO1_INT_W1S, CIU_EN2_PP0_IP2, CIU_EN2_PP1_IP2, CIU_EN2_PP2_IP2, CIU_EN2_PP3_IP2, CIU_EN2_PP0_IP2_W1C, CIU_EN2_PP1_IP2_W1C, CIU_EN2_PP2_IP2_W1C, CIU_EN2_PP3_IP2_W1C, CIU_EN2_PP0_IP2_W1S, CIU_EN2_PP1_IP2_W1S, CIU_EN2_PP2_IP2_W1S, CIU_EN2_PP3_IP2_W1S, CIU_EN2_PP0_IP3, CIU_EN2_PP1_IP3, CIU_EN2_PP2_IP3, CIU_EN2_PP3_IP3, CIU_EN2_PP0_IP3_W1C, CIU_EN2_PP1_IP3_W1C, CIU_EN2_PP2_IP3_W1C, CIU_EN2_PP3_IP3_W1C, CIU_EN2_PP0_IP3_W1S, CIU_EN2_PP1_IP3_W1S, CIU_EN2_PP2_IP3_W1S, CIU_EN2_PP3_IP3_W1S, CIU_EN2_PP0_IP4, CIU_EN2_PP1_IP4, CIU_EN2_PP2_IP4, CIU_EN2_PP3_IP4
REG_WINDOW = CIU_EN2_PP0_IP4_W1C CIU_EN2_PP0_IP4_W1C, CIU_EN2_PP1_IP4_W1C, CIU_EN2_PP2_IP4_W1C, CIU_EN2_PP3_IP4_W1C, CIU_EN2_PP0_IP4_W1S, CIU_EN2_PP1_IP4_W1S, CIU_EN2_PP2_IP4_W1S, CIU_EN2_PP3_IP4_W1S, CIU_FUSE, CIU_GSTOP, CIU_INT0_EN0, CIU_INT1_EN0, CIU_INT2_EN0, CIU_INT3_EN0, CIU_INT4_EN0, CIU_INT5_EN0, CIU_INT6_EN0, CIU_INT7_EN0, CIU_INT32_EN0, CIU_INT33_EN0, CIU_INT0_EN0_W1C, CIU_INT1_EN0_W1C, CIU_INT2_EN0_W1C, CIU_INT3_EN0_W1C, CIU_INT4_EN0_W1C, CIU_INT5_EN0_W1C, CIU_INT6_EN0_W1C, CIU_INT7_EN0_W1C, CIU_INT32_EN0_W1C, CIU_INT33_EN0_W1C, CIU_INT0_EN0_W1S, CIU_INT1_EN0_W1S, CIU_INT2_EN0_W1S, CIU_INT3_EN0_W1S, CIU_INT4_EN0_W1S, CIU_INT5_EN0_W1S, CIU_INT6_EN0_W1S, CIU_INT7_EN0_W1S, CIU_INT32_EN0_W1S, CIU_INT33_EN0_W1S, CIU_INT0_EN1, CIU_INT1_EN1, CIU_INT2_EN1, CIU_INT3_EN1, CIU_INT4_EN1, CIU_INT5_EN1, CIU_INT6_EN1, CIU_INT7_EN1, CIU_INT32_EN1, CIU_INT33_EN1
REG_WINDOW = CIU_INT0_EN1_W1C CIU_INT0_EN1_W1C, CIU_INT1_EN1_W1C, CIU_INT2_EN1_W1C, CIU_INT3_EN1_W1C, CIU_INT4_EN1_W1C, CIU_INT5_EN1_W1C, CIU_INT6_EN1_W1C, CIU_INT7_EN1_W1C, CIU_INT32_EN1_W1C, CIU_INT33_EN1_W1C, CIU_INT0_EN1_W1S, CIU_INT1_EN1_W1S, CIU_INT2_EN1_W1S, CIU_INT3_EN1_W1S, CIU_INT4_EN1_W1S, CIU_INT5_EN1_W1S, CIU_INT6_EN1_W1S, CIU_INT7_EN1_W1S, CIU_INT32_EN1_W1S, CIU_INT33_EN1_W1S, CIU_INT0_EN4_0, CIU_INT1_EN4_0, CIU_INT2_EN4_0, CIU_INT3_EN4_0, CIU_INT0_EN4_0_W1C, CIU_INT1_EN4_0_W1C, CIU_INT2_EN4_0_W1C, CIU_INT3_EN4_0_W1C, CIU_INT0_EN4_0_W1S, CIU_INT1_EN4_0_W1S, CIU_INT2_EN4_0_W1S, CIU_INT3_EN4_0_W1S, CIU_INT0_EN4_1, CIU_INT1_EN4_1, CIU_INT2_EN4_1, CIU_INT3_EN4_1, CIU_INT0_EN4_1_W1C, CIU_INT1_EN4_1_W1C, CIU_INT2_EN4_1_W1C, CIU_INT3_EN4_1_W1C, CIU_INT0_EN4_1_W1S, CIU_INT1_EN4_1_W1S, CIU_INT2_EN4_1_W1S, CIU_INT3_EN4_1_W1S, CIU_INT0_SUM0, CIU_INT1_SUM0, CIU_INT2_SUM0, CIU_INT3_SUM0, CIU_INT4_SUM0, CIU_INT5_SUM0
REG_WINDOW = CIU_INT6_SUM0 CIU_INT6_SUM0, CIU_INT7_SUM0, CIU_INT32_SUM0, CIU_INT0_SUM4, CIU_INT1_SUM4, CIU_INT2_SUM4, CIU_INT3_SUM4, CIU_INT33_SUM0, CIU_INT_SUM1, CIU_INTR_SLOWDOWN, CIU_MBOX_CLR0, CIU_MBOX_CLR1, CIU_MBOX_CLR2, CIU_MBOX_CLR3, CIU_MBOX_SET0, CIU_MBOX_SET1, CIU_MBOX_SET2, CIU_MBOX_SET3, CIU_NMI, CIU_PCI_INTA, CIU_PP_DBG, CIU_PP_POKE0, CIU_PP_POKE1, CIU_PP_POKE2, CIU_PP_POKE3, CIU_PP_RST, CIU_PP_RST_PENDING, CIU_SOFT_BIST, CIU_SUM1_IO0_INT, CIU_SUM1_IO1_INT, CIU_SUM1_PP0_IP2, CIU_SUM1_PP1_IP2, CIU_SUM1_PP2_IP2, CIU_SUM1_PP3_IP2, CIU_SUM1_PP0_IP3, CIU_SUM1_PP1_IP3, CIU_SUM1_PP2_IP3, CIU_SUM1_PP3_IP3, CIU_SUM1_PP0_IP4, CIU_SUM1_PP1_IP4, CIU_SUM1_PP2_IP4, CIU_SUM1_PP3_IP4, CIU_SUM2_IO0_INT, CIU_SUM2_IO1_INT, CIU_SUM2_PP0_IP2, CIU_SUM2_PP1_IP2, CIU_SUM2_PP2_IP2, CIU_SUM2_PP3_IP2, CIU_SUM2_PP0_IP3, CIU_SUM2_PP1_IP3
REG_WINDOW = CIU_SUM2_PP2_IP3 CIU_SUM2_PP2_IP3, CIU_SUM2_PP3_IP3, CIU_SUM2_PP0_IP4, CIU_SUM2_PP1_IP4, CIU_SUM2_PP2_IP4, CIU_SUM2_PP3_IP4, CIU_TIM0, CIU_TIM1, CIU_TIM2, CIU_TIM3, CIU_TIM4, CIU_TIM5, CIU_TIM6, CIU_TIM7, CIU_TIM8, CIU_TIM9, CIU_TIM_MULTI_CAST, CIU_WDOG0, CIU_WDOG1, CIU_WDOG2, CIU_WDOG3
REG_WINDOW = DFA DFA_BIST0, DFA_BIST1, DFA_CONFIG, DFA_CONTROL, DFA_DBELL, DFA_DEBUG0, DFA_DEBUG1, DFA_DEBUG2, DFA_DEBUG3, DFA_DIFCTL, DFA_DIFRDPTR, DFA_DTCFADR, DFA_ERROR, DFA_INTMSK, DFA_MEMHIDAT, DFA_PFC0_CNT, DFA_PFC0_CTL, DFA_PFC1_CNT, DFA_PFC1_CTL, DFA_PFC2_CNT, DFA_PFC2_CTL, DFA_PFC3_CNT, DFA_PFC3_CTL, DFA_PFC_GCTL
REG_WINDOW = DPI_BIST_STATUS DPI_BIST_STATUS, DPI_CTL, DPI_DMA0_COUNTS, DPI_DMA1_COUNTS, DPI_DMA2_COUNTS, DPI_DMA3_COUNTS, DPI_DMA4_COUNTS, DPI_DMA5_COUNTS, DPI_DMA6_COUNTS, DPI_DMA7_COUNTS, DPI_DMA0_DBELL, DPI_DMA1_DBELL, DPI_DMA2_DBELL, DPI_DMA3_DBELL, DPI_DMA4_DBELL, DPI_DMA5_DBELL, DPI_DMA6_DBELL, DPI_DMA7_DBELL, DPI_DMA0_ERR_RSP_STATUS, DPI_DMA1_ERR_RSP_STATUS, DPI_DMA2_ERR_RSP_STATUS, DPI_DMA3_ERR_RSP_STATUS, DPI_DMA4_ERR_RSP_STATUS, DPI_DMA5_ERR_RSP_STATUS, DPI_DMA6_ERR_RSP_STATUS, DPI_DMA7_ERR_RSP_STATUS, DPI_DMA0_IBUFF_SADDR, DPI_DMA1_IBUFF_SADDR, DPI_DMA2_IBUFF_SADDR, DPI_DMA3_IBUFF_SADDR, DPI_DMA4_IBUFF_SADDR, DPI_DMA5_IBUFF_SADDR, DPI_DMA6_IBUFF_SADDR, DPI_DMA7_IBUFF_SADDR, DPI_DMA0_IFLIGHT, DPI_DMA1_IFLIGHT, DPI_DMA2_IFLIGHT, DPI_DMA3_IFLIGHT, DPI_DMA4_IFLIGHT, DPI_DMA5_IFLIGHT, DPI_DMA6_IFLIGHT, DPI_DMA7_IFLIGHT, DPI_DMA0_NADDR, DPI_DMA1_NADDR, DPI_DMA2_NADDR, DPI_DMA3_NADDR, DPI_DMA4_NADDR, DPI_DMA5_NADDR, DPI_DMA6_NADDR, DPI_DMA7_NADDR
REG_WINDOW = DPI_DMA0_REQBNK0 DPI_DMA0_REQBNK0, DPI_DMA1_REQBNK0, DPI_DMA2_REQBNK0, DPI_DMA3_REQBNK0, DPI_DMA4_REQBNK0, DPI_DMA5_REQBNK0, DPI_DMA6_REQBNK0, DPI_DMA7_REQBNK0, DPI_DMA0_REQBNK1, DPI_DMA1_REQBNK1, DPI_DMA2_REQBNK1, DPI_DMA3_REQBNK1, DPI_DMA4_REQBNK1, DPI_DMA5_REQBNK1, DPI_DMA6_REQBNK1, DPI_DMA7_REQBNK1, DPI_DMA_CONTROL, DPI_DMA_ENG0_EN, DPI_DMA_ENG1_EN, DPI_DMA_ENG2_EN, DPI_DMA_ENG3_EN, DPI_DMA_ENG4_EN, DPI_DMA_ENG5_EN, DPI_DMA_PP0_CNT, DPI_DMA_PP1_CNT, DPI_DMA_PP2_CNT, DPI_DMA_PP3_CNT, DPI_ENG0_BUF, DPI_ENG1_BUF, DPI_ENG2_BUF, DPI_ENG3_BUF, DPI_ENG4_BUF, DPI_ENG5_BUF, DPI_INFO_REG, DPI_INT_EN, DPI_INT_REG, DPI_NCB0_CFG, DPI_PINT_INFO, DPI_PKT_ERR_RSP, DPI_REQ_ERR_RSP, DPI_REQ_ERR_RSP_EN, DPI_REQ_ERR_RST, DPI_REQ_ERR_RST_EN, DPI_REQ_ERR_SKIP_COMP, DPI_REQ_GBL_EN, DPI_SLI_PRT0_CFG, DPI_SLI_PRT1_CFG, DPI_SLI_PRT2_CFG, DPI_SLI_PRT0_ERR, DPI_SLI_PRT1_ERR
REG_WINDOW = DPI_SLI_PRT2_ERR DPI_SLI_PRT2_ERR, DPI_SLI_PRT0_ERR_INFO, DPI_SLI_PRT1_ERR_INFO, DPI_SLI_PRT2_ERR_INFO
REG_WINDOW = DTX_AGL_BCST_RSP DTX_AGL_BCST_RSP, DTX_AGL_CTL, DTX_AGL_DAT0, DTX_AGL_DAT1, DTX_AGL_ENA0, DTX_AGL_ENA1, DTX_AGL_SEL0, DTX_AGL_SEL1, DTX_BROADCAST_CTL, DTX_BROADCAST_ENA0, DTX_BROADCAST_ENA1, DTX_BROADCAST_SEL0, DTX_BROADCAST_SEL1, DTX_DFA_BCST_RSP, DTX_DFA_CTL, DTX_DFA_DAT0, DTX_DFA_DAT1, DTX_DFA_ENA0, DTX_DFA_ENA1, DTX_DFA_SEL0, DTX_DFA_SEL1, DTX_DPI_BCST_RSP, DTX_DPI_CTL, DTX_DPI_DAT0, DTX_DPI_DAT1, DTX_DPI_ENA0, DTX_DPI_ENA1, DTX_DPI_SEL0, DTX_DPI_SEL1, DTX_FPA_BCST_RSP, DTX_FPA_CTL, DTX_FPA_DAT0, DTX_FPA_DAT1, DTX_FPA_ENA0, DTX_FPA_ENA1, DTX_FPA_SEL0, DTX_FPA_SEL1, DTX_GMX0_BCST_RSP, DTX_GMX1_BCST_RSP, DTX_GMX0_CTL, DTX_GMX1_CTL, DTX_GMX0_DAT000, DTX_GMX0_DAT001, DTX_GMX1_DAT000, DTX_GMX1_DAT001, DTX_GMX0_ENA000, DTX_GMX0_ENA001, DTX_GMX1_ENA000, DTX_GMX1_ENA001, DTX_GMX0_SEL000
REG_WINDOW = DTX_GMX0_SEL001 DTX_GMX0_SEL001, DTX_GMX1_SEL000, DTX_GMX1_SEL001, DTX_IOB_BCST_RSP, DTX_IOB_CTL, DTX_IOB_DAT0, DTX_IOB_DAT1, DTX_IOB_ENA0, DTX_IOB_ENA1, DTX_IOB_SEL0, DTX_IOB_SEL1, DTX_IPD_BCST_RSP, DTX_IPD_CTL, DTX_IPD_DAT0, DTX_IPD_DAT1, DTX_IPD_ENA0, DTX_IPD_ENA1, DTX_IPD_SEL0, DTX_IPD_SEL1, DTX_KEY_BCST_RSP, DTX_KEY_CTL, DTX_KEY_DAT0, DTX_KEY_DAT1, DTX_KEY_ENA0, DTX_KEY_ENA1, DTX_KEY_SEL0, DTX_KEY_SEL1, DTX_L2C_CBC0_BCST_RSP, DTX_L2C_CBC0_CTL, DTX_L2C_CBC0_DAT000, DTX_L2C_CBC0_DAT001, DTX_L2C_CBC0_ENA000, DTX_L2C_CBC0_ENA001, DTX_L2C_CBC0_SEL000, DTX_L2C_CBC0_SEL001, DTX_L2C_MCI0_BCST_RSP, DTX_L2C_MCI0_CTL, DTX_L2C_MCI0_DAT000, DTX_L2C_MCI0_DAT001, DTX_L2C_MCI0_ENA000, DTX_L2C_MCI0_ENA001, DTX_L2C_MCI0_SEL000, DTX_L2C_MCI0_SEL001, DTX_L2C_TAD0_BCST_RSP, DTX_L2C_TAD0_CTL, DTX_L2C_TAD0_DAT000, DTX_L2C_TAD0_DAT001, DTX_L2C_TAD0_ENA000, DTX_L2C_TAD0_ENA001, DTX_L2C_TAD0_SEL000
REG_WINDOW = DTX_L2C_TAD0_SEL001 DTX_L2C_TAD0_SEL001, DTX_LMC0_BCST_RSP, DTX_LMC0_CTL, DTX_LMC0_DAT000, DTX_LMC0_DAT001, DTX_LMC0_ENA000, DTX_LMC0_ENA001, DTX_LMC0_SEL000, DTX_LMC0_SEL001, DTX_MIO_BCST_RSP, DTX_MIO_CTL, DTX_MIO_DAT0, DTX_MIO_DAT1, DTX_MIO_ENA0, DTX_MIO_ENA1, DTX_MIO_SEL0, DTX_MIO_SEL1, DTX_PCS0_BCST_RSP, DTX_PCS1_BCST_RSP, DTX_PCS0_CTL, DTX_PCS1_CTL, DTX_PCS0_DAT000, DTX_PCS0_DAT001, DTX_PCS1_DAT000, DTX_PCS1_DAT001, DTX_PCS0_ENA000, DTX_PCS0_ENA001, DTX_PCS1_ENA000, DTX_PCS1_ENA001, DTX_PCS0_SEL000, DTX_PCS0_SEL001, DTX_PCS1_SEL000, DTX_PCS1_SEL001, DTX_PEM0_BCST_RSP, DTX_PEM1_BCST_RSP, DTX_PEM2_BCST_RSP, DTX_PEM0_CTL, DTX_PEM1_CTL, DTX_PEM2_CTL, DTX_PEM0_DAT000, DTX_PEM0_DAT001, DTX_PEM1_DAT000, DTX_PEM1_DAT001, DTX_PEM2_DAT000, DTX_PEM2_DAT001, DTX_PEM0_ENA000, DTX_PEM0_ENA001, DTX_PEM1_ENA000, DTX_PEM1_ENA001, DTX_PEM2_ENA000
REG_WINDOW = DTX_PEM2_ENA001 DTX_PEM2_ENA001, DTX_PEM0_SEL000, DTX_PEM0_SEL001, DTX_PEM1_SEL000, DTX_PEM1_SEL001, DTX_PEM2_SEL000, DTX_PEM2_SEL001, DTX_PIP_BCST_RSP, DTX_PIP_CTL, DTX_PIP_DAT0, DTX_PIP_DAT1, DTX_PIP_ENA0, DTX_PIP_ENA1, DTX_PIP_SEL0, DTX_PIP_SEL1, DTX_PKO_BCST_RSP, DTX_PKO_CTL, DTX_PKO_DAT0, DTX_PKO_DAT1, DTX_PKO_ENA0, DTX_PKO_ENA1, DTX_PKO_SEL0, DTX_PKO_SEL1, DTX_POW_BCST_RSP, DTX_POW_CTL, DTX_POW_DAT0, DTX_POW_DAT1, DTX_POW_ENA0, DTX_POW_ENA1, DTX_POW_SEL0, DTX_POW_SEL1, DTX_RST_BCST_RSP, DTX_RST_CTL, DTX_RST_DAT0, DTX_RST_DAT1, DTX_RST_ENA0, DTX_RST_ENA1, DTX_RST_SEL0, DTX_RST_SEL1, DTX_SATA_BCST_RSP, DTX_SATA_CTL, DTX_SATA_DAT0, DTX_SATA_DAT1, DTX_SATA_ENA0, DTX_SATA_ENA1, DTX_SATA_SEL0, DTX_SATA_SEL1, DTX_SLI_BCST_RSP, DTX_SLI_CTL, DTX_SLI_DAT0
REG_WINDOW = DTX_SLI_DAT1 DTX_SLI_DAT1, DTX_SLI_ENA0, DTX_SLI_ENA1, DTX_SLI_SEL0, DTX_SLI_SEL1, DTX_TIM_BCST_RSP, DTX_TIM_CTL, DTX_TIM_DAT0, DTX_TIM_DAT1, DTX_TIM_ENA0, DTX_TIM_ENA1, DTX_TIM_SEL0, DTX_TIM_SEL1, DTX_USBDRD0_BCST_RSP, DTX_USBDRD1_BCST_RSP, DTX_USBDRD0_CTL, DTX_USBDRD1_CTL, DTX_USBDRD0_DAT000, DTX_USBDRD0_DAT001, DTX_USBDRD1_DAT000, DTX_USBDRD1_DAT001, DTX_USBDRD0_ENA000, DTX_USBDRD0_ENA001, DTX_USBDRD1_ENA000, DTX_USBDRD1_ENA001, DTX_USBDRD0_SEL000, DTX_USBDRD0_SEL001, DTX_USBDRD1_SEL000, DTX_USBDRD1_SEL001
REG_WINDOW = FPA_ADDR_RANGE_ERROR FPA_ADDR_RANGE_ERROR, FPA_BIST_STATUS, FPA_CTL_STATUS, FPA_FPF1_MARKS, FPA_FPF2_MARKS, FPA_FPF3_MARKS, FPA_FPF4_MARKS, FPA_FPF5_MARKS, FPA_FPF6_MARKS, FPA_FPF7_MARKS, FPA_FPF1_SIZE, FPA_FPF2_SIZE, FPA_FPF3_SIZE, FPA_FPF4_SIZE, FPA_FPF5_SIZE, FPA_FPF6_SIZE, FPA_FPF7_SIZE, FPA_FPF0_MARKS, FPA_FPF0_SIZE, FPA_INT_ENB, FPA_INT_SUM, FPA_PACKET_THRESHOLD, FPA_POOL0_END_ADDR, FPA_POOL1_END_ADDR, FPA_POOL2_END_ADDR, FPA_POOL3_END_ADDR, FPA_POOL4_END_ADDR, FPA_POOL5_END_ADDR, FPA_POOL6_END_ADDR, FPA_POOL7_END_ADDR, FPA_POOL0_START_ADDR, FPA_POOL1_START_ADDR, FPA_POOL2_START_ADDR, FPA_POOL3_START_ADDR, FPA_POOL4_START_ADDR, FPA_POOL5_START_ADDR, FPA_POOL6_START_ADDR, FPA_POOL7_START_ADDR, FPA_POOL0_THRESHOLD, FPA_POOL1_THRESHOLD, FPA_POOL2_THRESHOLD, FPA_POOL3_THRESHOLD, FPA_POOL4_THRESHOLD, FPA_POOL5_THRESHOLD, FPA_POOL6_THRESHOLD, FPA_POOL7_THRESHOLD, FPA_QUE0_AVAILABLE, FPA_QUE1_AVAILABLE, FPA_QUE2_AVAILABLE, FPA_QUE3_AVAILABLE
REG_WINDOW = FPA_QUE4_AVAILABLE FPA_QUE4_AVAILABLE, FPA_QUE5_AVAILABLE, FPA_QUE6_AVAILABLE, FPA_QUE7_AVAILABLE, FPA_QUE0_PAGE_INDEX, FPA_QUE1_PAGE_INDEX, FPA_QUE2_PAGE_INDEX, FPA_QUE3_PAGE_INDEX, FPA_QUE4_PAGE_INDEX, FPA_QUE5_PAGE_INDEX, FPA_QUE6_PAGE_INDEX, FPA_QUE7_PAGE_INDEX, FPA_QUE_ACT, FPA_QUE_EXP, FPA_WQE_THRESHOLD
REG_WINDOW = GMX0_BAD_REG GMX0_BAD_REG, GMX0_BIST, GMX0_CLK_EN, GMX0_HG2_CONTROL, GMX0_INF_MODE, GMX0_NXA_ADR, GMX0_PRT000_CBFC_CTL, GMX0_PRT000_CFG, GMX0_PRT001_CFG, GMX0_PRT002_CFG, GMX0_PRT003_CFG, GMX0_QSGMII_CTL, GMX0_RX000_ADR_CAM0, GMX0_RX001_ADR_CAM0, GMX0_RX002_ADR_CAM0, GMX0_RX003_ADR_CAM0, GMX0_RX000_ADR_CAM1, GMX0_RX001_ADR_CAM1, GMX0_RX002_ADR_CAM1, GMX0_RX003_ADR_CAM1, GMX0_RX000_ADR_CAM2, GMX0_RX001_ADR_CAM2, GMX0_RX002_ADR_CAM2, GMX0_RX003_ADR_CAM2, GMX0_RX000_ADR_CAM3, GMX0_RX001_ADR_CAM3, GMX0_RX002_ADR_CAM3, GMX0_RX003_ADR_CAM3, GMX0_RX000_ADR_CAM4, GMX0_RX001_ADR_CAM4, GMX0_RX002_ADR_CAM4, GMX0_RX003_ADR_CAM4, GMX0_RX000_ADR_CAM5, GMX0_RX001_ADR_CAM5, GMX0_RX002_ADR_CAM5, GMX0_RX003_ADR_CAM5, GMX0_RX000_ADR_CAM_ALL_EN, GMX0_RX001_ADR_CAM_ALL_EN, GMX0_RX002_ADR_CAM_ALL_EN, GMX0_RX003_ADR_CAM_ALL_EN, GMX0_RX000_ADR_CAM_EN, GMX0_RX001_ADR_CAM_EN, GMX0_RX002_ADR_CAM_EN, GMX0_RX003_ADR_CAM_EN, GMX0_RX000_ADR_CTL, GMX0_RX001_ADR_CTL, GMX0_RX002_ADR_CTL, GMX0_RX003_ADR_CTL, GMX0_RX000_DECISION, GMX0_RX001_DECISION
REG_WINDOW = GMX0_RX002_DECISION GMX0_RX002_DECISION, GMX0_RX003_DECISION, GMX0_RX000_FRM_CHK, GMX0_RX001_FRM_CHK, GMX0_RX002_FRM_CHK, GMX0_RX003_FRM_CHK, GMX0_RX000_FRM_CTL, GMX0_RX001_FRM_CTL, GMX0_RX002_FRM_CTL, GMX0_RX003_FRM_CTL, GMX0_RX000_IFG, GMX0_RX001_IFG, GMX0_RX002_IFG, GMX0_RX003_IFG, GMX0_RX000_INT_EN, GMX0_RX001_INT_EN, GMX0_RX002_INT_EN, GMX0_RX003_INT_EN, GMX0_RX000_INT_REG, GMX0_RX001_INT_REG, GMX0_RX002_INT_REG, GMX0_RX003_INT_REG, GMX0_RX000_JABBER, GMX0_RX001_JABBER, GMX0_RX002_JABBER, GMX0_RX003_JABBER, GMX0_RX000_PAUSE_DROP_TIME, GMX0_RX001_PAUSE_DROP_TIME, GMX0_RX002_PAUSE_DROP_TIME, GMX0_RX003_PAUSE_DROP_TIME, GMX0_RX000_STATS_CTL, GMX0_RX001_STATS_CTL, GMX0_RX002_STATS_CTL, GMX0_RX003_STATS_CTL, GMX0_RX000_STATS_OCTS, GMX0_RX001_STATS_OCTS, GMX0_RX002_STATS_OCTS, GMX0_RX003_STATS_OCTS, GMX0_RX000_STATS_OCTS_CTL, GMX0_RX001_STATS_OCTS_CTL, GMX0_RX002_STATS_OCTS_CTL, GMX0_RX003_STATS_OCTS_CTL, GMX0_RX000_STATS_OCTS_DMAC, GMX0_RX001_STATS_OCTS_DMAC, GMX0_RX002_STATS_OCTS_DMAC, GMX0_RX003_STATS_OCTS_DMAC, GMX0_RX000_STATS_OCTS_DRP, GMX0_RX001_STATS_OCTS_DRP, GMX0_RX002_STATS_OCTS_DRP, GMX0_RX003_STATS_OCTS_DRP
REG_WINDOW = GMX0_RX000_STATS_PKTS GMX0_RX000_STATS_PKTS, GMX0_RX001_STATS_PKTS, GMX0_RX002_STATS_PKTS, GMX0_RX003_STATS_PKTS, GMX0_RX000_STATS_PKTS_BAD, GMX0_RX001_STATS_PKTS_BAD, GMX0_RX002_STATS_PKTS_BAD, GMX0_RX003_STATS_PKTS_BAD, GMX0_RX000_STATS_PKTS_CTL, GMX0_RX001_STATS_PKTS_CTL, GMX0_RX002_STATS_PKTS_CTL, GMX0_RX003_STATS_PKTS_CTL, GMX0_RX000_STATS_PKTS_DMAC, GMX0_RX001_STATS_PKTS_DMAC, GMX0_RX002_STATS_PKTS_DMAC, GMX0_RX003_STATS_PKTS_DMAC, GMX0_RX000_STATS_PKTS_DRP, GMX0_RX001_STATS_PKTS_DRP, GMX0_RX002_STATS_PKTS_DRP, GMX0_RX003_STATS_PKTS_DRP, GMX0_RX000_UDD_SKP, GMX0_RX001_UDD_SKP, GMX0_RX002_UDD_SKP, GMX0_RX003_UDD_SKP, GMX0_RX_BP_DROP000, GMX0_RX_BP_DROP001, GMX0_RX_BP_DROP002, GMX0_RX_BP_DROP003, GMX0_RX_BP_OFF000, GMX0_RX_BP_OFF001, GMX0_RX_BP_OFF002, GMX0_RX_BP_OFF003, GMX0_RX_BP_ON000, GMX0_RX_BP_ON001, GMX0_RX_BP_ON002, GMX0_RX_BP_ON003, GMX0_RX_HG2_STATUS, GMX0_RX_PRT_INFO, GMX0_RX_PRTS, GMX0_RX_XAUI_BAD_COL, GMX0_RX_XAUI_CTL, GMX0_RXAUI_CTL, GMX0_SMAC000, GMX0_SMAC001, GMX0_SMAC002, GMX0_SMAC003, GMX0_STAT_BP, GMX0_TB_REG, GMX0_TX000_APPEND, GMX0_TX001_APPEND
REG_WINDOW = GMX0_TX002_APPEND GMX0_TX002_APPEND, GMX0_TX003_APPEND, GMX0_TX000_BCK_CRDT, GMX0_TX001_BCK_CRDT, GMX0_TX002_BCK_CRDT, GMX0_TX003_BCK_CRDT, GMX0_TX000_BURST, GMX0_TX001_BURST, GMX0_TX002_BURST, GMX0_TX003_BURST, GMX0_TX000_CBFC_XOFF, GMX0_TX000_CBFC_XON, GMX0_TX000_CTL, GMX0_TX001_CTL, GMX0_TX002_CTL, GMX0_TX003_CTL, GMX0_TX000_JAM_MODE, GMX0_TX001_JAM_MODE, GMX0_TX002_JAM_MODE, GMX0_TX003_JAM_MODE, GMX0_TX000_MIN_PKT, GMX0_TX001_MIN_PKT, GMX0_TX002_MIN_PKT, GMX0_TX003_MIN_PKT, GMX0_TX000_PAUSE_PKT_INTERVAL, GMX0_TX001_PAUSE_PKT_INTERVAL, GMX0_TX002_PAUSE_PKT_INTERVAL, GMX0_TX003_PAUSE_PKT_INTERVAL, GMX0_TX000_PAUSE_PKT_TIME, GMX0_TX001_PAUSE_PKT_TIME, GMX0_TX002_PAUSE_PKT_TIME, GMX0_TX003_PAUSE_PKT_TIME, GMX0_TX000_PAUSE_TOGO, GMX0_TX001_PAUSE_TOGO, GMX0_TX002_PAUSE_TOGO, GMX0_TX003_PAUSE_TOGO, GMX0_TX000_PAUSE_ZERO, GMX0_TX001_PAUSE_ZERO, GMX0_TX002_PAUSE_ZERO, GMX0_TX003_PAUSE_ZERO, GMX0_TX000_SGMII_CTL, GMX0_TX001_SGMII_CTL, GMX0_TX002_SGMII_CTL, GMX0_TX003_SGMII_CTL, GMX0_TX000_SLOT, GMX0_TX001_SLOT, GMX0_TX002_SLOT, GMX0_TX003_SLOT, GMX0_TX000_SOFT_PAUSE, GMX0_TX001_SOFT_PAUSE
REG_WINDOW = GMX0_TX002_SOFT_PAUSE GMX0_TX002_SOFT_PAUSE, GMX0_TX003_SOFT_PAUSE, GMX0_TX000_STAT0, GMX0_TX001_STAT0, GMX0_TX002_STAT0, GMX0_TX003_STAT0, GMX0_TX000_STAT1, GMX0_TX001_STAT1, GMX0_TX002_STAT1, GMX0_TX003_STAT1, GMX0_TX000_STAT2, GMX0_TX001_STAT2, GMX0_TX002_STAT2, GMX0_TX003_STAT2, GMX0_TX000_STAT3, GMX0_TX001_STAT3, GMX0_TX002_STAT3, GMX0_TX003_STAT3, GMX0_TX000_STAT4, GMX0_TX001_STAT4, GMX0_TX002_STAT4, GMX0_TX003_STAT4, GMX0_TX000_STAT5, GMX0_TX001_STAT5, GMX0_TX002_STAT5, GMX0_TX003_STAT5, GMX0_TX000_STAT6, GMX0_TX001_STAT6, GMX0_TX002_STAT6, GMX0_TX003_STAT6, GMX0_TX000_STAT7, GMX0_TX001_STAT7, GMX0_TX002_STAT7, GMX0_TX003_STAT7, GMX0_TX000_STAT8, GMX0_TX001_STAT8, GMX0_TX002_STAT8, GMX0_TX003_STAT8, GMX0_TX000_STAT9, GMX0_TX001_STAT9, GMX0_TX002_STAT9, GMX0_TX003_STAT9, GMX0_TX000_STATS_CTL, GMX0_TX001_STATS_CTL, GMX0_TX002_STATS_CTL, GMX0_TX003_STATS_CTL, GMX0_TX000_THRESH, GMX0_TX001_THRESH, GMX0_TX002_THRESH, GMX0_TX003_THRESH
REG_WINDOW = GMX0_TX_BP GMX0_TX_BP, GMX0_TX_COL_ATTEMPT, GMX0_TX_CORRUPT, GMX0_TX_HG2_REG1, GMX0_TX_HG2_REG2, GMX0_TX_IFG, GMX0_TX_INT_EN, GMX0_TX_INT_REG, GMX0_TX_JAM, GMX0_TX_LFSR, GMX0_TX_OVR_BP, GMX0_TX_PAUSE_PKT_DMAC, GMX0_TX_PAUSE_PKT_TYPE, GMX0_TX_PRTS, GMX0_TX_XAUI_CTL, GMX0_WOL_CTL, GMX0_XAUI_EXT_LOOPBACK
REG_WINDOW = GMX1_BAD_REG GMX1_BAD_REG, GMX1_BIST, GMX1_CLK_EN, GMX1_HG2_CONTROL, GMX1_INF_MODE, GMX1_NXA_ADR, GMX1_PRT000_CBFC_CTL, GMX1_PRT000_CFG, GMX1_PRT001_CFG, GMX1_PRT002_CFG, GMX1_PRT003_CFG, GMX1_QSGMII_CTL, GMX1_RX000_ADR_CAM0, GMX1_RX001_ADR_CAM0, GMX1_RX002_ADR_CAM0, GMX1_RX003_ADR_CAM0, GMX1_RX000_ADR_CAM1, GMX1_RX001_ADR_CAM1, GMX1_RX002_ADR_CAM1, GMX1_RX003_ADR_CAM1, GMX1_RX000_ADR_CAM2, GMX1_RX001_ADR_CAM2, GMX1_RX002_ADR_CAM2, GMX1_RX003_ADR_CAM2, GMX1_RX000_ADR_CAM3, GMX1_RX001_ADR_CAM3, GMX1_RX002_ADR_CAM3, GMX1_RX003_ADR_CAM3, GMX1_RX000_ADR_CAM4, GMX1_RX001_ADR_CAM4, GMX1_RX002_ADR_CAM4, GMX1_RX003_ADR_CAM4, GMX1_RX000_ADR_CAM5, GMX1_RX001_ADR_CAM5, GMX1_RX002_ADR_CAM5, GMX1_RX003_ADR_CAM5, GMX1_RX000_ADR_CAM_ALL_EN, GMX1_RX001_ADR_CAM_ALL_EN, GMX1_RX002_ADR_CAM_ALL_EN, GMX1_RX003_ADR_CAM_ALL_EN, GMX1_RX000_ADR_CAM_EN, GMX1_RX001_ADR_CAM_EN, GMX1_RX002_ADR_CAM_EN, GMX1_RX003_ADR_CAM_EN, GMX1_RX000_ADR_CTL, GMX1_RX001_ADR_CTL, GMX1_RX002_ADR_CTL, GMX1_RX003_ADR_CTL, GMX1_RX000_DECISION, GMX1_RX001_DECISION
REG_WINDOW = GMX1_RX002_DECISION GMX1_RX002_DECISION, GMX1_RX003_DECISION, GMX1_RX000_FRM_CHK, GMX1_RX001_FRM_CHK, GMX1_RX002_FRM_CHK, GMX1_RX003_FRM_CHK, GMX1_RX000_FRM_CTL, GMX1_RX001_FRM_CTL, GMX1_RX002_FRM_CTL, GMX1_RX003_FRM_CTL, GMX1_RX000_IFG, GMX1_RX001_IFG, GMX1_RX002_IFG, GMX1_RX003_IFG, GMX1_RX000_INT_EN, GMX1_RX001_INT_EN, GMX1_RX002_INT_EN, GMX1_RX003_INT_EN, GMX1_RX000_INT_REG, GMX1_RX001_INT_REG, GMX1_RX002_INT_REG, GMX1_RX003_INT_REG, GMX1_RX000_JABBER, GMX1_RX001_JABBER, GMX1_RX002_JABBER, GMX1_RX003_JABBER, GMX1_RX000_PAUSE_DROP_TIME, GMX1_RX001_PAUSE_DROP_TIME, GMX1_RX002_PAUSE_DROP_TIME, GMX1_RX003_PAUSE_DROP_TIME, GMX1_RX000_STATS_CTL, GMX1_RX001_STATS_CTL, GMX1_RX002_STATS_CTL, GMX1_RX003_STATS_CTL, GMX1_RX000_STATS_OCTS, GMX1_RX001_STATS_OCTS, GMX1_RX002_STATS_OCTS, GMX1_RX003_STATS_OCTS, GMX1_RX000_STATS_OCTS_CTL, GMX1_RX001_STATS_OCTS_CTL, GMX1_RX002_STATS_OCTS_CTL, GMX1_RX003_STATS_OCTS_CTL, GMX1_RX000_STATS_OCTS_DMAC, GMX1_RX001_STATS_OCTS_DMAC, GMX1_RX002_STATS_OCTS_DMAC, GMX1_RX003_STATS_OCTS_DMAC, GMX1_RX000_STATS_OCTS_DRP, GMX1_RX001_STATS_OCTS_DRP, GMX1_RX002_STATS_OCTS_DRP, GMX1_RX003_STATS_OCTS_DRP
REG_WINDOW = GMX1_RX000_STATS_PKTS GMX1_RX000_STATS_PKTS, GMX1_RX001_STATS_PKTS, GMX1_RX002_STATS_PKTS, GMX1_RX003_STATS_PKTS, GMX1_RX000_STATS_PKTS_BAD, GMX1_RX001_STATS_PKTS_BAD, GMX1_RX002_STATS_PKTS_BAD, GMX1_RX003_STATS_PKTS_BAD, GMX1_RX000_STATS_PKTS_CTL, GMX1_RX001_STATS_PKTS_CTL, GMX1_RX002_STATS_PKTS_CTL, GMX1_RX003_STATS_PKTS_CTL, GMX1_RX000_STATS_PKTS_DMAC, GMX1_RX001_STATS_PKTS_DMAC, GMX1_RX002_STATS_PKTS_DMAC, GMX1_RX003_STATS_PKTS_DMAC, GMX1_RX000_STATS_PKTS_DRP, GMX1_RX001_STATS_PKTS_DRP, GMX1_RX002_STATS_PKTS_DRP, GMX1_RX003_STATS_PKTS_DRP, GMX1_RX000_UDD_SKP, GMX1_RX001_UDD_SKP, GMX1_RX002_UDD_SKP, GMX1_RX003_UDD_SKP, GMX1_RX_BP_DROP000, GMX1_RX_BP_DROP001, GMX1_RX_BP_DROP002, GMX1_RX_BP_DROP003, GMX1_RX_BP_OFF000, GMX1_RX_BP_OFF001, GMX1_RX_BP_OFF002, GMX1_RX_BP_OFF003, GMX1_RX_BP_ON000, GMX1_RX_BP_ON001, GMX1_RX_BP_ON002, GMX1_RX_BP_ON003, GMX1_RX_HG2_STATUS, GMX1_RX_PRT_INFO, GMX1_RX_PRTS, GMX1_RX_XAUI_BAD_COL, GMX1_RX_XAUI_CTL, GMX1_RXAUI_CTL, GMX1_SMAC000, GMX1_SMAC001, GMX1_SMAC002, GMX1_SMAC003, GMX1_STAT_BP, GMX1_TB_REG, GMX1_TX000_APPEND, GMX1_TX001_APPEND
REG_WINDOW = GMX1_TX002_APPEND GMX1_TX002_APPEND, GMX1_TX003_APPEND, GMX1_TX000_BCK_CRDT, GMX1_TX001_BCK_CRDT, GMX1_TX002_BCK_CRDT, GMX1_TX003_BCK_CRDT, GMX1_TX000_BURST, GMX1_TX001_BURST, GMX1_TX002_BURST, GMX1_TX003_BURST, GMX1_TX000_CBFC_XOFF, GMX1_TX000_CBFC_XON, GMX1_TX000_CTL, GMX1_TX001_CTL, GMX1_TX002_CTL, GMX1_TX003_CTL, GMX1_TX000_JAM_MODE, GMX1_TX001_JAM_MODE, GMX1_TX002_JAM_MODE, GMX1_TX003_JAM_MODE, GMX1_TX000_MIN_PKT, GMX1_TX001_MIN_PKT, GMX1_TX002_MIN_PKT, GMX1_TX003_MIN_PKT, GMX1_TX000_PAUSE_PKT_INTERVAL, GMX1_TX001_PAUSE_PKT_INTERVAL, GMX1_TX002_PAUSE_PKT_INTERVAL, GMX1_TX003_PAUSE_PKT_INTERVAL, GMX1_TX000_PAUSE_PKT_TIME, GMX1_TX001_PAUSE_PKT_TIME, GMX1_TX002_PAUSE_PKT_TIME, GMX1_TX003_PAUSE_PKT_TIME, GMX1_TX000_PAUSE_TOGO, GMX1_TX001_PAUSE_TOGO, GMX1_TX002_PAUSE_TOGO, GMX1_TX003_PAUSE_TOGO, GMX1_TX000_PAUSE_ZERO, GMX1_TX001_PAUSE_ZERO, GMX1_TX002_PAUSE_ZERO, GMX1_TX003_PAUSE_ZERO, GMX1_TX000_SGMII_CTL, GMX1_TX001_SGMII_CTL, GMX1_TX002_SGMII_CTL, GMX1_TX003_SGMII_CTL, GMX1_TX000_SLOT, GMX1_TX001_SLOT, GMX1_TX002_SLOT, GMX1_TX003_SLOT, GMX1_TX000_SOFT_PAUSE, GMX1_TX001_SOFT_PAUSE
REG_WINDOW = GMX1_TX002_SOFT_PAUSE GMX1_TX002_SOFT_PAUSE, GMX1_TX003_SOFT_PAUSE, GMX1_TX000_STAT0, GMX1_TX001_STAT0, GMX1_TX002_STAT0, GMX1_TX003_STAT0, GMX1_TX000_STAT1, GMX1_TX001_STAT1, GMX1_TX002_STAT1, GMX1_TX003_STAT1, GMX1_TX000_STAT2, GMX1_TX001_STAT2, GMX1_TX002_STAT2, GMX1_TX003_STAT2, GMX1_TX000_STAT3, GMX1_TX001_STAT3, GMX1_TX002_STAT3, GMX1_TX003_STAT3, GMX1_TX000_STAT4, GMX1_TX001_STAT4, GMX1_TX002_STAT4, GMX1_TX003_STAT4, GMX1_TX000_STAT5, GMX1_TX001_STAT5, GMX1_TX002_STAT5, GMX1_TX003_STAT5, GMX1_TX000_STAT6, GMX1_TX001_STAT6, GMX1_TX002_STAT6, GMX1_TX003_STAT6, GMX1_TX000_STAT7, GMX1_TX001_STAT7, GMX1_TX002_STAT7, GMX1_TX003_STAT7, GMX1_TX000_STAT8, GMX1_TX001_STAT8, GMX1_TX002_STAT8, GMX1_TX003_STAT8, GMX1_TX000_STAT9, GMX1_TX001_STAT9, GMX1_TX002_STAT9, GMX1_TX003_STAT9, GMX1_TX000_STATS_CTL, GMX1_TX001_STATS_CTL, GMX1_TX002_STATS_CTL, GMX1_TX003_STATS_CTL, GMX1_TX000_THRESH, GMX1_TX001_THRESH, GMX1_TX002_THRESH, GMX1_TX003_THRESH
REG_WINDOW = GMX1_TX_BP GMX1_TX_BP, GMX1_TX_COL_ATTEMPT, GMX1_TX_CORRUPT, GMX1_TX_HG2_REG1, GMX1_TX_HG2_REG2, GMX1_TX_IFG, GMX1_TX_INT_EN, GMX1_TX_INT_REG, GMX1_TX_JAM, GMX1_TX_LFSR, GMX1_TX_OVR_BP, GMX1_TX_PAUSE_PKT_DMAC, GMX1_TX_PAUSE_PKT_TYPE, GMX1_TX_PRTS, GMX1_TX_XAUI_CTL, GMX1_WOL_CTL, GMX1_XAUI_EXT_LOOPBACK
REG_WINDOW = GPIO GPIO_BIT_CFG0, GPIO_BIT_CFG1, GPIO_BIT_CFG2, GPIO_BIT_CFG3, GPIO_BIT_CFG4, GPIO_BIT_CFG5, GPIO_BIT_CFG6, GPIO_BIT_CFG7, GPIO_BIT_CFG8, GPIO_BIT_CFG9, GPIO_BIT_CFG10, GPIO_BIT_CFG11, GPIO_BIT_CFG12, GPIO_BIT_CFG13, GPIO_BIT_CFG14, GPIO_BIT_CFG15, GPIO_CLK_GEN0, GPIO_CLK_GEN1, GPIO_CLK_GEN2, GPIO_CLK_GEN3, GPIO_CLK_SYNCE0, GPIO_CLK_SYNCE1, GPIO_INT_CLR, GPIO_MULTI_CAST, GPIO_OCLA_EXTEN_TRIG, GPIO_RX_DAT, GPIO_SATA_CTL, GPIO_TX_CLR, GPIO_TX_SET, GPIO_USBH_CTL, GPIO_XBIT_CFG16, GPIO_XBIT_CFG17, GPIO_XBIT_CFG18, GPIO_XBIT_CFG19
REG_WINDOW = GSER0_DLM000_LOOPBK_EN GSER0_DLM000_LOOPBK_EN, GSER0_DLM000_LOS_BIAS, GSER0_DLM001_LOS_BIAS, GSER0_DLM002_LOS_BIAS, GSER0_DLM000_LOS_LEVEL, GSER0_DLM001_LOS_LEVEL, GSER0_DLM002_LOS_LEVEL, GSER0_DLM000_MISC_STATUS, GSER0_DLM000_MPLL_EN, GSER0_DLM000_MPLL_HALF_RATE, GSER0_DLM001_MPLL_HALF_RATE, GSER0_DLM002_MPLL_HALF_RATE, GSER0_DLM000_MPLL_MULTIPLIER, GSER0_DLM001_MPLL_MULTIPLIER, GSER0_DLM002_MPLL_MULTIPLIER, GSER0_DLM000_MPLL_STATUS, GSER0_DLM001_MPLL_STATUS, GSER0_DLM002_MPLL_STATUS, GSER0_DLM000_PHY_RESET, GSER0_DLM001_PHY_RESET, GSER0_DLM002_PHY_RESET, GSER0_DLM000_REF_CLKDIV2, GSER0_DLM001_REF_CLKDIV2, GSER0_DLM002_REF_CLKDIV2, GSER0_DLM000_REF_SSP_EN, GSER0_DLM000_REF_USE_PAD, GSER0_DLM001_REF_USE_PAD, GSER0_DLM002_REF_USE_PAD, GSER0_DLM000_REFCLK_SEL, GSER0_DLM001_REFCLK_SEL, GSER0_DLM002_REFCLK_SEL, GSER0_DLM000_RX_DATA_EN, GSER0_DLM000_RX_EQ, GSER0_DLM001_RX_EQ, GSER0_DLM002_RX_EQ, GSER0_DLM000_RX_LOS_EN, GSER0_DLM000_RX_PLL_EN, GSER0_DLM000_RX_RATE, GSER0_DLM000_RX_RESET, GSER0_DLM000_RX_STATUS, GSER0_DLM001_RX_STATUS, GSER0_DLM002_RX_STATUS, GSER0_DLM000_RX_TERM_EN, GSER0_DLM000_TEST_BYPASS, GSER0_DLM001_TEST_BYPASS, GSER0_DLM002_TEST_BYPASS, GSER0_DLM000_TEST_POWERDOWN, GSER0_DLM001_TEST_POWERDOWN, GSER0_DLM002_TEST_POWERDOWN, GSER0_DLM000_TX_AMPLITUDE
REG_WINDOW = GSER0_DLM000_TX_CM_EN GSER0_DLM000_TX_CM_EN, GSER0_DLM000_TX_DATA_EN, GSER0_DLM000_TX_EN, GSER0_DLM000_TX_PREEMPH, GSER0_DLM000_TX_RATE, GSER0_DLM000_TX_RESET, GSER0_DLM000_TX_STATUS, GSER0_DLM001_TX_STATUS, GSER0_DLM002_TX_STATUS, GSER0_DLM000_TX_TERM_OFFSET, GSER0_DLM001_TX_TERM_OFFSET, GSER0_DLM002_TX_TERM_OFFSET, GSER0_PCIE_PCS_CLK_REQ, GSER0_PCIE_PIPE000_TXDEEMPH, GSER0_PCIE_PIPE001_TXDEEMPH, GSER0_PCIE_PIPE002_TXDEEMPH, GSER0_PCIE_PIPE003_TXDEEMPH, GSER0_PCIE_PIPE_COM_CLK, GSER0_PCIE_PIPE_CRST, GSER0_PCIE_PIPE_PORT_LOOPBK, GSER0_PCIE_PIPE_PORT_SEL, GSER0_PCIE_PIPE_RST, GSER0_PCIE_PIPE_RST_STS, GSER0_PCIE_PIPE_STATUS, GSER0_PCIE_TX_DEEMPH_GEN1, GSER0_PCIE_TX_DEEMPH_GEN2_3P5DB, GSER0_PCIE_TX_DEEMPH_GEN2_6DB, GSER0_PCIE_TX_SWING_FULL, GSER0_PCIE_TX_SWING_LOW, GSER0_PCIE_TX_VBOOST_LVL, GSER0_PHY000_IDCODE_HI, GSER0_PHY001_IDCODE_HI, GSER0_PHY002_IDCODE_HI, GSER0_PHY000_IDCODE_LO, GSER0_PHY001_IDCODE_LO, GSER0_PHY002_IDCODE_LO, GSER0_PHY000_LANE0_LOOPBACK, GSER0_PHY001_LANE0_LOOPBACK, GSER0_PHY002_LANE0_LOOPBACK, GSER0_PHY000_LANE0_RX_LBERT_CTL, GSER0_PHY001_LANE0_RX_LBERT_CTL, GSER0_PHY002_LANE0_RX_LBERT_CTL, GSER0_PHY000_LANE0_RX_LBERT_ERR, GSER0_PHY001_LANE0_RX_LBERT_ERR, GSER0_PHY002_LANE0_RX_LBERT_ERR, GSER0_PHY000_LANE0_RX_OVRD_IN_LO, GSER0_PHY001_LANE0_RX_OVRD_IN_LO, GSER0_PHY002_LANE0_RX_OVRD_IN_LO, GSER0_PHY000_LANE0_TX_LBERT_CTL, GSER0_PHY001_LANE0_TX_LBERT_CTL
REG_WINDOW = GSER0_PHY002_LANE0_TX_LBERT_CTL GSER0_PHY002_LANE0_TX_LBERT_CTL, GSER0_PHY000_LANE0_TX_OVRD_IN_HI, GSER0_PHY001_LANE0_TX_OVRD_IN_HI, GSER0_PHY002_LANE0_TX_OVRD_IN_HI, GSER0_PHY000_LANE0_TX_OVRD_IN_LO, GSER0_PHY001_LANE0_TX_OVRD_IN_LO, GSER0_PHY002_LANE0_TX_OVRD_IN_LO, GSER0_PHY000_LANE0_TXDEBUG, GSER0_PHY001_LANE0_TXDEBUG, GSER0_PHY002_LANE0_TXDEBUG, GSER0_PHY000_LANE1_LOOPBACK, GSER0_PHY001_LANE1_LOOPBACK, GSER0_PHY002_LANE1_LOOPBACK, GSER0_PHY000_LANE1_RX_LBERT_CTL, GSER0_PHY001_LANE1_RX_LBERT_CTL, GSER0_PHY002_LANE1_RX_LBERT_CTL, GSER0_PHY000_LANE1_RX_LBERT_ERR, GSER0_PHY001_LANE1_RX_LBERT_ERR, GSER0_PHY002_LANE1_RX_LBERT_ERR, GSER0_PHY000_LANE1_RX_OVRD_IN_LO, GSER0_PHY001_LANE1_RX_OVRD_IN_LO, GSER0_PHY002_LANE1_RX_OVRD_IN_LO, GSER0_PHY000_LANE1_TX_LBERT_CTL, GSER0_PHY001_LANE1_TX_LBERT_CTL, GSER0_PHY002_LANE1_TX_LBERT_CTL, GSER0_PHY000_LANE1_TX_OVRD_IN_HI, GSER0_PHY001_LANE1_TX_OVRD_IN_HI, GSER0_PHY002_LANE1_TX_OVRD_IN_HI, GSER0_PHY000_LANE1_TX_OVRD_IN_LO, GSER0_PHY001_LANE1_TX_OVRD_IN_LO, GSER0_PHY002_LANE1_TX_OVRD_IN_LO, GSER0_PHY000_LANE1_TXDEBUG, GSER0_PHY001_LANE1_TXDEBUG, GSER0_PHY002_LANE1_TXDEBUG, GSER0_PHY000_OVRD_IN_LO, GSER0_PHY001_OVRD_IN_LO, GSER0_PHY002_OVRD_IN_LO, GSER0_SATA_CFG, GSER0_SATA_LANE_RST, GSER0_SATA_P0_TX_AMP_GEN001, GSER0_SATA_P0_TX_AMP_GEN002, GSER0_SATA_P0_TX_AMP_GEN003, GSER0_SATA_P0_TX_PREEMPH_GEN001, GSER0_SATA_P0_TX_PREEMPH_GEN002, GSER0_SATA_P0_TX_PREEMPH_GEN003, GSER0_SATA_P1_TX_AMP_GEN001, GSER0_SATA_P1_TX_AMP_GEN002, GSER0_SATA_P1_TX_AMP_GEN003, GSER0_SATA_P1_TX_PREEMPH_GEN001, GSER0_SATA_P1_TX_PREEMPH_GEN002
REG_WINDOW = GSER0_SATA_P1_TX_PREEMPH_GEN003 GSER0_SATA_P1_TX_PREEMPH_GEN003, GSER0_SATA_REF_SSP_EN, GSER0_SATA_RX_INVERT, GSER0_SATA_SSC_CLK_SEL, GSER0_SATA_SSC_EN, GSER0_SATA_SSC_RANGE, GSER0_SATA_STATUS, GSER0_SATA_TX_INVERT
REG_WINDOW = IOB IOB_BIST_STATUS, IOB_CHIP_CUR_PWR, IOB_CHIP_GLB_PWR_THROTTLE, IOB_CHIP_PWR_OUT, IOB_CTL_STATUS, IOB_DWB_PRI_CNT, IOB_FAU_TIMEOUT, IOB_I2C_PRI_CNT, IOB_INB_CONTROL_MATCH, IOB_INB_CONTROL_MATCH_ENB, IOB_INB_DATA_MATCH, IOB_INB_DATA_MATCH_ENB, IOB_INT_ENB, IOB_INT_SUM, IOB_N2C_L2C_PRI_CNT, IOB_N2C_RSP_PRI_CNT, IOB_OUTB_COM_PRI_CNT, IOB_OUTB_CONTROL_MATCH, IOB_OUTB_CONTROL_MATCH_ENB, IOB_OUTB_DATA_MATCH, IOB_OUTB_DATA_MATCH_ENB, IOB_OUTB_FPA_PRI_CNT, IOB_OUTB_REQ_PRI_CNT, IOB_P2C_REQ_PRI_CNT, IOB_PKT_ERR, IOB_PP_BIST_STATUS, IOB_TO_CMB_CREDITS
REG_WINDOW = IPD_1ST_MBUFF_SKIP IPD_1ST_MBUFF_SKIP, IPD_1ST_NEXT_PTR_BACK, IPD_2ND_NEXT_PTR_BACK, IPD_BIST_STATUS, IPD_BP_PRT_RED_END, IPD_CLK_COUNT, IPD_CTL_STATUS, IPD_INT_ENB, IPD_INT_SUM, IPD_NOT_1ST_MBUFF_SKIP, IPD_PACKET_MBUFF_SIZE, IPD_PKT_PTR_VALID, IPD_PORT0_BP_PAGE_CNT, IPD_PORT1_BP_PAGE_CNT, IPD_PORT2_BP_PAGE_CNT, IPD_PORT3_BP_PAGE_CNT, IPD_PORT16_BP_PAGE_CNT, IPD_PORT17_BP_PAGE_CNT, IPD_PORT18_BP_PAGE_CNT, IPD_PORT19_BP_PAGE_CNT, IPD_PORT24_BP_PAGE_CNT, IPD_PORT32_BP_PAGE_CNT, IPD_PORT33_BP_PAGE_CNT, IPD_PORT34_BP_PAGE_CNT, IPD_PORT35_BP_PAGE_CNT, IPD_PORT36_BP_PAGE_CNT2, IPD_PORT37_BP_PAGE_CNT2, IPD_PORT38_BP_PAGE_CNT2, IPD_PORT39_BP_PAGE_CNT2, IPD_PORT40_BP_PAGE_CNT3, IPD_PORT41_BP_PAGE_CNT3, IPD_PORT42_BP_PAGE_CNT3, IPD_PORT43_BP_PAGE_CNT3, IPD_PORT44_BP_PAGE_CNT3, IPD_PORT45_BP_PAGE_CNT3, IPD_PORT46_BP_PAGE_CNT3, IPD_PORT47_BP_PAGE_CNT3, IPD_PORT_BP_COUNTERS2_PAIR36, IPD_PORT_BP_COUNTERS2_PAIR37, IPD_PORT_BP_COUNTERS2_PAIR38, IPD_PORT_BP_COUNTERS2_PAIR39, IPD_PORT_BP_COUNTERS3_PAIR40, IPD_PORT_BP_COUNTERS3_PAIR41, IPD_PORT_BP_COUNTERS3_PAIR42, IPD_PORT_BP_COUNTERS3_PAIR43, IPD_PORT_BP_COUNTERS4_PAIR44, IPD_PORT_BP_COUNTERS4_PAIR45, IPD_PORT_BP_COUNTERS4_PAIR46, IPD_PORT_BP_COUNTERS4_PAIR47, IPD_PORT_BP_COUNTERS_PAIR0
REG_WINDOW = IPD_PORT_BP_COUNTERS_PAIR1 IPD_PORT_BP_COUNTERS_PAIR1, IPD_PORT_BP_COUNTERS_PAIR2, IPD_PORT_BP_COUNTERS_PAIR3, IPD_PORT_BP_COUNTERS_PAIR16, IPD_PORT_BP_COUNTERS_PAIR17, IPD_PORT_BP_COUNTERS_PAIR18, IPD_PORT_BP_COUNTERS_PAIR19, IPD_PORT_BP_COUNTERS_PAIR24, IPD_PORT_BP_COUNTERS_PAIR32, IPD_PORT_BP_COUNTERS_PAIR33, IPD_PORT_BP_COUNTERS_PAIR34, IPD_PORT_BP_COUNTERS_PAIR35, IPD_PORT_QOS_0_CNT, IPD_PORT_QOS_1_CNT, IPD_PORT_QOS_2_CNT, IPD_PORT_QOS_3_CNT, IPD_PORT_QOS_4_CNT, IPD_PORT_QOS_5_CNT, IPD_PORT_QOS_6_CNT, IPD_PORT_QOS_7_CNT, IPD_PORT_QOS_8_CNT, IPD_PORT_QOS_9_CNT, IPD_PORT_QOS_10_CNT, IPD_PORT_QOS_11_CNT, IPD_PORT_QOS_12_CNT, IPD_PORT_QOS_13_CNT, IPD_PORT_QOS_14_CNT, IPD_PORT_QOS_15_CNT, IPD_PORT_QOS_16_CNT, IPD_PORT_QOS_17_CNT, IPD_PORT_QOS_18_CNT, IPD_PORT_QOS_19_CNT, IPD_PORT_QOS_20_CNT, IPD_PORT_QOS_21_CNT, IPD_PORT_QOS_22_CNT, IPD_PORT_QOS_23_CNT, IPD_PORT_QOS_24_CNT, IPD_PORT_QOS_25_CNT, IPD_PORT_QOS_26_CNT, IPD_PORT_QOS_27_CNT, IPD_PORT_QOS_28_CNT, IPD_PORT_QOS_29_CNT, IPD_PORT_QOS_30_CNT, IPD_PORT_QOS_31_CNT, IPD_PORT_QOS_128_CNT, IPD_PORT_QOS_129_CNT, IPD_PORT_QOS_130_CNT, IPD_PORT_QOS_131_CNT, IPD_PORT_QOS_132_CNT, IPD_PORT_QOS_133_CNT
REG_WINDOW = IPD_PORT_QOS_134_CNT IPD_PORT_QOS_134_CNT, IPD_PORT_QOS_135_CNT, IPD_PORT_QOS_136_CNT, IPD_PORT_QOS_137_CNT, IPD_PORT_QOS_138_CNT, IPD_PORT_QOS_139_CNT, IPD_PORT_QOS_140_CNT, IPD_PORT_QOS_141_CNT, IPD_PORT_QOS_142_CNT, IPD_PORT_QOS_143_CNT, IPD_PORT_QOS_144_CNT, IPD_PORT_QOS_145_CNT, IPD_PORT_QOS_146_CNT, IPD_PORT_QOS_147_CNT, IPD_PORT_QOS_148_CNT, IPD_PORT_QOS_149_CNT, IPD_PORT_QOS_150_CNT, IPD_PORT_QOS_151_CNT, IPD_PORT_QOS_152_CNT, IPD_PORT_QOS_153_CNT, IPD_PORT_QOS_154_CNT, IPD_PORT_QOS_155_CNT, IPD_PORT_QOS_156_CNT, IPD_PORT_QOS_157_CNT, IPD_PORT_QOS_158_CNT, IPD_PORT_QOS_159_CNT, IPD_PORT_QOS_192_CNT, IPD_PORT_QOS_193_CNT, IPD_PORT_QOS_194_CNT, IPD_PORT_QOS_195_CNT, IPD_PORT_QOS_196_CNT, IPD_PORT_QOS_197_CNT, IPD_PORT_QOS_198_CNT, IPD_PORT_QOS_199_CNT, IPD_PORT_QOS_256_CNT, IPD_PORT_QOS_257_CNT, IPD_PORT_QOS_258_CNT, IPD_PORT_QOS_259_CNT, IPD_PORT_QOS_260_CNT, IPD_PORT_QOS_261_CNT, IPD_PORT_QOS_262_CNT, IPD_PORT_QOS_263_CNT, IPD_PORT_QOS_264_CNT, IPD_PORT_QOS_265_CNT, IPD_PORT_QOS_266_CNT, IPD_PORT_QOS_267_CNT, IPD_PORT_QOS_268_CNT, IPD_PORT_QOS_269_CNT, IPD_PORT_QOS_270_CNT, IPD_PORT_QOS_271_CNT
REG_WINDOW = IPD_PORT_QOS_272_CNT IPD_PORT_QOS_272_CNT, IPD_PORT_QOS_273_CNT, IPD_PORT_QOS_274_CNT, IPD_PORT_QOS_275_CNT, IPD_PORT_QOS_276_CNT, IPD_PORT_QOS_277_CNT, IPD_PORT_QOS_278_CNT, IPD_PORT_QOS_279_CNT, IPD_PORT_QOS_280_CNT, IPD_PORT_QOS_281_CNT, IPD_PORT_QOS_282_CNT, IPD_PORT_QOS_283_CNT, IPD_PORT_QOS_284_CNT, IPD_PORT_QOS_285_CNT, IPD_PORT_QOS_286_CNT, IPD_PORT_QOS_287_CNT, IPD_PORT_QOS_288_CNT, IPD_PORT_QOS_289_CNT, IPD_PORT_QOS_290_CNT, IPD_PORT_QOS_291_CNT, IPD_PORT_QOS_292_CNT, IPD_PORT_QOS_293_CNT, IPD_PORT_QOS_294_CNT, IPD_PORT_QOS_295_CNT, IPD_PORT_QOS_296_CNT, IPD_PORT_QOS_297_CNT, IPD_PORT_QOS_298_CNT, IPD_PORT_QOS_299_CNT, IPD_PORT_QOS_300_CNT, IPD_PORT_QOS_301_CNT, IPD_PORT_QOS_302_CNT, IPD_PORT_QOS_303_CNT, IPD_PORT_QOS_304_CNT, IPD_PORT_QOS_305_CNT, IPD_PORT_QOS_306_CNT, IPD_PORT_QOS_307_CNT, IPD_PORT_QOS_308_CNT, IPD_PORT_QOS_309_CNT, IPD_PORT_QOS_310_CNT, IPD_PORT_QOS_311_CNT, IPD_PORT_QOS_312_CNT, IPD_PORT_QOS_313_CNT, IPD_PORT_QOS_314_CNT, IPD_PORT_QOS_315_CNT, IPD_PORT_QOS_316_CNT, IPD_PORT_QOS_317_CNT, IPD_PORT_QOS_318_CNT, IPD_PORT_QOS_319_CNT, IPD_PORT_QOS_320_CNT, IPD_PORT_QOS_321_CNT
REG_WINDOW = IPD_PORT_QOS_322_CNT IPD_PORT_QOS_322_CNT, IPD_PORT_QOS_323_CNT, IPD_PORT_QOS_324_CNT, IPD_PORT_QOS_325_CNT, IPD_PORT_QOS_326_CNT, IPD_PORT_QOS_327_CNT, IPD_PORT_QOS_328_CNT, IPD_PORT_QOS_329_CNT, IPD_PORT_QOS_330_CNT, IPD_PORT_QOS_331_CNT, IPD_PORT_QOS_332_CNT, IPD_PORT_QOS_333_CNT, IPD_PORT_QOS_334_CNT, IPD_PORT_QOS_335_CNT, IPD_PORT_QOS_336_CNT, IPD_PORT_QOS_337_CNT, IPD_PORT_QOS_338_CNT, IPD_PORT_QOS_339_CNT, IPD_PORT_QOS_340_CNT, IPD_PORT_QOS_341_CNT, IPD_PORT_QOS_342_CNT, IPD_PORT_QOS_343_CNT, IPD_PORT_QOS_344_CNT, IPD_PORT_QOS_345_CNT, IPD_PORT_QOS_346_CNT, IPD_PORT_QOS_347_CNT, IPD_PORT_QOS_348_CNT, IPD_PORT_QOS_349_CNT, IPD_PORT_QOS_350_CNT, IPD_PORT_QOS_351_CNT, IPD_PORT_QOS_352_CNT, IPD_PORT_QOS_353_CNT, IPD_PORT_QOS_354_CNT, IPD_PORT_QOS_355_CNT, IPD_PORT_QOS_356_CNT, IPD_PORT_QOS_357_CNT, IPD_PORT_QOS_358_CNT, IPD_PORT_QOS_359_CNT, IPD_PORT_QOS_360_CNT, IPD_PORT_QOS_361_CNT, IPD_PORT_QOS_362_CNT, IPD_PORT_QOS_363_CNT, IPD_PORT_QOS_364_CNT, IPD_PORT_QOS_365_CNT, IPD_PORT_QOS_366_CNT, IPD_PORT_QOS_367_CNT, IPD_PORT_QOS_368_CNT, IPD_PORT_QOS_369_CNT, IPD_PORT_QOS_370_CNT, IPD_PORT_QOS_371_CNT
REG_WINDOW = IPD_PORT_QOS_372_CNT IPD_PORT_QOS_372_CNT, IPD_PORT_QOS_373_CNT, IPD_PORT_QOS_374_CNT, IPD_PORT_QOS_375_CNT, IPD_PORT_QOS_376_CNT, IPD_PORT_QOS_377_CNT, IPD_PORT_QOS_378_CNT, IPD_PORT_QOS_379_CNT, IPD_PORT_QOS_380_CNT, IPD_PORT_QOS_381_CNT, IPD_PORT_QOS_382_CNT, IPD_PORT_QOS_383_CNT, IPD_PORT_QOS_INT0, IPD_PORT_QOS_INT2, IPD_PORT_QOS_INT3, IPD_PORT_QOS_INT4, IPD_PORT_QOS_INT_ENB0, IPD_PORT_QOS_INT_ENB2, IPD_PORT_QOS_INT_ENB3, IPD_PORT_QOS_INT_ENB4, IPD_PRC_HOLD_PTR_FIFO_CTL, IPD_PRC_PORT_PTR_FIFO_CTL, IPD_PTR_COUNT, IPD_PWP_PTR_FIFO_CTL, IPD_QOS0_RED_MARKS, IPD_QOS1_RED_MARKS, IPD_QOS2_RED_MARKS, IPD_QOS3_RED_MARKS, IPD_QOS4_RED_MARKS, IPD_QOS5_RED_MARKS, IPD_QOS6_RED_MARKS, IPD_QOS7_RED_MARKS, IPD_QUE0_FREE_PAGE_CNT, IPD_RED_PORT_ENABLE, IPD_RED_PORT_ENABLE2, IPD_RED_QUE0_PARAM, IPD_RED_QUE1_PARAM, IPD_RED_QUE2_PARAM, IPD_RED_QUE3_PARAM, IPD_RED_QUE4_PARAM, IPD_RED_QUE5_PARAM, IPD_RED_QUE6_PARAM, IPD_RED_QUE7_PARAM, IPD_SUB_PORT_BP_PAGE_CNT, IPD_SUB_PORT_FCS, IPD_SUB_PORT_QOS_CNT, IPD_WQE_FPA_QUEUE, IPD_WQE_PTR_VALID
REG_WINDOW = KEY KEY_BIST_REG, KEY_CTL_STATUS, KEY_INT_ENB, KEY_INT_SUM
REG_WINDOW = L2C L2C_BIG_CTL, L2C_CBC0_BIST_STATUS, L2C_CBC0_INT, L2C_CBC0_IOCERR, L2C_CBC0_RSDERR, L2C_COP0_ADR, L2C_COP0_DAT, L2C_CTL, L2C_ECC_CTL, L2C_INV0_PFC, L2C_IOC0_PFC, L2C_IOR0_PFC, L2C_MCI0_BIST_STATUS, L2C_MCI0_ERR, L2C_MCI0_INT, L2C_QOS_IOB0, L2C_QOS_PP0, L2C_QOS_PP1, L2C_QOS_PP2, L2C_QOS_PP3, L2C_QOS_WGT, L2C_RSC0_PFC, L2C_RSD0_PFC, L2C_TAD0_DLL, L2C_TAD0_ERR, L2C_TAD0_INT, L2C_TAD0_PFC000, L2C_TAD0_PFC001, L2C_TAD0_PFC002, L2C_TAD0_PFC003, L2C_TAD0_PRF, L2C_TAD0_TAG, L2C_TAD_CTL, L2C_TBF0_BIST_STATUS, L2C_TDT0_BIST_STATUS, L2C_TQD0_ERR, L2C_TTG0_BIST_STATUS, L2C_TTG0_ERR, L2C_WPAR_IOB0, L2C_WPAR_PP0, L2C_WPAR_PP1, L2C_WPAR_PP2, L2C_WPAR_PP3, L2C_XMC0_PFC, L2C_XMC_CMD, L2C_XMD0_PFC
REG_WINDOW = LMC0_BIST_CTL LMC0_BIST_CTL, LMC0_CHAR_CTL, LMC0_CHAR_DQ_ERR_COUNT, LMC0_CHAR_MASK0, LMC0_CHAR_MASK1, LMC0_CHAR_MASK2, LMC0_CHAR_MASK3, LMC0_CHAR_MASK4, LMC0_COMP_CTL2, LMC0_CONFIG, LMC0_CONTROL, LMC0_DCLK_CNT, LMC0_DDR4_DIMM_CTL, LMC0_DDR_PLL_CTL, LMC0_DIMM000_DDR4_PARAMS0, LMC0_DIMM001_DDR4_PARAMS0, LMC0_DIMM000_DDR4_PARAMS1, LMC0_DIMM001_DDR4_PARAMS1, LMC0_DIMM000_PARAMS, LMC0_DIMM001_PARAMS, LMC0_DIMM_CTL, LMC0_DLL_CTL2, LMC0_DLL_CTL3, LMC0_DUAL_MEMCFG, LMC0_ECC_SYND, LMC0_EXT_CONFIG, LMC0_FADR, LMC0_IFB_CNT, LMC0_INT, LMC0_INT_EN, LMC0_MODEREG_PARAMS0, LMC0_MODEREG_PARAMS1, LMC0_MODEREG_PARAMS2, LMC0_MODEREG_PARAMS3, LMC0_MPR_DATA0, LMC0_MPR_DATA1, LMC0_MPR_DATA2, LMC0_MR_MPR_CTL, LMC0_NXM, LMC0_NXM_FADR, LMC0_OPS_CNT, LMC0_PHY_CTL, LMC0_RESET_CTL, LMC0_RLEVEL_CTL, LMC0_RLEVEL_DBG, LMC0_RLEVEL_RANK000, LMC0_RLEVEL_RANK001, LMC0_RLEVEL_RANK002, LMC0_RLEVEL_RANK003, LMC0_RODT_MASK
REG_WINDOW = LMC0_SCRAMBLE_CFG0 LMC0_SCRAMBLE_CFG0, LMC0_SCRAMBLE_CFG1, LMC0_SCRAMBLED_FADR, LMC0_SEQ_CTL, LMC0_SLOT_CTL0, LMC0_SLOT_CTL1, LMC0_SLOT_CTL2, LMC0_TIMING_PARAMS0, LMC0_TIMING_PARAMS1, LMC0_TIMING_PARAMS2, LMC0_WLEVEL_CTL, LMC0_WLEVEL_DBG, LMC0_WLEVEL_RANK000, LMC0_WLEVEL_RANK001, LMC0_WLEVEL_RANK002, LMC0_WLEVEL_RANK003, LMC0_WODT_MASK
REG_WINDOW = MIO_BOOT_BIST_STAT MIO_BOOT_BIST_STAT, MIO_BOOT_COMP, MIO_BOOT_CTL, MIO_BOOT_ERR, MIO_BOOT_INT, MIO_BOOT_LOC_ADR, MIO_BOOT_LOC_CFG0, MIO_BOOT_LOC_CFG1, MIO_BOOT_LOC_DAT, MIO_BOOT_PIN_DEFS, MIO_BOOT_REG_CFG0, MIO_BOOT_REG_CFG1, MIO_BOOT_REG_CFG2, MIO_BOOT_REG_CFG3, MIO_BOOT_REG_CFG4, MIO_BOOT_REG_CFG5, MIO_BOOT_REG_CFG6, MIO_BOOT_REG_CFG7, MIO_BOOT_REG_TIM0, MIO_BOOT_REG_TIM1, MIO_BOOT_REG_TIM2, MIO_BOOT_REG_TIM3, MIO_BOOT_REG_TIM4, MIO_BOOT_REG_TIM5, MIO_BOOT_REG_TIM6, MIO_BOOT_REG_TIM7, MIO_BOOT_THR, MIO_EMM_BUF_DAT, MIO_EMM_BUF_IDX, MIO_EMM_CFG, MIO_EMM_CMD, MIO_EMM_DMA, MIO_EMM_INT, MIO_EMM_INT_EN, MIO_EMM_MODE0, MIO_EMM_MODE1, MIO_EMM_MODE2, MIO_EMM_MODE3, MIO_EMM_RCA, MIO_EMM_RSP_HI, MIO_EMM_RSP_LO, MIO_EMM_RSP_STS, MIO_EMM_SAMPLE, MIO_EMM_STS_MASK, MIO_EMM_SWITCH, MIO_EMM_WDOG, MIO_FUS_BNK_DAT0, MIO_FUS_BNK_DAT1, MIO_FUS_DAT0, MIO_FUS_DAT1
REG_WINDOW = MIO_FUS_DAT2 MIO_FUS_DAT2, MIO_FUS_DAT3, MIO_FUS_DAT4, MIO_FUS_INT, MIO_FUS_PDF, MIO_FUS_PLL, MIO_FUS_PROG, MIO_FUS_PROG_TIMES, MIO_FUS_RCMD, MIO_FUS_READ_TIMES, MIO_FUS_RPR_DAT0, MIO_FUS_RPR_DAT1, MIO_FUS_SOFT_REPAIR, MIO_FUS_TGG, MIO_FUS_WADR, MIO_GPIO_COMP, MIO_NDF_DMA_CFG, MIO_NDF_DMA_INT, MIO_NDF_DMA_INT_EN, MIO_PTP_CKOUT_HI_INCR, MIO_PTP_CKOUT_LO_INCR, MIO_PTP_CKOUT_THRESH_HI, MIO_PTP_CKOUT_THRESH_LO, MIO_PTP_CLOCK_CFG, MIO_PTP_CLOCK_COMP, MIO_PTP_CLOCK_HI, MIO_PTP_CLOCK_LO, MIO_PTP_EVT_CNT, MIO_PTP_PPS_HI_INCR, MIO_PTP_PPS_LO_INCR, MIO_PTP_PPS_THRESH_HI, MIO_PTP_PPS_THRESH_LO, MIO_PTP_TIMESTAMP, MIO_TWS0_INT, MIO_TWS1_INT, MIO_TWS0_SW_TWSI, MIO_TWS1_SW_TWSI, MIO_TWS0_SW_TWSI_EXT, MIO_TWS1_SW_TWSI_EXT, MIO_TWS0_TWSI_SW, MIO_TWS1_TWSI_SW, MIO_UART0_DLH, MIO_UART1_DLH, MIO_UART0_DLL, MIO_UART1_DLL, MIO_UART0_FAR, MIO_UART1_FAR, MIO_UART0_FCR, MIO_UART1_FCR, MIO_UART0_HTX
REG_WINDOW = MIO_UART1_HTX MIO_UART1_HTX, MIO_UART0_IER, MIO_UART1_IER, MIO_UART0_IIR, MIO_UART1_IIR, MIO_UART0_LCR, MIO_UART1_LCR, MIO_UART0_LSR, MIO_UART1_LSR, MIO_UART0_MCR, MIO_UART1_MCR, MIO_UART0_MSR, MIO_UART1_MSR, MIO_UART0_RBR, MIO_UART1_RBR, MIO_UART0_RFL, MIO_UART1_RFL, MIO_UART0_RFW, MIO_UART1_RFW, MIO_UART0_SBCR, MIO_UART1_SBCR, MIO_UART0_SCR, MIO_UART1_SCR, MIO_UART0_SFE, MIO_UART1_SFE, MIO_UART0_SRR, MIO_UART1_SRR, MIO_UART0_SRT, MIO_UART1_SRT, MIO_UART0_SRTS, MIO_UART1_SRTS, MIO_UART0_STT, MIO_UART1_STT, MIO_UART0_TFL, MIO_UART1_TFL, MIO_UART0_TFR, MIO_UART1_TFR, MIO_UART0_THR, MIO_UART1_THR, MIO_UART0_USR, MIO_UART1_USR
REG_WINDOW = MPI MPI_CFG, MPI_DAT0, MPI_DAT1, MPI_DAT2, MPI_DAT3, MPI_DAT4, MPI_DAT5, MPI_DAT6, MPI_DAT7, MPI_DAT8, MPI_STS, MPI_TX, MPI_WIDE_DAT
REG_WINDOW = NDF NDF_BT_PG_INFO, NDF_CMD, NDF_DRBELL, NDF_ECC_CNT, NDF_INT, NDF_INT_EN, NDF_MISC, NDF_ST_REG
REG_WINDOW = OCLA0_BIST_RESULT OCLA0_BIST_RESULT, OCLA0_CDH000_CTL, OCLA0_CDH001_CTL, OCLA0_CONST, OCLA0_DAT000, OCLA0_DAT001, OCLA0_DAT002, OCLA0_DAT003, OCLA0_DAT004, OCLA0_DAT005, OCLA0_DAT006, OCLA0_DAT007, OCLA0_DAT008, OCLA0_DAT009, OCLA0_DAT010, OCLA0_DAT011, OCLA0_DAT012, OCLA0_DAT013, OCLA0_DAT014, OCLA0_DAT015, OCLA0_DAT016, OCLA0_DAT017, OCLA0_DAT018, OCLA0_DAT019, OCLA0_DAT020, OCLA0_DAT021, OCLA0_DAT022, OCLA0_DAT023, OCLA0_DAT024, OCLA0_DAT025, OCLA0_DAT026, OCLA0_DAT027, OCLA0_DAT028, OCLA0_DAT029, OCLA0_DAT030, OCLA0_DAT031, OCLA0_DAT032, OCLA0_DAT033, OCLA0_DAT034, OCLA0_DAT035, OCLA0_DAT036, OCLA0_DAT037, OCLA0_DAT038, OCLA0_DAT039, OCLA0_DAT040, OCLA0_DAT041, OCLA0_DAT042, OCLA0_DAT043, OCLA0_DAT044, OCLA0_DAT045
REG_WINDOW = OCLA0_DAT046 OCLA0_DAT046, OCLA0_DAT047, OCLA0_DAT048, OCLA0_DAT049, OCLA0_DAT050, OCLA0_DAT051, OCLA0_DAT052, OCLA0_DAT053, OCLA0_DAT054, OCLA0_DAT055, OCLA0_DAT056, OCLA0_DAT057, OCLA0_DAT058, OCLA0_DAT059, OCLA0_DAT060, OCLA0_DAT061, OCLA0_DAT062, OCLA0_DAT063, OCLA0_DAT064, OCLA0_DAT065, OCLA0_DAT066, OCLA0_DAT067, OCLA0_DAT068, OCLA0_DAT069, OCLA0_DAT070, OCLA0_DAT071, OCLA0_DAT072, OCLA0_DAT073, OCLA0_DAT074, OCLA0_DAT075, OCLA0_DAT076, OCLA0_DAT077, OCLA0_DAT078, OCLA0_DAT079, OCLA0_DAT080, OCLA0_DAT081, OCLA0_DAT082, OCLA0_DAT083, OCLA0_DAT084, OCLA0_DAT085, OCLA0_DAT086, OCLA0_DAT087, OCLA0_DAT088, OCLA0_DAT089, OCLA0_DAT090, OCLA0_DAT091, OCLA0_DAT092, OCLA0_DAT093, OCLA0_DAT094, OCLA0_DAT095
REG_WINDOW = OCLA0_DAT096 OCLA0_DAT096, OCLA0_DAT097, OCLA0_DAT098, OCLA0_DAT099, OCLA0_DAT100, OCLA0_DAT101, OCLA0_DAT102, OCLA0_DAT103, OCLA0_DAT104, OCLA0_DAT105, OCLA0_DAT106, OCLA0_DAT107, OCLA0_DAT108, OCLA0_DAT109, OCLA0_DAT110, OCLA0_DAT111, OCLA0_DAT112, OCLA0_DAT113, OCLA0_DAT114, OCLA0_DAT115, OCLA0_DAT116, OCLA0_DAT117, OCLA0_DAT118, OCLA0_DAT119, OCLA0_DAT120, OCLA0_DAT121, OCLA0_DAT122, OCLA0_DAT123, OCLA0_DAT124, OCLA0_DAT125, OCLA0_DAT126, OCLA0_DAT127, OCLA0_DAT128, OCLA0_DAT129, OCLA0_DAT130, OCLA0_DAT131, OCLA0_DAT132, OCLA0_DAT133, OCLA0_DAT134, OCLA0_DAT135, OCLA0_DAT136, OCLA0_DAT137, OCLA0_DAT138, OCLA0_DAT139, OCLA0_DAT140, OCLA0_DAT141, OCLA0_DAT142, OCLA0_DAT143, OCLA0_DAT144, OCLA0_DAT145
REG_WINDOW = OCLA0_DAT146 OCLA0_DAT146, OCLA0_DAT147, OCLA0_DAT148, OCLA0_DAT149, OCLA0_DAT150, OCLA0_DAT151, OCLA0_DAT152, OCLA0_DAT153, OCLA0_DAT154, OCLA0_DAT155, OCLA0_DAT156, OCLA0_DAT157, OCLA0_DAT158, OCLA0_DAT159, OCLA0_DAT160, OCLA0_DAT161, OCLA0_DAT162, OCLA0_DAT163, OCLA0_DAT164, OCLA0_DAT165, OCLA0_DAT166, OCLA0_DAT167, OCLA0_DAT168, OCLA0_DAT169, OCLA0_DAT170, OCLA0_DAT171, OCLA0_DAT172, OCLA0_DAT173, OCLA0_DAT174, OCLA0_DAT175, OCLA0_DAT176, OCLA0_DAT177, OCLA0_DAT178, OCLA0_DAT179, OCLA0_DAT180, OCLA0_DAT181, OCLA0_DAT182, OCLA0_DAT183, OCLA0_DAT184, OCLA0_DAT185, OCLA0_DAT186, OCLA0_DAT187, OCLA0_DAT188, OCLA0_DAT189, OCLA0_DAT190, OCLA0_DAT191, OCLA0_DAT192, OCLA0_DAT193, OCLA0_DAT194, OCLA0_DAT195
REG_WINDOW = OCLA0_DAT196 OCLA0_DAT196, OCLA0_DAT197, OCLA0_DAT198, OCLA0_DAT199, OCLA0_DAT200, OCLA0_DAT201, OCLA0_DAT202, OCLA0_DAT203, OCLA0_DAT204, OCLA0_DAT205, OCLA0_DAT206, OCLA0_DAT207, OCLA0_DAT208, OCLA0_DAT209, OCLA0_DAT210, OCLA0_DAT211, OCLA0_DAT212, OCLA0_DAT213, OCLA0_DAT214, OCLA0_DAT215, OCLA0_DAT216, OCLA0_DAT217, OCLA0_DAT218, OCLA0_DAT219, OCLA0_DAT220, OCLA0_DAT221, OCLA0_DAT222, OCLA0_DAT223, OCLA0_DAT224, OCLA0_DAT225, OCLA0_DAT226, OCLA0_DAT227, OCLA0_DAT228, OCLA0_DAT229, OCLA0_DAT230, OCLA0_DAT231, OCLA0_DAT232, OCLA0_DAT233, OCLA0_DAT234, OCLA0_DAT235, OCLA0_DAT236, OCLA0_DAT237, OCLA0_DAT238, OCLA0_DAT239, OCLA0_DAT240, OCLA0_DAT241, OCLA0_DAT242, OCLA0_DAT243, OCLA0_DAT244, OCLA0_DAT245
REG_WINDOW = OCLA0_DAT246 OCLA0_DAT246, OCLA0_DAT247, OCLA0_DAT248, OCLA0_DAT249, OCLA0_DAT250, OCLA0_DAT251, OCLA0_DAT252, OCLA0_DAT253, OCLA0_DAT254, OCLA0_DAT255, OCLA0_DAT256, OCLA0_DAT257, OCLA0_DAT258, OCLA0_DAT259, OCLA0_DAT260, OCLA0_DAT261, OCLA0_DAT262, OCLA0_DAT263, OCLA0_DAT264, OCLA0_DAT265, OCLA0_DAT266, OCLA0_DAT267, OCLA0_DAT268, OCLA0_DAT269, OCLA0_DAT270, OCLA0_DAT271, OCLA0_DAT272, OCLA0_DAT273, OCLA0_DAT274, OCLA0_DAT275, OCLA0_DAT276, OCLA0_DAT277, OCLA0_DAT278, OCLA0_DAT279, OCLA0_DAT280, OCLA0_DAT281, OCLA0_DAT282, OCLA0_DAT283, OCLA0_DAT284, OCLA0_DAT285, OCLA0_DAT286, OCLA0_DAT287, OCLA0_DAT288, OCLA0_DAT289, OCLA0_DAT290, OCLA0_DAT291, OCLA0_DAT292, OCLA0_DAT293, OCLA0_DAT294, OCLA0_DAT295
REG_WINDOW = OCLA0_DAT296 OCLA0_DAT296, OCLA0_DAT297, OCLA0_DAT298, OCLA0_DAT299, OCLA0_DAT300, OCLA0_DAT301, OCLA0_DAT302, OCLA0_DAT303, OCLA0_DAT304, OCLA0_DAT305, OCLA0_DAT306, OCLA0_DAT307, OCLA0_DAT308, OCLA0_DAT309, OCLA0_DAT310, OCLA0_DAT311, OCLA0_DAT312, OCLA0_DAT313, OCLA0_DAT314, OCLA0_DAT315, OCLA0_DAT316, OCLA0_DAT317, OCLA0_DAT318, OCLA0_DAT319, OCLA0_DAT320, OCLA0_DAT321, OCLA0_DAT322, OCLA0_DAT323, OCLA0_DAT324, OCLA0_DAT325, OCLA0_DAT326, OCLA0_DAT327, OCLA0_DAT328, OCLA0_DAT329, OCLA0_DAT330, OCLA0_DAT331, OCLA0_DAT332, OCLA0_DAT333, OCLA0_DAT334, OCLA0_DAT335, OCLA0_DAT336, OCLA0_DAT337, OCLA0_DAT338, OCLA0_DAT339, OCLA0_DAT340, OCLA0_DAT341, OCLA0_DAT342, OCLA0_DAT343, OCLA0_DAT344, OCLA0_DAT345
REG_WINDOW = OCLA0_DAT346 OCLA0_DAT346, OCLA0_DAT347, OCLA0_DAT348, OCLA0_DAT349, OCLA0_DAT350, OCLA0_DAT351, OCLA0_DAT352, OCLA0_DAT353, OCLA0_DAT354, OCLA0_DAT355, OCLA0_DAT356, OCLA0_DAT357, OCLA0_DAT358, OCLA0_DAT359, OCLA0_DAT360, OCLA0_DAT361, OCLA0_DAT362, OCLA0_DAT363, OCLA0_DAT364, OCLA0_DAT365, OCLA0_DAT366, OCLA0_DAT367, OCLA0_DAT368, OCLA0_DAT369, OCLA0_DAT370, OCLA0_DAT371, OCLA0_DAT372, OCLA0_DAT373, OCLA0_DAT374, OCLA0_DAT375, OCLA0_DAT376, OCLA0_DAT377, OCLA0_DAT378, OCLA0_DAT379, OCLA0_DAT380, OCLA0_DAT381, OCLA0_DAT382, OCLA0_DAT383, OCLA0_DAT384, OCLA0_DAT385, OCLA0_DAT386, OCLA0_DAT387, OCLA0_DAT388, OCLA0_DAT389, OCLA0_DAT390, OCLA0_DAT391, OCLA0_DAT392, OCLA0_DAT393, OCLA0_DAT394, OCLA0_DAT395
REG_WINDOW = OCLA0_DAT396 OCLA0_DAT396, OCLA0_DAT397, OCLA0_DAT398, OCLA0_DAT399, OCLA0_DAT400, OCLA0_DAT401, OCLA0_DAT402, OCLA0_DAT403, OCLA0_DAT404, OCLA0_DAT405, OCLA0_DAT406, OCLA0_DAT407, OCLA0_DAT408, OCLA0_DAT409, OCLA0_DAT410, OCLA0_DAT411, OCLA0_DAT412, OCLA0_DAT413, OCLA0_DAT414, OCLA0_DAT415, OCLA0_DAT416, OCLA0_DAT417, OCLA0_DAT418, OCLA0_DAT419, OCLA0_DAT420, OCLA0_DAT421, OCLA0_DAT422, OCLA0_DAT423, OCLA0_DAT424, OCLA0_DAT425, OCLA0_DAT426, OCLA0_DAT427, OCLA0_DAT428, OCLA0_DAT429, OCLA0_DAT430, OCLA0_DAT431, OCLA0_DAT432, OCLA0_DAT433, OCLA0_DAT434, OCLA0_DAT435, OCLA0_DAT436, OCLA0_DAT437, OCLA0_DAT438, OCLA0_DAT439, OCLA0_DAT440, OCLA0_DAT441, OCLA0_DAT442, OCLA0_DAT443, OCLA0_DAT444, OCLA0_DAT445
REG_WINDOW = OCLA0_DAT446 OCLA0_DAT446, OCLA0_DAT447, OCLA0_DAT448, OCLA0_DAT449, OCLA0_DAT450, OCLA0_DAT451, OCLA0_DAT452, OCLA0_DAT453, OCLA0_DAT454, OCLA0_DAT455, OCLA0_DAT456, OCLA0_DAT457, OCLA0_DAT458, OCLA0_DAT459, OCLA0_DAT460, OCLA0_DAT461, OCLA0_DAT462, OCLA0_DAT463, OCLA0_DAT464, OCLA0_DAT465, OCLA0_DAT466, OCLA0_DAT467, OCLA0_DAT468, OCLA0_DAT469, OCLA0_DAT470, OCLA0_DAT471, OCLA0_DAT472, OCLA0_DAT473, OCLA0_DAT474, OCLA0_DAT475, OCLA0_DAT476, OCLA0_DAT477, OCLA0_DAT478, OCLA0_DAT479, OCLA0_DAT480, OCLA0_DAT481, OCLA0_DAT482, OCLA0_DAT483, OCLA0_DAT484, OCLA0_DAT485, OCLA0_DAT486, OCLA0_DAT487, OCLA0_DAT488, OCLA0_DAT489, OCLA0_DAT490, OCLA0_DAT491, OCLA0_DAT492, OCLA0_DAT493, OCLA0_DAT494, OCLA0_DAT495
REG_WINDOW = OCLA0_DAT496 OCLA0_DAT496, OCLA0_DAT497, OCLA0_DAT498, OCLA0_DAT499, OCLA0_DAT500, OCLA0_DAT501, OCLA0_DAT502, OCLA0_DAT503, OCLA0_DAT504, OCLA0_DAT505, OCLA0_DAT506, OCLA0_DAT507, OCLA0_DAT508, OCLA0_DAT509, OCLA0_DAT510, OCLA0_DAT511, OCLA0_DAT512, OCLA0_DAT513, OCLA0_DAT514, OCLA0_DAT515, OCLA0_DAT516, OCLA0_DAT517, OCLA0_DAT518, OCLA0_DAT519, OCLA0_DAT520, OCLA0_DAT521, OCLA0_DAT522, OCLA0_DAT523, OCLA0_DAT524, OCLA0_DAT525, OCLA0_DAT526, OCLA0_DAT527, OCLA0_DAT528, OCLA0_DAT529, OCLA0_DAT530, OCLA0_DAT531, OCLA0_DAT532, OCLA0_DAT533, OCLA0_DAT534, OCLA0_DAT535, OCLA0_DAT536, OCLA0_DAT537, OCLA0_DAT538, OCLA0_DAT539, OCLA0_DAT540, OCLA0_DAT541, OCLA0_DAT542, OCLA0_DAT543, OCLA0_DAT544, OCLA0_DAT545
REG_WINDOW = OCLA0_DAT546 OCLA0_DAT546, OCLA0_DAT547, OCLA0_DAT548, OCLA0_DAT549, OCLA0_DAT550, OCLA0_DAT551, OCLA0_DAT552, OCLA0_DAT553, OCLA0_DAT554, OCLA0_DAT555, OCLA0_DAT556, OCLA0_DAT557, OCLA0_DAT558, OCLA0_DAT559, OCLA0_DAT560, OCLA0_DAT561, OCLA0_DAT562, OCLA0_DAT563, OCLA0_DAT564, OCLA0_DAT565, OCLA0_DAT566, OCLA0_DAT567, OCLA0_DAT568, OCLA0_DAT569, OCLA0_DAT570, OCLA0_DAT571, OCLA0_DAT572, OCLA0_DAT573, OCLA0_DAT574, OCLA0_DAT575, OCLA0_DAT576, OCLA0_DAT577, OCLA0_DAT578, OCLA0_DAT579, OCLA0_DAT580, OCLA0_DAT581, OCLA0_DAT582, OCLA0_DAT583, OCLA0_DAT584, OCLA0_DAT585, OCLA0_DAT586, OCLA0_DAT587, OCLA0_DAT588, OCLA0_DAT589, OCLA0_DAT590, OCLA0_DAT591, OCLA0_DAT592, OCLA0_DAT593, OCLA0_DAT594, OCLA0_DAT595
REG_WINDOW = OCLA0_DAT596 OCLA0_DAT596, OCLA0_DAT597, OCLA0_DAT598, OCLA0_DAT599, OCLA0_DAT600, OCLA0_DAT601, OCLA0_DAT602, OCLA0_DAT603, OCLA0_DAT604, OCLA0_DAT605, OCLA0_DAT606, OCLA0_DAT607, OCLA0_DAT608, OCLA0_DAT609, OCLA0_DAT610, OCLA0_DAT611, OCLA0_DAT612, OCLA0_DAT613, OCLA0_DAT614, OCLA0_DAT615, OCLA0_DAT616, OCLA0_DAT617, OCLA0_DAT618, OCLA0_DAT619, OCLA0_DAT620, OCLA0_DAT621, OCLA0_DAT622, OCLA0_DAT623, OCLA0_DAT624, OCLA0_DAT625, OCLA0_DAT626, OCLA0_DAT627, OCLA0_DAT628, OCLA0_DAT629, OCLA0_DAT630, OCLA0_DAT631, OCLA0_DAT632, OCLA0_DAT633, OCLA0_DAT634, OCLA0_DAT635, OCLA0_DAT636, OCLA0_DAT637, OCLA0_DAT638, OCLA0_DAT639, OCLA0_DAT640, OCLA0_DAT641, OCLA0_DAT642, OCLA0_DAT643, OCLA0_DAT644, OCLA0_DAT645
REG_WINDOW = OCLA0_DAT646 OCLA0_DAT646, OCLA0_DAT647, OCLA0_DAT648, OCLA0_DAT649, OCLA0_DAT650, OCLA0_DAT651, OCLA0_DAT652, OCLA0_DAT653, OCLA0_DAT654, OCLA0_DAT655, OCLA0_DAT656, OCLA0_DAT657, OCLA0_DAT658, OCLA0_DAT659, OCLA0_DAT660, OCLA0_DAT661, OCLA0_DAT662, OCLA0_DAT663, OCLA0_DAT664, OCLA0_DAT665, OCLA0_DAT666, OCLA0_DAT667, OCLA0_DAT668, OCLA0_DAT669, OCLA0_DAT670, OCLA0_DAT671, OCLA0_DAT672, OCLA0_DAT673, OCLA0_DAT674, OCLA0_DAT675, OCLA0_DAT676, OCLA0_DAT677, OCLA0_DAT678, OCLA0_DAT679, OCLA0_DAT680, OCLA0_DAT681, OCLA0_DAT682, OCLA0_DAT683, OCLA0_DAT684, OCLA0_DAT685, OCLA0_DAT686, OCLA0_DAT687, OCLA0_DAT688, OCLA0_DAT689, OCLA0_DAT690, OCLA0_DAT691, OCLA0_DAT692, OCLA0_DAT693, OCLA0_DAT694, OCLA0_DAT695
REG_WINDOW = OCLA0_DAT696 OCLA0_DAT696, OCLA0_DAT697, OCLA0_DAT698, OCLA0_DAT699, OCLA0_DAT700, OCLA0_DAT701, OCLA0_DAT702, OCLA0_DAT703, OCLA0_DAT704, OCLA0_DAT705, OCLA0_DAT706, OCLA0_DAT707, OCLA0_DAT708, OCLA0_DAT709, OCLA0_DAT710, OCLA0_DAT711, OCLA0_DAT712, OCLA0_DAT713, OCLA0_DAT714, OCLA0_DAT715, OCLA0_DAT716, OCLA0_DAT717, OCLA0_DAT718, OCLA0_DAT719, OCLA0_DAT720, OCLA0_DAT721, OCLA0_DAT722, OCLA0_DAT723, OCLA0_DAT724, OCLA0_DAT725, OCLA0_DAT726, OCLA0_DAT727, OCLA0_DAT728, OCLA0_DAT729, OCLA0_DAT730, OCLA0_DAT731, OCLA0_DAT732, OCLA0_DAT733, OCLA0_DAT734, OCLA0_DAT735, OCLA0_DAT736, OCLA0_DAT737, OCLA0_DAT738, OCLA0_DAT739, OCLA0_DAT740, OCLA0_DAT741, OCLA0_DAT742, OCLA0_DAT743, OCLA0_DAT744, OCLA0_DAT745
REG_WINDOW = OCLA0_DAT746 OCLA0_DAT746, OCLA0_DAT747, OCLA0_DAT748, OCLA0_DAT749, OCLA0_DAT750, OCLA0_DAT751, OCLA0_DAT752, OCLA0_DAT753, OCLA0_DAT754, OCLA0_DAT755, OCLA0_DAT756, OCLA0_DAT757, OCLA0_DAT758, OCLA0_DAT759, OCLA0_DAT760, OCLA0_DAT761, OCLA0_DAT762, OCLA0_DAT763, OCLA0_DAT764, OCLA0_DAT765, OCLA0_DAT766, OCLA0_DAT767, OCLA0_DAT768, OCLA0_DAT769, OCLA0_DAT770, OCLA0_DAT771, OCLA0_DAT772, OCLA0_DAT773, OCLA0_DAT774, OCLA0_DAT775, OCLA0_DAT776, OCLA0_DAT777, OCLA0_DAT778, OCLA0_DAT779, OCLA0_DAT780, OCLA0_DAT781, OCLA0_DAT782, OCLA0_DAT783, OCLA0_DAT784, OCLA0_DAT785, OCLA0_DAT786, OCLA0_DAT787, OCLA0_DAT788, OCLA0_DAT789, OCLA0_DAT790, OCLA0_DAT791, OCLA0_DAT792, OCLA0_DAT793, OCLA0_DAT794, OCLA0_DAT795
REG_WINDOW = OCLA0_DAT796 OCLA0_DAT796, OCLA0_DAT797, OCLA0_DAT798, OCLA0_DAT799, OCLA0_DAT800, OCLA0_DAT801, OCLA0_DAT802, OCLA0_DAT803, OCLA0_DAT804, OCLA0_DAT805, OCLA0_DAT806, OCLA0_DAT807, OCLA0_DAT808, OCLA0_DAT809, OCLA0_DAT810, OCLA0_DAT811, OCLA0_DAT812, OCLA0_DAT813, OCLA0_DAT814, OCLA0_DAT815, OCLA0_DAT816, OCLA0_DAT817, OCLA0_DAT818, OCLA0_DAT819, OCLA0_DAT820, OCLA0_DAT821, OCLA0_DAT822, OCLA0_DAT823, OCLA0_DAT824, OCLA0_DAT825, OCLA0_DAT826, OCLA0_DAT827, OCLA0_DAT828, OCLA0_DAT829, OCLA0_DAT830, OCLA0_DAT831, OCLA0_DAT832, OCLA0_DAT833, OCLA0_DAT834, OCLA0_DAT835, OCLA0_DAT836, OCLA0_DAT837, OCLA0_DAT838, OCLA0_DAT839, OCLA0_DAT840, OCLA0_DAT841, OCLA0_DAT842, OCLA0_DAT843, OCLA0_DAT844, OCLA0_DAT845
REG_WINDOW = OCLA0_DAT846 OCLA0_DAT846, OCLA0_DAT847, OCLA0_DAT848, OCLA0_DAT849, OCLA0_DAT850, OCLA0_DAT851, OCLA0_DAT852, OCLA0_DAT853, OCLA0_DAT854, OCLA0_DAT855, OCLA0_DAT856, OCLA0_DAT857, OCLA0_DAT858, OCLA0_DAT859, OCLA0_DAT860, OCLA0_DAT861, OCLA0_DAT862, OCLA0_DAT863, OCLA0_DAT864, OCLA0_DAT865, OCLA0_DAT866, OCLA0_DAT867, OCLA0_DAT868, OCLA0_DAT869, OCLA0_DAT870, OCLA0_DAT871, OCLA0_DAT872, OCLA0_DAT873, OCLA0_DAT874, OCLA0_DAT875, OCLA0_DAT876, OCLA0_DAT877, OCLA0_DAT878, OCLA0_DAT879, OCLA0_DAT880, OCLA0_DAT881, OCLA0_DAT882, OCLA0_DAT883, OCLA0_DAT884, OCLA0_DAT885, OCLA0_DAT886, OCLA0_DAT887, OCLA0_DAT888, OCLA0_DAT889, OCLA0_DAT890, OCLA0_DAT891, OCLA0_DAT892, OCLA0_DAT893, OCLA0_DAT894, OCLA0_DAT895
REG_WINDOW = OCLA0_DAT896 OCLA0_DAT896, OCLA0_DAT897, OCLA0_DAT898, OCLA0_DAT899, OCLA0_DAT900, OCLA0_DAT901, OCLA0_DAT902, OCLA0_DAT903, OCLA0_DAT904, OCLA0_DAT905, OCLA0_DAT906, OCLA0_DAT907, OCLA0_DAT908, OCLA0_DAT909, OCLA0_DAT910, OCLA0_DAT911, OCLA0_DAT912, OCLA0_DAT913, OCLA0_DAT914, OCLA0_DAT915, OCLA0_DAT916, OCLA0_DAT917, OCLA0_DAT918, OCLA0_DAT919, OCLA0_DAT920, OCLA0_DAT921, OCLA0_DAT922, OCLA0_DAT923, OCLA0_DAT924, OCLA0_DAT925, OCLA0_DAT926, OCLA0_DAT927, OCLA0_DAT928, OCLA0_DAT929, OCLA0_DAT930, OCLA0_DAT931, OCLA0_DAT932, OCLA0_DAT933, OCLA0_DAT934, OCLA0_DAT935, OCLA0_DAT936, OCLA0_DAT937, OCLA0_DAT938, OCLA0_DAT939, OCLA0_DAT940, OCLA0_DAT941, OCLA0_DAT942, OCLA0_DAT943, OCLA0_DAT944, OCLA0_DAT945
REG_WINDOW = OCLA0_DAT946 OCLA0_DAT946, OCLA0_DAT947, OCLA0_DAT948, OCLA0_DAT949, OCLA0_DAT950, OCLA0_DAT951, OCLA0_DAT952, OCLA0_DAT953, OCLA0_DAT954, OCLA0_DAT955, OCLA0_DAT956, OCLA0_DAT957, OCLA0_DAT958, OCLA0_DAT959, OCLA0_DAT960, OCLA0_DAT961, OCLA0_DAT962, OCLA0_DAT963, OCLA0_DAT964, OCLA0_DAT965, OCLA0_DAT966, OCLA0_DAT967, OCLA0_DAT968, OCLA0_DAT969, OCLA0_DAT970, OCLA0_DAT971, OCLA0_DAT972, OCLA0_DAT973, OCLA0_DAT974, OCLA0_DAT975, OCLA0_DAT976, OCLA0_DAT977, OCLA0_DAT978, OCLA0_DAT979, OCLA0_DAT980, OCLA0_DAT981, OCLA0_DAT982, OCLA0_DAT983, OCLA0_DAT984, OCLA0_DAT985, OCLA0_DAT986, OCLA0_DAT987, OCLA0_DAT988, OCLA0_DAT989, OCLA0_DAT990, OCLA0_DAT991, OCLA0_DAT992, OCLA0_DAT993, OCLA0_DAT994, OCLA0_DAT995
REG_WINDOW = OCLA0_DAT996 OCLA0_DAT996, OCLA0_DAT997, OCLA0_DAT998, OCLA0_DAT999, OCLA0_DAT1000, OCLA0_DAT1001, OCLA0_DAT1002, OCLA0_DAT1003, OCLA0_DAT1004, OCLA0_DAT1005, OCLA0_DAT1006, OCLA0_DAT1007, OCLA0_DAT1008, OCLA0_DAT1009, OCLA0_DAT1010, OCLA0_DAT1011, OCLA0_DAT1012, OCLA0_DAT1013, OCLA0_DAT1014, OCLA0_DAT1015, OCLA0_DAT1016, OCLA0_DAT1017, OCLA0_DAT1018, OCLA0_DAT1019, OCLA0_DAT1020, OCLA0_DAT1021, OCLA0_DAT1022, OCLA0_DAT1023, OCLA0_DAT_POP, OCLA0_FIFO_DEPTH, OCLA0_FIFO_LIMIT, OCLA0_FIFO_TAIL, OCLA0_FIFO_TRIG, OCLA0_FIFO_WRAP, OCLA0_GEN_CTL, OCLA0_MAT000_COUNT, OCLA0_MAT001_COUNT, OCLA0_MAT002_COUNT, OCLA0_MAT003_COUNT, OCLA0_MAT000_CTL, OCLA0_MAT001_CTL, OCLA0_MAT002_CTL, OCLA0_MAT003_CTL, OCLA0_MAT000_THRESH, OCLA0_MAT001_THRESH, OCLA0_MAT002_THRESH, OCLA0_MAT003_THRESH, OCLA0_RAW000, OCLA0_RAW001, OCLA0_SFT_RST
REG_WINDOW = OCLA0_STACK_BASE OCLA0_STACK_BASE, OCLA0_STACK_CUR, OCLA0_STACK_STORE_CNT, OCLA0_STACK_TOP, OCLA0_STACK_WRAP, OCLA0_STAGE000, OCLA0_STAGE001, OCLA0_STAGE002, OCLA0_STAGE003, OCLA0_STAGE004, OCLA0_STAGE005, OCLA0_STAGE006, OCLA0_STAGE007, OCLA0_STAGE008, OCLA0_STAGE009, OCLA0_STAGE010, OCLA0_STAGE011, OCLA0_STAGE012, OCLA0_STAGE013, OCLA0_STAGE014, OCLA0_STAGE015, OCLA0_STAGE016, OCLA0_STAGE017, OCLA0_STAGE018, OCLA0_STAGE019, OCLA0_STAGE020, OCLA0_STAGE021, OCLA0_STAGE022, OCLA0_STAGE023, OCLA0_STAGE024, OCLA0_STAGE025, OCLA0_STAGE026, OCLA0_STAGE027, OCLA0_STAGE028, OCLA0_STAGE029, OCLA0_STAGE030, OCLA0_STAGE031, OCLA0_STAGE032, OCLA0_STAGE033, OCLA0_STAGE034, OCLA0_STAGE035, OCLA0_STAGE036, OCLA0_STAGE037, OCLA0_STAGE038, OCLA0_STAGE039, OCLA0_STAGE040, OCLA0_STAGE041, OCLA0_STAGE042, OCLA0_STAGE043, OCLA0_STAGE044
REG_WINDOW = OCLA0_STAGE045 OCLA0_STAGE045, OCLA0_STAGE046, OCLA0_STAGE047, OCLA0_STAGE048, OCLA0_STAGE049, OCLA0_STAGE050, OCLA0_STAGE051, OCLA0_STAGE052, OCLA0_STAGE053, OCLA0_STAGE054, OCLA0_STAGE055, OCLA0_STAGE056, OCLA0_STAGE057, OCLA0_STAGE058, OCLA0_STAGE059, OCLA0_STAGE060, OCLA0_STAGE061, OCLA0_STAGE062, OCLA0_STAGE063, OCLA0_STAGE064, OCLA0_STAGE065, OCLA0_STAGE066, OCLA0_STAGE067, OCLA0_STAGE068, OCLA0_STAGE069, OCLA0_STAGE070, OCLA0_STAGE071, OCLA0_STATE_INT, OCLA0_STATE_SET, OCLA0_TIME
REG_WINDOW = PCM PCM_CLK0_CFG, PCM_CLK1_CFG, PCM_CLK0_DBG, PCM_CLK1_DBG, PCM_CLK0_GEN, PCM_CLK1_GEN
REG_WINDOW = PCM0 PCM0_DMA_CFG, PCM0_INT_ENA, PCM0_INT_SUM, PCM0_RXADDR, PCM0_RXCNT, PCM0_RXMSK000, PCM0_RXMSK001, PCM0_RXMSK002, PCM0_RXMSK003, PCM0_RXMSK004, PCM0_RXMSK005, PCM0_RXMSK006, PCM0_RXMSK007, PCM0_RXSTART, PCM0_TDM_CFG, PCM0_TDM_DBG, PCM0_TXADDR, PCM0_TXCNT, PCM0_TXMSK000, PCM0_TXMSK001, PCM0_TXMSK002, PCM0_TXMSK003, PCM0_TXMSK004, PCM0_TXMSK005, PCM0_TXMSK006, PCM0_TXMSK007, PCM0_TXSTART
REG_WINDOW = PCM1 PCM1_DMA_CFG, PCM1_INT_ENA, PCM1_INT_SUM, PCM1_RXADDR, PCM1_RXCNT, PCM1_RXMSK000, PCM1_RXMSK001, PCM1_RXMSK002, PCM1_RXMSK003, PCM1_RXMSK004, PCM1_RXMSK005, PCM1_RXMSK006, PCM1_RXMSK007, PCM1_RXSTART, PCM1_TDM_CFG, PCM1_TDM_DBG, PCM1_TXADDR, PCM1_TXCNT, PCM1_TXMSK000, PCM1_TXMSK001, PCM1_TXMSK002, PCM1_TXMSK003, PCM1_TXMSK004, PCM1_TXMSK005, PCM1_TXMSK006, PCM1_TXMSK007, PCM1_TXSTART
REG_WINDOW = PCM2 PCM2_DMA_CFG, PCM2_INT_ENA, PCM2_INT_SUM, PCM2_RXADDR, PCM2_RXCNT, PCM2_RXMSK000, PCM2_RXMSK001, PCM2_RXMSK002, PCM2_RXMSK003, PCM2_RXMSK004, PCM2_RXMSK005, PCM2_RXMSK006, PCM2_RXMSK007, PCM2_RXSTART, PCM2_TDM_CFG, PCM2_TDM_DBG, PCM2_TXADDR, PCM2_TXCNT, PCM2_TXMSK000, PCM2_TXMSK001, PCM2_TXMSK002, PCM2_TXMSK003, PCM2_TXMSK004, PCM2_TXMSK005, PCM2_TXMSK006, PCM2_TXMSK007, PCM2_TXSTART
REG_WINDOW = PCM3 PCM3_DMA_CFG, PCM3_INT_ENA, PCM3_INT_SUM, PCM3_RXADDR, PCM3_RXCNT, PCM3_RXMSK000, PCM3_RXMSK001, PCM3_RXMSK002, PCM3_RXMSK003, PCM3_RXMSK004, PCM3_RXMSK005, PCM3_RXMSK006, PCM3_RXMSK007, PCM3_RXSTART, PCM3_TDM_CFG, PCM3_TDM_DBG, PCM3_TXADDR, PCM3_TXCNT, PCM3_TXMSK000, PCM3_TXMSK001, PCM3_TXMSK002, PCM3_TXMSK003, PCM3_TXMSK004, PCM3_TXMSK005, PCM3_TXMSK006, PCM3_TXMSK007, PCM3_TXSTART
REG_WINDOW = PCS0_AN000_ADV_REG PCS0_AN000_ADV_REG, PCS0_AN001_ADV_REG, PCS0_AN002_ADV_REG, PCS0_AN003_ADV_REG, PCS0_AN000_EXT_ST_REG, PCS0_AN001_EXT_ST_REG, PCS0_AN002_EXT_ST_REG, PCS0_AN003_EXT_ST_REG, PCS0_AN000_LP_ABIL_REG, PCS0_AN001_LP_ABIL_REG, PCS0_AN002_LP_ABIL_REG, PCS0_AN003_LP_ABIL_REG, PCS0_AN000_RESULTS_REG, PCS0_AN001_RESULTS_REG, PCS0_AN002_RESULTS_REG, PCS0_AN003_RESULTS_REG, PCS0_INT000_EN_REG, PCS0_INT001_EN_REG, PCS0_INT002_EN_REG, PCS0_INT003_EN_REG, PCS0_INT000_REG, PCS0_INT001_REG, PCS0_INT002_REG, PCS0_INT003_REG, PCS0_LINK000_TIMER_COUNT_REG, PCS0_LINK001_TIMER_COUNT_REG, PCS0_LINK002_TIMER_COUNT_REG, PCS0_LINK003_TIMER_COUNT_REG, PCS0_LOG_ANL000_REG, PCS0_LOG_ANL001_REG, PCS0_LOG_ANL002_REG, PCS0_LOG_ANL003_REG, PCS0_MAC_CRDT_CNT000_REG, PCS0_MAC_CRDT_CNT001_REG, PCS0_MAC_CRDT_CNT002_REG, PCS0_MAC_CRDT_CNT003_REG, PCS0_MISC000_CTL_REG, PCS0_MISC001_CTL_REG, PCS0_MISC002_CTL_REG, PCS0_MISC003_CTL_REG, PCS0_MR000_CONTROL_REG, PCS0_MR001_CONTROL_REG, PCS0_MR002_CONTROL_REG, PCS0_MR003_CONTROL_REG, PCS0_MR000_STATUS_REG, PCS0_MR001_STATUS_REG, PCS0_MR002_STATUS_REG, PCS0_MR003_STATUS_REG, PCS0_RX000_STATES_REG, PCS0_RX001_STATES_REG
REG_WINDOW = PCS0_RX002_STATES_REG PCS0_RX002_STATES_REG, PCS0_RX003_STATES_REG, PCS0_RX000_SYNC_REG, PCS0_RX001_SYNC_REG, PCS0_RX002_SYNC_REG, PCS0_RX003_SYNC_REG, PCS0_SERDES_CRDT_CNT000_REG, PCS0_SERDES_CRDT_CNT001_REG, PCS0_SERDES_CRDT_CNT002_REG, PCS0_SERDES_CRDT_CNT003_REG, PCS0_SGM000_AN_ADV_REG, PCS0_SGM001_AN_ADV_REG, PCS0_SGM002_AN_ADV_REG, PCS0_SGM003_AN_ADV_REG, PCS0_SGM000_LP_ADV_REG, PCS0_SGM001_LP_ADV_REG, PCS0_SGM002_LP_ADV_REG, PCS0_SGM003_LP_ADV_REG, PCS0_TX000_STATES_REG, PCS0_TX001_STATES_REG, PCS0_TX002_STATES_REG, PCS0_TX003_STATES_REG, PCS0_TX_RX000_POLARITY_REG, PCS0_TX_RX001_POLARITY_REG, PCS0_TX_RX002_POLARITY_REG, PCS0_TX_RX003_POLARITY_REG
REG_WINDOW = PCS1_AN000_ADV_REG PCS1_AN000_ADV_REG, PCS1_AN001_ADV_REG, PCS1_AN002_ADV_REG, PCS1_AN003_ADV_REG, PCS1_AN000_EXT_ST_REG, PCS1_AN001_EXT_ST_REG, PCS1_AN002_EXT_ST_REG, PCS1_AN003_EXT_ST_REG, PCS1_AN000_LP_ABIL_REG, PCS1_AN001_LP_ABIL_REG, PCS1_AN002_LP_ABIL_REG, PCS1_AN003_LP_ABIL_REG, PCS1_AN000_RESULTS_REG, PCS1_AN001_RESULTS_REG, PCS1_AN002_RESULTS_REG, PCS1_AN003_RESULTS_REG, PCS1_INT000_EN_REG, PCS1_INT001_EN_REG, PCS1_INT002_EN_REG, PCS1_INT003_EN_REG, PCS1_INT000_REG, PCS1_INT001_REG, PCS1_INT002_REG, PCS1_INT003_REG, PCS1_LINK000_TIMER_COUNT_REG, PCS1_LINK001_TIMER_COUNT_REG, PCS1_LINK002_TIMER_COUNT_REG, PCS1_LINK003_TIMER_COUNT_REG, PCS1_LOG_ANL000_REG, PCS1_LOG_ANL001_REG, PCS1_LOG_ANL002_REG, PCS1_LOG_ANL003_REG, PCS1_MAC_CRDT_CNT000_REG, PCS1_MAC_CRDT_CNT001_REG, PCS1_MAC_CRDT_CNT002_REG, PCS1_MAC_CRDT_CNT003_REG, PCS1_MISC000_CTL_REG, PCS1_MISC001_CTL_REG, PCS1_MISC002_CTL_REG, PCS1_MISC003_CTL_REG, PCS1_MR000_CONTROL_REG, PCS1_MR001_CONTROL_REG, PCS1_MR002_CONTROL_REG, PCS1_MR003_CONTROL_REG, PCS1_MR000_STATUS_REG, PCS1_MR001_STATUS_REG, PCS1_MR002_STATUS_REG, PCS1_MR003_STATUS_REG, PCS1_RX000_STATES_REG, PCS1_RX001_STATES_REG
REG_WINDOW = PCS1_RX002_STATES_REG PCS1_RX002_STATES_REG, PCS1_RX003_STATES_REG, PCS1_RX000_SYNC_REG, PCS1_RX001_SYNC_REG, PCS1_RX002_SYNC_REG, PCS1_RX003_SYNC_REG, PCS1_SERDES_CRDT_CNT000_REG, PCS1_SERDES_CRDT_CNT001_REG, PCS1_SERDES_CRDT_CNT002_REG, PCS1_SERDES_CRDT_CNT003_REG, PCS1_SGM000_AN_ADV_REG, PCS1_SGM001_AN_ADV_REG, PCS1_SGM002_AN_ADV_REG, PCS1_SGM003_AN_ADV_REG, PCS1_SGM000_LP_ADV_REG, PCS1_SGM001_LP_ADV_REG, PCS1_SGM002_LP_ADV_REG, PCS1_SGM003_LP_ADV_REG, PCS1_TX000_STATES_REG, PCS1_TX001_STATES_REG, PCS1_TX002_STATES_REG, PCS1_TX003_STATES_REG, PCS1_TX_RX000_POLARITY_REG, PCS1_TX_RX001_POLARITY_REG, PCS1_TX_RX002_POLARITY_REG, PCS1_TX_RX003_POLARITY_REG
REG_WINDOW = PCSX0 PCSX0_10GBX_STATUS_REG, PCSX0_BIST_STATUS_REG, PCSX0_BIT_LOCK_STATUS_REG, PCSX0_CONTROL1_REG, PCSX0_CONTROL2_REG, PCSX0_INT_EN_REG, PCSX0_INT_REG, PCSX0_LOG_ANL_REG, PCSX0_MISC_CTL_REG, PCSX0_RX_SYNC_STATES_REG, PCSX0_SERDES_CRDT_CNT_REG, PCSX0_SPD_ABIL_REG, PCSX0_STATUS1_REG, PCSX0_STATUS2_REG, PCSX0_TX_RX_POLARITY_REG, PCSX0_TX_RX_STATES_REG
REG_WINDOW = PEM0 PEM0_BAR1_INDEX000, PEM0_BAR1_INDEX001, PEM0_BAR1_INDEX002, PEM0_BAR1_INDEX003, PEM0_BAR1_INDEX004, PEM0_BAR1_INDEX005, PEM0_BAR1_INDEX006, PEM0_BAR1_INDEX007, PEM0_BAR1_INDEX008, PEM0_BAR1_INDEX009, PEM0_BAR1_INDEX010, PEM0_BAR1_INDEX011, PEM0_BAR1_INDEX012, PEM0_BAR1_INDEX013, PEM0_BAR1_INDEX014, PEM0_BAR1_INDEX015, PEM0_BAR2_MASK, PEM0_BAR_CTL, PEM0_BIST_STATUS, PEM0_BIST_STATUS2, PEM0_CFG, PEM0_CFG_RD, PEM0_CFG_WR, PEM0_CLK_EN, PEM0_CPL_LUT_VALID, PEM0_CTL_STATUS, PEM0_DBG_INFO, PEM0_DBG_INFO_EN, PEM0_DIAG_STATUS, PEM0_ECC_ENA, PEM0_ECC_SYND_CTRL, PEM0_INB_READ_CREDITS, PEM0_INT_ENB, PEM0_INT_ENB_INT, PEM0_INT_SUM, PEM0_ON, PEM0_P2N_BAR0_START, PEM0_P2N_BAR1_START, PEM0_P2N_BAR2_START, PEM0_SPI_CTL, PEM0_SPI_DATA, PEM0_STRAP, PEM0_TLP_CREDITS
REG_WINDOW = PEM1 PEM1_BAR1_INDEX000, PEM1_BAR1_INDEX001, PEM1_BAR1_INDEX002, PEM1_BAR1_INDEX003, PEM1_BAR1_INDEX004, PEM1_BAR1_INDEX005, PEM1_BAR1_INDEX006, PEM1_BAR1_INDEX007, PEM1_BAR1_INDEX008, PEM1_BAR1_INDEX009, PEM1_BAR1_INDEX010, PEM1_BAR1_INDEX011, PEM1_BAR1_INDEX012, PEM1_BAR1_INDEX013, PEM1_BAR1_INDEX014, PEM1_BAR1_INDEX015, PEM1_BAR2_MASK, PEM1_BAR_CTL, PEM1_BIST_STATUS, PEM1_BIST_STATUS2, PEM1_CFG, PEM1_CFG_RD, PEM1_CFG_WR, PEM1_CLK_EN, PEM1_CPL_LUT_VALID, PEM1_CTL_STATUS, PEM1_DBG_INFO, PEM1_DBG_INFO_EN, PEM1_DIAG_STATUS, PEM1_ECC_ENA, PEM1_ECC_SYND_CTRL, PEM1_INB_READ_CREDITS, PEM1_INT_ENB, PEM1_INT_ENB_INT, PEM1_INT_SUM, PEM1_ON, PEM1_P2N_BAR0_START, PEM1_P2N_BAR1_START, PEM1_P2N_BAR2_START, PEM1_SPI_CTL, PEM1_SPI_DATA, PEM1_STRAP, PEM1_TLP_CREDITS
REG_WINDOW = PEM2 PEM2_BAR1_INDEX000, PEM2_BAR1_INDEX001, PEM2_BAR1_INDEX002, PEM2_BAR1_INDEX003, PEM2_BAR1_INDEX004, PEM2_BAR1_INDEX005, PEM2_BAR1_INDEX006, PEM2_BAR1_INDEX007, PEM2_BAR1_INDEX008, PEM2_BAR1_INDEX009, PEM2_BAR1_INDEX010, PEM2_BAR1_INDEX011, PEM2_BAR1_INDEX012, PEM2_BAR1_INDEX013, PEM2_BAR1_INDEX014, PEM2_BAR1_INDEX015, PEM2_BAR2_MASK, PEM2_BAR_CTL, PEM2_BIST_STATUS, PEM2_BIST_STATUS2, PEM2_CFG, PEM2_CFG_RD, PEM2_CFG_WR, PEM2_CLK_EN, PEM2_CPL_LUT_VALID, PEM2_CTL_STATUS, PEM2_DBG_INFO, PEM2_DBG_INFO_EN, PEM2_DIAG_STATUS, PEM2_ECC_ENA, PEM2_ECC_SYND_CTRL, PEM2_INB_READ_CREDITS, PEM2_INT_ENB, PEM2_INT_ENB_INT, PEM2_INT_SUM, PEM2_ON, PEM2_P2N_BAR0_START, PEM2_P2N_BAR1_START, PEM2_P2N_BAR2_START, PEM2_SPI_CTL, PEM2_SPI_DATA, PEM2_STRAP, PEM2_TLP_CREDITS
REG_WINDOW = PIP_ALT_SKIP_CFG0 PIP_ALT_SKIP_CFG0, PIP_ALT_SKIP_CFG1, PIP_ALT_SKIP_CFG2, PIP_ALT_SKIP_CFG3, PIP_BCK_PRS, PIP_BIST_STATUS, PIP_BSEL_EXT_CFG0, PIP_BSEL_EXT_CFG1, PIP_BSEL_EXT_CFG2, PIP_BSEL_EXT_CFG3, PIP_BSEL_EXT_POS0, PIP_BSEL_EXT_POS1, PIP_BSEL_EXT_POS2, PIP_BSEL_EXT_POS3, PIP_BSEL_TBL_ENT0, PIP_BSEL_TBL_ENT1, PIP_BSEL_TBL_ENT2, PIP_BSEL_TBL_ENT3, PIP_BSEL_TBL_ENT4, PIP_BSEL_TBL_ENT5, PIP_BSEL_TBL_ENT6, PIP_BSEL_TBL_ENT7, PIP_BSEL_TBL_ENT8, PIP_BSEL_TBL_ENT9, PIP_BSEL_TBL_ENT10, PIP_BSEL_TBL_ENT11, PIP_BSEL_TBL_ENT12, PIP_BSEL_TBL_ENT13, PIP_BSEL_TBL_ENT14, PIP_BSEL_TBL_ENT15, PIP_BSEL_TBL_ENT16, PIP_BSEL_TBL_ENT17, PIP_BSEL_TBL_ENT18, PIP_BSEL_TBL_ENT19, PIP_BSEL_TBL_ENT20, PIP_BSEL_TBL_ENT21, PIP_BSEL_TBL_ENT22, PIP_BSEL_TBL_ENT23, PIP_BSEL_TBL_ENT24, PIP_BSEL_TBL_ENT25, PIP_BSEL_TBL_ENT26, PIP_BSEL_TBL_ENT27, PIP_BSEL_TBL_ENT28, PIP_BSEL_TBL_ENT29, PIP_BSEL_TBL_ENT30, PIP_BSEL_TBL_ENT31, PIP_BSEL_TBL_ENT32, PIP_BSEL_TBL_ENT33, PIP_BSEL_TBL_ENT34, PIP_BSEL_TBL_ENT35
REG_WINDOW = PIP_BSEL_TBL_ENT36 PIP_BSEL_TBL_ENT36, PIP_BSEL_TBL_ENT37, PIP_BSEL_TBL_ENT38, PIP_BSEL_TBL_ENT39, PIP_BSEL_TBL_ENT40, PIP_BSEL_TBL_ENT41, PIP_BSEL_TBL_ENT42, PIP_BSEL_TBL_ENT43, PIP_BSEL_TBL_ENT44, PIP_BSEL_TBL_ENT45, PIP_BSEL_TBL_ENT46, PIP_BSEL_TBL_ENT47, PIP_BSEL_TBL_ENT48, PIP_BSEL_TBL_ENT49, PIP_BSEL_TBL_ENT50, PIP_BSEL_TBL_ENT51, PIP_BSEL_TBL_ENT52, PIP_BSEL_TBL_ENT53, PIP_BSEL_TBL_ENT54, PIP_BSEL_TBL_ENT55, PIP_BSEL_TBL_ENT56, PIP_BSEL_TBL_ENT57, PIP_BSEL_TBL_ENT58, PIP_BSEL_TBL_ENT59, PIP_BSEL_TBL_ENT60, PIP_BSEL_TBL_ENT61, PIP_BSEL_TBL_ENT62, PIP_BSEL_TBL_ENT63, PIP_BSEL_TBL_ENT64, PIP_BSEL_TBL_ENT65, PIP_BSEL_TBL_ENT66, PIP_BSEL_TBL_ENT67, PIP_BSEL_TBL_ENT68, PIP_BSEL_TBL_ENT69, PIP_BSEL_TBL_ENT70, PIP_BSEL_TBL_ENT71, PIP_BSEL_TBL_ENT72, PIP_BSEL_TBL_ENT73, PIP_BSEL_TBL_ENT74, PIP_BSEL_TBL_ENT75, PIP_BSEL_TBL_ENT76, PIP_BSEL_TBL_ENT77, PIP_BSEL_TBL_ENT78, PIP_BSEL_TBL_ENT79, PIP_BSEL_TBL_ENT80, PIP_BSEL_TBL_ENT81, PIP_BSEL_TBL_ENT82, PIP_BSEL_TBL_ENT83, PIP_BSEL_TBL_ENT84, PIP_BSEL_TBL_ENT85
REG_WINDOW = PIP_BSEL_TBL_ENT86 PIP_BSEL_TBL_ENT86, PIP_BSEL_TBL_ENT87, PIP_BSEL_TBL_ENT88, PIP_BSEL_TBL_ENT89, PIP_BSEL_TBL_ENT90, PIP_BSEL_TBL_ENT91, PIP_BSEL_TBL_ENT92, PIP_BSEL_TBL_ENT93, PIP_BSEL_TBL_ENT94, PIP_BSEL_TBL_ENT95, PIP_BSEL_TBL_ENT96, PIP_BSEL_TBL_ENT97, PIP_BSEL_TBL_ENT98, PIP_BSEL_TBL_ENT99, PIP_BSEL_TBL_ENT100, PIP_BSEL_TBL_ENT101, PIP_BSEL_TBL_ENT102, PIP_BSEL_TBL_ENT103, PIP_BSEL_TBL_ENT104, PIP_BSEL_TBL_ENT105, PIP_BSEL_TBL_ENT106, PIP_BSEL_TBL_ENT107, PIP_BSEL_TBL_ENT108, PIP_BSEL_TBL_ENT109, PIP_BSEL_TBL_ENT110, PIP_BSEL_TBL_ENT111, PIP_BSEL_TBL_ENT112, PIP_BSEL_TBL_ENT113, PIP_BSEL_TBL_ENT114, PIP_BSEL_TBL_ENT115, PIP_BSEL_TBL_ENT116, PIP_BSEL_TBL_ENT117, PIP_BSEL_TBL_ENT118, PIP_BSEL_TBL_ENT119, PIP_BSEL_TBL_ENT120, PIP_BSEL_TBL_ENT121, PIP_BSEL_TBL_ENT122, PIP_BSEL_TBL_ENT123, PIP_BSEL_TBL_ENT124, PIP_BSEL_TBL_ENT125, PIP_BSEL_TBL_ENT126, PIP_BSEL_TBL_ENT127, PIP_BSEL_TBL_ENT128, PIP_BSEL_TBL_ENT129, PIP_BSEL_TBL_ENT130, PIP_BSEL_TBL_ENT131, PIP_BSEL_TBL_ENT132, PIP_BSEL_TBL_ENT133, PIP_BSEL_TBL_ENT134, PIP_BSEL_TBL_ENT135
REG_WINDOW = PIP_BSEL_TBL_ENT136 PIP_BSEL_TBL_ENT136, PIP_BSEL_TBL_ENT137, PIP_BSEL_TBL_ENT138, PIP_BSEL_TBL_ENT139, PIP_BSEL_TBL_ENT140, PIP_BSEL_TBL_ENT141, PIP_BSEL_TBL_ENT142, PIP_BSEL_TBL_ENT143, PIP_BSEL_TBL_ENT144, PIP_BSEL_TBL_ENT145, PIP_BSEL_TBL_ENT146, PIP_BSEL_TBL_ENT147, PIP_BSEL_TBL_ENT148, PIP_BSEL_TBL_ENT149, PIP_BSEL_TBL_ENT150, PIP_BSEL_TBL_ENT151, PIP_BSEL_TBL_ENT152, PIP_BSEL_TBL_ENT153, PIP_BSEL_TBL_ENT154, PIP_BSEL_TBL_ENT155, PIP_BSEL_TBL_ENT156, PIP_BSEL_TBL_ENT157, PIP_BSEL_TBL_ENT158, PIP_BSEL_TBL_ENT159, PIP_BSEL_TBL_ENT160, PIP_BSEL_TBL_ENT161, PIP_BSEL_TBL_ENT162, PIP_BSEL_TBL_ENT163, PIP_BSEL_TBL_ENT164, PIP_BSEL_TBL_ENT165, PIP_BSEL_TBL_ENT166, PIP_BSEL_TBL_ENT167, PIP_BSEL_TBL_ENT168, PIP_BSEL_TBL_ENT169, PIP_BSEL_TBL_ENT170, PIP_BSEL_TBL_ENT171, PIP_BSEL_TBL_ENT172, PIP_BSEL_TBL_ENT173, PIP_BSEL_TBL_ENT174, PIP_BSEL_TBL_ENT175, PIP_BSEL_TBL_ENT176, PIP_BSEL_TBL_ENT177, PIP_BSEL_TBL_ENT178, PIP_BSEL_TBL_ENT179, PIP_BSEL_TBL_ENT180, PIP_BSEL_TBL_ENT181, PIP_BSEL_TBL_ENT182, PIP_BSEL_TBL_ENT183, PIP_BSEL_TBL_ENT184, PIP_BSEL_TBL_ENT185
REG_WINDOW = PIP_BSEL_TBL_ENT186 PIP_BSEL_TBL_ENT186, PIP_BSEL_TBL_ENT187, PIP_BSEL_TBL_ENT188, PIP_BSEL_TBL_ENT189, PIP_BSEL_TBL_ENT190, PIP_BSEL_TBL_ENT191, PIP_BSEL_TBL_ENT192, PIP_BSEL_TBL_ENT193, PIP_BSEL_TBL_ENT194, PIP_BSEL_TBL_ENT195, PIP_BSEL_TBL_ENT196, PIP_BSEL_TBL_ENT197, PIP_BSEL_TBL_ENT198, PIP_BSEL_TBL_ENT199, PIP_BSEL_TBL_ENT200, PIP_BSEL_TBL_ENT201, PIP_BSEL_TBL_ENT202, PIP_BSEL_TBL_ENT203, PIP_BSEL_TBL_ENT204, PIP_BSEL_TBL_ENT205, PIP_BSEL_TBL_ENT206, PIP_BSEL_TBL_ENT207, PIP_BSEL_TBL_ENT208, PIP_BSEL_TBL_ENT209, PIP_BSEL_TBL_ENT210, PIP_BSEL_TBL_ENT211, PIP_BSEL_TBL_ENT212, PIP_BSEL_TBL_ENT213, PIP_BSEL_TBL_ENT214, PIP_BSEL_TBL_ENT215, PIP_BSEL_TBL_ENT216, PIP_BSEL_TBL_ENT217, PIP_BSEL_TBL_ENT218, PIP_BSEL_TBL_ENT219, PIP_BSEL_TBL_ENT220, PIP_BSEL_TBL_ENT221, PIP_BSEL_TBL_ENT222, PIP_BSEL_TBL_ENT223, PIP_BSEL_TBL_ENT224, PIP_BSEL_TBL_ENT225, PIP_BSEL_TBL_ENT226, PIP_BSEL_TBL_ENT227, PIP_BSEL_TBL_ENT228, PIP_BSEL_TBL_ENT229, PIP_BSEL_TBL_ENT230, PIP_BSEL_TBL_ENT231, PIP_BSEL_TBL_ENT232, PIP_BSEL_TBL_ENT233, PIP_BSEL_TBL_ENT234, PIP_BSEL_TBL_ENT235
REG_WINDOW = PIP_BSEL_TBL_ENT236 PIP_BSEL_TBL_ENT236, PIP_BSEL_TBL_ENT237, PIP_BSEL_TBL_ENT238, PIP_BSEL_TBL_ENT239, PIP_BSEL_TBL_ENT240, PIP_BSEL_TBL_ENT241, PIP_BSEL_TBL_ENT242, PIP_BSEL_TBL_ENT243, PIP_BSEL_TBL_ENT244, PIP_BSEL_TBL_ENT245, PIP_BSEL_TBL_ENT246, PIP_BSEL_TBL_ENT247, PIP_BSEL_TBL_ENT248, PIP_BSEL_TBL_ENT249, PIP_BSEL_TBL_ENT250, PIP_BSEL_TBL_ENT251, PIP_BSEL_TBL_ENT252, PIP_BSEL_TBL_ENT253, PIP_BSEL_TBL_ENT254, PIP_BSEL_TBL_ENT255, PIP_BSEL_TBL_ENT256, PIP_BSEL_TBL_ENT257, PIP_BSEL_TBL_ENT258, PIP_BSEL_TBL_ENT259, PIP_BSEL_TBL_ENT260, PIP_BSEL_TBL_ENT261, PIP_BSEL_TBL_ENT262, PIP_BSEL_TBL_ENT263, PIP_BSEL_TBL_ENT264, PIP_BSEL_TBL_ENT265, PIP_BSEL_TBL_ENT266, PIP_BSEL_TBL_ENT267, PIP_BSEL_TBL_ENT268, PIP_BSEL_TBL_ENT269, PIP_BSEL_TBL_ENT270, PIP_BSEL_TBL_ENT271, PIP_BSEL_TBL_ENT272, PIP_BSEL_TBL_ENT273, PIP_BSEL_TBL_ENT274, PIP_BSEL_TBL_ENT275, PIP_BSEL_TBL_ENT276, PIP_BSEL_TBL_ENT277, PIP_BSEL_TBL_ENT278, PIP_BSEL_TBL_ENT279, PIP_BSEL_TBL_ENT280, PIP_BSEL_TBL_ENT281, PIP_BSEL_TBL_ENT282, PIP_BSEL_TBL_ENT283, PIP_BSEL_TBL_ENT284, PIP_BSEL_TBL_ENT285
REG_WINDOW = PIP_BSEL_TBL_ENT286 PIP_BSEL_TBL_ENT286, PIP_BSEL_TBL_ENT287, PIP_BSEL_TBL_ENT288, PIP_BSEL_TBL_ENT289, PIP_BSEL_TBL_ENT290, PIP_BSEL_TBL_ENT291, PIP_BSEL_TBL_ENT292, PIP_BSEL_TBL_ENT293, PIP_BSEL_TBL_ENT294, PIP_BSEL_TBL_ENT295, PIP_BSEL_TBL_ENT296, PIP_BSEL_TBL_ENT297, PIP_BSEL_TBL_ENT298, PIP_BSEL_TBL_ENT299, PIP_BSEL_TBL_ENT300, PIP_BSEL_TBL_ENT301, PIP_BSEL_TBL_ENT302, PIP_BSEL_TBL_ENT303, PIP_BSEL_TBL_ENT304, PIP_BSEL_TBL_ENT305, PIP_BSEL_TBL_ENT306, PIP_BSEL_TBL_ENT307, PIP_BSEL_TBL_ENT308, PIP_BSEL_TBL_ENT309, PIP_BSEL_TBL_ENT310, PIP_BSEL_TBL_ENT311, PIP_BSEL_TBL_ENT312, PIP_BSEL_TBL_ENT313, PIP_BSEL_TBL_ENT314, PIP_BSEL_TBL_ENT315, PIP_BSEL_TBL_ENT316, PIP_BSEL_TBL_ENT317, PIP_BSEL_TBL_ENT318, PIP_BSEL_TBL_ENT319, PIP_BSEL_TBL_ENT320, PIP_BSEL_TBL_ENT321, PIP_BSEL_TBL_ENT322, PIP_BSEL_TBL_ENT323, PIP_BSEL_TBL_ENT324, PIP_BSEL_TBL_ENT325, PIP_BSEL_TBL_ENT326, PIP_BSEL_TBL_ENT327, PIP_BSEL_TBL_ENT328, PIP_BSEL_TBL_ENT329, PIP_BSEL_TBL_ENT330, PIP_BSEL_TBL_ENT331, PIP_BSEL_TBL_ENT332, PIP_BSEL_TBL_ENT333, PIP_BSEL_TBL_ENT334, PIP_BSEL_TBL_ENT335
REG_WINDOW = PIP_BSEL_TBL_ENT336 PIP_BSEL_TBL_ENT336, PIP_BSEL_TBL_ENT337, PIP_BSEL_TBL_ENT338, PIP_BSEL_TBL_ENT339, PIP_BSEL_TBL_ENT340, PIP_BSEL_TBL_ENT341, PIP_BSEL_TBL_ENT342, PIP_BSEL_TBL_ENT343, PIP_BSEL_TBL_ENT344, PIP_BSEL_TBL_ENT345, PIP_BSEL_TBL_ENT346, PIP_BSEL_TBL_ENT347, PIP_BSEL_TBL_ENT348, PIP_BSEL_TBL_ENT349, PIP_BSEL_TBL_ENT350, PIP_BSEL_TBL_ENT351, PIP_BSEL_TBL_ENT352, PIP_BSEL_TBL_ENT353, PIP_BSEL_TBL_ENT354, PIP_BSEL_TBL_ENT355, PIP_BSEL_TBL_ENT356, PIP_BSEL_TBL_ENT357, PIP_BSEL_TBL_ENT358, PIP_BSEL_TBL_ENT359, PIP_BSEL_TBL_ENT360, PIP_BSEL_TBL_ENT361, PIP_BSEL_TBL_ENT362, PIP_BSEL_TBL_ENT363, PIP_BSEL_TBL_ENT364, PIP_BSEL_TBL_ENT365, PIP_BSEL_TBL_ENT366, PIP_BSEL_TBL_ENT367, PIP_BSEL_TBL_ENT368, PIP_BSEL_TBL_ENT369, PIP_BSEL_TBL_ENT370, PIP_BSEL_TBL_ENT371, PIP_BSEL_TBL_ENT372, PIP_BSEL_TBL_ENT373, PIP_BSEL_TBL_ENT374, PIP_BSEL_TBL_ENT375, PIP_BSEL_TBL_ENT376, PIP_BSEL_TBL_ENT377, PIP_BSEL_TBL_ENT378, PIP_BSEL_TBL_ENT379, PIP_BSEL_TBL_ENT380, PIP_BSEL_TBL_ENT381, PIP_BSEL_TBL_ENT382, PIP_BSEL_TBL_ENT383, PIP_BSEL_TBL_ENT384, PIP_BSEL_TBL_ENT385
REG_WINDOW = PIP_BSEL_TBL_ENT386 PIP_BSEL_TBL_ENT386, PIP_BSEL_TBL_ENT387, PIP_BSEL_TBL_ENT388, PIP_BSEL_TBL_ENT389, PIP_BSEL_TBL_ENT390, PIP_BSEL_TBL_ENT391, PIP_BSEL_TBL_ENT392, PIP_BSEL_TBL_ENT393, PIP_BSEL_TBL_ENT394, PIP_BSEL_TBL_ENT395, PIP_BSEL_TBL_ENT396, PIP_BSEL_TBL_ENT397, PIP_BSEL_TBL_ENT398, PIP_BSEL_TBL_ENT399, PIP_BSEL_TBL_ENT400, PIP_BSEL_TBL_ENT401, PIP_BSEL_TBL_ENT402, PIP_BSEL_TBL_ENT403, PIP_BSEL_TBL_ENT404, PIP_BSEL_TBL_ENT405, PIP_BSEL_TBL_ENT406, PIP_BSEL_TBL_ENT407, PIP_BSEL_TBL_ENT408, PIP_BSEL_TBL_ENT409, PIP_BSEL_TBL_ENT410, PIP_BSEL_TBL_ENT411, PIP_BSEL_TBL_ENT412, PIP_BSEL_TBL_ENT413, PIP_BSEL_TBL_ENT414, PIP_BSEL_TBL_ENT415, PIP_BSEL_TBL_ENT416, PIP_BSEL_TBL_ENT417, PIP_BSEL_TBL_ENT418, PIP_BSEL_TBL_ENT419, PIP_BSEL_TBL_ENT420, PIP_BSEL_TBL_ENT421, PIP_BSEL_TBL_ENT422, PIP_BSEL_TBL_ENT423, PIP_BSEL_TBL_ENT424, PIP_BSEL_TBL_ENT425, PIP_BSEL_TBL_ENT426, PIP_BSEL_TBL_ENT427, PIP_BSEL_TBL_ENT428, PIP_BSEL_TBL_ENT429, PIP_BSEL_TBL_ENT430, PIP_BSEL_TBL_ENT431, PIP_BSEL_TBL_ENT432, PIP_BSEL_TBL_ENT433, PIP_BSEL_TBL_ENT434, PIP_BSEL_TBL_ENT435
REG_WINDOW = PIP_BSEL_TBL_ENT436 PIP_BSEL_TBL_ENT436, PIP_BSEL_TBL_ENT437, PIP_BSEL_TBL_ENT438, PIP_BSEL_TBL_ENT439, PIP_BSEL_TBL_ENT440, PIP_BSEL_TBL_ENT441, PIP_BSEL_TBL_ENT442, PIP_BSEL_TBL_ENT443, PIP_BSEL_TBL_ENT444, PIP_BSEL_TBL_ENT445, PIP_BSEL_TBL_ENT446, PIP_BSEL_TBL_ENT447, PIP_BSEL_TBL_ENT448, PIP_BSEL_TBL_ENT449, PIP_BSEL_TBL_ENT450, PIP_BSEL_TBL_ENT451, PIP_BSEL_TBL_ENT452, PIP_BSEL_TBL_ENT453, PIP_BSEL_TBL_ENT454, PIP_BSEL_TBL_ENT455, PIP_BSEL_TBL_ENT456, PIP_BSEL_TBL_ENT457, PIP_BSEL_TBL_ENT458, PIP_BSEL_TBL_ENT459, PIP_BSEL_TBL_ENT460, PIP_BSEL_TBL_ENT461, PIP_BSEL_TBL_ENT462, PIP_BSEL_TBL_ENT463, PIP_BSEL_TBL_ENT464, PIP_BSEL_TBL_ENT465, PIP_BSEL_TBL_ENT466, PIP_BSEL_TBL_ENT467, PIP_BSEL_TBL_ENT468, PIP_BSEL_TBL_ENT469, PIP_BSEL_TBL_ENT470, PIP_BSEL_TBL_ENT471, PIP_BSEL_TBL_ENT472, PIP_BSEL_TBL_ENT473, PIP_BSEL_TBL_ENT474, PIP_BSEL_TBL_ENT475, PIP_BSEL_TBL_ENT476, PIP_BSEL_TBL_ENT477, PIP_BSEL_TBL_ENT478, PIP_BSEL_TBL_ENT479, PIP_BSEL_TBL_ENT480, PIP_BSEL_TBL_ENT481, PIP_BSEL_TBL_ENT482, PIP_BSEL_TBL_ENT483, PIP_BSEL_TBL_ENT484, PIP_BSEL_TBL_ENT485
REG_WINDOW = PIP_BSEL_TBL_ENT486 PIP_BSEL_TBL_ENT486, PIP_BSEL_TBL_ENT487, PIP_BSEL_TBL_ENT488, PIP_BSEL_TBL_ENT489, PIP_BSEL_TBL_ENT490, PIP_BSEL_TBL_ENT491, PIP_BSEL_TBL_ENT492, PIP_BSEL_TBL_ENT493, PIP_BSEL_TBL_ENT494, PIP_BSEL_TBL_ENT495, PIP_BSEL_TBL_ENT496, PIP_BSEL_TBL_ENT497, PIP_BSEL_TBL_ENT498, PIP_BSEL_TBL_ENT499, PIP_BSEL_TBL_ENT500, PIP_BSEL_TBL_ENT501, PIP_BSEL_TBL_ENT502, PIP_BSEL_TBL_ENT503, PIP_BSEL_TBL_ENT504, PIP_BSEL_TBL_ENT505, PIP_BSEL_TBL_ENT506, PIP_BSEL_TBL_ENT507, PIP_BSEL_TBL_ENT508, PIP_BSEL_TBL_ENT509, PIP_BSEL_TBL_ENT510, PIP_BSEL_TBL_ENT511, PIP_CLKEN, PIP_DEC_IPSEC0, PIP_DEC_IPSEC1, PIP_DEC_IPSEC2, PIP_DEC_IPSEC3, PIP_DSA_SRC_GRP, PIP_DSA_VID_GRP, PIP_FRM_LEN_CHK0, PIP_GBL_CFG, PIP_GBL_CTL, PIP_HG_PRI_QOS, PIP_INT_EN, PIP_INT_REG, PIP_IP_OFFSET, PIP_PRT_CFG0, PIP_PRT_CFG1, PIP_PRT_CFG2, PIP_PRT_CFG3, PIP_PRT_CFG16, PIP_PRT_CFG17, PIP_PRT_CFG18, PIP_PRT_CFG19, PIP_PRT_CFG24, PIP_PRT_CFG32
REG_WINDOW = PIP_PRT_CFG33 PIP_PRT_CFG33, PIP_PRT_CFG34, PIP_PRT_CFG35, PIP_PRT_CFG36, PIP_PRT_CFG37, PIP_PRT_CFG38, PIP_PRT_CFG39, PIP_PRT_CFGB0, PIP_PRT_CFGB1, PIP_PRT_CFGB2, PIP_PRT_CFGB3, PIP_PRT_CFGB16, PIP_PRT_CFGB17, PIP_PRT_CFGB18, PIP_PRT_CFGB19, PIP_PRT_CFGB24, PIP_PRT_CFGB32, PIP_PRT_CFGB33, PIP_PRT_CFGB34, PIP_PRT_CFGB35, PIP_PRT_CFGB36, PIP_PRT_CFGB37, PIP_PRT_CFGB38, PIP_PRT_CFGB39, PIP_PRT_TAG0, PIP_PRT_TAG1, PIP_PRT_TAG2, PIP_PRT_TAG3, PIP_PRT_TAG16, PIP_PRT_TAG17, PIP_PRT_TAG18, PIP_PRT_TAG19, PIP_PRT_TAG24, PIP_PRT_TAG32, PIP_PRT_TAG33, PIP_PRT_TAG34, PIP_PRT_TAG35, PIP_PRT_TAG36, PIP_PRT_TAG37, PIP_PRT_TAG38, PIP_PRT_TAG39, PIP_QOS_DIFF0, PIP_QOS_DIFF1, PIP_QOS_DIFF2, PIP_QOS_DIFF3, PIP_QOS_DIFF4, PIP_QOS_DIFF5, PIP_QOS_DIFF6, PIP_QOS_DIFF7, PIP_QOS_DIFF8
REG_WINDOW = PIP_QOS_DIFF9 PIP_QOS_DIFF9, PIP_QOS_DIFF10, PIP_QOS_DIFF11, PIP_QOS_DIFF12, PIP_QOS_DIFF13, PIP_QOS_DIFF14, PIP_QOS_DIFF15, PIP_QOS_DIFF16, PIP_QOS_DIFF17, PIP_QOS_DIFF18, PIP_QOS_DIFF19, PIP_QOS_DIFF20, PIP_QOS_DIFF21, PIP_QOS_DIFF22, PIP_QOS_DIFF23, PIP_QOS_DIFF24, PIP_QOS_DIFF25, PIP_QOS_DIFF26, PIP_QOS_DIFF27, PIP_QOS_DIFF28, PIP_QOS_DIFF29, PIP_QOS_DIFF30, PIP_QOS_DIFF31, PIP_QOS_DIFF32, PIP_QOS_DIFF33, PIP_QOS_DIFF34, PIP_QOS_DIFF35, PIP_QOS_DIFF36, PIP_QOS_DIFF37, PIP_QOS_DIFF38, PIP_QOS_DIFF39, PIP_QOS_DIFF40, PIP_QOS_DIFF41, PIP_QOS_DIFF42, PIP_QOS_DIFF43, PIP_QOS_DIFF44, PIP_QOS_DIFF45, PIP_QOS_DIFF46, PIP_QOS_DIFF47, PIP_QOS_DIFF48, PIP_QOS_DIFF49, PIP_QOS_DIFF50, PIP_QOS_DIFF51, PIP_QOS_DIFF52, PIP_QOS_DIFF53, PIP_QOS_DIFF54, PIP_QOS_DIFF55, PIP_QOS_DIFF56, PIP_QOS_DIFF57, PIP_QOS_DIFF58
REG_WINDOW = PIP_QOS_DIFF59 PIP_QOS_DIFF59, PIP_QOS_DIFF60, PIP_QOS_DIFF61, PIP_QOS_DIFF62, PIP_QOS_DIFF63, PIP_QOS_VLAN0, PIP_QOS_VLAN1, PIP_QOS_VLAN2, PIP_QOS_VLAN3, PIP_QOS_VLAN4, PIP_QOS_VLAN5, PIP_QOS_VLAN6, PIP_QOS_VLAN7, PIP_QOS_WATCH0, PIP_QOS_WATCH1, PIP_QOS_WATCH2, PIP_QOS_WATCH3, PIP_QOS_WATCH4, PIP_QOS_WATCH5, PIP_QOS_WATCH6, PIP_QOS_WATCH7, PIP_RAW_WORD, PIP_SFT_RST, PIP_STAT0_PRT0, PIP_STAT0_PRT1, PIP_STAT0_PRT2, PIP_STAT0_PRT3, PIP_STAT0_PRT16, PIP_STAT0_PRT17, PIP_STAT0_PRT18, PIP_STAT0_PRT19, PIP_STAT0_PRT24, PIP_STAT0_PRT32, PIP_STAT0_PRT33, PIP_STAT0_PRT34, PIP_STAT0_PRT35, PIP_STAT0_PRT36, PIP_STAT0_PRT37, PIP_STAT0_PRT38, PIP_STAT0_PRT39, PIP_STAT10_PRT0, PIP_STAT10_PRT1, PIP_STAT10_PRT2, PIP_STAT10_PRT3, PIP_STAT10_PRT16, PIP_STAT10_PRT17, PIP_STAT10_PRT18, PIP_STAT10_PRT19, PIP_STAT10_PRT24, PIP_STAT10_PRT32
REG_WINDOW = PIP_STAT10_PRT33 PIP_STAT10_PRT33, PIP_STAT10_PRT34, PIP_STAT10_PRT35, PIP_STAT10_PRT36, PIP_STAT10_PRT37, PIP_STAT10_PRT38, PIP_STAT10_PRT39, PIP_STAT11_PRT0, PIP_STAT11_PRT1, PIP_STAT11_PRT2, PIP_STAT11_PRT3, PIP_STAT11_PRT16, PIP_STAT11_PRT17, PIP_STAT11_PRT18, PIP_STAT11_PRT19, PIP_STAT11_PRT24, PIP_STAT11_PRT32, PIP_STAT11_PRT33, PIP_STAT11_PRT34, PIP_STAT11_PRT35, PIP_STAT11_PRT36, PIP_STAT11_PRT37, PIP_STAT11_PRT38, PIP_STAT11_PRT39, PIP_STAT1_PRT0, PIP_STAT1_PRT1, PIP_STAT1_PRT2, PIP_STAT1_PRT3, PIP_STAT1_PRT16, PIP_STAT1_PRT17, PIP_STAT1_PRT18, PIP_STAT1_PRT19, PIP_STAT1_PRT24, PIP_STAT1_PRT32, PIP_STAT1_PRT33, PIP_STAT1_PRT34, PIP_STAT1_PRT35, PIP_STAT1_PRT36, PIP_STAT1_PRT37, PIP_STAT1_PRT38, PIP_STAT1_PRT39, PIP_STAT2_PRT0, PIP_STAT2_PRT1, PIP_STAT2_PRT2, PIP_STAT2_PRT3, PIP_STAT2_PRT16, PIP_STAT2_PRT17, PIP_STAT2_PRT18, PIP_STAT2_PRT19, PIP_STAT2_PRT24
REG_WINDOW = PIP_STAT2_PRT32 PIP_STAT2_PRT32, PIP_STAT2_PRT33, PIP_STAT2_PRT34, PIP_STAT2_PRT35, PIP_STAT2_PRT36, PIP_STAT2_PRT37, PIP_STAT2_PRT38, PIP_STAT2_PRT39, PIP_STAT3_PRT0, PIP_STAT3_PRT1, PIP_STAT3_PRT2, PIP_STAT3_PRT3, PIP_STAT3_PRT16, PIP_STAT3_PRT17, PIP_STAT3_PRT18, PIP_STAT3_PRT19, PIP_STAT3_PRT24, PIP_STAT3_PRT32, PIP_STAT3_PRT33, PIP_STAT3_PRT34, PIP_STAT3_PRT35, PIP_STAT3_PRT36, PIP_STAT3_PRT37, PIP_STAT3_PRT38, PIP_STAT3_PRT39, PIP_STAT4_PRT0, PIP_STAT4_PRT1, PIP_STAT4_PRT2, PIP_STAT4_PRT3, PIP_STAT4_PRT16, PIP_STAT4_PRT17, PIP_STAT4_PRT18, PIP_STAT4_PRT19, PIP_STAT4_PRT24, PIP_STAT4_PRT32, PIP_STAT4_PRT33, PIP_STAT4_PRT34, PIP_STAT4_PRT35, PIP_STAT4_PRT36, PIP_STAT4_PRT37, PIP_STAT4_PRT38, PIP_STAT4_PRT39, PIP_STAT5_PRT0, PIP_STAT5_PRT1, PIP_STAT5_PRT2, PIP_STAT5_PRT3, PIP_STAT5_PRT16, PIP_STAT5_PRT17, PIP_STAT5_PRT18, PIP_STAT5_PRT19
REG_WINDOW = PIP_STAT5_PRT24 PIP_STAT5_PRT24, PIP_STAT5_PRT32, PIP_STAT5_PRT33, PIP_STAT5_PRT34, PIP_STAT5_PRT35, PIP_STAT5_PRT36, PIP_STAT5_PRT37, PIP_STAT5_PRT38, PIP_STAT5_PRT39, PIP_STAT6_PRT0, PIP_STAT6_PRT1, PIP_STAT6_PRT2, PIP_STAT6_PRT3, PIP_STAT6_PRT16, PIP_STAT6_PRT17, PIP_STAT6_PRT18, PIP_STAT6_PRT19, PIP_STAT6_PRT24, PIP_STAT6_PRT32, PIP_STAT6_PRT33, PIP_STAT6_PRT34, PIP_STAT6_PRT35, PIP_STAT6_PRT36, PIP_STAT6_PRT37, PIP_STAT6_PRT38, PIP_STAT6_PRT39, PIP_STAT7_PRT0, PIP_STAT7_PRT1, PIP_STAT7_PRT2, PIP_STAT7_PRT3, PIP_STAT7_PRT16, PIP_STAT7_PRT17, PIP_STAT7_PRT18, PIP_STAT7_PRT19, PIP_STAT7_PRT24, PIP_STAT7_PRT32, PIP_STAT7_PRT33, PIP_STAT7_PRT34, PIP_STAT7_PRT35, PIP_STAT7_PRT36, PIP_STAT7_PRT37, PIP_STAT7_PRT38, PIP_STAT7_PRT39, PIP_STAT8_PRT0, PIP_STAT8_PRT1, PIP_STAT8_PRT2, PIP_STAT8_PRT3, PIP_STAT8_PRT16, PIP_STAT8_PRT17, PIP_STAT8_PRT18
REG_WINDOW = PIP_STAT8_PRT19 PIP_STAT8_PRT19, PIP_STAT8_PRT24, PIP_STAT8_PRT32, PIP_STAT8_PRT33, PIP_STAT8_PRT34, PIP_STAT8_PRT35, PIP_STAT8_PRT36, PIP_STAT8_PRT37, PIP_STAT8_PRT38, PIP_STAT8_PRT39, PIP_STAT9_PRT0, PIP_STAT9_PRT1, PIP_STAT9_PRT2, PIP_STAT9_PRT3, PIP_STAT9_PRT16, PIP_STAT9_PRT17, PIP_STAT9_PRT18, PIP_STAT9_PRT19, PIP_STAT9_PRT24, PIP_STAT9_PRT32, PIP_STAT9_PRT33, PIP_STAT9_PRT34, PIP_STAT9_PRT35, PIP_STAT9_PRT36, PIP_STAT9_PRT37, PIP_STAT9_PRT38, PIP_STAT9_PRT39, PIP_STAT_CTL, PIP_STAT_INB_ERRS0, PIP_STAT_INB_ERRS1, PIP_STAT_INB_ERRS2, PIP_STAT_INB_ERRS3, PIP_STAT_INB_ERRS16, PIP_STAT_INB_ERRS17, PIP_STAT_INB_ERRS18, PIP_STAT_INB_ERRS19, PIP_STAT_INB_ERRS24, PIP_STAT_INB_ERRS32, PIP_STAT_INB_ERRS33, PIP_STAT_INB_ERRS34, PIP_STAT_INB_ERRS35, PIP_STAT_INB_ERRS36, PIP_STAT_INB_ERRS37, PIP_STAT_INB_ERRS38, PIP_STAT_INB_ERRS39, PIP_STAT_INB_OCTS0, PIP_STAT_INB_OCTS1, PIP_STAT_INB_OCTS2, PIP_STAT_INB_OCTS3, PIP_STAT_INB_OCTS16
REG_WINDOW = PIP_STAT_INB_OCTS17 PIP_STAT_INB_OCTS17, PIP_STAT_INB_OCTS18, PIP_STAT_INB_OCTS19, PIP_STAT_INB_OCTS24, PIP_STAT_INB_OCTS32, PIP_STAT_INB_OCTS33, PIP_STAT_INB_OCTS34, PIP_STAT_INB_OCTS35, PIP_STAT_INB_OCTS36, PIP_STAT_INB_OCTS37, PIP_STAT_INB_OCTS38, PIP_STAT_INB_OCTS39, PIP_STAT_INB_PKTS0, PIP_STAT_INB_PKTS1, PIP_STAT_INB_PKTS2, PIP_STAT_INB_PKTS3, PIP_STAT_INB_PKTS16, PIP_STAT_INB_PKTS17, PIP_STAT_INB_PKTS18, PIP_STAT_INB_PKTS19, PIP_STAT_INB_PKTS24, PIP_STAT_INB_PKTS32, PIP_STAT_INB_PKTS33, PIP_STAT_INB_PKTS34, PIP_STAT_INB_PKTS35, PIP_STAT_INB_PKTS36, PIP_STAT_INB_PKTS37, PIP_STAT_INB_PKTS38, PIP_STAT_INB_PKTS39, PIP_TAG_INC0, PIP_TAG_INC1, PIP_TAG_INC2, PIP_TAG_INC3, PIP_TAG_INC4, PIP_TAG_INC5, PIP_TAG_INC6, PIP_TAG_INC7, PIP_TAG_INC8, PIP_TAG_INC9, PIP_TAG_INC10, PIP_TAG_INC11, PIP_TAG_INC12, PIP_TAG_INC13, PIP_TAG_INC14, PIP_TAG_INC15, PIP_TAG_INC16, PIP_TAG_INC17, PIP_TAG_INC18, PIP_TAG_INC19, PIP_TAG_INC20
REG_WINDOW = PIP_TAG_INC21 PIP_TAG_INC21, PIP_TAG_INC22, PIP_TAG_INC23, PIP_TAG_INC24, PIP_TAG_INC25, PIP_TAG_INC26, PIP_TAG_INC27, PIP_TAG_INC28, PIP_TAG_INC29, PIP_TAG_INC30, PIP_TAG_INC31, PIP_TAG_INC32, PIP_TAG_INC33, PIP_TAG_INC34, PIP_TAG_INC35, PIP_TAG_INC36, PIP_TAG_INC37, PIP_TAG_INC38, PIP_TAG_INC39, PIP_TAG_INC40, PIP_TAG_INC41, PIP_TAG_INC42, PIP_TAG_INC43, PIP_TAG_INC44, PIP_TAG_INC45, PIP_TAG_INC46, PIP_TAG_INC47, PIP_TAG_INC48, PIP_TAG_INC49, PIP_TAG_INC50, PIP_TAG_INC51, PIP_TAG_INC52, PIP_TAG_INC53, PIP_TAG_INC54, PIP_TAG_INC55, PIP_TAG_INC56, PIP_TAG_INC57, PIP_TAG_INC58, PIP_TAG_INC59, PIP_TAG_INC60, PIP_TAG_INC61, PIP_TAG_INC62, PIP_TAG_INC63, PIP_TAG_MASK, PIP_TAG_SECRET, PIP_TODO_ENTRY, PIP_VLAN_ETYPES0, PIP_VLAN_ETYPES1
REG_WINDOW = PKO PKO_MEM_COUNT0, PKO_MEM_COUNT1, PKO_MEM_DEBUG0, PKO_MEM_DEBUG1, PKO_MEM_DEBUG10, PKO_MEM_DEBUG11, PKO_MEM_DEBUG12, PKO_MEM_DEBUG13, PKO_MEM_DEBUG14, PKO_MEM_DEBUG2, PKO_MEM_DEBUG3, PKO_MEM_DEBUG4, PKO_MEM_DEBUG5, PKO_MEM_DEBUG6, PKO_MEM_DEBUG7, PKO_MEM_DEBUG8, PKO_MEM_DEBUG9, PKO_MEM_PORT_PTRS, PKO_MEM_PORT_QOS, PKO_MEM_PORT_RATE0, PKO_MEM_PORT_RATE1, PKO_MEM_QUEUE_PTRS, PKO_MEM_QUEUE_QOS, PKO_REG_BIST_RESULT, PKO_REG_CMD_BUF, PKO_REG_DEBUG0, PKO_REG_DEBUG1, PKO_REG_DEBUG2, PKO_REG_DEBUG3, PKO_REG_ENGINE_INFLIGHT, PKO_REG_ENGINE_THRESH, PKO_REG_ERROR, PKO_REG_FLAGS, PKO_REG_GMX_PORT_MODE, PKO_REG_INT_MASK, PKO_REG_PREEMPT, PKO_REG_QUEUE_MODE, PKO_REG_QUEUE_PREEMPT, PKO_REG_QUEUE_PTRS1, PKO_REG_READ_IDX, PKO_REG_TIMESTAMP
REG_WINDOW = POW_BIST_STAT POW_BIST_STAT, POW_DS_PC, POW_ECC_ERR, POW_INT_CTL, POW_IQ_CNT0, POW_IQ_CNT1, POW_IQ_CNT2, POW_IQ_CNT3, POW_IQ_CNT4, POW_IQ_CNT5, POW_IQ_CNT6, POW_IQ_CNT7, POW_IQ_COM_CNT, POW_IQ_INT, POW_IQ_INT_EN, POW_IQ_THR0, POW_IQ_THR1, POW_IQ_THR2, POW_IQ_THR3, POW_IQ_THR4, POW_IQ_THR5, POW_IQ_THR6, POW_IQ_THR7, POW_NOS_CNT, POW_NW_TIM, POW_PF_RST_MSK, POW_PP_GRP_MSK0, POW_PP_GRP_MSK1, POW_PP_GRP_MSK2, POW_PP_GRP_MSK3, POW_QOS_RND0, POW_QOS_RND1, POW_QOS_RND2, POW_QOS_RND3, POW_QOS_RND4, POW_QOS_RND5, POW_QOS_RND6, POW_QOS_RND7, POW_QOS_THR0, POW_QOS_THR1, POW_QOS_THR2, POW_QOS_THR3, POW_QOS_THR4, POW_QOS_THR5, POW_QOS_THR6, POW_QOS_THR7, POW_TS_PC, POW_WA_COM_PC, POW_WA_PC0, POW_WA_PC1
REG_WINDOW = POW_WA_PC2 POW_WA_PC2, POW_WA_PC3, POW_WA_PC4, POW_WA_PC5, POW_WA_PC6, POW_WA_PC7, POW_WQ_INT, POW_WQ_INT_CNT0, POW_WQ_INT_CNT1, POW_WQ_INT_CNT2, POW_WQ_INT_CNT3, POW_WQ_INT_CNT4, POW_WQ_INT_CNT5, POW_WQ_INT_CNT6, POW_WQ_INT_CNT7, POW_WQ_INT_CNT8, POW_WQ_INT_CNT9, POW_WQ_INT_CNT10, POW_WQ_INT_CNT11, POW_WQ_INT_CNT12, POW_WQ_INT_CNT13, POW_WQ_INT_CNT14, POW_WQ_INT_CNT15, POW_WQ_INT_PC, POW_WQ_INT_THR0, POW_WQ_INT_THR1, POW_WQ_INT_THR2, POW_WQ_INT_THR3, POW_WQ_INT_THR4, POW_WQ_INT_THR5, POW_WQ_INT_THR6, POW_WQ_INT_THR7, POW_WQ_INT_THR8, POW_WQ_INT_THR9, POW_WQ_INT_THR10, POW_WQ_INT_THR11, POW_WQ_INT_THR12, POW_WQ_INT_THR13, POW_WQ_INT_THR14, POW_WQ_INT_THR15, POW_WS_PC0, POW_WS_PC1, POW_WS_PC2, POW_WS_PC3, POW_WS_PC4, POW_WS_PC5, POW_WS_PC6, POW_WS_PC7, POW_WS_PC8, POW_WS_PC9
REG_WINDOW = POW_WS_PC10 POW_WS_PC10, POW_WS_PC11, POW_WS_PC12, POW_WS_PC13, POW_WS_PC14, POW_WS_PC15
REG_WINDOW = RAD RAD_MEM_DEBUG0, RAD_MEM_DEBUG1, RAD_MEM_DEBUG2, RAD_REG_BIST_RESULT, RAD_REG_CMD_BUF, RAD_REG_CTL, RAD_REG_DEBUG0, RAD_REG_DEBUG1, RAD_REG_DEBUG10, RAD_REG_DEBUG11, RAD_REG_DEBUG12, RAD_REG_DEBUG2, RAD_REG_DEBUG3, RAD_REG_DEBUG4, RAD_REG_DEBUG5, RAD_REG_DEBUG6, RAD_REG_DEBUG7, RAD_REG_DEBUG8, RAD_REG_DEBUG9, RAD_REG_ERROR, RAD_REG_INT_MASK, RAD_REG_POLYNOMIAL, RAD_REG_READ_IDX
REG_WINDOW = RNM RNM_BIST_STATUS, RNM_CTL_STATUS, RNM_EER_DBG, RNM_EER_KEY, RNM_SERIAL_NUM
REG_WINDOW = RST RST_BOOT, RST_CFG, RST_CKILL, RST_CTL0, RST_CTL1, RST_CTL2, RST_DELAY, RST_INT, RST_PP_POWER, RST_SOFT_PRST0, RST_SOFT_PRST1, RST_SOFT_PRST2, RST_SOFT_RST
REG_WINDOW = SATA_UAHC_GBL_BISTAFR SATA_UAHC_GBL_BISTAFR, SATA_UAHC_GBL_BISTCR, SATA_UAHC_GBL_BISTDECR, SATA_UAHC_GBL_BISTFCTR, SATA_UAHC_GBL_BISTSR, SATA_UAHC_GBL_CAP, SATA_UAHC_GBL_CAP2, SATA_UAHC_GBL_CCC_CTL, SATA_UAHC_GBL_CCC_PORTS, SATA_UAHC_GBL_GHC, SATA_UAHC_GBL_GPARAM1R, SATA_UAHC_GBL_GPARAM2R, SATA_UAHC_GBL_IDR, SATA_UAHC_GBL_IS, SATA_UAHC_GBL_OOBR, SATA_UAHC_GBL_PI, SATA_UAHC_GBL_PPARAMR, SATA_UAHC_GBL_TESTR, SATA_UAHC_GBL_TIMER1MS, SATA_UAHC_GBL_VERSIONR, SATA_UAHC_GBL_VS, SATA_UAHC_P0_CI, SATA_UAHC_P1_CI, SATA_UAHC_P0_CLB, SATA_UAHC_P1_CLB, SATA_UAHC_P0_CMD, SATA_UAHC_P1_CMD, SATA_UAHC_P0_DMACR, SATA_UAHC_P1_DMACR, SATA_UAHC_P0_FB, SATA_UAHC_P1_FB, SATA_UAHC_P0_FBS, SATA_UAHC_P1_FBS, SATA_UAHC_P0_IE, SATA_UAHC_P1_IE, SATA_UAHC_P0_IS, SATA_UAHC_P1_IS, SATA_UAHC_P0_PHYCR, SATA_UAHC_P1_PHYCR, SATA_UAHC_P0_PHYSR, SATA_UAHC_P1_PHYSR, SATA_UAHC_P0_SACT, SATA_UAHC_P1_SACT, SATA_UAHC_P0_SCTL, SATA_UAHC_P1_SCTL, SATA_UAHC_P0_SERR, SATA_UAHC_P1_SERR, SATA_UAHC_P0_SIG, SATA_UAHC_P1_SIG, SATA_UAHC_P0_SNTF
REG_WINDOW = SATA_UAHC_P1_SNTF SATA_UAHC_P1_SNTF, SATA_UAHC_P0_SSTS, SATA_UAHC_P1_SSTS, SATA_UAHC_P0_TFD, SATA_UAHC_P1_TFD, SATA_UCTL_BIST_STATUS, SATA_UCTL_CTL, SATA_UCTL_INTSTAT, SATA_UCTL_SHIM_CFG, SATA_UCTL_SPARE0, SATA_UCTL_SPARE1
REG_WINDOW = SLI_BIST_STATUS SLI_BIST_STATUS, SLI_CTL_PORT0, SLI_CTL_PORT1, SLI_CTL_PORT2, SLI_CTL_STATUS, SLI_DATA_OUT_CNT, SLI_DMA0_CNT, SLI_DMA1_CNT, SLI_DMA0_INT_LEVEL, SLI_DMA1_INT_LEVEL, SLI_DMA0_TIM, SLI_DMA1_TIM, SLI_INT_ENB_CIU, SLI_INT_ENB_PORT0, SLI_INT_ENB_PORT1, SLI_INT_ENB_PORT2, SLI_INT_SUM, SLI_LAST_WIN_RDATA0, SLI_LAST_WIN_RDATA1, SLI_LAST_WIN_RDATA2, SLI_LAST_WIN_RDATA3, SLI_MAC_CREDIT_CNT, SLI_MAC_CREDIT_CNT2, SLI_MEM_ACCESS_CTL, SLI_MEM_ACCESS_SUBID12, SLI_MEM_ACCESS_SUBID13, SLI_MEM_ACCESS_SUBID14, SLI_MEM_ACCESS_SUBID15, SLI_MEM_ACCESS_SUBID16, SLI_MEM_ACCESS_SUBID17, SLI_MEM_ACCESS_SUBID18, SLI_MEM_ACCESS_SUBID19, SLI_MEM_ACCESS_SUBID20, SLI_MEM_ACCESS_SUBID21, SLI_MEM_ACCESS_SUBID22, SLI_MEM_ACCESS_SUBID23, SLI_MEM_ACCESS_SUBID24, SLI_MEM_ACCESS_SUBID25, SLI_MEM_ACCESS_SUBID26, SLI_MEM_ACCESS_SUBID27, SLI_MSI_ENB0, SLI_MSI_ENB1, SLI_MSI_ENB2, SLI_MSI_ENB3, SLI_MSI_RCV0, SLI_MSI_RCV1, SLI_MSI_RCV2, SLI_MSI_RCV3, SLI_MSI_RD_MAP, SLI_MSI_W1C_ENB0
REG_WINDOW = SLI_MSI_W1C_ENB1 SLI_MSI_W1C_ENB1, SLI_MSI_W1C_ENB2, SLI_MSI_W1C_ENB3, SLI_MSI_W1S_ENB0, SLI_MSI_W1S_ENB1, SLI_MSI_W1S_ENB2, SLI_MSI_W1S_ENB3, SLI_MSI_WR_MAP, SLI_PCIE_MSI_RCV, SLI_PCIE_MSI_RCV_B1, SLI_PCIE_MSI_RCV_B2, SLI_PCIE_MSI_RCV_B3, SLI_PKT0_CNTS, SLI_PKT1_CNTS, SLI_PKT2_CNTS, SLI_PKT3_CNTS, SLI_PKT4_CNTS, SLI_PKT5_CNTS, SLI_PKT6_CNTS, SLI_PKT7_CNTS, SLI_PKT8_CNTS, SLI_PKT9_CNTS, SLI_PKT10_CNTS, SLI_PKT11_CNTS, SLI_PKT12_CNTS, SLI_PKT13_CNTS, SLI_PKT14_CNTS, SLI_PKT15_CNTS, SLI_PKT16_CNTS, SLI_PKT17_CNTS, SLI_PKT18_CNTS, SLI_PKT19_CNTS, SLI_PKT20_CNTS, SLI_PKT21_CNTS, SLI_PKT22_CNTS, SLI_PKT23_CNTS, SLI_PKT24_CNTS, SLI_PKT25_CNTS, SLI_PKT26_CNTS, SLI_PKT27_CNTS, SLI_PKT28_CNTS, SLI_PKT29_CNTS, SLI_PKT30_CNTS, SLI_PKT31_CNTS, SLI_PKT0_IN_BP, SLI_PKT1_IN_BP, SLI_PKT2_IN_BP, SLI_PKT3_IN_BP, SLI_PKT4_IN_BP, SLI_PKT5_IN_BP
REG_WINDOW = SLI_PKT6_IN_BP SLI_PKT6_IN_BP, SLI_PKT7_IN_BP, SLI_PKT8_IN_BP, SLI_PKT9_IN_BP, SLI_PKT10_IN_BP, SLI_PKT11_IN_BP, SLI_PKT12_IN_BP, SLI_PKT13_IN_BP, SLI_PKT14_IN_BP, SLI_PKT15_IN_BP, SLI_PKT16_IN_BP, SLI_PKT17_IN_BP, SLI_PKT18_IN_BP, SLI_PKT19_IN_BP, SLI_PKT20_IN_BP, SLI_PKT21_IN_BP, SLI_PKT22_IN_BP, SLI_PKT23_IN_BP, SLI_PKT24_IN_BP, SLI_PKT25_IN_BP, SLI_PKT26_IN_BP, SLI_PKT27_IN_BP, SLI_PKT28_IN_BP, SLI_PKT29_IN_BP, SLI_PKT30_IN_BP, SLI_PKT31_IN_BP, SLI_PKT0_INSTR_BADDR, SLI_PKT1_INSTR_BADDR, SLI_PKT2_INSTR_BADDR, SLI_PKT3_INSTR_BADDR, SLI_PKT4_INSTR_BADDR, SLI_PKT5_INSTR_BADDR, SLI_PKT6_INSTR_BADDR, SLI_PKT7_INSTR_BADDR, SLI_PKT8_INSTR_BADDR, SLI_PKT9_INSTR_BADDR, SLI_PKT10_INSTR_BADDR, SLI_PKT11_INSTR_BADDR, SLI_PKT12_INSTR_BADDR, SLI_PKT13_INSTR_BADDR, SLI_PKT14_INSTR_BADDR, SLI_PKT15_INSTR_BADDR, SLI_PKT16_INSTR_BADDR, SLI_PKT17_INSTR_BADDR, SLI_PKT18_INSTR_BADDR, SLI_PKT19_INSTR_BADDR, SLI_PKT20_INSTR_BADDR, SLI_PKT21_INSTR_BADDR, SLI_PKT22_INSTR_BADDR, SLI_PKT23_INSTR_BADDR
REG_WINDOW = SLI_PKT24_INSTR_BADDR SLI_PKT24_INSTR_BADDR, SLI_PKT25_INSTR_BADDR, SLI_PKT26_INSTR_BADDR, SLI_PKT27_INSTR_BADDR, SLI_PKT28_INSTR_BADDR, SLI_PKT29_INSTR_BADDR, SLI_PKT30_INSTR_BADDR, SLI_PKT31_INSTR_BADDR, SLI_PKT0_INSTR_BAOFF_DBELL, SLI_PKT1_INSTR_BAOFF_DBELL, SLI_PKT2_INSTR_BAOFF_DBELL, SLI_PKT3_INSTR_BAOFF_DBELL, SLI_PKT4_INSTR_BAOFF_DBELL, SLI_PKT5_INSTR_BAOFF_DBELL, SLI_PKT6_INSTR_BAOFF_DBELL, SLI_PKT7_INSTR_BAOFF_DBELL, SLI_PKT8_INSTR_BAOFF_DBELL, SLI_PKT9_INSTR_BAOFF_DBELL, SLI_PKT10_INSTR_BAOFF_DBELL, SLI_PKT11_INSTR_BAOFF_DBELL, SLI_PKT12_INSTR_BAOFF_DBELL, SLI_PKT13_INSTR_BAOFF_DBELL, SLI_PKT14_INSTR_BAOFF_DBELL, SLI_PKT15_INSTR_BAOFF_DBELL, SLI_PKT16_INSTR_BAOFF_DBELL, SLI_PKT17_INSTR_BAOFF_DBELL, SLI_PKT18_INSTR_BAOFF_DBELL, SLI_PKT19_INSTR_BAOFF_DBELL, SLI_PKT20_INSTR_BAOFF_DBELL, SLI_PKT21_INSTR_BAOFF_DBELL, SLI_PKT22_INSTR_BAOFF_DBELL, SLI_PKT23_INSTR_BAOFF_DBELL, SLI_PKT24_INSTR_BAOFF_DBELL, SLI_PKT25_INSTR_BAOFF_DBELL, SLI_PKT26_INSTR_BAOFF_DBELL, SLI_PKT27_INSTR_BAOFF_DBELL, SLI_PKT28_INSTR_BAOFF_DBELL, SLI_PKT29_INSTR_BAOFF_DBELL, SLI_PKT30_INSTR_BAOFF_DBELL, SLI_PKT31_INSTR_BAOFF_DBELL, SLI_PKT0_INSTR_FIFO_RSIZE, SLI_PKT1_INSTR_FIFO_RSIZE, SLI_PKT2_INSTR_FIFO_RSIZE, SLI_PKT3_INSTR_FIFO_RSIZE, SLI_PKT4_INSTR_FIFO_RSIZE, SLI_PKT5_INSTR_FIFO_RSIZE, SLI_PKT6_INSTR_FIFO_RSIZE, SLI_PKT7_INSTR_FIFO_RSIZE, SLI_PKT8_INSTR_FIFO_RSIZE, SLI_PKT9_INSTR_FIFO_RSIZE
REG_WINDOW = SLI_PKT10_INSTR_FIFO_RSIZE SLI_PKT10_INSTR_FIFO_RSIZE, SLI_PKT11_INSTR_FIFO_RSIZE, SLI_PKT12_INSTR_FIFO_RSIZE, SLI_PKT13_INSTR_FIFO_RSIZE, SLI_PKT14_INSTR_FIFO_RSIZE, SLI_PKT15_INSTR_FIFO_RSIZE, SLI_PKT16_INSTR_FIFO_RSIZE, SLI_PKT17_INSTR_FIFO_RSIZE, SLI_PKT18_INSTR_FIFO_RSIZE, SLI_PKT19_INSTR_FIFO_RSIZE, SLI_PKT20_INSTR_FIFO_RSIZE, SLI_PKT21_INSTR_FIFO_RSIZE, SLI_PKT22_INSTR_FIFO_RSIZE, SLI_PKT23_INSTR_FIFO_RSIZE, SLI_PKT24_INSTR_FIFO_RSIZE, SLI_PKT25_INSTR_FIFO_RSIZE, SLI_PKT26_INSTR_FIFO_RSIZE, SLI_PKT27_INSTR_FIFO_RSIZE, SLI_PKT28_INSTR_FIFO_RSIZE, SLI_PKT29_INSTR_FIFO_RSIZE, SLI_PKT30_INSTR_FIFO_RSIZE, SLI_PKT31_INSTR_FIFO_RSIZE, SLI_PKT0_INSTR_HEADER, SLI_PKT1_INSTR_HEADER, SLI_PKT2_INSTR_HEADER, SLI_PKT3_INSTR_HEADER, SLI_PKT4_INSTR_HEADER, SLI_PKT5_INSTR_HEADER, SLI_PKT6_INSTR_HEADER, SLI_PKT7_INSTR_HEADER, SLI_PKT8_INSTR_HEADER, SLI_PKT9_INSTR_HEADER, SLI_PKT10_INSTR_HEADER, SLI_PKT11_INSTR_HEADER, SLI_PKT12_INSTR_HEADER, SLI_PKT13_INSTR_HEADER, SLI_PKT14_INSTR_HEADER, SLI_PKT15_INSTR_HEADER, SLI_PKT16_INSTR_HEADER, SLI_PKT17_INSTR_HEADER, SLI_PKT18_INSTR_HEADER, SLI_PKT19_INSTR_HEADER, SLI_PKT20_INSTR_HEADER, SLI_PKT21_INSTR_HEADER, SLI_PKT22_INSTR_HEADER, SLI_PKT23_INSTR_HEADER, SLI_PKT24_INSTR_HEADER, SLI_PKT25_INSTR_HEADER, SLI_PKT26_INSTR_HEADER, SLI_PKT27_INSTR_HEADER
REG_WINDOW = SLI_PKT28_INSTR_HEADER SLI_PKT28_INSTR_HEADER, SLI_PKT29_INSTR_HEADER, SLI_PKT30_INSTR_HEADER, SLI_PKT31_INSTR_HEADER, SLI_PKT0_OUT_SIZE, SLI_PKT1_OUT_SIZE, SLI_PKT2_OUT_SIZE, SLI_PKT3_OUT_SIZE, SLI_PKT4_OUT_SIZE, SLI_PKT5_OUT_SIZE, SLI_PKT6_OUT_SIZE, SLI_PKT7_OUT_SIZE, SLI_PKT8_OUT_SIZE, SLI_PKT9_OUT_SIZE, SLI_PKT10_OUT_SIZE, SLI_PKT11_OUT_SIZE, SLI_PKT12_OUT_SIZE, SLI_PKT13_OUT_SIZE, SLI_PKT14_OUT_SIZE, SLI_PKT15_OUT_SIZE, SLI_PKT16_OUT_SIZE, SLI_PKT17_OUT_SIZE, SLI_PKT18_OUT_SIZE, SLI_PKT19_OUT_SIZE, SLI_PKT20_OUT_SIZE, SLI_PKT21_OUT_SIZE, SLI_PKT22_OUT_SIZE, SLI_PKT23_OUT_SIZE, SLI_PKT24_OUT_SIZE, SLI_PKT25_OUT_SIZE, SLI_PKT26_OUT_SIZE, SLI_PKT27_OUT_SIZE, SLI_PKT28_OUT_SIZE, SLI_PKT29_OUT_SIZE, SLI_PKT30_OUT_SIZE, SLI_PKT31_OUT_SIZE, SLI_PKT0_SLIST_BADDR, SLI_PKT1_SLIST_BADDR, SLI_PKT2_SLIST_BADDR, SLI_PKT3_SLIST_BADDR, SLI_PKT4_SLIST_BADDR, SLI_PKT5_SLIST_BADDR, SLI_PKT6_SLIST_BADDR, SLI_PKT7_SLIST_BADDR, SLI_PKT8_SLIST_BADDR, SLI_PKT9_SLIST_BADDR, SLI_PKT10_SLIST_BADDR, SLI_PKT11_SLIST_BADDR, SLI_PKT12_SLIST_BADDR, SLI_PKT13_SLIST_BADDR
REG_WINDOW = SLI_PKT14_SLIST_BADDR SLI_PKT14_SLIST_BADDR, SLI_PKT15_SLIST_BADDR, SLI_PKT16_SLIST_BADDR, SLI_PKT17_SLIST_BADDR, SLI_PKT18_SLIST_BADDR, SLI_PKT19_SLIST_BADDR, SLI_PKT20_SLIST_BADDR, SLI_PKT21_SLIST_BADDR, SLI_PKT22_SLIST_BADDR, SLI_PKT23_SLIST_BADDR, SLI_PKT24_SLIST_BADDR, SLI_PKT25_SLIST_BADDR, SLI_PKT26_SLIST_BADDR, SLI_PKT27_SLIST_BADDR, SLI_PKT28_SLIST_BADDR, SLI_PKT29_SLIST_BADDR, SLI_PKT30_SLIST_BADDR, SLI_PKT31_SLIST_BADDR, SLI_PKT0_SLIST_BAOFF_DBELL, SLI_PKT1_SLIST_BAOFF_DBELL, SLI_PKT2_SLIST_BAOFF_DBELL, SLI_PKT3_SLIST_BAOFF_DBELL, SLI_PKT4_SLIST_BAOFF_DBELL, SLI_PKT5_SLIST_BAOFF_DBELL, SLI_PKT6_SLIST_BAOFF_DBELL, SLI_PKT7_SLIST_BAOFF_DBELL, SLI_PKT8_SLIST_BAOFF_DBELL, SLI_PKT9_SLIST_BAOFF_DBELL, SLI_PKT10_SLIST_BAOFF_DBELL, SLI_PKT11_SLIST_BAOFF_DBELL, SLI_PKT12_SLIST_BAOFF_DBELL, SLI_PKT13_SLIST_BAOFF_DBELL, SLI_PKT14_SLIST_BAOFF_DBELL, SLI_PKT15_SLIST_BAOFF_DBELL, SLI_PKT16_SLIST_BAOFF_DBELL, SLI_PKT17_SLIST_BAOFF_DBELL, SLI_PKT18_SLIST_BAOFF_DBELL, SLI_PKT19_SLIST_BAOFF_DBELL, SLI_PKT20_SLIST_BAOFF_DBELL, SLI_PKT21_SLIST_BAOFF_DBELL, SLI_PKT22_SLIST_BAOFF_DBELL, SLI_PKT23_SLIST_BAOFF_DBELL, SLI_PKT24_SLIST_BAOFF_DBELL, SLI_PKT25_SLIST_BAOFF_DBELL, SLI_PKT26_SLIST_BAOFF_DBELL, SLI_PKT27_SLIST_BAOFF_DBELL, SLI_PKT28_SLIST_BAOFF_DBELL, SLI_PKT29_SLIST_BAOFF_DBELL, SLI_PKT30_SLIST_BAOFF_DBELL, SLI_PKT31_SLIST_BAOFF_DBELL
REG_WINDOW = SLI_PKT0_SLIST_FIFO_RSIZE SLI_PKT0_SLIST_FIFO_RSIZE, SLI_PKT1_SLIST_FIFO_RSIZE, SLI_PKT2_SLIST_FIFO_RSIZE, SLI_PKT3_SLIST_FIFO_RSIZE, SLI_PKT4_SLIST_FIFO_RSIZE, SLI_PKT5_SLIST_FIFO_RSIZE, SLI_PKT6_SLIST_FIFO_RSIZE, SLI_PKT7_SLIST_FIFO_RSIZE, SLI_PKT8_SLIST_FIFO_RSIZE, SLI_PKT9_SLIST_FIFO_RSIZE, SLI_PKT10_SLIST_FIFO_RSIZE, SLI_PKT11_SLIST_FIFO_RSIZE, SLI_PKT12_SLIST_FIFO_RSIZE, SLI_PKT13_SLIST_FIFO_RSIZE, SLI_PKT14_SLIST_FIFO_RSIZE, SLI_PKT15_SLIST_FIFO_RSIZE, SLI_PKT16_SLIST_FIFO_RSIZE, SLI_PKT17_SLIST_FIFO_RSIZE, SLI_PKT18_SLIST_FIFO_RSIZE, SLI_PKT19_SLIST_FIFO_RSIZE, SLI_PKT20_SLIST_FIFO_RSIZE, SLI_PKT21_SLIST_FIFO_RSIZE, SLI_PKT22_SLIST_FIFO_RSIZE, SLI_PKT23_SLIST_FIFO_RSIZE, SLI_PKT24_SLIST_FIFO_RSIZE, SLI_PKT25_SLIST_FIFO_RSIZE, SLI_PKT26_SLIST_FIFO_RSIZE, SLI_PKT27_SLIST_FIFO_RSIZE, SLI_PKT28_SLIST_FIFO_RSIZE, SLI_PKT29_SLIST_FIFO_RSIZE, SLI_PKT30_SLIST_FIFO_RSIZE, SLI_PKT31_SLIST_FIFO_RSIZE, SLI_PKT_CNT_INT, SLI_PKT_CNT_INT_ENB, SLI_PKT_CTL, SLI_PKT_DATA_OUT_ES, SLI_PKT_DATA_OUT_NS, SLI_PKT_DATA_OUT_ROR, SLI_PKT_DPADDR, SLI_PKT_IN_BP, SLI_PKT_IN_DONE0_CNTS, SLI_PKT_IN_DONE1_CNTS, SLI_PKT_IN_DONE2_CNTS, SLI_PKT_IN_DONE3_CNTS, SLI_PKT_IN_DONE4_CNTS, SLI_PKT_IN_DONE5_CNTS, SLI_PKT_IN_DONE6_CNTS, SLI_PKT_IN_DONE7_CNTS, SLI_PKT_IN_DONE8_CNTS, SLI_PKT_IN_DONE9_CNTS
REG_WINDOW = SLI_PKT_IN_DONE10_CNTS SLI_PKT_IN_DONE10_CNTS, SLI_PKT_IN_DONE11_CNTS, SLI_PKT_IN_DONE12_CNTS, SLI_PKT_IN_DONE13_CNTS, SLI_PKT_IN_DONE14_CNTS, SLI_PKT_IN_DONE15_CNTS, SLI_PKT_IN_DONE16_CNTS, SLI_PKT_IN_DONE17_CNTS, SLI_PKT_IN_DONE18_CNTS, SLI_PKT_IN_DONE19_CNTS, SLI_PKT_IN_DONE20_CNTS, SLI_PKT_IN_DONE21_CNTS, SLI_PKT_IN_DONE22_CNTS, SLI_PKT_IN_DONE23_CNTS, SLI_PKT_IN_DONE24_CNTS, SLI_PKT_IN_DONE25_CNTS, SLI_PKT_IN_DONE26_CNTS, SLI_PKT_IN_DONE27_CNTS, SLI_PKT_IN_DONE28_CNTS, SLI_PKT_IN_DONE29_CNTS, SLI_PKT_IN_DONE30_CNTS, SLI_PKT_IN_DONE31_CNTS, SLI_PKT_IN_INSTR_COUNTS, SLI_PKT_IN_PCIE_PORT, SLI_PKT_INPUT_CONTROL, SLI_PKT_INSTR_ENB, SLI_PKT_INSTR_RD_SIZE, SLI_PKT_INSTR_SIZE, SLI_PKT_INT_LEVELS, SLI_PKT_IPTR, SLI_PKT_OUT_BMODE, SLI_PKT_OUT_ENB, SLI_PKT_OUTPUT_WMARK, SLI_PKT_PCIE_PORT, SLI_PKT_PORT_IN_RST, SLI_PKT_SLIST_ES, SLI_PKT_SLIST_NS, SLI_PKT_SLIST_ROR, SLI_PKT_TIME_INT, SLI_PKT_TIME_INT_ENB, SLI_S2M_PORT0_CTL, SLI_S2M_PORT1_CTL, SLI_S2M_PORT2_CTL, SLI_SCRATCH_1, SLI_SCRATCH_2, SLI_STATE1, SLI_STATE2, SLI_STATE3, SLI_WINDOW_CTL
REG_WINDOW = SMI SMI_DRV_CTL
REG_WINDOW = SMI0 SMI0_CLK, SMI0_CMD, SMI0_EN, SMI0_RD_DAT, SMI0_WR_DAT
REG_WINDOW = SMI1 SMI1_CLK, SMI1_CMD, SMI1_EN, SMI1_RD_DAT, SMI1_WR_DAT
REG_WINDOW = TIM TIM_MEM_DEBUG0, TIM_MEM_DEBUG1, TIM_MEM_DEBUG2, TIM_MEM_RING0, TIM_MEM_RING1, TIM_REG_BIST_RESULT, TIM_REG_ERROR, TIM_REG_FLAGS, TIM_REG_INT_MASK, TIM_REG_READ_IDX
REG_WINDOW = USBDRD0_UAHC_CAPLENGTH USBDRD0_UAHC_CAPLENGTH, USBDRD0_UAHC_CONFIG, USBDRD0_UAHC_CRCR, USBDRD0_UAHC_DALEPENA, USBDRD0_UAHC_DB000, USBDRD0_UAHC_DB001, USBDRD0_UAHC_DB002, USBDRD0_UAHC_DB003, USBDRD0_UAHC_DB004, USBDRD0_UAHC_DB005, USBDRD0_UAHC_DB006, USBDRD0_UAHC_DB007, USBDRD0_UAHC_DB008, USBDRD0_UAHC_DB009, USBDRD0_UAHC_DB010, USBDRD0_UAHC_DB011, USBDRD0_UAHC_DB012, USBDRD0_UAHC_DB013, USBDRD0_UAHC_DB014, USBDRD0_UAHC_DB015, USBDRD0_UAHC_DB016, USBDRD0_UAHC_DB017, USBDRD0_UAHC_DB018, USBDRD0_UAHC_DB019, USBDRD0_UAHC_DB020, USBDRD0_UAHC_DB021, USBDRD0_UAHC_DB022, USBDRD0_UAHC_DB023, USBDRD0_UAHC_DB024, USBDRD0_UAHC_DB025, USBDRD0_UAHC_DB026, USBDRD0_UAHC_DB027, USBDRD0_UAHC_DB028, USBDRD0_UAHC_DB029, USBDRD0_UAHC_DB030, USBDRD0_UAHC_DB031, USBDRD0_UAHC_DB032, USBDRD0_UAHC_DB033, USBDRD0_UAHC_DB034, USBDRD0_UAHC_DB035, USBDRD0_UAHC_DB036, USBDRD0_UAHC_DB037, USBDRD0_UAHC_DB038, USBDRD0_UAHC_DB039, USBDRD0_UAHC_DB040, USBDRD0_UAHC_DB041, USBDRD0_UAHC_DB042, USBDRD0_UAHC_DB043, USBDRD0_UAHC_DB044, USBDRD0_UAHC_DB045
REG_WINDOW = USBDRD0_UAHC_DB046 USBDRD0_UAHC_DB046, USBDRD0_UAHC_DB047, USBDRD0_UAHC_DB048, USBDRD0_UAHC_DB049, USBDRD0_UAHC_DB050, USBDRD0_UAHC_DB051, USBDRD0_UAHC_DB052, USBDRD0_UAHC_DB053, USBDRD0_UAHC_DB054, USBDRD0_UAHC_DB055, USBDRD0_UAHC_DB056, USBDRD0_UAHC_DB057, USBDRD0_UAHC_DB058, USBDRD0_UAHC_DB059, USBDRD0_UAHC_DB060, USBDRD0_UAHC_DB061, USBDRD0_UAHC_DB062, USBDRD0_UAHC_DB063, USBDRD0_UAHC_DB064, USBDRD0_UAHC_DBOFF, USBDRD0_UAHC_DCBAAP, USBDRD0_UAHC_DCFG, USBDRD0_UAHC_DCTL, USBDRD0_UAHC_DEPCMD000, USBDRD0_UAHC_DEPCMD001, USBDRD0_UAHC_DEPCMD002, USBDRD0_UAHC_DEPCMD003, USBDRD0_UAHC_DEPCMD004, USBDRD0_UAHC_DEPCMD005, USBDRD0_UAHC_DEPCMD006, USBDRD0_UAHC_DEPCMD007, USBDRD0_UAHC_DEPCMD008, USBDRD0_UAHC_DEPCMD009, USBDRD0_UAHC_DEPCMD010, USBDRD0_UAHC_DEPCMD011, USBDRD0_UAHC_DEPCMD012, USBDRD0_UAHC_DEPCMD013, USBDRD0_UAHC_DEPCMD014, USBDRD0_UAHC_DEPCMD015, USBDRD0_UAHC_DEPCMDPAR0_000, USBDRD0_UAHC_DEPCMDPAR0_001, USBDRD0_UAHC_DEPCMDPAR0_002, USBDRD0_UAHC_DEPCMDPAR0_003, USBDRD0_UAHC_DEPCMDPAR0_004, USBDRD0_UAHC_DEPCMDPAR0_005, USBDRD0_UAHC_DEPCMDPAR0_006, USBDRD0_UAHC_DEPCMDPAR0_007, USBDRD0_UAHC_DEPCMDPAR0_008, USBDRD0_UAHC_DEPCMDPAR0_009, USBDRD0_UAHC_DEPCMDPAR0_010
REG_WINDOW = USBDRD0_UAHC_DEPCMDPAR0_011 USBDRD0_UAHC_DEPCMDPAR0_011, USBDRD0_UAHC_DEPCMDPAR0_012, USBDRD0_UAHC_DEPCMDPAR0_013, USBDRD0_UAHC_DEPCMDPAR0_014, USBDRD0_UAHC_DEPCMDPAR0_015, USBDRD0_UAHC_DEPCMDPAR1_000, USBDRD0_UAHC_DEPCMDPAR1_001, USBDRD0_UAHC_DEPCMDPAR1_002, USBDRD0_UAHC_DEPCMDPAR1_003, USBDRD0_UAHC_DEPCMDPAR1_004, USBDRD0_UAHC_DEPCMDPAR1_005, USBDRD0_UAHC_DEPCMDPAR1_006, USBDRD0_UAHC_DEPCMDPAR1_007, USBDRD0_UAHC_DEPCMDPAR1_008, USBDRD0_UAHC_DEPCMDPAR1_009, USBDRD0_UAHC_DEPCMDPAR1_010, USBDRD0_UAHC_DEPCMDPAR1_011, USBDRD0_UAHC_DEPCMDPAR1_012, USBDRD0_UAHC_DEPCMDPAR1_013, USBDRD0_UAHC_DEPCMDPAR1_014, USBDRD0_UAHC_DEPCMDPAR1_015, USBDRD0_UAHC_DEPCMDPAR2_000, USBDRD0_UAHC_DEPCMDPAR2_001, USBDRD0_UAHC_DEPCMDPAR2_002, USBDRD0_UAHC_DEPCMDPAR2_003, USBDRD0_UAHC_DEPCMDPAR2_004, USBDRD0_UAHC_DEPCMDPAR2_005, USBDRD0_UAHC_DEPCMDPAR2_006, USBDRD0_UAHC_DEPCMDPAR2_007, USBDRD0_UAHC_DEPCMDPAR2_008, USBDRD0_UAHC_DEPCMDPAR2_009, USBDRD0_UAHC_DEPCMDPAR2_010, USBDRD0_UAHC_DEPCMDPAR2_011, USBDRD0_UAHC_DEPCMDPAR2_012, USBDRD0_UAHC_DEPCMDPAR2_013, USBDRD0_UAHC_DEPCMDPAR2_014, USBDRD0_UAHC_DEPCMDPAR2_015, USBDRD0_UAHC_DEVTEN, USBDRD0_UAHC_DGCMD, USBDRD0_UAHC_DGCMDPAR, USBDRD0_UAHC_DNCTRL, USBDRD0_UAHC_DSTS, USBDRD0_UAHC_ERDP000, USBDRD0_UAHC_ERSTBA000, USBDRD0_UAHC_ERSTSZ000, USBDRD0_UAHC_GBUSERRADDR, USBDRD0_UAHC_GCTL, USBDRD0_UAHC_GDBGBMU, USBDRD0_UAHC_GDBGEPINFO, USBDRD0_UAHC_GDBGFIFOSPACE
REG_WINDOW = USBDRD0_UAHC_GDBGLNMCC USBDRD0_UAHC_GDBGLNMCC, USBDRD0_UAHC_GDBGLSP, USBDRD0_UAHC_GDBGLSPMUX, USBDRD0_UAHC_GDBGLTSSM, USBDRD0_UAHC_GDMAHLRATIO, USBDRD0_UAHC_GEVNTADR000, USBDRD0_UAHC_GEVNTCOUNT000, USBDRD0_UAHC_GEVNTSIZ000, USBDRD0_UAHC_GFLADJ, USBDRD0_UAHC_GGPIO, USBDRD0_UAHC_GHWPARAMS0, USBDRD0_UAHC_GHWPARAMS1, USBDRD0_UAHC_GHWPARAMS2, USBDRD0_UAHC_GHWPARAMS3, USBDRD0_UAHC_GHWPARAMS4, USBDRD0_UAHC_GHWPARAMS5, USBDRD0_UAHC_GHWPARAMS6, USBDRD0_UAHC_GHWPARAMS7, USBDRD0_UAHC_GHWPARAMS8, USBDRD0_UAHC_GPMSTS, USBDRD0_UAHC_GPRTBIMAP, USBDRD0_UAHC_GPRTBIMAP_FS, USBDRD0_UAHC_GPRTBIMAP_HS, USBDRD0_UAHC_GRLSID, USBDRD0_UAHC_GRXFIFOPRIHST, USBDRD0_UAHC_GRXFIFOSIZ000, USBDRD0_UAHC_GRXFIFOSIZ001, USBDRD0_UAHC_GRXFIFOSIZ002, USBDRD0_UAHC_GRXTHRCFG, USBDRD0_UAHC_GSBUSCFG0, USBDRD0_UAHC_GSBUSCFG1, USBDRD0_UAHC_GSTS, USBDRD0_UAHC_GTXFIFOPRIDEV, USBDRD0_UAHC_GTXFIFOPRIHST, USBDRD0_UAHC_GTXFIFOSIZ000, USBDRD0_UAHC_GTXFIFOSIZ001, USBDRD0_UAHC_GTXFIFOSIZ002, USBDRD0_UAHC_GTXFIFOSIZ003, USBDRD0_UAHC_GTXTHRCFG, USBDRD0_UAHC_GUCTL, USBDRD0_UAHC_GUCTL1, USBDRD0_UAHC_GUID, USBDRD0_UAHC_GUSB2I2CCTL000, USBDRD0_UAHC_GUSB2PHYCFG000, USBDRD0_UAHC_GUSB3PIPECTL000, USBDRD0_UAHC_HCCPARAMS, USBDRD0_UAHC_HCSPARAMS1, USBDRD0_UAHC_HCSPARAMS2, USBDRD0_UAHC_HCSPARAMS3, USBDRD0_UAHC_IMAN000
REG_WINDOW = USBDRD0_UAHC_IMOD000 USBDRD0_UAHC_IMOD000, USBDRD0_UAHC_MFINDEX, USBDRD0_UAHC_PAGESIZE, USBDRD0_UAHC_PORTHLPMC_20000, USBDRD0_UAHC_PORTHLPMC_SS001, USBDRD0_UAHC_PORTLI_20000, USBDRD0_UAHC_PORTLI_SS001, USBDRD0_UAHC_PORTPMSC_20000, USBDRD0_UAHC_PORTPMSC_SS001, USBDRD0_UAHC_PORTSC000, USBDRD0_UAHC_PORTSC001, USBDRD0_UAHC_RTSOFF, USBDRD0_UAHC_SUPTPRT2_DW0, USBDRD0_UAHC_SUPTPRT2_DW1, USBDRD0_UAHC_SUPTPRT2_DW2, USBDRD0_UAHC_SUPTPRT2_DW3, USBDRD0_UAHC_SUPTPRT3_DW0, USBDRD0_UAHC_SUPTPRT3_DW1, USBDRD0_UAHC_SUPTPRT3_DW2, USBDRD0_UAHC_SUPTPRT3_DW3, USBDRD0_UAHC_USBCMD, USBDRD0_UAHC_USBLEGCTLSTS, USBDRD0_UAHC_USBLEGSUP, USBDRD0_UAHC_USBSTS, USBDRD0_UCTL_BIST_STATUS, USBDRD0_UCTL_CTL, USBDRD0_UCTL_ECC, USBDRD0_UCTL_HOST_CFG, USBDRD0_UCTL_INTSTAT, USBDRD0_UCTL_PORT000_CFG_HS, USBDRD0_UCTL_PORT000_CFG_SS, USBDRD0_UCTL_PORT000_CR_DBG_CFG, USBDRD0_UCTL_PORT000_CR_DBG_STATUS, USBDRD0_UCTL_SHIM_CFG, USBDRD0_UCTL_SPARE0, USBDRD0_UCTL_SPARE1
REG_WINDOW = USBDRD1_UAHC_CAPLENGTH USBDRD1_UAHC_CAPLENGTH, USBDRD1_UAHC_CONFIG, USBDRD1_UAHC_CRCR, USBDRD1_UAHC_DALEPENA, USBDRD1_UAHC_DB000, USBDRD1_UAHC_DB001, USBDRD1_UAHC_DB002, USBDRD1_UAHC_DB003, USBDRD1_UAHC_DB004, USBDRD1_UAHC_DB005, USBDRD1_UAHC_DB006, USBDRD1_UAHC_DB007, USBDRD1_UAHC_DB008, USBDRD1_UAHC_DB009, USBDRD1_UAHC_DB010, USBDRD1_UAHC_DB011, USBDRD1_UAHC_DB012, USBDRD1_UAHC_DB013, USBDRD1_UAHC_DB014, USBDRD1_UAHC_DB015, USBDRD1_UAHC_DB016, USBDRD1_UAHC_DB017, USBDRD1_UAHC_DB018, USBDRD1_UAHC_DB019, USBDRD1_UAHC_DB020, USBDRD1_UAHC_DB021, USBDRD1_UAHC_DB022, USBDRD1_UAHC_DB023, USBDRD1_UAHC_DB024, USBDRD1_UAHC_DB025, USBDRD1_UAHC_DB026, USBDRD1_UAHC_DB027, USBDRD1_UAHC_DB028, USBDRD1_UAHC_DB029, USBDRD1_UAHC_DB030, USBDRD1_UAHC_DB031, USBDRD1_UAHC_DB032, USBDRD1_UAHC_DB033, USBDRD1_UAHC_DB034, USBDRD1_UAHC_DB035, USBDRD1_UAHC_DB036, USBDRD1_UAHC_DB037, USBDRD1_UAHC_DB038, USBDRD1_UAHC_DB039, USBDRD1_UAHC_DB040, USBDRD1_UAHC_DB041, USBDRD1_UAHC_DB042, USBDRD1_UAHC_DB043, USBDRD1_UAHC_DB044, USBDRD1_UAHC_DB045
REG_WINDOW = USBDRD1_UAHC_DB046 USBDRD1_UAHC_DB046, USBDRD1_UAHC_DB047, USBDRD1_UAHC_DB048, USBDRD1_UAHC_DB049, USBDRD1_UAHC_DB050, USBDRD1_UAHC_DB051, USBDRD1_UAHC_DB052, USBDRD1_UAHC_DB053, USBDRD1_UAHC_DB054, USBDRD1_UAHC_DB055, USBDRD1_UAHC_DB056, USBDRD1_UAHC_DB057, USBDRD1_UAHC_DB058, USBDRD1_UAHC_DB059, USBDRD1_UAHC_DB060, USBDRD1_UAHC_DB061, USBDRD1_UAHC_DB062, USBDRD1_UAHC_DB063, USBDRD1_UAHC_DB064, USBDRD1_UAHC_DBOFF, USBDRD1_UAHC_DCBAAP, USBDRD1_UAHC_DCFG, USBDRD1_UAHC_DCTL, USBDRD1_UAHC_DEPCMD000, USBDRD1_UAHC_DEPCMD001, USBDRD1_UAHC_DEPCMD002, USBDRD1_UAHC_DEPCMD003, USBDRD1_UAHC_DEPCMD004, USBDRD1_UAHC_DEPCMD005, USBDRD1_UAHC_DEPCMD006, USBDRD1_UAHC_DEPCMD007, USBDRD1_UAHC_DEPCMD008, USBDRD1_UAHC_DEPCMD009, USBDRD1_UAHC_DEPCMD010, USBDRD1_UAHC_DEPCMD011, USBDRD1_UAHC_DEPCMD012, USBDRD1_UAHC_DEPCMD013, USBDRD1_UAHC_DEPCMD014, USBDRD1_UAHC_DEPCMD015, USBDRD1_UAHC_DEPCMDPAR0_000, USBDRD1_UAHC_DEPCMDPAR0_001, USBDRD1_UAHC_DEPCMDPAR0_002, USBDRD1_UAHC_DEPCMDPAR0_003, USBDRD1_UAHC_DEPCMDPAR0_004, USBDRD1_UAHC_DEPCMDPAR0_005, USBDRD1_UAHC_DEPCMDPAR0_006, USBDRD1_UAHC_DEPCMDPAR0_007, USBDRD1_UAHC_DEPCMDPAR0_008, USBDRD1_UAHC_DEPCMDPAR0_009, USBDRD1_UAHC_DEPCMDPAR0_010
REG_WINDOW = USBDRD1_UAHC_DEPCMDPAR0_011 USBDRD1_UAHC_DEPCMDPAR0_011, USBDRD1_UAHC_DEPCMDPAR0_012, USBDRD1_UAHC_DEPCMDPAR0_013, USBDRD1_UAHC_DEPCMDPAR0_014, USBDRD1_UAHC_DEPCMDPAR0_015, USBDRD1_UAHC_DEPCMDPAR1_000, USBDRD1_UAHC_DEPCMDPAR1_001, USBDRD1_UAHC_DEPCMDPAR1_002, USBDRD1_UAHC_DEPCMDPAR1_003, USBDRD1_UAHC_DEPCMDPAR1_004, USBDRD1_UAHC_DEPCMDPAR1_005, USBDRD1_UAHC_DEPCMDPAR1_006, USBDRD1_UAHC_DEPCMDPAR1_007, USBDRD1_UAHC_DEPCMDPAR1_008, USBDRD1_UAHC_DEPCMDPAR1_009, USBDRD1_UAHC_DEPCMDPAR1_010, USBDRD1_UAHC_DEPCMDPAR1_011, USBDRD1_UAHC_DEPCMDPAR1_012, USBDRD1_UAHC_DEPCMDPAR1_013, USBDRD1_UAHC_DEPCMDPAR1_014, USBDRD1_UAHC_DEPCMDPAR1_015, USBDRD1_UAHC_DEPCMDPAR2_000, USBDRD1_UAHC_DEPCMDPAR2_001, USBDRD1_UAHC_DEPCMDPAR2_002, USBDRD1_UAHC_DEPCMDPAR2_003, USBDRD1_UAHC_DEPCMDPAR2_004, USBDRD1_UAHC_DEPCMDPAR2_005, USBDRD1_UAHC_DEPCMDPAR2_006, USBDRD1_UAHC_DEPCMDPAR2_007, USBDRD1_UAHC_DEPCMDPAR2_008, USBDRD1_UAHC_DEPCMDPAR2_009, USBDRD1_UAHC_DEPCMDPAR2_010, USBDRD1_UAHC_DEPCMDPAR2_011, USBDRD1_UAHC_DEPCMDPAR2_012, USBDRD1_UAHC_DEPCMDPAR2_013, USBDRD1_UAHC_DEPCMDPAR2_014, USBDRD1_UAHC_DEPCMDPAR2_015, USBDRD1_UAHC_DEVTEN, USBDRD1_UAHC_DGCMD, USBDRD1_UAHC_DGCMDPAR, USBDRD1_UAHC_DNCTRL, USBDRD1_UAHC_DSTS, USBDRD1_UAHC_ERDP000, USBDRD1_UAHC_ERSTBA000, USBDRD1_UAHC_ERSTSZ000, USBDRD1_UAHC_GBUSERRADDR, USBDRD1_UAHC_GCTL, USBDRD1_UAHC_GDBGBMU, USBDRD1_UAHC_GDBGEPINFO, USBDRD1_UAHC_GDBGFIFOSPACE
REG_WINDOW = USBDRD1_UAHC_GDBGLNMCC USBDRD1_UAHC_GDBGLNMCC, USBDRD1_UAHC_GDBGLSP, USBDRD1_UAHC_GDBGLSPMUX, USBDRD1_UAHC_GDBGLTSSM, USBDRD1_UAHC_GDMAHLRATIO, USBDRD1_UAHC_GEVNTADR000, USBDRD1_UAHC_GEVNTCOUNT000, USBDRD1_UAHC_GEVNTSIZ000, USBDRD1_UAHC_GFLADJ, USBDRD1_UAHC_GGPIO, USBDRD1_UAHC_GHWPARAMS0, USBDRD1_UAHC_GHWPARAMS1, USBDRD1_UAHC_GHWPARAMS2, USBDRD1_UAHC_GHWPARAMS3, USBDRD1_UAHC_GHWPARAMS4, USBDRD1_UAHC_GHWPARAMS5, USBDRD1_UAHC_GHWPARAMS6, USBDRD1_UAHC_GHWPARAMS7, USBDRD1_UAHC_GHWPARAMS8, USBDRD1_UAHC_GPMSTS, USBDRD1_UAHC_GPRTBIMAP, USBDRD1_UAHC_GPRTBIMAP_FS, USBDRD1_UAHC_GPRTBIMAP_HS, USBDRD1_UAHC_GRLSID, USBDRD1_UAHC_GRXFIFOPRIHST, USBDRD1_UAHC_GRXFIFOSIZ000, USBDRD1_UAHC_GRXFIFOSIZ001, USBDRD1_UAHC_GRXFIFOSIZ002, USBDRD1_UAHC_GRXTHRCFG, USBDRD1_UAHC_GSBUSCFG0, USBDRD1_UAHC_GSBUSCFG1, USBDRD1_UAHC_GSTS, USBDRD1_UAHC_GTXFIFOPRIDEV, USBDRD1_UAHC_GTXFIFOPRIHST, USBDRD1_UAHC_GTXFIFOSIZ000, USBDRD1_UAHC_GTXFIFOSIZ001, USBDRD1_UAHC_GTXFIFOSIZ002, USBDRD1_UAHC_GTXFIFOSIZ003, USBDRD1_UAHC_GTXTHRCFG, USBDRD1_UAHC_GUCTL, USBDRD1_UAHC_GUCTL1, USBDRD1_UAHC_GUID, USBDRD1_UAHC_GUSB2I2CCTL000, USBDRD1_UAHC_GUSB2PHYCFG000, USBDRD1_UAHC_GUSB3PIPECTL000, USBDRD1_UAHC_HCCPARAMS, USBDRD1_UAHC_HCSPARAMS1, USBDRD1_UAHC_HCSPARAMS2, USBDRD1_UAHC_HCSPARAMS3, USBDRD1_UAHC_IMAN000
REG_WINDOW = USBDRD1_UAHC_IMOD000 USBDRD1_UAHC_IMOD000, USBDRD1_UAHC_MFINDEX, USBDRD1_UAHC_PAGESIZE, USBDRD1_UAHC_PORTHLPMC_20000, USBDRD1_UAHC_PORTHLPMC_SS001, USBDRD1_UAHC_PORTLI_20000, USBDRD1_UAHC_PORTLI_SS001, USBDRD1_UAHC_PORTPMSC_20000, USBDRD1_UAHC_PORTPMSC_SS001, USBDRD1_UAHC_PORTSC000, USBDRD1_UAHC_PORTSC001, USBDRD1_UAHC_RTSOFF, USBDRD1_UAHC_SUPTPRT2_DW0, USBDRD1_UAHC_SUPTPRT2_DW1, USBDRD1_UAHC_SUPTPRT2_DW2, USBDRD1_UAHC_SUPTPRT2_DW3, USBDRD1_UAHC_SUPTPRT3_DW0, USBDRD1_UAHC_SUPTPRT3_DW1, USBDRD1_UAHC_SUPTPRT3_DW2, USBDRD1_UAHC_SUPTPRT3_DW3, USBDRD1_UAHC_USBCMD, USBDRD1_UAHC_USBLEGCTLSTS, USBDRD1_UAHC_USBLEGSUP, USBDRD1_UAHC_USBSTS, USBDRD1_UCTL_BIST_STATUS, USBDRD1_UCTL_CTL, USBDRD1_UCTL_ECC, USBDRD1_UCTL_HOST_CFG, USBDRD1_UCTL_INTSTAT, USBDRD1_UCTL_PORT000_CFG_HS, USBDRD1_UCTL_PORT000_CFG_SS, USBDRD1_UCTL_PORT000_CR_DBG_CFG, USBDRD1_UCTL_PORT000_CR_DBG_STATUS, USBDRD1_UCTL_SHIM_CFG, USBDRD1_UCTL_SPARE0, USBDRD1_UCTL_SPARE1
REG_WINDOW = VRM0 VRM0_ALT_FUSE, VRM0_FUSE_BYPASS, VRM0_MISC_INFO, VRM0_TELEMETRY_CMND, VRM0_TELEMETRY_READ, VRM0_TS_TEMP_CONV_COEFF_FSM, VRM0_TS_TEMP_CONV_CTL, VRM0_TS_TEMP_CONV_RESULT, VRM0_TS_TEMP_NOFF_MC, VRM0_TWS_TWSI_SW
