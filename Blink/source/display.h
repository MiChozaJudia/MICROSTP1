#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdint.h>
#include <stdbool.h>




typedef enum { BRIGHTNESSHIGH=11, BRIGHTNESSMEDIUM=5, BRIGHTNESSLOW=1,BRIGHTNESSOFF=0 } tipo_de_brillo_t;
typedef enum {NOLED=0,FIRSTLED=1,SECONDLED=2,THIRDLED=3} led_t;

void inicializarDisplay();
//InicializarDispla():
//Utiliza los pines asignados en board.h para configurar los pines desigandos para utilizar el display
//Inicializa Timer
//Inicializa la interrupción para el multiplexor
//Inicializa la interrupción para poder realizar un roll
//Configura inicialmente brightness max y '80CA'
void ResetDisplay();
//ResetDisplay():
//Coloca '80CA' en el display y finaliza el proceso de roll o pisa la palabra existente
void escribirDisplay(char *p,uint8_t DP);
//escribirDisplay(char *p, uint8_t DP):
//toma un valor en string de 4 caracteres y lo coloca en el display.
//toma un valor de DP para indicar si se quiere prender el . para alguno de los segmentos
void setBrightness(uint8_t value);
//setBrightness(uint8_t value):
//Toma un valor de 0 a 4 siendo 4 la intensidad máxima y 0 apagado el nivel de intensidad del display
void setRollData(char *data);
//setRollDatachar(char *data):
//Recibe un string de hasta 8 caracteres y lo muestra en formato roll en el display


#endif
