#include "aps.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define CRIAR_PASTA(caminho) _mkdir(caminho)
#else
#include <sys/stat.h>
#define CRIAR_PASTA(caminho) mkdir(caminho, 0777)
#endif

/* Tres vetores correspondem a entrada, copia e auxiliar usados na comparacao.
   A ordenacao quadratica nao faz parte deste teste de capacidade. */
static int testar_vetores(size_t orcamento, int ate_falha, const char *caminho) {
    (void)CRIAR_PASTA("resultados");
    FILE *saida = fopen(caminho, "w");
    if (!saida) { perror("Falha ao criar CSV de memoria"); return 0; }
    fprintf(saida, "registros,bytes_tres_vetores,limite_bytes,resultado\n");
    size_t ultimo = 0;
    int falhou = 0;
    for (size_t n = 1000; n <= SIZE_MAX / (3 * sizeof(Imagem)) && n <= INT_MAX; ) {
        size_t bytes = n * 3 * sizeof(Imagem);
        if (!ate_falha && bytes > orcamento) break;
        if (ate_falha) {
            printf("Tentando %zu registros (3 vetores, %.2f MiB)...\n",
                   n, (double)bytes / (1024.0 * 1024.0));
            fflush(stdout);
        }
        Imagem *a = malloc(n * sizeof *a);
        Imagem *b = malloc(n * sizeof *b);
        Imagem *c = malloc(n * sizeof *c);
        if (!a || !b || !c) {
            fprintf(saida, "%zu,%zu,%zu,falha_alocacao\n", n, bytes, orcamento);
            falhou = 1;
            free(a); free(b); free(c);
            break;
        }
        gerar_cenario(a, n, 2);
        memcpy(b, a, n * sizeof *a);
        memcpy(c, a, n * sizeof *a);
        /* Forca compromisso de paginas dos tres vetores. Sem isso, malloc
           pode apenas reservar enderecos e superestimar a capacidade. */
        volatile Imagem *vb = b;
        volatile Imagem *vc = c;
        for (size_t i = 0; i < n; i += 256) {
            vb[i].nuvens = a[i].nuvens;
            vc[i].nuvens = a[i].nuvens;
        }
        fprintf(saida, "%zu,%zu,%zu,sucesso\n", n, bytes, orcamento);
        fflush(saida);
        if (ate_falha) {
            puts("Alocacao e preenchimento concluidos.");
            fflush(stdout);
        }
        ultimo = n;
        free(a); free(b); free(c);
        if (n > SIZE_MAX / 2 || n > INT_MAX / 2) break;
        n *= 2;
    }
    if (!falhou) fprintf(saida, "%zu,%zu,%zu,%s\n",
                         ultimo, ultimo * 3 * sizeof(Imagem), orcamento,
                         ate_falha ? "limite_representacao" : "limite_seguranca");
    int gravado = !ferror(saida);
    if (fclose(saida) != 0) gravado = 0;
    if (!gravado) { fprintf(stderr, "Falha ao gravar CSV de memoria.\n"); return 0; }
    printf("Maior tamanho concluido: %zu registros (3 vetores, %.2f MiB).\n",
           ultimo, (double)(ultimo * 3 * sizeof(Imagem)) / (1024.0 * 1024.0));
    puts(falhou ? "Parada: falha de alocacao." :
                  ate_falha ? "Parada: limite do campo ID (int); nao houve falha de memoria." :
                              "Parada: limite de seguranca; nao houve falha de memoria.");
    printf("Detalhes em %s.\n", caminho);
    return ate_falha ? falhou : 1;
}

int executar_teste_memoria(size_t limite_mb) {
    if (limite_mb < 1 || limite_mb > 1024) return 0;
    size_t orcamento = limite_mb * (size_t)1024 * 1024;
#ifdef _WIN32
    MEMORYSTATUSEX memoria = {0};
    memoria.dwLength = sizeof memoria;
    if (GlobalMemoryStatusEx(&memoria) && memoria.ullAvailPhys / 4 < orcamento)
        orcamento = (size_t)(memoria.ullAvailPhys / 4);
#endif
    return testar_vetores(orcamento, 0, "resultados/escala_memoria.csv");
}

int executar_teste_memoria_filho(void) {
#ifdef _WIN32
    return testar_vetores(0, 1,
                          "resultados/escala_falha.csv");
#else
    fprintf(stderr, "Teste ate falha requer Windows.\n");
    return 0;
#endif
}

int executar_teste_memoria_ate_falha(void) {
#ifdef _WIN32
    char programa[MAX_PATH], comando[MAX_PATH + 32];
    DWORD tamanho = GetModuleFileNameA(NULL, programa, sizeof programa);
    if (tamanho == 0 || tamanho >= sizeof programa ||
        snprintf(comando, sizeof comando, "\"%s\" --probe-child", programa) >= (int)sizeof comando) {
        fprintf(stderr, "Nao foi possivel localizar o executavel.\n");
        return 0;
    }
    STARTUPINFOA inicio = {0};
    PROCESS_INFORMATION processo = {0};
    inicio.cb = sizeof inicio;
    if (!CreateProcessA(programa, comando, NULL, NULL, TRUE, 0,
                        NULL, NULL, &inicio, &processo)) {
        fprintf(stderr, "Nao foi possivel iniciar o processo isolado.\n");
        return 0;
    }
    puts("Processo separado, sem teto artificial de memoria; tentando ate falhar.");
    fflush(stdout);
    WaitForSingleObject(processo.hProcess, INFINITE);
    DWORD codigo = 1;
    GetExitCodeProcess(processo.hProcess, &codigo);
    CloseHandle(processo.hThread);
    CloseHandle(processo.hProcess);
    if (codigo != 0) {
        fprintf(stderr, "O teste isolado terminou com erro (codigo %lu).\n",
                (unsigned long)codigo);
        return 0;
    }
    return 1;
#else
    fprintf(stderr, "O teste ate falha requer Windows.\n");
    return 0;
#endif
}
