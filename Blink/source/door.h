

#ifndef DOOR_H_
#define DOOR_H_

#include <stdint.h>
#include <stdbool.h>

void initDoor();
void openDoor5sec(void);
void openDoor(int time); //EN MS
bool isDoorOpen(void);
void closeDoor(void);
bool isDoorJustClosed(void);

#endif /* DOOR_H_ */
