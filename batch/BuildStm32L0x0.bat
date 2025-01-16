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
set BOARD=stm32l010rb
set BOARD_ID=2
set COMPILER=arm-none-eabi-gcc
set MPU=cortex-m0
set LINKER_SCRIPT=STM32L010RBTX_FLASH.ld
set STARTUP_SCRIPT=startup_stm32l010rbtx.s
set PROGRAMMER=openocd
set OPTIMIZATION=O0
set OBJDUMP=arm-none-eabi-objcopy
set FLOAT_ABI=soft
set READELF=arm-none-eabi-readelf
set ERROR_FILE=error.txt


:: If build folder not existed, create one
if not exist build ( mkdir build )

:: Compile .c to .o
echo Compiling main.c to main.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\main.o source\main.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_main_%ERROR_FILE%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb --std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\main.o source\main.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_main_%ERROR_FILE%
echo Compiling startup.c to startup.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb --std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\startup.o board\%BOARD%\%STARTUP_SCRIPT% -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_startup_%ERROR_FILE%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\startup.o board\%BOARD%\%STARTUP_SCRIPT% -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_startup_%ERROR_FILE%
echo Compiling sysmem.c to sysmem.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\sysmem.o board\%BOARD%\sysmem.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_sysmem_%ERROR_FILE%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\sysmem.o board\%BOARD%\sysmem.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_sysmem_%ERROR_FILE%
echo Compiling syscalls.c to syscalls.o
echo %COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\syscalls.o board\%BOARD%\syscalls.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_syscalls_%ERROR_FILE%
%COMPILER% -c -mcpu=%MPU% -%OPTIMIZATION% -mthumb -std=gnu11 -mfloat-abi=%FLOAT_ABI% --specs=nano.specs -o build\syscalls.o board\%BOARD%\syscalls.c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_syscalls_%ERROR_FILE%

:: Link all .o files
echo Linking .o files and create main.elf
echo %COMPILER% -mcpu=%MPU% -T board\%BOARD%\%LINKER_SCRIPT% -Wl,-Map=build\main.map --specs=nano.specs -mfloat-abi=%FLOAT_ABI% -mthumb -o build\main.elf build\main.o build\startup.o build\sysmem.o build\syscalls.o 2> build\%BOARD%_%PROJECT_NAME%_Link_%ERROR_FILE%
%COMPILER% -mcpu=%MPU% -T board\%BOARD%\%LINKER_SCRIPT% -Wl,-Map=build\main.map --specs=nano.specs -mfloat-abi=%FLOAT_ABI% -mthumb -o build\main.elf build\main.o build\startup.o build\sysmem.o build\syscalls.o 2> build\%BOARD%_%PROJECT_NAME%_Link_%ERROR_FILE%

:: Convert to binary
echo Convert main.elf to main.hex and main.bin
echo %OBJDUMP% -O ihex build\main.elf build\main.hex
%OBJDUMP% -O ihex build\main.elf build\main.hex

:: Store symbol files
echo Store symbol files
echo %READELF% -s build\main.elf > build\main.sym
%READELF% -s build\main.elf > build\main.sym