#include "../include/menu.h"
#include "../include/item.h"
#include "../include/sequencial.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int geradorAscendente(FILE *pFile, DadosPesquisa entrada){

    Item v;

    for(int i = 0; i < entrada.quant; i++){
        
        v.chave = i;
        v.dado1 = rand();
        //v.dado2[0] = 'a' + (char)(rand()%26);
        //v.dado3[0] = 'a' + (char)(rand()%26); 
        

        fwrite(&v, sizeof(Item), 1, pFile);
        
    }

    return 0;
}
//arquivo que vai ser gerado por stream, Os dados de entrada
int gerar(FILE *pFile, DadosPesquisa entrada){

    switch(entrada.situacao){
        case 1:
            pFile = fopen("ascendente.bin", "wb");
            geradorAscendente(pFile,  entrada);
            fclose(pFile);
        break;
        case 2:
            pFile = fopen("descendente.bin", "wb");

            fclose(pFile);
        break;
        case 3:
            pFile = fopen("aleatorio.bin", "wb");

            fclose(pFile);
        break;
        default:
        printf("\nOpção incorreta\n");

    }


    return 0;

}



