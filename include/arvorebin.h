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

void imprimeAbin(Item);
void criaArvoreBin(FILE* ,FILE* ,long* ,Analise* );
bool pesquisaArvoreBin(FILE* ,long* ,int* ,int ,Analise );


#endif // arvorebin.h