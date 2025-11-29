#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Sala {
    char *nome;
    struct Sala *esq;
    struct Sala *dir;
} Sala;

char *copiaStr(const char *s) {
    char *p = malloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}

/* cria nó da arvore */
Sala *criarSala(const char *nome, Sala *e, Sala *d) {
    Sala *s = malloc(sizeof(Sala));
    s->nome = copiaStr(nome);
    s->esq = e;
    s->dir = d;
    return s;
}

/* libera memoria da arvore */
void freeSala(Sala *r) {
    if (!r) return;
    freeSala(r->esq);
    freeSala(r->dir);
    free(r->nome);
    free(r);
}

char lerOpcao() {
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin)) return 's';
    for (int i = 0; buf[i]; i++) {
        if (!isspace((unsigned char)buf[i])) {
            return tolower(buf[i]);
        }
    }
    return '\0';
}

/* navega pela mansao */
void explorarSalas(Sala *inicio) {
    Sala *atual = inicio;
    const char *visitadas[200];
    int qtd = 0;

    while (atual) {
        visitadas[qtd++] = atual->nome;
        printf("\nVoce esta em: %s\n", atual->nome);

        if (!atual->esq && !atual->dir) {
            printf("Nao ha mais caminhos.\n");
            break;
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

    printf("\nCaminho percorrido:\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. %s\n", i+1, visitadas[i]);
    }
}

int main() {
    /* cria mapa fixo da mansao */
    Sala *bib = criarSala("Biblioteca", NULL, NULL);
    Sala *coz = criarSala("Cozinha", NULL, NULL);
    Sala *jardim = criarSala("Jardim", NULL, NULL);
    Sala *oficina = criarSala("Oficina", NULL, NULL);
    Sala *quarto = criarSala("Quarto dos Segredos", NULL, NULL);

    Sala *estar = criarSala("Sala de Estar", bib, coz);
    Sala *tempo = criarSala("Sala do Tempo", oficina, quarto);
    Sala *corredor = criarSala("Corredor", tempo, jardim);

    Sala *hall = criarSala("Hall de Entrada", estar, corredor);

    explorarSalas(hall);

    freeSala(hall);
    return 0;
}
