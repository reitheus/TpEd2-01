#include "../include/arvorebin.h"

void imprimeDado(Analise *dado, Registro *entrada) {
    printf("|  Chave: %i\n", entrada->key.chave);
    printf("|  Dado1: %li\n", entrada->key.dado1);
    printf("|  Dado2: %s\n", entrada->key.dado2);
    printf("|  Dado3: %s\n", entrada->key.dado3);
    printf("-----------------------------------------------------------------------------------------\n");
    printf("|  Quantidade de transferencias no Pré processamento = %d\n", dado->transpre);
    printf("|  Quantidade de transferencias na pesquisa = %d\n", dado->transpesquisa);
    printf("|  Quantidade de comparações no pré processamento = %d\n", dado->comppre);
    printf("|  Quantidade de comparações na pesquisa = %d\n", dado->comppesquisa);
    printf("|  Tempo de execução do pré processamento = %lf segundos\n", dado->timepre);
    printf("|  Tempo de execução na pesquisa = %lf segundos\n", dado->timepesquisa);
    printf("-----------------------------------------------------------------------------------------\n");
}

// cria uma arvore em memoria externa
void criaArvoreBin(FILE *arvoreBin, FILE *arquivo, long *quantidade, Analise *dado) {
    time_t ini = time(NULL);
    if (arvoreBin == NULL || arquivo == NULL) {
        printf("Erro no arquivo\n");
        return;
    }
    Registro aux, aux1;
    Item info;
    int att, cont;
    long i = 0;
    dado->transpre += 1;
    while (fread(&info, sizeof(Item), 1, arquivo) == 1 && i < *quantidade) {
        att = 0;
        aux.dir = -1;
        aux.esq = -1;
        aux.key = info;
        fseek(arvoreBin, 0, SEEK_END);
        dado->transpre += 1;
        fwrite(&aux, sizeof(Registro), 1, arvoreBin);
        cont = ftell(arvoreBin);
        fseek(arvoreBin, 0, SEEK_SET);
        dado->comppre += 3;
        while (att == 0) {
            dado->transpre += 1;
            if (fread(&aux1, sizeof(Registro), 1, arvoreBin) == 1) {
                if (aux.key.chave < aux1.key.chave) {
                    if (aux1.esq == -1) {
                        att = 1;
                        aux1.esq = cont;
                        dado->comppre += 1;
                    } else {
                        dado->comppre += 1;
                        fseek(arvoreBin, sizeof(Registro) * aux1.esq, 0);
                    }
                } else if (aux.key.chave > aux1.key.chave) {
                    if (aux1.dir == -1) {
                        att = 1;
                        aux1.dir = cont;
                        dado->comppre += 1;
                    } else {
                        dado->comppre += 1;
                        fseek(arvoreBin, sizeof(Registro) * aux1.dir, 0);
                    }
                }
            } else {
                att = 1;
            }

        }
        i++;
    }
    time_t fim = time(NULL);
    dado->timepre = difftime(fim, ini);
}

// Pesquisa da Arvore binaria de pesquisa em memoria externa
bool pesquisaArvoreBin(FILE *arvoreBin, int chaveP, Analise *dado, Registro* aux) {
    time_t timepesquisa0 = time(NULL);
    dado->transpesquisa += 1;
    fseek(arvoreBin, 0, SEEK_SET);
    while (fread(aux, sizeof(Registro), 1, arvoreBin) == 1) {
        dado->transpesquisa += 1;
        if (aux->key.chave != chaveP) {
            if (chaveP < aux->key.chave) {
                dado->comppesquisa += 1;
                fseek(arvoreBin,0 , sizeof(Registro) * aux->esq);
            }
            if (chaveP > aux->key.chave) {
                dado->comppesquisa += 1;
                fseek(arvoreBin,0 ,sizeof(Registro) * aux->dir);
            }
        }
        if (aux->key.chave == chaveP) {
            if (dado->comppesquisa == 0) {
                dado->comppesquisa += 1;
            }
            dado->timepesquisa = difftime(time(NULL), timepesquisa0);
            return true;
        }
    }
    dado->timepesquisa = difftime(time(NULL), timepesquisa0);
    return false;
}

void arvoreBinaria(FILE* arquivo, long* quantidade, int *chave, Analise dado) {
    FILE *arvoreBin = fopen("ArvoreBin.bin", "w+b");
    Registro entrada;
    dado.comppesquisa = 0;
    dado.transpesquisa = 0;
    dado.comppre = 0;
    dado.transpre = 0;
    criaArvoreBin(arvoreBin, arquivo, quantidade, &dado);

    if (pesquisaArvoreBin(arvoreBin, *chave, &dado, &entrada)) {
        printf("\n");
        printf("---------------------------------------------------------------------------------------\n");
        printf("|  Encontrou a Chave!  \n");
        printf("---------------------------------------------------------------------------------------\n");
        imprimeDado(&dado, &entrada);
        printf("\n");
    } else {
        printf("\n");
        printf("---------------------------------------------------------------------------------------\n");
        printf("|  Não encontrou a Chave!  \n");
        printf("---------------------------------------------------------------------------------------\n");
        imprimeDado(&dado, &entrada);
        printf("\n");
    }
    fclose(arvoreBin);
}
