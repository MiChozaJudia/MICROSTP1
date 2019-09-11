#include "fsm.h"



STATE* fsm(STATE *curr_state, EVENTO curr_event, user_data * data)
{
	while (curr_state->event != curr_event//Recorro las tablas de estado hasta encontrar el evento deseado
		&& curr_state->event != FIN_TABLA)//Si me llega algun estado no contemplado, se resetea la FSM
		++curr_state;
	(*curr_state->callback) (data);//Si encuentro el evento en la tabla de estado ejecuto la rutina de acción
	curr_state = curr_state->next_state;//Y mi estado actual es el que apuntaba la tabla de estados anterior
	return(curr_state);//Devuelvo el puntero a la nueva tabla de estados
}



