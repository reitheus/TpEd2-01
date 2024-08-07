#include "../include/arvorebEst.h"

void iniciaArvoreBE(Apontador* Arvore) { // Cria apontador para a árvore
    *Arvore = NULL;
    printf("Arvore inicializada\n");
}

// Função para liberar memória de uma página da árvore
void liberaPaginaBE(Apontador Ap) {
    if (Ap != NULL) {
        // Recursivamente libera todas as páginas filhas
        for (int i = 0; i <= Ap->UU.U0.quant; i++) {
            liberaPaginaBE(Ap->UU.U0.filho[i]);
        }
        // Libera a memória da página atual
        free(Ap);
    }
}

// Função para liberar a árvore inteira
void liberaArvoreBE(Apontador *Arvore) {
    if (*Arvore != NULL) {
        // Libera a memória da árvore
        liberaPaginaBE(*Arvore);
        *Arvore = NULL;
    }
}


// Função para pesquisar um dado na árvore
void pesquisaArvorebEst(Item *dado, Apontador *arvore, Analise* info, bool *achou) {
    int i;
    Apontador Pg = *arvore;

    // Se o nó é interno
    if (Pg->Pt == Interna) {
        i = 1;
        // Encontra o filho adequado para seguir a busca
        while (i < Pg->UU.U0.quant && dado->chave > Pg->UU.U0.pai[i-1]) {
            i++;
            info->comppesquisa += 1;
        }
        // Chama a função recursivamente no filho adequado
        if (dado->chave < Pg->UU.U0.pai[i-1]) {
            info->transpesquisa += 1;
            pesquisaArvorebEst(dado, &Pg->UU.U0.filho[i-1], info, achou);
        } else {
            info->transpesquisa += 1;
            pesquisaArvorebEst(dado, &Pg->UU.U0.filho[i], info, achou);
        }
        return;
    }

    // Se o nó é externo (folha)
    i = 1;
    while (i < Pg->UU.U1.ne && dado->chave > Pg->UU.U1.re[i-1].chave) {
        i++;
        info->comppesquisa += 1;
    }
    // Verifica se o dado foi encontrado
    if (dado->chave == Pg->UU.U1.re[i-1].chave) {
        info->comppesquisa += 1;
        *achou = true;
    } else {
        info->comppesquisa += 1;
        *achou = false;
    }
}

// Função para inserir um registro em uma página externa
void inserenaPaginaBI(Apontador arvore, TipoChave dados, Apontador ApDir, Analise* analise) {
    int k;
    k = arvore->UU.U0.quant;
    analise->transpre++;
    // Insere o registro na posição correta
    while (k > 0 && dados < arvore->UU.U0.pai[k-1]) {
        analise->comppre++;
        arvore->UU.U0.pai[k] = arvore->UU.U0.pai[k-1];
        arvore->UU.U0.filho[k + 1] = arvore->UU.U0.filho[k];
        analise->transpre += 2;
        k--;
    }
    arvore->UU.U0.pai[k] = dados;
    arvore->UU.U0.filho[k + 1] = ApDir; 
    arvore->UU.U0.quant++;
    analise->transpre += 2;

}
    
void inserenaPaginaBE(Apontador arvore, Item dados, Analise* analise){
    int k;
    k = arvore->UU.U1.ne;
    analise->transpre++;

    while (k > 0 && dados.chave < arvore->UU.U1.re[k-1].chave) {
        analise->comppre++;
        arvore->UU.U1.re[k] = arvore->UU.U1.re[k-1];
        k--;
    }
    arvore->UU.U1.re[k] = dados;
    arvore->UU.U1.ne++;
    analise->transpre++;

    
}

// Função para inserir um registro na árvore B* e lidar com a divisão de páginas
void insBE(Item registro, Apontador Arvore, short *cresceu, TipoChave* registroretorno, Apontador* ApRetorno, Analise* analise) {
    int i = 1;
    int j;
    Apontador ApTemp;

    // Se a árvore está vazia, cria um novo nó
    if (Arvore == NULL) {
        printf("Arvore vazia\n");
        *cresceu = 1;
        *registroretorno = registro.chave;
        *ApRetorno = NULL;
        return;
    }

    if(Arvore->Pt == Interna){
        // Encontra o filho adequado para seguir a inserção
        while (i < Arvore->UU.U0.quant && registro.chave > Arvore->UU.U0.pai[i-1]) {
            //printf("chave: %d > ChaveArvore: %d; i: %d\n",registro.chave,Arvore->UU.U0.pai[i-1],i);
            i++;
            analise->comppre+=1;
        }

        insBE(registro,Arvore,cresceu,registroretorno,ApRetorno,analise);
        if(!*cresceu)return;

        // Se a página não precisa ser dividida
        if (Arvore->UU.U0.quant < M) {
           // printf("inseriu interna\n");
            inserenaPaginaBI(Arvore, *registroretorno,*ApRetorno,analise);
            *cresceu = 0;
            return;
        }
        

        //printf("precisa ser dividida/RegRetorno: %d\n",*registroretorno);
        // Divide a página
        ApTemp = (Apontador)malloc(sizeof(TipoPagina)); // Aloca uma nova página
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.quant = 0;
        ApTemp->UU.U0.filho[0] = NULL; 

    
        if (i <= M / 2) {
            inserenaPaginaBI(ApTemp, Arvore->UU.U0.pai[M-1], Arvore->UU.U0.filho[M], analise);
            printf("inseriu interna\n");
            Arvore->UU.U0.quant--;
            inserenaPaginaBI(Arvore, *registroretorno, *ApRetorno, analise);
            printf("inseriu interna\n");
        } else {
            inserenaPaginaBI(ApTemp, *registroretorno, *ApRetorno, analise);
            printf("inseriu interna\n");
        }

        for (j = M / 2 + 1; j < M; j++) {
            inserenaPaginaBI(ApTemp, Arvore->UU.U0.pai[j], Arvore->UU.U0.filho[j + 1], analise);
            printf("inseriu interna\n");
        }

        Arvore->UU.U0.quant = M / 2;
        ApTemp->UU.U0.filho[0] = Arvore->UU.U0.filho[M / 2 + 1];
        *registroretorno = Arvore->UU.U0.pai[M / 2];
        *ApRetorno = ApTemp;
        analise->transpre+=1;
    }
    //Externo
    printf("externo\n");
    i = 1;
    //acha a posição
    while(i < Arvore->UU.U1.ne && registro.chave > Arvore->UU.U1.re[i-1].chave){
        printf("chave: %d > ChaveArvore: %d; i: %d\n",registro.chave,Arvore->UU.U0.pai[i-1],i);
        i++;
        analise->comppre++;
    }

    if (i < Arvore->UU.U1.ne && registro.chave == Arvore->UU.U1.re[i-1].chave) {
        printf("chave: %d < ChaveArvore: %d; i: %d\n",registro.chave,Arvore->UU.U0.pai[i-1],i);
        *cresceu = 0;
        analise->comppre+=1;
    }

    //pagina não ta cheia
    if(Arvore->UU.U1.ne < 2*M){
        inserenaPaginaBE(Arvore,registro,analise);
        printf("inseriu externa\n");
        *cresceu=0;
        return;
    }
    ApTemp = (Apontador)malloc(sizeof(TipoPagina));
    ApTemp->Pt = Externa;
    ApTemp->UU.U1.ne = 0;

    for(j = M / 2 + 1;j < M;j++){
        ApTemp->UU.U1.re[j - M / 2] = Arvore->UU.U1.re[j];
    }
    ApTemp->UU.U1.ne = M / 2;
    Arvore->UU.U1.ne = M / 2;

    if(i < M + 1){
        inserenaPaginaBE(Arvore,registro,analise);
        printf("inseriu externa\n");
    }else{
        inserenaPaginaBE(ApTemp,registro,analise);
        printf("inseriu externa\n");
    }

    *registroretorno = ApTemp->UU.U1.re[0].chave;
    *ApRetorno = ApTemp;
    *cresceu = 1;
    
}

// Função para inserir um registro na árvore B* e lidar com o caso em que a raiz precisa ser dividida
void insereBE(Item reg,Apontador* Arvore, Analise* analise) {
    short cresceu;
    TipoChave registroretorno;
    TipoPagina *ApRetorno, *ApTemp;

    // Insere o registro na árvore
    insBE(reg, *Arvore, &cresceu, &registroretorno, &ApRetorno, analise);

    // Se a raiz foi dividida e precisa de uma nova raiz
    if (cresceu) {
        printf("creceu\n");
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.quant = 1;
        ApTemp->UU.U0.pai[0] = registroretorno;
        ApTemp->UU.U0.filho[1] = ApRetorno;
        ApTemp->UU.U0.filho[0] = *Arvore;
        *Arvore = ApTemp;
        analise->transpre+=5;
    }
}

// Função para criar a árvore B* a partir de dados de um arquivo e realizar pesquisas
void criaarvoreBE(FILE *arquivo, Item *registro, DadosPesquisa *dados) {
    Apontador Arvore;
    int i = 0;
    iniciaArvoreBE(&Arvore);
    dados->analise.timepre = (double)clock(); // Marca o tempo de início do processamento
    dados->analise.comppre += 1;
    // Lê os dados do arquivo e insere na árvore
    while ((fread(registro, sizeof(Item), 1, arquivo) == 1) && i < dados->quant) {
        insereBE(*registro, &Arvore, &dados->analise);

        i++;
        dados->analise.transpre += 1;
    }
    dados->analise.timepre = (((double)clock() - dados->analise.timepre) / CLOCKS_PER_SEC); // Marca o tempo de fim do processamento

    // Realiza pesquisa na árvore
    dados->analise.timepesquisa = (double)clock();
    bool pesq;
    pesquisaArvorebEst(registro, &Arvore, &dados->analise, &pesq);
    dados->analise.timepesquisa = (((double)clock() - dados->analise.timepesquisa) / CLOCKS_PER_SEC);

    // Imprime o resultado da pesquisa
    impreResultado(pesq, dados, registro);

    // Libera a memória da árvore
    liberaArvoreBE(&Arvore);
}
