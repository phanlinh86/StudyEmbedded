#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdint.h>
#include "Utility/math.h"
#include "Utility/util.h"
#include "Int/Int.h"

// Project name
#define PROJECT_INVALID            0
#define PROJECT_BLINK_LED          1
#define PROJECT_DISPLAY_CLOCK      2
#define PROJECT_PS2                3


    #ifndef PROJECT
#define PROJECT PROJECT_INVALID
    #endif // PROJECT

// Board ID
#define BOARD_ID_STM32F411E         0x01
#define BOARD_ID_STM32L010RB        0x02
#define BOARD_ID_ATMEGA328P        	0x03

	#if ( PROJECT == PROJECT_BLINK_LED )
#include "Project/BlinkLed.h"
    #endif // PROJECT == ??

    #if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#include "Mcu/McuHardwareStm32F411E.h"
#include "Mcu/McuDriverStm32F411E.c"
#include "Int/InterruptServiceStm32F411E.c"
    #elif ( BOARD_ID ==  BOARD_ID_STM32L010RB )
#include "Mcu/McuHardwareStm32L0R0B.h"
#include "Mcu/McuDriverStm32L0R0B.c"
#include "Int/InterruptServiceStm32L0R0B.c"
    #elif ( BOARD_ID ==  BOARD_ID_ATMEGA328P )
#include "Mcu/McuHardwareAtmega328P.h"
#include "Mcu/McuDriverAtmega328P.c"
#include "Int/InterruptServiceAtmega328P.c"
    #endif // BOARD_ID == ?
	
	
#include "Int/InterruptDriver.c"

// Utility
	#if ( BOARD_ID !=  BOARD_ID_STM32L010RB )
#include "Utility/io.c"
#include "Utility/cmd.c"
	#endif // BOARD_ID ==  BOARD_ID_STM32F411E
	

	#if ( PROJECT == PROJECT_BLINK_LED )
#include "Project/BlinkLed.c"
    #endif // PROJECT == ??


#endif //MAIN_H
