#ifndef __IM8724_PHY_H_
#define __IM8724_PHY_H_

#include <stdint.h>

#define VSC8221_LED_CTL_REG      0x1b
#define VSC8221_LED_PIN2_MASK    0x0c00
#define VSC8221_LED_PIN2_CFG     0x0800
#define VSC8221_LED_PIN1_MASK    0x0300
#define VSC8221_LED_PIN1_CFG     0x0000
#define VSC8221_LED_PIN0_MASK    0x00c0
#define VSC8221_LED_PIN0_CFG     0x0000
#define VSC8221_LED_CFGS_MASK    0x003f
#define VSC8221_LED_CFGS_MODE    0x0033

#define MAX_SLOTS_NUM            3
#define MAX_SLICES_NUM           8
#define MAX_ETH_PORT_NUM         20
#define MAX_MODULES_NUM          3
#define MAX_SFP_NUM              8

/* NOTE: this enum must be the same as   *
 *corresponding enum from niagara driver */
enum cortina_slice_mode {
   CORTINA_SLICE_SR,      /** Optical */
   CORTINA_SLICE_CX,      /** Passive copper */
   CORTINA_SLICE_1000X,   /** 1000Base-X */
   CORTINA_SLICE_UNKNOWN, /** Unknown/default mode */
   CORTINA_SLICE_MAX = CORTINA_SLICE_UNKNOWN
};

typedef struct {
    uint32_t   bgx;
    uint32_t   indx;
} bgx_lane_map_t;

extern int sgmii_mode;
extern int fp_ports_num;
extern int quad_mode[MAX_SLOTS_NUM];


int get_eth_by_sfp(uint32_t *eth, uint32_t module, uint32_t sfp, uint8_t rxaui_mode);
int get_sfp_by_eth(uint32_t eth, uint32_t *module, uint32_t *sfp, uint8_t rxaui_mode);
int get_eth_by_bgx(bgx_lane_map_t *bgx_lane, uint32_t *eth, uint8_t rxaui_mode);
int get_bgx_by_eth(bgx_lane_map_t *bgx_lane, uint32_t eth, uint8_t rxaui_mode);
int get_sfp_by_bgx(bgx_lane_map_t *bgx_lane, uint32_t *module, uint32_t *sfp, uint8_t rxaui_mode);
int get_bgx_by_sfp(bgx_lane_map_t *bgx_lane, uint32_t module, uint32_t sfp, uint8_t rxaui_mode);

void pim_set_defines(void);
int pim_port_check_updt(uint16_t slot, uint16_t port, enum cortina_slice_mode mode);

int is_cs4318_phy(void);
int is_sgmii_board_mode(void);
int get_pim_interface_activity(int slot, int port);
int get_fp_interface_activity(int port);
int get_mgmt_interface_status(void);
uint8_t get_mgmt_interface_speed(void);
void set_mgmt_interface_mode(void);
void set_fp_interface_mode(void);

#endif /* __IM8724_PHY_H_ */
