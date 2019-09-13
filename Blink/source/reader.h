#ifndef READER_H_
#define READER_H_

#include <stdint.h>
#include <stdbool.h>


//PUBLIC FUNCTION


/**
 * @brief Configures inputs and interrupts that correspond to the reader
 * @no params
 */
bool init_reader();

/**
 * @brief return 0 if no valid ID yet
 * @param ID pointer to the array where the ID want to be saved
 * @param id_len length of the array where the ID want to be saved
 */
bool get_ID(char* ID,uint8_t id_len);


#endif /* READER_H_ */
