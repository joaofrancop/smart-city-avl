#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int total_rotacoes = 0;

/* ---------------------------------------------------------------------------
 * AVL: o fator de balanceamento de um nó é h(subárvore esquerda) − h(direita).
 * Valores fora de {−1, 0, 1} exigem rotação. Após inserção/remoção recalcula-se
 * a altura (1 + max das filhas) e aplica-se LL, RR, LR ou RL conforme o caso.
 * --------------------------------------------------------------------------- */

int obterAltura(NoAVL* no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

int maiorValor(int a, int b) {
    return (a > b) ? a : b;
}

/* Retorna a diferença de alturas: esquerda menos direita (padrão clássico AVL). */
int obterFatorBalanceamento(NoAVL* no) {
    if (no == NULL) {
        return 0;
    }
    return obterAltura(no->esq) - obterAltura(no->dir);
}

/* Rotação simples à direita em torno de y (caso LL na inserção: filho esq mais alto).
 * Equivale a "puxar" o filho esquerdo x para a raiz local; T2 passa a ser filho esq de y. */
NoAVL* rotacaoDireita(NoAVL* y) {
    NoAVL* x = y->esq;
    NoAVL* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = maiorValor(obterAltura(y->esq), obterAltura(y->dir)) + 1;
    x->altura = maiorValor(obterAltura(x->esq), obterAltura(x->dir)) + 1;

    extern int total_rotacoes;
    total_rotacoes++;
    return x;
}

/* Rotação simples à esquerda em torno de x (caso RR na inserção: filho dir mais alto). */
NoAVL* rotacaoEsquerda(NoAVL* x) {
    NoAVL* y = x->dir;
    NoAVL* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = maiorValor(obterAltura(x->esq), obterAltura(x->dir)) + 1;
    y->altura = maiorValor(obterAltura(y->esq), obterAltura(y->dir)) + 1;

    extern int total_rotacoes;
    total_rotacoes++;
    return y;
}

NoAVL* criarNo(int id, TipoEvento tipo, int severidade, DataHora data, Regiao regiao, StatusEvento status) {
    NoAVL* novoNo = (NoAVL*)malloc(sizeof(NoAVL));
    if (novoNo == NULL) {
        return NULL;
    }

    novoNo->id = id;
    novoNo->tipo = tipo;
    novoNo->severidade = severidade;
    novoNo->data_registro = data;
    novoNo->regiao = regiao;
    novoNo->status = status;
    novoNo->altura = 1;
    novoNo->esq = NULL;
    novoNo->dir = NULL;

    return novoNo;
}

/* Inserção BST por id; depois atualiza altura e rebalanceia.
 * LL: fator > 1 e id inserido na subárvore esquerda do filho esq → rotacaoDireita.
 * RR: fator < -1 e id na subárvore direita do filho dir → rotacaoEsquerda.
 * LR: fator > 1 mas id entrou à direita do filho esq → rotacaoEsquerda no esq, depois rotacaoDireita.
 * RL: fator < -1 mas id entrou à esquerda do filho dir → rotacaoDireita no dir, depois rotacaoEsquerda. */
NoAVL* inserirNo(NoAVL* raiz, int id, TipoEvento tipo, int severidade, DataHora data, Regiao regiao, StatusEvento status) {
    if (raiz == NULL) {
        return criarNo(id, tipo, severidade, data, regiao, status);
    }

    if (id < raiz->id) {
        raiz->esq = inserirNo(raiz->esq, id, tipo, severidade, data, regiao, status);
    } else if (id > raiz->id) {
        raiz->dir = inserirNo(raiz->dir, id, tipo, severidade, data, regiao, status);
    } else {
        return raiz;
    }

    raiz->altura = 1 + maiorValor(obterAltura(raiz->esq), obterAltura(raiz->dir));
    int fator = obterFatorBalanceamento(raiz);

    if (fator > 1 && id < raiz->esq->id) {
        return rotacaoDireita(raiz);
    }
    if (fator < -1 && id > raiz->dir->id) {
        return rotacaoEsquerda(raiz);
    }
    if (fator > 1 && id > raiz->esq->id) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fator < -1 && id < raiz->dir->id) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

/* Remoção BST; após remover, rebalanceia com os mesmos quatro tipos, usando o fator
 * do filho (inserção compara id com neto; remoção olha o sinal do fator do filho).
 * Trava de negócio: se o nó-alvo está ATIVO, não remove — devolve a mesma raiz e avisa
 * (a interface também valida antes de chamar; a defesa aqui garante consistência se
 * removerNo for usado de outro lugar). */
NoAVL* removerNo(NoAVL* raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id < raiz->id) {
        raiz->esq = removerNo(raiz->esq, id);
    } else if (id > raiz->id) {
        raiz->dir = removerNo(raiz->dir, id);
    } else {
        /* Impede exclusão física enquanto o evento estiver ATIVO (sincronizado com a UI). */
        if (raiz->status == ATIVO) {
            printf("Aviso: Evento %d ainda esta ATIVO e nao pode ser removido.\n", id);
            return raiz;
        }

        if (raiz->esq == NULL || raiz->dir == NULL) {
            NoAVL* filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;

            if (filho == NULL) {
                free(raiz);
                return NULL;
            } else {
                NoAVL* antigo = raiz;
                raiz = filho;
                free(antigo);
            }
        } else {
            NoAVL* sucessor = raiz->dir;
            while (sucessor->esq != NULL) {
                sucessor = sucessor->esq;
            }

            raiz->id = sucessor->id;
            raiz->tipo = sucessor->tipo;
            raiz->severidade = sucessor->severidade;
            raiz->data_registro = sucessor->data_registro;
            raiz->regiao = sucessor->regiao;
            raiz->status = sucessor->status;

            raiz->dir = removerNo(raiz->dir, sucessor->id);
        }
    }

    if (raiz == NULL) {
        return NULL;
    }

    /* Rebalanceamento pós-remoção: mesmo LL/RR/LR/RL da inserção, identificados pelo
     * fator do nó atual e pelo fator do filho na direção mais pesada. */
    raiz->altura = 1 + maiorValor(obterAltura(raiz->esq), obterAltura(raiz->dir));
    int fator = obterFatorBalanceamento(raiz);

    if (fator > 1 && obterFatorBalanceamento(raiz->esq) >= 0) {
        return rotacaoDireita(raiz);
    }
    if (fator > 1 && obterFatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (fator < -1 && obterFatorBalanceamento(raiz->dir) <= 0) {
        return rotacaoEsquerda(raiz);
    }
    if (fator < -1 && obterFatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

NoAVL* buscarNo(NoAVL* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }

    if (id < raiz->id) {
        return buscarNo(raiz->esq, id);
    }
    return buscarNo(raiz->dir, id);
}

