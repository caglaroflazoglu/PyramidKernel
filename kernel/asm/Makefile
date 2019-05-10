OBJECTS = interrupt.asm.o input_output.asm.o loader.asm.o descriptors.asm.o multiboot.asm.o

AS = nasm
ASFLAGS = -f elf

all: $(OBJECTS)
%.asm.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

