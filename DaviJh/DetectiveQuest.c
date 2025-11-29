#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Arvore da mansao --- */
typedef struct Sala {
    char *nome;
    char *pista;
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* --- BST das pistas --- */
typedef struct PistaNode {
    char *pista;
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* copia simples de string */
char *copiaStr(const char *s) {
    if (!s) return NULL;
    char *p = malloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}

/* cria sala com ou sem pista */
Sala *criarSala(const char *nome, const char *pista, Sala *e, Sala *d) {
    Sala *s = malloc(sizeof(Sala));
    s->nome = copiaStr(nome);
    s->pista = pista ? copiaStr(pista) : NULL;
    s->esq = e;
    s->dir = d;
    return s;
}

/* insere pista na BST */
PistaNode *inserirPista(PistaNode *raiz, const char *p) {
    if (!p) return raiz;
    if (!raiz) {
        raiz = malloc(sizeof(PistaNode));
        raiz->pista = copiaStr(p);
        raiz->esq = raiz->dir = NULL;
        return raiz;
    }
    int cmp = strcmp(p, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, p);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, p);
    return raiz;
}

/* exibe pistas em ordem alfabetica */
void exibirPistas(PistaNode *r) {
    if (!r) return;
    exibirPistas(r->esq);
    printf("- %s\n", r->pista);
    exibirPistas(r->dir);
}

/* libera arvore da mansao */
void freeSala(Sala *r) {
    if (!r) return;
    freeSala(r->esq);
    freeSala(r->dir);
    free(r->nome);
    if (r->pista) free(r->pista);
    free(r);
}

/* libera arvore de pistas */
void freeBST(PistaNode *r) {
    if (!r) return;
    freeBST(r->esq);
    freeBST(r->dir);
    free(r->pista);
    free(r);
}

char lerOpcao() {
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin)) return 's';
    for (int i = 0; buf[i]; i++)
        if (!isspace((unsigned char)buf[i]))
            return tolower(buf[i]);
    return '\0';
}

/* exploracao + coleta */
void explorarSalasComPistas(Sala *inicio, PistaNode **raizPistas) {
    Sala *atual = inicio;

    while (atual) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->pista) {
            printf("Pista encontrada: %s\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        printf("e = esquerda | d = direita | s = sair\nOpcao: ");
        char op = lerOpcao();

        if (op == 's') break;

        if (op == 'e') {
            if (atual->esq) atual = atual->esq;
            else printf("Sem caminho a esquerda.\n");
        } else if (op == 'd') {
            if (atual->dir) atual = atual->dir;
            else printf("Sem caminho a direita.\n");
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

int main() {
    /* mapa fixo com pistas */
    Sala *bib     = criarSala("Biblioteca",          "Livro rasgado", NULL, NULL);
    Sala *coz     = criarSala("Cozinha",             "Faca suja",     NULL, NULL);
    Sala *jardim  = criarSala("Jardim",              "Pegadas",       NULL, NULL);
    Sala *oficina = criarSala("Oficina",             "Parafuso solto",NULL, NULL);
    Sala *quarto  = criarSala("Quarto dos Segredos", "Diario aberto", NULL, NULL);

    Sala *estar   = criarSala("Sala de Estar", NULL, bib, coz);
    Sala *tempo   = criarSala("Sala do Tempo", NULL, oficina, quarto);
    Sala *corredor= criarSala("Corredor", "Corrente quebrada", tempo, jardim);

    Sala *hall    = criarSala("Hall de Entrada", "Mapa antigo", estar, corredor);

    PistaNode *pistas = NULL;

    explorarSalasComPistas(hall, &pistas);

    printf("\n--- Pistas coletadas ---\n");
    exibirPistas(pistas);

    freeSala(hall);
    freeBST(pistas);

    return 0;
}
