#include "../include/arvorebin.h"

void imprimeAbin(Registro *dado,Analise *entrada){

    printf("\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("|  Encontrou a Chave!  \n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("|  Chave: %i\n",dado->key.chave);
    printf("|  Dado1: %li\n",dado->key.dado1);
    printf("|  Dado2: %s\n",dado->key.dado2);
    printf("|  Dado3: %s\n",dado->key.dado3);
    printf("-----------------------------------------------------------------------------------------\n");
    printf("|  Quantidade de transferencias no Pré processamento = %i\n", entrada->transpre);
    printf("|  Quantidade de transferencias na pesquisa = %i\n", entrada->transpesquisa);
    printf("|  Quantidade de comparações no pré processamento = %i\n", entrada->comppre);
    printf("|  Quantidade de comparações na pesquisa = %i\n", entrada->transpesquisa);
    printf("|  Tempo de execução do pré processamento = %lf\n", ((double)entrada->transpre/CLOCKS_PER_SEC));
    printf("|  Tempo de execução na pesquisa = %lf\n", ((double)entrada->transpre/CLOCKS_PER_SEC));
    printf("-----------------------------------------------------------------------------------------\n");
    printf("\n");

}

//cria uma arvore em memoria externa
void criaArvoreBin(FILE *arvoreBin, FILE *arquivo, long *quantidade, Analise *dado){
    
    if(arvoreBin == NULL || arquivo == NULL){
        printf("Erro no arquivo\n");
        return;
    }
    Registro aux,aux1;
    Item info;
    int att,cont;
    long i = 0;
    dado->transpre++;
    printf("Console log: inicio da criação da arvore.");
    while(fread(&info,sizeof(Item),1,arquivo) == 1 && i < *quantidade){
        att = 0;
        aux.dir = -1;
        aux.esq = -1;
        aux.key = info;
        printf("chave:%d\n",aux.key.chave);
        fseek(arvoreBin,0,SEEK_END);
        dado->transpre++;
        fwrite(&aux,sizeof(Registro),1,arvoreBin);
        cont = ftell(arvoreBin);
        fseek(arvoreBin,0,SEEK_SET);
        dado->comppesquisa += 3;
        while (att == 0){
            dado->transpre++;
            if(fread(&aux1,sizeof(Registro),1,arvoreBin)==1){
                if(aux.key.chave < aux1.key.chave){
                    if(aux1.esq == -1){
                        att = 1;
                        aux1.esq = cont;
                        printf("aux1.esq: %d\n",aux1.esq);
                    }else{
                        dado->comppre++;
                        fseek(arvoreBin,sizeof(Registro) * aux1.esq,0);
                    }
                }else if(aux.key.chave > aux1.key.chave){
                    if(aux1.dir == -1){
                        att = 1;
                        aux1.dir = cont;
                        printf("aux1.dir: %d\n",aux1.dir);
                    }else{
                        dado->comppre++;
                        fseek(arvoreBin,sizeof(Registro) * aux1.dir,0);
                    }
                }
            }else{
                att=1;
            }


        }
        
        i++;
    }
}

//Pesquisa da Arvore binaria de pesquisa em memoria externa
bool pesquisaArvoreBin(FILE *arquivo,long *quantidade,int* situacao,int chaveP,Analise dado){
    
    FILE *arvoreBin = fopen("ArvoreBin.bin","w+b");
    Registro aux;
    dado.comppesquisa = 0;
    dado.transpesquisa = 0;
    dado.comppre = 0;
    dado.transpre = 0;
    criaArvoreBin(arvoreBin,arquivo,quantidade,&dado);
    printf("arvore criada\n");
    dado.comppesquisa += 1;
    fseek(arvoreBin,0,SEEK_SET);
    printf("console log: pesquisa na arvore\n");
    while (fread(&aux,sizeof(Registro),1,arvoreBin) == 1){
        printf("chave: %d\n",aux.key.chave);
        if(aux.key.chave != chaveP){
            if(chaveP < aux.key.chave){
                dado.comppesquisa += 1;
                fseek(arvoreBin,0,sizeof(Registro) * aux.esq);
            }
            if(chaveP > aux.key.chave){
                dado.comppesquisa += 1;
                fseek(arvoreBin,0,sizeof(Registro) * aux.dir);
            }
        }
        if(aux.key.chave == chaveP){
            if(dado.comppesquisa == 0){
                dado.comppesquisa = 1;
            }
            imprimeAbin(&aux,&dado);
            fclose(arvoreBin);
            return true;
        }

    }
    printf("Não encontrou!\n");
    fclose(arvoreBin);
    
    return false;
}