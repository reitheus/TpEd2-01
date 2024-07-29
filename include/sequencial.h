#ifndef SEQUENCIAL_H
#define SEQUENCIAL_H
#include <time.h>
#include "../include/item.h"
#include "../include/pesquisaBinariaInterna.h"



//Parametros: arquivo, quantidade de itens no arquivo, situação, Item passado por poteiro,chave que vai ser pesquisada
int acessoSequencial(FILE *, int , int , Item *, int , DadosPesquisa *);

#endif // SEQUENCIAL_H


