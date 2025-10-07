#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMP 20
#define BUF 128

// larguras mínimas (para reproduzir o espaçamento do anexo)
#define MIN_NAME_W 16
#define MIN_TYPE_W 13
#define MIN_QTY_W 10
#define MIN_PRIO_W 10

typedef struct {
    char nome[50];
    char tipo[30];
    int quantidade;
    int prioridade;
} Componente;

Componente torre[MAX_COMP];
int totalComp = 0;

/* lê linha segura e remove newline */
void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) { buf[0] = '\0'; return; }
    size_t len = strcspn(buf, "\n");
    buf[len] = '\0';
}

/* lê int com prompt (usa fgets internamente) */
int read_int(const char *prompt) {
    char tmp[BUF];
    while (1) {
        printf("%s", prompt);
        if (fgets(tmp, sizeof(tmp), stdin) == NULL) return 0;
        // permitir entrada vazia -> continua pedindo
        if (tmp[0] == '\n') continue;
        int v = atoi(tmp);
        return v;
    }
}

/* pausa esperando ENTER */
void press_enter() {
    char tmp[BUF];
    printf("\nPressione Enter para continuar...");
    read_line(tmp, sizeof(tmp));
}

/* calcula número de dígitos de um inteiro (para largura) */
int int_len(int v) {
    char s[32];
    snprintf(s, sizeof(s), "%d", v);
    return (int)strlen(s);
}

/* mostra inventário alinhado como no anexo */
void mostrarComponentes(Componente v[], int n) {
    if (n == 0) {
        printf("\n--- INVENTARIO ATUAL (0/%d) ---\n", MAX_COMP);
        printf("--------------------------------------------------------------\n");
        printf("NOME             | TIPO          | QUANTIDADE | PRIORIDADE\n");
        printf("--------------------------------------------------------------\n");
        printf("--------------------------------------------------------------\n");
        return;
    }

    // calcula larguras com base no conteúdo, respeitando mínimas
    int nameW = MIN_NAME_W;
    int typeW = MIN_TYPE_W;
    int qtyW  = MIN_QTY_W;
    int prioW = MIN_PRIO_W;

    for (int i = 0; i < n; i++) {
        int ln = (int)strlen(v[i].nome);
        if (ln > nameW) nameW = ln;
        int lt = (int)strlen(v[i].tipo);
        if (lt > typeW) typeW = lt;
        int lq = int_len(v[i].quantidade);
        if (lq > qtyW) qtyW = lq;
        int lp = int_len(v[i].prioridade);
        if (lp > prioW) prioW = lp;
    }

    // garantir mínimo para os títulos
    if ((int)strlen("NOME") > nameW) nameW = (int)strlen("NOME");
    if ((int)strlen("TIPO") > typeW) typeW = (int)strlen("TIPO");
    if ((int)strlen("QUANTIDADE") > qtyW) qtyW = (int)strlen("QUANTIDADE");
    if ((int)strlen("PRIORIDADE") > prioW) prioW = (int)strlen("PRIORIDADE");

    // largura total da linha de traços (considera " | " entre colunas)
    int totalLen = nameW + 3 + typeW + 3 + qtyW + 3 + prioW;

    // Cabeçalho com contagem
    printf("\n--- INVENTARIO ATUAL (%d/%d) ---\n", n, MAX_COMP);

    // linha de traços
    for (int i = 0; i < totalLen; i++) putchar('-');
    putchar('\n');

    // títulos
    // usamos formatações com larguras calculadas (left-aligned)
    printf("%-*s | %-*s | %-*s | %-*s\n",
           nameW, "NOME",
           typeW, "TIPO",
           qtyW, "QUANTIDADE",
           prioW, "PRIORIDADE");

    // linha de traços
    for (int i = 0; i < totalLen; i++) putchar('-');
    putchar('\n');

    // linhas de dados
    for (int i = 0; i < n; i++) {
        printf("%-*s | %-*s | %-*d | %-*d\n",
               nameW, v[i].nome,
               typeW, v[i].tipo,
               qtyW, v[i].quantidade,
               prioW, v[i].prioridade);
    }

    // linha final de traços
    for (int i = 0; i < totalLen; i++) putchar('-');
    putchar('\n');
}

/* cadastro conforme solicitado (tipo: 1-estrutural 2-energia 3-eletronico) */
void cadastrarComponente() {
    if (totalComp >= MAX_COMP) {
        printf("\n⚠️ Limite atingido (%d).\n", MAX_COMP);
        press_enter();
        return;
    }

    Componente novo;
    // Nome
    printf("\n--- Coletando Novo Componente ---\n");
    printf("Nome: ");
    read_line(novo.nome, sizeof(novo.nome));
    // Tipo
    int t;
    while (1) {
        t = read_int("Tipo (1 - estrutural | 2 - energia | 3 - eletronico): ");
        if (t >= 1 && t <= 3) break;
        printf("Escolha inválida. Digite 1, 2 ou 3.\n");
    }
    if (t == 1) strcpy(novo.tipo, "estrutural");
    else if (t == 2) strcpy(novo.tipo, "energia");
    else strcpy(novo.tipo, "eletronico");

    // Quantidade
    while (1) {
        novo.quantidade = read_int("Quantidade: ");
        if (novo.quantidade >= 0) break;
        printf("Quantidade inválida. Digite um número >= 0.\n");
    }

    // Prioridade 1-5
    while (1) {
        novo.prioridade = read_int("Prioridade de Montagem (1-5): ");
        if (novo.prioridade >= 1 && novo.prioridade <= 5) break;
        printf("Prioridade inválida. Digite entre 1 e 5.\n");
    }

    torre[totalComp++] = novo;
    printf("\nComponente '%s' adicionado!\n", novo.nome);
    mostrarComponentes(torre, totalComp);
    press_enter();
}

/* descartar pelo nome (exatamente igual) */
void descartarComponente() {
    if (totalComp == 0) {
        printf("\n⚠️ Nenhum componente para descartar!\n");
        press_enter();
        return;
    }
    char nome[50];
    printf("\nDigite o nome do componente a descartar: ");
    read_line(nome, sizeof(nome));

    int idx = -1;
    for (int i = 0; i < totalComp; i++) {
        if (strcmp(torre[i].nome, nome) == 0) { idx = i; break; }
    }
    if (idx == -1) {
        printf("\n❌ Componente não encontrado!\n");
    } else {
        for (int j = idx; j < totalComp - 1; j++) torre[j] = torre[j+1];
        totalComp--;
        printf("\n🗑️ Componente '%s' descartado com sucesso!\n", nome);
    }
    mostrarComponentes(torre, totalComp);
    press_enter();
}

/* main: menu simples usando read_int para evitar mistura de scanf/fgets */
int main() {
    int opc;
    while (1) {
        printf("\n=========== 🛰️ MISSÃO FINAL: TORRE DE FUGA ===========\n");
        printf("Itens na mochila: %d/%d\n", totalComp, MAX_COMP);
        printf("1 - Cadastrar componente\n");
        printf("2 - Descartar componente\n");
        printf("3 - Listar componentes\n");
        printf("4 - Organizar mochila (ordenar)\n");
        printf("5 - Buscar componente-chave (binária)\n");
        printf("0 - Ativar torre de fuga (sair)\n");
        opc = read_int("Escolha uma opção: ");

        if (opc == 1) cadastrarComponente();
        else if (opc == 2) descartarComponente();
        else if (opc == 3) { mostrarComponentes(torre, totalComp); press_enter(); }
        else if (opc == 4) { printf("\n(ordenacao disponível: opcao 4 ainda implementável)\n"); press_enter(); }
        else if (opc == 5) { printf("\n(busca binária: opcao 5 ainda implementável)\n"); press_enter(); }
        else if (opc == 0) { printf("\n🚀 Torre de fuga ativada! Fim da missão!\n"); break; }
        else { printf("\nOpção inválida!\n"); press_enter(); }
    }
    return 0;
}
