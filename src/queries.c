#include <stdio.h>
#include "queries.h"

/*
 * Consultas avançadas: a árvore está ordenada por id. Percorrer esquerda → nó → direita
 * (em-ordem) lista os ids em ordem crescente; isso garante relatórios por região e por
 * severidade com saída ordenada por ID. O intervalo de IDs usa em-ordem com poda: só
 * desce à esquerda se o id atual pode ainda ser > min_id, e à direita se < max_id.
 */

static const char* tipoParaTexto(TipoEvento tipo) {
    switch (tipo) {
        case TRANSITO:
            return "Transito";
        case SEMAFORO:
            return "Semaforo";
        case ENERGIA:
            return "Energia";
        case ALAGAMENTO:
            return "Alagamento";
        case INCENDIO:
            return "Incendio";
        default:
            return "Desconhecido";
    }
}

static const char* statusParaTexto(StatusEvento status) {
    switch (status) {
        case ATIVO:
            return "ATIVO";
        case RESOLVIDO:
            return "RESOLVIDO";
        default:
            return "DESCONHECIDO";
    }
}

static const char* regiaoParaTexto(Regiao regiao) {
    const char* nome_regiao[] = {"Norte", "Sul", "Leste", "Oeste", "Centro"};
    if (regiao < NORTE || regiao > CENTRO) {
        return "Desconhecida";
    }
    return nome_regiao[regiao];
}

static void imprimirEvento(NoAVL* no) {
    if (no == NULL) {
        return;
    }

    printf("ID: %d | Tipo: %s | Severidade: %d | Data/Hora: %02d/%02d/%04d %02d:%02d | Regiao: %s | Status: %s\n",
           no->id,
           tipoParaTexto(no->tipo),
           no->severidade,
           no->data_registro.dia,
           no->data_registro.mes,
           no->data_registro.ano,
           no->data_registro.hora,
           no->data_registro.minuto,
           regiaoParaTexto(no->regiao),
           statusParaTexto(no->status));
}

/* Travessia em-ordem (LNR): filtra ATIVOS com severidade em [min, max]; ordem de saída = ordem de id. */
void listarPorSeveridade(NoAVL* raiz, int min, int max) {
    if (raiz == NULL) {
        return;
    }

    listarPorSeveridade(raiz->esq, min, max);

    if (raiz->status == ATIVO && raiz->severidade >= min && raiz->severidade <= max) {
        imprimirEvento(raiz);
    }

    listarPorSeveridade(raiz->dir, min, max);
}

/* Em-ordem sobre toda a árvore: entre os ATIVOS da região pedida, impressão ordenada por id. */
void relatorioPorRegiao(NoAVL* raiz, Regiao regiao) {
    if (raiz == NULL) {
        return;
    }

    relatorioPorRegiao(raiz->esq, regiao);

    if (raiz->status == ATIVO && raiz->regiao == regiao) {
        imprimirEvento(raiz);
    }

    relatorioPorRegiao(raiz->dir, regiao);
}

/* Em-ordem com poda pela ordenação por id: evita visitar ramos que não podem cair no intervalo. */
void listarPorIntervaloID(NoAVL* raiz, int min_id, int max_id) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->id > min_id) {
        listarPorIntervaloID(raiz->esq, min_id, max_id);
    }

    if (raiz->id >= min_id && raiz->id <= max_id) {
        imprimirEvento(raiz);
    }

    if (raiz->id < max_id) {
        listarPorIntervaloID(raiz->dir, min_id, max_id);
    }
}

void alterarStatus(NoAVL* raiz, int id, StatusEvento novo_status) {
    NoAVL* evento = buscarNo(raiz, id);
    if (evento != NULL) {
        evento->status = novo_status;
        printf("Status do evento %d alterado com sucesso para %s.\n", id, statusParaTexto(novo_status));
    } else {
        printf("Evento com ID %d nao foi encontrado.\n", id);
    }
}

void atualizarSeveridade(NoAVL* raiz, int id, int nova_severidade) {
    NoAVL* evento = buscarNo(raiz, id);
    if (evento == NULL) {
        printf("Evento com ID %d nao foi encontrado.\n", id);
        return;
    }

    if (evento->status == ATIVO) {
        evento->severidade = nova_severidade;
        printf("Severidade do evento %d atualizada com sucesso para %d.\n", id, nova_severidade);
    } else {
        printf("Nao e possivel alterar a severidade do evento %d porque ele esta RESOLVIDO.\n", id);
    }
}
