#ifndef ARVOREB_H
#define ARVOREB_H

#include "../include/item.h"

typedef long TipoChave;
typedef struct TipoRegistro {
TipoChave Chave;
/* outros componentes */
} TipoRegistro;
    typedef struct TipoPagina* TipoApontador;
    typedef struct TipoPagina {
    short n;
    TipoRegistro r[2];
    TipoApontador p[2 + 1];
} TipoPagina;

#endif //arvoreb.h