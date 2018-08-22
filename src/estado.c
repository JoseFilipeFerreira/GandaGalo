/**
@file estado.c
\brief Ficheiro contendo o código relativo ao módulo @c ESTADO.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estado.h"
#include "cgi.h"
#include "frontend.h"
#include "filemanager.h"
#include "state.h"
#include "solver.h"
#include "leaderboard.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */

/* Funções primárias */
ESTADO makeState(ESTADO e);
void destroyState (ESTADO e);
ESTADO inicializar (char * user, int ln, int col);
long n_solutions (ESTADO e);

/* Setters */
void setE_state (ESTADO dest, ESTADO sourc);
void setE_user (ESTADO e, char * user);
void setE_lins (ESTADO e, int lins);
void setE_cols (ESTADO e, int cols);
void setE_menu (ESTADO e, int menu);
void setE_elem (ESTADO e, int i, int j, char val);
void setE_elemT (ESTADO e, int i, int j, char (*map) (char));
void setE_stack (ESTADO e, STACK s, int c);
void setE_flag (ESTADO e, int flag);
void setE_transverse (ESTADO e, int li, int ci, char (*map) (char));
void setE_base (ESTADO e, char (*map) (char));
void setE_help (ESTADO e, int help);
void setE_helpB (ESTADO e);

/* Getters */
char * getE_user (ESTADO e);
int getE_cols (ESTADO e);
int getE_lins (ESTADO e);
int getE_menu (ESTADO e);
STACK getE_stack (ESTADO e, int c);
int getE_flag (ESTADO e);
char getE_elem(ESTADO e, int i, int j);
int getE_help (ESTADO e);
int getE_verf (ESTADO e, int (*inclusivecase) (ESTADO,int,int));

// ------------------------------------------------------------------------------

/**
\brief Estrutura que armazena o estado do jogo.
*/
typedef struct estado {
	char user[MAX_USER];             /**< Nome de utilizador */
	int num_lins;                    /**< Número de linhas */
	int num_cols;                    /**< Número de colunas */
	int flag;                        /**< Flag */
	int menu;                        /**< Menu atual */
	int help;                        /**< Número restante de hints */
	int wins;						 /**< Número de vitórias*/
	char grelha[MAX_GRID][MAX_GRID]; 				/**< Grelha do jogo */
	STACK passado;                   /**< Stack para undo */
	STACK futuro;                    /**< Stack para redo */
} * ESTADO;

// ------------------------------------------------------------------------------

/**
\brief Função que cria um estado.

@param e Estado a copiar.

@returns Caso 'e' não seja NULL devolve um pointer para uma cópia de 'e', caso contrário devolve um pointer para um estado indefinido

@see setE_state
*/
ESTADO makeState(ESTADO e)
{	
	ESTADO new = malloc(sizeof(struct estado));
	new->passado = NULL;
	new->futuro = NULL;
	if (e != NULL)
		setE_state(new,e);
	return new;
}

/**
\brief Função que destroi o estado.

@param e Estado a destruir.
*/
void destroyState (ESTADO e)
{
	free(e);
}

/**
\brief Função que inicializa o estado.

@param user Uma string com o nome de utilizador do novo estado.
@param ln Número de linhas no mapa.
@param col Número de colunas no mapa.

@returns Um novo estado.

@see setE_lins
@see setE_cols
@see setE_base
@see setE_flag
@see setE_user
@see setE_wins
@see setE_menu
*/
ESTADO inicializar(char * user, int ln, int col) {
	ESTADO e = makeState(NULL);
	setE_lins(e,ln);
	setE_cols(e,col);
	setE_base(e,NULL);
	setE_flag(e,0);
	setE_user(e,user);
	setE_wins(e,0);
	setE_menu(e,SELECT_MENU);
	return e;
}

/**
\brief Função que indica o número de soluções de um tabuleiro.

@param e Estado onde se encontra o tabuleiro a calcular.

@returns Número de soluções.

@see makefixo
@see solve
*/
long n_solutions (ESTADO e)
{
	long m;
	ESTADO tmp = makeState(e);
	makefixo(tmp);
	solve(tmp,&m);
	destroyState(tmp);
	return m;
}

/**
\brief Função que copia um estado para outro.

@param dest Para onde é copiado.
@param sourc De onde é copidado.
*/
void setE_state (ESTADO dest, ESTADO sourc)
{
	int i, j;
	*dest = *sourc;
	for (i = 0 ; i < sourc->num_lins; i++)
		for (j = 0; j < sourc->num_cols; j++)
			dest->grelha[i][j] = sourc->grelha[i][j];
}

/**
\brief Função que altera o nome de utilizador num estado.

@param e Estado a alterar.
@param user Utilizador a colocar.

@see estado::user
*/
void setE_user (ESTADO e, char * user)
{
	strcpy(e->user,user);
}

/**
\brief Função que altera o número de linhas.

@param e Estado a alterar.
@param lins Novo número de linhas.

@see estado::num_lins
*/
void setE_lins (ESTADO e, int lins)
{
	e->num_lins = lins;
}

/**
\brief Função que altera o número de colunas.

@param e Estado a alterar.
@param cols Novo número de colunas.

@see estado::num_cols
*/
void setE_cols (ESTADO e, int cols)
{
	e->num_cols = cols;
}

/**
\brief Função que altera o menu.

@param e Estado a alterar.
@param menu Novo menu.

@see estado::menu
*/
void setE_menu (ESTADO e, int menu)
{
	e->menu = menu;
}

/**
\brief Função que altera um elemento do tabuleiro.

@param e Estado a alterar.
@param i Linha a alterar.
@param j Coluna a alterar.
@param val A colocar.
*/
void setE_elem (ESTADO e, int i, int j, char val)
{
	e->grelha[i][j] = val;
}

/**
\brief Altera um elemento consoante uma função mapping.

@param e @c ESTADO a alterar.
@param i Linha a alterar.
@param j Coluna a alterar.
@param map Função de mapping.

@see setE_elem
*/
void setE_elemT (ESTADO e, int i, int j, char (*map) (char))
{
	char val = e->grelha[i][j];
	if (map == NULL)
		setE_elem(e,i,j,VAZIA);
	else 
		setE_elem(e,i,j,(*map)(val));
}

/**
\brief Função que altera as stack no estado.

@param e @c ESTADO a alterar.
@param s @c STACK a colocar.
@param c Indicador da @c STACK a alterar. Se for 0 a @c STACK alterada será a passado, caso contrário será a futuro.

@see STACK
@see estado::passado
@see estado::futuro
*/
void setE_stack (ESTADO e, STACK s, int c)
{
	if (c == 0)
		e->passado = s;
	else
		e->futuro = s;
}

/**
Função que altera a flag.

@param e @c ESTADO a alterar.
@param flag Flag a alterar.

@see estado::flag
*/
void setE_flag (ESTADO e, int flag)
{
	e->flag = flag;
}

/**
\brief Aplica uma função de mapping a vários elementos da grelha.

Partindo da linha @p li e da coluna @p ci, a função percorre todos os elementos até alcançar 
o limite de linhas e colunas máximo, aplicando a todos os elementos uma função de mapping.

@param e @c ESTADO que irá ser alterado.
@param li Linha inicial de onde se pretende começar a percorrer
@param ci Coluna inicial de onde se pretende começar a percorrer.
@param map Função de mapping.

@see getE_lins
@see getE_cols
@see setE_elemT
*/
void setE_transverse (ESTADO e, int li, int ci, char (*map) (char))
{
	int i, j, l, c;
	l = getE_lins(e); c = getE_cols(e);

	for (i = li; i < l; i++)
		for (j = ci; j < c; j++)
			setE_elemT(e,i,j,map);
}

/**
\brief Coloca no estado indicado um base.

A base colocada em @p e corresponde a:
	- Reajustar o número de ajudas para @c MAX_HELP.
	- Inicializar a @c STACK @c passado'.
	- Inicializar a @c STACK @c futuro'.
	- Aplicar uma função de mapping a todos os elementos da grelha.

@param e @c ESTADO onde irá ser colocada a base.
@param map Função de mapping.

@see MAX_HELP
@see STACK
@see setE_help
@see estado::passado
@see estado::futuro
@see setE_stack
@see initS()
@see setE_transverse
*/
void setE_base (ESTADO e, char (*map) (char))
{
	setE_helpB(e);
	setE_stack(e,initS(),0);
	setE_stack(e,initS(),1);
	setE_transverse(e,0,0,map);
}

/**
\brief Função que altera o número de ajudas.

@param e @c ESTADO a alterar.
@param help Novo número de ajudas.

@see estado::help
*/
void setE_help (ESTADO e, int help)
{
	e->help = help;
}

/**
\brief Função que coloca um número base de ajudas em @p e .

@param e @c ESTADO cujas ajudas iram ser alteradas.
*/
void setE_helpB (ESTADO e)
{
	e->help = (e->num_lins + e->num_cols)/2;
}

/**
\brief Função que coloca o número de vitórias no @c ESTADO e no ficheiro para tal.

@param e @c ESTADO que irá ser alterado.
@param wins Número de vitórias novo.

@see push_info
*/
void setE_wins (ESTADO e, int wins)
{
	push_info(e->user,wins);
	e->wins = wins;
}

/**
\brief Função que devolve o nome de utilizador.

@param e @c ESTADO a procurar.

@returns Nome de utilizador.

@see estado::user
*/
char * getE_user (ESTADO e)
{
	return (e->user);
}

/**
\brief Função que devolve o menu atual.

@param e @c ESTADO a procurar.

@returns Menu atual.

@see estado::menu
*/
int getE_menu (ESTADO e)
{
	return (e->menu);
}

/**
\brief Função que devolve o número de linhas na grelha.

@param e Estado a procurar.

@returns Número de linhas.

@see estado::num_lins
*/	
int getE_lins (ESTADO e)
{
	return (e->num_lins);
}

/**
\brief Função que devolve o número de colunas na grelha.

@param e Estado a procurar.

@returns Número de colunas.

@see estado::num_cols
*/	
int getE_cols (ESTADO e)
{
	return (e->num_cols);
}

/**
\brief Função que devolve uma @c STACK do @c ESTADO.

@param e @c ESTADO a procurar.
@param c Indicador da @c STACK a devolver. Se for 0 a @c STACK alterada será a passado, caso contrário será a futuro.

@returns Uma @c STACK. Se @p c for 0 devolve a @c STACK do @b undo, caso contrário devolve a @c STACK do @b redo.

@see STACK
@see estado::passado
@see estado::futuro
*/	
STACK getE_stack (ESTADO e, int c)
{
	if (c == 0)
		return (e->passado);
	else
		return (e->futuro);
}

/**
\brief Função que devolve a @c flag.

@param e @c ESTADO a procurar.

@returns @c flag do @c Estado.

@see estado::flag
*/	
int getE_flag (ESTADO e)
{
	return (e->flag);
}

/**
\brief Função que devolve um elemento da grelha.

@param e Estado a procurar.
@param i Linha do elemento.
@param j Coluna do elemento.

@returns Elemento na linha 'i' e coluna 'j'.
*/	
char getE_elem (ESTADO e, int i, int j)
{
	return (e->grelha[i][j]);
}

/**
\brief Função que devolve o número de ajudas.

@param e Estado a procurar.

@returns Número de ajudas.

@see estado::help
*/	
int getE_help (ESTADO e)
{
	return (e->help);
}

/**
\brief Função que verifica todas as peças de um tabuleiro.

@param e Estado a procurar.
@param inclusivecase Apontador para a função que contem a condição para verificar se um elemento é inclusivo.

@returns 1 Se se verificar em todas, 0 se alguma não o verificar.
*/
int getE_verf (ESTADO e, int (*inclusivecase) (ESTADO,int,int))
{
	int i, j, l, c;
	l = getE_lins(e); c = getE_cols(e);

	if (inclusivecase){
		for (i = 0; i < l; i++)
			for (j = 0; j < c; j++)
				if ( ! (*inclusivecase)(e,i,j) )
					return 0;
	}
	return 1;
}

/**
\brief Função que obtem o número de vitórias do estado.

@param e @c ESTADO a procurar.

@returns Devolve o número de vitórias.
*/
int getE_wins (ESTADO e)
{
	return (e->wins);
}