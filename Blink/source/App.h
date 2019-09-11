/*
 * App.h
 *
 *  Created on: 7 sep. 2019
 *      Author: Manuel
 */

#ifndef APP_H_
#define APP_H_

#include "buffer.h"
#include "fsm.h"
#include "fsmtable.h"

//Función que se llama una vez para inicializar lo relacionado al programa
void App_Init (user_data *data, circ_bbuf_t * buff, STATE ** p2state);

//Función que se llama cada vez en el run loop
void App_Run (user_data *data, circ_bbuf_t * buff, STATE ** p2state);

#endif /* APP_H_ */
