# Purpose: Build and flash the blink_a project
# The target can be separated by the project name and the board used
# _a is used for STM32F4Discovery board
# _b is used for STM32F4Nucleo board
# _c is used for atmega328p board

all: blink

blink: blink_a blink_b blink_c

options: clean
ifeq ($(rev),)
rev = 0000
endif

blink_a: options
	batch\BuildStm32F411E BlinkLed $(rev)
	
blink_b: options
	batch\BuildStm32L0x0 BlinkLed $(rev)
	
blink_c: options
	batch\BuildAtmega328P BlinkLed $(rev)

clean:
	del /q build
	
flash_a:
	openocd -f board/stm32f4discovery.cfg -c "program build/main.elf verify reset exit"

flash_b:
	openocd -f interface/stlink.cfg -f target/stm32l0.cfg -c "program build/main.elf verify reset exit"
	
flash_c:
	avrdude -p atmega328p -c arduino -P "COM7" -b 115200 -U flash:w:build\main.hex
