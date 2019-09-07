/*
 * displayWrapper.h
 *
 *  Created on: 6 sep. 2019
 *      Author: Usuario
 */

#include "display.h"

#ifndef DISPLAYWRAPPER_H_
#define DISPLAYWRAPPER_H_

typedef enum
{
	IDLE,
	ID,
	CARD,
	PIN,
	ROLL
}displayMode_t;


void updateDisplay(displayMode_t displayMode, char* data, uint8_t number);

//Para FSM:
// IDLE-->sin importar data o number coloca BOCA en pantalla, es el reset o el estado inicial del display
// ID-->requiere un data de ID de no mas de 39 caracteres (intentar limitarse a 8) y un number que indica donde
//colocar el punto de referencia (number = 0 indica que el numero de referencia es el primero).
//la salida sera del ejemplo: entrada: "1234567" number 1, salida en display: "Id12."
//										"1234567" number 2, salida en display "d123."
//										"1234567" number 0, salida en display "Id1.2"
//CARD-->sin importar data o number escribe en el display "CArd"
//ROLL-->colocando data de 1 a 15 sin importar number scrollea en pantalla el contenido hasta que se llama a otra
//funcion que modifica panalla.
//PIN--> funciona muy parecido a ID solo que coloca en el display "Pin-." de llamarlo con number = 0
//in--. de llamarlo con number = 1 y asi hasta 4 ya que se admiten 5 numeros para PIN.

#endif /* DISPLAYWRAPPER_H_ */
