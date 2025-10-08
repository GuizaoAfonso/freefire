#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMP 20

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;
} Componente;

Componente torre[MAX_COMP];
int totalComp = 0;
int ordenadoPorNome = 0; // Flag para controle da ordenação
int comparacoes = 0;     // Contador global de comparações

// ===================== FUNÇÕES AUXILIARES =====================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void linhaTabela() {
    printf("\n -----------------------------------------------------------------------\n");
}

void linhaMeio() {
    printf("\n -----------------------------------------------------------------------\n");
}

void linhaFinal() {
    printf("\n -----------------------------------------------------------------------\n");
}

void mostrarComponentes(Componente v[], int n) {
    if (n == 0) {
        printf("\n Nenhum componente cadastrado!\n");
        return;
    }

    printf("\n===================== INVENTÁRIO DA MOCHILA =====================\n");
    linhaTabela();
    printf("║ %-20s ║ %-20s ║ %-20s ║ %-20s ║\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    linhaMeio();

    for (int i = 0; i < n; i++) {
        printf("║ %-20s ║ %-20s ║ %-20d ║ %-20d ║\n",
               v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
    }

    linhaFinal();
    printf("Total de componentes: %d/%d\n", n, MAX_COMP);
}

// ===================== ORDENAÇÕES =====================
void bubbleSortNome(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
    ordenadoPorNome = 1;
}

void insertionSortTipo(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente key = v[i];
        int j = i - 1;
        while (j >= 0 && strcmp(v[j].tipo, key.tipo) > 0) {
            comparacoes++;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
    ordenadoPorNome = 0;
}

void selectionSortPrioridade(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade < v[min].prioridade)
                min = j;
        }
        if (min != i) {
            Componente temp = v[i];
            v[i] = v[min];
            v[min] = temp;
        }
    }
    ordenadoPorNome = 0;
}

// ===================== MEDIÇÃO DE TEMPO =====================
void medirTempo(void (*algoritmo)(Componente[], int), Componente v[], int n, const char *nomeAlgoritmo) {
    clock_t inicio = clock();
    algoritmo(v, n);
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n Medição de desempenho - %s\n", nomeAlgoritmo);
    printf(" Comparações realizadas: %d\n", comparacoes);
    printf("  Tempo de execução: %.6f segundos\n", tempo);
}

// ===================== BUSCA BINÁRIA =====================
int buscaBinariaPorNome(Componente v[], int n, char chave[]) {
    int inicio = 0, fim = n - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(chave, v[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else inicio = meio + 1;
    }
    return -1;
}

// ===================== CADASTRAR =====================
void cadastrarComponente() {
    if (totalComp >= MAX_COMP) {
        printf("\n A mochila está cheia! Não é possível adicionar mais componentes.\n");
        return;
    }

    Componente novo;
    int escolhaTipo;

    printf("\n-- Coletando Novo Componente --\n");

    printf("Nome: ");
    limparBuffer();
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Tipo (1 - Estrutural | 2 - Energia | 3 - Eletronico): ");
    scanf("%d", &escolhaTipo);

    switch (escolhaTipo) {
        case 1: strcpy(novo.tipo, "Estrutural"); break;
        case 2: strcpy(novo.tipo, "Energia"); break;
        case 3: strcpy(novo.tipo, "Eletronico"); break;
        default: strcpy(novo.tipo, "Desconhecido"); break;
    }

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Prioridade de Montagem (1-5): ");
    scanf("%d", &novo.prioridade);

    torre[totalComp++] = novo;

    printf("\n Componente '%s' adicionado com sucesso!\n", novo.nome);
    mostrarComponentes(torre, totalComp);
}

 //===================== DESCARTAR =====================
void descartarComponente() {
    if (totalComp == 0) {
        printf("\n Nenhum componente para descartar!\n");
        return;
    }

    char nome[30];
    printf("\nDigite o nome do componente a descartar: ");
    limparBuffer();
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int encontrado = -1;
    for (int i = 0; i < totalComp; i++) {
        if (strcmp(torre[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n Componente não encontrado!\n");
    } else {
        for (int i = encontrado; i < totalComp - 1; i++) {
            torre[i] = torre[i + 1];
        }
        totalComp--;
        printf("\n Componente '%s' descartado com sucesso!\n", nome);
    }
}

// ===================== SUBMENU DE ORDENAÇÃO =====================
void organizarMochila() {
    if (totalComp == 0) {
        printf("\n Nenhum componente para organizar!\n");
        return;
    }

    int opc;
    printf("\n======  Opções de Ordenação ======\n");
    printf("1 - por Nome\n");
    printf("2 - por Tipo\n");
    printf("3 - por Prioridade\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opc);

    switch (opc) {
        case 1: medirTempo(bubbleSortNome, torre, totalComp, "por Nome"); break;
        case 2: medirTempo(insertionSortTipo, torre, totalComp, "por Tipo"); break;
        case 3: medirTempo(selectionSortPrioridade, torre, totalComp, "por Prioridade"); break;
        default: printf("\n Opção inválida!\n"); return;
    }

    printf("\n Mochila organizada com sucesso!\n");
    mostrarComponentes(torre, totalComp);
}

// ===================== BUSCA BINÁRIA (ITEM 5) =====================
void buscarComponente() {
    if (totalComp == 0) {
        printf("\n Nenhum componente na mochila!\n");
        return;
    }

    //Verifica se a mochila está ordenada por nome antes de permitir a busca
    if (!ordenadoPorNome) {
        printf("\n A busca binária só pode ser realizada após ordenar os componentes por nome!\n");
        printf(" Vá até o MENU 4 e escolha (por Nome) para ordenar.\n");
        return;
    }

    char chave[30];
    printf("\n Digite o nome do componente-chave: ");
    limparBuffer();
    fgets(chave, sizeof(chave), stdin);
    chave[strcspn(chave, "\n")] = 0;

    int pos = buscaBinariaPorNome(torre, totalComp, chave);
    if (pos != -1) {
        printf("\n Componente encontrado! Pronto para montagem:\n");
        printf("   Nome: %s | Tipo: %s | Quantidade: %d | Prioridade: %d\n",
               torre[pos].nome, torre[pos].tipo, torre[pos].quantidade, torre[pos].prioridade);
    } else {
        printf("\n Componente não encontrado.\n");
    }
}

//===================== MAIN =====================
int main() {
    int opcao;

    do {
        printf("\n===========  MISSÃO FINAL: TORRE DE FUGA ===========\n");
        printf("Itens na mochila: %d/%d\n", totalComp, MAX_COMP);
        printf("--------------------------------------------------------\n");
        printf("1 - Cadastrar componente\n");
        printf("2 - Descartar componente\n");
        printf("3 - Listar componentes\n");
        printf("4 - Organizar mochila (ordenar)\n");
        printf("5 - Buscar binária por componente-chave (nome)\n");
        printf("0 - Ativar torre de fuga (sair)\n");
        printf("--------------------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarComponente(); break;
            case 2: descartarComponente(); break;
            case 3: mostrarComponentes(torre, totalComp); break;
            case 4: organizarMochila(); break;
            case 5: buscarComponente(); break;
            case 0: printf("\n Torre de fuga ativada! Fim da missão!\n"); break;
            default: printf("\n Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
