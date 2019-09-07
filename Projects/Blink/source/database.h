#pragma once


typedef struct database
{
}data_base;

void init_database(void);

int is_ID_allowed(char * ID, int ID_len);

int is_PIN_correct(char * ID, int ID_len, char * pin, int pin_len);

void add_to_whitelist(char * ID, int ID_len,char * pin);

void ban(char * ID, int ID_len);

int is_banned(char * ID, int ID_len);