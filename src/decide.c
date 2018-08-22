/**
@file decide.c
\brief Módulo encarregue da interatividade com peças.
*/

#include <stdlib.h>
#include <stdio.h>
#include "estado.h"
#include "validate.h"
#include "decide.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */
int formove (ESTADO state, int i, int j);
char advance (char num, int n, char mode);
void playPos(ESTADO e, int i, int j);

/* Metódos privados */
static char drawadvance (char num, int n);

// ------------------------------------------------------------------------------

/**
\brief
	Função utilizada para o avanço sucessivo de peças para o modo de desenho.

Avança cada valor normalmente de modo a que seja possível jogar o valor BLOQUEADO.

@param num O valor apartir do qual se pretende avançar.
@param n O numéro de vezes que se irá avançar.

@returns O valor resultante.
*/
static char drawadvance (char num, int n)
{
	char rval;
	if (num == VAZIA && n == 0)
		rval = BLOQUEADA;
	else if (num == BLOQUEADA)
		rval = (VAZIA + n);
	else if (num + n > SOL_O)
		rval = BLOQUEADA;
	else
		rval = num + n;
	return rval;
}

/**
\brief Função que indica quantas casas devem ser avançadas de modo a manter a validade do jogo.

@param state Estado do qual se pretend aferir a validade.
@param i Linha que se pretende avaliar.
@param j Coluna que se pretende avaliar.

@returns Quantas casas deve avançar.
*/
int formove (ESTADO state, int i, int j)
{
	int r = 0;
	char initElem = getE_elem(state,i,j);

	switch (initElem){
		case VAZIA: setE_elem(state,i,j,SOL_X);
					if (valtab(state, i, j)) {r = 1; break;}
					setE_elem(state,i,j,SOL_O);
					if (valtab(state, i, j)) r = 2;
					break;
		case SOL_X: setE_elem(state,i,j,SOL_O);
					if (valtab(state, i, j)) {r = 1; break;}
					r = 2;
					break;
		case SOL_O: r = 1; break;
	}

	setE_elem(state,i,j,initElem);
	return r;
}

/**
\brief Função decide internamente que tipo de função aplicar ao avanço do jogo consoante o modo.

@param num O valor apartir do qual se pretende avançar.
@param n O numéro de vezes que se irá avançar.
@param mode O modo do jogo.

@returns O valor resultante.
*/
char advance (char num, int n, char mode)
{
	char rval;
	if (mode == DRAW)
		rval = drawadvance(num,n);
	else
		rval = playadvance(num,n);
	return rval;
}

/**
\brief Função que faz uma jogada no estado consoante uma de jogo e a posição.

@param e Apontador para o estado que irá ser alterado.
@param i Linha onde se pretende fazer a jogada.
@param j Colunas onde se pretende fazer a jogada.
*/
void playPos(ESTADO e, int i, int j){
    int n = formove(e,i,j);
    char val = getE_elem(e,i,j);
	push(getE_stack(e,0),i,j,val);
	setE_elem(e,i,j,advance(val,n,getE_menu(e)));
}

