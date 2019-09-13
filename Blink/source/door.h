

#ifndef DOOR_H_
#define DOOR_H_

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief Configures the door and the corresponding pin
 */
void initDoor();
/**
 * @brief Open the door for 5 seconds
 */
void openDoor5sec(void);
/**
 * @brief Open the door for certain time
 * @param time time the door will be open in ms
 */
void openDoor(int time);
/**
 * @brief Return true if door is open
 */
bool isDoorOpen(void);
/**
 * @brief Close the door
 */
void closeDoor(void);
/**
 * @brief Return true if the close was closed since the last reading of the flag

 */
bool isDoorJustClosed(void);

#endif /* DOOR_H_ */
