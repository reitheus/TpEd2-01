#include <stdio.h>
#include <stdlib.h>
#include "../include/sequencial.h"


int escolheQuantItens(int numItems){

  Item *vetItems;
  int itensPagina = numItems;

  while( (vetItems = (Item*)malloc(sizeof(Item) * itensPagina)) == NULL ){

    itensPagina = itensPagina/2;

  }

  free(vetItems);

  return itensPagina;

}

//Parametros: ponteiro da tabela de paginas, tamanho da tabela, item que vai ser retornado, arquivo, quantidade de itens por pagina
// situação da ordem do arquivo, chave que vai ser pesquisada
int pesquisa(int *tab, int tam, Item* item, FILE *pFile, int itensPagina, int situation, int chave){
  Dicionario dicio;
  Item *pagina;
  
  pagina = (Item*)malloc(sizeof(Item) * itensPagina);// alloca uma pagina
  int i, quantItens;// i e quantidade de itens da pagina
  int posNaPag;
  long desloc;

  i = 0;
  
  while(i < tam && tab[i] <= chave){//procura em qual pagina se encontra o item
    //printf("\nfind %i\n", tab[i]);
    i++;
    //printf("\ni %i\n", i);
  }
  
  if(i == 0) //se i = 0 o item não esta no arquivo
    return 0;
  else {
    // a ultima página pode não estar completa
    if (i < tam){ 
      quantItens = itensPagina;
    } else {// se for a ultima pagina
      fseek(pFile, 0, SEEK_END);
      quantItens = (ftell(pFile)/sizeof(Item))%itensPagina;
      
      if(quantItens == 0){
        quantItens = itensPagina; 
      }

    }

    // lê a página desejada do arquivo
    desloc = (i-1) * itensPagina * sizeof(Item);
    
    fseek (pFile, desloc, SEEK_SET);
    fread (pagina, sizeof(Item), quantItens, pFile);
    // Inicia o dicionario para a pesquisa Binaria
    dicionarioInicio(&dicio, quantItens, pagina);

    //Faz a pesquisa binaria se for crescente ou decrescente, se estiver fora de ordem faz a pesquisa sequencial
    posNaPag = dicionarioFind(&dicio , situation, chave);
    //printf("ii %i", chave);
    
    //Verifica se achou o item se não achar retorna 0
    if(posNaPag < 0){
      //*item = NULL;
      free(pagina);
      return 0;

    }else{// se achar o item retorna o item por ponteiro e retorna verdadeiro
      *item = pagina[posNaPag];
      printf("item %i",item->chave);
      free(pagina);
      return 1;
    }
    
    free(pagina);
    return 0;
  }

}

//Parametros: arquivo, quantidade de itens no arquivo, chave que vai ser pesquisada
int acessoSequencial(FILE *pFile, int numItems, int situation, Item *x, int chave){

  //variavel para testes
  
  int itensPagina;// = escolheQuantItens(numItems);//calcula quantos itens vai ter por pagina
  itensPagina = 5;
  int maxTabela = numItems/itensPagina;//calcula quantidade de paginas para criar a tabela
  //printf("\nmaxTab %i\n", maxTabela);
  int *tabela;
  tabela = (int*)malloc(sizeof(int) * maxTabela);//alloca a tabela de paginas

  Item *vetX;
  vetX = (Item*)malloc(sizeof(Item) * itensPagina);// aloca um vetor de itens, uma pagina com a quantidade de itens por pagina


  int pos;//posição na tabela de paginas

  pos = 0;//inicia a posição
  while (fread(vetX, sizeof(Item), itensPagina, pFile) != 0){//Cria a tabela de paginas
    //printf("\npage %i\n", vetX[0].chave);
    tabela[pos] = vetX[0].chave;
    pos++;
    
  }
  free(vetX);
 
  if(pesquisa(tabela, pos, x, pFile, itensPagina, situation, chave)){//usa a função pesquisa para achar o item no arquivo
    printf("\nItem localizado com Sucesso!\n");
    free(tabela);
    
    return 1;
  }else{
    printf("\nItem não localizado!\n");
    free(tabela);
    
    return 0;

  }

  free(tabela);
  
  return 0;
}
