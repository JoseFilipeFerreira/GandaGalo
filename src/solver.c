/**
@file solver.c
\brief Ficheiro dedicado à análise do espaço de soluções.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "solver.h"
#include <string.h>

// ------------------------------------------------------------------------------

/* Métodos publicos */
ESTADO solve(ESTADO b, long *number_of_solutions);
GTree ana(T e, PositionSelector func);
int cluster(T e, int u, int *cp, int sig);
T baseMap(int numl, int numc, float probB);
int findk(GTree node, GTree *v);
void destroyGTree(GTree node);
void pickG(T e, GTree tr);
void showS(T e);
void writeMap(T e, char *difficulty);
int countUseS(T e);
int getDimension(T e);
void destroyit(T e);
char **convert_external(T e);

/* Métodos privados */
static int verifica(int x, int y, int vectorx, int vectory, int **state, int l, int c);
static int valida(int x, int y, int **e, int maxl, int maxc);
static T makeit(int l, int c);
static T convert_internal(ESTADO a);
static int findWay(T current, GTree node, int flag);
static int numberOfsolutions(GTree node);
static int recAna(int n, T current, GTree *node, PositionSelector func);
static int change_aim(int cd[2], int lin, int col);
static void pick(T e, int i, int j, int value);

// ------------------------------------------------------------------------------

/**
\brief Macro que verifica se valores se encontram dentro de um intervalo

@param x valor 1 a verificar
@param y valor 2 a verificar
@param num_lins valor superior do intervalo para x
@param num_cols valor superior do intervalo para y

@returns 1 se x estiver entre 0 e num_lins e y entre 0 e num_cols, caso contrário devolve 0
*/
#define testall(x, y, num_lins, num_cols) ((y < num_lins) && (x < num_cols) && (x >= 0) && (y >= 0))

/**
\brief Macro que verifica se dua pecas estão dentro do tabuleiro

@param x1 linha da primeira peca
@param y1 coluna da primeira peca
@param x2 linha da segunda peca
@param y2 coluna da segunda peca
@param num_lins valor superior do intervalo para x
@param num_cols valor superior do intervalo para y

@returns 1 se ambas estiverem dentro do tabuleiro, 0 caso contrário
*/
#define contain(x1, y1, x2, y2, num_lins, num_cols) (testall(x1, y1, num_lins, num_cols) && testall(x2, y2, num_lins, num_cols))

/**
\brief Macro que verifica se duas pecas são iguais e diferentes de VAZIO e BLOQUEADA

@param x1 linha da primeira peca
@param y1 coluna da primeira peca
@param x2 linha da segunda peca
@param y2 coluna da segunda peca
@param state estado onde se encontram as pecas

@returns 1 se as duas pecas são iguais e diferentes de VAZIO e BLOQUEADA, 0 caso contrário

*/
#define countval(x1, y1, x2, y2, state) (equal(x1, y1, x2, y2, state) && ignore(x1, y1, state))

/**
\brief Macro para verificar se uma posição é diferente de VAZIA e de BLOQUEADA

@param x linha da posição
@param y coluna da posição
@param state estado onde se encontra a peca

@returns 1 se for diferente de VAZIA e de BLOQUEADA, 0 caso contrário
*/
#define ignore(x, y, state) ((acess(x, y, state) != VAZIA) && (acess(x, y, state) != BLOQUEADA))

/**
\brief Macro que devolve o valor de uma posição

@param x linha da posição
@param y coluna da posição
@param state estado onde se encontra a peca

@returns valor da peca na posição (x,y)
*/
#define acess(x, y, state) (state[x][y])

/**
\brief Macro para verificar se duas posições são iguais

@param x1 linha da primeira peca
@param y1 coluna da primeira peca
@param x2 linha da segunda peca
@param y2 coluna da segunda peca
@param state estado onde se encontram as pecas

@returns 1 forem iguais, o se forem diferentes
*/
#define equal(x1, y1, x2, y2, state) (acess(x1, y1, state) == acess(x2, y2, state))

/**
\brief Macro para cálculo do valor máximo

@param X valor 1
@param Y valor 2

@returns o maior entre X e Y
*/
#define MAX(X, Y) ((X) > (Y)) ? (X) : (Y)

//--(1) Declarações  ------------------------------------------------------------------------------------

/**
\brief Declaração do estado interno
*/
typedef struct state
{
    int **grelha; /**< Grelha de valores das Peças.*/
    int **table;  /**< Tabela de valores das posições alteráveis.*/
    int num_cols; /**< Número de colunas.*/
    int num_lins; /**< Número de linhas.*/
    int sqr;      /**< Tamanho do lado do tabuleiro.*/
} * T;

/**
\brief Estrutura que representa o espaço de soluções de um dado Estado.
*/
typedef struct gtree
{
    int side; /**< Tipo de nodo da gtree. */

    /**
    \brief Estrutura que contem apontadores para os filhos do nodo.
    */
    struct pecas /***< Estrutura que contem os filhos. */
    {
        struct gtree *X; /**< Apontador para um filho do nodo.*/
        struct gtree *O; /**< Apontador para um filho do nodo.*/
    } no;                /**< Estrutura para armazenar filhos do nodo. */

    int value;            /**< Valor presente na peca.*/
    int i;                /**< Linha da peca.*/
    int j;                /**< Coluna da peca.*/
    struct gtree *parent; /**< Apontador para o nodo anterior na árvore.*/

} * GTree;

/**
\brief Tipos de nodos de uma GTree.
*/
typedef enum
{
    ROAD,   /**< Caminho com várias soluções.*/
    TRAIL,  /**< Caminnho para uma solução única.*/
    TRESURE /**< Solução.*/
} HUNT;

//--(2) Verificações ------------------------------------------------------------------------------------

/* Implementação de métodos privados. */

/**
\brief 
    Verifica se no sentido de um dado vetor em questão tem menos de 3 em linhas.
    @param x abcissa da jogada.
    @param y ordenada da jogada.
    @param vectorx projeçao nas abcissas do vetor direção.
    @param vectory projeçao nas ordenas do vetor direção.
    @param state matrix do tabuleiro.
    @param l numero de linhas da matrix tabuleiro.
    @param c numero de colunas da matrix tabuleiro.

    @returns O numero de peças iguais numa determinada direção.

    @see countval
    @see contain
*/

static int verifica(int x, int y, int vectorx, int vectory, int **state, int l, int c)
{
    int tmpX, tmpY, count = 1;

    tmpX = x;
    tmpY = y;

    while (contain(x, y, x + vectorx, y + vectory, l, c) && countval(x, y, x + vectorx, y + vectory, state))
    {
        ++count;
        y += vectory;
        x += vectorx;
    }

    x = tmpX;
    y = tmpY;

    while (contain(x, y, x - vectorx, y - vectory, l, c) && countval(x, y, x - vectorx, y - vectory, state))
    {
        ++count;
        y -= vectory;
        x -= vectorx;
    }

    return count;
}

/**
\brief
    Válida se uma dada jogada será valida.
    @param x Abcissa da jogada.
    @param y Ordenada da jogada.
    @param e Matrix do tabuleiro.
    @param maxl Número de linhas da matrix tabuleiro.
    @param maxc Número de colunas da matrix tabuleiro.

    @return Booleano que indica se a peça indicada é valida.
    
    @see verifica 
*/
static int valida(int x, int y, int **e, int maxl, int maxc)
{
    int v1, v2, v3, v4;

    // Verifica Horizontal;
    v1 = verifica(x, y, 1, 0, e, maxl, maxc);
    // Verifica Vertical;
    v2 = verifica(x, y, 0, 1, e, maxl, maxc);
    // Verifica bissetriz dos quadrantes impares
    v3 = verifica(x, y, 1, -1, e, maxl, maxc);
    // Verifica bissetriz dos quadrantes pares
    v4 = verifica(x, y, 1, 1, e, maxl, maxc);

    return (v1 < 3 && v2 < 3 && v3 < 3 && v4 < 3);
}

//--(4) Handlers de estado -----------------------------------------------------------------------------

/**
\brief
    Construtor do elemento T(estado interno).
    @param l numero de linhas deste elemento.
    @param c numero de colunas deste elemento.

    @returns instância do tipo T;

*/
static T makeit(int l, int c)
{
    int i, j, n = MAX(l, c);
    T e = (T)malloc(sizeof(struct state));

    e->grelha = malloc(n * sizeof(int *));
    e->table = malloc(n * sizeof(int *));

    e->num_lins = l;
    e->num_cols = c;
    e->sqr = n;

    for (i = 0; i < n; i++)
    {
        e->grelha[i] = malloc(n * sizeof(int));
        e->table[i] = malloc(n * sizeof(int));
        for (j = 0; j < n; j++)
        {
            e->grelha[i][j] = VAZIA;

            if (i < l && j < c)
            {
                e->table[i][j] = 1;
            }
            else
            {
                e->grelha[i][j] = BLOQUEADA;
                e->table[i][j] = 0;
            }
        }
    }

    return e;
}

/**
\brief
    Converte o estado externo num estado interno.
    @param a é um estado Externo que se pretende converter para um estado interno.
    
    @returns estado interno. 

    @see makeit
    @see getE_lins
    @see getE_cols
    @see getE_elem
    @see pick
*/
static T convert_internal(ESTADO a)
{
    int i, j;
    T e = makeit(getE_lins(a), getE_cols(a));

    for (i = 0; i < getE_lins(a); i++)
        for (j = 0; j < getE_cols(a); j++)
        {
            switch (getE_elem(a, i, j))
            {
            case BLOQUEADA:
                pick(e, i, j, BLOQUEADA);
                break;
            case FIXO_X:
                pick(e, i, j, SOL_X);
                break;
            case FIXO_O:
                pick(e, i, j, SOL_O);
                break;
            default:
                break;
            }
        }

    return e;
}

/**
\brief Converte o estado interno numa grelha do estado externo.

@param e é um estado Interno que se pretende converter para um estado externo.
    
@returns grelha do estado Externo.
*/
char **convert_external(T e)
{

    char **final = (char **)malloc(sizeof(char *) * e->num_lins);
    int i, j;

    for (i = 0; i < e->num_lins; i++)
    {
        final[i] = (char *)malloc(sizeof(char) * e->num_cols);
        for (j = 0; j < e->num_cols; j++)
        {
            switch (e->grelha[i][j])
            {
            case SOL_X:
                final[i][j] = FIXO_X;
                break;
            case SOL_O:
                final[i][j] = FIXO_O;
                break;
            default:
                final[i][j] = (char)e->grelha[i][j];
                break;
            }
        }
    }
    return final;
}

/**
\brief Destroi uma instaância do estado interno.

@param e estado a ser eliminado.

@see MAX
*/
void destroyit(T e)
{
    int i, n = MAX(e->num_lins, e->num_cols);

    for (i = 0; i < n; i++)
    {
        free(e->grelha[i]);
        free(e->table[i]);
    }
    free(e->grelha);
    free(e->table);

    free(e);
}

/**
\brief Destroi uma instância da Árvore que representa o espaço de soluções.

@param node árvore a ser eliminada.
*/
void destroyGTree(GTree node)
{

    if (node)
    {
        if (node->no.O)
        {
            destroyGTree(node->no.O);
        }

        if (node->no.X)
        {

            destroyGTree(node->no.X);
        }

        free(node);
    }
}

/**
\brief
    Altera o estado interno por forma a conter o elemento que o nodo contêm.
    @param e estado a ser alterado.
    @param tr nodo que se pretende incorporar

    @see pick
*/
void pickG(T e, GTree tr)
{
    pick(e, tr->i, tr->j, tr->value);
}

/**
\brief Escreve um estado interno no ecra.

@param e estado a ser impresso.
*/
void showS(T e)
{
    int i, j;
    printf("%d %d\n", e->num_lins, e->num_cols);
    for (i = 0; i < e->num_lins; i++)
    {
        for (j = 0; j < e->num_cols; j++)
        {
            switch (e->grelha[i][j])
            {
            case SOL_X:
                printf("X");
                break;
            case SOL_O:
                printf("O");
                break;
            case VAZIA:
                printf(".");
                break;
            case BLOQUEADA:
                printf("#");
                break;
            }
        }
        printf("\n");
    }
}

/**
\brief Escreve um mapa em ficheiro.

@param e Estado que irá escrever.
@param difficulty Dificuldade do mapa.
*/
void writeMap(T e, char *difficulty)
{
    int i, j;
    char *file = (char *)malloc(sizeof(char) * (strlen(RANDOMDIR) + strlen(difficulty) + strlen(".txt") + 1));
    strcpy(file, RANDOMDIR);
    strcat(file, difficulty);
    strcat(file, ".txt");
    FILE *fp = fopen(file, "w");
    fprintf(fp, "%d %d\n", e->num_lins, e->num_cols);
    for (i = 0; i < e->num_lins; i++)
    {
        for (j = 0; j < e->num_cols; j++)
        {
            switch (e->grelha[i][j])
            {
            case SOL_X:
                fputc('X', fp);
                break;
            case SOL_O:
                fputc('O', fp);
                break;
            case VAZIA:
                fputc('.', fp);
                break;
            case BLOQUEADA:
                fputc('#', fp);
                break;
            }
        }
        if (i != e->num_lins - 1)
            fputc('\n', fp);
    }
}

/**
\brief
    Consulta o numero de casas que podem ser alterados na grelha do estado interno
    @param e estado a consultar.

    @returns Numero de casas que podem ser alterados na grelha.
*/
int countUseS(T e)
{
    int s = 0, i, j;

    for (i = 0; i < e->num_lins; i++)
        for (j = 0; j < e->num_cols; j++)
            s += e->table[i][j];

    return s;
}

/**
\brief
    Consulta o numero de casas na grelha do estado interno
    @param e estado a consultar.

    @returns Numero de casas na grelha.
*/
int getDimension(T e)
{
    return e->num_cols * e->num_lins;
}
//----------------------------------------------------------------------------------------

/**
\brief 
    Esta função define qual a próxima coordenada.
    @param cd vetor com as coordenadas do presente elemento.
    @param lin numero de linhas.
    @param col numero de colunas

    @returns 1 no caso de sucesso e 0 caso contrário.

*/
static int change_aim(int cd[2], int lin, int col)
{
    int min = lin < col ? lin : col;
    int r;

    r = !(cd[0] == cd[1]);

    if (r) // não é igual.
    {
        if (cd[0] > cd[1])
        { //horizontal;
            if (cd[0] >= col)
            {
                // já está a escrever abaixo do quadrado.
                if (cd[1] + 1 == col)
                { //está no limite direito.
                    if (cd[0] + 1 == lin)
                    {
                        r = 0; //acabou a festa.
                    }
                    else
                    { // muda para uma nova linha.
                        cd[0]++;
                        cd[1] = 0;
                    }
                }
                else
                {
                    cd[1]++;
                }
            }
            else
            {
                cd[1]++;
            }
        }
        else
        { //vertical.
            if (cd[0] + 1 == cd[1])
            { //começa a horizontal.
                cd[0] = cd[1];
                cd[1] = 0;
            }
            else
            {
                cd[0]++;
            }
        }
    }
    else
    { //é igual.

        if (cd[0] + 1 == min || cd[1] + 1 == min)
        { // está na borda.
            r = 0;
        }
        else
        { // mover para o novo quadrante.
            cd[0] = 0;
            cd[1]++;

            r = 1;
        }
    }
    return r;
}

/**
\brief 
    Esta função altera uma das entradas de elemento.
    @param e Elemento que se pertende alterar.
    @param i linha onde se pertende fazer a adição.
    @param j coluna onde se pertende fazer a adição.
    @param value valor a adicionar.

*/
static void pick(T e, int i, int j, int value)
{
    e->table[i][j] = 0;
    e->grelha[i][j] = value;
}

/**
\brief
    Resolve um dado ESTADO.
    @param a Estado que se pertende resolver.
    @param number_of_solutions variavel que contêm o endereço da variavel que se ira colocar o numeor de soluções do mapa.
    
    @returns ESTADO externo resolvido.(se tiver solução)
    
    @see destroyit
    @see conver_internal
    @see findSol
    @see ana
    @see getE_lins
    @see getE_cols
    @see setE_elem
    @see numberOfsolutions
    @see destroyGTree
*/
ESTADO solve(ESTADO a, long *number_of_solutions)
{
    T e = convert_internal(a);
    GTree tr = ana(e, cluster);
    int i, j;
    char val;

    if (findWay(e, tr, 1))
    {
        for (i = 0; i < getE_lins(a); i++)
            for (j = 0; j < getE_cols(a); j++)
            {
                val = getE_elem(a, i, j);
                if (val != BLOQUEADA && val != FIXO_O && val != FIXO_X)
                    setE_elem(a, i, j, e->grelha[i][j]);
            }
    }

    *number_of_solutions = numberOfsolutions(tr);
    destroyit(e);
    destroyGTree(tr);

    return a;
}

/**
\brief
    Atribui a current uma das soluções (escolhida aleatóriamente) da árvore dada.
    @param current Estado que se pertende resolver.
    @param node Raiz da árvore que se pretende selecionar uma solução.
    @param flag Determina se os valores da grelha são gravados como fixos(0) ou não(1).
    
    @returns ESTADO externo resolvido.(se tiver solução)
    
    @see pick
*/
static int findWay(T current, GTree node, int flag)
{
    int i, j, went;
    GTree aux;

    if (node)
    {

        i = node->i;
        j = node->j;
        if (node->i != -1)
        {
            if (flag)
                current->grelha[i][j] = node->value;
            else
                pick(current, i, j, node->value);
        }

        if (node->side == TRESURE)
            return 1;

        if (node->no.X && node->no.O)
        {
            if (node->no.X->side == TRAIL)
            {
                aux = node->no.X;
            }
            else if (node->no.O->side == TRAIL)
            {
                aux = node->no.O;
            }
            else
            {
                if (flag)
                {
                    aux = node->no.X ? node->no.X : node->no.O;
                }
                else
                {
                    srand(time(NULL) + clock() + rand());
		            went = (rand() % 2);   
                    aux  = went ? node->no.X : node->no.O;
                }
            }
        }
        else
        {
            aux = node->no.X ? node->no.X : node->no.O;
        }

        return findWay(current, aux, flag);
    }
    return 0;
}

/**
\brief
    Calcula o numero de soluções da árvore recebida.    
    @param node Raiz da árvore de soluções.
    
    @returns numero de soluções da árvore indicada.
    
*/
static int numberOfsolutions(GTree node)
{

    int s = 0;

    if (node)
    {
        if (node->side == TRESURE)
        {
            return 1;
        }

        if (node->no.O)
        {
            s += numberOfsolutions(node->no.O);
        }

        if (node->no.X)
        {

            s += numberOfsolutions(node->no.X);
        }

        return s;
    }
    else
    {
        return 0;
    }
}

/**
\brief
    Esta função implementa uma Position Selector.
    Este Position Selector foi calibrado para promover clustering para obter estados invalidos o mais rapidamente possivel.    
    @param e estado interno.
    @param sig sinal que indica se a função está no modo de criação ou remoção.
    @param u numero peças já percorridas
    @param cp receberá as proximas coordenadas

    @returns no caso de sucesso 1 caso contrário 0.
    
    @see PositionSelector
    @see change_aim
*/
int cluster(T e, int u, int *cp, int sig)
{
    int flag, n = u;
    if (sig)
    {
        if (!n && e->table[0][0])
        {
            n++;
            cp[0] = cp[1] = 0;
            flag = 1;
        }
        else
        {
            if (!n)
                cp[0] = cp[1] = 0;

            do
            {
                flag = change_aim(cp, e->sqr, e->sqr);
                n++;
            } while (flag && !e->table[cp[0]][cp[1]]); //enquanto for menor que sqr*Sqr
        }
    }
    return n;
}

/**
\brief
    Esta função, um anamorfimo, cria árvores de soluções do estado indicado conforme a função recebida.
    @param e estado interno.
    @param func função que determina a próxima posição a ser inspecionada.

    @returns árvore de soluções.
    
    @see PositionSelector
    @see rec_Ana
*/
GTree ana(T e, PositionSelector func)
{
    T target = e;
    GTree tree;

    tree = (GTree)malloc(sizeof(struct gtree));
    tree->parent = NULL;
    tree->value = -1;
    tree->i = -1;
    tree->j = -1;
    recAna(0, target, &tree, func);
    return tree;
}

/**
\brief
    Esta função vai recursivamente inspecionando elementos e desenvolvendo a arvore recebida.
    @param node endereço do árvore a desenvolver.
    @param current estado interno.
    @param func função que determina a próxima posição a ser inspecionada.
    @param n numero de elementos já inspecionados.

    @returns árvore de soluções.
    
    @see PositionSelector
    @see valida
*/
static int recAna(int n, T current, GTree *node, PositionSelector func)
{
    int cp[2], l, r, ans = TRAIL, sqr;

    GTree aux;

    sqr = current->sqr;

    (*node)->no.O = NULL;
    (*node)->no.X = NULL;

    cp[0] = (*node)->i;
    cp[1] = (*node)->j;

    if ((cp[0] != -1) && !valida(cp[0], cp[1], current->grelha, sqr, sqr))
    { /* não é valida*/
        free(*node);
        *node = NULL;
        ans = -1;
    }
    else if (n == sqr * sqr)
    {
        (*node)->side = ans = TRESURE;
    }
    else
    {
        n = func(current, n, cp, 1); /*seleciona a proxima posição*/

        if (n > sqr * sqr)
        {
            return recAna(sqr * sqr, current, node, func);
        }

        if (cp[0] < current->num_lins && cp[1] < current->num_cols)
        {
            /* Faz X */
            current->grelha[cp[0]][cp[1]] = SOL_X; //<----
            (*node)->no.X = (GTree)malloc(sizeof(struct gtree));
            (*node)->no.X->value = SOL_X;
            aux = (*node)->no.X;
            aux->i = cp[0];
            aux->j = cp[1];
            aux->parent = (*node);

            l = recAna(n, current, &((*node)->no.X), func);

            /*Faz O*/
            current->grelha[cp[0]][cp[1]] = SOL_O; //<----
            (*node)->no.O = (GTree)malloc(sizeof(struct gtree));
            (*node)->no.O->value = SOL_O;
            aux = (*node)->no.O;
            aux->i = cp[0];
            aux->j = cp[1];
            aux->parent = (*node);

            r = recAna(n, current, &((*node)->no.O), func);

            /* reverte o que foi feito em corrent */
            current->grelha[cp[0]][cp[1]] = VAZIA;
            func(current, n, cp, 0);

            if (l == -1 && r == -1)
            {
                free(*node);
                *node = NULL;
                ans = -1;
            }
            else if ((l == -1 && (r == TRESURE || r == TRAIL)) || ((l == TRESURE || l == TRAIL) && r == -1))
            {
                (*node)->side = TRAIL;
                ans = TRAIL;
            }
            else
            {
                (*node)->side = ROAD;
                ans = ROAD;
            }
        }
    }
    return ans;
}

/**
\brief
    Esta função cria um array de nodos com as peças que levam a uma solução.
    @param node árvore com uma unica solução.
    @param v array a construir.

    @returns árvore de soluções.
    
*/
int findk(GTree node, GTree *v)
{
    GTree aux;
    int r;
    if (node)
    {
	r = (node->i != -1);
        v[0] = (GTree)malloc(sizeof(struct gtree));

        v[0]->i = node->i;
        v[0]->j = node->j;
        v[0]->value = node->value;

        if (node->side == TRESURE)
            return 1;

        aux = node->no.O ? node->no.O : node->no.X;
        if(!aux)
            return 0;

        if (r)
            return findk(aux, &v[1]);
        else
            return findk(aux, v);
    }
    return 0;
}

/**
\brief
    Esta função cria um estado interno aleatório com o minimo de peças preenchidas possível.
    @param numl numero de linhas do estado a criar.
    @param numc numero de coluna do estado a criar.
    @param probB probabilidade de uma dada peças estar Bloqueada.

    @returns estado interno.
    
    @see makeit
    @see pick
    @see ana
    @see findWay
    @see numberOfSolutions
    @see destroyit
    @see destroyGTree
*/
T baseMap(int numl, int numc, float probB)
{
    GTree tr, mapnode;
    T e = makeit(numl, numc);
    int i, j, **cu, val;

    srand(time(NULL) + clock() + rand());

    for (i = 0; i < e->num_lins; i++)
        for (j = 0; j < e->num_cols; j++)
        {
            val = (int)(probB * 100);
            if ((rand() % 100) < val)
                pick(e, i, j, BLOQUEADA);
        }

    cu = (int **)malloc(sizeof(int *) * e->num_lins);

    for (i = 0; i < e->num_lins; i++)
    {
        cu[i] = (int *)malloc(sizeof(int) * e->num_cols);
        for (j = 0; j < e->num_cols; j++)
            cu[i][j] = e->table[i][j];
    }

    tr = ana(e, cluster);

    if (!tr)
    {
        perror(" Erro ao gerar a os bloqueados \n");
        exit(1);
    }

    do
    {
        findWay(e, tr, 0);

        for (i = 0; i < e->num_lins; i++)
        {
            for (j = 0; j < e->num_cols; j++)
            {
                if (cu[i][j])
                {
                    e->table[i][j] = 1;
                    val = e->grelha[i][j];
                    e->grelha[i][j] = VAZIA;
                    mapnode = ana(e, cluster);

                    if (!mapnode)
                    {
                        perror(" Erro ao deconstruir solução \n");
                        exit(1);
                    }
                    if (mapnode->side == ROAD)
                    {
                        e->table[i][j] = 0;
                        e->grelha[i][j] = val;
                    }
                    destroyGTree(mapnode);
                }
            }
        }
        val = MAX(e->num_cols, e->num_cols);

        for (i = 0; val && (i < e->num_lins); i++)
            for (j = 0; val && (j < e->num_cols); j++)
                val -= e->table[i][j];

    } while (val);

    for (i = 0; i < e->num_lins; i++)
        free(cu[i]);
    free(cu);

    destroyGTree(tr);

    return e;
}
