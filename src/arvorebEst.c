#include "../include/arvorebEst.h"

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
        while (i < Pg->UU.U0.quant && dado->chave > Pg->UU.U0.pai[i-1].chave) {
            i++;
            info->comppesquisa += 1;
        }
        // Chama a função recursivamente no filho adequado
        if (dado->chave < Pg->UU.U0.pai[i-1].chave) {
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
void inserenaPaginaBE(Apontador Arvore, Item dados, Apontador ApDir, Analise* analise) {
    int k;
    k = Arvore->UU.U1.ne;
    analise->transpre++;
    // Insere o registro na posição correta
    while (k > 0){
        if(dados.chave >= Arvore->UU.U1.re[k-1].chave) {
            break;
        }
        analise->comppre++;
        Arvore->UU.U1.re[k] = Arvore->UU.U1.re[k-1];
        Arvore->UU.U0.filho[k + 1] = Arvore->UU.U0.filho[k];
        analise->transpre+=2;
        k--;
        if(k < 1)
            k = 0; 
    }
    Arvore->UU.U1.re[k] = dados;
    Arvore->UU.U0.filho[k + 1] = ApDir; 
    Arvore->UU.U1.ne++;
    analise->transpre+=2;
    
}

// Função para inserir um registro na árvore B* e lidar com a divisão de páginas
void insBE(Item registro, Apontador Arvore, short *cresceu, Item* registroretorno, Apontador* ApRetorno, Analise* analise) {
    long i = 1;
    long j;
    Apontador ApTemp;

    // Se a árvore está vazia, cria um novo nó
    if (Arvore == NULL) {
        *cresceu = 1;
        *registroretorno = registro;
        *ApRetorno = NULL;
        return;
    }
    if(Arvore->Pt == Interna){

        // Encontra o filho adequado para seguir a inserção
        while (i < Arvore->UU.U0.quant && registro.chave > Arvore->UU.U0.pai[i-1].chave) {
            i++;
            analise->comppre+=1;
        }

        // Se o item já existir ele não é reinserido 
        if (registro.chave == Arvore->UU.U0.pai[i-1].chave) {
            printf("Registro existente");
            analise->comppre+=1;
            *cresceu = 0;
            return;
        }

        if (registro.chave < Arvore->UU.U0.pai[i-1].chave) {
            i--;
            analise->comppre+=1;
        }

        // Insere o registro no filho apropriado
        insBE(registro, Arvore->UU.U0.filho[i], cresceu, registroretorno, ApRetorno, analise);
        if (!*cresceu) {
        return;
        }
            
        // Se a página não precisa ser dividida
        if (Arvore->UU.U0.quant < 2 * M) {
            inserenaPaginaI(Arvore, *registroretorno, *ApRetorno, analise);
            *cresceu = 0;
            return;
        }

        // Divide a página
        ApTemp = (Apontador)malloc(sizeof(TipoPagina)); // Aloca uma nova página
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.quant = 0;
        ApTemp->UU.U0.filho[0] = NULL; 

        // Insere as chaves e filhos adequados na nova página
        if (i < M + 1) {
            inserenaPaginaI(ApTemp, Arvore->UU.U0.pai[2 * M - 1], Arvore->UU.U0.filho[2 * M], analise);
            Arvore->UU.U0.quant--;
            inserenaPaginaI(Arvore, *registroretorno, *ApRetorno, analise);
        } else {
            inserenaPaginaI(ApTemp, *registroretorno, *ApRetorno, analise);
        }

        for (j = M + 2; j <= 2 * M; j++) {
            inserenaPaginaI(ApTemp, Arvore->UU.U0.pai[j-1], Arvore->UU.U0.filho[j], analise);
        }

        Arvore->UU.U1.ne = M;
        ApTemp->UU.U0.filho[0] = Arvore->UU.U0.filho[M + 1];
        *registroretorno = Arvore->UU.U0.pai[M];
        *ApRetorno = ApTemp;
        *cresceu = 1;
        analise->transpre+=1;
    }
    i = 1;
    while(i < Arvore->UU.U1.ne && registro.chave > Arvore->UU.U1.re[i-1].chave){
        i++;
        analise->comppre++;
    }

    if(Arvore->UU.U1.ne < 2*M){
        inserenaPaginaE(Arvore,*registroretorno,*ApRetorno,analise);
        *cresceu=0;
        return;
    }
    //precisa dividr
    ApTemp = (Apontador)malloc(sizeof(TipoPagina));
    ApTemp->Pt = Externa;
    ApTemp->UU.U1.ne = 0;

    if(i < M + 1){

    }else{
        inserenaPaginaE(ApTemp,*registroretorno, *ApRetorno,analise);
    }
    for(j = M+2; j <= M*2;j++){
        ApTemp->UU.U1.re[j] = Arvore->UU.U1.re[j-1];
        analise->transpre++;
    }

    Arvore->UU.U1.ne = M+1;
    ApTemp->UU.U0.filho[0]
}

// Função para inserir um registro na árvore B* e lidar com o caso em que a raiz precisa ser dividida
void insereBE(Item reg, Apontador* Arvore, Analise* analise) {
    short cresceu;
    Item registroretorno;
    Apontador ApRetorno, ApTemp;

    // Insere o registro na árvore
    insBE(reg, *Arvore, &cresceu, &registroretorno, &ApRetorno, analise);

    // Se a raiz foi dividida e precisa de uma nova raiz
    if (cresceu) {
        ApTemp = (Apontador)malloc(sizeof(TipoPagina));
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
    Apontador Arvore = NULL;
    int i = 0;

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
