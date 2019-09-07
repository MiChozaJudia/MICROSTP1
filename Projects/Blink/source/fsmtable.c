#include <stdio.h>
#include "fsm.h"
#include "fsmtable.h"
#include "reader.h"
#include "encoder.h"
#include "displayWrapper.h"

/*Foward Declarations*/

extern STATE boca_state[];
extern STATE setLuminosity_state[];
extern STATE waitCard_state[];
extern STATE waitID_state[];
extern STATE idValidation_state[];
extern STATE displayID_state[];
extern STATE waitPIN_state[];
extern STATE PINValidation_state[];
extern STATE enter_state[];

/// prototipos
static void do_nothing(user_data* data);

static void setLumin(user_data* data);
static void increase_luminosity(user_data* data);
static void decrease_luminosity(user_data* data);

static void set_waitCard(user_data* data);

static void set_waitID(user_data* data);
static void displayID(user_data* data);
static void check_ID(user_data* data);
static void set_retry(user_data* data);

static void check_PIN(user_data* data);
static void set_waitPIN(user_data* data);

static void increase_num(user_data* data);
static void switch_left(user_data* data);
static void switch_right(user_data* data);

static void cancel(user_data* data);
static void reset_FSM(user_data* data);	

static void open_door(user_data* data);

/*** tablas de estado ***/

///boca_state
STATE boca_state[] =
{
	{ENCODER_LEFT,setLuminosity_state,setLumin},
	{ENCODER_RIGHT,waitCard_state,set_waitCard},
	{BUTTON_PRESS,waitID_state,set_waitID},
	{NO_EVENT,boca_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///setluminosity_state
STATE setLuminosity_state[] =
{
	{ENCODER_LEFT,setLuminosity_state,decrease_luminosity},
	{ENCODER_RIGHT,setLuminosity_state,increase_luminosity},
	{BUTTON_PRESS,boca_state,cancel},
	{NO_EVENT,setLuminosity_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitCard_state
STATE waitCard_state[] =
{
	{BUTTON_PRESS,boca_state,cancel},
	{CARD_PASS,idValidation_state,check_ID},
	{NO_EVENT,waitCard_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitID_state 
STATE waitID_state[] =
{
	{BUTTON_PRESS,waitID_state,increase_num},
	{LONG_BUTTON_PRESS,idValidation_state,check_ID},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,waitID_state,switch_left},
	{ENCODER_RIGHT,waitID_state,switch_right},
	{NO_EVENT,waitID_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///idValidation_state
STATE idValidation_state[] =
{
	{VALID,displayID_state,displayID},
	{NOT_VALID,waitID_state,set_retry},
	{NO_EVENT,idValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

STATE displayID_state[] =
{
	{BUTTON_PRESS,waitPIN_state,set_waitPIN},
	{LONG_BUTTON_PRESS,boca_state,cancel},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitPIN_state
STATE waitPIN_state[] =
{
	{BUTTON_PRESS,waitPIN_state,increase_num},
	{LONG_BUTTON_PRESS,PINValidation_state,do_nothing},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,waitPIN_state,switch_left},
	{ENCODER_RIGHT,waitPIN_state,switch_right},
	{NO_EVENT,waitPIN_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///PINValidation_state
STATE PINValidation_state[] =
{
	{VALID,enter_state,open_door},
	{NOT_VALID,boca_state,cancel},
	{NO_EVENT,PINValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///enter_state
STATE enter_state[] =
{
	{NO_EVENT,enter_state,do_nothing},
	{FIN_TABLA,enter_state,do_nothing}//CAMBIAR ESTO, ESTA COMO DEBUG AHORA
};

///========interfaz=================
STATE *FSM_GetInitState(void)
{
	init_reader();
	init_encoder();
	init_display();
	init_door();
	printBOCA();
	return (boca_state);
}

///========Rutinas de accion===============
///POR AHORA TODAS ESTAS FUNCIONES SON TEMPORARIAS TENGO QUE PASAR A VER LOS HEADERS DE LOS PERIFERICOS

///Preparo el display para la pantalla de seteo de brillo
void setLumin(user_data* data)
{
	print4('8888', 4);
}

///AUMENTO LA LUMINOSIDAD DEL DISPLAY
void increase_luminosity(user_data* data)
{
	data->luminosity++;
	set_brightness(data->luminosity);
}

///DISMINUYO LA LUMINOSIDAD DEL DISPLAY
void decrease_luminosity(user_data* data)
{
	data->luminosity--;
	set_brightness(data->luminosity);
}

///
void set_waitCard(user_data* data)
{
	printCard();
}

///PREPARO PARA ESPERAR UN ID
void set_waitID(user_data* data)
{
	//update_display(ID,data->curr_number)
}

///
void displayID(user_data* data)
{
	roll(data->ID, ID_LENGTH, 4);//el ultimo valor es la freq que se mueve el roll por ahora es un magic number
}

///
void check_ID(user_data* data)
{
	/*for (int i = 0; i < ADMINISTRATORS; i++)
	{
		data->validData = true;
		for (int j = 0; j < ID_LENGTH; j++)
		{
			if (data->ID[data->curr_number] != data->white_listID[i][data->curr_number])
			{
				data->validData = false;
				break;
			}

		}
		if (data->validData == true)
			break;
	}*/
	if (is_allowed(data->ID, ID_LENGTH))
		data->validData = true;

}

///PREPARO PARA ESPERAR UN PIN
void set_waitPIN(user_data* data)
{
	data->validData = false;
	printIDandPin(data->IDorPIN, data->PIN, PIN_LENGTH, 0);//EL 1 del principio es un magic number que alude a que es un PIN lo que quiero mostrar.
}

///
void check_PIN(user_data* data)
{
	for (int i = 0; i < ADMINISTRATORS; i++)
	{
		data->validData = true;
		for (int j = 0; j < PIN_LENGTH; j++)
		{
			if (data->PIN[data->curr_number] != data->white_listPIN[i][data->curr_number])
			{
				data->validData = false;
				break;
			}

		}
		if (data->validData == true)
			break;
	}
}

///
void set_retry(user_data* data)
{
	data->curr_tries++;
	data->validData = false;
	if (!data->IDorPIN)
	{
		for (int i = 0; i < PIN_LENGTH; i++)
		{
			data->PIN[i] = 0;
		}
	}
}

///DISMINUYO LA LUMINOSIDAD DEL DISPLAY
void increase_num(user_data* data)
{
	if (data->IDorPIN)
	{
		if (data->ID[data->curr_number] == '9')
			data->ID[data->curr_number] == '0';
		else data->ID[data->curr_number]++;
		printIDandPin(data->IDorPIN, data->ID, ID_LENGTH, data->curr_number);
	}
	else
	{
		if (data->PIN[data->curr_number] == '9')
			data->PIN[data->curr_number] == '0';
		else data->PIN[data->curr_number]++;
		printIDandPin(data->IDorPIN, data->PIN, PIN_LENGTH, data->curr_number);
	}
}

///
void switch_left(user_data* data)
{
	if (data->curr_number > 0)
		data->curr_number--;
	if(data->IDorPIN)
		printIDandPin(data->IDorPIN, data->ID, ID_LENGTH, data->curr_number);
	else
		printIDandPin(data->IDorPIN, data->PIN, PIN_LENGTH, data->curr_number);

}

///
void switch_right(user_data* data)
{
	if (data->IDorPIN)
	{
		if (data->curr_number < ID_LENGTH)
			data->curr_number++;
	}
	else
	{
		if (data->curr_number < PIN_LENGTH)
			data->curr_number++;
	}
	if (data->IDorPIN)
		printIDandPin(data->IDorPIN, data->ID, ID_LENGTH, data->curr_number);
	else
		printIDandPin(data->IDorPIN, data->PIN, PIN_LENGTH, data->curr_number);
}

///
void open_door(user_data* data)
{
	print4('1234',4);
}

///
void cancel(user_data* data)
{
	data->IDorPIN = true;
	data->curr_tries = 0;
	data->curr_number = 0;
	data->validData = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 0;
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 0;
	}
	printBOCA();
}

///
void do_nothing(user_data* data)
{

}

///
void reset_FSM(user_data* data)
{
	data->IDorPIN = true;
	data->validData = false;
	data->curr_tries = 0;
	data->curr_number = 0;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 0;
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 0;
	}
	printf("Reset\n");
}
