

#include "door.h"
#include "gpio.h"
#include "timer.h"
#include "board.h"


typedef enum
{
	OPEN,
	CLOSED
}doorState_t;

static bool doorJustClosed;
static doorState_t doorState;


void initDoor()
{
	gpioMode(PIN_STAT0,OUTPUT);
	gpioMode(PIN_STAT1,OUTPUT);
	gpioWrite(PIN_STAT0,false);
	gpioWrite(PIN_STAT1,false);
	doorState=CLOSED;
	doorJustClosed=false;
};


void openDoor(int time)
{
	doorState=OPEN;

	gpioWrite(PIN_STAT0,true);
	timerStart(timerGetId(), TIMER_MS2TICKS(time), TIM_MODE_SINGLESHOT, &closeDoor );
};

void openDoor5sec(void)
{
	openDoor(16000);
};

void closeDoor(void)
{
	doorState=CLOSED;
	//gpioWrite(myPin, false);
	gpioWrite(PIN_STAT0,false);
	doorJustClosed=true;
};

bool isDoorOpen(void)
{
	return (doorState==OPEN);
};

bool isDoorJustClosed()
{
	bool retVal;
	if(doorJustClosed==false) retVal=false;
	else
	{
		retVal=true;
		doorJustClosed=false;
	}
	return retVal;
}
