#ifndef QUERIES_H
#define QUERIES_H

#include "avl.h"

void listarPorSeveridade(NoAVL* raiz, int min, int max);
void relatorioPorRegiao(NoAVL* raiz, Regiao regiao);
void listarPorIntervaloID(NoAVL* raiz, int min_id, int max_id);
void alterarStatus(NoAVL* raiz, int id, StatusEvento novo_status);
void atualizarSeveridade(NoAVL* raiz, int id, int nova_severidade);

#endif /* QUERIES_H */
#ifndef QUERIES_H
#define QUERIES_H

#include "avl.h"

void listarPorSeveridade(NoAVL* raiz, int min, int max);
void relatorioPorRegiao(NoAVL* raiz, const char* regiao);
void listarPorIntervaloID(NoAVL* raiz, int min_id, int max_id);
void alterarStatus(NoAVL* raiz, int id, StatusEvento novo_status);
void atualizarSeveridade(NoAVL* raiz, int id, int nova_severidade);

#endif /* QUERIES_H */
