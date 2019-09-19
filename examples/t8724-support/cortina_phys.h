#ifndef _CORTINA_PHYS_H_
#define _CORTINA_PHYS_H_

#define MAX_MODULES_NUM          2
#define MAX_SLICES_NUM           8

#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLA 0x108F
#define CS4224_PP_LINE_SDS_COMMON_STX0_TX_OUTPUT_CTRLB 0x1090
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_LSB        0x12AC
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE22_MSB        0x12AD
#define CS4224_PP_LINE_SDS_DSP_MSEQ_SPARE24_LSB        0x12B0
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_LSB        0x1AAC
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE22_MSB        0x1AAD
#define CS4224_PP_HOST_SDS_DSP_MSEQ_SPARE24_LSB        0x1AB0

/** Mode that the slice is running in */
enum cortina_slice_mode {
   CORTINA_SLICE_SR,      /** Optical */
   CORTINA_SLICE_CX,      /** Passive copper */
   CORTINA_SLICE_1000X,   /** 1000Base-X */
   CORTINA_SLICE_UNKNOWN, /** Unknown/default mode */
};

int cs4318_reg_get(int slice, int addr, uint16_t* data);
int cs4318_reg_set(int slice, int addr, uint16_t data);

int cs4343_reg_get(int bus, int slice, int addr, uint16_t* data);
int cs4343_reg_set(int bus, int slice, int addr, uint16_t data);

int pim_port_check_updt(uint16_t slot, uint16_t port);
void pim_set_defines(void);

#endif /* _CORTINA_PHYS_H_ */
