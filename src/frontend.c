/**
*@file frontend.c
\brief Módulo de tratamento gráfico.
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
#include "frontendTab.h"
#include "leaderboard.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */
void printstate (ESTADO state, int windowsize);
void selectFile(char * nameTag, char * tag);

/* Metódos privados */
static void drawMenuDesignValidate(ESTADO state, int windowsize);
static void drawMenuTabDesignStack(ESTADO state, int windowsize);
static void drawMenuDesignSize(ESTADO state, int windowsize);
static void drawbuttonsplayStack(ESTADO state, int windowsize);
static void buttonPlacer (int x, int y, int size, char* link, char* pic);
static void circulateButtons(ESTADO state, int windowsize, int menuPrev, int menuNext);
static void drawMenuTabDesign(ESTADO state, int windowsize);
static void drawbuttonsplay(ESTADO state, int windowsize);
static void drawPickMap(ESTADO state, int  windowsize);
static void modeSelectMenu(ESTADO state, int windowsize);
static void drawConfirmMap(ESTADO state, int windowsize);
static void drawMenuVictory (ESTADO state,int windowsize);
static void drawMenuInvalidMap (ESTADO state,int windowsize);
static void drawMenuInitial (ESTADO state,int windowsize);
static void menuRandomMap (ESTADO state, int windowsize);
static void menuConfirmRMap (ESTADO state, int windowsize);
static void drawHint(ESTADO state, int windowsize);
static void helpP10(ESTADO state, int windowsize);
static void helpP11(ESTADO state, int windowsize);
static void helpP12(ESTADO state, int windowsize);
static void helpP13(ESTADO state, int windowsize);
static void helpP14(ESTADO state, int windowsize);
static void trofyPlacer(int windowsize, int NLeader);
static void scorePlacer(int windowsize, int NLeader, INFO v);

// ------------------------------------------------------------------------------

/**
\brief Constante que representa o número de mapas pré-definidos
*/
#define NMAPS 7

/**
\brief Macro para criar um valor de margem para trabalhar em frontend
@param windowsize a àrea disponível para desenhar

@returns valor de margem
*/
#define  MARGIN(windowsize)     (windowsize/8)


/**
\brief Macro para criar um valor de margem de texto para trabalhar em frontend
@param windowsize a àrea disponível para desenhar

@returns valor de margem de texto
*/
#define  TEXTMARGIN(windowsize) (windowsize/80)

/**
\brief Macro para criar um valor de tamanho para trabalhar em frontend
@param windowsize a àrea disponível para desenhar
@param x valor a multiplicar
@param d valor a dividir

@returns valor de tamanho
*/
#define SIZE(windowsize, x, d) (d? (((windowsize*x)/16) + (windowsize/(16*d))) : ((windowsize*x)/16))


// ------------------------------------------------------------------------------
/**
\brief colocar um botão
@param x offset do botão no eixo Ox
@param y offset do botão no eixo Oy
@param size A tamanho do botão
@param link fim da hiperligação a colocar no botão
@param pic nome da imagem do botão
*/
static void buttonPlacer (int x, int y, int size, char* link, char* pic)
{
	char s[MAX_BUFFER];

	sprintf(s,"http://localhost/cgi-bin/GandaGalo?%s", link);

	ABRIR_LINK(s);

	ACU_IMAGE (x, y , size, pic);

	FECHAR_LINK;
}


/**
\brief Função que desenha o rodapé do menu help

@param state estado a desenhar
@param windowsize tamanho da área disponível para desenhar
@param menuPrev menu anterior
@param menuNext menu seguinte
*/
static void circulateButtons(ESTADO state, int windowsize, int menuPrev, int menuNext)
{
	char link[MAX_BUFFER];

	if(menuPrev != -1)
	{
	    //botão para previous
	    sprintf(link, "%s/M%d",getE_user(state), menuPrev);
	    buttonPlacer(
	    	calculate(windowsize, 0, 0, 0),
	    	calculate(windowsize, 1, -2, 3),
	    	SIZE(windowsize, 1, 2),
	    	link,
	    	"undo.png");
	}
	
    if(menuNext != -1)
	{
	    //botão para next
	    sprintf(link, "%s/M%d",getE_user(state), menuNext);
	    buttonPlacer(
	    	calculate(windowsize, 0, 1, 0),
	    	calculate(windowsize, 1, -2, 3),
	    	SIZE(windowsize, 1, 2),
	    	link,
	    	"redo.png");
	}

	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M7");
	buttonPlacer(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 1, -2, 0),
		SIZE(windowsize, 3, 0),
		link,
		"back.png");
}

//----------------------------------------------------------------------------------------

/**
\brief Função auxiliar para criar a validação do Menu desenhar tabuleiro

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuDesignValidate(ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];
	long n_sol;

	n_sol = n_solutions(state);
	if (!n_sol){
		//aviso mapa inválido
		ACU_IMAGE(
			calculate(windowsize, 0, 2, 3),
			calculate(windowsize, 1, -4, -3),
			SIZE(windowsize,5,0),
			"invalidMap.png");

		//non-clickable start game button
		ACU_IMAGE(
			calculate(windowsize, 1, -2, -7),
			calculate(windowsize, 1, -3, 0),
			SIZE(windowsize, 5,0),
			"start2.png");
	}
	else
	{
		//start game button
	    sprintf(link, "%s/%s",getE_user(state), "safedraw");
	    buttonPlacer(
	    	calculate(windowsize, 1, -2, -7),
	    	calculate(windowsize, 1, -3, 0),
	    	SIZE(windowsize, 5, 0),
	    	link,
	    	"start2.png");
        
        //número de soluções
	    sprintf(link, "Number of solutions:%ld", n_sol);
	    TEXT(
	    	calculate(windowsize, 0, 2, 3),
	    	calculate(windowsize, 1, -3, -2),
	    	"black",
	    	link);
	}
}

/**
\brief Função auxiliar para criar o alterar tamanho do Menu desenhar tabuleiro

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuDesignSize(ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];

	sprintf(link, "x dimension: %d",getE_cols(state));
	TEXT(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 0, 1, -1),
		"black",
		link);
	//increase size y button
	sprintf(link, "%s/%s",getE_user(state), "x1");
	buttonPlacer(
		calculate(windowsize, 1, -1, 0),
		calculate(windowsize, 0, 1, 0) ,
		SIZE(windowsize,1,0),
		link,
		"plus.png");

	//decrease size y button
	sprintf(link, "%s/%s",getE_user(state), "x-1");
	buttonPlacer(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 0, 1, 0),
		SIZE(windowsize,1,0),
		link,
		"minus.png");

    sprintf(link, "y dimension: %d",getE_lins(state));
	TEXT(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 0, 2, -1),
		"black",
		link);
	//increase size x button
	sprintf(link, "%s/%s",getE_user(state), "y1");
	buttonPlacer(
		calculate(windowsize, 1, -1, 0),
		calculate(windowsize, 0, 2, 0),
		SIZE(windowsize,1,0),
		link,
		"plus.png");

	//decrease size x button
	sprintf(link, "%s/%s",getE_user(state), "y-1");
	buttonPlacer(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 0, 2, 0),
		SIZE(windowsize,1,0),
		link,
		"minus.png");
}

/**
\brief Função auxiliar para criar o undo/redo do Menu desenhar tabuleiro

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuTabDesignStack(ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];

    //botão para retroceder
	sprintf(link, "%s/%s",getE_user(state), "undo");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -4, 5),
		SIZE(windowsize, 1, 2),
		link,
		"undo.png");

	//botão para redo
	sprintf(link, "%s/%s",getE_user(state), "redo");
	buttonPlacer(
		calculate(windowsize, 0, 1, 3),
		calculate(windowsize, 1, -4, 5),
		SIZE(windowsize,1,2),
		link,
		"redo.png");
}

/**
\brief Função para criar o Menu desenhar tabuleiro

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuTabDesign(ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];

	drawTab (state, windowsize - 400, 40, 40);

	TEXT(
		TEXTMARGIN(windowsize),
		25,
		"black",
		"MAP GENERATOR");
	
	drawMenuDesignSize(state, windowsize);

	drawMenuDesignValidate(state, windowsize);

	drawMenuTabDesignStack(state, windowsize);

	//botão para limpar as pecas colocadas pelo jogador
	sprintf(link, "%s/%s",getE_user(state), "clearS");
	buttonPlacer(
		calculate(windowsize, 1, -2, 0),
		calculate(windowsize, 0, 2, 6),
		SIZE(windowsize,3,0),
		link,
		"clear_all2.png");
	
	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3,0),
		link,
		"back.png");
}

/**
\brief Função auxiliar para o Menu jogar que desenha as hint
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawHint(ESTADO state, int windowsize)
{
	char link[MAX_BUFFER];

	if(getE_help(state) > 0)
	{
	sprintf(link, "%s/%s",getE_user(state), "help");
	buttonPlacer(
		calculate(windowsize, 1, 0, -5),
		calculate(windowsize, 0, 0, 2),
		SIZE(windowsize,2,0),
		link,
		"help.png");
	}
	else
	{
		ACU_IMAGE(
			calculate(windowsize, 1, 0, -5),
		    calculate(windowsize, 0, 0, 2),
		    SIZE(windowsize,2,0),
		    "help.png");
	}

	sprintf(link, "Hints left: %d",getE_help(state));
	TEXT(
		calculate(windowsize, 1, 0, -5),
		calculate(windowsize, 0, 0, 2),
		"black",
		link);

}

/**
\brief Função auxiliar para o Menu jogar
Desenha undo/redo e ancoras
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawbuttonsplayStack(ESTADO state, int windowsize)
{
	char link[MAX_BUFFER];
	//load checkpoint	
	sprintf(link, "%s/%s",getE_user(state), "getAnc");
	buttonPlacer(
		calculate(windowsize, 1, -1, 0),
		calculate(windowsize, 0, 2, 0),
		SIZE(windowsize, 4,0),
		link ,
		"load_checkpoint.png");

    //chekpoint game button
	sprintf(link, "%s/%s",getE_user(state), "saveCheckpoint");
	buttonPlacer(
		calculate(windowsize, 1, -1, 0),
		calculate(windowsize, 0, 1, 0),
		SIZE(windowsize,4,0),
		link,
		"save_checkpoint.png");

	//botão para undo
	sprintf(link, "%s/%s",getE_user(state), "undo");
	buttonPlacer(
		calculate(windowsize, 0, 0, 0),
		calculate(windowsize, 1, -2, 3),
		SIZE(windowsize,1,2),
		link,
		"undo.png");

	//botão para redo
	sprintf(link, "%s/%s",getE_user(state), "redo");
	buttonPlacer(
		calculate(windowsize, 0, 1, 0),
		calculate(windowsize, 1, -2, 3),
		SIZE(windowsize,1,2),
		link,
		"redo.png");

}
/**
\brief Função para o Menu jogar
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawbuttonsplay(ESTADO state, int windowsize)
{
	char link[MAX_BUFFER];
	char* buffer = malloc(sizeof(char)*MAX_BUFFER);
	//pop_Anc2(tmp,buffer);

	drawTab (state, windowsize - 200, 10, 10);

	//botão para limpar todas as jogadas feitas pelo jogador
	sprintf(link, "%s/%s",getE_user(state), "clear");
	buttonPlacer(
		calculate(windowsize, 1, -1, 2),
		calculate(windowsize, 0, 4, 3),
		SIZE(windowsize, 3, 0),
		link,
		"clear_all2.png");

	//botão para resolver automaticamente o tabuleiro
	sprintf(link, "%s/%s",getE_user(state), "solve");
	buttonPlacer (
		calculate(windowsize, 1, -1, 0),
		calculate(windowsize, 0, 5, 0),
		SIZE(windowsize,4,0),
		link,
		"solveMap.png");
	
	drawbuttonsplayStack(state, windowsize);
	
	drawHint(state, windowsize);
	
	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer(
		calculate(windowsize, 0, 2, 3),
		calculate(windowsize, 1, -2, 0),
		SIZE(windowsize,3,0),
		link,
		"back.png");

	free(buffer);
}

/**
\brief Função que desenha o menu de escolher mapas
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawPickMap(ESTADO state, int  windowsize)
{
	ESTADO aux;
	char link [MAX_BUFFER];
	int i,j;
	int idMap = 1;
	int ncoll = 2;
	int nlins = 3;

	TEXT(
	calculate(windowsize, 0, 0, 0),
	calculate(windowsize, 0, 0, 2),
	"black",
	"Select Map:");

    for (i = 1; i <= ncoll && idMap <= NMAPS; ++i){
    	for (j = 0; j <=nlins && idMap <= NMAPS; ++j){

    		aux = SELECT(idMap);
    		setE_menu(aux,SELECT_MAP);

    	    mapPlacer(
    	    	j * SIZE(windowsize,4,2), 
    	    	(i - 1) * SIZE(windowsize, 4, 2) + calculate(windowsize, 0, 1, 0),
    	    	SIZE(windowsize, 4, 0),
    	    	idMap,
    	    	aux,
    	    	getE_user(state));
    	    idMap++;
    	}
    }

    //botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3 ,0),
		link,
		"back.png");
	
	destroyState(aux);
}

/**
\brief Função que desenha o Menu para escolher o modo de jogo
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void modeSelectMenu(ESTADO state, int windowsize)
{ 
    char link [MAX_BUFFER];

    //desenhar mapa
    sprintf(link, "%s/%s",getE_user(state), "M0");
	buttonPlacer(
		windowsize / 2,
		calculate(windowsize, 0,  1, 0),
		SIZE(windowsize, 5 ,0),
		link,
		"drawMenu.png");

	//mapa aleatório
	sprintf(link, "%s/%s",getE_user(state), "M8");
	buttonPlacer(
		windowsize / 2,
		calculate(windowsize, 0,  2, 2),
		SIZE(windowsize, 5, 0),
		link,
		"randomM.png");

    //escolhaer mapa de ficheiro
	sprintf(link, "%s/%s",getE_user(state), "M4");
	buttonPlacer(
		windowsize / 2,
		calculate(windowsize, 0,  4, 0),
		SIZE(windowsize, 5, 0),
		link,
		"selectMap.png");
    
    //botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M7");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3, 0),
		link,
		"back.png");
}

/**
\brief Função que desenha o Menu para confirmar o mapa escolhido
@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawConfirmMap(ESTADO state, int windowsize)
{
    
    char link [MAX_BUFFER];

    sprintf(link, "%s/%s",getE_user(state), "M1");
	buttonPlacer(
		calculate(windowsize, 0, 5, -2),
		calculate(windowsize, 0, 3, 1),
		SIZE(windowsize, 2, 0),
		link,
		"accept.png");

	sprintf(link, "%s/%s",getE_user(state), "M4");
	buttonPlacer(
		calculate(windowsize, 0, 3, -2),
		calculate(windowsize, 0, 3, 1),
		SIZE(windowsize, 2, 0),
		link,
		"decline.png");

	drawTab(state, windowsize/3, windowsize/2 - MARGIN(windowsize), 0);

	
}

/**
\brief Função que desenha o Menu de Vitória

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuVictory (ESTADO state,int windowsize)
{
	char link [MAX_BUFFER];

    //imagem de fundo
    ACU_IMAGE (
		0,
		0,
		windowsize + 100,
		"winback.png");

    //texto de vitória + voltar ao menu inicial
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer (
		0,
		0,
		windowsize + 100,
		link,
		"win.png");

    //desenhar o mapa completado
	drawTab(
		state,
		windowsize/3,
		windowsize/2 - MARGIN(windowsize),
		0);
}

/**
\brief Função que desenha o Menu de mapa inválido

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuInvalidMap (ESTADO state,int windowsize)
{
	char link [MAX_BUFFER];

    ACU_IMAGE(
        windowsize/2 - SIZE(windowsize, 5, 0),
        calculate(windowsize, 0, 1, 0),
        SIZE(windowsize, 10,0),
        "invalidMap.png");

	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M4");
	buttonPlacer(
	    calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3,0),
	    link,
	    "back.png");
}

/**
\brief Função que desenha o Menu de mapa inválido

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuInvalidRandom (ESTADO state,int windowsize)
{
	char link [MAX_BUFFER];

    ACU_IMAGE(
        windowsize/2 - SIZE(windowsize, 5, 0),
        calculate(windowsize, 0, 1, 0),
        SIZE(windowsize, 10,0),
        "invalidMap.png");

	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M8");
	buttonPlacer(
	    calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3,0),
	    link,
	    "back.png");
}


/**
\brief Função que coloca uma textbox

@param nameTag nome da TextBox
@param tag parte de trás da string
*/
void selectFile(char * nameTag, char * tag)
{
    printf("<form action="">\n %s \n<input type=\"text\" name=\"%s\" \n>\n <br> <input type=\"submit\">\n</form>",nameTag, tag);
}

/**
\brief Função que desenha o Menu inicial

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawMenuInitial (ESTADO state,int windowsize)
{
	char link [MAX_BUFFER];

    //botão start
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer(
	    windowsize / 2,
	    calculate(windowsize, 0, 1, 0),
	    SIZE(windowsize, 5, 0),
	    link,
	    "play.png");

	//botão leaderboard
	sprintf(link, "%s/%s",getE_user(state), "M16");
	buttonPlacer(
	    windowsize / 2,
	    calculate(windowsize, 0, 2, 2),
	    SIZE(windowsize, 5, 0),
	    link,
	    "leaderboard.png");

	//botão help
	sprintf(link, "%s/%s",getE_user(state), "M10");
	buttonPlacer(
	    windowsize / 2,
	    calculate(windowsize, 0, 4, 0),
	    SIZE(windowsize, 5 ,0),
	    link,
	    "helpM.png");
}

/**
\brief Função que desenha o Menu de mapas aleatórios

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void menuRandomMap (ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];

	//botão para mapa aleatório easy
	sprintf(link, "%s/%s",getE_user(state), "R1");
	buttonPlacer(
		windowsize/2,
		calculate(windowsize, 0, 0, 3),
		SIZE(windowsize, 6,0),
		link,
		"easy.png");

	//botão para mapa aleatório hard
	sprintf(link, "%s/%s",getE_user(state), "R2");
	buttonPlacer(
		windowsize/2,
		calculate(windowsize, 0, 2, 3),
		SIZE(windowsize, 6,0),
		link,
		"hard.png");

    //botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M2");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3,0),
		link,
		"back.png");
}

/**
\brief Função que desenha o Menu para cnfirmar mapas

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void menuConfirmRMap (ESTADO state, int windowsize)
{
	char link [MAX_BUFFER],n_sol;

	//aceitar mapa
    sprintf(link, "%s/%s",getE_user(state), "safedraw");
	buttonPlacer(
		calculate(windowsize, 0, 5, -2),
		calculate(windowsize, 0, 3, 1),
		SIZE(windowsize, 2, 0),
		link,
		"accept.png");

    //recusar mapa
	sprintf(link, "%s/%s",getE_user(state), "M8");
	buttonPlacer(
		calculate(windowsize, 0, 3, -2),
		calculate(windowsize, 0, 3, 1),
		SIZE(windowsize, 2, 0),
		link,
		"decline.png");

    //desenhar mapa
	drawTab(state, windowsize/3, windowsize/2 - MARGIN(windowsize), 0);

    n_sol = n_solutions(state);
    //número de soluções
	sprintf(link, "Number of solutions:%d", n_sol);
	TEXT(
		calculate(windowsize, 0, 3, 0),
		calculate(windowsize, 0, 3, -1),
		"black",
		link);
}

/**
\brief Função que desenha o Menu de ajuda sobre leadearboard

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void helpP10(ESTADO state, int windowsize)
{
	ACU_IMAGE(
		calculate(windowsize, 0, 0, 0),
		calculate(windowsize, 0, 0, -20),
		SIZE(windowsize, 18, 0),
		"helpLeaderboard.png"
	);

    circulateButtons(state, windowsize, -1, 11);
}

/**
\brief Função que desenha o Menu de ajudas sobre jogar

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void helpP11(ESTADO state, int windowsize)
{
	ACU_IMAGE(
		calculate(windowsize, 0, 0, 0),
		calculate(windowsize, 0, 0, -16),
		SIZE(windowsize, 18, 0),
		"helpMenuPlay.png"
	);
	
	circulateButtons(state, windowsize, 10, 12);
}

/**
\brief Função que desenha o Menu de ajudas sobre desenhar mapas

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void helpP12(ESTADO state, int windowsize)
{	
	ACU_IMAGE(
		calculate(windowsize, 0, 0, 0),
		calculate(windowsize, 0, 0, -15),
		SIZE(windowsize, 18, 2),
		"helpMenuMapGenerator.png"
	);
	
	circulateButtons(state, windowsize, 11, 13);
}

/**
\brief Função que desenha o Menu de ajudas sobre geração aleatória de mapas

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void helpP13(ESTADO state, int windowsize)
{
	ACU_IMAGE(
		calculate(windowsize, 0, 0, 4),
		calculate(windowsize, 0, 0, -4),
		SIZE(windowsize, 14, 0),
		"menuHelpRandom.png"
	);
	
	circulateButtons(state, windowsize, 12, 14);
}

/**
\brief Função que desenha o Menu de ajudas sobre selecionar ficheiros

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void helpP14(ESTADO state, int windowsize)
{
	ACU_IMAGE(
		calculate(windowsize, 0, 0, 0),
		calculate(windowsize, 0, 0, -11),
		SIZE(windowsize, 16, 0),
		"helpMenuSelectFile.png"
	);
	
	circulateButtons(state, windowsize, 13, -1);
}

/**
\brief Função que desenha os trofeus no menu de leaderboard

@param windowsize tamanho da área disponível para desenhar
@param NLeader número de utilizadores a escrever
*/
static void trofyPlacer(int windowsize, int NLeader)
{
	char name[MAX_BUFFER];
	for(int i = 0; i < NLeader; i++)
	{
		sprintf(name, "%dPlace.png", (i + 1));
		ACU_IMAGE(
		    calculate(windowsize, 0, 3, 0),
		    calculate(windowsize, 0, i, 30),
		    SIZE(windowsize, 1,2),
		    name);
	}
}

/**
\brief Função que desenha os resultados no menu de leaderboard

@param windowsize tamanho da área disponível para desenhar
@param NLeader número de utilizadores a escrever
@param v informação sobre a leaderboard
*/
static void scorePlacer(int windowsize, int NLeader, INFO v)
{
	char link [MAX_BUFFER];	

	for (int i = 0; i < NLeader; i++)
	{

	    TEXT(
	    	calculate(windowsize, 0, 3, 9),
	    	calculate(windowsize, 0, i, 33),
	    	"black",
	    	getInfo_user(v,i));

		sprintf(link, "%d", getInfo_wins(v,i));

	    TEXT(
	    	calculate(windowsize, 0, 3, 9),
	    	calculate(windowsize, 0, i, 35),
	    	"black",
	    	link);
	}
}

/**
\brief Função que desenha o Menu de leaderboard

@param state o estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
static void drawLeaderBoard(ESTADO state, int windowsize)
{
	char link [MAX_BUFFER];

	INFO v;
	int NLeader;
	NLeader = get_info(&v, 3);

	ACU_IMAGE(
		0,
		calculate(windowsize, 0, -2, -7),
		windowsize,
		"backLeaderboard.png"
	);

	trofyPlacer(windowsize, NLeader);

	scorePlacer(windowsize, NLeader, v);

	//botão para voltar ao menu anterior
	sprintf(link, "%s/%s",getE_user(state), "M7");
	buttonPlacer(
		calculate(windowsize, 0, 0, 3),
		calculate(windowsize, 1, -3, 5),
		SIZE(windowsize, 3,0),
		link,
		"back.png");
}

/**
\brief Função que deesenha o Estado

@param state estado a desenhar
@param windowsize tamanho da área disponível para desenhar
*/
void printstate (ESTADO state, int windowsize)
{
	char tagger[100];

	if (victory (state) && getE_menu(state) == 1) setE_menu(state, VICTORY);
	switch (getE_menu(state)){

		case DRAW_TAB:
		    ABRIR_SVG(windowsize + 100, windowsize);
			    //drawButtons(initButtons(DRAW_TAB), state);
			    drawMenuTabDesign (state, windowsize);
			FECHAR_SVG;
			break;

		case PLAY_TAB:
		    ABRIR_SVG(windowsize + 100, windowsize); 
			    drawbuttonsplay (state, windowsize);
            FECHAR_SVG;
			break;

		case SELECT_MENU:
		    ABRIR_SVG(windowsize + 100, windowsize); 
		        modeSelectMenu(state, windowsize);
		    FECHAR_SVG;
		    break;

		case VICTORY:
		    ABRIR_SVG(windowsize + 100, windowsize); 
			    drawMenuVictory (state, windowsize);
			FECHAR_SVG;
			break;
		case SELECT_MAP:
		    sprintf(tagger, "%s_mapName", getE_user(state));
		    selectFile("File Name:", tagger);

		    ABRIR_SVG(windowsize + 100, windowsize); 
		        drawPickMap(state, windowsize);
		    FECHAR_SVG;
			break;

		case CONFIRM_MAP:
		    ABRIR_SVG(windowsize + 100, windowsize); 
		        drawConfirmMap(state, windowsize);
		    FECHAR_SVG;
			break;

		case INVALID_MAP:
		    ABRIR_SVG(windowsize + 100, windowsize); 
	            drawMenuInvalidMap (state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case INVALID_RANDOM:
		    ABRIR_SVG(windowsize + 100, windowsize); 
	            drawMenuInvalidRandom (state, windowsize);
	        FECHAR_SVG;
	        break;

	    case INITIAL_MENU:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            drawMenuInitial (state, windowsize);
	        FECHAR_SVG;
	        break;

	    case SELECT_DIFFICULTY:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            menuRandomMap (state, windowsize);
	        FECHAR_SVG;
	        break;

	    case RANDOM_MENU:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            menuConfirmRMap (state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case H10: 
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            helpP10(state, windowsize);
	        FECHAR_SVG;
	        break;
	    
		case H11: 
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            helpP11(state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case H12: 
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            helpP12(state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case H13:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            helpP13(state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case H14:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            helpP14(state, windowsize);
	        FECHAR_SVG;
	        break;
		
		case LEADERBOARD:
	        ABRIR_SVG(windowsize + 100, windowsize); 
	            drawLeaderBoard(state, windowsize);
	        FECHAR_SVG;
	        break;
	}
}