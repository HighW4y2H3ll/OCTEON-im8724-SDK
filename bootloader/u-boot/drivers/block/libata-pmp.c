/*
 * libata-pmp.c - libata port multiplier support
 *
 * Copyright (c) 2007  SUSE Linux Products GmbH
 * Copyright (c) 2007  Tejun Heo <teheo@suse.de>
 * Copyright (c) 2013  Cavium, Inc. <support@cavium.com>
 *
 * This file is released under the GPLv2.
 */
#define DEBUG
#include <common.h>
#include <malloc.h>
#include <linux/dma-direction.h>
#include <libata.h>
#include <ata.h>
#ifdef CONFIG_AHCI
#include <ahci.h>
#endif

extern struct ata_port_operations sata_port_ops;

const struct ata_port_operations sata_pmp_port_ops = {
	.inherits		= &sata_port_ops,
	.pmp_prereset		= ata_std_prereset,
	.pmp_hardreset		= sata_std_hardreset,
	.pmp_postreset		= ata_std_postreset,
};

/**
 *	sata_pmp_read - read PMP register
 *	@link: link to read PMP register for
 *	@reg: register to read
 *	@r_val: resulting value
 *
 *	Read PMP register.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, AC_ERR_* mask on failure.
 */
static unsigned int sata_pmp_read(struct ata_link *link, int reg, u32 *r_val)
{
	struct ata_port *ap = link->ap;
	struct ata_taskfile tf;
	struct ata_device *pmp_dev = &link->device[0];
	unsigned int err_mask;

	debug("%s(%d, 0x%x, 0x%p)\n", __func__, ap->port_no, reg, r_val);

	ata_tf_init(pmp_dev, &tf);
	tf.command = ATA_CMD_PMP_READ;
	tf.protocol = ATA_PROT_NODATA;
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE | ATA_TFLAG_LBA48;
	tf.feature = reg;
	tf.device = link->pmp;
	debug("  map: 0x%x\n", link->pmp);

	err_mask = ata_exec_internal(pmp_dev, &tf, NULL, DMA_NONE, NULL, 0,
				     SATA_PMP_RW_TIMEOUT);
	if (err_mask) {
		debug("%s: err mask: 0x%x\n", __func__, err_mask);
		return err_mask;
	}

	*r_val = tf.nsect | (tf.lbal << 8) | (tf.lbam << 16) | (tf.lbah << 24);
	debug("%s: rval: 0x%x\n", __func__, *r_val);
	return 0;
}

/**
 *	sata_pmp_write - write PMP register
 *	@link: link to write PMP register for
 *	@reg: register to write
 *	@r_val: value to write
 *
 *	Write PMP register.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, AC_ERR_* mask on failure.
 */
static unsigned int sata_pmp_write(struct ata_link *link, int reg, u32 val)
{
	struct ata_port *ap = link->ap;
	struct ata_device *pmp_dev = ap->link.device;
	struct ata_taskfile tf;

	debug("%s(%d, 0x%x, 0x%x)\n", __func__, ap->port_no, reg, val);
	ata_tf_init(pmp_dev, &tf);
	tf.command = ATA_CMD_PMP_WRITE;
	tf.protocol = ATA_PROT_NODATA;
	tf.flags |= ATA_TFLAG_ISADDR | ATA_TFLAG_DEVICE | ATA_TFLAG_LBA48;
	tf.feature = reg;
	tf.device = link->pmp;
	tf.nsect = val & 0xff;
	tf.lbal = (val >> 8) & 0xff;
	tf.lbam = (val >> 16) & 0xff;
	tf.lbah = (val >> 24) & 0xff;

	return ata_exec_internal(pmp_dev, &tf, NULL, DMA_NONE, NULL, 0,
				 SATA_PMP_RW_TIMEOUT);
}

/**
 *	sata_pmp_scr_read - read PSCR
 *	@link: ATA link to read PSCR for
 *	@reg: PSCR to read
 *	@r_val: resulting value
 *
 *	Read PSCR @reg into @r_val for @link, to be called from
 *	ata_scr_read().
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int sata_pmp_scr_read(struct ata_link *link, int reg, u32 *r_val)
{
	unsigned int err_mask;

	debug("%s(%d, %d, 0x%x\n", __func__, link->ap->port_no, reg, r_val);
	if (reg > SATA_PMP_PSCR_CONTROL)
		return -EINVAL;

	err_mask = sata_pmp_read(link, reg, r_val);
	debug("%s(%d, 0x%x) = 0x%x\n", __func__, link->ap->port_no, reg, *r_val);
	if (err_mask) {
		printf("failed to read SCR %d (Emask=0x%x)\n", reg, err_mask);
		return -EIO;
	}
	debug("%s: r_val: 0x%x\n", __func__, *r_val);
	return 0;
}

/**
 *	sata_pmp_scr_write - write PSCR
 *	@link: ATA link to write PSCR for
 *	@reg: PSCR to write
 *	@val: value to be written
 *
 *	Write @val to PSCR @reg for @link, to be called from
 *	ata_scr_write() and ata_scr_write_flush().
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int sata_pmp_scr_write(struct ata_link *link, int reg, u32 val)
{
	unsigned int err_mask;

	if (reg > SATA_PMP_PSCR_CONTROL)
		return -EINVAL;

	debug("%s(%d, 0x%x, 0x%x)\n", __func__, link->ap->port_no, reg, val);
	err_mask = sata_pmp_write(link, reg, val);
	if (err_mask) {
		printf("failed to write SCR %d (Emask=0x%x)\n",
		       reg, err_mask);
		return -EIO;
	}
	return 0;
}
/**
 *	sata_pmp_read_gscr - read GSCR block of SATA PMP
 *	@link: PMP device link
 *	@gscr: buffer to read GSCR block into
 *
 *	Read selected PMP GSCRs from the PMP at @dev.  This will serve
 *	as configuration and identification info for the PMP.
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
static int sata_pmp_read_gscr(struct ata_link *link, u32 *gscr)
{
	static const int gscr_to_read[] = { 0, 1, 2, 32, 33, 64, 96 };
	int i;

	debug("%s(0x%p (%d), 0x%p)\n", __func__, link, link->ap->port_no, gscr);
	for (i = 0; i < ARRAY_SIZE(gscr_to_read); i++) {
		int reg = gscr_to_read[i];
		unsigned int err_mask;

		debug(" reg[%d]: 0x%x\n", i, reg);
		err_mask = sata_pmp_read(link, reg, &gscr[reg]);
		if (err_mask) {
			printf("failed to read PMP GSCR[%d] (Emask=0x%x)\n",
			       reg, err_mask);
			return -EIO;
		}
	}
	debug("%s EXIT\n", __func__);
	return 0;
}

static const char *sata_pmp_spec_rev_str(const u32 *gscr)
{
	u32 rev = gscr[SATA_PMP_GSCR_REV];

	if (rev & (1 << 3))
		return "1.2";
	if (rev & (1 << 2))
		return "1.1";
	if (rev & (1 << 1))
		return "1.0";
	return "<unknown>";
}

#define PMP_GSCR_SII_POL 129

static int sata_pmp_configure(struct ata_device *dev, int print_info)
{
	struct ata_port *ap = dev->link->ap;
	u32 *gscr = dev->gscr;
	u16 vendor = sata_pmp_gscr_vendor(gscr);
	u16 devid = sata_pmp_gscr_devid(gscr);
	unsigned int err_mask = 0;
	const char *reason;
	int nr_ports, rc;

	debug("%s(%d, %d)\n", __func__, ap->port_no, print_info);
	nr_ports = sata_pmp_gscr_ports(gscr);
	debug("  %d total ports\n", nr_ports);
	debug("  gscr: %p\n", gscr);

	if (nr_ports <= 0 || nr_ports > SATA_PMP_MAX_PORTS) {
		debug("  nr_ports %d is invalid\n", nr_ports);
		rc = -EINVAL;
		reason = "invalid nr_ports";
		goto fail;
	}

	if ((ap->flags & ATA_FLAG_AN) &&
	    (gscr[SATA_PMP_GSCR_FEAT] & SATA_PMP_FEAT_NOTIFY))
		dev->flags |= ATA_DFLAG_AN;

	/* monitor SERR_PHYRDY_CHG on fan-out ports */
	err_mask = sata_pmp_write(dev->link, SATA_PMP_GSCR_ERROR_EN,
				  SERR_PHYRDY_CHG);
	if (err_mask) {
		rc = -EIO;
		reason = "failed to write GSCR_ERROR_EN";
		goto fail;
	}

	/* Disable sending Early R_OK.
	 * With "cached read" HDD testing and multiple ports busy on a SATA
	 * host controller, 3726 PMP will very rarely drop a deferred
	 * R_OK that was intended for the host. Symptom will be all
	 * 5 drives under test will timeout, get reset, and recover.
	 */
	if (vendor == 0x1095 && devid == 0x3726) {
		u32 reg;
		debug(" Handling device Silicon Image 1095:3726\n");
		err_mask = sata_pmp_read(&ap->link, PMP_GSCR_SII_POL, &reg);
		if (err_mask) {
			rc = -EIO;
			reason = "failed to read Sil3726 Private Register";
			goto fail;
		}
		reg &= ~0x1;
		err_mask = sata_pmp_write(&ap->link, PMP_GSCR_SII_POL, reg);
		if (err_mask) {
			rc = -EIO;
			reason = "failed to write Sil3726 Private Register";
			goto fail;
		}
	}

	if (print_info) {
		printf("Port Multiplier %s, "
		       "0x%04x:0x%04x r%d, %d ports, feat 0x%x/0x%x\n",
		       sata_pmp_spec_rev_str(gscr), vendor, devid,
		       sata_pmp_gscr_rev(gscr),
		       nr_ports, gscr[SATA_PMP_GSCR_FEAT_EN],
		       gscr[SATA_PMP_GSCR_FEAT]);

		if (!(dev->flags & ATA_DFLAG_AN))
			printf("Asynchronous notification not supported, "
			       "hotplug won't work on fan-out ports. Use warm-plug instead.\n");
	}

	return 0;

 fail:
	printf("failed to configure Port Multiplier (%s, Emask=0x%x)\n",
	       reason, err_mask);
	return rc;
}

static int sata_pmp_init_links(struct ata_port *ap, int nr_ports)
{
	struct ata_link *pmp_link = ap->pmp_link;
	int i, err = 0;

	debug("%s(%p(%d), %d)\n", __func__, ap, ap->port_no, nr_ports);
	debug("%s: ap: %p, pmp_link: %p\n", __func__, ap, pmp_link);
	if (!pmp_link) {
		debug("  allocating link info\n");
		pmp_link = calloc(sizeof(pmp_link[0]), SATA_PMP_MAX_PORTS);
		if (!pmp_link)
			return -ENOMEM;

		debug("%s: Initializing PMP links\n", __func__);
		for (i = 0; i < SATA_PMP_MAX_PORTS; i++)
			ata_link_init(ap, &pmp_link[i], i);

		ap->pmp_link = pmp_link;
		debug("  ap(%d) pmp_link: 0x%p, link: %p\n", ap->port_no,
		      pmp_link, ap->link);
#warning TODO?
#if 0
		for (i = 0; i < SATA_PMP_MAX_PORTS; i++) {
			err = ata_tlink_add(&pmp_link[i]);
			if (err) {
				goto err_tlink;
			}
		}
#endif
	}

	for (i = 0; i < nr_ports; i++) {
		struct ata_link *link = &pmp_link[i];

		link->flags = 0;
		/* Should now reset all links */
		if (ap->ops->add_pmp_attached_dev) {
			debug("%s: Attaching device for port %d, pmp link %d\n",
			      __func__, ap->port_no, i);
			debug("%s: ap: %p, link: %p, link dev: %p, link dev link: %p\n",
			      __func__, ap, link, link->device,
			      link->device->link);
			ap->ops->add_pmp_attached_dev(link);
		}
#if 0
		ata_dev_init(link->device);
#endif
	}

	return 0;

#if 0
err_tlink:
	while (--i >= 0)
		ata_tlink_delete(&pmp_link[i]);
	free(pmp_link);
	ap->pmp_link = NULL;
	return err;
#endif
}

/**
 *	sata_pmp_attach - attach a SATA PMP device
 *	@dev: SATA PMP device to attach
 *
 *	Configure and attach SATA PMP device @dev.  This function is
 *	also responsible for allocating and initializing PMP links.
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
int sata_pmp_attach(struct ata_device *dev)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	unsigned long flags;
	struct ata_link *tlink;
	int rc;

	debug("%s(%p(%d)) dev: %p, link: %p, ap: %p\n", __func__, dev,
	      ap->port_no, dev, link, ap);
	/* is it hanging off the right place? */
	if (!sata_pmp_supported(ap)) {
		printf("AHCI host does not support Port Multiplier\n");
		return -EINVAL;
	}

	if (!ata_is_host_link(link)) {
		printf("Port Multipliers cannot be nested\n");
		return -EINVAL;
	}

	if (link->pmp) {
		printf("Port Multiplier must be the first device\n");
		return -EINVAL;
	}

	link->pmp = SATA_PMP_CTRL_PORT;
	debug("%s: pmp: 0x%x\n", __func__, link->pmp);
	/* read GSCR block */
	rc = sata_pmp_read_gscr(link, dev->gscr);
	if (rc)
		goto fail;

#ifdef DEBUG
	debug("%s: GSCR:\n", __func__);
	print_buffer(0, dev->gscr, 4, SATA_PMP_GSCR_DWORDS, 0);
#endif
	/* config PMP */
	debug("%s: Configuring PMP\n", __func__);
	rc = sata_pmp_configure(dev, 1);
	if (rc) {
		debug("%s: pmp configure failed, rc: %d\n", __func__, rc);
		goto fail;
	}

	debug("%s: pmp: 0x%x\n", __func__, link->pmp);
	debug("%s: initializing links\n", __func__);
	rc = sata_pmp_init_links(ap, sata_pmp_gscr_ports(dev->gscr));
	if (rc) {
		printf("failed to initialize PMP links\n");
		goto fail;
	}

	/* attach it */
	debug("%s: pmp: 0x%x\n", __func__, link->pmp);
	ap->nr_pmp_links = sata_pmp_gscr_ports(dev->gscr);
	debug("%s: nr_pmp_links: %d, ap: %p\n", __func__, ap->nr_pmp_links, ap);
	if (ap->ops->pmp_attach) {
		debug("%s: Calling pmp_attach %p\n", __func__,
		      ap->ops->pmp_attach);
		ap->ops->pmp_attach(ap);
	} else {
		debug("No pmp attach op\n");
	}

	debug("%s: pmp: 0x%x\n", __func__, link->pmp);
	debug("%s: Initializing speed\n", __func__);
	ata_for_each_link(tlink, ap, EDGE) {
		debug("%s: Initializing SATA SPD\n", __func__);
		sata_link_init_spd(tlink);
		dev->pio_mode = XFER_PIO_0;
		dev->dma_mode = 0xff;
	}

	debug("%s: pmp: 0x%x\n", __func__, link->pmp);
	debug("%s: EXIT\n", __func__);
	return 0;

 fail:
	printf("%s: failed, rc: %d\n", __func__, rc);
	link->pmp = 0;
	return rc;
}

static void sata_pmp_detach(struct ata_device *dev)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	struct ata_link *tlink;

	if (!ata_is_host_link(link) || dev->devno ||
	    link->pmp != SATA_PMP_CTRL_PORT) {
		printf("Warning: pmp link on port %d is not the control port\n",
		       ap->port_no);
	}

	if (ap->ops->pmp_detach)
		ap->ops->pmp_detach(ap);

	ata_for_each_link(tlink, ap, EDGE)
		ata_eh_detach_dev(tlink->device);

	ap->nr_pmp_links = 0;
	link->pmp = 0;
}

/**
 *	sata_pmp_same_pmp - does new GSCR match the configured PMP?
 *	@dev: PMP device to compare against
 *	@new_gscr: GSCR block of the new device
 *
 *	Compare @new_gscr against @dev and determine whether @dev is
 *	the PMP described by @new_gscr.
 *
 *	RETURNS:
 *	1 if @dev matches @new_gscr, 0 otherwise
 */
static int sata_pmp_same_pmp(struct ata_device *dev, const u32 *new_gscr)
{
	const u32 *old_gscr = dev->gscr;
	u16 old_vendor, new_vendor, old_devid, new_devid;
	int old_nr_ports, new_nr_ports;

	old_vendor = sata_pmp_gscr_vendor(old_gscr);
	new_vendor = sata_pmp_gscr_vendor(new_gscr);
	old_devid = sata_pmp_gscr_devid(old_gscr);
	new_devid = sata_pmp_gscr_devid(new_gscr);
	old_nr_ports = sata_pmp_gscr_ports(old_gscr);
	new_nr_ports = sata_pmp_gscr_ports(new_gscr);

	if (old_vendor != new_vendor) {
		ata_dev_info(dev,
			     "Port Multiplier vendor mismatch '0x%x' != '0x%x'\n",
			     old_vendor, new_vendor);
		return 0;
	}

	if (old_devid != new_devid) {
		ata_dev_info(dev,
			     "Port Multiplier device ID mismatch '0x%x' != '0x%x'\n",
			     old_devid, new_devid);
		return 0;
	}

	if (old_nr_ports != new_nr_ports) {
		ata_dev_info(dev,
			     "Port Multiplier nr_ports mismatch '0x%x' != '0x%x'\n",
			     old_nr_ports, new_nr_ports);
		return 0;
	}

	return 1;
}

/**
 *	sata_pmp_revalidate - revalidate SATA PMP
 *	@dev: PMP device to revalidate
 *	@new_class: new class code
 *
 *	Re-read GSCR block and make sure @dev is still attached to the
 *	port and properly configured.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno otherwise.
 */
static int sata_pmp_revalidate(struct ata_device *dev, unsigned int new_class)
{
	struct ata_link *link = dev->link;
	struct ata_port *ap = link->ap;
	u32 *gscr = (void *)ap->sector_buf;
	int rc;

	DPRINTK("ENTER\n");

	ata_eh_about_to_do(link, NULL, ATA_EH_REVALIDATE);

	if (!ata_dev_enabled(dev)) {
		rc = -ENODEV;
		goto fail;
	}

	/* wrong class? */
	if (ata_class_enabled(new_class) && new_class != ATA_DEV_PMP) {
		rc = -ENODEV;
		goto fail;
	}

	/* read GSCR */
	rc = sata_pmp_read_gscr(dev, gscr);
	if (rc)
		goto fail;

	/* is the pmp still there? */
	if (!sata_pmp_same_pmp(dev, gscr)) {
		rc = -ENODEV;
		goto fail;
	}

	memcpy(dev->gscr, gscr, sizeof(gscr[0]) * SATA_PMP_GSCR_DWORDS);

	rc = sata_pmp_configure(dev, 0);
	if (rc)
		goto fail;

	ata_eh_done(link, NULL, ATA_EH_REVALIDATE);

	DPRINTK("EXIT, rc=0\n");
	return 0;

 fail:
	ata_dev_err(dev, "PMP revalidation failed (errno=%d)\n", rc);
	DPRINTK("EXIT, rc=%d\n", rc);
	return rc;
}

/**
 *	sata_pmp_eh_recover_pmp - recover PMP
 *	@ap: ATA port PMP is attached to
 *	@prereset: prereset method (can be NULL)
 *	@softreset: softreset method
 *	@hardreset: hardreset method
 *	@postreset: postreset method (can be NULL)
 *
 *	Recover PMP attached to @ap.  Recovery procedure is somewhat
 *	similar to that of ata_eh_recover() except that reset should
 *	always be performed in hard->soft sequence and recovery
 *	failure results in PMP detachment.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
static int sata_pmp_eh_recover_pmp(struct ata_port *ap,
		ata_prereset_fn_t prereset, ata_reset_fn_t softreset,
		ata_reset_fn_t hardreset, ata_postreset_fn_t postreset)
{
	struct ata_link *link = &ap->link;
	struct ata_eh_context *ehc = &link->eh_context;
	struct ata_device *dev = link->device;
	int tries = ATA_EH_PMP_TRIES;
	int detach = 0, rc = 0;
	int reval_failed = 0;

	DPRINTK("ENTER\n");

	if (dev->flags & ATA_DFLAG_DETACH) {
		detach = 1;
		goto fail;
	}

 retry:
	ehc->classes[0] = ATA_DEV_UNKNOWN;

	if (ehc->i.action & ATA_EH_RESET) {
		struct ata_link *tlink;

		/* reset */
		rc = ata_eh_reset(link, 0, prereset, softreset, hardreset,
				  postreset);
		if (rc) {
			ata_link_err(link, "failed to reset PMP, giving up\n");
			goto fail;
		}

		/* PMP is reset, SErrors cannot be trusted, scan all */
		ata_for_each_link(tlink, ap, EDGE) {
			struct ata_eh_context *ehc = &tlink->eh_context;

			ehc->i.probe_mask |= ATA_ALL_DEVICES;
			ehc->i.action |= ATA_EH_RESET;
		}
	}

	/* If revalidation is requested, revalidate and reconfigure;
	 * otherwise, do quick revalidation.
	 */
	if (ehc->i.action & ATA_EH_REVALIDATE)
		rc = sata_pmp_revalidate(dev, ehc->classes[0]);
	else
		rc = sata_pmp_revalidate_quick(dev);

	if (rc) {
		tries--;

		if (rc == -ENODEV) {
			ehc->i.probe_mask |= ATA_ALL_DEVICES;
			detach = 1;
			/* give it just two more chances */
			tries = min(tries, 2);
		}

		if (tries) {
			/* consecutive revalidation failures? speed down */
			if (reval_failed)
				sata_down_spd_limit(link, 0);
			else
				reval_failed = 1;

			ehc->i.action |= ATA_EH_RESET;
			goto retry;
		} else {
			ata_dev_err(dev,
				    "failed to recover PMP after %d tries, giving up\n",
				    ATA_EH_PMP_TRIES);
			goto fail;
		}
	}

	/* okay, PMP resurrected */
	ehc->i.flags = 0;

	DPRINTK("EXIT, rc=0\n");
	return 0;

 fail:
	sata_pmp_detach(dev);
	if (detach)
		ata_eh_detach_dev(dev);
	else
		ata_dev_disable(dev);

	DPRINTK("EXIT, rc=%d\n", rc);
	return rc;
}

static int sata_pmp_eh_handle_disabled_links(struct ata_port *ap)
{
	struct ata_link *link;
	unsigned long flags;
	int rc;

	ata_for_each_link(link, ap, EDGE) {
		if (!(link->flags & ATA_LFLAG_DISABLED))
			continue;

		/* Some PMPs require hardreset sequence to get
		 * SError.N working.
		 */
		sata_link_hardreset(link, sata_deb_timing_normal,
				    ATA_TMOUT_INTERNAL_QUICK,
				    NULL, NULL);

		/* unconditionally clear SError.N */
		rc = sata_scr_write(link, SCR_ERROR, SERR_PHYRDY_CHG);
		if (rc) {
			ata_link_err(link,
				     "failed to clear SError.N (errno=%d)\n",
				     rc);
			return rc;
		}
	}

	return 0;
}

static int sata_pmp_handle_link_fail(struct ata_link *link, int *link_tries)
{
	struct ata_port *ap = link->ap;
	unsigned long flags;

	if (link_tries[link->pmp] && --link_tries[link->pmp])
		return 1;

	/* disable this link */
	if (!(link->flags & ATA_LFLAG_DISABLED)) {
		ata_link_warn(link,
			"failed to recover link after %d tries, disabling\n",
			ATA_EH_PMP_LINK_TRIES);

		link->flags |= ATA_LFLAG_DISABLED;
	}

	ata_dev_disable(link->device);
	link->eh_context.i.action = 0;

	return 0;
}

/**
 *	sata_pmp_eh_recover - recover PMP-enabled port
 *	@ap: ATA port to recover
 *
 *	Drive EH recovery operation for PMP enabled port @ap.  This
 *	function recovers host and PMP ports with proper retrials and
 *	fallbacks.  Actual recovery operations are performed using
 *	ata_eh_recover() and sata_pmp_eh_recover_pmp().
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	0 on success, -errno on failure.
 */
static int sata_pmp_eh_recover(struct ata_port *ap)
{
	struct ata_port_operations *ops = ap->ops;
	int pmp_tries, link_tries[SATA_PMP_MAX_PORTS];
	struct ata_link *pmp_link = &ap->link;
	struct ata_device *pmp_dev = pmp_link->device;
	struct ata_eh_context *pmp_ehc = &pmp_link->eh_context;
	u32 *gscr = pmp_dev->gscr;
	struct ata_link *link;
	struct ata_device *dev;
	unsigned int err_mask;
	u32 gscr_error, sntf;
	int cnt, rc;

	pmp_tries = ATA_EH_PMP_TRIES;
	ata_for_each_link(link, ap, EDGE)
		link_tries[link->pmp] = ATA_EH_PMP_LINK_TRIES;

 retry:
	/* PMP attached? */
	if (!sata_pmp_attached(ap)) {
		rc = ata_eh_recover(ap, ops->prereset, ops->softreset,
				    ops->hardreset, ops->postreset, NULL);
		if (rc) {
			ata_for_each_dev(dev, &ap->link, ALL)
				ata_dev_disable(dev);
			return rc;
		}

		if (pmp_dev->class != ATA_DEV_PMP)
			return 0;

		/* new PMP online */
		ata_for_each_link(link, ap, EDGE)
			link_tries[link->pmp] = ATA_EH_PMP_LINK_TRIES;

		/* fall through */
	}

	/* recover pmp */
	rc = sata_pmp_eh_recover_pmp(ap, ops->prereset, ops->softreset,
				     ops->hardreset, ops->postreset);
	if (rc)
		goto pmp_fail;

	/* PHY event notification can disturb reset and other recovery
	 * operations.  Turn it off.
	 */
	if (gscr[SATA_PMP_GSCR_FEAT_EN] & SATA_PMP_FEAT_NOTIFY) {
		gscr[SATA_PMP_GSCR_FEAT_EN] &= ~SATA_PMP_FEAT_NOTIFY;

		err_mask = sata_pmp_write(pmp_link, SATA_PMP_GSCR_FEAT_EN,
					  gscr[SATA_PMP_GSCR_FEAT_EN]);
		if (err_mask) {
			ata_link_warn(pmp_link,
				"failed to disable NOTIFY (err_mask=0x%x)\n",
				err_mask);
			goto pmp_fail;
		}
	}

	/* handle disabled links */
	rc = sata_pmp_eh_handle_disabled_links(ap);
	if (rc)
		goto pmp_fail;

	/* recover links */
	rc = ata_eh_recover(ap, ops->pmp_prereset, ops->pmp_softreset,
			    ops->pmp_hardreset, ops->pmp_postreset, &link);
	if (rc)
		goto link_fail;

	/* clear SNotification */
	rc = sata_scr_read(&ap->link, SCR_NOTIFICATION, &sntf);
	if (rc == 0)
		sata_scr_write(&ap->link, SCR_NOTIFICATION, sntf);

	/*
	 * Connection status might have changed while resetting other
	 * links, enable notification and check SATA_PMP_GSCR_ERROR
	 * before returning.
	 */

	/* enable notification */
	if (pmp_dev->flags & ATA_DFLAG_AN) {
		gscr[SATA_PMP_GSCR_FEAT_EN] |= SATA_PMP_FEAT_NOTIFY;

		err_mask = sata_pmp_write(pmp_link, SATA_PMP_GSCR_FEAT_EN,
					  gscr[SATA_PMP_GSCR_FEAT_EN]);
		if (err_mask) {
			ata_dev_err(pmp_dev,
				    "failed to write PMP_FEAT_EN (Emask=0x%x)\n",
				    err_mask);
			rc = -EIO;
			goto pmp_fail;
		}
	}

	/* check GSCR_ERROR */
	err_mask = sata_pmp_read(pmp_link, SATA_PMP_GSCR_ERROR, &gscr_error);
	if (err_mask) {
		ata_dev_err(pmp_dev,
			    "failed to read PMP_GSCR_ERROR (Emask=0x%x)\n",
			    err_mask);
		rc = -EIO;
		goto pmp_fail;
	}

	return 0;

 link_fail:
	if (sata_pmp_handle_link_fail(link, link_tries)) {
		pmp_ehc->i.action |= ATA_EH_RESET;
		goto retry;
	}

	/* fall through */
 pmp_fail:
	/* Control always ends up here after detaching PMP.  Shut up
	 * and return if we're unloading.
	 */
	if (ap->pflags & ATA_PFLAG_UNLOADING)
		return rc;

	if (!sata_pmp_attached(ap))
		goto retry;

	if (--pmp_tries) {
		pmp_ehc->i.action |= ATA_EH_RESET;
		goto retry;
	}

	ata_port_err(ap, "failed to recover PMP after %d tries, giving up\n",
		     ATA_EH_PMP_TRIES);
	sata_pmp_detach(pmp_dev);
	ata_dev_disable(pmp_dev);

	return rc;
}

/**
 *	sata_pmp_error_handler - do standard error handling for PMP-enabled host
 *	@ap: host port to handle error for
 *
 *	Perform standard error handling sequence for PMP-enabled host
 *	@ap.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 */
void sata_pmp_error_handler(struct ata_port *ap)
{
	sata_pmp_eh_recover(ap);
}
