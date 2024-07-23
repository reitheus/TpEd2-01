#include "../include/arvoreb.h"

void Inicializa (TipoApontador Arvore){
    Arvore = NULL;
}

void Imprime(TipoApontador arvore){
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        Imprime(arvore->p[i]);
        if (i != arvore->n)
        cout << arvore->r[i].Chave << " ";
        i++;
    }
 }
