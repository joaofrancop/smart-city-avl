#ifndef AVL_H
#define AVL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TRANSITO,
    SEMAFORO,
    ENERGIA,
    ALAGAMENTO,
    INCENDIO
} TipoEvento;

typedef enum {
    ATIVO,
    RESOLVIDO
} StatusEvento;

typedef enum {
    NORTE,
    SUL,
    LESTE,
    OESTE,
    CENTRO
} Regiao;

/* Representa a data e hora de registro do evento. */
typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} DataHora;

/* Nó principal da Árvore AVL para eventos críticos. */
typedef struct NoAVL {
    int id;
    TipoEvento tipo;
    int severidade;
    DataHora data_registro;
    Regiao regiao;
    StatusEvento status;
    int altura;
    struct NoAVL* esq;
    struct NoAVL* dir;
} NoAVL;

extern int total_rotacoes;

int obterAltura(NoAVL* no);
int obterFatorBalanceamento(NoAVL* no);
int maiorValor(int a, int b);

NoAVL* rotacaoEsquerda(NoAVL* x);
NoAVL* rotacaoDireita(NoAVL* y);

NoAVL* criarNo(int id, TipoEvento tipo, int severidade, DataHora data, Regiao regiao, StatusEvento status);
NoAVL* inserirNo(NoAVL* raiz, int id, TipoEvento tipo, int severidade, DataHora data, Regiao regiao, StatusEvento status);
NoAVL* removerNo(NoAVL* raiz, int id);
NoAVL* buscarNo(NoAVL* raiz, int id);

#endif /* AVL_H */
