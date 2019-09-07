//includes
#include "timer.h"
#include "gpio.h"
#include "board.h"	//este tiene q irse
#include "encoder.h"

//structures

typedef struct {
	bool currentStatusLeft;
	bool pastLeft;
	bool currentStatusRight;
	bool pastRight;
	bool currentStatusButton;
	bool pastButton;
}edgeStatus;

//static variables for the file
 static pin_t pins[MAXPINSFORENCODER];
 static encoderStates nextState = IDLE;
 static encoderEvents newEvent;
 static uint32_t internalTimer;
 static edgeStatus edgeDetector;



//functions

//podria independizarme de gpio
void initEncoder (pin_t pinsAssigned[]){
	int i;
	//en el orden left right push
	for(i=0; i < MAXPINSFORENCODER; i++){
		pins[i] = pinsAssigned[i];
		gpioMode(pins[i],INPUT);
	}
	internalTimer = 0;
	timerInit();
	tim_id_t timerID = timerGetId();
	timerStart(timerID, 300, TIM_MODE_PERIODIC, &encoderFSM);
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
		if(newEvent == NEGEDGEROTLEFT) nextState = ROTRIGHT;
		//si me llega un flanco positivo a la derecha antes q el negativo a la izquierda
		//fue un movimiento en falso
		if(newEvent == POSEDGEROTRIGHT) nextState = IDLE;
	} break;
	case STARTROTLEFT:
	{
		//si me llega un flanco negativo derecha (defasaje) asumo q se giro efectivamente
		if(newEvent == NEGEDGEROTRIGHT) nextState = ROTLEFT;
		//si me llega un flanco positivo a la izquierda antes q el negativo a la derecha
		//fue un movimiento en falso
		if(newEvent == POSEDGEROTRIGHT) nextState = IDLE;
	} break;
	case ROTRIGHT:
	{
		gpioToggle(PIN_LED_BLUE);
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
		if(internalTimer > TIMEISLONG){
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
			if( (POSEDGEROTLEFT==newEvent) | (POSEDGEROTRIGHT == newEvent) ){
				nextState = PUSHANDROTATE;
				internalTimer =0;
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
	case PUSHANDROTATE:
	{
		nextState = IDLE;
	} break;

	}
}

encoderEvents readEvent (void) {
	//tengo q escibir
	encoderEvents encoder = NOEVENT;
	edge_t auxEdge;
	auxEdge = detectEdge(&edgeDetector.currentStatusButton, &edgeDetector.pastButton, pins[BUTTON]);
	if(auxEdge == POSEDGE) encoder = POSEDGEBUTTON;
	if(auxEdge == NEGEDGE) encoder = NEGEDGEBUTTON;
	auxEdge = detectEdge(&edgeDetector.currentStatusLeft, &edgeDetector.pastLeft, pins[LEFT]);
	if(auxEdge == POSEDGE) encoder = POSEDGEROTLEFT;
	if(auxEdge == NEGEDGE) encoder = NEGEDGEROTLEFT;
	auxEdge = detectEdge(&edgeDetector.currentStatusRight, &edgeDetector.pastRight, pins[RIGHT]);
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

encoderStates getFSM_ev(void){
	return nextState;
}
