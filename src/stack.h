/**
*@file stack.h
\brief Módulo para operar sobre STACK.
*/
#ifndef STACK_H
#define STACK_H

// ------------------------------------------------------------------------------

/**
\brief Declaração da Estrutura principal.
*/
typedef struct stack* STACK;

// ------------------------------------------------------------------------------

STACK initS ();

void destroyS(STACK s);

int isEmpty (STACK  s);

void push (STACK s, int i, int j, char val);

int pop (STACK s, int * i, int * j, char * flag);

int peek (STACK s);

int canGetAnc (STACK s);

int makeAnc (STACK s);

void addToTail (STACK s, char i, char j, char val, char check);

char *completeS(STACK s);

#endif
