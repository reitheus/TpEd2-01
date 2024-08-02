#ifndef ARVOREBEST_H
#define ARVOREBEST_H

#include "../include/item.h"


#define M 10// Ordem da árvore

//criando variável ponteiro do tipo página
typedef struct Pagina* Apontadores;

typedef struct Pagina {
    short quant;// quantidades de itens nos vetores 
    Item pai[M*M];// vetor de itens 
    Apontadores filhos[(M*M) + 1]; // vetor de apontadores 
} Pagina; 

void inicializa(Apontadores Arvore);
void pesquisar(Item* x, Apontadores Arvore);
void insere(Item x, Apontadores* Arvore); 
void ins(Item x,Apontadores Arvore, short *cresceu, Item* registroretorno, Apontadores* ApRetorno);
void inserenaPagina(Apontadores Arvore, Item dados, Apontadores ApDir);
void imprimir(Item x);

#endif// arvorebEst.h
