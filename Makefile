CFLAGS      = -mcpu=cortex-m3 -mthumb

# Build rules
all: build/

build/
	mkdir -p $@

flash-stlink:
	st-flash write build/main.elf 0x8000000

flash-openocd:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/main.elf verify reset exit"