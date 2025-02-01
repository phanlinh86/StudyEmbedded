# Purpose: Build and flash for MyEmbeddedSystem project
#
# The makefile is used to build and flash the project
# The project can be separated by the project name
# blink is used for blink led project
#
# The target can be separated by the project name and the board used
# _a is used for STM32F4Discovery board
# _b is used for STM32F4Nucleo board
# _c is used for Atmega328p board
#
# Compilation options: rev, rel, path, com --------------------------
# rev : is used to specify firmware revision number to be released
# This number can be read out from the board using USART command
#
# rel : is used to specify release type
# 0 is used for build only (default)
# 1 is used for release the build
# 2 is used for release the build with elf, hex, and sym files
# 3 is used for release the build with elf, hex, and sym files and test folder
#
# path : is used to define the path of the elf file
# The elf path is used to flash the elf file to the board
# Default: build/main.elf
#
# com : is used to define which COM port is used to flash the hex file to the board
# Default: COM7
#
# Usage of the makefile ----------------------------------------------
# 1. To build all compilation option
# make all
# 2. To build all compilation option with revision and release option
# make all rev=xxxx rel=0
# 3. To release the build with elf, hex, and sym files and test folder
# make all rev=xxxx rel=3
# 4. To flash the build to the STM32F4 board with elf from default build/main.elf
# make flash_a
# 5. To flash the build to the STM32L0 board with elf from release/blink_b/xxxx/main.elf
# make flash_b elf_path=release/blink_b/xxxx/main.elf
# 6. Clean the build folder
# make clean


all: blink

blink: blink_a blink_b blink_c

options: clean
ifeq ($(rev),)
rev = 0000
endif
ifeq ($(rel),)
rel = 0
endif
ifeq ($(path),)
path = build/main.elf
endif
ifeq ($(com),)
com = COM7
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
	openocd -f board/stm32f4discovery.cfg -c "program $(path) verify reset exit"

flash_b:
	openocd -f interface/stlink.cfg -f target/stm32l0.cfg -c "program $(path) verify reset exit"
	
flash_c:
	avrdude -p atmega328p -c arduino -P "$(com)" -b 115200 -U flash:w:$(path)
