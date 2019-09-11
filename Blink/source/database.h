#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdbool.h>

#define ALLOWED 5

typedef struct database
{
	char allowed_IDS[ALLOWED][8];
	char allowed_PINS[ALLOWED][5];
}data_base;

void init_database(data_base * database);

bool is_ID_allowed(data_base * database,char * ID, int ID_len);

bool is_PIN_correct(data_base * database,char * ID, int ID_len, char * pin, int PIN_len);

void add_to_whitelist(char * ID, int ID_len,char * pin);

void ban(char * ID, int ID_len);

int is_banned(char * ID, int ID_len);

#endif /* DATABASE_H */
#pragma once
