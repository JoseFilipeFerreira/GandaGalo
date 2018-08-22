/**
@file filemanager.c
\brief Módulo de leitura de ficheiros.
*/

#include <stdlib.h>
#include <string.h>
#include "solver.h"
#include "filemanager.h"
#include "estado.h"
#include "cgi.h"
#include "frontend.h"
#include "state.h"

// ------------------------------------------------------------------------------

/* Métodos públicos */
ESTADO select_arcade(char *path, int id);
ESTADO select_padrao(char *path, char *map, int *flag);
ESTADO select_random(int id, int * flag);

/* Métodos privados */
static ESTADO ler_puzzle_padrao(FILE *file, int *flag);
static char convertFl(char ch, int *flag);

// ------------------------------------------------------------------------------

/**
\brief
	Esta função lê um puzzle na convenção dos docentes de li2.
	@param file descritor de ficheiro FILE já aberto.
	@param flag variavel com o endereço da variavel que registara o valor de sucesso ou insucceso desta função.

	@returns Devolve o @c ESTADO lido.
*/
static ESTADO ler_puzzle_padrao(FILE *file, int *flag)
{
	ESTADO e = makeState(NULL);
	int n_lin, n_col;
	int lLidas = 0, cLidas, ch = 0;

	ch = fscanf(file, "%d %d\n", &n_lin, &n_col);
	if (ch != 2)
	{
		*flag = 0;
		return e;
	}

	setE_lins(e, n_lin);
	setE_cols(e, n_col);
	setE_base(e,NULL);

	while (!feof(file))
	{
		cLidas = 0;

		while ((ch = fgetc(file)) != '\n' && ch != EOF)
		{
			ch = convertFl((char)ch, flag);
			setE_elem(e, lLidas, cLidas, ch);
			if (*flag == 0)
				return e;
			cLidas++;
		}
		if (cLidas != n_col)
		{
			*flag = 0;
			return e;
		}

		lLidas++;
	}
	if (lLidas != n_lin)
	{
		*flag = 0;
		return e;
	}
	*flag = 1;
	return e;
}

/**
\brief Verifica a validade de uma determinada peça.

@param ch Valor a comparar.
@param flag Endereço onde irá ser colocado 1 se o caráter for válido, ou 0 caso contrário.

@returns Devolve o valor convertido caso seja válido, ou o próprio valor de comparação caso seja inválido.

*/
static char convertFl(char ch, int *flag)
{
	char rval;
	switch (ch)
	{
	case 'X':
		rval = FIXO_X;
		break;
	case 'O':
		rval = FIXO_O;
		break;
	case '.':
		rval = VAZIA;
		break;
	case '#':
		rval = BLOQUEADA;
		break;
	default:
		*flag = 0;
		return ch;
	}
	*flag = 1;
	return rval;
}

// ------------------------------------------------------------------------------

/**
\brief
	Seleciona um dos mapas disponibilizados pela aplicação.
	@param path Caminho interno onde estão guardados os mapas.
	@param id ID do mapa em questão.

	@returns Devolve o @c ESTADO selecionado.
*/
ESTADO select_arcade(char *path, int id)
{
	FILE *file;
	ESTADO e = makeState(NULL);
	int cur_id;
	char str[4048];
	int flag;

	file = fopen(path, "r");

	if (!file)
	{
		setE_flag(e, -1);
		return e;
	}

	do
	{
		flag = 1;
		do
		{
			flag = fscanf(file, "%s", str);

			if (EOF == flag)
			{
				fclose(file);
				return e;
			}

		} while (strcmp("::>", str));

		flag = fscanf(file, "%d", &cur_id);

		if (EOF == flag)
		{
			fclose(file);
			return e;
		}

	} while (id != cur_id);

	e = ler_puzzle_padrao(file, &flag);
	setE_menu(e, PLAY_TAB);
	setE_flag(e, 0);

	fclose(file);
	return e;
}

/**
\brief
	Carrega um mapa selecionado pelo utilizador.
	@param path Caminho do mapa que o utilizador pretende abrir.
	@param map Nome do ficheiro a carregar.
	@param flag Variavél com o endereço da variavél que registara o valor de sucesso ou insucceso desta função.

	@returns Devolve o @c ESTADO selecionado.
*/
ESTADO select_padrao(char *path, char *map, int *flag)
{
	FILE *file;
	ESTADO e = makeState(NULL);
	char *aux = (char *)malloc(sizeof(char) * (strlen(path) + strlen(map) + 1));

	strcpy(aux, path);
	strcat(aux, map);
	file = fopen(aux, "r");
	if (!file)
	{
		*flag = 0;
		return e;
	}
	e = ler_puzzle_padrao(file, flag);
	setE_menu(e, PLAY_TAB);
	setE_flag(e, 0);
	if ( (!validTab(e)) || !(*flag) ){
		*flag = 0;
		destroyState(e);
		e = NULL;
	}
	fclose(file);
	return e;
}

/**
\brief Função utilizada para ler um tabuleiro random.

@param id Id do ficheiro a carregar.
@param flag Variavél onde se registará o valor de sucesso ou insucesso desta função.

@returns Estado lido.
*/
ESTADO select_random(int id, int * flag)
{
	ESTADO e = NULL;
	if (id == 1){
		e = select_padrao(RANDOMDIR,"1.txt",flag);
	}
	else if (id == 2)
		e = select_padrao(RANDOMDIR,"2.txt",flag);
	else
		*flag = 0;
	return e;
}
