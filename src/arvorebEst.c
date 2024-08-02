#include "../include/arvorebEst.h" 

#include <stdio.h>
#include <stdbool.h>

#define M 10// Ordem da árvore

void inicializa(Apontadores Arvore){
    Arvore = NULL; 
}

void inserenaPagina(Apontadores Arvore, Item dados, Apontadores ApDir){
    
    short Nhaposicao;
    int k;
    k = Arvore->quant; 
    Nhaposicao = (k > 0);

    While(Nhaposicao){

        if(registro.chave >= Arvore->pai[k-1].chave){
            Nhaposicao = false;
            break;
        }
        
        Arvore->pai[k] = Arvore->pai[k-1];
        Arvore->filhos[k+1] = Arvore->filhos[k];
        k--;
        if(K < 1)Nhaposicao = false;
    }

    Arvore->pai[k] = registro;
    Arvore->filhos[k+1] = ApDir;
    Arvore->quant++;
}

void ins(Item x,Apontadores Arvore, short *cresceu, Item* registroretorno, Apontadores* ApRetorno){

    long i = 1;
    long j;

    Apontadores ApTemp;
    
    if(Arvore == NULL){
        *cresceu = true;
        (*registroretorno) = x;
        (*ApRetorno) = NULL;
        return;
    }
    while(i < Arvore->quant && x.chave > Arvore->pai[i-1].chave) i++;
    
    if(x.chave == Arvore->pai[i-1].chave){
        printf("Registro existente");
        *cresceu = false;
        return;
    }

    if(x.chave < Arvore->pai[i-1].chave) i--;
    ins(x,Arvore->filhos[i], cresceu, registroretorno, ApRetorno);
    if(!*cresceu)
        return;
    if(Arvore->quant < M*M){
        inserenaPagina(Arvore, *registroretorno, *ApRetorno);
        *cresceu = false;
        return;
    }

    ApTemp = (Apontadores)malloc(sizeof(Pagina));
    ApTemp->quant = 0;
    ApTemp->filhos[0]= NULL;

    if(i < M+1){
        inserenaPagina(ApTemp, Arvore->pai[M*M-1], Arvore->filhos[M*M]);
        Arvore->quant--;
        inserenaPagina(Arvore, *registroretorno, *ApRetorno);
    }

    else
        inserenaPagina(ApTemp, *registroretorno, *ApRetorno);

    for(j = M+2;j <= M*M; j++){

        inserenaPagina(ApTemp, Arvore->pai[j-1],Arvore->filhos[j]);
    
    }

    Arvore->quant = M; 
    ApTemp->filhos[0] = Arvore->filhos[M+1];
    *registroretorno = Arvore->pai[M];
    *ApRetorno = ApTemp;
}


void insere(Item x, Apontadores* Arvore){

    short cresceu;
    Item registroretorno; 
    Pagina* ApRetorno, *ApTemp; 
    ins(x, *Arvore, &cresceu, &registroretorno, &ApRetorno);

    if(cresceu){
 
        ApTemp = (Pagina*) malloc(sizeof(Pagina));// alocando memória
        ApTemp->quant = 1;
        ApTemp->pai[0] = registroretorno;
        ApTemp->filhos[1] = ApRetorno;
        ApTemp->filhos[0] = *Arvore; 
        *Arvore = ApTemp;

    }
}

// imprimir dados da pesquisa , tempo , leituras e as comparações 
void imprimirDados(Item x){

    printf("Chave: %d\n", x.chave);
    printf("Dado1: %ld\n", x.dado1);
    printf("Dado2: %s\n", x.dado2);
    printf("Dado3: %s\n", x.dado3);
           
}


void pesquisar(Item *x, Apontadores Arvore){

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



