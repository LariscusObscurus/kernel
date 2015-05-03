all: kernel

kernel:
	$(MAKE) -C src/kernel

clean:
	$(MAKE) clean

.PHONY: all kernel clean