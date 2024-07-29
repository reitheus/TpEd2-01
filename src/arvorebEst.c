#include "../include/arvorebEst.h" 

typedef struct TipoPagina{
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            TipoChave ri[2];
            TipoApontador pi [2 + 1];
        }U0;
        struct {
            int ne;
            ArvoreBest re[2];
        }U1;
        
    }UU;
    
}TipoPagina;

void PesquisaEst(ArvoreBest *x, TipoApontador *Ap){
    int i;
    TipoApontador Pag;
    Pag = *Ap;
    if((*Ap)->Pt == Interna){
        i=1;
        while (i < Pag->UU.U0.ni && x->Chave > Pag->UU.U0.ri[i-1])i++;
        if(x->Chave < Pag->UU.U0.ri[i-1]){
            PesquisaEst(x,&Pag->UU.U0.pi[i-1]);
        }else PesquisaEst(x,&Pag->UU.U0.pi[i]);
        return;
    }
    i = 1;
    while (i < Pag->UU.U1.ne && x->Chave > Pag->UU.U1.re[i-1].Chave)
        i++;
    if(x->Chave == Pag->UU.U1.re[i-1].Chave){
        *x=Pag->UU.U1.re[i-1];
    }else printf("TipoRegistro não está presente na arvore\n");
    
}