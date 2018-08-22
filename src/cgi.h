#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
\brief Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
* Caminho para as ancoras
*/
#define ANC_PATH							"/var/www/html/ficheiro/ancora.save"

/**
* Caminho para mapas pré-definidos
*/
#define FILE_PATH							"/var/www/html/ficheiro/mapas/selectedmap.map"

/**
* Caminho para chekpoints
*/
#define CHECK_PATH							"/var/www/html/ficheiro/checkpoint.save"

/**
* Caminho para a última sava
*/
#define LAST_PATH							"/var/www/html/ficheiro/last.save"

/**
* Caminho para os utilizadores
*/
#define USER_PATH							"/usr/local/games/GandaGalo/users/"

/**
* Caminho para os mapas
*/
#define MAP_PATH							"/var/www/html/ficheiro/mapas/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n<html>\n")


/**
\brief Macro para abrir um svg

@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d style='text-align:center;'>\n",tamx, tamy)

/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")

/**
\brief Macro para criar uma imagem

@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s%s />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH, FICHEIRO)

/**
\brief Macro para criar um quadrado

@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COR)			printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, COR)

/**
\brief Macro para escrever texto no ecrã

@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param COR A cor de preenchimento do quadrado
@param text o texto a escrever
*/
#define TEXT(X, Y, COR, text)                printf("<text x=%d y=%d fill=%s>%s</text>\n",  \
                                                X ,Y, COR,text)

/**
\brief Macro para abrir um link

@param link O caminho para o link
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")

/**
\brief Macro para fechar o html
*/
#define FECHAR_HTML							printf("</html>\n")

#endif
