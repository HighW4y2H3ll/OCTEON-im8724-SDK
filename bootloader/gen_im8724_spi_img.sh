# Binary im8724_spi_image.bin was built in Octeon SDK 3.1.0 p6 branch and copied here, because
# source of spi-boot must be fixed in current, Octeon SDK 3.1.2 branch:

#cp spi-boot/spi-boot.bin im8724_spi_image.bin
cp im8724_spi_image.bin.templ im8724_spi_image.bin
dd if=u-boot/u-boot-octeon_im8724_spi_stage2.bin of=im8724_spi_image.bin seek=128 conv=notrunc
