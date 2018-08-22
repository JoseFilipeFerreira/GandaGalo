/**
@file validate.c
\brief Módulo das funções utilizadas para verificação do tabuleiro.
*/

#include "estado.h"
#include "validate.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */
int valtab(ESTADO state, int i, int j);

/* Metódos privados */
static int validside(int i, int j, int vectorx, int vectory, ESTADO state);

// ------------------------------------------------------------------------------

/**
\brief Macro utilizada para verificar se o primeiro paramêtro é menor que o número de colunas máximo do estado.
@param j Componente das abcissas de uma posição.
@param state Estado utilizado para verificar o número de colunas máximo.
*/
#define lessCol(j, state) ((j) < (getE_cols(state)))

/**
\brief Macro utilizada para verificar se o primeiro paramêtro é menor que o número de linhas máximo do estado.

@param i Componente das abcissas de uma posição.
@param state Estado utilizado para verificar o número de linhas máximo.
*/
#define lessLin(i, state) ((i) < (getE_lins(state)))

/**
\brief Macro utilizada para ignorar os casos em que a peça em questão é @c VAZIA ou @c BLOQUEADA .

@param i Linha do elemento.
@param j Coluna do elemento.
@param state Estado do qual se pretende obter a informação.
*/
#define ignore(i, j, state) (getE_elem(state, i, j) != VAZIA && getE_elem(state, i, j) != BLOQUEADA)

/**
\brief Macro verifica se dois valores são equivalentes.
	Dois valores são equivalentes neste cenário, quando são iguais ou se representarem fixos e soltos.

@param num1 Primeiro valor do tabuleiro a ser verificado.
@param num2 Segundo valor do tabuleiro a ser verificado.

@returns Se os dois valores forem equivalente devolve 1, caso contrário devolve 0.
*/
#define equals(num1, num2) ((num1 == num2) || (num1 == num2 + 3) || (num1 == num2 - 3))

/**
\brief Macro que passa argumentos para a Macro equals segundo valores obtidos do @c ESTADO.

@param i1 Linha do primeiro elemento
@param j1 Coluna do primeiro elemento
@param i2 Linha do segundo elemento
@param j2 Coluna do segundo elemento
@param state @c ESTADO onde se encontram os elementos a comparar

@returns Se os elementos forem iguais retorna 1, caso contrário devolve 0.

@see equals
*/
#define equal(i1, j1, i2, j2, state) (equals(getE_elem(state, i1, j1), getE_elem(state, i2, j2)))
/**
\brief Macro utilizada para a verificação da validade de uma sequência de peças.
	Comando 'countval' é utilizado para ignorar peças bloqueadas ou vazias e confirmar
	se a peça seguinte à atual é válida, incluindo para fixos, ou não.

@param i1 Linha do primeiro elemento.
@param j1 Coluna do primeiro elemento.
@param i2 Linha do segundo elemento.
@param j2 Coluna do segundo elemento.
@param state @c ESTADO onde se encontram os elementos a comparar.

@see equal
@see ignore
*/
#define countval(i1, j1, i2, j2, state) (equal(i1, j1, i2, j2, state) && ignore(i1, j1, state))

/**
\brief Macro utilizada para restringir as váriaveis aos limites da grelha.

@param i Linha a restringir.
@param j Coluna a restringir.
@param state Estado do qual se pretende obter os valores de restrição.
*/
#define testall(i, j, state) ((lessLin(i, state)) && (lessCol(j, state)) && (i >= 0) && (j >= 0))

/**
\brief Macro que passa argumentos à macro testall de forma restringir dois elementos.

@param i1 Linha do primeiro elemento a restringir.
@param j1 Coluna do primeiro elemento a restringir.
@param i2 Linha do segundo elemento a restringir.
@param j2 Coluna do segundo elemento a restringir.
@param state Estado do qual se pretende obter os valores de restrição.
*/
#define contain(i1, j1, i2, j2, state) (testall(i1, j1, state) && testall(i2, j2, state))

// ------------------------------------------------------------------------------

/**
\brief Verifica se dado um vetor uma posição é válida no estado.
	A função segue a trajetória indicada pelo vetor passado, bem como no sentido aposto a este. Utilizando
	para tal as macros @c contain e @c countval para obter a contagem de elementos iguais segundo a direção
	do @b vetor passado como argumento.

@param j representa a coluna que se pretende verificar.
@param i representa a linha que se pretende verificar.
@param vectori representa a componente das linhas do vetor utilizado.
@param vectorj representa a componente das colunas do vetor utilizado.
@param state Estado do qual se pretende analisar a grelha. 

@returns Quantas peças do mesmo valor há à volta de uma certa posição, segundo a direção do vetor.

@see contain
@see countval
*/
static int validside(int i, int j, int vectori, int vectorj, ESTADO state)
{
	int tmpX, tmpY, count = 1;

	tmpX = i;
	tmpY = j;

	while (contain(i, j, i + vectori, j + vectorj, state) && countval(i, j, i + vectori, j + vectorj, state))
	{
		++count;
		j += vectorj;
		i += vectori;
	}

	i = tmpX;
	j = tmpY;

	while (contain(i, j, i - vectori, j - vectorj, state) && countval(i, j, i - vectori, j - vectorj, state))
	{
		++count;
		j -= vectorj;
		i -= vectori;
	}

	return count;
}

/**
\brief Verifica se uma posição na grelha do estado passado, é válida (Ou seja, não possui 3 em linhas em nenhuma direção).

@param i Linha que se pretende verificar.
@param j Coluna que se pretende verificar.
@param state Apontador para o estado que possui a grelha onde se pretende avaliar a posição.

@returns Validade da posição na grelha.

@see validside
*/
int valtab(ESTADO state, int i, int j)
{
	int horizontalC, verticalC;
	int bissImparC, bissParC;

	// Verifica Horizontal;
	horizontalC = validside(i, j, 1, 0, state);
	// Verifica Vertical;
	verticalC = validside(i, j, 0, 1, state);
	// Verifica bissetriz dos quadrantes impares
	bissImparC = validside(i, j, 1, -1, state);
	// Verifica bissetriz dos quadrantes pares
	bissParC = validside(i, j, 1, 1, state);

	return (horizontalC < 3 && verticalC < 3 && bissImparC < 3 && bissParC < 3);
}
