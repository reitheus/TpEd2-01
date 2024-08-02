#include "../include/arvoreb.h"
#include "../include/item.h"

#include <stdio.h>
#include <stdbool.h>

#define M 10// Ordem da árvore

void inicializa(Apontadores* Arvore){// cria apontador para a arvore
    *Arvore = NULL; 
}

void criaarvoreb(Analise *analise, FILE *arquivo, Item *registro, int situacao, long int quant, int metodo){

    Apontadores Arvore;
    clock_t timepre;//tempo na pre processamento
    clock_t timepesquisa;//tempo de pesquisa 

     //dados de analise do processo 
    Inicializa(&Arvore);
    analise->comppesquisa = 0;//comparações na pesquisa
    analise->comppre = 0; //comparações no pré processamento
    analise->transpre = 0;///transferencia no pré processamento
    analise->transpesquisa = 0;//transferencia na pesquisa

    int i = 0;

    timepre = clock();// marcar o tempo de inicio do processamento

    //lendo dados do arquivo e inserindo na arvore
    while((fread(&registro, sizeof(Item), 1, arquivo) == 1) && i<quant){     
           if (metodo == 1){
            printf("Chave %ld\n", registro->chave);
        }
        i++;
        insere(*registro, &Arvore, analise);
    } 

    timepre = clock() - (double)timepre; // fim do processamento 
    //numero de comparações ao inserir dados
    analise->comppre = ((double)timepre) / CLOCKS_PER_SEC;

    timepesquisa = clock();//guarda o tempo inicial da pesquisa 
    pesquisar(registro, Arvore, analise);

    timepesquisa = clock() - (double)timepesquisa;//marca fim da pesquisa e realiza calcula a diferença de tempo 
    // quantidade de comparações com a chave
    analise->comppesquisa = ((double)(timepesquisa))/ CLOCKS_PER_SEC;

    LiberaArvore(&Arvore); //chamada da função para liberar arvore da memória principal
}

//insere os registros nas paginas 
void inserenaPagina(Apontadores Arvore, Item dados, Apontadores ApDir, Analise* analise){
    
    short Nhaposicao;
    int k;
    k = Arvore->quant; 
    Nhaposicao = (k > 0);

    while(Nhaposicao){// loop até que a posição seja encontrada

        analise->comppre++;// acrescenta a cada comparação

        // se a chave a ser inserida for maior que 
        if(dados.chave >= Arvore->pai[k-1].chave){
            Nhaposicao = 0;
            break;
        }
        
        Arvore->pai[k] = Arvore->pai[k-1];
        Arvore->filhos[k+1] = Arvore->filhos[k];
        k--;
        if(k < 1)
        Nhaposicao = 0;
    }

    Arvore->pai[k] = dados;
    Arvore->filhos[k+1] = ApDir; 
    Arvore->quant++;//os itens na pagina aumenta
}

void ins(Item x,Apontadores Arvore, short *cresceu, Item* registroretorno, Apontadores* ApRetorno, Analise* analise){

    long i = 1;
    long j;

    Apontadores ApTemp;
    
    if(Arvore == NULL){
        *cresceu = true;
        (*registroretorno) = x;
        (*ApRetorno) = NULL;
        return;
    }

// Enquanto o valor de `i` for menor que a quantidade de itens na árvore
// e a chave do item `x` for maior que a chave do item na posição `i-1` do vetor `pai` da árvore,
// incrementa `i` para continuar a busca na próxima posição.
    while(i < Arvore->quant && x.chave > Arvore->pai[i-1].chave){
        i++;
        analise->comppre++; 
    }
    
    // se o item ja existir ele não é reinserido 
    if(x.chave == Arvore->pai[i-1].chave){
            printf("Registro existente");
            analise->comppre++;
            *cresceu = false;
            return;
    }

    analise->comppre++;

    //se a chave a ser inserida for menor que a chave da pagina
    if(x.chave < Arvore->pai[i-1].chave) i--;
    ins(x,Arvore->filhos[i], cresceu, registroretorno, ApRetorno, analise);
    if(!*cresceu)
        return;
    
    //pagina tem espaço
    if(Arvore->quant < M*M){
        inserenaPagina(Arvore, *registroretorno, *ApRetorno, analise);
        *cresceu = false;
        return;
    }

    //dividir pagina numeros de itens maximo atingido
    ApTemp = (Apontadores)malloc(sizeof(Pagina));//alocação de nova pagina
    ApTemp->quant = 0;
    ApTemp->filhos[0]= NULL;

    if(i < M+1){
        inserenaPagina(ApTemp, Arvore->pai[M*M-1], Arvore->filhos[M*M], analise);
        Arvore->quant--;
        inserenaPagina(Arvore, *registroretorno, *ApRetorno, analise);
    }

    else
        inserenaPagina(ApTemp, *registroretorno, *ApRetorno, analise);

    for(j = M+2;j <= M*M; j++){

        inserenaPagina(ApTemp, Arvore->pai[j-1],Arvore->filhos[j], analise);
    
    }

    Arvore->quant = M; 
    ApTemp->filhos[0] = Arvore->filhos[M+1];
    *registroretorno = Arvore->pai[M];
    *ApRetorno = ApTemp;
}


void insere(Item x, Apontadores* Arvore, Analise, Analise* analise){

    short cresceu;
    Item registroretorno; 
    Pagina* ApRetorno, *ApTemp; 
    ins(x, *Arvore, &cresceu, &registroretorno, &ApRetorno, analise);

    if(cresceu){//crescimento na altura da raiz
 
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


void pesquisar(Item *x, Apontadores Arvore, Analise* analise){

    long i = 1; 
    
    //Se a árvore estiver vazia 
    if(Arvore == NULL){
        analise->comppesquisa++;
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    analise->comppesquisa++;
    // enquanto i for menor que a quantidade de Itens na arvore e a chave for  
    while(i < Arvore->quant && (x->chave > Arvore-> pai[i-1].chave)){
        i++;
        analise->comppesquisa++;
    } 

    analise->comppesquisa++;
    // se a chave for igual ao item da árvore
    if(x->chave == Arvore->pai[i-1].chave){
        *x = Arvore->pai[i-1];
        imprimirDados(*x);
        analise->comppesquisa++;
        return;
    }

    analise->comppesquisa++;
    // se a chave for menor que item da arvore retorna chamada recurssiva pelaesquerda 
    if(x->chave < Arvore->pai[i-1].chave)
        pesquisar(x, Arvore->filhos[i-1], analise);
    
    // se a chave for maior chamada recurssiva para a direita
    else 
        pesquisar(x, Arvore->filhos[i], analise);
}

void LiberaPagina(Apontadores Ap){ // Funcão recurssiva para liberar cada nodo da arvore 
    if (Ap != NULL){
        for (int i = 0; i <= Ap->quant; i++){
            LiberaPagina(Ap->filhos[i]); 
        }
        free(Ap);
    }
}

void LiberaArvore(Apontadores *Arvore){
    if (*Arvore != NULL){
        LiberaPagina(*Arvore); // chamada para função recurssiva
        *Arvore = NULL; 
    }
}


