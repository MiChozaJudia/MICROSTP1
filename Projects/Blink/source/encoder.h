/*
 * encoder.h
 *
 *  Created on: 4 sep. 2019
 *      Author: Tomas
 */

#ifndef ENCODER_H_
#define ENCODER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stddef.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAXPINSFORENCODER 3
#define TIMEISLONG	200


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum {LEFT, RIGHT, BUTTON};
typedef enum {NONE, POSEDGE, NEGEDGE} edge_t;

typedef enum {
	IDLE,
	ROTLEFT,
	STARTROTLEFT,
	ROTRIGHT,
	STARTROTRIGHT,
	CLICK,
	LONGCLICK,
	SIMPLECLICK,
	PUSHANDROTATE
} encoderStates;

typedef enum {
	NOEVENT,
	NEGEDGEROTLEFT,
	POSEDGEROTLEFT,
	NEGEDGEROTRIGHT,
	POSEDGEROTRIGHT,
	NEGEDGEBUTTON,
	POSEDGEBUTTON
} encoderEvents;

	/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initializes the pins assigned to the enconder
 * @param pin[] array of pins available (according PORTNUM2PIN)
 */
void initEncoder (pin_t pinsAssigned[]);

/**
 * @brief Detects which movement was made in the encoder
 */
void encoderFSM ( void );

/**
 * @brief Detects which event happened in the encoder
 */
encoderEvents readEvent ( void );

/**
 * @brief Detects which edge occured in the respective pin
 * @param current variable where the current value read will be stored
 * @param past variable where the past value read will be stored
 * @param pin the pin whose edge will be detected
 */
edge_t detectEdge (bool *current, bool *past, pin_t pin);

/**
 * @brief Returns fsm state which is the event for the FSM on the upper level file
 */
encoderStates getFSM_ev(void);

/*******************************************************************************
 ******************************************************************************/


#endif /* ENCODER_H_ */
