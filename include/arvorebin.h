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

void imprimeDado(Analise* ,Registro* );

void arvoreBinaria(FILE* ,long* ,int* ,Analise );

void criaArvoreBin(FILE* ,FILE* ,long* ,Analise* );

bool pesquisaArvoreBin(FILE* ,int ,Analise* ,Registro* );


#endif // arvorebin.h