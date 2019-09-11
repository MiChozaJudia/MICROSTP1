#include "fsm.h"
#include "fsmtable.h"


/*Foward Declarations de los estados*/

extern STATE boca_state[];
extern STATE setLuminosity_state[];
extern STATE waitCard_state[];
extern STATE waitID_state[];
extern STATE idValidation_state[];
extern STATE displayID_state[];
extern STATE waitPIN_state[];
extern STATE PINValidation_state[];
extern STATE enter_state[];

/*rutinas de acción*/

static void do_nothing(user_data* data);

static void setLumin(user_data* data);
static void increase_luminosity(user_data* data);
static void decrease_luminosity(user_data* data);

static void set_waitCard(user_data* data);

static void set_waitID(user_data* data);
static void displayID(user_data* data);
static void check_ID(user_data* data);
static void set_IDretry(user_data* data);

static void check_PIN(user_data* data);
static void set_waitPIN(user_data* data);
static void set_PINretry(user_data* data);

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
	{BUTTON_PRESS,waitID_state,set_waitID},
	{LONG_BUTTON_PRESS,boca_state,do_nothing},
	{LONGER_BUTTON_PRESS,boca_state,do_nothing},
	{ENCODER_LEFT,setLuminosity_state,setLumin},
	{ENCODER_RIGHT,waitCard_state,set_waitCard},
	{CARD_PASS,boca_state,do_nothing},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,boca_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///setluminosity_state
STATE setLuminosity_state[] =
{
	{BUTTON_PRESS,boca_state,cancel},
	{LONG_BUTTON_PRESS,setLuminosity_state,do_nothing},
	{LONGER_BUTTON_PRESS,setLuminosity_state,do_nothing},
	{ENCODER_LEFT,setLuminosity_state,decrease_luminosity},
	{ENCODER_RIGHT,setLuminosity_state,increase_luminosity},
	{CARD_PASS,setLuminosity_state,do_nothing},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,setLuminosity_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitCard_state
STATE waitCard_state[] =
{
	{BUTTON_PRESS,boca_state,cancel},
	{LONG_BUTTON_PRESS,waitCard_state,do_nothing},
	{LONGER_BUTTON_PRESS,waitCard_state,do_nothing},
	{ENCODER_LEFT,waitCard_state,do_nothing},
	{ENCODER_RIGHT,waitCard_state,do_nothing},
	{CARD_PASS,idValidation_state,check_ID},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,waitCard_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitID_state
STATE waitID_state[] =
{
	{BUTTON_PRESS,waitID_state,increase_num},
	{LONG_BUTTON_PRESS,idValidation_state,check_ID},
	{LONGER_BUTTON_PRESS,idValidation_state,check_ID},
	{ENCODER_LEFT,waitID_state,switch_left},
	{ENCODER_RIGHT,waitID_state,switch_right},
	{CARD_PASS,waitID_state,do_nothing},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,waitID_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///idValidation_state
STATE idValidation_state[] =
{
	{BUTTON_PRESS,idValidation_state,do_nothing},
	{LONG_BUTTON_PRESS,idValidation_state,do_nothing},
	{LONGER_BUTTON_PRESS,idValidation_state,do_nothing},
	{ENCODER_LEFT,idValidation_state,do_nothing},
	{ENCODER_RIGHT,idValidation_state,do_nothing},
	{CARD_PASS,idValidation_state,do_nothing},
	{VALID,displayID_state,displayID},
	{NOT_VALID,boca_state,set_IDretry},
	{NO_EVENT,idValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

STATE displayID_state[] =
{
	{BUTTON_PRESS,waitPIN_state,set_waitPIN},
	{LONG_BUTTON_PRESS,boca_state,cancel},
	{LONGER_BUTTON_PRESS,displayID_state,do_nothing},
	{ENCODER_LEFT,displayID_state,do_nothing},
	{ENCODER_RIGHT,displayID_state,do_nothing},
	{CARD_PASS,displayID_state,do_nothing},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,displayID_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitPIN_state
STATE waitPIN_state[] =
{
	{BUTTON_PRESS,waitPIN_state,increase_num},
	{LONG_BUTTON_PRESS,PINValidation_state,check_PIN},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,waitPIN_state,switch_left},
	{ENCODER_RIGHT,waitPIN_state,switch_right},
	{CARD_PASS,waitPIN_state,do_nothing},
	{VALID,boca_state,reset_FSM},
	{NOT_VALID,boca_state,reset_FSM},
	{NO_EVENT,waitPIN_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///PINValidation_state
STATE PINValidation_state[] =
{
	{BUTTON_PRESS,PINValidation_state,do_nothing},
	{LONG_BUTTON_PRESS,PINValidation_state,do_nothing},
	{LONGER_BUTTON_PRESS,PINValidation_state,do_nothing},
	{ENCODER_LEFT,PINValidation_state,do_nothing},
	{ENCODER_RIGHT,PINValidation_state,do_nothing},
	{CARD_PASS,PINValidation_state,do_nothing},
	{VALID,enter_state,open_door},
	{NOT_VALID,boca_state,set_PINretry},
	{NO_EVENT,PINValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///enter_state
STATE enter_state[] =
{
	{BUTTON_PRESS,enter_state,do_nothing},
	{LONG_BUTTON_PRESS,enter_state,do_nothing},
	{LONGER_BUTTON_PRESS,enter_state,do_nothing},
	{ENCODER_LEFT,enter_state,do_nothing},
	{ENCODER_RIGHT,enter_state,do_nothing},
	{CARD_PASS,enter_state,do_nothing},
	{VALID,enter_state,do_nothing},
	{NOT_VALID,enter_state,do_nothing},
	{NO_EVENT,enter_state,do_nothing},
	{FIN_TABLA,enter_state,do_nothing}//CAMBIAR ESTO, ESTA COMO DEBUG AHORA
};

///========interfaz=================
STATE *FSM_GetInitState(user_data* data)
{
	init_reader();
	initEncoder();
	init_display();
	//init_door();
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,0);//IMPRIMO BOCA
	return (boca_state);
}

///========Rutinas de accion===============
///POR AHORA TODAS ESTAS FUNCIONES SON TEMPORARIAS TENGO QUE PASAR A VER LOS HEADERS DE LOS PERIFERICOS

///Preparo el display para la pantalla de seteo de brillo
void setLumin(user_data* data)
{
	displayMode_t mode=LUMIN;
	updateDisplay(mode,data->ID,0);//
}

///AUMENTO LA LUMINOSIDAD DEL DISPLAY
void increase_luminosity(user_data* data)
{
	if(data->luminosity<3)
		data->luminosity++;
	set_brightness(data->luminosity);
}

///DISMINUYO LA LUMINOSIDAD DEL DISPLAY
void decrease_luminosity(user_data* data)
{
	if(data->luminosity>0)
		data->luminosity--;
	set_brightness(data->luminosity);
}

///
void set_waitCard(user_data* data)
{
	displayMode_t mode=CARD;
	updateDisplay(mode,data->ID,0);//IMPRIMO CARD
}

///PREPARO PARA ESPERAR UN ID
void set_waitID(user_data* data)
{
	displayMode_t mode=ID;
	updateDisplay(mode,data->ID,data->curr_number);
}

///
void displayID(user_data* data)
{
	data->validation=false;
	displayMode_t mode=ROLL;
	updateDisplay(mode, data->ID, 5);//el ultimo valor es la freq que se mueve el roll por ahora es un magic number
}

///
void check_ID(user_data* data)
{
	data->validation = true;
	data->validData =is_ID_allowed(&data->database,data->ID, ID_LENGTH);

}

///
void set_IDretry(user_data* data)
{
	data->curr_tries++;
	data->validData = false;
	data->validation = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = '0';
	}
	displayMode_t mode=ID;
	updateDisplay(mode,data->ID,data->curr_number);
}

///PREPARO PARA ESPERAR UN PIN
void set_waitPIN(user_data* data)
{
	data->validData = false;
	data->validation = false;
	data->curr_number = 0;
	data->isID=false;
	displayMode_t mode=PIN;
	updateDisplay(mode,data->PIN,data->curr_number);
}

///
void check_PIN(user_data* data)
{
	data->validation = true;
	data->validData = is_PIN_correct(&data->database,data->ID, ID_LENGTH, data->PIN, PIN_LENGTH);
}

///
void set_PINretry(user_data* data)
{
	data->curr_tries++;
	data->validData = false;
	data->validation = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = '0';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,0);//IMPRIMO BOCA
}

///
void increase_num(user_data* data)
{
	if (data->isID)
	{
		if (data->ID[data->curr_number] == '9')
			data->ID[data->curr_number] = '0';
		else data->ID[data->curr_number]++;
		displayMode_t mode=ID;
		updateDisplay(mode,data->ID,data->curr_number);
	}
	else
	{
		if (data->PIN[data->curr_number] == '9')
			data->PIN[data->curr_number] = '0';
		else data->PIN[data->curr_number]++;
		displayMode_t mode=PIN;
		updateDisplay(mode,data->PIN,data->curr_number);
	}
}

///
void switch_left(user_data* data)
{
	if (data->curr_number > 0)
		data->curr_number--;
	if(data->isID)
	{
		displayMode_t mode=ID;
		updateDisplay(mode,data->ID,data->curr_number);
	}
	else
	{
		displayMode_t mode=PIN;
		updateDisplay(mode,data->PIN,data->curr_number);
	}
}

///
void switch_right(user_data* data)
{
	if (data->isID)
	{
		if (data->curr_number < ID_LENGTH-1)
			data->curr_number++;
		displayMode_t mode=ID;
		updateDisplay(mode,data->ID,data->curr_number);
	}
	else
	{
		if (data->curr_number < PIN_LENGTH)
			data->curr_number++;
		displayMode_t mode=PIN;
		updateDisplay(mode,data->PIN,data->curr_number);
	}
}

///
void open_door(user_data* data)
{
	data->validation=false;
	displayMode_t mode=DOOR;
	leds(2);
	updateDisplay(mode,data->ID,0);//

	/*if (!isDoorOpen())
	{
		openDoor5sec();
	}*/
}

///
void cancel(user_data* data)
{
	data->isID = true;
	data->curr_number = 0;
	data->validData = false;
	data->validation = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = '0';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = '0';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,0);//IMPRIMO BOCA
}

///
void do_nothing(user_data* data)
{

}

///
void reset_FSM(user_data* data)
{
	data->isID = true;
	data->validData = false;
	data->curr_tries = 0;
	data->curr_number = 0;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = '0';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = '0';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,0);//IMPRIMO BOCA
}
