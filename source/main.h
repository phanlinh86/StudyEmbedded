#ifndef MAIN_H
#define MAIN_H

// Project name
#define PROJECT_INVALID            0
#define PROJECT_BLINK_LED          1
#define PROJECT_DISPLAY_CLOCK      2
#define PROJECT_PS2                3

    #ifndef PROJECT
#define PROJECT PROJECT_INVALID
    #endif // PROJECT

	#if ( PROJECT == PROJECT_BLINK_LED )
#include "Project/BlinkLed.c"
    #endif // PROJECT == ??


#endif //MAIN_H
