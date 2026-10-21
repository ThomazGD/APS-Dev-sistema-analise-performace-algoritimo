#ifndef APS_H
#define APS_H

#include <stddef.h>

typedef struct {
    int id;
    int data; /* AAAAMMDD */
    int nuvens; /* percentual de 0 a 100; chave de ordenacao */
} Imagem;

typedef struct Trabalho Trabalho;
typedef struct ResultadoNo ResultadoNo;

typedef struct {
    Trabalho *primeiro;
    Trabalho *ultimo;
    size_t quantidade;
} FilaTrabalhos;

typedef struct {
    size_t quantidade_dados;
    double tempos_ms[3];
    int corretos[3];
} Resultado;

typedef struct {
    ResultadoNo *topo;
    size_t quantidade;
} PilhaResultados;

void fila_iniciar(FilaTrabalhos *fila);
int fila_enfileirar(FilaTrabalhos *fila, const Imagem *dados, size_t n);
int fila_desenfileirar(FilaTrabalhos *fila, Imagem **dados, size_t *n);
void fila_limpar(FilaTrabalhos *fila);

void pilha_iniciar(PilhaResultados *pilha);
int pilha_empilhar(PilhaResultados *pilha, Resultado resultado);
int pilha_desempilhar(PilhaResultados *pilha, Resultado *resultado);
int pilha_consultar(const PilhaResultados *pilha, Resultado *resultado);
void pilha_limpar(PilhaResultados *pilha);

void insertion_sort(Imagem *dados, size_t n);
void merge_sort(Imagem *dados, Imagem *aux, size_t n);
void quick_sort(Imagem *dados, size_t n);
size_t carregar_csv(const char *caminho, Imagem *dados, size_t capacidade);
void gerar_dados(Imagem *dados, size_t n);
void gerar_cenario(Imagem *dados, size_t n, int cenario);
void mostrar_dados(const Imagem *dados, size_t n);
void mostrar_dados_completo(const Imagem *dados, size_t n);
int esta_ordenado(const Imagem *dados, size_t n);
double tempo_agora(void);
int executar_bateria(void);
int executar_bateria_config(const size_t *tamanhos, size_t quantidade, int repeticoes);
int executar_teste_memoria(size_t limite_mb);
int executar_teste_memoria_ate_falha(size_t limite_mb);
int executar_teste_memoria_filho(void);

#endif
