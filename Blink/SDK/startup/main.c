/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
#include "App.h"

#define STACK_SIZE 20
int main (void)
{
    hw_Init();
    hw_DisableInterrupts();
    user_data data;//Estructura de datos utilizada por la FSM
    circ_bbuf_t buffer;//Buffer circular utilizado para implementar una cola de eventos
    STATE *p2state=NULL;//Puntero utilizado para recorrer las tablas de estados de la FSM
    App_Init(&data,&buffer,&p2state);//Inicializo el programa
    hw_EnableInterrupts();

    __FOREVER__
        App_Run(&data,&buffer,&p2state);//Run loop del programa
}
