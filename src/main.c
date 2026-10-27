#include "aps.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_ESTATICA 1000

static Imagem armazenamento_estatico[CAPACIDADE_ESTATICA];

static const char *nomes[] = {"Insertion Sort", "Merge Sort", "Quick Sort"};

static int ler_inteiro(const char *pergunta, size_t *valor) {
    char linha[64], *fim;
    printf("%s", pergunta);
    if (!fgets(linha, sizeof linha, stdin)) return 0;
    if (linha[0] == '-') return 0;
    errno = 0;
    unsigned long long numero = strtoull(linha, &fim, 10);
    if (errno || fim == linha || (*fim != '\n' && *fim != '\0') || numero > SIZE_MAX) return 0;
    *valor = (size_t)numero;
    return 1;
}

static int preparar_vetores(Imagem **original, Imagem **copia, Imagem **aux,
                           size_t *capacidade, size_t necessario, size_t existentes) {
    if (necessario <= *capacidade) return 1;
    if (necessario > SIZE_MAX / sizeof(Imagem)) return 0;
    Imagem *novo_original = malloc(necessario * sizeof(Imagem));
    Imagem *nova_copia = malloc(necessario * sizeof(Imagem));
    Imagem *novo_aux = malloc(necessario * sizeof(Imagem));
    if (!novo_original || !nova_copia || !novo_aux) {
        free(novo_original); free(nova_copia); free(novo_aux);
        return 0;
    }
    if (*original && existentes)
        memcpy(novo_original, *original, existentes * sizeof(Imagem));
    free(*original); free(*copia); free(*aux);
    *original = novo_original;
    *copia = nova_copia;
    *aux = novo_aux;
    *capacidade = necessario;
    return 1;
}

static void mostrar_conforme_modo(const Imagem *dados, size_t n, int completo) {
    if (completo) mostrar_dados_completo(dados, n);
    else mostrar_dados(dados, n);
}

static Resultado comparar(const Imagem *original, size_t n, Imagem *copia, Imagem *aux,
                          int completo) {
    Resultado resultado = {0};
    resultado.quantidade_dados = n;
    puts("\nAntes da ordenacao:");
    mostrar_conforme_modo(original, n, completo);
    for (int algoritmo = 0; algoritmo < 3; ++algoritmo) {
        memcpy(copia, original, n * sizeof *copia);
        double inicio = tempo_agora();
        if (algoritmo == 0) insertion_sort(copia, n);
        else if (algoritmo == 1) merge_sort(copia, aux, n);
        else quick_sort(copia, n);
        resultado.tempos_ms[algoritmo] = (tempo_agora() - inicio) * 1000.0;
        resultado.corretos[algoritmo] = esta_ordenado(copia, n);
        printf("\nDepois (%s):\n", nomes[algoritmo]);
        mostrar_conforme_modo(copia, n, completo);
    }
    puts("\nCOMPARACAO DOS TRES METODOS");
    puts("METODO           TEMPO (ms)    VALIDACAO");
    puts("------------------------------------------");
    for (int i = 0; i < 3; ++i)
        printf("%-16s\t%12.6f    %s\n", nomes[i], resultado.tempos_ms[i],
               resultado.corretos[i] ? "OK" : "FALHA");
    int melhor = 0, pior = 0;
    for (int i = 1; i < 3; ++i) {
        if (resultado.tempos_ms[i] < resultado.tempos_ms[melhor]) melhor = i;
        if (resultado.tempos_ms[i] > resultado.tempos_ms[pior]) pior = i;
    }
    printf("\nNesta execucao, %s foi o mais rapido (%.6f ms) e %s o mais lento (%.6f ms).\n",
           nomes[melhor], resultado.tempos_ms[melhor], nomes[pior], resultado.tempos_ms[pior]);
    puts("Os tempos medem somente a ordenacao, usando copias da mesma entrada.");
    return resultado;
}

static void mostrar_resultado(const Resultado *resultado) {
    printf("Ultimo teste: %zu registros\n", resultado->quantidade_dados);
    puts("METODO           TEMPO (ms)");
    puts("----------------------------");
    for (int i = 0; i < 3; ++i)
        printf("%-16s\t%12.6f%s\n", nomes[i], resultado->tempos_ms[i],
               resultado->corretos[i] ? "" : "  FALHA NA VALIDACAO");
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--probe-child") == 0)
        return executar_teste_memoria_filho() ? EXIT_SUCCESS : EXIT_FAILURE;
    Imagem *armazenamento_dinamico = NULL;
    Imagem *copia = NULL;
    Imagem *aux = NULL;
    Imagem *original = armazenamento_estatico;
    size_t capacidade = 0;
    FilaTrabalhos fila;
    PilhaResultados pilha;
    fila_iniciar(&fila);
    pilha_iniciar(&pilha);
    srand(42); /* Semente fixa para facilitar a reproducao dos testes. */
    size_t n = 0;
    int exibicao_completa = 0;
    char linha[512];
    for (;;) {
        puts("\nAPS - Comparacao de ordenacao em C");
        puts("\nDADOS\n 1\tGerar dados por tamanho e cenario\n 2\tCarregar CSV\n 3\tExibir dados\n\nCOMPARACAO\n 4\tComparar os tres metodos\n 5\tEnfileirar dados atuais\n 6\tExecutar proximo teste da fila\n 7\tConsultar ultimo resultado\n 8\tRemover ultimo resultado\n 9\tBateria padrao e exportar CSV/SVG\n10\tBateria com tamanhos escolhidos\n\nEXIBICAO E MEMORIA\n11\tTeste controlado de escala de memoria\n12\tAlternar listagem completa/amostra\n13\tTeste sem teto artificial ate falha de memoria\n 0\tSair");
        printf("Exibicao: %s\n", exibicao_completa ? "todos os registros" : "amostra de 20 registros");
        printf("Fila: %zu teste(s) | Pilha: %zu resultado(s)\nOpcao: ",
               fila.quantidade, pilha.quantidade);
        if (!fgets(linha, sizeof linha, stdin)) break;
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strcmp(linha, "0") == 0) break;
        if (strcmp(linha, "1") == 0) {
            size_t quantidade, cenario;
            if (!ler_inteiro("Quantidade de registros: ", &quantidade) ||
                quantidade < 1 || quantidade > INT_MAX ||
                !ler_inteiro("Cenario (1 aleatorio, 2 ordenado, 3 invertido): ", &cenario) ||
                cenario < 1 || cenario > 3) {
                puts("Quantidade ou cenario invalido.");
                continue;
            }
            if (!preparar_vetores(&armazenamento_dinamico, &copia, &aux,
                                 &capacidade, quantidade, n > CAPACIDADE_ESTATICA ? n : 0)) {
                puts("Memoria insuficiente para os tres vetores. A base anterior foi mantida.");
                continue;
            }
            n = quantidade;
            original = n <= CAPACIDADE_ESTATICA ? armazenamento_estatico : armazenamento_dinamico;
            gerar_cenario(original, n, (int)cenario);
            printf("%zu registros gerados (%s).\n", n,
                   original == armazenamento_estatico ? "alocacao estatica" : "alocacao dinamica");
        } else if (strcmp(linha, "2") == 0) {
            printf("Caminho do CSV (ex.: dados/exemplo.csv): ");
            if (!fgets(linha, sizeof linha, stdin)) break;
            linha[strcspn(linha, "\r\n")] = '\0';
            if (!preparar_vetores(&armazenamento_dinamico, &copia, &aux,
                                 &capacidade, 100000, n > CAPACIDADE_ESTATICA ? n : 0)) {
                puts("Memoria insuficiente para carregar o CSV.");
                continue;
            }
            original = n <= CAPACIDADE_ESTATICA ? armazenamento_estatico : armazenamento_dinamico;
            size_t carregados = carregar_csv(linha, copia, 100000);
            if (carregados) {
                n = carregados;
                original = n <= CAPACIDADE_ESTATICA ? armazenamento_estatico : armazenamento_dinamico;
                memcpy(original, copia, n * sizeof *original);
                printf("%zu registros carregados.\n", n);
            } else puts("Nenhum dado carregado. A base anterior foi mantida.");
        } else if (strcmp(linha, "3") == 0) {
            mostrar_conforme_modo(original, n, exibicao_completa);
        } else if (strcmp(linha, "4") == 0) {
            if (!n) { puts("Carregue ou gere dados primeiro."); continue; }
            Resultado resultado = comparar(original, n, copia, aux, exibicao_completa);
            if (!pilha_empilhar(&pilha, resultado))
                fprintf(stderr, "Memoria insuficiente para guardar o resultado.\n");
        } else if (strcmp(linha, "5") == 0) {
            if (!n) { puts("Carregue ou gere dados primeiro."); continue; }
            if (fila_enfileirar(&fila, original, n))
                printf("Teste com %zu registros adicionado a fila.\n", n);
            else fprintf(stderr, "Memoria insuficiente para enfileirar.\n");
        } else if (strcmp(linha, "6") == 0) {
            Imagem *pendente;
            size_t quantidade;
            if (!fila_desenfileirar(&fila, &pendente, &quantidade)) {
                puts("A fila esta vazia.");
                continue;
            }
            if (!preparar_vetores(&armazenamento_dinamico, &copia, &aux,
                                 &capacidade, quantidade, n > CAPACIDADE_ESTATICA ? n : 0)) {
                puts("Memoria insuficiente para executar o teste enfileirado.");
                free(pendente);
                continue;
            }
            original = n <= CAPACIDADE_ESTATICA ? armazenamento_estatico : armazenamento_dinamico;
            Resultado resultado = comparar(pendente, quantidade, copia, aux, exibicao_completa);
            free(pendente);
            if (!pilha_empilhar(&pilha, resultado))
                fprintf(stderr, "Memoria insuficiente para guardar o resultado.\n");
        } else if (strcmp(linha, "7") == 0) {
            Resultado resultado;
            if (pilha_consultar(&pilha, &resultado)) mostrar_resultado(&resultado);
            else puts("A pilha de resultados esta vazia.");
        } else if (strcmp(linha, "8") == 0) {
            Resultado resultado;
            if (pilha_desempilhar(&pilha, &resultado)) {
                printf("Resultado removido (%zu registros).\n", resultado.quantidade_dados);
            } else puts("A pilha de resultados esta vazia.");
        } else if (strcmp(linha, "9") == 0) {
            (void)executar_bateria();
        } else if (strcmp(linha, "10") == 0) {
            size_t quantidade, repeticoes, valor;
            size_t tamanhos[6];
            if (!ler_inteiro("Quantos tamanhos (2 a 6): ", &quantidade) ||
                quantidade < 2 || quantidade > 6) {
                puts("Quantidade de tamanhos invalida.");
                continue;
            }
            size_t i;
            for (i = 0; i < quantidade; ++i) {
                if (!ler_inteiro("Tamanho (em ordem crescente): ", &valor) ||
                    valor < 1 || valor > INT_MAX ||
                    (i > 0 && valor <= tamanhos[i - 1])) break;
                tamanhos[i] = (size_t)valor;
            }
            if (i != quantidade ||
                !ler_inteiro("Repeticoes por combinacao (1 a 10): ", &repeticoes) ||
                repeticoes < 1 || repeticoes > 10) {
                puts("Configuracao invalida.");
                continue;
            }
            (void)executar_bateria_config(tamanhos, (size_t)quantidade, (int)repeticoes);
        } else if (strcmp(linha, "11") == 0) {
            size_t limite;
            if (!ler_inteiro("Limite de memoria para o teste em MiB (1 a 1024): ", &limite) ||
                limite < 1 || limite > 1024) {
                puts("Limite invalido.");
                continue;
            }
            (void)executar_teste_memoria((size_t)limite);
        } else if (strcmp(linha, "12") == 0) {
            exibicao_completa = !exibicao_completa;
            printf("Exibicao alterada: %s.\n",
                   exibicao_completa ? "todos os registros" : "amostra de 20 registros");
        } else if (strcmp(linha, "13") == 0) {
            (void)executar_teste_memoria_ate_falha();
        } else puts("Opcao invalida.");
    }
    fila_limpar(&fila);
    pilha_limpar(&pilha);
    free(armazenamento_dinamico); free(copia); free(aux);
    return EXIT_SUCCESS;
}
