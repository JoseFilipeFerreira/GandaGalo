/**
*@file state.c
\brief Módulo para alterar estado.
*/

#include "estado.h"
#include "state.h"
#include "decide.h"
#include "stack.h"
#include "filemanager.h"
#include "solver.h"
#include "frontend.h"
#include "validate.h"
#include <string.h>

// ------------------------------------------------------------------------------

/* Métodos públicos */
void clearstate(ESTADO e);
void clearcanvas(ESTADO e);
void makefixo(ESTADO e);
void increase(ESTADO e, int ox, int oy);
void safedraw(ESTADO e);
int victory(ESTADO e);
int saveAnc(ESTADO e);
void pop_Anc(ESTADO e);
int validTab (ESTADO e);

/* Metódos privados */
static char cleansolto(char val);
static char fixatepec(char val);

// ------------------------------------------------------------------------------

/**
\brief Macro que verifica se o 'num' se encontra nos limites indicados.

@param num Valor a comparar.

@returns devolve 1 se o valor se encontrar entre 0 e MAX_GRID, devolve 0 caso contrário.
*/
#define limit(num) ((num > 0) && (num < MAX_GRID))

/**
\brief Macro que verifica se 'num' e 'vec' se encontram no caso especifico indicado.

@param num Valor a comparar.
@param vec Valor a comparar.

@returns devolve 1 se os valores se encontrarem no estado indicado, devolve 0 caso contrário.
*/
#define extracase(num,vec) ( (num == MAX_GRID && vec < 0))

// ------------------------------------------------------------------------------

/**
\brief Função que recebe uma peça e verifica se ela é solta.

@param val Peça a verificar.

@returns Se val for SOL devolve VAZIA, caso contrário devolve val.
 */
static char cleansolto(char val)
{
	char rval = val;
	if (val == SOL_X || val == SOL_O)
		rval = VAZIA;
	return rval;
}
/**
\brief Função que converte para vazia todas as peças não fixas.
@param e Estado a limpar.
*/
void clearstate(ESTADO e)
{
	setE_base(e, cleansolto);
}

/**
\brief Função que converte todas as peças para vazia.

@param e Estado a limpar.

@see setE_base
*/
void clearcanvas(ESTADO e) { setE_base(e, NULL); }

/**
\brief Função transforma uma peça na sua contraparte @b FIXA .

@param val Valor inicial.

@returns Contraparte de val.
*/
static char fixatepec(char val)
{
	if (val == SOL_X || val == SOL_O)
		val -= SOL_O - FIXO_O;
	return val;
}

/**
\brief Função que converte todas as peças soltas para fixas.

@param e Apontador para o estado a converter.

@see fixatepec
@see setE_base
*/
void makefixo(ESTADO e) {
	setE_base(e, fixatepec);
}

/**
\brief Função que incrementa o tamanho de um tabuleiro.

@param e Apontador para o estado onde se encontra o tabuleiro.
@param ox Valor a incrementar no eixo ox.
@param oy Valor a incrementar no eixo oy.

@see extracase
@see limit
@see setE_transverse
*/
void increase(ESTADO e, int ox, int oy)
{
	int l, c;
	l = getE_lins(e);
	c = getE_cols(e);

	if (!(l == 1 && oy < 0))
		setE_lins(e, l + ((limit(l) || extracase(l,oy)) ? oy : 0) );
	if (!(c == 1 && ox < 0))
		setE_cols(e, c + ((limit(c) || extracase(c,ox)) ? ox : 0) );

	if (ox > 0)
		setE_transverse(e,0,c,NULL);

	if (oy > 0)
		setE_transverse(e,l,0,NULL);
}

/**
\brief Função que converte todas as peças para fixo e muda para o menu de Jogar.

Esta conversão é só efetuada se o tabuleiro em questão possuir soluções.

@param e apontador para o estado a modificar.

@see n_solutions
@see makefixo
@see setE_menu
*/
void safedraw(ESTADO e)
{
	if (n_solutions(e)){
		makefixo(e);
		setE_menu(e,PLAY_TAB);
	}
}

/**
\brief Função que transfere um elemento entre as stacks do estado.

A função efetua um @c pop da stack obtida por @c getE_stack segundo @p c , e de seguida efetua um push 
que coloca o elemento retirado desta primeira na contra-parte. Por exemplo se @p c for 0 , então é efetuado @c pop 
em @c passado e @c push em futuro.

@param e Estado onde se encontra a stack a alterar.
@param c indicador da primeira stack a alterar stack a alterar.

@see pop
@see push
@see getE_stack
@see estado::passado
@see estado::futuro
*/
void pipestack(ESTADO e, int c)
{
	int i, j;
	char val, flag;
	val = pop(getE_stack(e, c), &i, &j, &flag);
	if (val != -1)
	{
		push(getE_stack(e, !c), i, j, getE_elem(e, i, j));
		setE_elem(e, i, j, val);
	}
}

/**
\brief Verifica se a peça dada não é vazia.

@param e Estado onde se encontra o tabuleiro.
@param i Linha da Peca
@param j Coluna da Peca

@returns 0 se for VAZIA, 1 caso contrário.
*/
int is_not_solto (ESTADO e, int i, int j)
{
	return (getE_elem(e,i,j) != VAZIA);
}

/**
\brief Função que verifica se o tabuleiro não tem peças vazias.

Visto que o Jogo não permite o utilizador colocar três peças iguais em linha, para verificar se um mapa está completo, basta verificar se nãoexistem espaços em branco.

@param e apontador para o estado a verificar.

@returns 1 se todas peças forem diferentes de vazia, caso contrário devolve 0.

@see is_not_solto
@see getE_verf
*/
int victory(ESTADO e)
{
	int r = getE_verf(e,is_not_solto);
	if (r){
		setE_menu(e,VICTORY);
		setE_wins(e,getE_wins(e)+1);
	}
	return r;
}

/**
\brief Verifica se um tabuleiro é válido.

@param e Estado a verificar.

@returns 1 se o tabuleiro for válido, 0 caso contrário.

@see valtab
@see getE_verf
*/
int validTab (ESTADO e)
{
	return (getE_verf(e,valtab));
}

/**
\brief Função que guarda uma âncora.

@param e Estado a modificar.

@returns 1 se não ocorrerem erros, caso contrário devolve 0.

@see makeAnc
@see getE_stack
@see estado::passado
*/
int saveAnc(ESTADO e) {
	return (makeAnc(getE_stack(e, 0)));
}

/**
\brief Função que carrega a última âncora

Verifica se há alguma posição na @c STACK que corresponda a um valor com âncora, e se 
existir faz pops sucessivos até alcançar este ponto. Colocando todos os elementos retirados da @c STACK @c passado em 
@c futuro .

@param e apontador para o estado a verificar

@see canGetAnc
@see peek
@see pop
@see push
@see estado::passado
@see estado::futuro
*/
void pop_Anc(ESTADO e)
{
	char flag, r;
	int i, j;
	STACK past = getE_stack(e,0);
	while (canGetAnc(past) && !peek(past) )
	{
		r = pop(past, &i, &j, &flag);
		push(getE_stack(e, 1), i, j, getE_elem(e, i, j));
		setE_elem(e, i, j, r);
	}
}
