#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_ITENS 10

//Estrutura para representar um item do inventário
typedef struct {
    char nome[30];   // Nome do item
    char tipo[20];   // Tipo do item (ex: arma, munição, cura)
    int quantidade;  // Quantidade do item
} Item;

// Vetor de itens (mochila)
Item mochila[MAX_ITENS];
int totalItens = 0; // Contador de itens cadastrados

// --- Funções -----
// Função para inserir um item na mochila
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n A mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\nDigite o nome do item: ");
    scanf(" %[^\n]", novo.nome); // lê string com espaço

    printf("Digite o tipo do item (arma, munição, cura...): ");
    scanf(" %[^\n]", novo.tipo);

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    mochila[totalItens] = novo;
    totalItens++;

    printf("\n Item cadastrado com sucesso!\n");
}

// Função para remover o item pelo nome
void removerItem() {
    if (totalItens == 0) {
        printf("\n A mochila está vazia, não há o que remover.\n");
        return;
    }

    char nome[30];
    printf("\nDigite o nome do item a ser removido: ");
    scanf(" %[^\n]", nome);

    int encontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n Item não encontrado!\n");
    } else {
        // Desloca os itens para "fechar o buraco"
        for (int i = encontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        printf("\n Item removido com sucesso!\n");
    }
}

//Função para listar todos os itens da mochila
void listarItens() {
    if (totalItens == 0) {
        printf("\n Mochila vazia!\n");
        return;
    }

    printf("\n Itens na mochila:\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função de busca sequencial por nome
void buscarItem() {
    if (totalItens == 0) {
        printf("\n A mochila está vazia, não há itens para buscar.\n");
        return;
    }

    char nome[30];
    printf("\nDigite o nome do item para buscar: ");
    scanf(" %[^\n]", nome);

    int encontrado = 0;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("\n Item encontrado:\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n Item não encontrado!\n");
    }
}

// ---------- Programa principal --------------
int main() {
    int opcao;

    do {
        printf("\n=== Sistema de Inventário ===\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                listarItens();
                break;
            case 2:
                removerItem();
                listarItens();
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

    return 0;
}
