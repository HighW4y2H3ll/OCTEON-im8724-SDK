include ../.config

DIR = lockstat
SRC_DIR = $(SOURCE_DIR)/$(DIR)

.PHONY: all
all:
	mkdir -p $(DIR)
	# If the libraries are present under ${SRC_DIR}, the embedded rootfs
	# skips building this library because of VPATH.
	$(MAKE) -C $(SRC_DIR) clean
	$(MAKE) -C $(DIR) -f $(SRC_DIR)/Makefile VPATH=$(SRC_DIR) \
	  CFLAGS="$(CFLAGS)"
	$(MAKE) -C $(DIR) -f $(SRC_DIR)/Makefile VPATH=$(SRC_DIR) \
	  CFLAGS="$(CFLAGS)" LIBDIR=$(ROOT)$(LIBDIR) BINDIR=$(ROOT)/usr/bin \
	  install
