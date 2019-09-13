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
extern STATE show_add_state[];
extern STATE wait_ID_2add[];
extern STATE is_ID_addable[];
extern STATE wait_PIN_2add[];
extern STATE is_PIN_addable[];

extern STATE show_ban_state[];
extern STATE wait_ID_2ban[];
extern STATE is_bannable_state[];

/*rutinas de acción*/

//
static void do_nothing(user_data* data);

//Función que setea el display para el estado de setear brillo
static void setLumin(user_data* data);
//Función que incrementa el brillo del display
static void increase_luminosity(user_data* data);
//Función que decrementa el brillo del display
static void decrease_luminosity(user_data* data);

//Función que setea el display para el estado de esperar una lectura de tarjeta
static void set_waitCard(user_data* data);

//Función que setea el display para el estado de esperar el ingreso de un ID
static void set_waitID(user_data* data);
//Función que setea al display para mostrar el ID presente en forma de marquesina
static void displayID(user_data* data);
//Función que chequea si el ID ingresado es valido
static void check_ID(user_data* data);
//Función llamada cuando el ID ingresado no es valido
static void set_IDretry(user_data* data);

///Función que chequea si el PIN ingresado es valido
static void check_PIN(user_data* data);
//Función que setea el display para el estado de esperar el ingreso de un PIN
static void set_waitPIN(user_data* data);
//Función llamada cuando el PIN ingresado no es valido
static void set_PINretry(user_data* data);

//Función que incrementa unitariamente el número marcado por el cursor
static void increase_num(user_data* data);
//Función que permite acceder al número a la izquierda del seleccionado en presente por el cursor
static void switch_left(user_data* data);
//Función que permite acceder al número a la derecha del seleccionado en presente por el cursor
static void switch_right(user_data* data);

//Función llamada cuando se desea cancelar la acción y volver al estado BOCA de la FSM
static void cancel(user_data* data);
//Función llamada que reseta todos los valores de la FSM
static void reset_FSM(user_data* data);

//Función que hace que la puerta se abra por 5 segundos
static void open_door(user_data* data);

//Funciòn que habilita al display para el estado ADD_ADMIN
static void show_add(user_data* data);
////Funciòn que habilita al display mostrar el PIN para agregar
static void set_PINadd(user_data* data);
//Funcion que agrega el ID con su PIN asociado a la base de datos
static void add_ID(user_data* data);
//Función que chequea si el ID que se desea agregar es valido
static void check_addID(user_data* data);
//Función que chequea si el PIN que se desea agregar es valido
static void check_addPIN(user_data* data);

//Funciòn que habilita al display para el estado BAN_ID
static void show_ban(user_data* data);
//Funciòn que habilita al display mostrar el ID para bannear
static void set_IDban(user_data* data);
//Funcion que bannea el ID ingresado
static void banID(user_data* data);

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
	{DOOR_CLOSED,boca_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
	{NO_EVENT,boca_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///setluminosity_state
STATE setLuminosity_state[] =
{
	{BUTTON_PRESS,boca_state,cancel},
	{LONG_BUTTON_PRESS,boca_state,cancel},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,setLuminosity_state,decrease_luminosity},
	{ENCODER_RIGHT,setLuminosity_state,increase_luminosity},
	{CARD_PASS,setLuminosity_state,do_nothing},
	{DOOR_CLOSED,setLuminosity_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
	{NO_EVENT,setLuminosity_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///waitCard_state
STATE waitCard_state[] =
{
	{BUTTON_PRESS,boca_state,cancel},
	{LONG_BUTTON_PRESS,boca_state,cancel},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,waitCard_state,do_nothing},
	{ENCODER_RIGHT,waitCard_state,do_nothing},
	{CARD_PASS,idValidation_state,check_ID},
	{DOOR_CLOSED,waitCard_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
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
	{CARD_PASS,waitID_state,do_nothing},
	{DOOR_CLOSED,waitID_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
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
	{DOOR_CLOSED,idValidation_state,do_nothing},
	{VALID,displayID_state,displayID},
	{NOT_VALID,waitID_state,set_IDretry},
	{NO_EVENT,idValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///displayID_state
STATE displayID_state[] =
{
	{BUTTON_PRESS,waitPIN_state,set_waitPIN},
	{LONG_BUTTON_PRESS,boca_state,cancel},
	{LONGER_BUTTON_PRESS,displayID_state,do_nothing},
	{ENCODER_LEFT,displayID_state,do_nothing},
	{ENCODER_RIGHT,displayID_state,do_nothing},
	{CARD_PASS,displayID_state,do_nothing},
	{DOOR_CLOSED,displayID_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
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
	{DOOR_CLOSED,waitPIN_state,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
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
	{DOOR_CLOSED,PINValidation_state,do_nothing},
	{VALID,enter_state,open_door},
	{NOT_VALID,waitPIN_state,set_PINretry},
	{NO_EVENT,PINValidation_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///enter_state
STATE enter_state[] =
{
	{BUTTON_PRESS,enter_state,do_nothing},
	{LONG_BUTTON_PRESS,enter_state,do_nothing},
	{LONGER_BUTTON_PRESS,enter_state,do_nothing},
	{ENCODER_LEFT,show_ban_state,show_ban},
	{ENCODER_RIGHT,show_add_state,show_add},
	{CARD_PASS,enter_state,do_nothing},
	{DOOR_CLOSED,boca_state,cancel},
	{VALID,enter_state,do_nothing},
	{NOT_VALID,enter_state,do_nothing},
	{NO_EVENT,enter_state,do_nothing},
	{FIN_TABLA,enter_state,reset_FSM}//CAMBIAR ESTO, ESTA COMO DEBUG AHORA
};

///show_add_state
STATE show_add_state[] =
{
	{BUTTON_PRESS,wait_ID_2add,set_IDban},
	{LONG_BUTTON_PRESS,wait_ID_2add,set_IDban},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,show_add_state,do_nothing},
	{ENCODER_RIGHT,show_add_state,do_nothing},
	{CARD_PASS,show_add_state,do_nothing},
	{DOOR_CLOSED,show_add_state,do_nothing},
	{VALID,show_add_state,do_nothing},
	{NOT_VALID,show_add_state,do_nothing},
	{NO_EVENT,show_add_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};


///wait_ID_2add
STATE wait_ID_2add[] =
{
	{BUTTON_PRESS,wait_ID_2add,increase_num},
	{LONG_BUTTON_PRESS,is_ID_addable,check_addID},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,wait_ID_2add,switch_left},
	{ENCODER_RIGHT,wait_ID_2add,switch_right},
	{CARD_PASS,wait_ID_2add,do_nothing},
	{DOOR_CLOSED,wait_ID_2add,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
	{NO_EVENT,wait_ID_2add,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///is_ID_addable
STATE is_ID_addable[] =
{
	{BUTTON_PRESS,is_ID_addable,do_nothing},
	{LONG_BUTTON_PRESS,is_ID_addable,do_nothing},
	{LONGER_BUTTON_PRESS,is_ID_addable,do_nothing},
	{ENCODER_LEFT,is_ID_addable,do_nothing},
	{ENCODER_RIGHT,is_ID_addable,do_nothing},
	{CARD_PASS,is_ID_addable,do_nothing},
	{DOOR_CLOSED,is_ID_addable,do_nothing},
	{VALID,wait_PIN_2add,set_PINadd},
	{NOT_VALID,boca_state,cancel},
	{NO_EVENT,is_ID_addable,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///wait_PIN_2add
STATE wait_PIN_2add[] =
{
	{BUTTON_PRESS,wait_PIN_2add,increase_num},
	{LONG_BUTTON_PRESS,is_PIN_addable,check_addPIN},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,wait_PIN_2add,switch_left},
	{ENCODER_RIGHT,wait_PIN_2add,switch_right},
	{CARD_PASS,wait_PIN_2add,do_nothing},
	{DOOR_CLOSED,wait_PIN_2add,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
	{NO_EVENT,wait_PIN_2add,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///is_PIN_addable
STATE is_PIN_addable[] =
{
	{BUTTON_PRESS,is_PIN_addable,do_nothing},
	{LONG_BUTTON_PRESS,is_PIN_addable,do_nothing},
	{LONGER_BUTTON_PRESS,is_PIN_addable,do_nothing},
	{ENCODER_LEFT,is_PIN_addable,do_nothing},
	{ENCODER_RIGHT,is_PIN_addable,do_nothing},
	{CARD_PASS,is_PIN_addable,do_nothing},
	{DOOR_CLOSED,is_PIN_addable,do_nothing},
	{VALID,boca_state,add_ID},
	{NOT_VALID,boca_state,cancel},
	{NO_EVENT,is_PIN_addable,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

///show_ban_state
STATE show_ban_state[] =
{
	{BUTTON_PRESS,wait_ID_2ban,set_IDban},
	{LONG_BUTTON_PRESS,wait_ID_2ban,set_IDban},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,show_ban_state,do_nothing},
	{ENCODER_RIGHT,show_ban_state,do_nothing},
	{CARD_PASS,show_ban_state,do_nothing},
	{DOOR_CLOSED,show_ban_state,do_nothing},
	{VALID,show_ban_state,do_nothing},
	{NOT_VALID,show_ban_state,do_nothing},
	{NO_EVENT,show_ban_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};



///wait_ID_2ban
STATE wait_ID_2ban[] =
{
	{BUTTON_PRESS,wait_ID_2ban,increase_num},
	{LONG_BUTTON_PRESS,is_bannable_state,check_ID},
	{LONGER_BUTTON_PRESS,boca_state,cancel},
	{ENCODER_LEFT,wait_ID_2ban,switch_left},
	{ENCODER_RIGHT,wait_ID_2ban,switch_right},
	{CARD_PASS,wait_ID_2ban,do_nothing},
	{DOOR_CLOSED,wait_ID_2ban,do_nothing},
	{VALID,boca_state,do_nothing},
	{NOT_VALID,boca_state,do_nothing},
	{NO_EVENT,wait_ID_2ban,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};

//is_bannable_state
STATE is_bannable_state[] =
{
	{BUTTON_PRESS,is_bannable_state,do_nothing},
	{LONG_BUTTON_PRESS,is_bannable_state,do_nothing},
	{LONGER_BUTTON_PRESS,is_bannable_state,do_nothing},
	{ENCODER_LEFT,is_bannable_state,do_nothing},
	{ENCODER_RIGHT,is_bannable_state,do_nothing},
	{CARD_PASS,is_bannable_state,do_nothing},
	{DOOR_CLOSED,is_bannable_state,do_nothing},
	{VALID,boca_state,banID},
	{NOT_VALID,boca_state,cancel},
	{NO_EVENT,is_bannable_state,do_nothing},
	{FIN_TABLA,boca_state,reset_FSM}
};



///========interfaz=================
STATE *FSM_GetInitState(user_data* data)
{
	init_reader();
	initEncoder();
	init_display();
	initDoor();
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,5);//IMPRIMO BOCA
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
	data->waitingCard = true;
	displayMode_t mode = CARD;
	updateDisplay(mode,data->ID,0);//IMPRIMO CARD
}

///PREPARO PARA ESPERAR UN ID
void set_waitID(user_data* data)
{
	displayMode_t mode = ID;
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
		data->ID[i] = 'n';
	}
	displayMode_t mode=ID;
	updateDisplay(mode,data->ID,data->curr_number);
}

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
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 'n';
	}
	displayMode_t mode=PIN;
	updateDisplay(mode,data->PIN,data->curr_number);
}

///
void increase_num(user_data* data)
{
	if (data->isID)
	{
		if(data->ID[data->curr_number]== 'n')
			data->ID[data->curr_number] = '0';
		else if (data->ID[data->curr_number] == '9')
			data->ID[data->curr_number] = '0';
		else data->ID[data->curr_number]++;
		displayMode_t mode=ID;
		updateDisplay(mode,data->ID,data->curr_number);
	}
	else
	{
		if (data->PIN[data->curr_number] == 'n')
			data->PIN[data->curr_number] = '0';
		else if (data->PIN[data->curr_number] == '9')
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
		if (data->curr_number < PIN_LENGTH-1)
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
	//leds(2);
	openDoor5sec();
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
	data->waitingCard = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 'n';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 'n';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,5);//IMPRIMO BOCA
}

///
void show_ban(user_data* data)
{
	displayMode_t mode=BAN;
	updateDisplay(mode,data->PIN,data->curr_number);
}

///
void banID(user_data* data)
{
	bool banned;
	banned = ban(&data->database,data->ID,ID_LENGTH,PIN_LENGTH);
	data->isID = true;
	data->curr_number = 0;
	data->validData = false;
	data->validation = false;
	data->waitingCard = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 'n';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 'n';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,5);//IMPRIMO BOCA
}

///
void set_IDban(user_data* data)
{
	data->curr_number=0;
	data->isID=true;
	for(int i=0;i<ID_LENGTH;i++)
	{
		data->ID[i]='n';
	}
	displayMode_t mode = ID;
	updateDisplay(mode,data->ID,data->curr_number);
}

void set_PINadd(user_data* data)
{
	data->validData = false;
	data->validation = false;
	data->curr_number = 0;
	data->isID=false;
	for(int i=0;i<PIN_LENGTH;i++)
		{
			data->PIN[i]='n';
		}
	displayMode_t mode=PIN;
	updateDisplay(mode,data->PIN,data->curr_number);
}

///
void show_add(user_data* data)
{
	displayMode_t mode=ADD;
	updateDisplay(mode,data->PIN,data->curr_number);
}

///
void add_ID(user_data* data)
{
	bool added;
	added = add_to_whitelist(&data->database,data->ID, ID_LENGTH,data->PIN, PIN_LENGTH);
	data->isID = true;
	data->curr_number = 0;
	data->validData = false;
	data->validation = false;
	data->waitingCard = false;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 'n';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 'n';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,5);//IMPRIMO BOCA
}

///
void check_addID(user_data* data)
{
	data->validation = true;
	data->validData =can_add_ID(&data->database,data->ID,ID_LENGTH);

}

///
void check_addPIN(user_data* data)
{
	data->validation =true;
	data->validData=can_add_PIN(&data->database,data->PIN, PIN_LENGTH);
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
	data->waitingCard = false;
	data->curr_tries = 0;
	data->curr_number = 0;
	for (int i = 0; i < ID_LENGTH; i++)
	{
		data->ID[i] = 'n';
	}
	for (int i = 0; i < PIN_LENGTH; i++)
	{
		data->PIN[i] = 'n';
	}
	displayMode_t mode=BOCA;
	updateDisplay(mode,data->ID,0);//IMPRIMO BOCA
}
