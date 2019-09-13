/*
 * fsm.h
 *
 *  Created on: 28/07/2014
 *      Author: Daniel Jacoby
 */


#ifndef FSM_H_
#define FSM_H_

#include <stdbool.h>
#include "database.h"

//LISTA DE EVENTOS CONTEMPLADOS POR LA FSM
typedef enum {NO_EVENT, BUTTON_PRESS,LONG_BUTTON_PRESS,LONGER_BUTTON_PRESS, ENCODER_RIGHT, ENCODER_LEFT, CARD_PASS,DOOR_CLOSED,VALID,NOT_VALID,FIN_TABLA} EVENTO;

typedef struct tabla_estado STATE;

//ESTRUCTURA QUE SE LE PASA A LAS FUNCIONES DE ACCIÓN DE LA FSM CON DATOS DE USUARIO QUE NECESITA LEER/MODIFICAR
typedef struct userdata
{
	//Entero entre 0 y 3, siendo 3 la luminosidad máxima del display y 0 siendo el display apagado.
	unsigned int luminosity;
	//True en caso de que se este modificando el ID, en caso de que se este modificando el PIN es false.
	bool isID;
	//True si se esta en  ID/PINvalidacion_state, false de lo contrario
	bool validation;
	//True si el ID/PIN ingresado es correcto, false de lo contrario
	bool validData;
	//True si estoy esperando una tarjeta
	bool waitingCard;
	//Número entero que representa que posición del ID/PIN se esta modificando actualmente. Su valor va de [0,ID/PIN_LEN-1]
	unsigned int curr_number;
	//Cantidad de ingresos fallidos en la sesión actual
	unsigned int curr_tries;

	//Estructura de datos que contiene los IDS y Pines permitidos.
	data_base database;

	//Arreglo que contiene el ID a ingresar
	char ID[ID_LENGTH];
	//Arreglo que contiene el PIN a ingresar
	char PIN[PIN_LENGTH];
}user_data;

struct tabla_estado
{
	EVENTO  event;//Evento que se esta esperando para actuar
	STATE *next_state;//Estado al que debo cambiar
	void(*callback)(user_data * userdata);//FUNCIÓN AL REALIZAR CUANDO SE CAMBIA DE ESTADO
};

// Interfaz
STATE* fsm(STATE *curr_state, EVENTO curr_event,user_data * data);

#endif /* FSM_H_ */
#pragma once
