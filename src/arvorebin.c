#include "../include/arvorebin.h"

//cria uma arvore em memoria externa
void criaArvore(FILE *arvoreBin,FILE *arquivo,int *quantidade){
    if(arvoreBin == NULL || arquivo == NULL){
        printf("Erro no arquivo");
        return;
    }
    Registro aux,aux1;
    Item info;
    int att,cont;
    quantidade = 0;
    while(fread(&info,sizeof(Item),1,arquivo) == 1){
        att = 0;
        aux.dir = -1;
        aux.esq = -1;
        aux.key = info;
        fseek(arvoreBin,0,SEEK_END);
        fwrite(&aux,sizeof(Registro),1,arvoreBin);
        cont = ftell(arvoreBin);
        fseek(arvoreBin,0,SEEK_SET);
        while (att == 0){
            if(fread(&aux1,sizeof(Registro),1,arvoreBin)==1){
                if(aux.key.chave < aux1.key.chave){
                    if(aux1.esq == -1){
                        att = 1;
                        aux1.esq = cont;
                    }else{
                        fseek(arvoreBin,sizeof(Registro) * aux1.esq,0);
                    }
                }else if(aux.key.chave > aux1.key.chave){
                    if(aux1.dir == -1){
                        att = 1;
                        aux1.dir = cont;
                    }else{
                        fseek(arvoreBin,sizeof(Registro) * aux1.dir,0);
                    }
                }
            }else{
                att=1;
            }


        }
        
        
    }
}

//Pesquisa da Arvore binaria de pesquisa em memoria externa
bool pesquisaArvoreBin(FILE *arvoreBin,int *quantidade, Item info){
    
    Registro aux;
    quantidade = 0;
    while (fread(&aux,sizeof(Registro),1,arvoreBin) == 1){
        if(aux.key.chave != info.chave){
            if(info < aux.key.chave){
                fseek(arvoreBin,sizeof(Registro) * aux.esq,0);
                quantidade++;
            }
            if(info > aux.key.chave){
                fseek(arvoreBin,sizeof(Registro) * aux.dir,0);
                quantidade++;
            }
        }
        if(aux.key.chave == info.chave){
            if(quantidade == 0){
                quantidade = 1;
            }
            return true;
        }


    }
    return false;
}