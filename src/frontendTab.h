/**
*@file frontendTab.h
\brief Módulo de tratamento gráfico, especializado ao tabuleiro.
*/
#ifndef frontendTab_h
#define frontendTab_h

// ------------------------------------------------------------------------------

void drawTab (ESTADO state,int windowsize, int x, int y);

void mapPlacer (int x, int y, int size, int id, ESTADO state, char * username);

#endif