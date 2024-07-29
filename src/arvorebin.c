#include "../include/arvorebin.h"

void imprimeAbin(Item dado){

    printf("\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("|  Encontrou a Chave!  |\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("|Chave: %i |\n",dado.chave);
    printf("|Dado1: %li|\n",dado.dado1);
    printf("|Dado2: %s |\n",dado.dado2);
    printf("|Dado3: %s |\n",dado.dado3);
    printf("-----------------------------------------------------------------------------------------\n");
    printf("\n");
}

//cria uma arvore em memoria externa
void criaArvoreBin(FILE *arvoreBin, FILE *arquivo, long *quantidade, Analise *dado){
    
    if(arvoreBin == NULL || arquivo == NULL){
        printf("Erro no arquivo");
        return;
    }
    Registro aux,aux1;
    Item info;
    int att,cont, i = 0;
    dado->transpre++;
    while(fread(&info,sizeof(Item),1,arquivo) == 1 && i < *quantidade){
        att = 0;
        aux.dir = -1;
        aux.esq = -1;
        aux.key = info;
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
                    }else{
                        dado->comppre++;
                        fseek(arvoreBin,sizeof(Registro) * aux1.esq,0);
                    }
                }else if(aux.key.chave > aux1.key.chave){
                    if(aux1.dir == -1){
                        att = 1;
                        aux1.dir = cont;
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
    
    FILE *arvoreBin = fopen("ArvoreBin.bin", "rb");
    Registro aux;
    dado.comppesquisa = 0;
    dado.transpesquisa = 0;
    dado.comppre = 0;
    dado.transpre = 0;
    criaArvoreBin(arvoreBin,arquivo,quantidade,&dado);
    dado.comppesquisa++;
    while (fread(&aux,sizeof(Registro),1,arvoreBin) == 1){
        if(aux.key.chave != chaveP){
            if(chaveP < aux.key.chave){
                dado.comppesquisa++;
                fseek(arvoreBin,sizeof(Registro) * aux.esq,0);
            }
            if(chaveP > aux.key.chave){
                dado.comppesquisa++;
                fseek(arvoreBin,sizeof(Registro) * aux.dir,0);
            }
        }
        if(aux.key.chave == chaveP){
            if(dado.comppesquisa == 0){
                dado.comppesquisa = 1;
            }
            imprimeAbin(aux.key);
            return true;
        }


    }
    return false;
}