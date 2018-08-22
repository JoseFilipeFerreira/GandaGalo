/**
*@file stack.c
\brief Módulo para operar sobre STACK.
*/
#include "stack.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------

/* Métodos públicos */
STACK initS();
void destroyS(STACK s);
int isEmpty(STACK s);
void push(STACK s, int i, int j, char val);
int pop(STACK s, int *i, int *j, char *flag);
int peek(STACK s);
int canGetAnc(STACK s);
int makeAnc(STACK s);
char *completeS(STACK s);
void addToTail(STACK s, char i, char j, char val, char check);

// ------------------------------------------------------------------------------

/**
\brief Estrutura para o valor a armazenar
*/
typedef struct record
{
    int i;      /**< abcissa a armazenar*/
    int j;      /**< ordenada a armazenar*/
    char val;   /**< valor */
    char check; /**< valor indicativo da âncora*/
} RECORD;

/**
\brief Estrutura para a lista ligada
*/
typedef struct lista
{
    RECORD valor;       /**< valor a armazenar*/
    struct lista *prox; /**< apontador para o próximo elemento*/

} * LRec;

/**
\brief Declaração da Estrutura principal
*/
typedef struct stack
{
    int size;     /**< tamanho da stack*/
    LRec *inicio; /**< apontador para o inicio da stack*/
    LRec *fim;    /**< apontador para o fim da stack*/
} * STACK;

// ------------------------------------------------------------------------------

/* Construtores */
/* Algebra */
/**
\brief 
    Cria uma instância destes módulo.
@returns @c STACK criada.
*/
STACK initS()
{ 
    STACK s = (STACK)malloc(sizeof(struct stack));
    s->size = 0;
    s->inicio = (LRec *)malloc(sizeof(LRec));
    s->fim = (LRec *)malloc(sizeof(LRec));
    *(s->inicio) = NULL;
    *(s->fim) = NULL;
    return s;
}
/* coAlgebra */
/**
\brief 
    Destrói uma instância destes módulo.
    @param s Instância de Stack a ser destruida.
*/
void destroyS(STACK s)
{
    LRec delete = NULL, cur = *(s->inicio);
    for (cur = *(s->inicio); cur; cur = cur->prox)
    {
        if (delete)
            free(delete);

        delete = cur;
    }
    if (delete)
        free(delete);
    /*
    if(s->fim)
        free(s->fim);
    if(s->inicio)
        free(s->inicio);*/
    free(s);
}

/* Operações */
/**
\brief
    Verifica se a stack está vazia.
    @param s Instância de Stack que se pertender verificar se está vazia.
*/
int isEmpty(STACK s)
{
    return (s->size == 0);
}

/**
\brief
    Põe um elemento no topo da stack.
    @param s Instância de Stack que se pertende adicionar uma entrada.
    @param i abcissas do elemento a guardar.
    @param j ordenada do elemento a guardar.
    @param val valor a guardar. 
*/
void push(STACK s, int i, int j, char val)
{
    LRec newnode = (LRec)malloc(sizeof(struct lista));
    newnode->valor.i = i;
    newnode->valor.j = j;
    newnode->valor.val = val;
    newnode->valor.check = 0;
    newnode->prox = *(s->inicio);
    *(s->inicio) = newnode;
    s->size++;
}
/**
\brief
    Tira um elemento no topo da stack.
    @param s Instância de Stack que se pertende remover uma entrada.
    @param i abcissas do elemento que se retirou.
    @param j ordenada do elemento que se retirou.
    @param flag valor a guardar.
    
    @returns @b -1 se a @c STACK for vazia, caso contrário retorna o valor retirado.

    @see record::val
*/
int pop(STACK s, int *i, int *j, char *flag)
{
    int r = -1;
    if (*(s->inicio) != NULL)
    {
        LRec aux = *(s->inicio);
        s->inicio = &((*(s->inicio))->prox);
        *i = aux->valor.i;
        *j = aux->valor.j;
        *flag = aux->valor.check;
        r = (int)aux->valor.val;
        s->size--;
    }
    else
        *flag = -1;
    return r;
}

/**
\brief
    Consulta o elemento no topo da stack e alterar este para 0.

    @param s Instância de Stack que se pertende consultar

    @returns @b -1 se a @c STACK for vazia, caso contrário devolve o valor indicativo de âncora no topo da @c STACK . 

    @see record::check
*/
int peek(STACK s)
{
    int r = -1;
    if (*(s->inicio) != NULL)
    {
        r = (*(s->inicio))->valor.check;
        (*(s->inicio))->valor.check = 0;
    }
    return r;
}

/**
\brief
    Verifica se a Stack possui uma âncora.
    @param s Instância de Stack que se pertende consultar

    @returns Devolve 1 se existir uma âncora na @c STACK, 0 caso contrário. 

    @see record::check
*/
int canGetAnc(STACK s)
{
    int r = 0;
    LRec aux = *(s->inicio);
    while (aux != NULL && !r)
    {
        if (aux->valor.check == 1)
            r = 1;
        aux = aux->prox;
    }
    return r;
}

/**
\brief
    Coloca, se possivél, uma âncora
    @param s A stack que onde irá ser guardada a âncora.

    @returns Devolve 1 se foi possivél colocar uma âncora, 0 caso contrário.

    @see record::check 
*/
int makeAnc(STACK s)
{
    int r = 1;
    if (*(s->inicio) == NULL)
        r = 0;
    else
        (*(s->inicio))->valor.check = 1;
    return r;
}

/**
\brief Recebe um conjunto de valores e adiciona à cauda da lista em tempo constante.

@param s stack onde irá colocar o elemento.
@param i Valor a adicionar.
@param j Valor a adicionar.
@param val Valor a adicionar.
@param check Valor a adicionar.
*/
void addToTail(STACK s, char i, char j, char val, char check)
{
    LRec newnode = (LRec)malloc(sizeof(struct lista));
    newnode->valor.i = i;
    newnode->valor.j = j;
    newnode->valor.val = val;
    newnode->valor.check = check;
    newnode->prox = NULL;

    s->size++;

    if (*(s->inicio) == NULL)
    {
        *(s->inicio) = newnode;
        s->fim = &(newnode->prox);
    }
    else
    {
        *(s->fim) = newnode;
        s->fim = &((*(s->fim))->prox);
    }
}

/**
\brief Cria uma string correspondente aos elementos de uma @c STACK , aplicando @c pop a todos os elementos.

@param s A @c STACK que irá passar a string.

@returns A string correspondente à @c STACK .

@see pop
*/
char *completeS(STACK s)
{
    int r, i, j;
    char check, *str, *aux;
    str = malloc((s->size+1) * (sizeof(char) * 600));
    aux = malloc(sizeof(char) * 600);
    
    str[0] = '\0';
    aux[0] = '\0';
    
    while ((r = pop(s, &i, &j, &check)) != -1)
    {
        sprintf(aux, " (%d,%d,%d,%d)", i, j, r, check);
        strcat(str, aux);
    }
    destroyS(s);
    free(aux);
    return str;
}
