#include "reader.h"
#include "board.h"

#define ENDSENTINEL 0b11111
#define STARTSENTINEL 0b01011

#define CLEAR_PARITY_MASK 0b01111
//#define ID_LEN 8
#define TRACK2_LEN 400
#define DATA_BIT_SIZE 5

//PRIVATE FUNCTION
//static char ID[ID_LEN];
static char TRACK2[TRACK2_LEN];
static uint8_t bitCounter=0;
static uint8_t bitData=0;
static uint8_t dataCounter=0;

static uint8_t myPort;
static uint8_t myClockPin;
static uint8_t myEnablePin;
static uint8_t myDataPin;
static bool end=false;

static bool enable=false;

//PRIVATE FUNCTION
static void set_enable(void);
static void clean_enable(void);
static void get_data(void);
static void toggle_enable(void);
//static char get_lrc(void);
static bool check_lrc(void);
static char set_parity(char data);
static void reset_reading(void);
static char clear_parity(char data);


bool init_reader()
{

	//TEST
	gpioMode (PIN_TEST, OUTPUT);
	gpioWrite(PIN_TEST,true);
	//END TEST
	myClockPin=PIN2NUM(PIN_CLOCK);
	//myClockPin=clock_pin;
	myEnablePin=PIN2NUM(PIN_ENABLE);
	//myEnablePin=enable_pin;
	myDataPin=PIN2NUM(PIN_DATA);
	//myDataPin=data_pin;
	myPort=PIN2PORT(PIN_ENABLE);
	//myPort=port;
	pinIrqFun_t enableFun=toggle_enable;
	pinIrqFun_t clockFun=get_data;
	gpioMode(PORTNUM2PIN(myPort,myEnablePin),INPUT);
	gpioMode(PORTNUM2PIN(myPort,myClockPin),INPUT);
	gpioMode(PORTNUM2PIN(myPort,myDataPin),INPUT);

	gpioIRQ (PORTNUM2PIN(myPort,myEnablePin), GPIO_IRQ_MODE_BOTH_EDGES, enableFun);
	gpioIRQ (PORTNUM2PIN(myPort,myClockPin), GPIO_IRQ_MODE_FALLING_EDGE, clockFun);
	return 1;
}






void set_enable(void)
{
	enable=true;
	bitCounter=0;
	bitData=0;
	dataCounter=0;
	end=0;
}

void clean_enable(void)
{
	enable=false;
	bitCounter=0;
	bitData=0;
	dataCounter=0;


}

void get_data(void)
{
	if(enable)
	{
	bitData|=(!gpioRead(PORTNUM2PIN(myPort,myDataPin)))<<bitCounter;
	bitCounter++;
	if((dataCounter==0)&&(bitCounter==DATA_BIT_SIZE))
	{
		if(bitData==STARTSENTINEL)
		{
			TRACK2[dataCounter]=bitData;
			bitData=0;
			bitCounter=0;
			dataCounter++;
		}
		else
		{
			bitData=bitData>>1;
			bitCounter--;
		}
	}
	else if((bitCounter==DATA_BIT_SIZE)&&(dataCounter!=0))
	{
		if(bitData==ENDSENTINEL)end=true;

		TRACK2[dataCounter]=bitData;
		bitData=0;
		bitCounter=0;

		if(!(dataCounter==TRACK2_LEN))dataCounter++;


	}
	}
}

void toggle_enable(void)
{
	enable=gpioRead(PORTNUM2PIN(myPort,myEnablePin));
	if(enable)clean_enable();
	else set_enable();
}



char set_parity(char data)
{
	char retVal=data;

	uint8_t i=0;
	uint8_t ones=0;
	while(i<(DATA_BIT_SIZE-1))
	{
		if(data&(1<<i))ones++;
		i++;
	}
	if(!(ones%2)) retVal=retVal|0b10000;
	else	retVal=retVal&0b01111;

	return retVal;
}

bool check_lrc(void)
{
	char lrc;
	uint8_t i=0;
	do
	{
		lrc^=TRACK2[i];
		i++;
	}while(TRACK2[i-1]!=ENDSENTINEL);
	lrc=set_parity(lrc);
	return (TRACK2[i]==lrc);
}

void reset_reading(void)
{
	bitCounter=0;
	bitData=0;
	dataCounter=0;
	end=0;
}

char clear_parity(char data)
{
	return data&CLEAR_PARITY_MASK;
}

bool get_ID(char* ID,uint8_t id_len)
{
	bool retVal=false;
	if((end==false)&&(enable==true))
	{
		retVal=false;
	}
	else if((end==true)&&(enable==false))
	{

		if(!(check_lrc()))
		{
			retVal=false;
			reset_reading();
		}
		else
		{
			retVal=true;

			uint8_t j=0;
			while(j<id_len)
			{
				ID[j]=clear_parity(TRACK2[j+1])+'0';
				j++;
			}
			reset_reading();

		}

	}
	return retVal;
}
