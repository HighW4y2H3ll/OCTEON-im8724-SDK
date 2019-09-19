#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include <cvmx.h>
#include <cvmx-platform.h>
#include <cvmx-mdio.h>

#include <cvmx-helper.h>
#include <cvmx-qlm.h>
#include <cvmx-gserx-defs.h>
#include <cvmx-bgxx-defs.h>

#include "board_mgmt.h"
#include "im8724_phy.h"

#include "board_common.h"

#define MGMT_OPT_ALL_PHYS         (MGMT_OPT_PIM_PHYS | MGMT_OPT_FP_PHYS | MGMT_OPT_MGMT_PHYS)

#define WRITE_SLEEP 250

uint8_t dbg_mode = 0;

static led_info_t pim_led_info[MAX_IM8724_MODULES][MAX_PORTS_PER_MODULE] = {{}};
static io_card_power_t pim_power_info[MAX_IM8724_MODULES];
static led_info_t fp_led_info[MAX_FRONTPANEL_PORTS] = {};
static mgmt_led_info_t mgmt_led_info;
static sys_leds_info_t sys_leds;
static int termination_requested = 0;
static int daemonize = 0;
static int sfp_mm_allow=0;
static uint32_t mgmt_mode = MGMT_OPT_PIM_PHYS;
static uint8_t target_temp = DEFAULT_BOARD_TEMP;
static int io_cards_presents = 0;
static unsigned long io_card_sign_l[MAX_IM8724_MODULES] = { 0, 0 };

static void oct_rd(uint64_t reg)
{
    uint64_t val = cvmx_read_csr(CVMX_ADD_IO_SEG(reg));
    printf("[RD] 0x%llx => 0x%llx\n", (unsigned long long)reg, (unsigned long long)val);
}

static void oct_wr(uint64_t reg, uint64_t val)
{
        printf("[WR] 0x%llx <= 0x%llx\n", (unsigned long long)reg, (unsigned long long)val);
        cvmx_write_csr(CVMX_ADD_IO_SEG(reg), val);
}

enum { MODE_READ = 1, MODE_WRITE = 2 };

static int mode = 0;
static uint64_t reg = 0;
static uint64_t val = 0;
static int dont_shutdown_on_temp = 0;

static void parse_args(int argc, char *argv[])
{
    int errflag = 0, c;

    while (!errflag && (c=getopt(argc,argv,"rwd:m:t:hDR:V:ns")) != -1 ) {
        switch (c) {
            case 'r':
                mode = MODE_READ;
                break;
            case 'w':
                mode = MODE_WRITE;
                break;
            case 'm':
                mgmt_mode = 0;
                if (strstr(optarg, MGMT_ALL_PHYS_STR))
                    mgmt_mode |= MGMT_OPT_ALL_PHYS;
                else {
                    if (strstr(optarg, MGMT_PIM_PHYS_STR))
                        mgmt_mode |= MGMT_OPT_PIM_PHYS;
                    if (strstr(optarg, MGMT_FP_PHYS_STR))
                        mgmt_mode |= MGMT_OPT_FP_PHYS;
                    if (strstr(optarg, MGMT_MGMT_PHYS_STR))
                        mgmt_mode |= MGMT_OPT_MGMT_PHYS;
                }
                break;
            case 'd':
                dbg_mode = atoi(optarg);
                break;
            case 'D':
                daemonize = 1;
                break;
            case 'R':
                reg = strtoull(optarg, NULL, 16);
                break;
            case 'V':
                val = strtoull(optarg, NULL, 16);
                break;
            case 't':
                target_temp = atoi(optarg);
                break;
            case 'n':
                dont_shutdown_on_temp = 1;
                break;
            case 's':
                sfp_mm_allow = 1;
                break;
            case 'h':
            default:
                errflag = 1;
        }
    }

    if (!mgmt_mode) {
        debug(1, "Error: management mode is not set!\n");
        _exit(-1);
    }
    if (!target_temp) {
        debug(1, "Warning: bad target temperature setting, usiung default value!\n");
        target_temp = DEFAULT_BOARD_TEMP;
    }
    if (errflag > 0) {
        printf("%s: usage: [options]\n", argv[0]);
        printf("options: -m - set managemen mode, comma separated list of params (default: %s)\n", MGMT_ALL_STR);
        printf("              params: %s | %s | %s | %s | %s\n",  MGMT_ALL_PHYS_STR,
                                          MGMT_PIM_PHYS_STR, MGMT_FP_PHYS_STR, MGMT_MGMT_PHYS_STR, MGMT_ALL_STR);
        printf("         -d - set debug mode: 0-9 (default: 0)\n");
        printf("         -D - run process in backround\n");
        printf("         -t - set target temperature (default: %d)\n", DEFAULT_BOARD_TEMP);
        printf("         -n - don't shutdown board on critical overtemperature (USE CAREFUL!)\n");
        _exit(-1);
    }
}

int get_max_slot_ports(int slot)
{
    return ((io_card_sign_l[slot] == IO_CARD_13264_SIG_VALUE)
        ? MAX_QSFP_PORTS_PER_MODULE
        : ((io_card_sign_l[slot] == IO_CARD_13262_SIG_VALUE)
            ? MAX_SFP28_PORTS_PER_MODULE
            : MAX_SFP_PORTS_PER_MODULE));
}

static void respawn_as_child(void)
{
    pid_t pid;
    int status;

    do {
        pid = fork();
        if (pid == -1) {
            exit(-1);
        } else if (pid == 0) {
            return;
        } else {
            status=0;
            wait(&status);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status)!= 0) {
                    exit(WEXITSTATUS(status));
                }
            } else {
                exit(-1);
            }
        }
    } while (1);
}

#define MAXFD   10
static void daemon_init(void)
{
    int i;
    pid_t pid;

    if ((pid = fork()) != 0)
        exit(0);
    setsid();
    signal(SIGHUP, SIG_IGN);
    if ( (pid = fork()) != 0)
       exit(0);
    umask(0);
    for (i = 0; i < MAXFD; i++)
        close(i);
}

static void sighand(int signo)
{
    if (termination_requested) {
        debug(5, "%d\n", signo);
        exit(-1);
    }
    termination_requested++;
}

static unsigned long mod_strtoul(const char *cp, unsigned int base)
{
    unsigned long result = 0,value;

    if (*cp == '0') {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1])) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
        ? toupper(*cp) : *cp)-'A'+10) < base) {
        result = result*base + value;
        cp++;
    }
    return result;
}

static int read_data_signed(int fd, long *val)
{
    int again = 0;
    char buf[16];
    ssize_t l;
    unsigned int base = 10, sign = 0;

    lseek(fd, 0, SEEK_SET);
    do {
        l = read(fd, buf, 16);
        if (((l < 0) && ((errno == EAGAIN) || (errno == EINTR))) || (l == 0)) {
            lseek(fd, 0, SEEK_SET);
            again = 1;
        }
    } while (again);

    if(*buf == '-')
        sign = 1;

    if((buf[sign] == '0') && ((buf[sign + 1] == 'x') || (buf[sign + 1] == 'X')))
        base = 16;

    *val = mod_strtoul(&buf[sign], base);

    if(sign)
        *val = -(*val);

    return 0;
}

static int read_data(int fd, unsigned long *val)
{
    int again = 0;
    char buf[16];
    ssize_t l;

    lseek(fd, 0, SEEK_SET);
    do {
        l = read(fd, buf, 16);
        if (((l < 0) && ((errno == EAGAIN) || (errno == EINTR))) || (l == 0)) {
            lseek(fd, 0, SEEK_SET);
            again = 1;
        }
    } while (again);
    *val = mod_strtoul(buf, 16);
    return 0;
}

static int write_data(int fd, unsigned long val)
{
    int again = 0;
    char buf[16];
    ssize_t l;

    l = sprintf(buf, "%lu\n", val);
    lseek(fd, 0, SEEK_SET);
    do {
        l = write(fd, buf, l);
        if (((l < 0) && ((errno == EAGAIN) || (errno == EINTR))) || (l == 0)) {
            lseek(fd, 0, SEEK_SET);
            again = 1;
        }
    } while (again);
    return 0;
}

static int im8724_io_card_present(int slot)
{
    static int io_cards_fd[MAX_IM8724_MODULES] = { -1, -1 }; // 2, fix init if define is changed
    static int io_sigcs_fd[MAX_IM8724_MODULES] = { -1, -1 };
    unsigned long data;

    if (slot != 0 && slot != 1)
        return -1;

    if (io_cards_fd[slot] < 0) {
        if ((io_cards_fd[slot] = open(slot ? IOB_CARD_PRES : IOA_CARD_PRES, O_RDONLY)) < 0)
            return -1;
    }

    if (io_sigcs_fd[slot] < 0) {
        read_data(io_cards_fd[slot], &data);
        if (!data)
            return 0;

        if ((io_sigcs_fd[slot] = open(slot ? IOB_CARD_SIGN : IOA_CARD_SIGN, O_RDONLY)) < 0)
            return -1;

         read_data(io_sigcs_fd[slot], &io_card_sign_l[slot]);

	   switch (io_card_sign_l[slot]) {
		case IO_CARD_13233_SIG_VALUE:
		case IO_CARD_13264_SIG_VALUE:
		case IO_CARD_13262_SIG_VALUE:
			break;
		default:
			debug(1, "Error: IO card %c: bad signature 0x%02x\n", slot?'B':'A', (unsigned int)io_card_sign_l[slot]);
	            return -1;
	   }
    }

    read_data(io_cards_fd[slot], &data);
    return data;
}

static int pim_led_init(void)
{
    int slot, port, fd;
    char buf[256];

    debug(3, "Start pim leds init... ");
    for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
        if (im8724_io_card_present(slot) != 1)
            continue;
        io_cards_presents |= 1 << slot;
        sprintf(buf, IO_CARD_LED_ACT_CFG, slot);
        if ((fd = open(buf, O_RDWR)) < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }
        write_data(fd, 1);
        close(fd);
        sprintf(buf, IO_CARD_LED_COL_CFG, slot);
        if ((fd = open(buf, O_RDWR)) < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }
        write_data(fd, 1);
        close(fd);
    }

    for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
        if (!(io_cards_presents & (1 << slot)))
            continue;

        sprintf(buf, IO_CARD_POWER_ENABLE, slot ? 'b' : 'a');
        pim_power_info[slot].io_card_pe = open(buf, O_RDWR);
        if(pim_power_info[slot].io_card_pe < 0) {
            pim_power_info[slot].io_card_pe = 0;
        }
        sprintf(buf, IO_CARD_HOTPLUG_POWER_ENABLE, slot ? 'b' : 'a');
        pim_power_info[slot].io_card_hp_pe = open(buf, O_RDWR);
        if(pim_power_info[slot].io_card_hp_pe < 0) {
            pim_power_info[slot].io_card_hp_pe = 0;
        }

        for (port = 0; port < get_max_slot_ports(slot); port++) {
            sprintf(buf, IO_CARD_LED_INDEX_FMT(quad_mode[slot]), slot, port);
            if ((fd = open(buf, O_RDWR)) < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }
            write_data(fd, 8 * slot + port);

            sprintf(buf, IO_CARD_SFP_PRES_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].sfp_pres = open(buf, O_RDONLY);
            if (pim_led_info[slot][port].sfp_pres < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }

            sprintf(buf, IO_CARD_SFP_RXLOS_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].sfp_rxlos = open(buf, O_RDONLY);
            if (pim_led_info[slot][port].sfp_rxlos < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }

            sprintf(buf, IO_CARD_SFP_SPEED_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].sfp_speed = open(buf, O_RDWR);
            if (pim_led_info[slot][port].sfp_speed < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }

            sprintf(buf, IO_CARD_MEDIA_TYPE_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].media_type = open(buf, O_RDWR);
            if (pim_led_info[slot][port].media_type < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }

            sprintf(buf, IO_CARD_SFP_TXDIS_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].sfp_txdis = open(buf, O_RDWR);
            if (pim_led_info[slot][port].sfp_txdis < 0)
                debug(1, "Error: can't open %s file!\n", buf);

            sprintf(buf, IO_CARD_SFP_TEMP_FMT(quad_mode[slot]), slot, port);
            pim_led_info[slot][port].sfp_temperature = open(buf, O_RDONLY);
            if (pim_led_info[slot][port].sfp_temperature < 0)
                debug(1, "Error: can't open %s file!\n", buf);

#ifdef MAIN_CPLD_MON
            sprintf(buf, PIM_LED_COL_FMT, 8 * slot + port);
#else
            sprintf(buf, IO_CARD_LED_ACT_FMT(quad_mode[slot]), slot, port);
#endif
            pim_led_info[slot][port].led_act = open(buf, O_RDWR);
            debug(8, "Open %s (fd: %d), slot:%d, port:%d\n",
                 buf, pim_led_info[slot][port].led_act, slot, port);
            if (pim_led_info[slot][port].led_act < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }
            write_data(pim_led_info[slot][port].led_act, LED_CNTRL_OFF);

#ifdef MAIN_CPLD_MON
            sprintf(buf, PIM_LED_COL_FMT, 8 * slot + port);
#else
            sprintf(buf, IO_CARD_LED_COL_FMT(quad_mode[slot]), slot, port);
#endif
            pim_led_info[slot][port].led_col = open(buf, O_RDWR);
            debug(8, "Open %s (fd: %d), slot:%d, port:%d\n",
                 buf, pim_led_info[slot][port].led_col, slot, port);
            if (pim_led_info[slot][port].led_col < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }
            write_data(pim_led_info[slot][port].led_col, LED_CLR_OFF);
        }
    }
    debug(3, "[%s]\n", io_cards_presents ? "done" : "skipped");
    return 0;
}

static unsigned long get_interface_qlm_speed(int xiface)
{
    struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);
    int qlm;
    unsigned long value=10;
    cvmx_gserx_cfg_t gserx_cfg;
    cvmx_gserx_phy_ctl_t phy_ctl;

    if(xi.interface >= 6)
        return 0;

    if (xi.interface < 2) {
        cvmx_bgxx_cmr_global_config_t gconfig;
        gconfig.u64 = cvmx_read_csr_node(xi.node,
            CVMX_BGXX_CMR_GLOBAL_CONFIG(xi.interface));
    if (gconfig.s.pmux_sds_sel)
        qlm = xi.interface + 2; /* QLM 2 or 3 */
    else
        qlm = xi.interface; /* QLM 0 or 1 */
    } else
        qlm = xi.interface + 2; /* QLM 4-7 */

    /* make sure the QLM is powered up and out of reset */
    phy_ctl.u64 = cvmx_read_csr_node(xi.node, CVMX_GSERX_PHY_CTL(qlm));
    if (phy_ctl.s.phy_pd || phy_ctl.s.phy_reset)
        return 0;

    gserx_cfg.u64 = cvmx_read_csr_node(xi.node, CVMX_GSERX_CFG(qlm));
    if (gserx_cfg.s.bgx) {
        cvmx_bgxx_cmrx_config_t cmr_config;
        int bgx = (qlm < 2) ? qlm : qlm - 2;

        cmr_config.u64 = cvmx_read_csr_node(xi.node, CVMX_BGXX_CMRX_CONFIG(0, bgx));

        if(cmr_config.s.lmac_type == 0) //CVMX_QLM_MODE_SGMII
            value = 1;
    } else
        return 0;

    return value;
}

/**
 * tested on IM8724 only!
 */
static void set_max_sfp_rate(int handler, unsigned long value)
{
    unsigned long speed_check;

    if(value)
    {
        write_data(handler, value);
        usleep(WRITE_SLEEP);
        read_data(handler, &speed_check);
        if(speed_check != value) {
            usleep(WRITE_SLEEP);
            write_data(handler, value);
        }
    }
}

static void pim_leds_mgmt(int slot)
{
    static sfp_status_t sfp[MAX_IM8724_MODULES][MAX_PORTS_PER_MODULE];
    static uint8_t first_time[MAX_IM8724_MODULES] = {1, 1};
    unsigned long sfp_pres, sfp_rxlos, speed, media_type;
    long temperature;
    uint8_t speed_1g, activity;
    int port, tx_force_disable;

    debug(4, "Start pim%d leds processing... ", slot);
    debug(5, "\n");
    /* Link/Act led: off - link down, green - link up, blinking green indicates activity *
     * Speed led: off - link down, orange - 10G, yellow - 1G */
    if (first_time[slot]) {
        for (port = 0; port < get_max_slot_ports(slot); port++) {
            sfp[slot][port].media_type = PORT_MEDIA_UNKNOWN;
            sfp[slot][port].sfp_rxlos = -1;
            sfp[slot][port].sfp_pres = -1;
            sfp[slot][port].activity = ACTIVITY_UNDEF;
            sfp[slot][port].speed = SPEED_UNDEF;
            sfp[slot][port].sfp_tx_force_disable = 0;
            sfp[slot][port].delay = 0;
        }
        first_time[slot] = 0;
    }
    for (port = 0; port < get_max_slot_ports(slot); port++) {
        speed_1g = activity = 0;
        read_data(pim_led_info[slot][port].sfp_pres, &sfp_pres);
        if (sfp_pres == 0) {
            if (sfp[slot][port].sfp_pres != sfp_pres) {
                write_data(pim_led_info[slot][port].led_act, LED_CNTRL_OFF);
                write_data(pim_led_info[slot][port].led_col, LED_CLR_OFF);
                sfp[slot][port].activity = ACTIVITY_UNDEF;
                sfp[slot][port].speed = SPEED_UNDEF;
                sfp[slot][port].sfp_tx_force_disable = 0;
            }
        } else { // sfp_pres == 1
            unsigned long port_speed =
                get_interface_qlm_speed(((0 - (((slot ? 8 : 0) + port) - 15)) / 4) + 2);

            read_data(pim_led_info[slot][port].media_type, &media_type);

            /* Force SFP+ speed to 10G, and check media type */
            /* Only for 10G modules */
            if((sfp[slot][port].sfp_pres != sfp_pres)
                && (io_card_sign_l[slot] == IO_CARD_13233_SIG_VALUE))
            {
                if(port_speed == 1) // 1G speed
                    media_type = PORT_MEDIA_1000X;

                if (sfp[slot][port].media_type != media_type) {
                    pim_port_check_updt(slot, port, media_type);
                    sfp[slot][port].media_type = media_type;
                }
                set_max_sfp_rate(pim_led_info[slot][port].sfp_speed, port_speed);
            }

            if(pim_led_info[slot][port].sfp_temperature && pim_led_info[slot][port].sfp_txdis) {
                  read_data_signed(pim_led_info[slot][port].sfp_temperature, &temperature);
                  tx_force_disable = (temperature > HI_TEMPERATURE);
                  if (sfp[slot][port].sfp_tx_force_disable != tx_force_disable) {
                        write_data(pim_led_info[slot][port].sfp_txdis, tx_force_disable ? 0x0f : 0x0);
                        //SFF-8636. Disabling 4 lanes in case of QSFP. In case of SFP module only lowest bit matters
                        sfp[slot][port].sfp_tx_force_disable = tx_force_disable;
                  }
            }

            read_data(pim_led_info[slot][port].sfp_rxlos, &sfp_rxlos);
            if (sfp_rxlos == 1) {
                if (sfp[slot][port].sfp_rxlos != sfp_rxlos) {
                    write_data(pim_led_info[slot][port].led_act, LED_CNTRL_OFF);
                    write_data(pim_led_info[slot][port].led_col, LED_CLR_OFF);
                    sfp[slot][port].activity = ACTIVITY_UNDEF;
                    sfp[slot][port].speed = SPEED_UNDEF;
                }
            } else {
                if (sfp[slot][port].delay-- < 0) {
                    read_data(pim_led_info[slot][port].sfp_speed, &speed);

                    if((port_speed != speed) && !sfp_mm_allow) { // interface and sfp type mismatch
                        speed = SPEED_UNDEF;
                        activity = ACTIVITY_UNDEF;
                    } else {
                        activity = get_pim_interface_activity(slot, port);
                        speed_1g = (port_speed == SPEED_1G);
                    }

                    if (sfp[slot][port].speed != speed) {

                        write_data(pim_led_info[slot][port].led_col,
                            (speed == SPEED_UNDEF) ? LED_CLR_OFF
                                : (speed_1g ? LED_CLR_YELLOW : LED_CLR_ORANGE));
                        sfp[slot][port].speed = speed;
                    }

                    if (sfp[slot][port].activity != activity) {
                        write_data(pim_led_info[slot][port].led_act,
                            (activity == ACTIVITY_UNDEF) ? LED_CNTRL_OFF
                                : (activity == 0 ? LED_CNTRL_ON : LED_CNTRL_BLINKING));
                        sfp[slot][port].activity = activity;
                    }

                    sfp[slot][port].delay = 10;
                }
            }
            sfp[slot][port].sfp_rxlos = sfp_rxlos;
        }
        sfp[slot][port].sfp_pres = sfp_pres;

        if(sfp_pres) {
            debug(5, "  port: %d:%d, speed: %s, activity: %s\n",
                slot, port, speed_1g ? "1G" : "10G", activity ? "on" : "off");
        } else {
            debug(5, "  port: %d:%d: no SFP\n", slot, port);
        }
    }
    debug(4, "[done]\n");
}

static int fp_led_init(void)
{
    char buf[256];
    int port;

    debug(3, "Start fp leds init... ");
    for (port = 0; port < fp_ports_num; port++) {
        sprintf(buf, FP_SFP_PRES_FMT, port);
        fp_led_info[port].sfp_pres = open(buf, O_RDONLY);
        if (fp_led_info[port].sfp_pres < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }
        sprintf(buf, FP_SFP_RXLOS_FMT, port);
        fp_led_info[port].sfp_rxlos = open(buf, O_RDONLY);
        if (fp_led_info[port].sfp_rxlos < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }
        sprintf(buf, FP_SFP_SPEED_FMT, port);
        fp_led_info[port].sfp_speed = open(buf, O_RDWR);
        if (fp_led_info[port].sfp_speed < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }

        sprintf(buf, FP_SFP_TXDIS_FMT, port);
        fp_led_info[port].sfp_txdis = open(buf, O_RDWR);
        if (fp_led_info[port].sfp_txdis < 0)
            debug(1, "Error: can't open %s file!\n", buf);

        sprintf(buf, FP_SFP_TEMP_FMT, port);
        fp_led_info[port].sfp_temperature = open(buf, O_RDONLY);
        if (fp_led_info[port].sfp_temperature < 0)
            debug(1, "Error: can't open %s file!\n", buf);

        if ((mgmt_mode & MGMT_OPT_FP_PHYS)) {
            sprintf(buf, FP_LED_ACT_FMT, port);
            fp_led_info[port].led_act = open(buf, O_RDWR);
            if (fp_led_info[port].led_act < 0) {
                debug(1, "Error: can't open %s file!\n", buf);
                return -1;
            }
            write_data(fp_led_info[port].led_act, LED_CNTRL_OFF);
        }
        /* SW fp led speed monitoring is unconditional */
        sprintf(buf, FP_LED_COL_FMT, port);
        fp_led_info[port].led_col = open(buf, O_RDWR);
        if (fp_led_info[port].led_col < 0) {
            debug(1, "Error: can't open %s file!\n", buf);
            return -1;
        }
        write_data(fp_led_info[port].led_col, LED_CLR_OFF);
    }
    debug(3, "[%s]\n", (mgmt_mode & MGMT_OPT_FP_PHYS) ? "done" : "skipped");
    return 0;
}

static void fp_leds_mgmt(void)
{
    static sfp_status_t sfp[MAX_FRONTPANEL_PORTS];
    static uint8_t first_time = 1;
    unsigned long sfp_pres, sfp_rxlos, speed;
    long temperature;
    uint8_t speed_1g, activity;
    int port, tx_force_disable;

    debug(4, "Start fp leds processing... ");
    debug(5, "\n");
    if (first_time) {
        for (port = 0; port < fp_ports_num; port++) {
            sfp[port].sfp_rxlos = -1;
            sfp[port].sfp_pres = -1;
            sfp[port].speed = SPEED_UNDEF;
            sfp[port].sfp_tx_force_disable = 0;
            sfp[port].activity = ACTIVITY_UNDEF;
        }
        first_time = 0;
    }
    /* Link/Act led: off - link down, green - link up, blinking green indicates activity *
     * Speed led: off - link down, orange - 10G, yellow - 1G */
    for (port = 0; port < fp_ports_num; port++) {
        speed_1g = activity = 0;
        read_data(fp_led_info[port].sfp_pres, &sfp_pres);
        if (sfp_pres == 0) {
            if (sfp[port].sfp_pres != sfp_pres) {
                write_data(fp_led_info[port].led_act, LED_CNTRL_OFF);
                write_data(fp_led_info[port].led_col, LED_CLR_OFF);
                sfp[port].speed = SPEED_UNDEF;
                sfp[port].sfp_tx_force_disable = 0;
                sfp[port].activity = ACTIVITY_UNDEF;
                sfp[port].delay = 0;
            }
        } else {
            unsigned long port_speed = get_interface_qlm_speed(port / 2);
            /* Force SFP+ speed to 10G */
            if (sfp[port].sfp_pres != sfp_pres)
                set_max_sfp_rate(fp_led_info[port].sfp_speed, port_speed);

            if(fp_led_info[port].sfp_temperature && fp_led_info[port].sfp_txdis) {
                read_data_signed(fp_led_info[port].sfp_temperature, &temperature);
                tx_force_disable = (temperature > HI_TEMPERATURE);
                if (sfp[port].sfp_tx_force_disable != tx_force_disable) {
                    write_data(fp_led_info[port].sfp_txdis, tx_force_disable);
                    sfp[port].sfp_tx_force_disable = tx_force_disable;
                }
            }

            read_data(fp_led_info[port].sfp_rxlos, &sfp_rxlos);
            if (sfp_rxlos == 1) {
                if (sfp[port].sfp_rxlos != sfp_rxlos) {
                    write_data(fp_led_info[port].led_act, LED_CNTRL_OFF);
                    write_data(fp_led_info[port].led_col, LED_CLR_OFF);
                    sfp[port].speed = SPEED_UNDEF;
                    sfp[port].activity = ACTIVITY_UNDEF;
                }
            } else {
                if (sfp[port].delay-- < 0) {
                    read_data(fp_led_info[port].sfp_speed, &speed);
                    if((port_speed != speed) && !sfp_mm_allow) { // interface and sfp type mismatch
                        speed = SPEED_UNDEF;
                        activity = ACTIVITY_UNDEF;
                    } else {
                        activity = get_fp_interface_activity(port);
                        speed_1g = (port_speed == SPEED_1G);
                    }

                    if (sfp[port].speed != speed) {
                        write_data(fp_led_info[port].led_col,
                            (speed == SPEED_UNDEF) ? LED_CLR_OFF
                                : (speed_1g ? LED_CLR_YELLOW : LED_CLR_ORANGE));
                        sfp[port].speed = speed;
                    }

			  if (sfp[port].activity != activity) {
				write_data(fp_led_info[port].led_act,
				    (activity == ACTIVITY_UNDEF) ? LED_CNTRL_OFF
					  : (activity == 0 ? LED_CNTRL_ON : LED_CNTRL_BLINKING));
				sfp[port].activity = activity;
			}
                    sfp[port].delay = 10;
                }
            }
            sfp[port].sfp_rxlos = sfp_rxlos;
        }
        sfp[port].sfp_pres = sfp_pres;
        if (dbg_mode == 5 && port == 0) printf("\n");
        debug(5, "  port: %d, speed: %s", port, speed_1g ? "1G" : "10G");
        if ((mgmt_mode & MGMT_OPT_FP_PHYS)) {
            debug(5, ", activity: %s", activity ? "on" : "off");
        }
        debug(5, "\n");
    }
    debug(4, "[done]\n");
}

static int mgmt_led_init(void)
{
    debug(3, "Start mgmt led init... ");
    if ((mgmt_mode & MGMT_OPT_MGMT_PHYS)) {
        mgmt_led_info.led_act = open(MGMT_LED_ACT, O_RDWR);
        if (mgmt_led_info.led_act < 0) {
            debug(1, "Error: can't open %s file!\n", MGMT_LED_ACT);
            return -1;
        }
        mgmt_led_info.rx_stats = open(MGMT_IFACE_RX_STATS, O_RDONLY);
        if (mgmt_led_info.rx_stats < 0) {
            debug(1, "Error: can't open %s file!\n", MGMT_IFACE_RX_STATS);
            return -1;
        }
        mgmt_led_info.tx_stats = open(MGMT_IFACE_TX_STATS, O_RDONLY);
        if (mgmt_led_info.tx_stats < 0) {
            debug(1, "Error: can't open %s file!\n", MGMT_IFACE_TX_STATS);
            return -1;
        }
    }
    /* SW mgmt led speed monitoring is unconditional */
    mgmt_led_info.led_col = open(MGMT_LED_COL, O_RDWR);
    if (mgmt_led_info.led_col < 0) {
        debug(1, "Error: can't open %s file!\n", MGMT_LED_COL);
        return -1;
    }
    debug(3, "[%s]\n", (mgmt_mode & MGMT_OPT_MGMT_PHYS) ? "done" : "skipped");
    return 0;
}

static void set_mgmt_led(void)
{
    static unsigned long mgmt_rx_stats, mgmt_tx_stats;
    static uint8_t mgmt_speed = SPEED_UNDEF;
    static int mgmt_status = 0xff;
    unsigned long rx_stats, tx_stats;
    uint8_t speed;
    int status;

    debug(4, "Start mgmt led processing... ");
    debug(5, "\n");
    /* Link/Act led: off - link down, green - link up, blinking green indicates activity *
     * Speed led: off - link down, green - 100MG, yellow - 1G */
    speed = get_mgmt_interface_speed();
    if (mgmt_speed != speed)
        write_data(mgmt_led_info.led_col, speed == SPEED_1G ?
                         LED_CLR_YELLOW : (speed == SPEED_100M) ? LED_CLR_GREEN : LED_CLR_OFF);
    mgmt_speed = speed;
    if ((mgmt_mode & MGMT_OPT_MGMT_PHYS)) {
        if ((status = get_mgmt_interface_status()) < 1) {
            if ( mgmt_status != status)
                write_data(mgmt_led_info.led_act, LED_CNTRL_OFF);
        } else {
            read_data(mgmt_led_info.rx_stats, &rx_stats);
            read_data(mgmt_led_info.tx_stats, &tx_stats);
            if (rx_stats != mgmt_rx_stats || tx_stats != mgmt_tx_stats) {
                write_data(mgmt_led_info.led_act, LED_CNTRL_BLINKING);
                mgmt_rx_stats = rx_stats;
                mgmt_tx_stats = tx_stats;
            } else
                write_data(mgmt_led_info.led_act, LED_CNTRL_ON);
        }
        mgmt_status = status;
    }
    debug(4, "[done]\n");
}

static void set_pim_status_led(int slot)
{
    static uint8_t clr[MAX_IM8724_MODULES], lc[MAX_IM8724_MODULES];
    unsigned long tm_overt, tm_alert;
    uint8_t c_clr, c_lc;

    read_data(sys_leds.pim_tm_alert[slot], &tm_alert);
    read_data(sys_leds.pim_tm_overt[slot], &tm_overt);

    if (tm_alert || tm_overt) {
        if(tm_overt && !dont_shutdown_on_temp) {
            if(pim_power_info[slot].io_card_pe) {
                write_data(pim_power_info[slot].io_card_pe, 0x00);
            }
            usleep(WRITE_SLEEP);
            if(pim_power_info[slot].io_card_hp_pe) {
                write_data(pim_power_info[slot].io_card_hp_pe, 0x00);
            }
        }
        c_clr = LED_CLR_RED;
        c_lc = LED_CNTRL_BLINKING;
    } else {
        c_clr = LED_CLR_GREEN;
        c_lc = LED_CNTRL_ON;
    }

    if (c_clr != clr[slot] || c_lc != lc[slot]) {
        write_data(sys_leds.pim_sys_led_col[slot], c_clr);
        write_data(sys_leds.pim_sys_led_mode[slot], c_lc);
        lc[slot] = c_lc; clr[slot] = c_clr;
    }
}

static void set_boardm_status_led(void)
{
    static uint8_t clr[3], lc[3], init[2];
    unsigned long fan_alrm = 0, tm_overt, tm_alert, pwr;
    uint8_t c_clr, c_lc;
    int indx;

    /* Sys leds mgmt */
    read_data(sys_leds.temp_ev, &tm_alert);
    read_data(sys_leds.overtemp_ev, &tm_overt);
    if (sys_leds.fan_ev > 0) {
        for (indx = 0; indx < 2*FANS_NUMBER; indx++) {
            if(sys_leds.fan_alarm[indx])
                read_data(sys_leds.fan_alarm[indx], &fan_alrm);
            if (fan_alrm) break;
        }
    }
    else
        read_data(sys_leds.fan_ev, &fan_alrm);

    if (tm_overt || tm_alert) {
        c_clr = LED_CLR_RED;
        c_lc = LED_CNTRL_BLINKING;
    } else if (fan_alrm) {
        c_clr = LED_CLR_RED;
        c_lc = LED_CNTRL_ON;
    } else {
        c_clr = LED_CLR_GREEN;
        c_lc = LED_CNTRL_ON;
    }

    if (c_clr != clr[2] || c_lc != lc[2]) {
        write_data(sys_leds.sys_led_col, c_clr);
        write_data(sys_leds.sys_led_mode, c_lc);
        lc[2] = c_lc; clr[2] = c_clr;
    }

    /* Power leds mgmt */
    for (indx = 0; indx < 2; indx++) {
         if (sys_leds.pwr_pres[indx] == 0) {
             c_clr = LED_CLR_GREEN;
             c_lc = LED_CNTRL_OFF;
         }
         else {
             read_data(sys_leds.pwr_ok[indx], &pwr);
             if (pwr == 0) {
                 read_data(sys_leds.pwr_int[indx], &pwr);
                 c_clr = pwr ? LED_CLR_RED : LED_CLR_GREEN;
                 c_lc = pwr ? LED_CNTRL_ON : LED_CNTRL_OFF;
             } else {
                 c_clr = LED_CLR_GREEN;
                 c_lc = LED_CNTRL_ON;
             }
         }
         if (c_clr != clr[indx] || c_lc != lc[indx] || !init[indx]) {
             write_data(sys_leds.pwr_led_col[indx], c_clr);
             write_data(sys_leds.pwr_led_mode[indx], c_lc);
             lc[indx] = c_lc; clr[indx] = c_clr; init[indx] = 1;
         }
    }
}

static void system_leds_init(void)
{
    int slot, indx, fd;
    char buf[256];

    debug(3, "Start system leds mgmt init... ");
    sys_leds.fan_ev = open(FAN_STATUS_EV, O_RDONLY);
    if ((fd = open(FAN_ENABLE_EV, O_RDWR)) > 0) {
        write_data(fd, 1);
        close(fd);
    }
    else if (sys_leds.fan_ev > 0){
        close(sys_leds.fan_ev);
        sys_leds.fan_ev = -1;
    }
    sys_leds.temp_ev = open(TEMP_ALERT_EV, O_RDONLY);
    if ((fd = open(TEMP_ALERT_ENABLE, O_RDWR)) > 0) {
        write_data(fd, 1);
        close(fd);
    }
    else if (sys_leds.temp_ev > 0){
        close(sys_leds.temp_ev);
        sys_leds.temp_ev = -1;
    }
    sys_leds.overtemp_ev = open(OVER_TEMP_EV, O_RDONLY);
    if ((fd = open(OVER_TEMP_ENABLE, O_RDWR)) > 0) {
        write_data(fd, 1);
        close(fd);
    }
    else if (sys_leds.overtemp_ev > 0){
        close(sys_leds.overtemp_ev);
        sys_leds.overtemp_ev = -1;
    }

    {
        char local_buf[256];
        int init_fan_alarm_ctrl = find_i2c_device("fan", local_buf, sizeof(local_buf));

        for (indx = 1; indx <= FANS_NUMBER; indx++) {
            if(init_fan_alarm_ctrl) {
                sprintf(buf, FAN1_ALARM_CNTRL, local_buf, indx);
                sys_leds.fan_alarm[indx-1] = open(buf, O_RDONLY);
                sprintf(buf, FAN2_ALARM_CNTRL, local_buf, indx);
                sys_leds.fan_alarm[indx+FANS_NUMBER-1] = open(buf, O_RDONLY);
            } else {
                sys_leds.fan_alarm[indx-1] = 0;
                sys_leds.fan_alarm[indx+FANS_NUMBER-1] = 0;
            }
        }
    }

    sys_leds.sys_led_col = open(SYS_LED_COLOR, O_RDWR);
    sys_leds.sys_led_mode = open(SYS_LED_MODE, O_RDWR);
    for (indx = 0; indx < 2; indx++) {
        sprintf(buf, PS_PRESENT, indx);
        sys_leds.pwr_pres[indx] = open(buf, O_RDONLY);
        sprintf(buf, PS_OK, indx);
        sys_leds.pwr_ok[indx] = open(buf, O_RDONLY);
        sprintf(buf, PS_INT, indx);
        sys_leds.pwr_int[indx] = open(buf, O_RDONLY);
        sprintf(buf, PWR_LED_COLOR, indx);
        sys_leds.pwr_led_col[indx] = open(buf, O_RDWR);
        sprintf(buf, PWR_LED_MODE, indx);
        sys_leds.pwr_led_mode[indx] = open(buf, O_RDWR);
    }
    set_boardm_status_led();

    for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
        if (!(io_cards_presents & (1 << slot)))
            continue;

        sprintf(buf, PIM_TEMP_EN, slot);
        if ((fd = open(buf, O_RDWR)) < 0)
            continue;
        write_data(fd, 1);
        close(fd);

        sprintf(buf, PIM_SYSLED_COLOR, slot);
        sys_leds.pim_sys_led_col[slot] = open(buf, O_RDWR);
        sprintf(buf, PIM_SYSLED_MODES, slot);
        sys_leds.pim_sys_led_mode[slot] = open(buf, O_RDWR);

        sprintf(buf, PIM_TM_ALERT, slot);
        sys_leds.pim_tm_alert[slot] = open(buf, O_RDONLY);
        sprintf(buf, PIM_TM_OVERT, slot);
        sys_leds.pim_tm_overt[slot] = open(buf, O_RDONLY);
        sprintf(buf, PIM_SW_RST, slot);
        sys_leds.pim_sw_rst[slot] = open(buf, O_RDWR);

        set_pim_status_led(slot);
    }
    debug(3, "[done]\n");
}

static void system_leds_mgmt(void)
{
    int slot;

    debug(4, "Start system leds processing... ");
    set_boardm_status_led();
    for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
         if (!(io_cards_presents & (1 << slot)))
             continue;
         set_pim_status_led(slot);
    }
    debug(4, "[done]\n");
}

static void main_board_leds_cfg(void)
{
    int fd;

    /* Speed indication always is done in SW mode */
    if ((fd = open(MAIN_BOARD_LEDS_CFG, O_RDWR)) > 0) {
        write_data(fd, 0xa);
        close(fd);
    }

    if ((fd = open(BOARD_FP_ACT_CFG, O_RDWR)) > 0) {
        write_data(fd, (mgmt_mode & MGMT_OPT_FP_PHYS) ? 0x1 : 0x0);
        close(fd);
    }

    if (!(mgmt_mode & MGMT_OPT_FP_PHYS))
        set_fp_interface_mode();

    if (sgmii_mode) {
        if ((fd = open(BOARD_MGMT_ACT_CFG, O_RDWR)) > 0) {
            write_data(fd, (mgmt_mode & MGMT_OPT_MGMT_PHYS) ? 0x1 : 0x0);
            close(fd);
        }

        if (!(mgmt_mode & MGMT_OPT_MGMT_PHYS))
            set_mgmt_interface_mode();
    }
}

int main (int argc, char** argv)
{
    int cs4318_phy;
    int slot;

    parse_args(argc,argv);

    cvmx_linux_enable_xkphys_access(1);

    if (mode == MODE_READ) {
        oct_rd(reg);
        return 0;
    }
    else if (mode == MODE_WRITE) {
        oct_wr(reg, val);
        return 0;
    }

    sgmii_mode = is_sgmii_board_mode();
    cs4318_phy = is_cs4318_phy();

    pim_set_defines();

    if (pim_led_init() < 0)
        return -1;

    if (fp_led_init() < 0)
        return -1;

    if (sgmii_mode && mgmt_led_init() < 0)
        return -1;

    system_leds_init();

    main_board_leds_cfg();

    if (!io_cards_presents && !(mgmt_mode & (MGMT_OPT_FP_PHYS | MGMT_OPT_MGMT_PHYS))) {
        debug(1, "Stopped leds management service, there isn't leds in SW mgmt mode!\n");
        return -1;
    }

    if (daemonize) {
        daemon_init();
        respawn_as_child();
    }
    else {
        signal(SIGINT, sighand);
        signal(SIGHUP, sighand);
    }
    signal(SIGTERM, sighand);

    while (!termination_requested) {
        if (io_cards_presents && (mgmt_mode & MGMT_OPT_PIM_PHYS)) {
            for (slot = 0; slot < MAX_IM8724_MODULES; slot++) {
                if (!(io_cards_presents & (1 << slot)))
                    continue;
                pim_leds_mgmt(slot);
            }
        }

        if (cs4318_phy && (mgmt_mode & MGMT_OPT_FP_PHYS))
            fp_leds_mgmt();

        if (sgmii_mode)
            set_mgmt_led();

        system_leds_mgmt();
        usleep(500);
    }
    return 0;
}
