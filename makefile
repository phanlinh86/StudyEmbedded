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
ifeq ($(rel),)
rel = 0
endif

blink_a: options
	batch\BuildStm32F411E BlinkLed $(rev)
	echo Build is successful. Release file according to release option
	@if $(rel)==0 ((robocopy build build\$@ /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==1 ((robocopy build release\$@\$(rev) /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==2 ((robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==3 (
	@(robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0
	@(robocopy test release\$@\$(rev)\test /NFL /NDL /njh /njs /ndl /nc /s) ^& IF %ERRORLEVEL% LEQ 1 exit 0	
	@7z a .\release\$@_$(rev).zip .\release\$@\$(rev)\* > nul)	
	
blink_b: options
	batch\BuildStm32L0x0 BlinkLed $(rev)
	echo Build is successful. Release file according to release option
	@if $(rel)==0 ((robocopy build build\$@ /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==1 ((robocopy build release\$@\$(rev) /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==2 ((robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==3 (
	@(robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0
	@(robocopy test release\$@\$(rev)\test /NFL /NDL /njh /njs /ndl /nc /s) ^& IF %ERRORLEVEL% LEQ 1 exit 0	
	@7z a .\release\$@_$(rev).zip .\release\$@\$(rev)\* > nul)
	
blink_c: options
	batch\BuildAtmega328P BlinkLed $(rev)
	echo Build is successful. Release file according to release option
	@if $(rel)==0 ((robocopy build build\$@ /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==1 ((robocopy build release\$@\$(rev) /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==2 ((robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0)
	@if $(rel)==3 (
	@(robocopy build release\$@\$(rev) *.elf *.hex *.sym /NFL /NDL /njh /njs /ndl /nc /ns) ^& IF %ERRORLEVEL% LEQ 1 exit 0
	@(robocopy test release\$@\$(rev)\test /NFL /NDL /njh /njs /ndl /nc /s) ^& IF %ERRORLEVEL% LEQ 1 exit 0	
	@7z a .\release\$@_$(rev).zip .\release\$@\$(rev)\* > nul)

clean:
	del /q build
	
flash_a:
	openocd -f board/stm32f4discovery.cfg -c "program build/main.elf verify reset exit"

flash_b:
	openocd -f interface/stlink.cfg -f target/stm32l0.cfg -c "program build/main.elf verify reset exit"
	
flash_c:
	avrdude -p atmega328p -c arduino -P "COM7" -b 115200 -U flash:w:build\main.hex
