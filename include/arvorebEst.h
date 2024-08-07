#ifndef ARVOREBEST_H
#define ARVOREBEST_H

#include "../include/item.h"
#define M 2
typedef int TipoChave;
typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* Apontador;

typedef struct TipoPagina{
    TipoIntExt Pt;
    union folha{
        struct interno{//interno
            int quant;//ni
            TipoChave pai[M];//ri
            Apontador filho[M + 1];//pi
        }U0;
        struct externo{//externo
            int ne;//quantida externo
            Item re[M];//Vetor de itens
            
        }U1;
    }UU;
    
} TipoPagina;

void iniciaArvoreBE(Apontador* );
void liberaPaginaBE(Apontador );
void liberaArvoreBE(Apontador *);
void pesquisaArvorebEst(Item* ,Apontador* ,Analise* ,bool *);
void inserenaPaginaBI(Apontador , TipoChave , Apontador , Analise* );
void inserenaPaginaBE(Apontador , Item , Analise* );
void insBE(Item , Apontador , short *, TipoChave* , Apontador* , Analise* );
void insereBE(Item , Apontador* , Analise* );
void criaarvoreBE(FILE *, Item *, DadosPesquisa *);

#endif// arvorebEst.h
