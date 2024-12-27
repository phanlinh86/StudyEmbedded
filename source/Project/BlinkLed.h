//
// Created by Phan Linh on 25/12/2024.
//

#ifndef BLINKLED_H
#define BLINKLED_H


typedef enum
{
	GREEN 	= 0,
	ORANGE 	= 1,
	RED		= 2,
	BLUE	= 3
} LED;

    #if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#define NUMBER_SUPPORTED_LED    4
#define LED_GREEN 	"D12"
#define LED_ORANGE	"D13"
#define LED_RED 	"D14"
#define LED_BLUE 	"D15"
#define BUTTON 		"C3"
    #elif ( BOARD_ID ==  BOARD_ID_STM32L010RB )
#define NUMBER_SUPPORTED_LED    1
#define LED_GREEN 	"A5"
#define LED_ORANGE	""
#define LED_RED 	""
#define LED_BLUE 	""
#define BUTTON      "A2"
    #endif // BOARD_ID == ?




void InitLed(void);
void BlinkLed(void);
void ButtonInterrupt(void);
void InitButtonInterrupt(void);
void Init(void);
void Loop(void);

#endif //BLINKLED_H
