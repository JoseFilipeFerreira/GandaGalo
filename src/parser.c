/**
@file parser.c
\brief Módulo de User input / Interpertador de comandos.
*/

#include "estado.h"
#include "frontend.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "userfiles.h"
#include "decide.h"
#include "filemanager.h"
#include "state.h"
#include "solver.h"
#include "givehelp.h"

// ------------------------------------------------------------------------------

/* Metódos públicos */
void pipe_env(void);

/* Metódos privados */
static int main_op (ESTADO e, char * command);
static void snd_op (ESTADO e, char * command);
static int get_resize (ESTADO e, char * command);
static int read_menu (ESTADO e, char * command);
static int choose_load (ESTADO e, char * command);
static void load_map (ESTADO e, char * command);
static void load_id (ESTADO e, char * command);
static int load_random (ESTADO e, char * name);
static int makePlay(ESTADO e, char * command);
static int readParse (char * command, int * i, int * j);
static ESTADO load_user (char * user);
static char * put_user (char * userQuery);
static char * convert(char *ler);
static int getUserC(char *query, char *user, char *command);

// ------------------------------------------------------------------------------

/**
\brief Macro para o tamanho da área a desenhar
*/
#define tabSize 800

// ------------------------------------------------------------------------------

/**
\brief Efetua todas as operações necessárias para load do utilizador e aplicação dos comandos.

Para a leitura de utilizadores e comandos há três situações, consoante o número de itens lidos.

- Caso nenhum item seja lido, então é colocada na página uma input box onde se deve colocar o utilizador.
- Caso seja só lido o utilizador, é aberto o @c ESTADO correspondente e apresentado.
- Caso seja lido o utilizador e o comando, é apresentado o @c ESTADO corresponde com as alterações do comando já efetuadas.
	- Neste caso acima, primeiro são analisadas as operações principais, e caso nenhuma seja efetuada, então passa para as operações secundárias.

No final, o @c ESTADO é escrito em ficheiro.

@see getUserC
@see convert
@see selectFile
@see load_user
@see main_op
@see snd_op
@see estado2file
@see printstate
@see destroyState
*/
void pipe_env (void)
{
	char * query = getenv("QUERY_STRING");
	char user[50], command[50];
	int nRead = getUserC(convert(query),user,command);

	ESTADO state;
	strcpy(user,put_user(user));
	if (nRead < 1 || !strcmp(user,""))
		selectFile("User Name", "newUser");
	else {
		state = load_user(user);
		if (nRead == 2)
			if(!main_op(state,command))
				snd_op(state,command);
		printstate(state, tabSize);
		estado2file(user, state);
		destroyState(state);
	}
}

/**
\brief Efetua a leitura de comandos considerados com principais.

Estes comandos mais importantes correspondem aos seguintes:

- Efetuar uma jogada na grelha.
- Efetuar operações de redimensão.
- Alterar o menu do @c ESTADO.
- Fazer load de uma grelha guardada em ficheiro.

@param e @c ESTADO que irá ser alterado.
@param command Comando passado na @b QUERY_STRING.

@returns 1 se o estado foi alterado, 0 caso contrário.

@see makePlay
@see get_resize
@see read_menu
@see choose_load
*/
static int main_op (ESTADO e, char * command)
{
	int r;
	r = makePlay(e,command);
	if (!r) r = get_resize(e,command);
	if (!r) r = read_menu(e,command);
	if (!r) r = choose_load(e,command);
	if (!r) r = load_random(e,command);
	return r;
}

/**
\brief Efetua a leitura de comandos considerados como secundários.

Efetua a leitura de comandos que são considerados menos importantes em termos de
aplicação, e por isso a sua verificação é feita numa só função. Estas funções correspondem 
aos seguintes comandos.

@b Comando : @b Função.

- help : givehelp.
- undo : pipestack, com o segundo argumento a 0.
- redo : pipestack, com o segundo argumento a 1.
- solve : solve.
- clear : clearstate.
- saveCheckpoint : saveAnc.
- safedraw : safedraw.
- getAnc : pop_Anc.
- clearS : clearcanvas.
.

@param e @c ESTADO que irá ser alterado.
@param command Comando obtido da @b QUERY_STRING.

@see givehelp
@see pipestack
@see solve
@see clearstate
@see saveAnc
@see safedraw
@see pop_Anc
@see clearcanvas
*/
static void snd_op (ESTADO e, char * command)
{
	long nsol;
	if (!strcmp(command, "help"))
		givehelp(e);
	if (!strcmp(command, "undo"))
		pipestack(e, 0);
	if (!strcmp(command, "redo"))
		pipestack(e, 1);
	if (!strcmp(command, "solve"))
		e = solve(e, &nsol);
	if (!strcmp(command, "clear"))
		clearstate(e);
	if (!strcmp(command, "saveCheckpoint"))
		saveAnc(e);
	if (!strcmp(command, "safedraw"))
		safedraw(e);
	if (!strcmp(command, "getAnc"))
		pop_Anc(e);
	if (!strcmp(command, "clearS"))
		clearcanvas(e);
}

/**
\brief Efetua a leitura do tipo de resize que se pretende fazer à grelha do tabuleiro.

@param e @c ESTADO que irá ser alterado.
@param command Comando obtido da @b QUERY_STRING.

@returns 1 se o tamanho de grelha foi alterado, 0 caso contrário.

@see increase
*/
static int get_resize (ESTADO e, char * command)
{
	int n, r = 1;;
	if (sscanf(command,"x%d",&n) > 0)
		increase(e,n,0);
	else if (sscanf(command,"y%d",&n) > 0)
		increase(e,0,n);
	else 
		r = 0;
	return r;
}

/**
\brief Efetua a leitura do menu para o qual se pretende alterar.

A função lê o menu para o qual se pretende alterar e faz a devida alteração,
sendo que quando o estado corresponde a MENU_INDEX::SELECT_MENU o @c ESTADO é inicializado.

@param e @c ESTADO que irá ser alterado.
@param command Comando passado na @b QUERY_STRING.

@returns 1, se o menu foi alterado, 0 caso contrário.

@see setE_menu
@see getE_menu
@see inicializar
*/
static int read_menu (ESTADO e, char * command)
{
	int menu, r = 1;
	if (sscanf(command, "M%d", &menu) > 0)
	{
		setE_menu(e, menu);
			if (menu == SELECT_MENU){
				setE_lins(e,5);
				setE_cols(e,5);
				setE_base(e,NULL);
			}
	}
	else
		r = 0;
	return r;
}

/**
\brief Consoante o comando passado na @b QUERY_STRING decide que tipo de load terá de ser feito.

@param e @c ESTADO onde terá de ser colocada a grelha.
@param command Comando obtido da @b QUERY_STRING.

@returns 1 se o comando corresponde à nomenclatura de algum load, 0 caso contrário.

@see load_map
@see load_id
*/
static int choose_load (ESTADO e, char * command)
{
	int r = 1;
	if (strstr(command,"="))
		load_map(e,command);
	else if (strstr(command,"$"))
		load_id(e,command);
	else
		r = 0;
	return r;
}

/**
\brief Efetua load de um mapa gerado aleatoriamente, anteriormente.
*/
static int load_random (ESTADO e, char * name)
{
	int id, flag, r = 1;
	ESTADO tmp;
	if (sscanf(name,"R%d",&id) == 1)
	{
		tmp = select_random(id,&flag);
		if (flag){
			strcpy(getE_user(tmp),getE_user(e));
			setE_wins(tmp,getE_wins(e));
			setE_state(e,tmp);
			setE_menu(e, RANDOM_MENU);
			setE_helpB(e);
			destroyState(tmp);
		} else {
			setE_menu(e, INVALID_RANDOM);
			r = 0;
		}
	} else 
		r = 0;

	return r;
}

/**
\brief Efetua load de um mapa criado pelo utilizador ao contrário de load_id.

Lê a grelha contida em ficheiro, e caso esta seja válida e se encontre no formato pedido,
então esta é colocada no @c ESTADO @p e passado como argumento.

@param e @c ESTADO onde irá ser colocada a grelha.
@param command Comando passado da @b QUERY_STRING.

@see sPadrao
@see setE_state
@see destroyState
@see setE_menu
@see setE_help
@see load_id
*/
static void load_map (ESTADO e, char * command)
{
	int flag;
	ESTADO tmp = sPadrao(strstr(command,"=") + 1, flag);
	if (flag){
		strcpy(getE_user(tmp),getE_user(e));
		setE_wins(tmp,getE_wins(e));
		setE_state(e,tmp);
		destroyState(tmp);
		setE_menu(e, CONFIRM_MAP);
		setE_helpB(e);
	}
	else
		setE_menu(e, INVALID_MAP);
}

/**
\brief Efetua load do mapa guardado em ficheiro.

Lê o mapa contido em ficheiro, fazendo a verificação da sua validade, 
e se este for válido, coloca em @p e a grelha correspondente. Caso contrário,
coloca o utilizador no menu de mapa inválido.

@param e @c ESTADO onde irá ser colocada a grelha lida.
@param command String correspondente ao comando passado na @b QUERY_STRING.

@see SELECT
@see setE_state
@see setE_menu
@see destroyState
*/
static void load_id (ESTADO e, char * command)
{
	ESTADO aux;
	int id;

	sscanf(command, "$%d", &id);
	aux = SELECT(id);
	setE_user(aux,getE_user(e));
	setE_wins(aux,getE_wins(e));
	setE_state(e,aux);
	if (validTab(aux))
		setE_menu(e, CONFIRM_MAP);
	else
		setE_menu(e, INVALID_MAP);
	destroyState(aux);
}

/**
\brief Consoante um comando, efetua um jogada em um dado @c ESTADO.

Após os valores da linha e coluna serem lidos do comando, é efetuada a jogada correspondente na posição.

@param e @c ESTADO que irá ser alterado.
@param command Comando obtido da @b QUERY_STRING.

@returns 1 se o @c ESTADO foi alterado, 0 caso contrário.

@see readParse
@see playPos
*/
static int makePlay(ESTADO e, char * command)
{
	int i, j, r, changed = 0;
	r = readParse(command,&i,&j);

	if (r == 2){
		playPos(e,i,j);
		changed = 1;
	}

	return changed;
}

/**
\brief Obtem os valores passados no comando de jogada.

Por nomenclatura um comando de jogada, é todo o comando que comece
com o caráter '@'.

@param command O comando lido da @b QUERY_STRING.
@param i Apontador onde ira colocar a linha onde se pretende jogar.
@param j Apontador onde ira colocar a coluna onde se pretende jogar.

@returns O número de elementos lidos com sucesso.
*/
static int readParse (char * command, int * i, int * j)
{
	return sscanf(command,"@l%dc%d",i,j);
}

/**
\brief Efetua o load do @c ESTADO associado a um certo utilizador.

Cria o @c ESTADO guardado em ficheiro de um certo utilizador, ou caso este
não exista, cria um novo @c ESTADO que será associado ao utilizador passado como argumentos.

@param user O utilizador do qual se pretende fazer load.

@returns O @c ESTADO lido.

@see put_user
@see file2estado
@see inicializar
@see setE_menu
*/
static ESTADO load_user (char * user)
{
	ESTADO e; int flag;

	e = file2estado(user,flag);
	if (!flag){
		e = inicializar(user,5,5);
		setE_menu(e, INITIAL_MENU);
	}

	return e;
}

/**
\brief Identifica em que posição de encontra o identificador do utilizador.

@param userQuery String lida da @b QUERY_STRING correspondente.

@returns Posição onde se encontra o identificador do utilizador.
*/
static char * put_user (char * userQuery)
{
	if (strstr(userQuery,"="))
		return strstr(userQuery,"=") + 1;
	else
		return userQuery;

}

/**
\brief Faz a conversão de uma string de forma a poder ser lida pela função getUserC.

A função converte todas as ocorrências de @b '/' e @b '_' em espaços, pois só desta forma é possível
ser lida.

@param ler A string que irá converter.

@returns A própria string.
*/
static char * convert(char *ler)
{
	int i;
	if (ler != NULL)
	{
		for (i = 0; ler[i] != '\0'; i++)
			if (ler[i] == '/' || ler[i] == '_')
				ler[i] = ' ';
	}
	return ler;
}

/**
\brief Identifica o utilizador e o comando a ser executado.

Identifica, caso existam, o utilizador e o comando passado na @b QUERY_STRING.

@param query @b QUERY de onde se irá retirar o utilizador e o comando.
@param user Apontador de onde irá ser colocado o utilizador passado na @b QUERY
@param command Apontador de onde irá ser colocado o comando passado na @b QUERY.

@returns O número de inputs lidos com sucesso.
*/
static int getUserC(char *query, char *user, char *command)
{ 
	int r = 0;
	if (query != NULL)
		r = sscanf(query, "%s %s", user, command);
	return r;
}