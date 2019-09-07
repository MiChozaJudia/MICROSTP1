#include <stdio.h>
#include "fsm.h"



STATE* fsm(STATE *curr_state, EVENTO curr_event, user_data * data)
{
	printf(" >>%c<<\n ", curr_event); //just for test (debug)
	while (curr_state->event != curr_event		//Recorre las tablas de estado
		&& curr_state->event != FIN_TABLA)
		++curr_state;
	(*curr_state->callback) (data);          /*rutina de accion corresondiente*/
	curr_state = curr_state->next_state;   /*siguiente estado*/
	return(curr_state);
}



