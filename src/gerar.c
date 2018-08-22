/**
@file gerar.c
\brief Ficheiro de geração de tabuleiro aleatório.
*/

#include <time.h>
#include "solver.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------

/**
\brief Macro destinada a definir o limite de dimensões.

@param num Valor a comparar.
*/
#define lim(num) ((num>0) && (num <= MAX_GRID))

// ------------------------------------------------------------------------------

/**
\brief Função main para gerar mapas random.
*/
int main(int argc, char *argv[])
{
    T e;
    int i, s, n, indx,dim;
    float fr,pb;
    GTree ol, *p;

    if (argc > 3)
    {
        if (!(lim(atoi(argv[2])) && lim(atoi(argv[3])) ) ){
            perror(" Dimensões inválidas.");
            return 0;
        }
        
        switch(atoi(argv[1])){
            case 2: pb = 0.11;fr=0.3; break;
            case 1: pb = 0.04;fr=0.4; break;
            default: perror(" Dificuldade pode apenas ter o valor 1 ou 2 \n"); return 0;
        }
        srand(time(NULL) + clock() + rand());
        e = baseMap(atoi(argv[2]), atoi(argv[3]), pb);
        s = countUseS(e);

        p = calloc(s, sizeof(GTree) * s);
        ol = ana(e, cluster);
        findk(ol, p);
        destroyGTree(ol);

        for (s = 0; p[s] ; s++);
		dim = getDimension(e);
        n = (int)(((float)dim) * fr);// n :: o numero de casas que tem de estar por preencher. 
        for (i = (dim -s); i < n; i++)
        {
            indx = rand() % s;
			pickG(e,p[indx]);
            free(p[indx]);
            p[indx] = p[s - 1];
            s--;
        }

        showS(e);
        writeMap(e,argv[1]);

        for (i = 0; i<s ; i++)
            free(p[i]);
        
        free(p);

        destroyit(e);
    }
    else
    {
        perror(" Numero de argumentos insuficientes \n");
    }
    return 0;
}
