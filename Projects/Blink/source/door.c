

#include "door.h"
#include "gpio.h"
#include "timer.h"

typedef enum
{
	OPEN,
	CLOSED
}doorState_t;

static doorState_t doorState;
static uint8_t myPin;

void initDoor(uint8_t pin)
{
	myPin=pin;
	gpioMode(myPin,OUTPUT);
	gpioWrite(myPin,false);
	doorState=CLOSED;	
};


void openDoor(int time)
{
	doorState=OPEN;	
	gpioWrite(myPin,true);
	timerStart(timerGetId(), TIMER_MS2TICKS(time), TIM_MODE_SINGLESHOT, closeDoor );
};

void openDoor5sec(void)
{
	openDoor(5000);
};

void closeDoor(void)
{
	doorState=CLOSED;
	gpioWrite(myPin, false);
};

bool isDoorOpen(void)
{
	return (doorState==OPEN);
};
