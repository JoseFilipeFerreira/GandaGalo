/**
*@file solver.h
\brief Algoritmo para resolver mapas e estruturas para obter mapas aleatórios.
*/
#ifndef solver_h
#define solver_h

#include "estado.h"

// ------------------------------------------------------------------------------

/**
\brief Macro que indica a diretória de mapas aleatórios.
*/
#define RANDOMDIR "/var/www/html/ficheiro/mapas/random/"

// ------------------------------------------------------------------------------

/**
\brief Estrutura que representa o espaço de soluções de um dado Estado.
*/
typedef struct gtree* GTree;

/**
\brief Declaração do estado interno
*/
typedef struct state *T;

/**
\brief Gene do anamorfismo da Árvore de Soluções
*/
typedef int (*PositionSelector)(T, int, int *, int);

// ------------------------------------------------------------------------------

/* Métodos para obter o estado Externo */

ESTADO solve(ESTADO a, long* number_of_solutions);

char** convert_external(T e);

GTree ana(T e, PositionSelector func);

int cluster(T e, int u, int *cp, int sig);

T baseMap(int numl, int numc, float probB);

int findk(GTree node, GTree *v);

void destroyGTree(GTree node);

void pickG(T e, GTree tr);

void showS(T e);

int countUseS(T e);

int getDimension(T e);

void destroyit(T e);

void writeMap (T e, char * difficulty);

#endif
