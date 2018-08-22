/**
@file estado.h
\brief Ficheiro header contendo os dados relativos ao módulo @c ESTADO.
*/

#ifndef ___ESTADO_H___
#define ___ESTADO_H___

#include "stack.h"

// ------------------------------------------------------------------------------

/**
\brief O tamanho máximo da grelha
*/
#define MAX_GRID   	20

/**
\brief O tamanho máximo do nome de utilizador.
*/
#define MAX_USER	100

/**
\brief O tamanho máximo do buffer
*/
#define MAX_BUFFER 	10240

// ------------------------------------------------------------------------------

/**
\brief Valores para facilitar leitura das Peças do tabuleiro 
*/
typedef enum {
	BLOQUEADA,
	FIXO_X,
	FIXO_O,
	VAZIA,
	SOL_X,
	SOL_O
} VALOR;

/**
\brief Valores para facilitar leitura do menu
*/
typedef enum {
	DRAW_TAB,              
	PLAY_TAB,              
    SELECT_MENU,            
    VICTORY,                
    SELECT_MAP,             
    CONFIRM_MAP,            
    INVALID_MAP,            
    INITIAL_MENU,           
    SELECT_DIFFICULTY,      
    RANDOM_MENU,            
    H10,
    H11,
    H12,
    H13,
    H14,
    INVALID_RANDOM,
    LEADERBOARD
} MENU_INDEX;

/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado * ESTADO;

// ------------------------------------------------------------------------------

/* Funções elementares */
ESTADO makeState(ESTADO e);
void destroyState (ESTADO e);
ESTADO inicializar(char * user, int ln, int col);
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
void setE_wins (ESTADO e, int wins);

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
int getE_wins (ESTADO e);

#endif