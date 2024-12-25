//
// Created by Phan Linh on 25/12/2024.
//

#ifndef BLINKLED_H
#define BLINKLED_H


	#ifndef headerfiles
#include <stdint.h>
#include "../Mcu/McuDriverStm32F411E.c"
#include "../Int/InterruptServiceStm32F411E.c"
	#endif // headerfiles


#define LED_GREEN 	"D12"
#define LED_ORANGE	"D13"
#define LED_RED 	"D14"
#define LED_BLUE 	"D15"
#define BUTTON 		"A0"

typedef enum 
{
	GREEN 	= 0,
	ORANGE 	= 1,
	RED		= 2,	
	BLUE	= 3
} LED;


void InitLed(void);
void BlinkLed(void);
void ButtonInterrupt(void);
void InitButtonInterrupt(void);
void Init(void);
void Loop(void);

#endif //BLINKLED_H
