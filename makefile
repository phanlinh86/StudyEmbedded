# Purpose: Build and flash the blink_a project
# The target can be separated by the project name and the board used
# _a is used for STM32F4Discovery board
# _b is used for STM32F4Nucleo board
# _c is used for atmega328p board


blink_a: clean
	batch\BuildStm32F411E BlinkLed

blink_b: clean
	batch\BuildStm32L0x0 BlinkLed

clean:
	del /q build
	
flash_a:
	openocd -f board/stm32f4discovery.cfg -c "program build/main.elf verify reset exit"

flash_b:
	openocd -f interface/stlink.cfg -f target/stm32l0.cfg -c "program build/main.elf verify reset exit"