mkdir -p obj/ dist/
make -C kernel/asm
make -C kernel/
cp kernel/asm/*.o obj/
cp kernel/*.o obj/
ld  -melf_i386 -nostdlib -O2 -T link.ld -o dist/kernel obj/*.o
