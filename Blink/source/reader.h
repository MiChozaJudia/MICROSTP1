#ifndef READER_H_
#define READER_H_

#include "gpio.h"


//PUBLIC FUNCTION

bool init_reader(); //return 0 if error
bool get_ID(char* ID,uint8_t id_len); //return 0 if no ID yet


#endif /* READER_H_ */
