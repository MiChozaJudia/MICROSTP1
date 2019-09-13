/*
 * displayWrapper.h
 *
 *  Created on: 6 sep. 2019
 *      Author: Usuario
 */

#include "display.h"

#ifndef DISPLAYWRAPPER_H_
#define DISPLAYWRAPPER_H_

#define DEFAULT_LUMINANCE 3// Valor que va entre 0 y 3 dependiendo el brillo deseado del display. 3 es máximo brillo, 0 es apagado.

typedef enum
{
	BOCA,
	LUMIN,
	ADD,
	BAN,
	DOOR,
	ID,
	CARD,
	PIN,
	ROLL
}displayMode_t;

void updateDisplay(displayMode_t displayMode, char* data, uint8_t number);

//updateDisplay(displayMode_y displayMode, char* data, uint8_t number):
// BOCA-->sin importar data o number coloca BOCA en pantalla, es el reset o el estado inicial del display
// ID-->requiere un data de ID de no mas de 8 caracteres  y un number que indica donde
//colocar el punto de referencia (number = 0 indica que el numero de referencia es el primero).
//la salida sera del ejemplo: entrada: "1234567" number 1, salida en display: "Id12."
//										"1234567" number 2, salida en display "d123."
//										"1234567" number 0, salida en display "Id1.2"
//CARD-->sin importar data o number escribe en el display "CArd"
//ROLL-->colocando data de 1 a 8 sin importar number scrollea en pantalla el contenido hasta que se llama a otra
//funcion que modifica panalla.
//PIN--> funciona muy parecido a ID solo que coloca en el display "Pin-." de llamarlo con number = 0
//in--. de llamarlo con number = 1 y asi hasta 4 ya que se admiten 5 numeros para PIN.
//BAN->-->sin importar data o number escribe en el display "BAn"
//ADD-->sin importar data o number escribe en el display "Add "
//DOOR-->sin importar data o number escribe en el display "d00r"
//
void init_display(void);
//Llama a inicializarDisplay de display.c

void set_brightness(unsigned int val);
//LLama a setBrightness de display.c
#endif /* DISPLAYWRAPPER_H_ */
