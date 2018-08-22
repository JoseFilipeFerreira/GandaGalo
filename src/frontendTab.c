/**
*@file frontendTab.c
\brief Módulo de tratamento gráfico, especializado ao tabuleiro.
*/
#include "estado.h"
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "frontend.h"
#include "filemanager.h"
#include "decide.h"
#include "givehelp.h"
#include "solver.h"

// ------------------------------------------------------------------------------

/*  Métodos públicos */
void drawTab (ESTADO state,int windowsize, int x, int y);
void mapPlacer (int x, int y, int size, int id, ESTADO state, char * username);

/* Métodos privados */
static void aidimage (int x, int y, int size, char * img, char * buffer,int sign);
static void putimage (int ox, int oy, int x, int y, int size, int value, char *buffer, char menu, char linker);

// ------------------------------------------------------------------------------

/**
\brief Macro que calcula o valor máximo entre dois valores
@param n1 valor a comparar
@param n2 valor a comparar

@returns o valor maior entre n1 e n2
*/
#define getmax(n1,n2) ((n2)>(n1)) ? ((int)(n2)):((int)(n1))

// ------------------------------------------------------------------------------

/**
\brief Macro para criar colocar uma peça do tabuleiro
@param x abcissa do canto superior esquerdo da imagem 
@param y ordenada do canto superior esquerdo da imagem
@param size tamanho da imagem
@param img nome do ficheiro da imagem
@param buffer hiperligação a colocar na imagem
@param sign 0 para imprimir a imagem, 1 para imprimir e associar o link
*/
static void aidimage (int x, int y, int size, char * img, char * buffer,int sign)
{
	switch(sign)
	{
		case 0: ACU_IMAGE(x,y,size,img); break;
		case 1: ABRIR_LINK(buffer);
				ACU_IMAGE(x,y,size,img);
				FECHAR_LINK;
				break;
	}
}

/**
\brief Macro para criar colocar uma peça do tabuleiro
@param ox offset da peça no eixo Ox
@param oy offset da peça no eixo Oy
@param x número da linha da peça 
@param y número da coluna da peça
@param size tamanho da peça
@param value descrição enum da peça
@param buffer hiperligação a colocar na peça
@param menu menu onde se encontra o tabuleiro
@param linker identificação para desenhar soltos com link
*/
static void putimage (int ox, int oy, int x, int y, int size, int value, char *buffer, char menu, char linker)
{

	switch (value){

		case BLOQUEADA:
					aidimage(x*size + ox, y*size + oy,size, "bloq2.png", buffer, !menu);
					break;

		case FIXO_X: 
					aidimage(x*size + ox, y*size + oy,size, "green_shroom2.png",buffer,!menu);
					break;
		
		case FIXO_O:
					aidimage(x*size + ox, y*size + oy,size, "mario_coin_darker.png",buffer,!menu);
					break;

		case VAZIA: 
					aidimage(x*size + ox, y*size + oy,size, "vazio.png",buffer,linker);
					break;
					
		case SOL_X: 
					aidimage(x*size + ox, y*size + oy,size, "red_shroom2.png",buffer,linker);
					break;

		case SOL_O: 
					aidimage(x*size + ox, y*size + oy,size, "mario_coin2.png",buffer,linker);
					break;
	}
}

//----------------------------------------------------------------------------------------

/**
\brief Função que desenha o Tabuleiro

@param state estado a desenhar
@param windowsize tamanho da área disponível para desenhar
@param x offset do tabuleiro no eixo Ox
@param y offset do tabuleiro no eixo Oy
*/
void drawTab (ESTADO state,int windowsize, int x, int y)
{
	
	int lSide = getmax(getE_lins(state),getE_cols(state)); 

	int size = windowsize/lSide;

	int i, j;

	char buffer[MAX_BUFFER];

	for (i = 0; i < getE_lins(state); ++i){
		for (j = 0; j < getE_cols(state); ++j){
			//puts("4");
			sprintf(buffer,"http://localhost/cgi-bin/GandaGalo?%s/@l%dc%d/",getE_user(state),i,j);
			putimage(x,y,j,i,size,getE_elem(state,i,j),buffer, getE_menu(state), getE_menu(state)==0 || getE_menu(state)==1);
		}
	}

}

/**
\brief Função que desenha um Mapa clicavél dos mapas pré-definidos

@param x abcissa do canto superior esquerdo do mapa
@param y ordenada do canto superior esquerdo do mapa
@param size tamanho do mapa
@param id número do mapa que está a ser desenhado
@param state o estado onde se encontra o mapa a desenhar
@param username o nome do utilizador que está a jogar
*/
void mapPlacer (int x, int y, int size, int id, ESTADO state, char * username)
{
	char s[MAX_BUFFER];

	sprintf(s,"http://localhost/cgi-bin/GandaGalo?%s/$%d",username, id);

	ABRIR_LINK(s);

	    drawTab(state, size, x, y);

	FECHAR_LINK;
}