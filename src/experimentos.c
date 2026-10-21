#include "aps.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define CRIAR_PASTA(caminho) _mkdir(caminho)
#else
#include <sys/stat.h>
#define CRIAR_PASTA(caminho) mkdir(caminho, 0777)
#endif

#define MAX_REPETICOES 10
#define MAX_TAMANHOS 6

static const char *cenarios[] = {"aleatorio", "ordenado", "invertido"};
static const char *algoritmos[] = {"Insertion Sort", "Merge Sort", "Quick Sort"};
static const size_t tamanhos_padrao[] = {100, 1000, 5000, 10000};
static const char *cores[] = {"#d1495b", "#0077b6", "#2a9d8f"};

static double mediana(const double *valores, int repeticoes) {
    double ordenados[MAX_REPETICOES];
    memcpy(ordenados, valores, (size_t)repeticoes * sizeof *ordenados);
    for (int i = 1; i < repeticoes; ++i) {
        double valor = ordenados[i];
        int j = i;
        while (j > 0 && ordenados[j - 1] > valor) {
            ordenados[j] = ordenados[j - 1];
            --j;
        }
        ordenados[j] = valor;
    }
    if (repeticoes % 2) return ordenados[repeticoes / 2];
    return (ordenados[repeticoes / 2 - 1] + ordenados[repeticoes / 2]) / 2.0;
}

static int gerar_grafico(int cenario, const size_t *tamanhos, size_t quantidade,
                         int repeticoes, double pontos[MAX_TAMANHOS][3]) {
    char caminho[96];
    snprintf(caminho, sizeof caminho, "resultados/grafico_%s.svg", cenarios[cenario]);
    FILE *arquivo = fopen(caminho, "w");
    if (!arquivo) { perror("Falha ao criar grafico"); return 0; }
    double teto = 0.01;
    for (size_t t = 0; t < quantidade; ++t)
        for (int a = 0; a < 3; ++a)
            while (pontos[t][a] > teto) teto *= 10.0;
    double escala = log10(teto / 0.001);
    fprintf(arquivo, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"520\" viewBox=\"0 0 800 520\">\n");
    fprintf(arquivo, "<rect width=\"800\" height=\"520\" fill=\"white\"/>\n");
    fprintf(arquivo, "<g font-family=\"Arial, sans-serif\" fill=\"#202a35\">\n");
    fprintf(arquivo, "<text x=\"80\" y=\"32\" font-size=\"22\" font-weight=\"bold\">%s: mediana de %d repeticoes</text>\n", cenarios[cenario], repeticoes);
    fprintf(arquivo, "<text x=\"80\" y=\"480\" font-size=\"13\">Quantidade de registros</text>\n");
    fprintf(arquivo, "<text x=\"80\" y=\"500\" font-size=\"12\">Tempo em ms (escala logaritmica; piso visual de 0,001 ms)</text>\n");
    for (double marca = 0.001; marca <= teto * 1.0001; marca *= 10.0) {
        double y = 400.0 - 330.0 * log10(marca / 0.001) / escala;
        fprintf(arquivo, "<line x1=\"90\" y1=\"%.1f\" x2=\"700\" y2=\"%.1f\" stroke=\"#dce3e8\"/>\n", y, y);
        fprintf(arquivo, "<text x=\"80\" y=\"%.1f\" text-anchor=\"end\" font-size=\"12\">%.3g</text>\n", y + 4, marca);
    }
    for (size_t t = 0; t < quantidade; ++t) {
        double x = 120.0 + (double)t * 555.0 / (double)(quantidade - 1);
        fprintf(arquivo, "<text x=\"%.1f\" y=\"430\" text-anchor=\"middle\" font-size=\"13\">%zu</text>\n", x, tamanhos[t]);
    }
    for (int a = 0; a < 3; ++a) {
        fprintf(arquivo, "<polyline fill=\"none\" stroke=\"%s\" stroke-width=\"3\" points=\"", cores[a]);
        for (size_t t = 0; t < quantidade; ++t) {
            double x = 120.0 + (double)t * 555.0 / (double)(quantidade - 1);
            double y = 400.0 - 330.0 * log10((pontos[t][a] + 0.001) / 0.001) / escala;
            fprintf(arquivo, "%.1f,%.1f ", x, y);
        }
        fprintf(arquivo, "\"/>\n");
        for (size_t t = 0; t < quantidade; ++t) {
            double x = 120.0 + (double)t * 555.0 / (double)(quantidade - 1);
            double y = 400.0 - 330.0 * log10((pontos[t][a] + 0.001) / 0.001) / escala;
            fprintf(arquivo, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"5\" fill=\"%s\"/>\n", x, y, cores[a]);
        }
        fprintf(arquivo, "<line x1=\"%d\" y1=\"455\" x2=\"%d\" y2=\"455\" stroke=\"%s\" stroke-width=\"3\"/>\n",
                110 + a * 210, 137 + a * 210, cores[a]);
        fprintf(arquivo, "<text x=\"%d\" y=\"459\" font-size=\"12\">%s</text>\n",
                145 + a * 210, algoritmos[a]);
    }
    fprintf(arquivo, "</g></svg>\n");
    return fclose(arquivo) == 0;
}

int executar_bateria_config(const size_t *tamanhos, size_t quantidade, int repeticoes) {
    if (!tamanhos || quantidade < 2 || quantidade > MAX_TAMANHOS ||
        repeticoes < 1 || repeticoes > MAX_REPETICOES) return 0;
    for (size_t i = 0; i < quantidade; ++i)
        if (tamanhos[i] < 1 || tamanhos[i] > 100000 ||
            (i && tamanhos[i] <= tamanhos[i - 1])) return 0;
    (void)CRIAR_PASTA("resultados");
    FILE *tempos = fopen("resultados/tempos.csv", "w");
    FILE *resumo = fopen("resultados/resumo.csv", "w");
    if (!tempos || !resumo) {
        perror("Falha ao criar os CSV de resultados");
        if (tempos) fclose(tempos);
        if (resumo) fclose(resumo);
        return 0;
    }
    size_t maximo = tamanhos[quantidade - 1];
    Imagem *original = malloc(maximo * sizeof *original);
    Imagem *copia = malloc(maximo * sizeof *copia);
    Imagem *aux = malloc(maximo * sizeof *aux);
    if (!original || !copia || !aux) {
        fprintf(stderr, "Memoria insuficiente para a bateria.\n");
        free(original); free(copia); free(aux);
        fclose(tempos); fclose(resumo);
        return 0;
    }
    fprintf(tempos, "cenario,tamanho,repeticao,semente,algoritmo,tempo_ms,correto\n");
    fprintf(resumo, "cenario,tamanho,algoritmo,media_ms,mediana_ms,minimo_ms,maximo_ms,repeticoes\n");
    int correto = 1;
    double medianas[3][MAX_TAMANHOS][3] = {{{0}}};
    for (int c = 0; c < 3; ++c) {
        for (size_t t = 0; t < quantidade; ++t) {
            size_t n = tamanhos[t];
            double medidas[3][MAX_REPETICOES];
            for (int repeticao = 0; repeticao < repeticoes; ++repeticao) {
                unsigned semente = 42u + (unsigned)c * 1000000u + (unsigned)n * 10u + (unsigned)repeticao;
                srand(semente);
                gerar_cenario(original, n, c + 1);
                for (int a = 0; a < 3; ++a) {
                    memcpy(copia, original, n * sizeof *copia);
                    double inicio = tempo_agora();
                    if (a == 0) insertion_sort(copia, n);
                    else if (a == 1) merge_sort(copia, aux, n);
                    else quick_sort(copia, n);
                    double tempo_ms = (tempo_agora() - inicio) * 1000.0;
                    int ordenado = esta_ordenado(copia, n);
                    medidas[a][repeticao] = tempo_ms;
                    if (!ordenado) correto = 0;
                    fprintf(tempos, "%s,%zu,%d,%u,%s,%.6f,%d\n",
                            cenarios[c], n, repeticao + 1, semente,
                            algoritmos[a], tempo_ms, ordenado);
                }
            }
            for (int a = 0; a < 3; ++a) {
                double soma = 0, minimo = medidas[a][0], maximo = medidas[a][0];
                for (int r = 0; r < repeticoes; ++r) {
                    double valor = medidas[a][r];
                    soma += valor;
                    if (valor < minimo) minimo = valor;
                    if (valor > maximo) maximo = valor;
                }
                fprintf(resumo, "%s,%zu,%s,%.6f,%.6f,%.6f,%.6f,%d\n",
                        cenarios[c], n, algoritmos[a], soma / repeticoes,
                        mediana(medidas[a], repeticoes), minimo, maximo, repeticoes);
                medianas[c][t][a] = mediana(medidas[a], repeticoes);
            }
            printf("Concluido: %s, %zu registros, %d repeticoes.\n",
                   cenarios[c], n, repeticoes);
        }
    }
    if (ferror(tempos) || ferror(resumo)) correto = 0;
    if (fclose(tempos) != 0) correto = 0;
    if (fclose(resumo) != 0) correto = 0;
    free(original); free(copia); free(aux);
    if (!correto) {
        fprintf(stderr, "Falha na ordenacao ou na gravacao dos resultados.\n");
        return 0;
    }
    for (int c = 0; c < 3; ++c)
        if (!gerar_grafico(c, tamanhos, quantidade, repeticoes, medianas[c])) return 0;
    puts("Arquivos gerados: resultados/tempos.csv, resultados/resumo.csv e 3 graficos SVG.");
    return 1;
}

int executar_bateria(void) {
    return executar_bateria_config(tamanhos_padrao,
                                  sizeof tamanhos_padrao / sizeof tamanhos_padrao[0], 5);
}
