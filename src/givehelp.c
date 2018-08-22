/**
@file givehelp.c
\brief Módulo para fornecimento de ajudas.
*/

#include "estado.h"
#include "decide.h"
#include "givehelp.h"
#include "validate.h"
#include "solver.h"
#include <stdio.h>
#include <string.h>

// ------------------------------------------------------------------------------

/* Metódos públicos. */
int possiblepath(ESTADO state, int i, int j);
void givehelp(ESTADO e);

/* Metódos privados. */
static int putnatural(ESTADO src, ESTADO comp);
static int searchcomp(ESTADO src, ESTADO comp, int indc);

// ------------------------------------------------------------------------------

/**
\brief verifica se uma peca é SOL ou VAZIA

@param num valor

@returns 1 se for SOL ou VAZIA, 0 caso contrário
*/
#define is_solto(num) ((num == SOL_X) || (num == SOL_O) || (num == VAZIA))

// ------------------------------------------------------------------------------

/**
\brief Indica o número de casas válidas que uma posição consegue percorrer.

Verifica a grelha de um estado, e consoante as peças à volta de uma posição, percorre as possibilidades
de peças até alcançar @b SOL_O indicando quantas destas peças intermédias são válidas.

@param state Estado que se pretende alterar.
@param i Linha que se pretende verificar.
@param j Coluna que se pretende verificar.

@returns O número de peças que são válidas.

@see valtab
*/
int possiblepath(ESTADO state, int i, int j)
{
	int n = 0;
	char initElem, oldElem, newElem;
	initElem = getE_elem(state,i,j);

	do{
		oldElem = getE_elem(state,i,j);
		newElem = oldElem + 1;

		setE_elem(state, i, j, newElem);

		if (valtab(state, i, j))
			n++;
	} while (newElem < SOL_O);

	setE_elem(state, i, j, initElem);
	return n;
}


/**
\brief Função que dá ajudas naturais
Se existirem duas pecas em linha ela coloca uma peça válida numa das extremidades

@param src estado onde se encontra o mapa a procurar
@param comp local onde são armazenadas as alterações

@returns 1 se conseguir encontrar pecas nessa condição, 0 se não encontrar

@see possiblepath
@see playadvance
@see formove
@see push
@see getE_stack
*/
static int putnatural(ESTADO src, ESTADO comp)
{
	int found = 0, i, j, l, c;
	char oldElem, newElem;
	l = getE_lins(src); c = getE_cols(src);

	for (i = 0; i < l && !found; i++)
	{
		for (j = 0; j < c && !found; j++)
		{
			oldElem = getE_elem(src,i,j);
			if (oldElem == VAZIA && possiblepath(src, i, j) == 1)
			{
				newElem = playadvance(oldElem, formove(src,i,j));
				setE_elem(src, i, j, newElem);

				if (newElem == getE_elem(comp, i, j))
				{
					push(getE_stack(src, 0), i, j, oldElem);
					found = 1;
				}
				else
					setE_elem(src, i, j, oldElem);
			}
		}
	}

	return found;
}

/**
\brief Faz um procura complementária entre dois estados diferentes.

A função percorre ambos os estados, sendo que @p comp deve corresponder à solução do estado 
@p src. A cada posição que corresponda a uma peça do tipo @b SOLTO ou @b VAZIA verifica que está 
é igual à peça que se encontra na mesma posição na solução, caso sejam iguais as peças nenhuma operação é efetuada.
Caso contrário, o elemento em questão é alterada para a peça correspondente na solução. O que gera três casos:

- A nova peça é válida, então o elemento é alterado, e colocado na @b STACK .
- A nova peça é inválida e @p indc é @b 0 , então a peça será alterada para o elemento antigo que lá se encontrava.
- A nova peça é inválida e @p indc é diferente de @b 0 , então é utilizada recursividade de forma a alterar um número arbitrário de peças até que o tabuleiro se torne válido.
.

@param src O estado que irá ser alterado.
@param comp O estado que servirá de base de alteração.
@param indc 0 se não se pretender utilizar recursividade, diferente de @b 0 caso contrário.

@returns 0 se encontrou uma peça para alterar, 1 caso contrário.

@see is_solto
@see push
@see getE_stack
*/
static int searchcomp(ESTADO src, ESTADO comp, int indc)
{
	int found = 0, i, j, l, c;
	char oldElem, newElem;
	l = getE_lins(src); c = getE_cols(src);

	for (i = 0; i < l && !found; i++)
	{
		for (j = 0; j < c && !found; j++)
		{
			oldElem = getE_elem(src,i,j);
			newElem = getE_elem(comp,i,j);
			if (is_solto(oldElem) && oldElem != newElem)
			{				 
				setE_elem(src, i, j, newElem);

				if (valtab(src, i, j))
				{
					push(getE_stack(src, 0), i, j, oldElem);
					found = 1;
				}
				else{
					if (!indc)
						setE_elem(src, i, j, oldElem);
					else
						searchcomp(src,comp,indc);
				}
			}
		}
	}

	return found;
}

/**
\brief Função que altera o Estado e dá ajudas.

A função inicialmente verifica se o número de ajudas disponivél é maior que 0, se o for, então é criada uma solução de @p state ,
sendo então utilizada a função @c putnatural , de forma a encontrar peças sem verificar a solução. Se @c putnatural não encontrar
nenhuma ajuda então é utilizada a função @c searchcomp com @p indc a @b 0 de forma a que não utilize recursividade. Se não encontrar 
nenhum elemento a alterar desta forma então corre a função @c searchcomp com @p indc a @b 1 , então será utilizada recursividade e mais de uma peça será alterada.

@param e Estado a alterar.

@see getE_help
@see solve
@see putnatural
@see searchcomp
*/
void givehelp(ESTADO e)
{
	int n_helps;
	long trash;
	n_helps = getE_help(e);

	if (n_helps > 0){
		ESTADO comp = makeState(e);
		solve(comp, &trash);

		if (!putnatural(e, comp))
			if(!searchcomp(e,comp,0))
				searchcomp(e,comp,1);

		destroyState(comp);
		setE_help(e,n_helps - 1);
	}
}
