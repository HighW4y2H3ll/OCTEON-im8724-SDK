#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>

#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-helper.h>
#include <asm/octeon/cvmx-helper-board.h>

#define N840_UART_MAJOR_NUM	97

#define IOCTL_N840_CPUID	_IOR(N840_UART_MAJOR_NUM, 0, int)

#define N840_DEVICE_NAME	"n840-uart"

#define ARR_SIZE(x)		(sizeof(x) / sizeof((x)[0]))

extern int32_t n840_do_mcu_cmd(uint8_t cmd, uint8_t arg1, uint8_t arg2);
extern uint8_t n840_get_cpuid(void);

typedef struct {
	int ilen;
	int olen;
} octcpu_cmd_t;

octcpu_cmd_t octcpu_cmds[] = 
{
	{ /*0x00,OCTCMD_CMD_NONE*/		0, 0, },
	{ /*0x01,OCTCMD_CPLD_GET*/		2, 1, },
	{ /*0x02,OCTCMD_CPLD_SET*/		3, 0, },
	{ /*0x03,OCTCMD_CPU_RESET*/		1, 0, },
	{ /*0x04,OCTCMD_TEMP_GET*/		2, 1, },
	{ /*0x05,OCTCMD_SYSLED_GET*/		2, 1, },
	{ /*0x06,OCTCMD_SYSLED_SET*/		2, 0, },
	{ /*0x07,OCTCMD_FAN_GET*/		2, 1, },
	{ /*0x08,OCTCMD_TLIMIT_GET*/		2, 1, },
	{ /*0x09,OCTCMD_TLIMIT_SET*/		3, 0, },
	{ /*0x0a,OCTCMD_CMD_NONE*/		0, 0, },
	{ /*0x0b,OCTCMD_CMD_NONE*/		0, 0, },
	{ /*0x0c,OCTCMD_HB_MODE_GET*/		1, 1, },
	{ /*0x0d,OCTCMD_HB_MODE_SET*/		2, 0, },
	{ /*0x0e,OCTCMD_HB_TIMEOUT_GET*/	1, 1, },
	{ /*0x0f,OCTCMD_HB_TIMEOUT_SET*/	2, 0, },
	{ /*0x10,OCTCMD_CMD_NONE*/		0, 0, },
	{ /*0x11,OCTCMD_PSU_STATUS_GET*/	1, 1, },
};

static int n840_mcu_open(struct inode *, struct file *);
static int n840_mcu_close(struct inode *, struct file *);
static long n840_mcu_ioctl(struct file *, unsigned int ioctl_num, unsigned long ioctl_param);
static ssize_t n840_mcu_read(struct file *, char *, size_t, loff_t *);
static ssize_t n840_mcu_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations n840_fops =
{
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = n840_mcu_open,
	.release = n840_mcu_close,
	.read = n840_mcu_read,
	.write = n840_mcu_write,
	.unlocked_ioctl = n840_mcu_ioctl,
};

DEFINE_MUTEX(n840_mcu_uart_lock);
DEFINE_MUTEX(fds_guard);

#define N840_MCU_MAX_FDS 10

struct {
	struct file *f;
	int32_t res;
} n840_mcu_resp[N840_MCU_MAX_FDS];

static int fds_count = 0;

static inline int n840_add_fd(struct file *f)
{
	int i, rv = 0;
	if (mutex_lock_interruptible(&fds_guard)) {
		return -EINTR;
	}
	if (fds_count >= N840_MCU_MAX_FDS) {
		rv = -EBUSY;
		goto done;
	}
	for (i = 0; i < N840_MCU_MAX_FDS; i++) {
		if (n840_mcu_resp[i].f == NULL) {
			n840_mcu_resp[i].f = f;
			n840_mcu_resp[i].res = -EINVAL;
			fds_count++;
			goto done;
		}
	}
	rv = -EFAULT;
done:
	mutex_unlock(&fds_guard);
	return rv;
}

static inline int n840_del_fd(struct file *f)
{
	int i, rv = 0;
	if (mutex_lock_interruptible(&fds_guard)) {
		return -EINTR;
	}
	if (fds_count <= 0) {
		rv = -EINVAL;
		goto done;
	}
	for (i = 0; i < N840_MCU_MAX_FDS; i++) {
		if (n840_mcu_resp[i].f == f) {
			n840_mcu_resp[i].f = NULL;
			fds_count--;
			goto done;
		}
	}
	rv = -EINVAL;
done:
	mutex_unlock(&fds_guard);
	return rv;
}

static inline int32_t n840_get_resp_for_fd(struct file *f)
{
	int i;
	if (f == NULL) return -1;
	for (i = 0; i < N840_MCU_MAX_FDS; i++) {
		if (n840_mcu_resp[i].f == f) {
			int32_t res = n840_mcu_resp[i].res;
			n840_mcu_resp[i].res = -EINVAL;
			return res;
		}
	}
	return -EBUSY;
}

static inline int n840_set_resp_for_fd(struct file *f, int32_t res)
{
	int i;
	if (f == NULL) return -1;
	for (i = 0; i < N840_MCU_MAX_FDS; i++) {
		if (n840_mcu_resp[i].f == f) {
			n840_mcu_resp[i].res = res;
			return 0;
		}
	}
	return -EBUSY;
}

int32_t n840_do_mcu_cmd_sync(uint8_t cmd, uint8_t arg1, uint8_t arg2)
{
	int32_t resp = -EINTR;
	if (!mutex_lock_interruptible(&n840_mcu_uart_lock)) {
		/* TODO: N840 port resp = n840_do_mcu_cmd(cmd, arg1, arg2); */
		mutex_unlock(&n840_mcu_uart_lock);
	}
	return resp;
}
EXPORT_SYMBOL(n840_do_mcu_cmd_sync);

static int n840_mcu_open(struct inode *inod, struct file *f)
{
	return n840_add_fd(f);
}

static int n840_mcu_close(struct inode *inode, struct file *f)
{
	// unregister process fd
	return n840_del_fd(f);
}

static ssize_t n840_mcu_read(struct file* f, char *buff, size_t len, loff_t *off)
{
	if (len == 1) {
		int32_t res = n840_get_resp_for_fd(f);
		if (res != -1) {
			put_user((char)(0xff & res), buff++);
			return 1;
		}
		return -EIO;
	}
	return -EINVAL;
}

static ssize_t n840_mcu_write(struct file *f, const char *buff, size_t len, loff_t *off)
{
	static int err_count = 0;
	if (len > 0) {
		uint8_t cmd = buff[0];
		uint8_t arg1 = 0, arg2 = 0;
		int32_t res;
		if ((octcpu_cmds[cmd].ilen != len) || (cmd >= ARR_SIZE(octcpu_cmds))) {
			return -EINVAL;
		}
		if (len > 1) {
			arg1 = buff[1];
		}
		if (len > 2) {
			arg2 = buff[2];
		}
		res = n840_do_mcu_cmd_sync(cmd, arg1, arg2);
		// request failed, return error
		if (res < 0) {
			err_count++;
			pr_debug("MCU/CPLD IO error: errno is '%d'\n", res);
			return res;
		}
		// return request result
		if (octcpu_cmds[cmd].olen == 1) {
			n840_set_resp_for_fd(f, res);
		}
		return len;
	}
	return -EINVAL;
}

static long n840_mcu_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int cpuid = 0; /* TODO: port N840 n840_get_cpuid(); */
	int rv = 0;
	if (ioctl_num == IOCTL_N840_CPUID) {
		put_user(cpuid, (int*)ioctl_param);
	} else {
		rv = -EINVAL;
	}
	return rv;
}

static int __init n840_init(void)
{
	int rc = 0;
	if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_CUST_N840)
		return rc;
	rc = register_chrdev(N840_UART_MAJOR_NUM, N840_DEVICE_NAME, &n840_fops);
	if (rc < 0) pr_err("N840 MCU UART registration failed\n");
	pr_info("N840 MCU module was created\n");
	return rc;
}

static void __exit n840_release(void)
{
	if (cvmx_sysinfo_get()->board_type != CVMX_BOARD_TYPE_CUST_N840)
		return;
	unregister_chrdev(N840_UART_MAJOR_NUM, N840_DEVICE_NAME);
}

module_init(n840_init);
module_exit(n840_release);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Interface Masters <support@interfacemasters.com>");
MODULE_DESCRIPTION("Niagara CPLD access driver");
