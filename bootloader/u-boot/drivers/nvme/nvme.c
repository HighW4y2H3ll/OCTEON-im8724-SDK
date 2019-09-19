/*
 * Copyright (C) 2017 NXP Semiconductors
 * Copyright (C) 2017 Bin Meng <bmeng.cn@gmail.com>
 * Copyright (C) 2017 Aaron Williams <aaron.williams@cavium.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <pci.h>
#include <watchdog.h>
#include <linux/types.h>
#include <linux/compat.h>
#ifdef CONFIG_DM
#include <dm/device-internal.h>
#endif
#include "nvme.h"

#ifndef CONFIG_DM
extern block_dev_desc_t nvme_dev_desc[CONFIG_SYS_NVME_MAX_DEVICE];
struct nvme_dev nvme_devs[CONFIG_SYS_NVME_MAX_DEVICE];
struct nvme_ns nvme_nses[CONFIG_SYS_NVME_MAX_DEVICE];
int nvme_last_device_idx = 0;
#endif

#define NVME_Q_DEPTH		2
#define NVME_AQ_DEPTH		2
#define NVME_SQ_SIZE(depth)	(depth * sizeof(struct nvme_command))
#define NVME_CQ_SIZE(depth)	(depth * sizeof(struct nvme_completion))
#define ADMIN_TIMEOUT		60
#define IO_TIMEOUT		30
#define MAX_PRP_POOL		512

/*
 * An NVM Express queue. Each device has at least two (one for admin
 * commands and one for I/O commands).
 */
struct nvme_queue {
	struct nvme_dev *dev;
	struct nvme_command *sq_cmds;
	struct nvme_completion *cqes;
	wait_queue_head_t sq_full;
	u32 __iomem *q_db;
	u16 q_depth;
	s16 cq_vector;
	u16 sq_head;
	u16 sq_tail;
	u16 cq_head;
	u16 qid;
	u8 cq_phase;
	u8 cqe_seen;
	unsigned long cmdid_data[];
};

#ifndef CONFIG_DM
static int nvme_info_init(void)
{
	struct nvme_info *info = calloc(sizeof(*info), 1);

	info->ns_num = 0;
	info->ndev_num = 0;
	INIT_LIST_HEAD(&info->dev_list);
	nvme_info = info;

	return 0;
}
#endif

static int nvme_wait_ready(struct nvme_dev *dev, bool enabled)
{
	u32 bit = enabled ? NVME_CSTS_RDY : 0;
	ulong start = get_timer(0);
	bool wait;

	while ((wait = ((readl(&dev->bar->csts) & NVME_CSTS_RDY) != bit)) &&
	       get_timer(start) < 20000);
		udelay(10000);
#ifdef DEBUG
	if (wait)
		printf("%s: Timed out waiting for bit 0x%x: status: 0x%x\n",
		       __func__, bit, readl(&dev->bar->csts));
#endif
	return wait ? -EBUSY : 0;
}

static int nvme_setup_prps(struct nvme_dev *dev, u64 *prp2,
			   int total_len, u64 dma_addr)
{
	u32 page_size = dev->page_size;
	int offset = dma_addr & (page_size - 1);
	u64 *prp_pool;
	int length = total_len;
	int i, nprps;
	length -= (page_size - offset);

	debug("%s(%p, %p, %d, 0x%llx), length: %d, offset: %d\n",
	      __func__, dev, prp2,
	      total_len, dma_addr, length, offset);
	if (length <= 0) {
		*prp2 = 0;
		return 0;
	}

	if (length)
		dma_addr += (page_size - offset);

	if (length <= page_size) {
		*prp2 = dma_addr;
		return 0;
	}

	debug("%s: length: %d, page size: %u\n", __func__, length, page_size);
	nprps = DIV_ROUND_UP(length, page_size);

	if (nprps > dev->prp_entry_num) {
		free(dev->prp_pool);
		dev->prp_pool = malloc(nprps << 3);
		if (!dev->prp_pool) {
			printf("Error: malloc prp_pool failed to allocate %u bytes\n",
			       nprps << 3);
			return -ENOMEM;
		}
		dev->prp_entry_num = nprps;
	}

	prp_pool = dev->prp_pool;
	i = 0;
	while (nprps) {
		if (i == ((page_size >> 3) - 1)) {
			*(prp_pool + i) = cpu_to_le64(cvmx_ptr_to_phys(prp_pool +
								       page_size));
			i = 0;
			prp_pool += page_size;
		}
		*(prp_pool + i++) = cpu_to_le64(dma_addr);
		dma_addr += page_size;
		nprps--;
	}
	*prp2 = cvmx_ptr_to_phys(dev->prp_pool);

	return 0;
}

static __le16 nvme_get_cmd_id(void)
{
	static unsigned short cmdid;

	return cpu_to_le16((cmdid < USHRT_MAX) ? cmdid++ : 0);
}

static u16 nvme_read_completion_status(struct nvme_queue *nvmeq, u16 index)
{
	u16 result;
	u64 start = (ulong)&nvmeq->cqes[index];
	u64 stop = start + sizeof(struct nvme_completion);

	invalidate_dcache_range(start, stop);

	result = le16_to_cpu(nvmeq->cqes[index].status);
	debug("%s(%p, %u) (cqes at %p (0x%llx)): result: 0x%x\n",
	      __func__, nvmeq, index, &nvmeq->cqes[index].status,
	      cvmx_ptr_to_phys(&nvmeq->cqes[index]), result);
#ifdef DEBUG
	print_buffer(0, &nvmeq->cqes[index], 1, sizeof(nvmeq->cqes[index]), 0);
#endif
	return result;
}

/**
 * nvme_submit_cmd() - copy a command into a queue and ring the doorbell
 *
 * @nvmeq:	The queue to use
 * @cmd:	The command to send
 */
static void nvme_submit_cmd(struct nvme_queue *nvmeq, struct nvme_command *cmd)
{
	u16 tail = nvmeq->sq_tail;

	debug("%s(%p, %p)\n", __func__, nvmeq, cmd);
	memcpy(&nvmeq->sq_cmds[tail], cmd, sizeof(*cmd));
#ifdef DEBUG
	printf("Command:\n");
	print_buffer(0, cmd, 1, sizeof(*cmd), 0);
#endif
	flush_dcache_range((ulong)&nvmeq->sq_cmds[tail],
			   (ulong)&nvmeq->sq_cmds[tail] + sizeof(*cmd));

	if (++tail == nvmeq->q_depth)
		tail = 0;
	debug("%s: Writing tail %u to queue at %p\n", __func__, tail,
	      nvmeq->q_db);
	writel(tail, nvmeq->q_db);
	nvmeq->sq_tail = tail;
}

static int nvme_submit_sync_cmd(struct nvme_queue *nvmeq,
				struct nvme_command *cmd,
				u32 *result, unsigned timeout)
{
	u16 head = nvmeq->cq_head;
	u16 phase = nvmeq->cq_phase;
	u16 status;
	ulong start_time;
	ulong timeout_us = timeout * 100000;

	debug("%s(%p, %p, %p, %u)\n", __func__, nvmeq, cmd, result, timeout);
	cmd->common.command_id = nvme_get_cmd_id();
	nvme_submit_cmd(nvmeq, cmd);

	start_time = timer_get_us();

	for (;;) {
		status = nvme_read_completion_status(nvmeq, head);
		if ((status & 0x01) == phase)
			break;
		if (timeout_us > 0 && (timer_get_us() - start_time)
		    >= timeout_us) {
			debug("%s: command timed out\n", __func__);
			return -ETIMEDOUT;
		}
		WATCHDOG_RESET();
	}

	status >>= 1;
	if (status) {
		printf("ERROR: status = %x, phase = %d, head = %d\n",
		       status, phase, head);
		status = 0;
		if (++head == nvmeq->q_depth) {
			head = 0;
			phase = !phase;
		}
		writel(head, nvmeq->q_db + nvmeq->dev->db_stride);
		nvmeq->cq_head = head;
		nvmeq->cq_phase = phase;

		return -EIO;
	}

	if (result)
		*result = readl(&(nvmeq->cqes[head].result));

	if (++head == nvmeq->q_depth) {
		head = 0;
		phase = !phase;
	}
	writel(head, nvmeq->q_db + nvmeq->dev->db_stride);
	nvmeq->cq_head = head;
	nvmeq->cq_phase = phase;

	return status;
}

static int nvme_submit_admin_cmd(struct nvme_dev *dev, struct nvme_command *cmd,
				 u32 *result)
{
	debug("%s(%p, %p, %p)\n", __func__, dev, cmd, result);
	return nvme_submit_sync_cmd(dev->queues[0], cmd, result, ADMIN_TIMEOUT);
}

static struct nvme_queue *nvme_alloc_queue(struct nvme_dev *dev,
					   int qid, int depth)
{
	struct nvme_queue *nvmeq = malloc(sizeof(*nvmeq));
	if (!nvmeq)
		return NULL;
	memset(nvmeq, 0, sizeof(*nvmeq));

	nvmeq->cqes = (void *)memalign(4096, NVME_CQ_SIZE(depth));
	if (!nvmeq->cqes)
		goto free_nvmeq;
	memset((void *)nvmeq->cqes, 0, NVME_CQ_SIZE(depth));

	nvmeq->sq_cmds = (void *)memalign(4096, NVME_SQ_SIZE(depth));
	if (!nvmeq->sq_cmds)
		goto free_queue;
	memset((void *)nvmeq->sq_cmds, 0, NVME_SQ_SIZE(depth));

	nvmeq->dev = dev;

	nvmeq->cq_head = 0;
	nvmeq->cq_phase = 1;
	nvmeq->q_db = &dev->dbs[qid * 2 * dev->db_stride];
	nvmeq->q_depth = depth;
	nvmeq->qid = qid;
	dev->queue_count++;
	dev->queues[qid] = nvmeq;

	return nvmeq;

 free_queue:
	free((void *)nvmeq->cqes);
 free_nvmeq:
	free(nvmeq);

	return NULL;
}

static int nvme_delete_queue(struct nvme_dev *dev, u8 opcode, u16 id)
{
	struct nvme_command c;

	memset(&c, 0, sizeof(c));
	c.delete_queue.opcode = opcode;
	c.delete_queue.qid = cpu_to_le16(id);

	return nvme_submit_admin_cmd(dev, &c, NULL);
}

static int nvme_delete_sq(struct nvme_dev *dev, u16 sqid)
{
	return nvme_delete_queue(dev, nvme_admin_delete_sq, sqid);
}

static int nvme_delete_cq(struct nvme_dev *dev, u16 cqid)
{
	return nvme_delete_queue(dev, nvme_admin_delete_cq, cqid);
}

static int nvme_enable_ctrl(struct nvme_dev *dev)
{
	dev->ctrl_config &= ~NVME_CC_SHN_MASK;
	dev->ctrl_config |= NVME_CC_ENABLE;
	writel(dev->ctrl_config, &dev->bar->cc);

	return nvme_wait_ready(dev, true);
}

static int nvme_disable_ctrl(struct nvme_dev *dev)
{
	dev->ctrl_config &= ~NVME_CC_SHN_MASK;
	dev->ctrl_config &= ~NVME_CC_ENABLE;
	writel(dev->ctrl_config, &dev->bar->cc);

	return nvme_wait_ready(dev, false);
}

static void nvme_free_queue(struct nvme_queue *nvmeq)
{
	free((void *)nvmeq->cqes);
	free(nvmeq->sq_cmds);
	free(nvmeq);
}

static void nvme_free_queues(struct nvme_dev *dev, int lowest)
{
	int i;

	for (i = dev->queue_count - 1; i >= lowest; i--) {
		struct nvme_queue *nvmeq = dev->queues[i];
		dev->queue_count--;
		dev->queues[i] = NULL;
		nvme_free_queue(nvmeq);
	}
}

static void nvme_init_queue(struct nvme_queue *nvmeq, u16 qid)
{
	struct nvme_dev *dev = nvmeq->dev;

	nvmeq->sq_tail = 0;
	nvmeq->cq_head = 0;
	nvmeq->cq_phase = 1;
	nvmeq->q_db = &dev->dbs[qid * 2 * dev->db_stride];
	memset((void *)nvmeq->cqes, 0, NVME_CQ_SIZE(nvmeq->q_depth));
	flush_dcache_range((ulong)nvmeq->cqes,
			   (ulong)nvmeq->cqes + NVME_CQ_SIZE(nvmeq->q_depth));
	dev->online_queues++;
}

static int nvme_configure_admin_queue(struct nvme_dev *dev)
{
	int result;
	u32 aqa;
	u64 cap = nvme_readq(&dev->bar->cap);
	struct nvme_queue *nvmeq;
	/* most architectures use 4KB as the page size */
	unsigned page_shift = 12;
	unsigned dev_page_min = NVME_CAP_MPSMIN(cap) + 12;
	unsigned dev_page_max = NVME_CAP_MPSMAX(cap) + 12;

	debug("%s(%p) page min: %u, max: %u, cap: 0x%llx\n", __func__, dev,
	      dev_page_min, dev_page_max, cap);
	if (page_shift < dev_page_min) {
		debug("Device minimum page size (%u) too large for host (%u)\n",
		      1 << dev_page_min, 1 << page_shift);
		return -ENODEV;
	}

	if (page_shift > dev_page_max) {
		debug("Device maximum page size (%u) smaller than host (%u)\n",
		      1 << dev_page_max, 1 << page_shift);
		page_shift = dev_page_max;
	}

	debug("%s: disabling ctrl\n", __func__);
	result = nvme_disable_ctrl(dev);
	if (result < 0) {
		printf("%s: disable ctrl failed\n", __func__);
		return result;
	}

	nvmeq = dev->queues[0];
	if (!nvmeq) {
		debug("%s: allocating queue\n", __func__);
		nvmeq = nvme_alloc_queue(dev, 0, NVME_AQ_DEPTH);
		if (!nvmeq)
			return -ENOMEM;
	}

	aqa = nvmeq->q_depth - 1;
	aqa |= aqa << 16;
	aqa |= aqa << 16;

	dev->page_size = 1 << page_shift;
	debug("%s: Setting page size to %d\n", __func__, dev->page_size);

	dev->ctrl_config = NVME_CC_CSS_NVM;
	dev->ctrl_config |= (page_shift - 12) << NVME_CC_MPS_SHIFT;
	dev->ctrl_config |= NVME_CC_ARB_RR | NVME_CC_SHN_NONE;
	dev->ctrl_config |= NVME_CC_IOSQES | NVME_CC_IOCQES;

	debug("%s: writing aqa 0x%x to %p\n", __func__, aqa, &dev->bar->aqa);
	writel(aqa, &dev->bar->aqa);
	debug("%s: writing sq_cmds (0x%llx) to asq (%p)\n", __func__,
	      cvmx_ptr_to_phys(nvmeq->sq_cmds), &dev->bar->asq);
	nvme_writeq(cvmx_ptr_to_phys(nvmeq->sq_cmds), &dev->bar->asq);
	debug("%s: Writing cqes (0x%llx) to acq (%p)\n", __func__,
	      cvmx_ptr_to_phys(nvmeq->cqes), &dev->bar->acq);
	nvme_writeq(cvmx_ptr_to_phys(nvmeq->cqes), &dev->bar->acq);
	debug("%s: enabling ctrl\n", __func__);
	result = nvme_enable_ctrl(dev);
	debug("%s: enable returned %d\n", __func__, result);
	if (result)
		goto free_nvmeq;

	nvmeq->cq_vector = 0;
	debug("%s: Initializing queue 0\n", __func__);
	nvme_init_queue(dev->queues[0], 0);

	debug("%s: Returning %d\n", __func__, result);
	return result;

 free_nvmeq:
	nvme_free_queues(dev, 0);

	return result;
}

static int nvme_alloc_cq(struct nvme_dev *dev, u16 qid,
			    struct nvme_queue *nvmeq)
{
	struct nvme_command c;
	int flags = NVME_QUEUE_PHYS_CONTIG | NVME_CQ_IRQ_ENABLED;

	memset(&c, 0, sizeof(c));
	c.create_cq.opcode = nvme_admin_create_cq;
	c.create_cq.prp1 = cpu_to_le64(cvmx_ptr_to_phys(nvmeq->cqes));
	c.create_cq.cqid = cpu_to_le16(qid);
	c.create_cq.qsize = cpu_to_le16(nvmeq->q_depth - 1);
	c.create_cq.cq_flags = cpu_to_le16(flags);
	c.create_cq.irq_vector = cpu_to_le16(nvmeq->cq_vector);

	return nvme_submit_admin_cmd(dev, &c, NULL);
}

static int nvme_alloc_sq(struct nvme_dev *dev, u16 qid,
			    struct nvme_queue *nvmeq)
{
	struct nvme_command c;
	int flags = NVME_QUEUE_PHYS_CONTIG | NVME_SQ_PRIO_MEDIUM;

	memset(&c, 0, sizeof(c));
	c.create_sq.opcode = nvme_admin_create_sq;
	c.create_sq.prp1 = cpu_to_le64(cvmx_ptr_to_phys(nvmeq->sq_cmds));
	c.create_sq.sqid = cpu_to_le16(qid);
	c.create_sq.qsize = cpu_to_le16(nvmeq->q_depth - 1);
	c.create_sq.sq_flags = cpu_to_le16(flags);
	c.create_sq.cqid = cpu_to_le16(qid);

	return nvme_submit_admin_cmd(dev, &c, NULL);
}

int nvme_identify(struct nvme_dev *dev, unsigned nsid,
		  unsigned cns, dma_addr_t dma_addr)
{
	struct nvme_command c;
	u32 page_size = dev->page_size;
	int offset = dma_addr & (page_size - 1);
	int length = sizeof(struct nvme_id_ctrl);

	debug("%s: dma address: 0x%llx, command addr: %p\n", __func__,
	      dma_addr, &c);
	memset(&c, 0, sizeof(c));
	c.identify.opcode = nvme_admin_identify;
	c.identify.nsid = cpu_to_le32(nsid);
	c.identify.prp1 = cpu_to_le64(dma_addr);
	length -= (page_size - offset);
	if (length <= 0) {
		c.identify.prp2 = 0;
	} else {
		dma_addr += (page_size - offset);
		c.identify.prp2 = cpu_to_le64(dma_addr);
	}

	c.identify.cns = cpu_to_le32(cns);
	debug("%s: nsid: 0x%x, prp1: 0x%llx, prp2: 0x%llx, cns: 0x%x, length: %d\n",
	      __func__, c.identify.nsid, c.identify.prp1, c.identify.prp2,
	      c.identify.cns, length);

	return nvme_submit_admin_cmd(dev, &c, NULL);
}

int nvme_get_features(struct nvme_dev *dev, unsigned fid, unsigned nsid,
		      dma_addr_t dma_addr, u32 *result)
{
	struct nvme_command c;

	memset(&c, 0, sizeof(c));
	c.features.opcode = nvme_admin_get_features;
	c.features.nsid = cpu_to_le32(nsid);
	c.features.prp1 = cpu_to_le64(dma_addr);
	c.features.fid = cpu_to_le32(fid);

	return nvme_submit_admin_cmd(dev, &c, result);
}

int nvme_set_features(struct nvme_dev *dev, unsigned fid, unsigned dword11,
		      dma_addr_t dma_addr, u32 *result)
{
	struct nvme_command c;

	memset(&c, 0, sizeof(c));
	c.features.opcode = nvme_admin_set_features;
	c.features.prp1 = cpu_to_le64(dma_addr);
	c.features.fid = cpu_to_le32(fid);
	c.features.dword11 = cpu_to_le32(dword11);

	return nvme_submit_admin_cmd(dev, &c, result);
}

static int nvme_create_queue(struct nvme_queue *nvmeq, int qid)
{
	struct nvme_dev *dev = nvmeq->dev;
	int result;

	nvmeq->cq_vector = qid - 1;
	result = nvme_alloc_cq(dev, qid, nvmeq);
	if (result < 0)
		goto release_cq;

	result = nvme_alloc_sq(dev, qid, nvmeq);
	if (result < 0)
		goto release_sq;

	nvme_init_queue(nvmeq, qid);

	return result;

 release_sq:
	nvme_delete_sq(dev, qid);
 release_cq:
	nvme_delete_cq(dev, qid);

	return result;
}

static int nvme_set_queue_count(struct nvme_dev *dev, int count)
{
	int status;
	u32 result;
	u32 q_count = (count - 1) | ((count - 1) << 16);

	status = nvme_set_features(dev, NVME_FEAT_NUM_QUEUES,
			q_count, 0, &result);

	if (status < 0)
		return status;
	if (status > 1)
		return 0;

	return min(result & 0xffff, result >> 16) + 1;
}

static void nvme_create_io_queues(struct nvme_dev *dev)
{
	unsigned int i;

	for (i = dev->queue_count; i <= dev->max_qid; i++)
		if (!nvme_alloc_queue(dev, i, dev->q_depth))
			break;

	for (i = dev->online_queues; i <= dev->queue_count - 1; i++)
		if (nvme_create_queue(dev->queues[i], i))
			break;
}

static int nvme_setup_io_queues(struct nvme_dev *dev)
{
	int nr_io_queues;
	int result;

	nr_io_queues = 1;
	result = nvme_set_queue_count(dev, nr_io_queues);
	if (result <= 0)
		return result;

	if (result < nr_io_queues)
		nr_io_queues = result;

	dev->max_qid = nr_io_queues;

	/* Free previously allocated queues */
	nvme_free_queues(dev, nr_io_queues + 1);
	nvme_create_io_queues(dev);

	return 0;
}

static int nvme_get_info_from_identify(struct nvme_dev *dev)
{
	u16 vendor, device;
	struct nvme_id_ctrl buf, *ctrl = &buf;
	int ret;
	int shift = NVME_CAP_MPSMIN(nvme_readq(&dev->bar->cap)) + 12;

	ret = nvme_identify(dev, 0, 1, cvmx_ptr_to_phys(ctrl));
	debug("%s: nvme_identify returned %d\n", __func__, ret);
	if (ret)
		return -EIO;

	dev->nn = le32_to_cpu(ctrl->nn);
	dev->vwc = ctrl->vwc;
	memcpy(dev->serial, ctrl->sn, sizeof(ctrl->sn));
	memcpy(dev->model, ctrl->mn, sizeof(ctrl->mn));
	memcpy(dev->firmware_rev, ctrl->fr, sizeof(ctrl->fr));
	if (ctrl->mdts)
		dev->max_transfer_shift = (ctrl->mdts + shift);
	else {
		/*
		 * Maximum Data Transfer Size (MDTS) field indicates the maximum
		 * data transfer size between the host and the controller. The
		 * host should not submit a command that exceeds this transfer
		 * size. The value is in units of the minimum memory page size
		 * and is reported as a power of two (2^n).
		 *
		 * The spec also says: a value of 0h indicates no restrictions
		 * on transfer size. But in nvme_blk_read/write() below we have
		 * the following algorithm for maximum number of logic blocks
		 * per transfer:
		 *
		 * u16 lbas = 1 << (dev->max_transfer_shift - ns->lba_shift);
		 *
		 * In order for lbas not to overflow, the maximum number is 15
		 * which means dev->max_transfer_shift = 15 + 9 (ns->lba_shift).
		 * Let's use 20 which provides 1MB size.
		 */
		dev->max_transfer_shift = 20;
	}

	/* Apply quirk stuff */
#ifdef CONFIG_DM
	dm_pci_read_config16(dev->pdev, PCI_VENDOR_ID, &vendor);
	dm_pci_read_config16(dev->pdev, PCI_DEVICE_ID, &device);
#else
	pci_read_config_word(dev->pci_dev, PCI_VENDOR_ID, &vendor);
	pci_read_config_word(dev->pci_dev, PCI_DEVICE_ID, &device);
#endif
	if ((vendor == PCI_VENDOR_ID_INTEL) &&
	    (device == 0x0953) && ctrl->vs[3]) {
		unsigned int max_transfer_shift;
		dev->stripe_size = (ctrl->vs[3] + shift);
		max_transfer_shift = (ctrl->vs[3] + 18);
		if (dev->max_transfer_shift) {
			dev->max_transfer_shift = min(max_transfer_shift,
						      dev->max_transfer_shift);
		} else {
			dev->max_transfer_shift = max_transfer_shift;
		}
	}

	return 0;
}
#ifdef CONFIG_DM
int nvme_scan_namespace(void)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	ret = uclass_get(UCLASS_NVME, &uc);
	if (ret)
		return ret;

	uclass_foreach_dev(dev, uc) {
		ret = device_probe(dev);
		if (ret)
			return ret;
	}

	return 0;
}

static int nvme_blk_probe(struct udevice *udev)
{
	struct nvme_dev *ndev = dev_get_priv(udev->parent);
	struct blk_desc *desc = dev_get_uclass_platdata(udev);
	struct nvme_ns *ns = dev_get_priv(udev);
	u8 flbas;
	u16 vendor;
	struct nvme_id_ns buf, *id = &buf;

	memset(ns, 0, sizeof(*ns));
	ns->dev = ndev;
	ns->ns_id = desc->dev - ndev->blk_dev_start + 1;
	if (nvme_identify(ndev, ns->ns_id, 0, cvmx_ptr_to_phys(id)))
		return -EIO;

	flbas = id->flbas & NVME_NS_FLBAS_LBA_MASK;
	ns->flbas = flbas;
	ns->lba_shift = id->lbaf[flbas].ds;
	ns->mode_select_num_blocks = le64_to_cpu(id->nsze);
	ns->mode_select_block_len = 1 << ns->lba_shift;
	list_add(&ns->list, &ndev->namespaces);

	desc->lba = ns->mode_select_num_blocks;
	desc->log2blksz = ns->lba_shift;
	desc->blksz = 1 << ns->lba_shift;
	desc->bdev = udev;
	pci_read_word(ndev->pci_dev, PCI_VENDOR_ID, &vendor);
	sprintf(desc->vendor, "0x%.4x", vendor);
	memcpy(desc->product, ndev->serial, sizeof(ndev->serial));
	memcpy(desc->revision, ndev->firmware_rev, sizeof(ndev->firmware_rev));
	part_init(desc);

	return 0;
}

ulong nvme_blk_read(struct udevice *udev, lbaint_t blknr,
		    lbaint_t blkcnt, void *buffer)
{
	struct nvme_ns *ns = dev_get_priv(udev);
	struct nvme_dev *dev = ns->dev;
	struct nvme_command c;
	struct blk_desc *desc = dev_get_uclass_platdata(udev);
	int status;
	u64 prp2;
	u64 total_len = blkcnt << desc->log2blksz;
	u64 temp_len = total_len;

	u64 slba = blknr;
	u16 lbas = 1 << (dev->max_transfer_shift - ns->lba_shift);
	u64 total_lbas = blkcnt;

	c.rw.opcode = nvme_cmd_read;
	c.rw.flags = 0;
	c.rw.nsid = cpu_to_le32(ns->ns_id);
	c.rw.control = 0;
	c.rw.dsmgmt = 0;
	c.rw.reftag = 0;
	c.rw.apptag = 0;
	c.rw.appmask = 0;
	c.rw.metadata = 0;

	while (total_lbas) {
		if (total_lbas < lbas) {
			lbas = (u16)total_lbas;
			total_lbas = 0;
		} else {
			total_lbas -= lbas;
		}

		if (nvme_setup_prps
		   (dev, &prp2, lbas << ns->lba_shift, cvmx_ptr_to_phys(buffer)))
			return -EIO;
		c.rw.slba = cpu_to_le64(slba);
		slba += lbas;
		c.rw.length = cpu_to_le16(lbas - 1);
		c.rw.prp1 = cpu_to_le64(cvmx_ptr_to_phys(buffer));
		c.rw.prp2 = cpu_to_le64(cvmx_ptr_to_phys(prp2));
		status = nvme_submit_sync_cmd(dev->queues[1],
				&c, NULL, IO_TIMEOUT);
		if (status)
			break;
		temp_len -= lbas << ns->lba_shift;
		buffer += lbas << ns->lba_shift;
	}

	return (total_len - temp_len) >> desc->log2blksz;
}

ulong nvme_blk_write(struct udevice *udev, lbaint_t blknr,
		     lbaint_t blkcnt, const void *buffer)
{
	struct nvme_ns *ns = dev_get_priv(udev);
	struct nvme_dev *dev = ns->dev;
	struct nvme_command c;
	struct blk_desc *desc = dev_get_uclass_platdata(udev);
	int status;
	u64 prp2;
	u64 total_len = blkcnt << desc->log2blksz;
	u64 temp_len = total_len;

	u64 slba = blknr;
	u16 lbas = 1 << (dev->max_transfer_shift - ns->lba_shift);
	u64 total_lbas = blkcnt;

	c.rw.opcode = nvme_cmd_write;
	c.rw.flags = 0;
	c.rw.nsid = cpu_to_le32(ns->ns_id);
	c.rw.control = 0;
	c.rw.dsmgmt = 0;
	c.rw.reftag = 0;
	c.rw.apptag = 0;
	c.rw.appmask = 0;
	c.rw.metadata = 0;

	while (total_lbas) {
		if (total_lbas < lbas) {
			lbas = (u16)total_lbas;
			total_lbas = 0;
		} else {
			total_lbas -= lbas;
		}

		if (nvme_setup_prps
		   (dev, &prp2, lbas << ns->lba_shift, cvmx_ptr_to_phys(buffer)))
			return -EIO;
		c.rw.slba = cpu_to_le64(slba);
		slba += lbas;
		c.rw.length = cpu_to_le16(lbas - 1);
		c.rw.prp1 = cpu_to_le64(cvmx_ptr_to_phys(buffer));
		c.rw.prp2 = cpu_to_le64(cvmx_ptr_to_phys(prp2));
		status = nvme_submit_sync_cmd(dev->queues[1],
				&c, NULL, IO_TIMEOUT);
		if (status)
			break;
		temp_len -= lbas << ns->lba_shift;
		buffer += lbas << ns->lba_shift;
	}

	return (total_len - temp_len) >> desc->log2blksz;
}

static const struct blk_ops nvme_blk_ops = {
	.read	= nvme_blk_read,
	.write	= nvme_blk_write,
};

U_BOOT_DRIVER(nvme_blk) = {
	.name	= "nvme-blk",
	.id	= UCLASS_BLK,
	.probe	= nvme_blk_probe,
	.ops	= &nvme_blk_ops,
	.priv_auto_alloc_size = sizeof(struct nvme_ns),
};

static int nvme_bind(struct udevice *udev)
{
	char name[20];
	sprintf(name, "nvme#%d", nvme_info->ndev_num++);

	return device_set_name(udev, name);
}

static int nvme_probe(struct udevice *udev)
{
	int ret;
	struct nvme_dev *ndev = dev_get_priv(udev);
	u64 cap;
	ndev->pdev = pci_get_controller(udev);
	ndev->instance = trailing_strtol(udev->name);

	INIT_LIST_HEAD(&ndev->namespaces);
	ndev->bar = dm_pci_map_bar(udev, PCI_BASE_ADDRESS_0,
			PCI_REGION_MEM);
	if (readl(&ndev->bar->csts) == -1) {
		ret = -ENODEV;
		printf("Error: %s: Out of memory!\n", udev->name);
		goto free_nvme;
	}

	ndev->queues = malloc(2 * sizeof(struct nvme_queue));
	if (!ndev->queues) {
		ret = -ENOMEM;
		printf("Error: %s: Out of memory!\n", udev->name);
		goto free_nvme;
	}
	memset(ndev->queues, 0, sizeof(2 * sizeof(struct nvme_queue)));

	ndev->prp_pool = malloc(MAX_PRP_POOL);
	if (!ndev->prp_pool) {
		ret = -ENOMEM;
		printf("Error: %s: Out of memory!\n", udev->name);
		goto free_nvme;
	}
	ndev->prp_entry_num = MAX_PRP_POOL >> 3;

	cap = nvme_readq(&ndev->bar->cap);
	ndev->q_depth = min_t(int, NVME_CAP_MQES(cap) + 1, NVME_Q_DEPTH);
	ndev->db_stride = 1 << NVME_CAP_STRIDE(cap);
	ndev->dbs = ((void __iomem *)ndev->bar) + 4096;

	ret = nvme_configure_admin_queue(ndev);
	if (ret)
		goto free_queue;

	ret = nvme_setup_io_queues(ndev);
	if (ret)
		goto free_queue;

	nvme_get_info_from_identify(ndev);
	ndev->blk_dev_start = nvme_info->ns_num;
	list_add(&ndev->node, &nvme_info->dev_list);

	return 0;

free_queue:
	free((void *)ndev->queues);
free_nvme:
	return ret;
}

U_BOOT_DRIVER(nvme) = {
	.name	= "nvme",
	.id	= UCLASS_NVME,
	.bind	= nvme_bind,
	.probe	= nvme_probe,
	.priv_auto_alloc_size = sizeof(struct nvme_dev),
};

struct pci_device_id nvme_supported[] = {
	{ PCI_DEVICE_CLASS(PCI_CLASS_STORAGE_EXPRESS, ~0) },
	{}
};

U_BOOT_PCI_DEVICE(nvme, nvme_supported);
#else

ulong nvme_read(int devnum, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
	__attribute__((weak, alias("__nvme_read")));

ulong __nvme_read(int devnum, lbaint_t blknr, lbaint_t blkcnt, void *buffer)
{
	struct nvme_ns *ns = &nvme_nses[devnum];
	struct nvme_dev *dev = &nvme_devs[devnum];
	struct nvme_command c;
	block_dev_desc_t *desc = &nvme_dev_desc[devnum];
	int status;
	u64 prp2;
	u64 total_len = blkcnt << desc->log2blksz;
	u64 temp_len = total_len;

	u64 slba = blknr;
	u64 lbas = 1ULL << (dev->max_transfer_shift - ns->lba_shift);
	u64 total_lbas = blkcnt;
	debug("%s(%d, 0x%llx, 0x%llx, %p) dev: %p, ns: %p\n",
	      __func__, devnum, blknr, blkcnt, buffer, dev, ns);
	c.rw.opcode = nvme_cmd_read;
	c.rw.flags = 0;
	c.rw.nsid = cpu_to_le32(ns->ns_id);
	c.rw.control = 0;
	c.rw.dsmgmt = 0;
	c.rw.reftag = 0;
	c.rw.apptag = 0;
	c.rw.appmask = 0;
	c.rw.metadata = 0;

	while (total_lbas) {
		if (total_lbas < lbas) {
			lbas = (u16)total_lbas;
			total_lbas = 0;
		} else {
			total_lbas -= lbas;
		}

		if (nvme_setup_prps(dev, &prp2, lbas << ns->lba_shift,
				    cvmx_ptr_to_phys(buffer)))
			return -EIO;

		c.rw.slba = cpu_to_le64(slba);
		slba += lbas;
		c.rw.length = cpu_to_le16(lbas - 1);
		c.rw.prp1 = cpu_to_le64(cvmx_ptr_to_phys(buffer));
		c.rw.prp2 = cpu_to_le64(prp2);
		status = nvme_submit_sync_cmd(dev->queues[1], &c,
					      NULL, IO_TIMEOUT);
		if (status)
			break;
		temp_len -= lbas << ns->lba_shift;
		buffer += lbas << ns->lba_shift;
	}

	return (total_len - temp_len) >> desc->log2blksz;
}

ulong nvme_write(int dev, lbaint_t blknr, lbaint_t blkcnt,
		 const void *buffer)
	__attribute__((weak, alias("__nvme_write")));

ulong __nvme_write(int devnum, lbaint_t blknr, lbaint_t blkcnt,
		      const void *buffer)
{
	struct nvme_ns *ns = &nvme_nses[devnum];
	struct nvme_dev *dev = &nvme_devs[devnum];
	struct nvme_command c;
	block_dev_desc_t *desc = &nvme_dev_desc[devnum];
	int status;
	u64 prp2;
	u64 total_len = blkcnt << desc->log2blksz;
	u64 temp_len = total_len;

	u64 slba = blknr;
	u16 lbas = 1 << (dev->max_transfer_shift - ns->lba_shift);
	u64 total_lbas = blkcnt;

	c.rw.opcode = nvme_cmd_write;
	c.rw.flags = 0;
	c.rw.nsid = cpu_to_le32(ns->ns_id);
	c.rw.control = 0;
	c.rw.dsmgmt = 0;
	c.rw.reftag = 0;
	c.rw.apptag = 0;
	c.rw.appmask = 0;
	c.rw.metadata = 0;

	while (total_lbas) {
		if (total_lbas < lbas) {
			lbas = (u16)total_lbas;
			total_lbas = 0;
		} else {
			total_lbas -= lbas;
		}
		if (nvme_setup_prps(dev, &prp2, lbas << ns->lba_shift,
				    cvmx_ptr_to_phys((void *)buffer)))
			return -EIO;
		c.rw.slba = cpu_to_le64(slba);
		slba += lbas;
		c.rw.length = cpu_to_le16(lbas - 1);
		c.rw.prp1 = cpu_to_le64(cvmx_ptr_to_phys((void *)buffer));
		c.rw.prp2 = cpu_to_le64(prp2);
		status = nvme_submit_sync_cmd(dev->queues[1],
				&c, NULL, IO_TIMEOUT);
		if (status)
			break;
		temp_len -= lbas << ns->lba_shift;
		buffer += lbas << ns->lba_shift;
	}

	return (total_len - temp_len) >> desc->log2blksz;
}

int nvme_blk_probe(int dev) __attribute__((weak, alias("__nvme_blk_probe")));
int __nvme_blk_probe(int dev)
{
	struct nvme_dev *ndev = &nvme_devs[dev];
	block_dev_desc_t *desc = &nvme_dev_desc[dev];
	struct nvme_ns *ns = &nvme_nses[dev];
	u8 flbas;
	u16 vendor;
	struct nvme_id_ns buf, *id = &buf;
	struct nvme_id_ctrl cbuf, *ctrl = &cbuf;

	if (!ndev->name)
		return -ENODEV;

	memset(ns, 0, sizeof(*ns));
	ns->dev = ndev;
	ns->devnum = dev;

	if(nvme_identify(ndev, 0, 1, cvmx_ptr_to_phys(ctrl)))
		return -EIO;

	if(le16_to_cpu(ctrl->oacs) & 0x08)
		ns->ns_id = desc->dev - ndev->blk_dev_start + 1;
	else
		ns->ns_id = 1;

	debug("%s(%d)\n", __func__, dev);
	if (nvme_identify(ndev, ns->ns_id, 0, cvmx_ptr_to_phys(id)))
		return -EIO;

	flbas = id->flbas & NVME_NS_FLBAS_LBA_MASK;
	ns->flbas = flbas;
	ns->lba_shift = id->lbaf[flbas].ds;
	ns->mode_select_num_blocks = le64_to_cpu(id->nsze);
	ns->mode_select_block_len = 1 << ns->lba_shift;
	debug("Adding namespaces\n");
	list_add(&ns->list, &ndev->namespaces);

	desc->type = DEV_TYPE_HARDDISK;
	desc->if_type = IF_TYPE_NVME;
	desc->lba = ns->mode_select_num_blocks;
	desc->log2blksz = ns->lba_shift;
	desc->blksz = 1 << ns->lba_shift;
#ifdef CONFIG_LBA48
	desc->lba48 = 1;
#endif
	pci_read_config_word(ndev->pci_dev, PCI_VENDOR_ID, &vendor);
	sprintf(desc->vendor, "0x%.4x", vendor);
	memcpy(desc->product, ndev->serial, sizeof(ndev->serial));
	memcpy(desc->revision, ndev->firmware_rev, sizeof(ndev->firmware_rev));
	debug("%s: vendor: %s, product: %s, revision: %s\n", __func__,
	      desc->vendor, desc->product, desc->revision);
	init_part(desc);
	return 0;
}

static int nvme_init_one(pci_dev_t pdev)
{
	struct nvme_dev *ndev;
	u64 cap;
	int ret;
	char name[20];

	debug("%s(0x%x)\n", __func__, pdev);
	if (nvme_last_device_idx >= CONFIG_SYS_NVME_MAX_DEVICE) {
		printf("%s: out of nvme devices.  Set CONFIG_SYS_NVME_MAX_DEVICE larger than %d\n",
		       __func__, CONFIG_SYS_NVME_MAX_DEVICE);
		return -ENOMEM;
	}

	ndev = &nvme_devs[nvme_last_device_idx];
	ndev->instance = nvme_last_device_idx;
	snprintf(name, sizeof(name), "ndev%d", nvme_last_device_idx);
	ndev->name = strdup(name);
	if (!ndev->name) {
		printf("%s: Error: nvme%d: Out of memory!\n", __func__,
		       ndev->instance);
	}
	INIT_LIST_HEAD(&ndev->namespaces);
	ndev->bar = pci_map_bar(pdev, PCI_BASE_ADDRESS_0, PCI_REGION_MEM);

	debug("%s(%s): bar mapped to 0x%p\n", __func__, ndev->name,
	      ndev->bar);

	if (readl(&ndev->bar->csts) == -1) {
		ret = -ENODEV;
		printf("%s: Error: %s: Out of memory!\n", __func__, ndev->name);
		goto free_nvme;
	}

	ndev->queues = calloc(sizeof(struct nvme_queue), 2);
	if (!ndev->queues) {
		ret = -ENOMEM;
		printf("%s: Error: %s: Out of memory!\n", __func__, ndev->name);
		goto free_nvme;
	}

	ndev->prp_pool = calloc(MAX_PRP_POOL, 1);
	if (!ndev->prp_pool) {
		ret = -ENOMEM;
		printf("%s: Error: %s: Out of memory!\n", __func__, ndev->name);
		goto free_nvme;
	}
	ndev->prp_entry_num = MAX_PRP_POOL >> 3;

	cap = nvme_readq(&ndev->bar->cap);
	ndev->q_depth = min_t(int, NVME_CAP_MQES(cap) + 1, NVME_Q_DEPTH);
	ndev->db_stride = 1ULL << NVME_CAP_STRIDE(cap);
	ndev->dbs = ((void __iomem *)ndev->bar) + 4096;

	debug("  q_depth: %d\n", ndev->q_depth);
	debug("%s: Configuring admin queue\n", __func__);
	ret = nvme_configure_admin_queue(ndev);
	if (ret)
		goto free_queue;

	debug("%s: Getting info from identify\n", __func__);
	ret = nvme_get_info_from_identify(ndev);
	if (ret)
		goto free_queue;

	debug("%s: Setting up IO queues\n", __func__);
	ret = nvme_setup_io_queues(ndev);
	if (ret)
		goto free_queue;

	nvme_get_info_from_identify(ndev);

	ret = nvme_blk_probe(nvme_last_device_idx);
	debug("%s: nvme_blk_probe(%d) returned %d\n",
	      __func__, nvme_last_device_idx, ret);

	nvme_last_device_idx++;

	return ret;

free_queue:
	free((void *)ndev->queues);

free_nvme:
	if (ndev->name)
		free((void *)(ndev->name));
	ndev->name = NULL;
	if (ndev->queues)
		free(ndev->queues);
	ndev->queues = NULL;
	if (ndev->prp_pool)
		free(ndev->prp_pool);
	ndev->prp_pool = NULL;

	return ret;
}

int init_nvme(int dev) __attribute__((weak, alias("__init_nvme")));

int __init_nvme(int dev)
{
	pci_dev_t pcidev;
	int host_num = 0;
	int rc = 0;
	static bool initted = false;
	int i;

	debug("%s: entry(%d)\n", __func__, dev);

	if (initted) {
		debug("Already initialized\n");
		if (dev < CONFIG_SYS_NVME_MAX_DEVICE) {
			return 0;
		} else {
			debug("dev %d not present\n", dev);
			return -1;
		}
	} else {
		for (i = 0; i < CONFIG_SYS_NVME_MAX_DEVICE; i++) {
			nvme_dev_desc[i].type = DEV_TYPE_UNKNOWN;
			nvme_dev_desc[i].if_type = IF_TYPE_UNKNOWN;
			nvme_dev_desc[i].dev = i;
		}
	}

#ifndef CONFIG_NVME_PLAT
	do {
		pcidev = pci_find_class(1	/* storage */,
					8	/* Express */,
					2	/* NVME    */,
					host_num++);
		if (pcidev == -1)
			break;

		debug("%s: Calling nvme_init_one(0x%x)\n", __func__, pcidev);
		rc = nvme_init_one(pcidev);
		if (rc) {
			debug("nvme_init_one(0x%x) returned %d\n", pcidev, rc);
			return rc;
		}
	} while (pcidev >= 0 && !rc);
#else
	rc = nvme_init_platform();
	if (rc)
		debug("%s: nvme_init_platform() returned %d\n", __func__, rc);
#endif
	if (!rc)
		initted = true;
	else
		printf("%s(%d) returned %d\n", __func__, dev, rc);
	return rc;
}
#endif
