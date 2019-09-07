
#ifndef FSM_H_
#define FSM_H_

#define FIN_TABLA 0xFF
#include <stdbool.h>
#include "database.h"

#define PIN_LENGTH 5
#define ID_LENGTH 8
#define ADMINISTRATORS 5
#define BLACK_lIST 5


//LOS EVENTOS QUE SE CONTEMPLAN SON GIRO A LA DERECHA E IZQUIERDA DEL ENCODER, PRESS DEL ENCODER Y EL PASADO DE UNA TARJETA POR LA LECTORA.
typedef enum {NO_EVENT, BUTTON_PRESS,LONG_BUTTON_PRESS,LONGER_BUTTON_PRESS, ENCODER_RIGHT, ENCODER_LEFT, CARD_PASS,TIMER_END,VALID,NOT_VALID} EVENTO; 

typedef struct tabla_estado STATE;

//ESTRUCTURA QUE SE LE PASA A LAS FUNCIONES DE ACCI�N DE LA FSM CON DATOS DE USUARIO QUE NECESITA LEER/MODIFICAR
typedef struct userdata 
{
	unsigned int luminosity;
	bool IDorPIN;//true si se esta modoficando el ID, falso si se modifica el PIN
	bool validData;
	unsigned int curr_number;
	unsigned int curr_tries;

	data_base database;

	char ID[ID_LENGTH];
	char PIN[PIN_LENGTH];
}user_data;

struct tabla_estado
{
	EVENTO  event;
	STATE *next_state;
	void(*callback)(user_data * userdata);//FUNCI�N AL REALIZAR CUANDO SE CAMBIA DE ESTADO
};

// Interfaz
STATE* fsm(STATE *curr_state, EVENTO curr_event,user_data * data);

#endif /* FSM_H_ */
#pragma once
