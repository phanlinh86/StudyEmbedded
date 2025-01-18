:: Compile, build and upload Arduino
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
set BOARD=atmega328p
set BOARD_ID=3

set COMPILER=avr-gcc
set MPU=atmega328p
set PROGRAMMER=avrdude
set OBJDUMP=avr-objcopy
set READELF=avr-readelf
set ERROR_FILE=error.txt

:: Project related
set COMPILE_FILE=main


:: Set the Arduino IDE path
set PATH=%PATH%;C:\Users\Phan Linh\Programs\avr8-gnu-toolchain\bin
set PATH=%PATH%;C:\Users\Phan Linh\Programs\avr8-gnu-toolchain\avr\bin
set PATH=%PATH%;C:\Users\Phan Linh\Programs\avrdude-mingw32

:: Check build directory exists
if not exist build ( 
    mkdir build 
) else (
    echo Deleting old build files
    del /Q build\*
)

:: Preprocess main.c to main.i
echo Preprocess main.c to main.i
echo %COMPILER% -E source\main.c -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% > build\main.i
%COMPILER% -E source\main.c -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% > build\main.i

:: Preprocess main.c to main.macro
echo Preprocess main.c to main.macro
echo %COMPILER% -dM -E source\main.c -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% > build\main.macro
%COMPILER% -dM -E source\main.c -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% > build\main.macro


:: Compile main.c to main.o
echo Compile main.c to main.o
echo %COMPILER% -c source\main.c -o build\main.o -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_main_%ERROR_FILE%
%COMPILER% -c source\main.c -o build\main.o -Wall -Os -DF_CPU=16000000UL -mmcu=%MPU% -c -DPROJECT=%PROJECT% -DBOARD_ID=%BOARD_ID% 2> build\%BOARD%_%PROJECT_NAME%_Compile_main_%ERROR_FILE%

:: Link main.o to main.elf
echo Link main.o to main.elf
echo %COMPILER% -mmcu=atmega328p -o build\main.elf build\main.o 2> build\%BOARD%_%PROJECT_NAME%_Link_%ERROR_FILE%.txt
%COMPILER% -mmcu=atmega328p -o build\main.elf build\main.o 2> build\%BOARD%_%PROJECT_NAME%_Link_%ERROR_FILE%.txt

:: Convert the elf file to hex
echo Convert the elf file to hex
echo %OBJDUMP% -O ihex build\main.elf build\main.hex
%OBJDUMP% -O ihex build\main.elf build\main.hex

:: Store symbo%l files
echo Store symbol files
echo %READELF% -s build\main.elf > build\main.sym
%READELF% -s build\main.elf > build\main.sym

