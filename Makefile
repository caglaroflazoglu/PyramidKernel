iso:
	cp dist/kernel iso_root/boot/
	grub-mkrescue iso_root -o PyramidKernel.iso
run:
	qemu-system-i386 -cdrom PyramidKernel.iso
bochs:
	echo "c" > commands
	bochs -q -f bochs_config -rc commands
clean:
	rm -rf obj/ dist/ kernel/*.o kernel/asm/*.o iso_root/boot/kernel PyramidKernel.iso bochslog.txt commands
