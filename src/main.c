#include "aps.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE 100000
#define CAPACIDADE_ESTATICA 1000

static Imagem armazenamento_estatico[CAPACIDADE_ESTATICA];

static const char *nomes[] = {"Insertion Sort", "Merge Sort", "Quick Sort"};

static int ler_inteiro(const char *pergunta, unsigned long *valor) {
    char linha[64], *fim;
    printf("%s", pergunta);
    if (!fgets(linha, sizeof linha, stdin)) return 0;
    errno = 0;
    unsigned long numero = strtoul(linha, &fim, 10);
    if (errno || fim == linha || (*fim != '\n' && *fim != '\0')) return 0;
    *valor = numero;
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
        printf("\n%s: %.6f ms | %s\n", nomes[algoritmo],
               resultado.tempos_ms[algoritmo],
               resultado.corretos[algoritmo] ? "Ordenacao correta" : "FALHA");
        mostrar_conforme_modo(copia, n, completo);
    }
    return resultado;
}

static void mostrar_resultado(const Resultado *resultado) {
    printf("Ultimo teste: %zu registros\n", resultado->quantidade_dados);
    for (int i = 0; i < 3; ++i)
        printf("%s: %.6f ms | %s\n", nomes[i], resultado->tempos_ms[i],
               resultado->corretos[i] ? "correto" : "FALHA");
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--probe-child") == 0)
        return executar_teste_memoria_filho() ? EXIT_SUCCESS : EXIT_FAILURE;
    Imagem *armazenamento_dinamico = malloc(CAPACIDADE * sizeof *armazenamento_dinamico);
    Imagem *original = armazenamento_estatico;
    Imagem *copia = malloc(CAPACIDADE * sizeof *copia);
    Imagem *aux = malloc(CAPACIDADE * sizeof *aux);
    if (!armazenamento_dinamico || !copia || !aux) {
        fprintf(stderr, "Memoria insuficiente.\n");
        free(armazenamento_dinamico); free(copia); free(aux);
        return EXIT_FAILURE;
    }
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
        puts("1 - Gerar dados por tamanho e cenario\n2 - Carregar CSV\n3 - Exibir dados\n4 - Comparar agora\n5 - Enfileirar dados atuais\n6 - Executar proximo teste da fila\n7 - Consultar ultimo resultado\n8 - Remover ultimo resultado\n9 - Bateria padrao e exportar CSV/SVG\n10 - Bateria com tamanhos escolhidos\n11 - Teste controlado de escala de memoria\n12 - Alternar listagem completa/amostra\n13 - Teste isolado ate falha de memoria\n0 - Sair");
        printf("Exibicao: %s\n", exibicao_completa ? "todos os registros" : "amostra de 20 registros");
        printf("Fila: %zu teste(s) | Pilha: %zu resultado(s)\nOpcao: ",
               fila.quantidade, pilha.quantidade);
        if (!fgets(linha, sizeof linha, stdin)) break;
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strcmp(linha, "0") == 0) break;
        if (strcmp(linha, "1") == 0) {
            unsigned long quantidade, cenario;
            if (!ler_inteiro("Quantidade (1 a 100000): ", &quantidade) ||
                quantidade < 1 || quantidade > CAPACIDADE ||
                !ler_inteiro("Cenario (1 aleatorio, 2 ordenado, 3 invertido): ", &cenario) ||
                cenario < 1 || cenario > 3) {
                puts("Quantidade ou cenario invalido.");
                continue;
            }
            n = (size_t)quantidade;
            original = n <= CAPACIDADE_ESTATICA ? armazenamento_estatico : armazenamento_dinamico;
            gerar_cenario(original, n, (int)cenario);
            printf("%zu registros gerados (%s).\n", n,
                   original == armazenamento_estatico ? "alocacao estatica" : "alocacao dinamica");
        } else if (strcmp(linha, "2") == 0) {
            printf("Caminho do CSV (ex.: dados/exemplo.csv): ");
            if (!fgets(linha, sizeof linha, stdin)) break;
            linha[strcspn(linha, "\r\n")] = '\0';
            size_t carregados = carregar_csv(linha, copia, CAPACIDADE);
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
            unsigned long quantidade, repeticoes, valor;
            size_t tamanhos[6];
            if (!ler_inteiro("Quantos tamanhos (2 a 6): ", &quantidade) ||
                quantidade < 2 || quantidade > 6) {
                puts("Quantidade de tamanhos invalida.");
                continue;
            }
            size_t i;
            for (i = 0; i < quantidade; ++i) {
                if (!ler_inteiro("Tamanho (1 a 100000, em ordem crescente): ", &valor) ||
                    valor < 1 || valor > CAPACIDADE ||
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
            unsigned long limite;
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
            unsigned long limite;
            if (!ler_inteiro("Limite isolado do processo em MiB (64 a 1024): ", &limite) ||
                limite < 64 || limite > 1024) {
                puts("Limite invalido.");
                continue;
            }
            (void)executar_teste_memoria_ate_falha((size_t)limite);
        } else puts("Opcao invalida.");
    }
    fila_limpar(&fila);
    pilha_limpar(&pilha);
    free(armazenamento_dinamico); free(copia); free(aux);
    return EXIT_SUCCESS;
}
