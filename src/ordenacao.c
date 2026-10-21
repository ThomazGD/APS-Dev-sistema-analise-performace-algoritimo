#include "aps.h"

void insertion_sort(Imagem *dados, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        Imagem atual = dados[i];
        size_t j = i;
        while (j > 0 && dados[j - 1].nuvens > atual.nuvens) {
            dados[j] = dados[j - 1];
            --j;
        }
        dados[j] = atual;
    }
}

static void intercalar(Imagem *dados, Imagem *aux, size_t inicio, size_t fim) {
    if (fim - inicio < 2) return;
    size_t meio = inicio + (fim - inicio) / 2;
    intercalar(dados, aux, inicio, meio);
    intercalar(dados, aux, meio, fim);
    size_t i = inicio, j = meio, k = inicio;
    while (i < meio && j < fim) {
        aux[k++] = dados[i].nuvens <= dados[j].nuvens ? dados[i++] : dados[j++];
    }
    while (i < meio) aux[k++] = dados[i++];
    while (j < fim) aux[k++] = dados[j++];
    for (k = inicio; k < fim; ++k) dados[k] = aux[k];
}

void merge_sort(Imagem *dados, Imagem *aux, size_t n) {
    intercalar(dados, aux, 0, n);
}

static void trocar(Imagem *a, Imagem *b) {
    Imagem tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Particao em tres grupos evita degradacao com muitas chaves repetidas. */
void quick_sort(Imagem *dados, size_t n) {
    while (n > 1) {
        int pivo = dados[n / 2].nuvens;
        size_t menor = 0, atual = 0, maior = n;
        while (atual < maior) {
            if (dados[atual].nuvens < pivo) {
                trocar(&dados[menor++], &dados[atual++]);
            } else if (dados[atual].nuvens > pivo) {
                trocar(&dados[atual], &dados[--maior]);
            } else {
                ++atual;
            }
        }
        /* Recursao apenas no grupo menor limita o uso da pilha. */
        if (menor < n - maior) {
            quick_sort(dados, menor);
            dados += maior;
            n -= maior;
        } else {
            quick_sort(dados + maior, n - maior);
            n = menor;
        }
    }
}
