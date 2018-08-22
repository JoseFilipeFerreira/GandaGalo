/**
@file exemplo.c
\brief Esqueleto do programa.
*/
#include "cgi.h"
#include "frontend.h"
#include "parser.h"

/**
\brief Função que liga o jogo todo.
*/
int main()
{
	COMECAR_HTML;
	BODY;
	DIV_CENTRAR;

	pipe_env();

	FECHAR_DIV;
	FECHAR_BODY;
	FECHAR_HTML;

	return 0;
}
