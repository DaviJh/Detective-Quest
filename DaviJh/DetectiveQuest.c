#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// --------- STRUCTS ---------

typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct NoPista {
    char pista[50];
    struct NoPista *esq;
    struct NoPista *dir;
} NoPista;

typedef struct HashItem {
    char pista[50];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

HashItem* tabelaHash[TAM_HASH];

// --------- FUNÇÕES OBRIGATÓRIAS ---------

// criarSala() – cria dinamicamente um cômodo.
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = nova->dir = NULL;
    return nova;
}

// inserirPista() – insere pista coletada na BST.
NoPista* inserirPista(NoPista *raiz, const char *p) {
    if (raiz == NULL) {
        NoPista *novo = (NoPista*) malloc(sizeof(NoPista));
        strcpy(novo->pista, p);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(p, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, p);
    else if (strcmp(p, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, p);
    return raiz;
}

// Função simples para gerar um número da hash
int hash(const char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % TAM_HASH;
}

// inserirNaHash() – insere associação pista/suspeito.
void inserirNaHash(const char *pista, const char *suspeito) {
    int h = hash(pista);

    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[h];
    tabelaHash[h] = novo;
}

// encontrarSuspeito() – consulta suspeito correspondente à pista.
char* encontrarSuspeito(const char *pista) {
    int h = hash(pista);
    HashItem *aux = tabelaHash[h];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

// Retorna a pista fixa da sala
char* pistaDaSala(const char *nome) {
    if (strcmp(nome, "Entrada") == 0) return "Pegada de sapato";
    if (strcmp(nome, "Sala de Estar") == 0) return "Luvas sujas";
    if (strcmp(nome, "Cozinha") == 0) return "Faca com arranhões";
    if (strcmp(nome, "Porão") == 0) return "Anotação suspeita";
    if (strcmp(nome, "Biblioteca") == 0) return "Livro rasgado";
    return NULL;
}

// explorarSalas() – navega pela árvore e coleta pistas.
void explorarSalas(Sala *atual, NoPista **colecao) {
    if (atual == NULL) return;

    printf("\nVocê entrou em: %s\n", atual->nome);

    char *p = pistaDaSala(atual->nome);
    if (p != NULL) {
        printf("Pista encontrada: %s\n", p);
        *colecao = inserirPista(*colecao, p);
    }

    char op;
    printf("\nEscolha: esquerda (e), direita (d) ou sair (s): ");
    scanf(" %c", &op);

    if (op == 'e') explorarSalas(atual->esq, colecao);
    else if (op == 'd') explorarSalas(atual->dir, colecao);
    else return;
}

// Percorre BST e conta quantas pistas apontam ao suspeito
int contarPistas(NoPista *raiz, const char *acusado) {
    if (raiz == NULL) return 0;

    int c = 0;
    char *s = encontrarSuspeito(raiz->pista);
    if (s != NULL && strcmp(s, acusado) == 0)
        c++;

    return c + contarPistas(raiz->esq, acusado) + contarPistas(raiz->dir, acusado);
}

// verificarSuspeitoFinal() – julgamento final.
void verificarSuspeitoFinal(NoPista *colecao) {
    char acusado[50];

    printf("\n==== FASE FINAL ====\n");
    printf("Digite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistas(colecao, acusado);

    if (qtd >= 2)
        printf("\nAcusação confirmada! %s é o culpado.\n", acusado);
    else
        printf("\nAcusação fraca! Pistas insuficientes contra %s.\n", acusado);
}

// Exibe pistas coletadas
void mostrarPistas(NoPista *r) {
    if (r == NULL) return;
    mostrarPistas(r->esq);
    printf("- %s\n", r->pista);
    mostrarPistas(r->dir);
}

// --------- MAIN ---------

int main() {

    // Montagem fixa da mansão
    Sala *entrada = criarSala("Entrada");
    Sala *sala = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *porao = criarSala("Porão");
    Sala *biblio = criarSala("Biblioteca");

    entrada->esq = sala;
    entrada->dir = cozinha;
    sala->esq = porao;
    sala->dir = biblio;

    // Popula tabela de suspeitos
    inserirNaHash("Pegada de sapato", "Arthur");
    inserirNaHash("Luvas sujas", "Beatriz");
    inserirNaHash("Faca com arranhões", "Arthur");
    inserirNaHash("Anotação suspeita", "Cláudio");
    inserirNaHash("Livro rasgado", "Beatriz");

    NoPista *colecao = NULL;

    printf("=== Detective Quest: Nivel Mestre ===\n");
    explorarSalas(entrada, &colecao);

    printf("\nPistas coletadas:\n");
    mostrarPistas(colecao);

    verificarSuspeitoFinal(colecao);

    return 0;
}
