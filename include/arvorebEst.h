#ifndef ARVOREBEST_H
#define ARVOREBEST_H

#include "../include/item.h"
#define M 10

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* Apontador;

typedef struct TipoPagina{
    TipoIntExt Pt;
    union folha{
        struct interno{//interno
            int quant;//ni
            Item pai[2*M];//ri
            Apontador filho[(2*M)+1];//pi
        }U0;
        struct externo{//externo
            int ne;//
            Item re[(2*M)];
        }U1;
    }UU;
    
} TipoPagina;

void liberaPaginaBE(Apontador );
void liberaArvoreBE(Apontador *);
void pesquisaArvorebEst(Item* ,Apontador* ,Analise* ,bool *);
void inserenaPaginaBE(Apontador , Item , Apontador , Analise* );
void insBE(Item , Apontador , short *, Item* , Apontador* , Analise* );
void insereBE(Item , Apontador* , Analise* );
void criaarvoreBE(FILE *, Item *, DadosPesquisa *);

#endif// arvorebEst.h
