#include "../include/item.h"

void imprimeDado(DadosPesquisa *dado, Item *entrada) {
    if(strcmp(dado->op,"-p")==0){
      printf("|  Chave: %i\n", entrada->chave);
      printf("|  Dado1: %li\n", entrada->dado1);
      printf("|  Dado2: %s\n", entrada->dado2);
      printf("|  Dado3: %s\n", entrada->dado3);
    }
    printf("|  Quantidade de transferencias no Pré processamento = %d\n", dado->analise.transpre);
    printf("|  Quantidade de transferencias na pesquisa = %d\n", dado->analise.transpesquisa);
    printf("|  Quantidade de comparações no pré processamento = %d\n", dado->analise.comppre);
    printf("|  Quantidade de comparações na pesquisa = %d\n", dado->analise.comppesquisa);
    printf("|  Tempo de execução do pré processamento = %lf segundos\n", dado->analise.timepre);
    printf("|  Tempo de execução na pesquisa = %lf segundos\n", dado->analise.timepesquisa);
    printf("-----------------------------------------------------------------------------------------\n");
}

void impreResultado(bool pesq,DadosPesquisa *dado,Item* entrada){

  if (pesq) {
    printf("\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("|  Encontrou a Chave!  \n");
    printf("-----------------------------------------------------------------------------------------\n");
    imprimeDado(dado, entrada);
    printf("\n");
  } else {
    printf("\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("|  Não encontrou a Chave!  \n");
    printf("-----------------------------------------------------------------------------------------\n");
    imprimeDado(dado, entrada);
    printf("\n");
  }
}