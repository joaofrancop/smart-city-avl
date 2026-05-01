#include <stdio.h>
#include "metrics.h"

/* Métricas agregadas por travessia recursiva (pós-ordem implícita: soma subárvores + nó). */

int contarNos(NoAVL* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

/* Conta nós com status ATIVO em toda a árvore. */
int contarEventosAtivos(NoAVL* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return (raiz->status == ATIVO ? 1 : 0) +
           contarEventosAtivos(raiz->esq) +
           contarEventosAtivos(raiz->dir);
}

/* Soma dos fatores locais (h_esq − h_dir) de cada nó; usada para a média aritmética. */
int somaFatorBalanceamento(NoAVL* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return obterFatorBalanceamento(raiz) +
           somaFatorBalanceamento(raiz->esq) +
           somaFatorBalanceamento(raiz->dir);
}

/* Média = soma dos fatores / número de nós (0 se árvore vazia). */
float calcularFatorBalanceamentoMedio(NoAVL* raiz) {
    int totalNos = contarNos(raiz);
    if (totalNos == 0) {
        return 0.0f;
    }
    return (float)somaFatorBalanceamento(raiz) / totalNos;
}

/* Imprime altura na raiz, contagens e total_rotacoes (incrementado em avl.c a cada rotação). */
void exibirMetricas(NoAVL* raiz) {
    printf("\n===== METRICAS DA AVL =====\n");
    printf("Altura total da arvore: %d\n", obterAltura(raiz));
    printf("Numero total de nos: %d\n", contarNos(raiz));
    printf("Numero de eventos ativos: %d\n", contarEventosAtivos(raiz));
    printf("Fator de balanceamento medio: %.2f\n", calcularFatorBalanceamentoMedio(raiz));
    printf("Quantidade total de rotacoes: %d\n", total_rotacoes);
    printf("===========================\n");
}
