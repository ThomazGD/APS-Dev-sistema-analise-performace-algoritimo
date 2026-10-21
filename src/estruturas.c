#include "aps.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Trabalho {
    Imagem *dados;
    size_t n;
    struct Trabalho *proximo;
};

struct ResultadoNo {
    Resultado valor;
    struct ResultadoNo *anterior;
};

void fila_iniciar(FilaTrabalhos *fila) {
    fila->primeiro = fila->ultimo = NULL;
    fila->quantidade = 0;
}

int fila_enfileirar(FilaTrabalhos *fila, const Imagem *dados, size_t n) {
    if (!dados || n == 0 || n > SIZE_MAX / sizeof *dados) return 0;
    Trabalho *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->dados = malloc(n * sizeof *dados);
    if (!novo->dados) { free(novo); return 0; }
    memcpy(novo->dados, dados, n * sizeof *dados);
    novo->n = n;
    novo->proximo = NULL;
    if (fila->ultimo) fila->ultimo->proximo = novo;
    else fila->primeiro = novo;
    fila->ultimo = novo;
    ++fila->quantidade;
    return 1;
}

int fila_desenfileirar(FilaTrabalhos *fila, Imagem **dados, size_t *n) {
    Trabalho *primeiro = fila->primeiro;
    if (!primeiro) return 0;
    *dados = primeiro->dados; /* O chamador deve liberar os dados. */
    *n = primeiro->n;
    fila->primeiro = primeiro->proximo;
    if (!fila->primeiro) fila->ultimo = NULL;
    --fila->quantidade;
    free(primeiro);
    return 1;
}

void fila_limpar(FilaTrabalhos *fila) {
    Imagem *dados;
    size_t n;
    while (fila_desenfileirar(fila, &dados, &n)) free(dados);
}

void pilha_iniciar(PilhaResultados *pilha) {
    pilha->topo = NULL;
    pilha->quantidade = 0;
}

int pilha_empilhar(PilhaResultados *pilha, Resultado resultado) {
    ResultadoNo *novo = malloc(sizeof *novo);
    if (!novo) return 0;
    novo->valor = resultado;
    novo->anterior = pilha->topo;
    pilha->topo = novo;
    ++pilha->quantidade;
    return 1;
}

int pilha_consultar(const PilhaResultados *pilha, Resultado *resultado) {
    if (!pilha->topo) return 0;
    *resultado = pilha->topo->valor;
    return 1;
}

int pilha_desempilhar(PilhaResultados *pilha, Resultado *resultado) {
    if (!pilha_consultar(pilha, resultado)) return 0;
    ResultadoNo *antigo = pilha->topo;
    pilha->topo = antigo->anterior;
    --pilha->quantidade;
    free(antigo);
    return 1;
}

void pilha_limpar(PilhaResultados *pilha) {
    Resultado resultado;
    while (pilha_desempilhar(pilha, &resultado)) {}
}
