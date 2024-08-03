#ifndef ARVOREBIN_H
#define ARVOREBIN_H

#include "../include/item.h"


typedef struct registro{
    Item key;
    int dir;
    int esq;
}Registro;

void imprimeDado(Analise* ,Registro* );

void arvoreBinaria(FILE* ,long int* ,int* ,DadosPesquisa* ,Item*);

void criaArvoreBin(FILE* ,FILE* ,long int* ,Analise* );

bool pesquisaArvoreBin(FILE* ,int ,Analise* ,Registro* );


#endif // arvorebin.h