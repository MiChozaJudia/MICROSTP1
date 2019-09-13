/*
 * fsmtable.h
 *
 *  Created on: 28/07/2014
 *      Author: Daniel Jacoby
 */


#ifndef FSMTABLE_H_
#define FSMTABLE_H_

#include "reader.h"
#include "encoder.h"
#include "displayWrapper.h"
#include "door.h"


//Devuelve el estado inicial de la FSM
STATE *FSM_GetInitState(user_data* data);

#endif /* FSMTABLE_H_ */
#pragma once
