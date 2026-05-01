#ifndef METRICS_H
#define METRICS_H

#include "avl.h"

int contarNos(NoAVL* raiz);
int contarEventosAtivos(NoAVL* raiz);
int somaFatorBalanceamento(NoAVL* raiz);
float calcularFatorBalanceamentoMedio(NoAVL* raiz);
void exibirMetricas(NoAVL* raiz);

#endif /* METRICS_H */
