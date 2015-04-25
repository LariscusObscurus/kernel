all: kernel

kernel:
	$(MAKE) -C src/kernel
.PHONY: all