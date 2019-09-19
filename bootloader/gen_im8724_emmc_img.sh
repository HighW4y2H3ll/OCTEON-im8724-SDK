#!/bin/bash

# stage 1 can be updated from emmc-boot/stage1/emm-boot.bin

pushd images

cp -v emm-boot.bin im8724_emmc_image.bin || exit 1

dd if=/dev/zero of=im8724_emmc_part1.bin bs=1M count=16 || exit 1
mkdosfs -F 16 -n "IM 8724" im8724_emmc_part1.bin || exit 1
sudo rm -rf /tmp/temp; mkdir /tmp/temp || exit 1
sudo mount -o loop im8724_emmc_part1.bin /tmp/temp || exit 1

sudo cp -v u-boot-octeon_im8724_emmc_stage2.bin /tmp/temp/octboot2.bin || exit 1
sudo cp -v u-boot-octeon_im8724.bin /tmp/temp/u-boot-octeon_im8724.bin || exit 1
sudo cp -v u-boot-octeon_im8724.bin /tmp/temp/u-boot-octeon_im8724.bak || exit 1

sudo umount /tmp/temp || exit 1

dd if=im8724_emmc_part1.bin of=im8724_emmc_image.bin seek=2048 conv=notrunc || exit 1
gzip -f im8724_emmc_image.bin || exit 1
rm -f im8724_emmc_part1.bin || exit 1
sudo rm -rf /tmp/temp

popd
