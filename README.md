# Smart City: Monitoramento de Eventos Críticos

## Descrição do Projeto

O **Smart City** é um sistema desenvolvido em **C** para o monitoramento e gestão de eventos críticos urbanos (acidentes de trânsito, falhas em semáforos, interrupções de energia, alagamentos e incêndios). 

O núcleo estrutural do sistema é uma **Árvore AVL**, onde cada evento é armazenado como um nó indexado pelo seu **ID numérico**. O uso da AVL garante que o sistema se mantenha auto-balanceado durante as inserções e remoções, otimizando o tempo de resposta das operações de busca e relatório.

O sistema possui um menu interativo que permite o cadastro de eventos, atualização de status e severidade, além de consultas com filtros específicos e visualização de métricas de desempenho da própria estrutura de dados (como fator de balanceamento e total de rotações).

## Funcionalidades Implementadas

* **Gerenciamento AVL:** Inserção, cálculo dinâmico de fator de balanceamento, rotações automáticas (LL, RR, LR, RL) e remoção (protegida por regra de negócio: apenas eventos "Resolvidos" podem ser removidos).
* **Consultas Avançadas:** 
  * Busca exata por ID.
  * Listagem de eventos ativos por intervalo de severidade.
  * Relatório de eventos ativos filtrados por Região (impressos em-ordem pelo ID).
  * Busca de eventos por intervalo de ID.
* **Operações de Atualização:** Mudança de status (Ativo para Resolvido) e alteração do nível de severidade (com validação de limites de 1 a 5).
* **Métricas do Sistema:** Exibição em tempo real da altura da árvore, número total de nós, contagem de eventos ativos, fator de balanceamento médio e quantidade total de rotações executadas desde o início.

## Estrutura do Código

| Arquivo | Responsabilidade |
|------------|--------|
| `src/avl.c` | Lógica central da Árvore AVL (inserção, remoção, rotações e balanceamento). |
| `src/queries.c` | Algoritmos de travessia (ex: em-ordem) para geração de relatórios e consultas filtradas. |
| `src/ui.c` | Interface textual, menus e validação de entrada de dados do usuário. |
| `src/metrics.c` | Cálculo e agregação das estatísticas da árvore. |
| `src/main.c` | Ponto de entrada do programa. |

## Instruções de Compilação e Execução

Este projeto não utiliza bibliotecas externas e requer apenas um compilador C padrão (como o GCC) e o utilitário Make.

**1. Compilar o projeto**
No terminal, navegue até a raiz do projeto (onde está localizado o arquivo `Makefile`) e execute:
```bash
mingw32-make
