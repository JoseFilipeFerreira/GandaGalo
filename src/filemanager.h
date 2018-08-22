/**
*@file filemanager.h
\brief Módulo de leitura de ficheiros.
*/

#ifndef ___FILEMANAGER_H___
#define ___FILEMANAGER_H___

#include "estado.h"
#include "solver.h"
#include "cgi.h"

// ------------------------------------------------------------------------------

/**
\brief Macro para ler um mapa prédefinido

@param id número do mapa

@returns estado com o mapa lido
*/
#define SELECT(id) select_arcade(FILE_PATH	,id)

/**
\brief Macro para ler um mapa do utilizador.

@param map Nome do ficheiro a carregar.
@param flag Variavél onde se registará o valor de sucesso ou insucesso desta função.

@returns estado com o mapa lido
*/
#define sPadrao(map,flag) (select_padrao(MAP_PATH,map,&(flag)))

// ------------------------------------------------------------------------------

ESTADO select_arcade(char *path, int id);

ESTADO select_padrao( char * path, char * map, int * flag);

ESTADO select_random(int id, int * flag);

#endif
