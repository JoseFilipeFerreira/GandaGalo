/**
@file decide.h
\brief Módulo encarregue da interatividade com peças.
*/

#ifndef DECIDE_H
#define DECIDE_H

#include "estado.h"

/**
\brief Estrutura indicativa do modo de jogo.
*/
typedef enum {DRAW, PLAY} MODE;

/**
\brief Macro que indica de que foram devem progredir as peças para o modo de jogo play.

@param num O valor apartir do qual se pretende avançar.
@param n O numéro de vezes que se irá avançar.

@returns o valor resultante.
*/
#define playadvance(num,n) (((num + n) > (SOL_O)) ? (VAZIA):((num) + n) )

int formove (ESTADO state, int i, int j);

char advance (char num, int n, char mode);

void playPos(ESTADO e, int i, int j);

#endif
