iso:
	grub-mkrescue iso_root -o PyramidKernel.iso
run:
	qemu-system-i386 -cdrom PyramidKernel.iso
bochs:
	echo "c" > commands
	bochs -q -f bochs_config -rc commands
clean:
	rm -rf obj/ kernel/*.o kernel/asm/*.o iso_root/boot/kernel.elf
	rm -rf kernel/mm/*.o  kernel/drivers/*.o kernel/lib/*.o
	rm -rf PyramidKernel.iso bochslog.txt commands
