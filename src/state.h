/**
*@file state.h
\brief Módulo para alterar estado.
*/
	
#ifndef state_h
#define state_h

#include "estado.h"

void clearstate(ESTADO e);

void clearcanvas(ESTADO e);

void makefixo(ESTADO e);

void increase(ESTADO e, int ox, int oy);

void safedraw(ESTADO e);

void pipestack (ESTADO e, int c);

int victory(ESTADO e);

int saveAnc(ESTADO e);

void pop_Anc(ESTADO e);

int validTab (ESTADO e);

#endif
