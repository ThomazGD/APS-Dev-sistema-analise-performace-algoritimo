#include "aps.h"
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
    for (size_t n = 1000; n <= SIZE_MAX / (3 * sizeof(Imagem)); ) {
        size_t bytes = n * 3 * sizeof(Imagem);
        if (!ate_falha && bytes > orcamento) break;
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
        ultimo = n;
        free(a); free(b); free(c);
        if (n > SIZE_MAX / 2) break;
        n *= 2;
    }
    if (!falhou) fprintf(saida, "%zu,%zu,%zu,limite_seguranca\n",
                         ultimo, ultimo * 3 * sizeof(Imagem), orcamento);
    int gravado = !ferror(saida);
    if (fclose(saida) != 0) gravado = 0;
    if (!gravado) { fprintf(stderr, "Falha ao gravar CSV de memoria.\n"); return 0; }
    printf("Maior tamanho concluido: %zu registros (3 vetores, %.2f MiB).\n",
           ultimo, (double)(ultimo * 3 * sizeof(Imagem)) / (1024.0 * 1024.0));
    puts(falhou ? "Parada: falha de alocacao." :
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
    BOOL associado = FALSE;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limites = {0};
    if (!IsProcessInJob(GetCurrentProcess(), NULL, &associado) || !associado ||
        !QueryInformationJobObject(NULL, JobObjectExtendedLimitInformation,
                                   &limites, sizeof limites, NULL) ||
        !(limites.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_PROCESS_MEMORY) ||
        limites.ProcessMemoryLimit < 64u * 1024u * 1024u ||
        limites.ProcessMemoryLimit > 1024u * 1024u * 1024u) {
        fprintf(stderr, "Teste cancelado: limite de memoria isolado nao confirmado.\n");
        return 0;
    }
    return testar_vetores((size_t)limites.ProcessMemoryLimit, 1,
                          "resultados/escala_falha.csv");
#else
    fprintf(stderr, "Teste ate falha requer Windows.\n");
    return 0;
#endif
}

int executar_teste_memoria_ate_falha(size_t limite_mb) {
#ifdef _WIN32
    if (limite_mb < 64 || limite_mb > 1024) return 0;
    char programa[MAX_PATH], comando[MAX_PATH + 32];
    DWORD tamanho = GetModuleFileNameA(NULL, programa, sizeof programa);
    if (tamanho == 0 || tamanho >= sizeof programa ||
        snprintf(comando, sizeof comando, "\"%s\" --probe-child", programa) >= (int)sizeof comando) {
        fprintf(stderr, "Nao foi possivel localizar o executavel.\n");
        return 0;
    }
    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (!job) { fprintf(stderr, "Nao foi possivel criar processo isolado.\n"); return 0; }
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limites = {0};
    limites.BasicLimitInformation.LimitFlags =
        JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    limites.ProcessMemoryLimit = limite_mb * (SIZE_T)1024 * 1024;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                 &limites, sizeof limites)) {
        fprintf(stderr, "Nao foi possivel limitar a memoria do processo isolado.\n");
        CloseHandle(job);
        return 0;
    }
    STARTUPINFOA inicio = {0};
    PROCESS_INFORMATION processo = {0};
    inicio.cb = sizeof inicio;
    if (!CreateProcessA(programa, comando, NULL, NULL, TRUE, CREATE_SUSPENDED,
                        NULL, NULL, &inicio, &processo)) {
        fprintf(stderr, "Nao foi possivel iniciar o processo isolado.\n");
        CloseHandle(job);
        return 0;
    }
    if (!AssignProcessToJobObject(job, processo.hProcess)) {
        fprintf(stderr, "Nao foi possivel aplicar o limite ao processo; teste cancelado.\n");
        TerminateProcess(processo.hProcess, 1);
        WaitForSingleObject(processo.hProcess, INFINITE);
        CloseHandle(processo.hThread);
        CloseHandle(processo.hProcess);
        CloseHandle(job);
        return 0;
    }
    printf("Processo isolado com limite de %zu MiB; tentando ate falhar.\n", limite_mb);
    fflush(stdout);
    ResumeThread(processo.hThread);
    WaitForSingleObject(processo.hProcess, INFINITE);
    DWORD codigo = 1;
    GetExitCodeProcess(processo.hProcess, &codigo);
    CloseHandle(processo.hThread);
    CloseHandle(processo.hProcess);
    CloseHandle(job);
    if (codigo != 0) {
        fprintf(stderr, "O teste isolado terminou com erro (codigo %lu).\n",
                (unsigned long)codigo);
        return 0;
    }
    return 1;
#else
    (void)limite_mb;
    fprintf(stderr, "O teste ate falha requer Windows.\n");
    return 0;
#endif
}
