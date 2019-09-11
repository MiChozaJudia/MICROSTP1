#ifndef BUFFER_H_
#define BUFFER_H_

#define BUFFER_SIZE 20

typedef struct {
    int buffer[BUFFER_SIZE];
    int len;
    int head;
    int tail;

} circ_bbuf_t;
/*
 INICIALIZO EL BUFFER
*/
void init_buffer(circ_bbuf_t * buff,int len);

/*
PUSHEO ELEMENTO AL BUFFER, HAY QUE CHEQUEAR DE ANTEMANO QUE EL BUFFER NO ESTÉ LLENO
*/
void push_buffer(circ_bbuf_t * buff, int data);

/*
POPPEO ELEMENTO DEL BUFFER, HAY QUE CHEQUEAR DE ANTEMANO QUE EL BUFFER NO ESTÉ VACÍO
*/
int pop_buffer(circ_bbuf_t * buff);

/*
DEVUELVE 1 SI EL BUFFER ESTÁ LLENO, 0 DE LO CONTRARIO
*/
int buffer_is_full(circ_bbuf_t * buff);

/*
DEVUELVE 1 SI EL BUFFER ESTÁ VACÍO, 0 DE LO CONTRARIO
*/
int buffer_is_empty(circ_bbuf_t * buff);

#endif /* FSMTABLE_H_ */
#pragma once
