/**
*@file userfiles.h
\brief Módulo para conversão entre ficheiros e ESTADO.
*/

#ifndef USER_H
#define USER_H

#include "cgi.h"
#include "estado.h"

// ------------------------------------------------------------------------------

/**
\brief Macro para converter um ficheiro em estado
@param user nome de utilizador
@param flag Apontador para o inteiro onde irá ser colocado o valor da validade do ficheiro.
*/
#define file2estado(user,flag) (file2estado_un(USER_PATH,user,&(flag)))

/**
\brief Macro para converter um estado em ficheiro
@param user nome de utilizador
@param e estado a guardar
*/
#define estado2file(user,e) (estado2file_un(USER_PATH,user,e))

// ------------------------------------------------------------------------------

ESTADO file2estado_un (char * path, char * user, int * flag);

void estado2file_un (char * path, char * user, ESTADO e);

#endif
