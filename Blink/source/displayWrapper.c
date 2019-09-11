
#include "displayWrapper.h"

#define IDLESTRING "BOCA"
#define IDSTRING "Id"
#define CARDSTRING "CArd"
#define DISPLAYLEN	4
#define MINNUM 3
#define PINSTRING "Pin-----"
char iddleString[5]=IDLESTRING;

enum
{
	POINT0,
	POINT1,
	POINT2,
	POINT3,
	NOPOINT
} ;


static void concatID(char *a,char*b);

void init_display(void)
{
	inicializarDisplay();
}

void init_leds(void)
{
	initLeds();
}



void set_brightness(unsigned int valorDel0Al3)
{
	if(valorDel0Al3 == 0){
		setBrightness(BRIGHTNESSOFF);
	}else if (valorDel0Al3 == 1){
		setBrightness(BRIGHTNESSLOW);
	}else if (valorDel0Al3 == 2){
		setBrightness(BRIGHTNESSMEDIUM);
	}else if (valorDel0Al3 == 3){
		setBrightness(BRIGHTNESSHIGH);
	}
}

void leds(unsigned int cual){
	//cual = 0 apaga, 1 prende el primero, 2 el segundo, 3 el tercero,
	escribirLeds(cual);
}

void updateDisplay(displayMode_t displayMode, char* data, uint8_t number)
{
	if(displayMode==BOCA)
	{
		//escribirDisplay(iddleString,NODISPLAY);
		ResetDisplay();
	}
	else if(displayMode==LUMIN)
		{
			//escribirDisplay(iddleString,NODISPLAY);
		escribirDisplay("Lun ",NOPOINT);
		}
	else if(displayMode == CARD)
	{
		escribirDisplay("CArd",NOPOINT);
		//EScribe en display card
	}
	else if(displayMode == DOOR)
		{
			escribirDisplay("d00r",NOPOINT);
			//EScribe en display card
		}
	else if (displayMode == ROLL)
	{
		//aca el string de la longitud que qiuieras rollea
		setRollData(data);
	}
	else if(displayMode==PIN)
	{
		char pinstring[9]= PINSTRING;
		pinstring[number+3]= data[number];
		escribirDisplay((pinstring+number),POINT3);
	}
	else if(displayMode==ID)
	{
		//anexar ID al principio
		char idString[10] = IDSTRING;
		concatID(idString,data);

		if(number>0)
		{
			escribirDisplay(idString+number-1,POINT3);
		}
		else if(number == 0)
		{
			escribirDisplay(idString,number+2);
		}
	}
}

static void concatID(char *a,char*b){
	for(int i=0; i<8;i++){
		a[i+2]=b[i];
	}
}
