#ifndef READER_H_
#define READER_H_

#include "gpio.h"



//PUBLIC FUNCTION

bool init_reader(uint8_t port,uint8_t enable_pin, uint8_t clock_pin, uint8_t data_pin); //return 0 if error
bool get_ID(char* ID,uint8_t id_len); //return 0 if no ID yet


#endif /* READER_H_ */
