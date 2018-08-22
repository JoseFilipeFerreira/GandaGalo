/**
@file leaderboard.h
\brief Módulo de obtenção da leaderboard.
*/

#ifndef USER_H
#define USER_H

// ------------------------------------------------------------------------------

/**
\brief Tamanho máximo de uma string que utilizador.
*/
#define MAX_S 	50

// ------------------------------------------------------------------------------

/**
\brief Estrutura onde é guardada a informação usada pela leaderboard.
*/
typedef struct info * INFO;

// ------------------------------------------------------------------------------

char * getInfo_user (INFO v, int i);

int getInfo_wins (INFO v, int i);

int get_info (INFO * v, int N);

void push_info (char * user, int wins);

#endif