#ifndef ARVOREBEST_H
#define ARVOREBEST_H

#include "../include/item.h"

typedef long TipoChave;
typedef struct arvorebest{
    TipoChave Chave;
}ArvoreBest;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* TipoApontador;

#endif// arvorebEst.h