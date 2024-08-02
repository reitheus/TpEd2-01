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

//Parametros: ponteiro da tabela de paginas, tamanho da tabela, item que vai ser retornado, arquivo, quantidade de itens por pagina, situação da ordem do arquivo, chave que vai ser pesquisada
bool pesquisaSequencial(int *tab, int tam, Item* item, FILE *pFile, int itensPagina, int situation, int chave, DadosPesquisa *entrada){
  Dicionario dicio;
  Item *pagina;
  entrada->analise.timepesquisa = clock();
  pagina = (Item*)malloc(sizeof(Item) * itensPagina);// alloca uma pagina
  int i, quantItens;// i e quantidade de itens da pagina
  int posNaPag;
  long desloc;

  i = 0;
  entrada->analise.comppesquisa++;
  while(i < tam && tab[i] <= chave){//procura em qual pagina se encontra o item

    entrada->analise.comppesquisa++;
    i++;

  }
  
  if(i == 0) //se i = 0 o item não esta no arquivo
    return false;
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
    entrada->analise.transpesquisa++;
    fread (pagina, sizeof(Item), quantItens, pFile);
    entrada->analise.transpesquisa++;
    // Inicia o dicionario para a pesquisa Binaria
    dicionarioInicio(&dicio, quantItens, pagina);

    //Faz a pesquisa binaria se for crescente ou decrescente, se estiver fora de ordem faz a pesquisa sequencial
    posNaPag = dicionarioFind(&dicio , situation, chave);

    
    //Verifica se achou o item se não achar retorna 0
    entrada->analise.timepesquisa = clock() - entrada->analise.timepesquisa;
    if(posNaPag < 0){

      free(pagina);
      return false;

    }else{// se achar o item retorna o item por ponteiro e retorna verdadeiro
      *item = pagina[posNaPag];
      
      free(pagina);
      return true;
    }
    
    free(pagina);
    return false;
  }

}

//Parametros: arquivo, quantidade de itens no arquivo, chave que vai ser pesquisada
void acessoSequencial(FILE *pFile, int numItems, int situation, Item *item, int chave, DadosPesquisa *entrada){

  //variavel para testes
  entrada->analise.timepre = clock();
  int itensPagina;// = escolheQuantItens(numItems);//calcula quantos itens vai ter por pagina
  itensPagina = 7;
  int maxTabela = numItems/itensPagina;//calcula quantidade de paginas para criar a tabela
  printf("\nmaxTab %i\n", maxTabela);
  int *tabela;
  tabela = (int*)malloc(sizeof(int) * maxTabela);//alloca a tabela de paginas

  Item *vetX;
  vetX = (Item*)malloc(sizeof(Item) * itensPagina);// aloca um vetor de itens, uma pagina com a quantidade de itens por pagina


  int pos;//posição na tabela de paginas
  int conti = 0;
  int quantpagtemp = itensPagina;
  entrada->analise.transpre++;
  pos = 0;//inicia a posição
  while (fread(vetX, sizeof(Item), quantpagtemp, pFile) != 0 && conti < entrada->quant){//Cria a tabela de paginas

    conti = conti + itensPagina;
    tabela[pos] = vetX[0].chave;
    entrada->analise.transpre++;
    pos++;
    if((entrada->quant - conti) < itensPagina){//Se chegar na ultima pagina e ela tiver menos itens atrbui a quantidade certa de itens
        quantpagtemp = (entrada->quant - conti) ;
    }
  }
  
 
  entrada->analise.timepre = clock() - entrada->analise.timepre;
  bool pesq = pesquisaSequencial(tabela, pos, item, pFile, itensPagina, situation, chave, entrada);
  impreResultado(pesq,entrada,item);//usa a função pesquisa para achar o item no arquivo

  free(vetX);
  free(tabela);

}
