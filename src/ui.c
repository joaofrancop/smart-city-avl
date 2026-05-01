#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "avl.h"
#include "queries.h"
#include "metrics.h"

static const char* regiaoParaTexto(Regiao regiao) {
    const char* nome_regiao[] = {"Norte", "Sul", "Leste", "Oeste", "Centro"};
    if (regiao < NORTE || regiao > CENTRO) {
        return "Desconhecida";
    }
    return nome_regiao[regiao];
}

static void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int lerData(DataHora* dt) {
    int campos_lidos = scanf("%2d%2d%4d", &dt->dia, &dt->mes, &dt->ano);
    limparBuffer();
    if (campos_lidos == 3) {
        printf("  -> %02d/%02d/%04d\n", dt->dia, dt->mes, dt->ano);
    }
    if (campos_lidos != 3 || dt->dia < 1 || dt->dia > 31 || dt->mes < 1 || dt->mes > 12 || dt->ano < 1900) {
        return 0;
    }
    return 1;
}

void iniciarSistema() {
    NoAVL* raiz = NULL;

    while (1) {
        int opcaoPrincipal;

        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Gerenciamento de Eventos\n");
        printf("2. Consultas Avancadas\n");
        printf("3. Metricas do Sistema\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: \n");

        if (scanf("%d", &opcaoPrincipal) != 1) {
            printf("Opção inválida!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();
        printf("  -> %d\n", opcaoPrincipal);

        if (opcaoPrincipal == 1) {
            int opcaoGerenciamento;

            printf("\n--- Gerenciamento de Eventos ---\n");
            printf("1. Cadastrar Novo Evento\n");
            printf("2. Alterar Status de um Evento (Ativo -> Resolvido)\n");
            printf("3. Atualizar Severidade\n");
            printf("4. Remover Evento (Regra: apenas resolvidos)\n");
            printf("Escolha uma opcao: \n");

            if (scanf("%d", &opcaoGerenciamento) != 1) {
                printf("Opção inválida!\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            printf("  -> %d\n", opcaoGerenciamento);

            if (opcaoGerenciamento == 1) {
                char continuar = 'S';
                do {
                    int id;
                    int tipoInt;
                    int campos_lidos;
                    int cadastroCancelado = 0;
                    TipoEvento tipo;
                    int severidade;
                    DataHora data;
                    int regiaoInt;
                    Regiao regiao;

                    printf("ID do evento: \n");
                    if (scanf("%d", &id) != 1) {
                        printf("Opção inválida!\n");
                        limparBuffer();
                        continue;
                    }
                    limparBuffer();
                    printf("  -> %d\n", id);

                    printf("Tipo (0=TRANSITO, 1=SEMAFORO, 2=ENERGIA, 3=ALAGAMENTO, 4=INCENDIO): \n");
                    if (scanf("%d", &tipoInt) != 1) {
                        printf("Opção inválida!\n");
                        limparBuffer();
                        continue;
                    }
                    limparBuffer();
                    printf("  -> %d\n", tipoInt);

                    if (tipoInt < 0 || tipoInt > 4) {
                        printf("Opção inválida!\n");
                        continue;
                    }
                    tipo = (TipoEvento)tipoInt;

                    {
                        int sev_lida;
                        do {
                            printf("Severidade (1 a 5): ");
                            if (scanf("%d", &sev_lida) != 1) {
                                printf("Valor invalido! A severidade deve ser entre 1 e 5.\n");
                                limparBuffer();
                                sev_lida = 0;
                            } else {
                                limparBuffer();
                                if (sev_lida < 1 || sev_lida > 5) {
                                    printf("Valor invalido! A severidade deve ser entre 1 e 5.\n");
                                }
                            }
                        } while (sev_lida < 1 || sev_lida > 5);
                        severidade = sev_lida;
                    }

                    printf("Data (DDMMAAAA): \n");
                    if (!lerData(&data)) {
                        printf("Data invalida. Operacao cancelada.\n");
                        cadastroCancelado = 1;
                    }
                    if (cadastroCancelado) {
                        continuar = 'N';
                        continue;
                    }

                    do {
                        printf("Hora (HHMM): \n");
                        campos_lidos = scanf("%2d%2d", &data.hora, &data.minuto);
                        limparBuffer();
                        if (campos_lidos != 2 || data.hora < 0 || data.hora > 23 || data.minuto < 0 || data.minuto > 59) {
                            printf("Hora invalida! Digite 4 numeros (HHMM).\n");
                            campos_lidos = 0;
                        } else {
                            printf("  -> %02d:%02d\n", data.hora, data.minuto);
                        }
                    } while (campos_lidos != 2);

                    printf("Regiao (0=NORTE, 1=SUL, 2=LESTE, 3=OESTE, 4=CENTRO): \n");
                    if (scanf("%d", &regiaoInt) != 1) {
                        printf("Opção inválida!\n");
                        limparBuffer();
                        continue;
                    }
                    limparBuffer();
                    printf("  -> %d\n", regiaoInt);

                    if (regiaoInt < 0 || regiaoInt > 4) {
                        printf("Opção inválida!\n");
                        continue;
                    }
                    regiao = (Regiao)regiaoInt;

                    raiz = inserirNo(raiz, id, tipo, severidade, data, regiao, ATIVO);
                    printf("\nEvento cadastrado com sucesso! Deseja cadastrar outro evento? (S/N): \n");
                    scanf(" %c", &continuar);
                    limparBuffer();
                    printf("  -> %c\n", continuar);
                } while (continuar == 'S' || continuar == 's');
            } else if (opcaoGerenciamento == 2) {
                int id;

                printf("ID do evento para alterar status: \n");
                if (scanf("%d", &id) != 1) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", id);

                alterarStatus(raiz, id, RESOLVIDO);
            } else if (opcaoGerenciamento == 3) {
                int id;
                int nova_sev;

                printf("ID do evento para atualizar severidade: \n");
                if (scanf("%d", &id) != 1) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", id);

                do {
                    printf("Nova Severidade (1 a 5): ");
                    if (scanf("%d", &nova_sev) != 1) {
                        printf("Valor invalido! A severidade deve ser entre 1 e 5.\n");
                        limparBuffer();
                        nova_sev = 0;
                    } else {
                        limparBuffer();
                        if (nova_sev < 1 || nova_sev > 5) {
                            printf("Valor invalido! A severidade deve ser entre 1 e 5.\n");
                        }
                    }
                } while (nova_sev < 1 || nova_sev > 5);

                atualizarSeveridade(raiz, id, nova_sev);
            } else if (opcaoGerenciamento == 4) {
                /*
                 * Trava na interface: só chama removerNo se o evento existir e estiver RESOLVIDO.
                 * Eventos ATIVOS não podem ser apagados (regra de negócio). A função removerNo em
                 * avl.c repete a verificação de ATIVO como camada extra de segurança.
                 */
                int id;
                NoAVL* evento;

                printf("ID do evento para remover: \n");
                if (scanf("%d", &id) != 1) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", id);

                evento = buscarNo(raiz, id);
                if (evento == NULL) {
                    printf("Evento nao encontrado.\n");
                } else if (evento->status != RESOLVIDO) {
                    printf("Remocao permitida apenas para eventos resolvidos.\n");
                } else {
                    raiz = removerNo(raiz, id);
                }
            } else {
                printf("Opção inválida!\n");
            }
        } else if (opcaoPrincipal == 2) {
            int opcaoConsulta;

            printf("\n--- Consultas Avancadas ---\n");
            printf("1. Busca de Evento por ID\n");
            printf("2. Listar Ativos por Severidade (min e max)\n");
            printf("3. Relatorio por Regiao\n");
            printf("4. Busca por Intervalo de ID\n");
            printf("Escolha uma opcao: \n");

            if (scanf("%d", &opcaoConsulta) != 1) {
                printf("Opção inválida!\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            printf("  -> %d\n", opcaoConsulta);

            if (opcaoConsulta == 1) {
                int id;
                NoAVL* evento;

                printf("ID do evento: \n");
                if (scanf("%d", &id) != 1) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", id);

                evento = buscarNo(raiz, id);
                if (evento == NULL) {
                    printf("Evento nao encontrado.\n");
                } else {
                    printf("Evento encontrado: ID=%d, Severidade=%d, Regiao=%s, Status=%d\n",
                           evento->id, evento->severidade, regiaoParaTexto(evento->regiao), evento->status);
                }
            } else if (opcaoConsulta == 2) {
                int min;
                int max;

                printf("Severidade minima e maxima: \n");
                if (scanf("%d %d", &min, &max) != 2) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", min);
                printf("  -> %d\n", max);

                listarPorSeveridade(raiz, min, max);
            } else if (opcaoConsulta == 3) {
                int regiaoInt;
                Regiao regiao;

                printf("Regiao (0=NORTE, 1=SUL, 2=LESTE, 3=OESTE, 4=CENTRO): \n");
                if (scanf("%d", &regiaoInt) != 1) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", regiaoInt);

                if (regiaoInt < 0 || regiaoInt > 4) {
                    printf("Opção inválida!\n");
                    continue;
                }
                regiao = (Regiao)regiaoInt;

                relatorioPorRegiao(raiz, regiao);
            } else if (opcaoConsulta == 4) {
                int minId;
                int maxId;

                printf("ID minimo e ID maximo: \n");
                if (scanf("%d %d", &minId, &maxId) != 2) {
                    printf("Opção inválida!\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("  -> %d\n", minId);
                printf("  -> %d\n", maxId);

                listarPorIntervaloID(raiz, minId, maxId);
            } else {
                printf("Opção inválida!\n");
            }
        } else if (opcaoPrincipal == 3) {
            exibirMetricas(raiz);
        } else if (opcaoPrincipal == 0) {
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }
}
