OBJECTS = interrupt.o input_output.o loader.o descriptors.o multiboot.o
ASM=nasm
ASMFLAGS=-felf32

all: $(OBJECTS)

%.o: %.s
	$(ASM) $(ASMFLAGS) $< -o $@
