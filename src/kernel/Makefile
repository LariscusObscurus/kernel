INCPATH = include
SRCPATH = src
OBJPATH = obj
ASMPATH = obj/asm


SRC = $(wildcard $(SRCPATH)/*.c)
ASM = $(wildcard $(SRCPATH)/*.S)
OBJ = $(SRC:$(SRCPATH)/%.c=$(OBJPATH)/%.o)
ASMOBJ = $(ASM:$(SRCPATH)/%.S=$(ASMPATH)/%.o)


CC = i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -Wall -g -std=gnu11 -I$(INCPATH)
LDFLAGS = -Tkernel.ld

$(ASMOBJ): $(ASMPATH)/%.o : $(SRCPATH)/%.S
	@$(CC) -c $< -o $@ $(CFLAGS)
	@echo "Compiled "$<" successfully!"

$(OBJ): $(OBJPATH)/%.o : $(SRCPATH)/%.c
	@$(CC) -c $< -o $@ $(CFLAGS) 
	@echo "Compiled "$<" successfully!"


kernel: mkOBJPATH $(OBJ) $(ASMOBJ)
	$(LD) -o $@ $(LDFLAGS) $(OBJ) $(ASMOBJ)

.PHONY: clean
clean:
	rm -r $(OBJPATH)/*
	@echo "Cleanup complete!"

mkOBJPATH:
	[[ -d $(OBJPATH) ]] || mkdir $(OBJPATH)
	[[ -d $(ASMPATH) ]] || mkdir $(ASMPATH)