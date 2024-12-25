@echo off

:: Arguments
set PROJECT_NAME=%1

if "%PROJECT_NAME%" == "" (
    echo Usage: build.bat PROJECT_NAME
    echo PROJECT_NAME: The name of the project
    exit /b
) else if "%PROJECT_NAME%" == "BlinkLed" (
    set PROJECT=1
) else (
    echo Invalid project name
    exit /b
)

:: Build configuration
set BOARD=stm32f4discovery
set COMPILER=arm-none-eabi-gcc
set MPU=cortex-m4
set LINKER_SCRIPT=STM32F411VETX_FLASH.ld
set STARTUP_SCRIPT=startup_stm32f411vetx.s
set PROGRAMMER=openocd
set OPTIMIZATION=O0
set OBJDUMP=arm-none-eabi-objcopy


:: If build folder not existed, create one
if not exist build ( mkdir build )

:: Compile .c to .o
echo Compiling main.c to main.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\main.o source\main.c -DPROJECT=%PROJECT%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\main.o source\main.c -DPROJECT=%PROJECT%
echo Compiling startup.c to startup.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\startup.o board\%BOARD%\%STARTUP_SCRIPT% -DPROJECT=%PROJECT%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\startup.o board\%BOARD%\%STARTUP_SCRIPT% -DPROJECT=%PROJECT%
echo Compiling sysmem.c to sysmem.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\sysmem.o board\%BOARD%\sysmem.c -DPROJECT=%PROJECT%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\sysmem.o board\%BOARD%\sysmem.c -DPROJECT=%PROJECT%
echo Compiling syscalls.c to syscalls.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\syscalls.o board\%BOARD%\syscalls.c -DPROJECT=%PROJECT%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -mfloat-abi=hard -std=gnu11 -mfloat-abi=hard --specs=nano.specs -o build\syscalls.o board\%BOARD%\syscalls.c -DPROJECT=%PROJECT%

:: Link all .o files
echo Linking .o files and create main.elf
echo %COMPILER% -mcpu=%MPU% -T board\%BOARD%\%LINKER_SCRIPT% -Wl,-Map=build\main.map --specs=nano.specs -mfloat-abi=hard -mthumb -o build\main.elf build\main.o build\startup.o build\sysmem.o build\syscalls.o
%COMPILER% -mcpu=%MPU% -T board\%BOARD%\%LINKER_SCRIPT% -Wl,-Map=build\main.map --specs=nano.specs -mfloat-abi=hard -mthumb -o build\main.elf build\main.o build\startup.o build\sysmem.o build\syscalls.o

:: Convert to binary
echo Convert main.elf to main.hex and main.bin
echo %OBJDUMP% -O ihex build\main.elf build\main.hex
%OBJDUMP% -O ihex build\main.elf build\main.hex

