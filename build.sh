mkdir -p obj/ 
make -C kernel/asm
cp kernel/asm/*.o obj/
make -C kernel/
cp kernel/*.o obj/
make -C kernel/mm/
cp kernel/mm/*.o obj/
make -C kernel/drivers/
cp kernel/drivers/*.o obj/
make -C kernel/lib/
cp kernel/lib/*.o obj/
ld -melf_i386 -nostdlib -O2 -T link.ld -o iso_root/boot/kernel.elf obj/*.o