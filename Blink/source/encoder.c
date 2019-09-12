/*
 * encoder.c
 *
 *  Created on: 4 sep. 2019
 *      Author: Tomas
 */

//includes
#include "timer.h"
#include "gpio.h"
#include "board.h"	//este tiene q irse
#include "encoder.h"

//structures

typedef enum {
	NOEVENT,
	NEGEDGEROTLEFT,
	POSEDGEROTLEFT,
	NEGEDGEROTRIGHT,
	POSEDGEROTRIGHT,
	NEGEDGEBUTTON,
	POSEDGEBUTTON
} encoderEvents;

typedef struct {
	bool currentStatusLeft;
	bool pastLeft;
	bool currentStatusRight;
	bool pastRight;
	bool currentStatusButton;
	bool pastButton;
}edgeStatus;

//static variables for the file
 static encoderStates nextState = IDLE;
 static encoderEvents newEvent;
 static uint32_t internalTimer;
 static edgeStatus edgeDetector;



//functions

 /**
  * @brief Detects which movement was made in the encoder
  */
 static void encoderFSM ( void );

 /**
  * @brief Detects which event happened in the encoder
  */
static encoderEvents readEvent ( void );

 /**
  * @brief Detects which edge occured in the respective pin
  * @param current variable where the current value read will be stored
  * @param past variable where the past value read will be stored
  * @param pin the pin whose edge will be detected
  */
static edge_t detectEdge (bool *current, bool *past, pin_t pin);

//podria independizarme de gpio
void initEncoder (void){
	gpioMode(PIN_BUTTON,INPUT);
	gpioMode(PIN_LEFT,INPUT);
	gpioMode(PIN_RIGHT,INPUT);
	internalTimer = 0;
	timerInit();
	tim_id_t timerID = timerGetId();
	timerStart(timerID, 33, TIM_MODE_PERIODIC, &encoderFSM);
	//porque en este encoder son activo bajo los pines
	edgeDetector.pastButton=TRUE;
	edgeDetector.pastLeft=TRUE;
	edgeDetector.pastRight=TRUE;
}

void encoderFSM (void) {
	newEvent = readEvent();
	switch(nextState) {
	case IDLE:
	{
		// si me llega flanco negativo a la derecha arranca la rotacion a la derecha
		if(NEGEDGEROTRIGHT == newEvent)	nextState = STARTROTRIGHT;
		// si me llega flanco negativo a la izquierda arranca la rotacion a la izquierda
		if(NEGEDGEROTLEFT == newEvent)	nextState = STARTROTLEFT;
		if(NEGEDGEBUTTON == newEvent)	nextState = CLICK;
	} break;
	case STARTROTRIGHT:
	{
		//si me llega un flanco negativo izquierda (defasaje) asumo q se giro efectivamente
		if(newEvent == NEGEDGEROTLEFT)
			{
			nextState = ROTRIGHT;
			}
		//si me llega un flanco positivo a la derecha antes q el negativo a la izquierda
		//fue un movimiento en falso
		if(newEvent == POSEDGEROTRIGHT) nextState = IDLE;
	} break;
	case STARTROTLEFT:
	{
		//si me llega un flanco negativo derecha (defasaje) asumo q se giro efectivamente
		if(newEvent == NEGEDGEROTRIGHT)
			{
			nextState = ROTLEFT;
			}
		//si me llega un flanco positivo a la izquierda antes q el negativo a la derecha
		//fue un movimiento en falso
		if(newEvent == POSEDGEROTRIGHT) nextState = IDLE;
	} break;
	case ROTRIGHT:
	{
		//gpioToggle(PIN_LED_BLUE);
		nextState = IDLE;
	} break;
	case ROTLEFT:
	{
		//gpioToggle(PIN_LED_GREEN);
		nextState = IDLE;
	} break;
	case CLICK:
	{
		internalTimer++;
		if(internalTimer > TIMEISTOOLONG){
			if (POSEDGEBUTTON == newEvent){
				nextState = VERYLONGCLICK;
				internalTimer=0;
			}

		}
		else if(internalTimer > TIMEISLONG){
			if (POSEDGEBUTTON == newEvent){
				nextState = LONGCLICK;
				internalTimer=0;
			}

		}
		else{		//internaltimer es menor
			if(POSEDGEBUTTON == newEvent) {
				nextState = SIMPLECLICK;
				internalTimer=0;
			}
		}
	} break;
	case LONGCLICK:
	{
		nextState = IDLE;
	} break;
	case SIMPLECLICK:
	{
		nextState = IDLE;
	} break;
	case VERYLONGCLICK:
	{
		nextState = IDLE;
	} break;

	}
}

encoderEvents readEvent (void) {
	//tengo q escibir
	encoderEvents encoder = NOEVENT;
	edge_t auxEdge;
	auxEdge = detectEdge(&edgeDetector.currentStatusButton, &edgeDetector.pastButton, PIN_BUTTON);
	if(auxEdge == POSEDGE) encoder = POSEDGEBUTTON;
	if(auxEdge == NEGEDGE) encoder = NEGEDGEBUTTON;
	auxEdge = detectEdge(&edgeDetector.currentStatusLeft, &edgeDetector.pastLeft, PIN_LEFT);
	if(auxEdge == POSEDGE) encoder = POSEDGEROTLEFT;
	if(auxEdge == NEGEDGE) encoder = NEGEDGEROTLEFT;
	auxEdge = detectEdge(&edgeDetector.currentStatusRight, &edgeDetector.pastRight, PIN_RIGHT);
	if(auxEdge == POSEDGE) encoder = POSEDGEROTRIGHT;
	if(auxEdge == NEGEDGE) encoder = NEGEDGEROTRIGHT;
	return encoder;
}

edge_t detectEdge (bool *current, bool *past, pin_t pin) {

	edge_t edge;
	*current = gpioRead(pin);
	if((*current != *past) && (*current == FALSE)){
		edge = NEGEDGE;
	}
	else if((*current != *past) && (*current == TRUE)){
		edge = POSEDGE;
	}
	else edge = NONE;
	*past = *current;
	return edge;
}

encoderStates getFSM_ev(void)
{
	encoderStates state= nextState;
	if((state==SIMPLECLICK) || (state==ROTRIGHT) || (state==ROTLEFT) || (state==LONGCLICK) || (state==VERYLONGCLICK))
	{
		nextState=IDLE;
	}
	return state;
}
