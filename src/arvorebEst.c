#include "../include/arvorebEst.h"

void liberaPaginaBE(Apontador Ap) { // Função recursiva para liberar cada nodo da árvore
    if (Ap != NULL) {
        for (int i = 0; i <= Ap->UU.U0.quant; i++) {
            liberaPaginaBE(Ap->UU.U0.filho[i]);
        }
        free(Ap);
    }
}

void liberaArvoreBE(Apontador *Arvore) {
    if (*Arvore != NULL) {
        liberaPaginaBE(*Arvore); // Chamada para função recursiva
        *Arvore = NULL;
    }
}


void pesquisaArvorebEst(Item *dado,Apontador *arvore, Analise* info,bool *achou){
    int i;
    Apontador Pg;
    Pg = *arvore;
    if((*arvore)->Pt == Interna){
        i = 1;
        while(i < Pg->UU.U0.quant && dado->chave > Pg->UU.U0.pai[i-1].chave){
            i++;
            info->comppesquisa += 1;
        }
        if(dado->chave < Pg->UU.U0.pai[i-1].chave){
            info->transpesquisa += 1;
            pesquisaArvorebEst(dado,&Pg->UU.U0.filho[i-1],info,achou);
        }else{
            info->transpesquisa += 1;
            pesquisaArvorebEst(dado,&Pg->UU.U0.filho[i],info,achou);
        }

        return;
    }
    i = 1;
    while(i < Pg->UU.U1.ne && dado->chave > Pg->UU.U1.re[i-1].chave){
        i++;
        info->comppesquisa += 1;
    }
    if(dado->chave == Pg->UU.U1.re[i-1].chave){
        info->comppesquisa += 1;
        *achou = true;
    }else{
        info->comppesquisa += 1;
        *achou = false;
    }
}

// Insere os registros nas páginas
void inserenaPaginaBE(Apontador Arvore, Item dados, Apontador ApDir, Analise* analise) {

    int k;
    k = Arvore->UU.U1.ne;

    while (k > 0) { // Loop até que a posição seja encontrada
        analise->comppre+=1; // Acrescenta a cada comparação

        // Se a chave a ser inserida for maior que 
        if (dados.chave >= Arvore->UU.U0.pai[k-1].chave) {
            k = 0;
            break;
        }

        Arvore->UU.U0.pai[k] = Arvore->UU.U0.pai[k-1];
        Arvore->UU.U0.filho[k+1] = Arvore->UU.U0.filho[k];
        k--;
        if (k < 1)
            k = 0;
    }

    Arvore->UU.U1.re[k] = dados;
    Arvore->UU.U0.filho[k+1] = ApDir;
    Arvore->UU.U1.ne++;; // Os itens na página aumentam
}

void insBE(Item registro, Apontador Arvore, short *cresceu, Item* registroretorno, Apontador* ApRetorno, Analise* analise) {
    long i = 1;
    long j;

    Apontador ApTemp;

    if (Arvore == NULL) {
        *cresceu = 1;
        *registroretorno = registro;
        *ApRetorno = NULL;
        return;
    }
    if(Arvore->Pt == Interna){

        // Enquanto o valor de `i` for menor que a quantidade de itens na árvore e a chave do item `x` for maior que a chave do item na posição `i-1` do vetor `pai` da árvore, incrementa `i` para continuar a busca na próxima posição.
        while (i < Arvore->UU.U0.quant && registro.chave > Arvore->UU.U0.pai[i-1].chave) {
            i++;
            analise->comppre++;
        }
        // Se a chave a ser inserida for menor que a chave da página
        if (registro.chave < Arvore->UU.U0.pai[i-1].chave) {
            i--;
        }
        insBE(registro, Arvore->UU.U0.filho[i], cresceu, registroretorno, ApRetorno, analise);
        if (!*cresceu) {
            return;
        }

    }
    // Se o item já existir ele não é reinserido 
    if (registro.chave == Arvore->UU.U1.re[i-1].chave) {
        printf("Registro existente");
        analise->comppre+=1;
        *cresceu = 0;
        return;
    }
    // Página tem espaço
    if (Arvore->UU.U1.ne < 2*M && Arvore->Pt == Externa) {
        
        inserenaPaginaBE(Arvore, *registroretorno, *ApRetorno, analise);
        *cresceu = 0;
        return;
    }



    // Dividir página: número de itens máximo atingido
    ApTemp = (Apontador)malloc(sizeof(TipoPagina)); // Alocação de nova página
    ApTemp->UU.U1.ne = 0;
    ApTemp->UU.U0.filho[0] = NULL;

    if (i < M+1) {
        inserenaPaginaBE(ApTemp, Arvore->UU.U0.pai[2 * M-1], Arvore->UU.U0.filho[2 * M], analise);
        Arvore->UU.U1.ne--;
        inserenaPaginaBE(Arvore, *registroretorno, *ApRetorno, analise);
    } else {
        inserenaPaginaBE(ApTemp, *registroretorno, *ApRetorno, analise);
    }

    for (j = M+2; j <= 2 * M; j++) {
        inserenaPaginaBE(ApTemp, Arvore->UU.U0.pai[j-1], Arvore->UU.U0.filho[j], analise);
    }

    Arvore->UU.U1.ne = M+1;
    ApTemp->UU.U0.filho[0] = Arvore->UU.U0.filho[M+1];
    *registroretorno = Arvore->UU.U0.pai[M];
    *ApRetorno = ApTemp;
}

void insereBE(Item reg, Apontador* Arvore, Analise* analise) {
    short cresceu;
    Item registroretorno;
    TipoPagina* ApRetorno, *ApTemp;
    insBE(reg, *Arvore, &cresceu, &registroretorno, &ApRetorno, analise);

    if (cresceu) { // Crescimento na altura da raiz
        ApTemp = (TipoPagina*)malloc(sizeof(TipoPagina)); // Alocando memória
        ApTemp->UU.U0.quant = 1;
        ApTemp->UU.U0.pai[0] = registroretorno;
        ApTemp->UU.U0.filho[1] = ApRetorno;
        ApTemp->UU.U0.filho[0] = *Arvore;
        *Arvore = ApTemp;
    }
}

void criaarvoreBE(FILE *arquivo, Item *registro, DadosPesquisa *dados) {
    Apontador *Arvore = NULL;

    int i = 0;

    dados->analise.timepre = (double)clock(); // Marcar o tempo de início do processamento
    dados->analise.comppre += 1;
    // Lendo dados do arquivo e inserindo na árvore
    while ((fread(registro, sizeof(Item), 1, arquivo) == 1) && i < dados->quant) {
        insereBE(*registro, Arvore, &dados->analise);
        i++;
        dados->analise.transpre += 1;
    }

    dados->analise.timepre = (((double)clock() - dados->analise.timepre) / CLOCKS_PER_SEC); // Fim do processamento 
    // Número de comparações ao inserir dados
    dados->analise.timepesquisa = (double)clock();
    bool pesq;
    pesquisaArvorebEst(registro, Arvore, &dados->analise, &pesq);
    dados->analise.timepesquisa = (((double)clock() - dados->analise.timepesquisa) / CLOCKS_PER_SEC);
    impreResultado(pesq, dados, registro);
    liberaArvoreBE(Arvore); // Chamada da função para liberar árvore da memória principal
}
