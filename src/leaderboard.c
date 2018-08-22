/**
@file leaderboard.c
\brief Módulo de obtenção da leaderboard.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "leaderboard.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */
char * getInfo_user (INFO v, int i);
int getInfo_wins (INFO v, int i);
int get_info (INFO * v, int N);
void push_info (char * user, int wins);

/* Metódos privados */
static void make_info ();
static int size_info ();
static INFO list_info (int * x);
static int cmp_info (const void * a, const void * b);
static void sort_info (INFO v);

// ------------------------------------------------------------------------------

/**
\brief Diretória onde se encontra o ficheiro onde são guardados todos os utilizadores.
*/
#define DIRINFO "/usr/local/games/GandaGalo/users/users.save"

// ------------------------------------------------------------------------------

/**
\brief Estrutura onde é guardada a informação usada pela leaderboard.
*/
typedef struct info {
	int wins;			/**< Número de vitórias guardadas. */
	char user[MAX_S];	/**< Utilizador guardado. */
} * INFO;

// ------------------------------------------------------------------------------

/**
\brief Função que obtêm o utilizador associado a um bloco de informação.

@param v Array de onde irá ser retirada a informação.
@param i Posição da informação.

@returns Devolve o utilizador.
*/
char * getInfo_user (INFO v, int i){
	return (v[i].user);
}

/**
\brief Função que obtêm o número de vitórias associado a um bloco de informação.

@param v Array de onde irá ser retirada a informação.
@param i Posição da informação.

@returns Devolve o número de vitórias.
*/
int getInfo_wins (INFO v, int i){
	return (v[i].wins);
}

// ------------------------------------------------------------------------------

/**
\brief Função que verifica a existência de @c DIRINFO .
Caso o ficheiro não exista, então é criado.

@see DIRINFO
*/
static void make_info ()
{
	FILE * fp = fopen(DIRINFO,"r");
	if (fp == NULL)
		fp = fopen(DIRINFO,"w");
	fclose(fp);
}

/**
\brief Função que adiciona nova informação ao ficheiro.
Consoante o utilizador e o número de vitória coloca no ficheiro @c DIRINFO a informação passada 
como argumento. Da seguinte forma:

- Se o utilizador não existir no ficheiro então é colocado no fim.
- Se o utilizador existir no ficheiro então o seu número de vitórias é atualizado.

@param user Utilizador em registo.
@param wins Nome de vitórias em registo.

@see make_info
@see DIRINFO
*/

void push_info (char * user, int wins)
{
	char user_read[MAX_S]; int tmp ;

	make_info();
	FILE * fp = fopen(DIRINFO,"r+");

	while ((tmp = fscanf(fp,"%s",user_read) ) != EOF
		&& !strstr(user_read,user))
		;
	if (tmp == EOF)
		fprintf(fp, "%s %3d\n", user, wins);
	else {
		fprintf(fp, " %3d", wins);
	}
	fclose(fp);
}

/**
\brief Função que devolve o número de elementos no ficheiro @c DIRINFO .

@returns Devolve o número de elementos no ficheiro @c DIRINFO .

@see DIRINFO
*/
static int size_info ()
{
	int r = 0, dummy; char usr[MAX_S];
	FILE * fp = fopen(DIRINFO,"r");
	if (fp != NULL){
		while (fscanf(fp,"%s %3d",usr,&dummy) != EOF)
			r++;
	}
	return r;
	fclose(fp);
}

/**
\brief Cria um array com todos os elementos do ficheiro.

@param x Endereço onde irá ser colocado o número de elementos lidos.

@returns O array criado.

@see size_info
*/
static INFO list_info (int * x)
{
	INFO r = NULL; FILE * fp;
	int i, wins; char user[MAX_S];
	*x = size_info();
	if (*x){
		fp = fopen(DIRINFO,"r");
		r = malloc(sizeof(struct info)*(*x));
	}
	for (i = 0 ; i < *x ; i++){
		if(fscanf(fp,"%s %3d",user,&wins) != EOF){
			r[i].wins = wins;
			strcpy(r[i].user,user);
		}
	}
	return r;
}

/**
\brief Função de comparação utilizada no sorting.

@param a Primeiro valor a comparar.
@param b Segundo valor a confirmar.

@returns O valor da comparação.
*/
static int cmp_info (const void * a, const void * b)
{
	return ( (*(INFO)b).wins - (*(INFO)a).wins);
}

/**
\brief Efetua o sorting do array de @c INFO .

@param v Array que irá ser ordenado.

@see size_info
*/
static void sort_info (INFO v)
{
	int sz = size_info();
	qsort(v,sz,sizeof(struct info),cmp_info);
}

/**
\brief Função que indica um array de @c INFO contendo a leaderboard.

@param v Apontador para o endereço onde irá ser colocada a leaderboard.
@param N Número máximo de elementos colocados.

@returns Números de elementos colocados.

@see list_info
@see sort_info
@see INFO
*/
int get_info (INFO * v, int N)
{
	int sz;
	*v = list_info(&sz);
	sort_info(*v);
	if (sz > N){
		*v = (INFO)realloc(*v,N);
		sz = N;
	}
	return sz;
}