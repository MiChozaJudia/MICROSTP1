/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "App.h"
#include "board.h"
#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void get_events(circ_bbuf_t * buff, user_data * data);
static void init_userdata(user_data* data);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (user_data *data, circ_bbuf_t * buff, STATE ** p2state)
{
	init_userdata(data);//inicializo la estructura de datos para la FSM
	init_buffer(buff,BUFFER_SIZE);//inicializo el buffer circular para usar como cola de eventos
	*p2state = FSM_GetInitState(data);// Inicializo la FSM con el estado inicial
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (user_data *data, circ_bbuf_t * buff, STATE ** p2state)
{
	get_events(buff, data);//Agrego eventos a la cola de eventos
	*p2state = fsm(*p2state, pop_buffer(buff), data);//Poppeo un evento de la cola de estados y se lo paso a la FSM
}
/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void get_events(circ_bbuf_t * buff, user_data * data)
{
	encoderStates encoder_state = getFSM_ev();//levanto el evento de encoder
	bool validity = data->validData;
	bool event_ocurred=false;

	if(buffer_is_full(buff)==0)//Si la cola de eventos no esta llena me fijo si puedo agregar el evento
	{
		if (get_ID(data->ID,ID_LENGTH))//Leo la tarjeta y guardo el ID leido
		{
			push_buffer(buff,CARD_PASS);//pusheo el evento a la cola de eventos
			event_ocurred=true;
		}
	}

	if(buffer_is_full(buff)==0)//Si la cola de eventos no esta llena me fijo si puedo agregar el evento
	{
		if(encoder_state ==SIMPLECLICK)//si el evento fue un click corto del encoder
		{
			push_buffer(buff,BUTTON_PRESS);//pusheo el evento
			event_ocurred=true;
		}
		else if(encoder_state ==LONGCLICK)//si el evento fue un click largo del encoder
		{
			push_buffer(buff,LONG_BUTTON_PRESS);//pusheo el evento
			event_ocurred=true;
		}
		else if(encoder_state ==PUSHANDROTATE)//si el eventof ue un click y rotacion del encoder
		{
			push_buffer(buff,LONGER_BUTTON_PRESS);//pusheo el evento
			event_ocurred=true;
		}
		else if(encoder_state ==ROTLEFT)//si el evento fue una rotación a la izquierda del encoder
		{
			push_buffer(buff,ENCODER_LEFT);//pusheo el evento
			event_ocurred=true;
		}
		else if(encoder_state ==ROTRIGHT)//si el evento fue una rotación a la derecha del encoder
		{
			push_buffer(buff,ENCODER_RIGHT);//pusheo el evento
			event_ocurred=true;
		}
	}

	if(data->validation)//Si estoy en el estado de validación de datos
	{
		if (buffer_is_full(buff) == 0)//Si la cola de eventos no esta llena me fijo si puedo agregar el evento
		{
			if (validity) push_buffer(buff,VALID);//pusheo evento de dato vá	lido
			else push_buffer(buff,NOT_VALID);//pusheo evento de dato no válido
			event_ocurred=true;
		}
	}

	 if(!event_ocurred)//Si no ocurrió ningun evento pusheo un NO_EVENT
	{
		if (buffer_is_full(buff)==0)
		{
			push_buffer(buff,NO_EVENT);//pusheo el evento a la cola de eventos
		}
	}
}

void init_userdata(user_data* data)
 {
	 data->luminosity=DEFAULT_LUMINANCE;//Se arranca con la luminosidad máxima por default.
	 data->validation = false;
	 data->validData = false;
	 data->isID=true;//Como primero se va a ingresar el ID su valor default es true
	 data->curr_number=0;
	 data->curr_tries = 0;
	 for (int i = 0; i < ID_LENGTH; i++)
	 {
		 data->ID[i] = '1'+i;
	 }
	 for (int i = 0; i < PIN_LENGTH; i++)
	 {
		 data->PIN[i] = '0';
	 }
	 init_database(&data->database);// inicializo la base de datos
 }

/*******************************************************************************
 ******************************************************************************/
