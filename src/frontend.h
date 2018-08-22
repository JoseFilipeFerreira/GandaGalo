/**
*@file frontend.h
\brief Módulo de tratamento gráfico.
*/
#ifndef frontend_h
#define frontend_h

#include "estado.h"

// ------------------------------------------------------------------------------

/**
\brief Macro para abrir um body com a cor de fundo.
*/
#define BODY printf("<body bgcolor='#8FD8D8'>\n")

/**
\brief Macro para fechar o body.
*/
#define FECHAR_BODY printf("</body>\n")

/**
\brief Macro para cria o div que centra.
*/
#define DIV_CENTRAR printf("<div style='text-align:center;padding:50px'>\n")

/**
\brief Macro para fechar div.
*/
#define FECHAR_DIV printf("</div>\n")

/**
\brief Macro para calcular o valor .

@param windowsize tamanho da área disponível para desenhar.
@param win multiplier pelo valor windowsize.
@param mar multiplier pelo valor MARGIN(windowsize).
@param text multiplier pelo valor TEXTMARGIN(windowsize).
*/
#define calculate(windowsize, win, mar, text) (windowsize * win + mar * MARGIN(windowsize) + text * TEXTMARGIN(windowsize))

/**
\brief Macro para criar uma imagem que ignora correções de escala.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param ESCALA A escala da imagem.
@param FICHEIRO O caminho para o link do ficheiro.
*/
#define ACU_IMAGE(X, Y, ESCALA, FICHEIRO) printf("<image x=%d y=%d width=%d height=%d xlink:href=%s%s />\n", \
												 X, Y, ESCALA, ESCALA, IMAGE_PATH, FICHEIRO)

// ------------------------------------------------------------------------------

void printstate (ESTADO state, int windowsize);

void selectFile(char * nameTag, char * tag);

#endif
