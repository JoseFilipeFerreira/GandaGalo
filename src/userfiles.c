/**
*@file userfiles.c
\brief Módulo para conversão entre ficheiros e ESTADO
*/

#include "userfiles.h"
#include "stack.h"
#include "estado.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// ------------------------------------------------------------------------------

/* Metódos públicos */
void estado2file_un(char *path, char *user, ESTADO e);
ESTADO file2estado_un(char *path, char *user, int *flag);

/* Metódos privados */
static STACK readAll(FILE *fp);

// ------------------------------------------------------------------------------

/**
\brief Escreve um estado para ficheiro.

@param path String correspondente à diretória onde se encontram os users.
@param user String correspondente ao nome do ficheiro que irá ser guardado.
@param e Apontador para o estado que irá ser guardado em ficheiro.
*/
void estado2file_un(char *path, char *user, ESTADO e)
{
	int l, c;
	l = getE_lins(e);
	c = getE_cols(e);
	char *aux = (char *)malloc(sizeof(char) * (strlen(path) + strlen(user) + strlen(".txt") + 1));
	sprintf(aux, "%s%s%s", path, user, ".txt");

	char *fut, *past;
	FILE *fp = fopen(aux, "w");
	/// fclose(fp);
	int i, j;
	chmod(aux, 0777);
	free(aux);

	fprintf(fp, "USER: %s\n", getE_user(e));
	fprintf(fp, "N_LINS: %d\n", l);
	fprintf(fp, "N_COLS: %d\n", c);
	fprintf(fp, "FLAG: %d\n", getE_flag(e));
	fprintf(fp, "MENU: %d\n", getE_menu(e));
	fprintf(fp, "HELP: %d\n", getE_help(e));
	fprintf(fp, "WINS: %d\n", getE_wins(e));
	fprintf(fp, "GRELHA:\n");
	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
			fprintf(fp, "%d ", getE_elem(e, i, j));
		fprintf(fp, "\n");
	}

	past = completeS(getE_stack(e, 0));
	fut = completeS(getE_stack(e, 1));
	fprintf(fp, "PASSADO:%s\n", past);
	fprintf(fp, "FUTURO:%s\n", fut);

	free(past);
	free(fut);

	fclose(fp);
}

/**
\brief Lê, enquanto for possível, os valores obtidos de uma Stream de ficheiro e cria uma stack correspondente.

@param fp Apontador para ficheiro apartir do qual se pretende começar a ler a stack.

@returns A stack criada apartir do ficheiro passado como argumento.
*/
static STACK readAll(FILE *fp)
{
	STACK stk;
	stk = initS();
	char aux[300];
	int r, i, j, val, check;
	char ch;
	ch = fgetc(fp);
	while ((r = fscanf(fp, "%s", aux)) != EOF && ch != '\n')
	{
		sscanf(aux, "(%d,%d,%d,%d)", &i, &j, &val, &check);
		addToTail(stk, i, j, val, check);
		ch = fgetc(fp);
	}
	return stk;
}

/**
\brief Passa um ficheiro para Estado.
	A função cria um Estado apartir de um ficheiro, assumindo que este está escrito corretamente.
	É colocado no valor apontador por flag uma represetação nuḿerica da validade do ficheiro lido.

@param path String correspondente à diretória onde se encontram os users.
@param user String correspondente ao nome do ficheiro que irá ser lido.
@param flag Apontador para o inteiro onde irá ser colocado o valor da validade do ficheiro.

@returns O Estado obtido do ficheiro.
*/
ESTADO file2estado_un(char *path, char *user, int *flag)
{
	int l, c, m;
	int f;
	ESTADO e = makeState(NULL);
	int r;
	int i = 0, j = 0;
	char ch;
	char *aux = (char *)malloc(sizeof(char) * (strlen(path) + strlen(user) + strlen(".txt") + 1));
	sprintf(aux, "%s%s%s", path, user, ".txt");

	FILE *fp;
	fp = fopen(aux, "r");
	if (fp == NULL)
	{
		free(aux);
		*flag = 0;
		return e;
	}

	r = 0;
	r = fscanf(fp, "USER: ");
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = 0;
	while ((ch = fgetc(fp)) != EOF && ch != '\n')
		r++;

	if (r > 0)
	{
		free(aux);
		aux = (char *)malloc(sizeof(char) * (r + 1));
		rewind(fp);
		r = fscanf(fp, "USER: %s", aux);
		if (r == EOF)
		{
			free(aux);
			*flag = 0;
			return e;
		}
		strcpy(getE_user(e), aux);
		fgetc(fp);
	}

	r = fscanf(fp, "N_LINS: %d\n", &l);
	setE_lins(e, l);
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "N_COLS: %d\n", &c);
	setE_cols(e, c);
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "FLAG: %d\n", &f);
	setE_flag(e, f);
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "MENU: %d\n", &m);
	setE_menu(e, m);
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "HELP: %d\n", &m);
	setE_help(e, m);
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "WINS: %d\n", &m);
	setE_wins(e, m);
	if (r == EOF)
	{
		*flag = 0;
		return e;
	}
	r = fscanf(fp, "GRELHA:\n");
	if (r == EOF)
	{
		*flag = 0;
		return e;
	}
	while (i < getE_lins(e))
	{
		j = 0;
		while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		{
			setE_elem(e, i, j, ch - '0');
			fgetc(fp);
			j++;
		}
		i++;
	}
	r = fscanf(fp, "PASSADO:");
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	setE_stack(e, readAll(fp), 0);
	r = fscanf(fp, "FUTURO:");
	if (r == EOF)
	{
		free(aux);
		*flag = 0;
		return e;
	}
	setE_stack(e, readAll(fp), 1);
	fclose(fp);
	*flag = 1;
	free(aux);
	return e;
}
