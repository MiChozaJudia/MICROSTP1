/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define PIN_LED_RED     // ???
#define PIN_LED_GREEN   // ???
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

#define LED_ACTIVE      LOW


// On Board User Switches
#define PIN_SW2         // ???
#define PIN_SW3         // ???

#define SW_ACTIVE       // ???
#define SW_INPUT_TYPE   // ???


#define PIN_SEGA		PORTNUM2PIN(PC,4)
#define PIN_SEGB		PORTNUM2PIN(PC,12)
#define PIN_SEGC		PORTNUM2PIN(PC,5)
#define PIN_SEGD		PORTNUM2PIN(PC,7)
#define PIN_SEGE		PORTNUM2PIN(PC,0)
#define PIN_SEGF		PORTNUM2PIN(PC,9)
#define PIN_SEGG		PORTNUM2PIN(PC,8)
#define PIN_PD			PORTNUM2PIN(PC,1)
#define PIN_SEL0		PORTNUM2PIN(PB,19)
#define PIN_SEL1		PORTNUM2PIN(PB,18)
#define PIN_STAT0		PORTNUM2PIN(PB,2)
#define PIN_STAT1		PORTNUM2PIN(PB,3)

#define PIN_LEFT		PORTNUM2PIN(PC,16)
#define PIN_RIGHT		PORTNUM2PIN(PC,17)
#define PIN_BUTTON		PORTNUM2PIN(PB,9)

#define PIN_ENABLE		PORTNUM2PIN(PD,1)
#define PIN_DATA		PORTNUM2PIN(PD,2)
#define PIN_CLOCK		PORTNUM2PIN(PD,3)

#define PIN_TEST  		PORTNUM2PIN(PE,26)
#define PIN_SYS			PORTNUM2PIN(PE,24)

/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
