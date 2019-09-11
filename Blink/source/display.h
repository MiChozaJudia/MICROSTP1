#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "timer.h"



typedef enum { BRIGHTNESSHIGH=11, BRIGHTNESSMEDIUM=5, BRIGHTNESSLOW=1,BRIGHTNESSOFF=0 } tipo_de_brillo_t;
typedef enum {NOLED=0,FIRSTLED=1,SECONDLED=2,THIRDLED=3} led_t;

void inicializarDisplay(); //para FSM
void ResetDisplay(); //para FSM y displayWrapper
void escribirDisplay(char *p,uint8_t dondequerespuntito); //Para displayWrapper
void setBrightness(uint8_t value); //Para FSM
void setRollData(char *data); //Para displayWrapper
void escribirLeds(uint8_t position); //Para FSM (todavia no anda)
void initLeds(); //para FSM (todavia no anda)

#endif
