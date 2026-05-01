# Smart City — Monitoramento de eventos críticos (Árvore AVL em C)

## Descrição geral do sistema

O **Smart City** é um sistema de monitoramento e gestão de **eventos críticos** urbanos (trânsito, semáforo, energia, alagamento, incêndio, etc.). Cada evento é armazenado como nó de uma **árvore AVL** em linguagem **C (C99)**, indexada pelo **identificador numérico (ID)**. A propriedade de balanceamento da AVL mantém operações típicas de inserção, busca e remoção com complexidade proporcional ao logaritmo do número de eventos (`O(log n)` em cenários balanceados).

O programa oferece menu interativo para cadastro, alteração de status e severidade, remoção com regras de negócio, consultas filtradas e métricas da estrutura (altura, contagens, fator médio de balanceamento e número de rotações).

## Instruções de compilação

**Pré-requisitos**

- Compilador **GCC** (`gcc`).
- Ferramenta **make** disponível no terminal (no Windows com MinGW, costuma-se usar **`mingw32-make`** ou **`make`**, conforme a instalação).

**Compilar**

Na pasta deste entregável (onde está o `Makefile`):

```bash
mingw32-make
```

Se o seu ambiente expuser o comando como `make` (por exemplo MSYS2, Git Bash ou Linux):

```bash
make
```

Isso gera o executável **`cidade_inteligente`** (no Windows, o GCC geralmente cria também **`cidade_inteligente.exe`** no mesmo diretório).

**Limpar artefatos de build**

```bash
mingw32-make clean
```

(o alvo `clean` deste projeto usa comandos orientados ao Windows.)

**Executar**

- Git Bash / MSYS / Linux:

```bash
./cidade_inteligente
```

- PowerShell ou **cmd** (Windows):

```powershell
.\cidade_inteligente.exe
```

Ou, se existir apenas o nome sem sufixo visível conforme o ambiente:

```powershell
.\cidade_inteligente
```

Também é possível compilar e em seguida tentar rodar pelo Makefile (**pode falhar no cmd puro do Windows** se `./cidade_inteligente` não resolver o `.exe`; nesse caso use o comando explícito acima):

```bash
mingw32-make run
```

## Funcionalidades

- **Cadastro de eventos** na AVL (novos eventos iniciam como **ATIVOS**).
- **Alteração de status** (por exemplo para **RESOLVIDO**).
- **Atualização de severidade** apenas para eventos ainda **ATIVOS**.
- **Remoção na árvore** permitida **somente** para eventos **RESOLVIDOS** (validação na interface e na rotina de remoção na AVL).
- **Consultas:**
  - busca por ID;
  - listagem de **ativos** por **intervalo de severidade**;
  - relatório por **região** (saída em **ordem crescente de ID** via percurso em-ordem);
  - listagem por **intervalo de ID** (com poda usando a ordenação da árvore).
- **Métricas:** altura da árvore, total de nós, quantidade de eventos ativos, **fator de balanceamento médio** entre os nós e **total de rotações** realizadas pela AVL desde o início da execução).

## Layout deste pacote

```
entrega/
├── Makefile
├── README.md
├── docs/
│   └── LEIA_ME.txt       # onde colocar relatório técnico e capturas de testes
└── src/                  # código-fonte (.c e .h)
```

## Estrutura relevante do código

| Componente | Papel |
|------------|--------|
| `src/avl.c` | Inserção, remoção, busca, rotações e balanceamento AVL |
| `src/queries.c` | Consultas avançadas e impressão formatada |
| `src/ui.c` | Menus e leitura de dados do usuário |
| `src/metrics.c` | Cálculo e exibição de métricas |
| `src/main.c` | Ponto de entrada (console UTF-8 no Windows) |

## Relatório técnico

O conteúdo do **relatório técnico** (documento PDF e/ou **capturas de testes realizados**) deve ser colocado na pasta **`docs/`**, conforme descrito em `docs/LEIA_ME.txt`.
