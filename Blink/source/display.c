#include "display.h"
#include "board.h"
#include "gpio.h"
#include "timer.h"

//------------------------------------VARIABLES GLOBALES------------------------------------

typedef struct SegmentValue {
   uint8_t pin;
   uint8_t value;

} PinParameters;

typedef struct DisplayControl {
   PinParameters a;
   PinParameters b;
   PinParameters c;
   PinParameters d;
   PinParameters e;
   PinParameters f;
   PinParameters g;
   PinParameters DP;

} DisplayControlSegment;

typedef struct DisplaySelection{
	PinParameters sel0;
	PinParameters sel1;
} DisplaySelection;

static uint8_t msj[4] = {'0','0','0','0'};
static uint8_t MsjLine=0;
static uint8_t rollmsj[40];
static uint8_t buffermsj[4];
static uint8_t rollstatus=0;
static uint8_t rollIndex=0;
static uint8_t BrightnessMsjLine = 0;
static uint8_t Brightness = 10;
static uint8_t dondepuntito =5;
tipo_de_brillo_t BrightnessValue;
DisplayControlSegment ControlSegment;
DisplaySelection Selection;
//Leds ControlLed;
//------------------------------------VARIABLES GLOBALES------------------------------------


//--------------------------DECLARACIONES ESTO VA EN UN .H LO DEJO AHORA ACA PARA TESTEAR-----------------------
static void place2Selection(uint8_t place);
//Uso interno y exclusivo de display.c
//Descripción: esta función recibe un uint8_t como por ejemplo 0 1 2 o 3 y traduce la informacion a el binario
//correspondiente a las lineas de seleccion para los cuadrados del display.

static void writeBlancSpace();
//Uso interno y exclusivo de display.c
//Desc: utiliza BrightnessMsjLine. Coloca un espacio vacio en un cuadrado del display. Al iterar en esta funcion
//que a su vez es llamada por escribir() se coloca un espacio en blanco en cada uno de los cuadrados del display

static void escribir();
//Uso interno y exclusivo de display.c
//Desc: Funciona con interrupciones desde timer, es la encargada de escribir en el display constantemente a una
//frecuencia establecida.


static void tryRoll();

static void writeOnSelectors();

static void writeOnDisplay();

static void val2Segments(uint8_t val,uint8_t puntitoBool);

//static void val2Leds(unsigned int place);

//--------------------------DECLARACIONES ESTO VA EN UN .H LO DEJO AHORA ACA PARA TESTEAR-----------------------


//-------------------------FUNCIONES---------------------------------------------------------------------------
void inicializarDisplay(){

//Configuración del modo de los Pines para controlar el display y guardado de los pines para escritura por interrupcion
	gpioMode(PIN_SEGA,OUTPUT);
	ControlSegment.a.pin=PIN_SEGA;

	gpioMode(PIN_SEGB,OUTPUT);
	ControlSegment.b.pin=PIN_SEGB;

	gpioMode(PIN_SEGC,OUTPUT);
	ControlSegment.c.pin=PIN_SEGC;

	gpioMode(PIN_SEGD,OUTPUT);
	ControlSegment.d.pin=PIN_SEGD;

	gpioMode(PIN_SEGE,OUTPUT);
	ControlSegment.e.pin=PIN_SEGE;

	gpioMode(PIN_SEGF,OUTPUT);
	ControlSegment.f.pin=PIN_SEGF;

	gpioMode(PIN_SEGG,OUTPUT);
	ControlSegment.g.pin=PIN_SEGG;

	gpioMode(PIN_PD,OUTPUT);
	ControlSegment.DP.pin=PIN_PD;

	gpioMode(PIN_SEL0,OUTPUT);
	Selection.sel0.pin=PIN_SEL0;

	gpioMode(PIN_SEL1,OUTPUT);
	Selection.sel1.pin=PIN_SEL1;

	//Brightness por default la seteo en HIGH.
	setBrightness(BRIGHTNESSHIGH);

	//Coloco todod en '80CA'.
	ResetDisplay();

	//inicializo Timer.
	timerInit();

	//Config de interrupción para escribir (multiplexar) en el display
	tim_id_t timerEscribir = timerGetId();
	timerStart(timerEscribir,1,TIM_MODE_PERIODIC,&escribir);

	//Config de interrupción para realizar un roll en el display
	tim_id_t timerRoll = timerGetId();
	timerStart(timerRoll,3333,TIM_MODE_PERIODIC,&tryRoll);

	//pin utilizado para medir las interrupciones
	gpioMode(PIN_SYS,OUTPUT);

}


void setBrightness(tipo_de_brillo_t value){
	//toma valores entre 0 y 10
	BrightnessValue = value;
}

void ResetDisplay(){
	rollstatus=0;
	escribirDisplay("80CA",5);
}

static void escribir(){
//llamada por systick
	gpioWrite(PIN_SYS, 1);
	if(Brightness<BrightnessValue){
		if (MsjLine < 4){
			place2Selection(MsjLine);
			writeOnSelectors();
			if(MsjLine == dondepuntito){
				val2Segments(msj[MsjLine],1);
			}else{
				val2Segments(msj[MsjLine],0);
			}
			writeOnDisplay();
			MsjLine++;
		}else{
			MsjLine=0;
			if(Brightness != 0){
				Brightness--;
			}else{
				Brightness=10;
			}
		}
	}else{
		writeBlancSpace();
	}
	gpioWrite(PIN_SYS, 0);
}

static void writeBlancSpace(){
	if ( BrightnessMsjLine < 4){
		place2Selection(BrightnessMsjLine);
		writeOnSelectors();
		val2Segments(' ',0);
		writeOnDisplay();
		BrightnessMsjLine++;
	}else{
		BrightnessMsjLine=0;
		if(Brightness != 0){
			Brightness--;
		}else{
			Brightness=10;
		}
	}
}
static void tryRoll(){
	if(rollstatus==1){
		for(int i=0; i<3;i++){
			msj[i]=msj[i+1];
		}
		if(rollmsj[rollIndex]!='\0'){
			msj[3]=rollmsj[rollIndex];
			rollIndex++;
		}else{
			rollIndex =0;
			for(int i=0; i<4;i++){
				msj[i]=buffermsj[i];
			}

		}

	}else{}
}


void setRollData(char *data){
	int j = 0;
	while(data[j]!='\0' && (j<8)){
		if(j<4){
		msj[j]= data[j];
		buffermsj[j]=data[j];
		}else{
			rollmsj[j-4]=data[j];
		}
		j++;
	}
	rollmsj[j]='\0';
	rollstatus=1;
	rollIndex=0;

}

void escribirDisplay(char *p,uint8_t dondequereselpuntito){
	for(int i =0; i<4; i++){
		msj[i] = p[i];
	}
	dondepuntito = dondequereselpuntito; //entre 0 y 3. 4 es no mostrarlo
	rollstatus=0;
}



static void writeOnSelectors(){
	gpioWrite(Selection.sel0.pin,Selection.sel0.value);
	gpioWrite(Selection.sel1.pin,Selection.sel1.value);
}

static void writeOnDisplay(){
	gpioWrite(ControlSegment.a.pin,ControlSegment.a.value);
	gpioWrite(ControlSegment.b.pin,ControlSegment.b.value);
	gpioWrite(ControlSegment.c.pin,ControlSegment.c.value);
	gpioWrite(ControlSegment.d.pin,ControlSegment.d.value);
	gpioWrite(ControlSegment.e.pin,ControlSegment.e.value);
	gpioWrite(ControlSegment.f.pin,ControlSegment.f.value);
	gpioWrite(ControlSegment.g.pin,ControlSegment.g.value);
	gpioWrite(ControlSegment.DP.pin,ControlSegment.DP.value);
}

static void place2Selection(uint8_t place){
	if(place == 0){
		Selection.sel0.value = 0;
		Selection.sel1.value = 0;
	}else if(place == 1){
		Selection.sel0.value = 1;
		Selection.sel1.value = 0;
	}else if(place == 2){
		Selection.sel0.value = 0;
		Selection.sel1.value = 1;
	}else if (place == 3){
		Selection.sel0.value = 1;
		Selection.sel1.value = 1;
	}else {}

}


static void val2Segments(uint8_t val,uint8_t puntito){

//Tengo por asumido y entendido que cuando sale un 1 al display entonces se coloca apagado ese segmento
		if(val=='0'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=1;
				ControlSegment.f.value=1;
				ControlSegment.g.value=0;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if(val=='1'){
				ControlSegment.a.value=0;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=0;
				ControlSegment.e.value=0;
				ControlSegment.f.value=0;
				ControlSegment.g.value=0;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if(val=='2'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=0;
				ControlSegment.d.value=1;
				ControlSegment.e.value=1;
				ControlSegment.f.value=0;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if(val=='3'){

				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=0;
				ControlSegment.f.value=0;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='4'){
				ControlSegment.a.value=0;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=0;
				ControlSegment.e.value=0;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='5'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=0;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=0;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='6'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=0;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=1;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='7'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=0;
				ControlSegment.e.value=0;
				ControlSegment.f.value=0;
				ControlSegment.g.value=0;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='8'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=1;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='9'){

				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=1;
				ControlSegment.e.value=0;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if (val=='A'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=1;
				ControlSegment.c.value=1;
				ControlSegment.d.value=0;
				ControlSegment.e.value=1;
				ControlSegment.f.value=1;
				ControlSegment.g.value=1;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if(val=='C'){
				ControlSegment.a.value=1;
				ControlSegment.b.value=0;
				ControlSegment.c.value=0;
				ControlSegment.d.value=1;
				ControlSegment.e.value=1;
				ControlSegment.f.value=1;
				ControlSegment.g.value=0;
				if(puntito == 1){
					ControlSegment.DP.value=1;
				}else{
					ControlSegment.DP.value=0;
				}

		}else if(val=='.'){
				ControlSegment.a.value=0;
				ControlSegment.b.value=0;
				ControlSegment.c.value=0;
				ControlSegment.d.value=0;
				ControlSegment.e.value=0;
				ControlSegment.f.value=0;
				ControlSegment.g.value=0;
				ControlSegment.DP.value=1;

		}else if (val == ' '){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=0;
			ControlSegment.f.value=0;
			ControlSegment.g.value=0;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'r'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'd'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=1;
			ControlSegment.c.value=1;
			ControlSegment.d.value=1;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'P'){
			ControlSegment.a.value=1;
			ControlSegment.b.value=1;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=1;
			ControlSegment.f.value=1;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'n'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=1;
			ControlSegment.d.value=0;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'o'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=1;
			ControlSegment.d.value=1;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'i'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=0;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}else if (val == 'I'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=1;
			ControlSegment.f.value=1;
			ControlSegment.g.value=0;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}
		else if (val == '-'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=0;
			ControlSegment.e.value=0;
			ControlSegment.f.value=0;
			ControlSegment.g.value=1;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}
		else if (val == 'L'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=0;
			ControlSegment.d.value=1;
			ControlSegment.e.value=1;
			ControlSegment.f.value=1;
			ControlSegment.g.value=0;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
		}
		else if (val == 'u'){
			ControlSegment.a.value=0;
			ControlSegment.b.value=0;
			ControlSegment.c.value=1;
			ControlSegment.d.value=1;
			ControlSegment.e.value=1;
			ControlSegment.f.value=0;
			ControlSegment.g.value=0;
			if(puntito == 1){
				ControlSegment.DP.value=1;
			}else{
				ControlSegment.DP.value=0;
			}
				}
		else {}

}
