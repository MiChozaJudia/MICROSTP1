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
#define TIMEISTOOLONG	400
#define TIMEISLONG		100


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum {LEFT, RIGHT, BUTTON};
typedef enum {NONE, POSEDGE, NEGEDGE} edge_t;

typedef enum {
	ROTRIGHT,
	IDLE,
	STARTROTLEFT,
	STARTROTRIGHT,
	CLICK,
	LONGCLICK,
	VERYLONGCLICK,
	ROTLEFT,
	SIMPLECLICK,
} encoderStates;



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
void initEncoder (void);



/**
 * @brief Returns fsm state which is the event for the FSM on the upper level file
 */
encoderStates getFSM_ev(void);

/*******************************************************************************
 ******************************************************************************/


#endif /* ENCODER_H_ */
