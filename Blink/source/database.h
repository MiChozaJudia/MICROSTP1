#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdbool.h>

#define ALLOWED 5
#define ID_LENGTH 8
#define PIN_LENGTH 5


typedef struct database
{
	char allowed_IDS[ALLOWED][ID_LENGTH];
	char allowed_PINS[ALLOWED][PIN_LENGTH];
}data_base;

//INICIALIZO LA BASE DE DATOS
void init_database(data_base * database);

//DEVUELVE true SI EL ID INGRESADO ESTA PERMITIDO
bool is_ID_allowed(data_base * database,char * ID, int ID_len);

//DEVUELVE true SI EL ID QUE SE DESEA INGRESAR ES VALIDO (8 CARACTÉRES NÚMERICOS PARA EL CASO)
bool can_add_ID(data_base * database,char * ID, int ID_len);

//DEVUELVE true SI EL PIN QUE SE DESEA INGRESAR ES VALIDO (AL MENÓS LOS PRIMEROS 4 CARACTERES SON NÚMEROS)
bool can_add_PIN(data_base * database,char * PIN, int PIN_len);

//DEVUELVE  true SI EL PIN DESEADO CORRESPONDE AL ID INGRESADO
bool is_PIN_correct(data_base * database,char * ID, int ID_len, char * pin, int PIN_len);

//EN CASO DE NO ESTAR PRESENTE EN LA BASE DE DATOS AGREGA EL ID Y PIN DESEADOS, DEVOLVIENDO true
bool add_to_whitelist(data_base * database,char * ID, int ID_len, char * pin, int PIN_len);

//REMUEVE AL ID Y SU PIN ASOCIADO DE LA BASE DE DATOS
bool ban(data_base * database,char * ID, int ID_len, int PIN_len);

//
int is_banned(char * ID, int ID_len);

#endif /* DATABASE_H */
#pragma once
