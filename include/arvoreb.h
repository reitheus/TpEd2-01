#ifndef ARVOREB_H
#define ARVOREB_H

#include "../include/item.h"
#define M 10// Ordem da árvore

//criando variável ponteiro do tipo página
typedef struct Pagina* Apontadores;

typedef struct Pagina {
    short quant;// quantidades de itens nos vetores 
    Item pai[M*M];// vetor de itens na página
    Apontadores filhos[(M*M) + 1]; // vetor de apontadores 
} Pagina; 

void inicializa(Apontadores* Arvore);

//void arvoreB(Analise*, FILE*, TipoRegistro *, int , int, int);

void pesquisar(Item* x, Apontadores Arvore, Analise*);
void insere(Item x, Apontadores* Arvore, Analise*); 
void ins(Item x,Apontadores Arvore, short *cresceu, Item* registroretorno, Apontadores* ApRetorno, Analise*);
void inserenaPagina(Apontadores Arvore, Item dados, Apontadores ApDir, Analise*);
void imprimir(Item x); // impressão das informações da chave encontrada. 

#endif
