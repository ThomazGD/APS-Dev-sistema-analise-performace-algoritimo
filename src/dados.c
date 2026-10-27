#include "aps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t carregar_csv(const char *caminho, Imagem *dados, size_t capacidade) {
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo");
        return 0;
    }
    char linha[256];
    size_t n = 0;
    if (!fgets(linha, sizeof linha, arquivo) ||
        strcmp(linha, "id,data,nuvens\n") != 0) {
        fprintf(stderr, "Cabecalho esperado: id,data,nuvens\n");
        fclose(arquivo);
        return 0;
    }
    while (fgets(linha, sizeof linha, arquivo)) {
        Imagem item;
        char extra;
        if (n == capacidade ||
            sscanf(linha, "%d,%d,%d %c", &item.id, &item.data, &item.nuvens, &extra) != 3 ||
            item.id <= 0 || item.data < 19000101 || item.data > 99991231 ||
            item.nuvens < 0 || item.nuvens > 100) {
            fprintf(stderr, "CSV invalido ou limite excedido na linha %zu.\n", n + 2);
            fclose(arquivo);
            return 0;
        }
        dados[n++] = item;
    }
    if (ferror(arquivo)) {
        fprintf(stderr, "Erro ao ler o CSV.\n");
        n = 0;
    }
    fclose(arquivo);
    return n;
}

void gerar_dados(Imagem *dados, size_t n) {
    gerar_cenario(dados, n, 1);
}

void gerar_cenario(Imagem *dados, size_t n, int cenario) {
    for (size_t i = 0; i < n; ++i) {
        dados[i].id = (int)i + 1;
        dados[i].data = 20260916;
        if (cenario == 2) dados[i].nuvens = n == 1 ? 0 : (int)(100 * i / (n - 1));
        else if (cenario == 3) dados[i].nuvens = n == 1 ? 0 : (int)(100 * (n - 1 - i) / (n - 1));
        else dados[i].nuvens = rand() % 101;
    }
}

static void imprimir_dados(const Imagem *dados, size_t n, size_t limite) {
    if (limite > n) limite = n;
    puts("ID\t\tDATA\t\tNUVENS (%)");
    for (size_t i = 0; i < limite; ++i)
        printf("%-8d\t%08d\t%3d\n", dados[i].id, dados[i].data, dados[i].nuvens);
    if (n > limite) printf("Exibindo %zu de %zu registros.\n", limite, n);
}

void mostrar_dados(const Imagem *dados, size_t n) {
    imprimir_dados(dados, n, 20);
}

void mostrar_dados_completo(const Imagem *dados, size_t n) {
    imprimir_dados(dados, n, n);
}

int esta_ordenado(const Imagem *dados, size_t n) {
    for (size_t i = 1; i < n; ++i)
        if (dados[i - 1].nuvens > dados[i].nuvens) return 0;
    return 1;
}
