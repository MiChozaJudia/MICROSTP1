

#ifndef DOOR_H_
#define DOOR_H_

#include <stdint.h>
#include <stdbool.h>

void initDoor(uint8_t pin);
void openDoor5sec(void);
void openDoor(int time); //EN MS
bool isDoorOpen(void);
void closeDoor(void);

#endif /* DOOR_H_ */
