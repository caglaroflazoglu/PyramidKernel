OBJECTS = loader.o descriptors.o input_output.o multiboot.o
ASM=nasm
ASMFLAGS=-felf32

all: $(OBJECTS)

%.o: %.s
	$(ASM) $(ASMFLAGS) $< -o $@
