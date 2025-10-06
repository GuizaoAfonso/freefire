#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constante
#define MAX_ITENS 10

// Estrutura do item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Estrutura do nó da lista
typedef struct No {
    Item item;
    struct No* prox;
} No;

// Ponteiro para o início da lista
No* inicio = NULL;
int totalItens = 0;

// -------------- Funções auxiliares ----------------

// Pausar a tela
void pressionarEnter() {
    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar();
}

// Listar itens em formato de 3 colunas
void listarItensColunas() {
    if (inicio == NULL) {
        printf("\n Mochila vazia!\n");
        return;
    }

    printf("\n Itens na mochila:\n");

    No* atual = inicio;
    int i = 1;
    while (atual != NULL) {
        printf("%d) %s | %s | %d\t", i, atual->item.nome, atual->item.tipo, atual->item.quantidade);
        if (i % 3 == 0) printf("\n");
        atual = atual->prox;
        i++;
    }
    printf("\n");
}

// --------------- Operações principais --------------

// Inserir item no final da lista
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n A mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\nDigite o nome do item: ");
    scanf(" %[^\n]", novo.nome);
    printf("Digite o tipo do item (arma, munição, cura...): ");
    scanf(" %[^\n]", novo.tipo);
    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    // Cria novo nó
    No* novoNo = (No*) malloc(sizeof(No));
    novoNo->item = novo;
    novoNo->prox = NULL;

    // Inserção no final
    if (inicio == NULL) {
        inicio = novoNo;
    } else {
        No* atual = inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoNo;
    }

    totalItens++;
    printf("\n Item adicionado com sucesso!\n");
    listarItensColunas();
    pressionarEnter();
}

// Remover item pelo nome
void removerItem() {
    if (inicio == NULL) {
        printf("\n Mochila vazia, não há o que remover.\n");
        return;
    }

    char nome[30];
    printf("\nDigite o nome do item a ser removido: ");
    scanf(" %[^\n]", nome);

    No* atual = inicio;
    No* anterior = NULL;

    while (atual != NULL && strcmp(atual->item.nome, nome) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("\n Item não encontrado!\n");
        return;
    }

    if (anterior == NULL) {
        inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    totalItens--;

    printf("\n Item removido com sucesso!\n");
    listarItensColunas();
    pressionarEnter();
}

// Buscar item pelo nome
void buscarItem() {
    if (inicio == NULL) {
        printf("\n Mochila vazia, não há itens para buscar.\n");
        return;
    }

    char nome[30];
    printf("\nDigite o nome do item a ser buscado: ");
    scanf(" %[^\n]", nome);

    No* atual = inicio;
    int encontrado = 0;

    while (atual != NULL) {
        if (strcmp(atual->item.nome, nome) == 0) {
            printf("\n Item encontrado:\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   atual->item.nome, atual->item.tipo, atual->item.quantidade);
            encontrado = 1;
            break;
        }
        atual = atual->prox;
    }

    if (!encontrado)
        printf("\n Item não encontrado!\n");

    pressionarEnter();
}

// Listar todos os itens
void listarItens() {
    listarItensColunas();
    pressionarEnter();
}

// ------------- Programa Principal ---------------
int main() {
    int opcao;

    do {
        printf("\n===  Mochila ===\n\n");
        printf("Itens na mochila: %d/%d\n\n", totalItens, MAX_ITENS);
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome\n");
        printf("0 - Sair\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\n Saindo do sistema... Até mais!\n");
                break;
            default:
                printf("\n Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    // Libera memória antes de sair
    No* atual = inicio;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;
}
