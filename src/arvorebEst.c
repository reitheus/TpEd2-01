#include "../include/arvorebEst.h"
#include "../include/arvoreb.h"

void pesquisaArvorebEst(Item *x, Apontadores Arvore){

    long i = 1; 
    
    // colocar tempo de execução, numero de quantas leituras e comparações  

    //Se a árvore estiver vazia 
    if(Arvore == NULL){
        long i = 1; 
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    // enquanto i for menor que a quantidade de Itens na arvore e a chave for  
    while(i < Arvore->quant && (x->chave > Arvore-> pai[i-1].chave)) i++;

    // se a chave for igual ao item da árvore
    if(x->chave == Arvore->pai[i-1].chave){
        *x = Arvore->pai[i-1];
        imprimirDados(*x);
        return 1;
    }

    // se a chave for menor que item da arvore retorna chamada recurssiva pelaesquerda 
    if(x->chave < Arvore->pai[i-1].chave)
        pesquisar(x, Arvore->filhos[i-1]);
    
    // se a chave for maior chamada recurssiva para a direita
    else 
        pesquisar(x, Arvore->filhos[i]);
}



