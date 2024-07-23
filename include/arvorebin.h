#ifndef ARVOREBIN_H
#define ARVOREBIN_H

#include "../include/item.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct registro{
    Item key;
    int dir;
    int esq;
}Registro;

void criaArvore(FILE* ,FILE* ,int );
bool pesquisaArvoreBin(FILE* ,int* ,Item );


#endif // arvorebin.h