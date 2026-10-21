#include "aps.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdlib.h>

static void testar_ordenacoes(void) {
    const Imagem entrada[] = {
        {1, 20260901, 9}, {2, 20260901, 1}, {3, 20260901, 9},
        {4, 20260901, 0}, {5, 20260901, 3}
    };
    for (int metodo = 0; metodo < 3; ++metodo) {
        Imagem dados[5], aux[5];
        for (int i = 0; i < 5; ++i) dados[i] = entrada[i];
        if (metodo == 0) insertion_sort(dados, 5);
        else if (metodo == 1) merge_sort(dados, aux, 5);
        else quick_sort(dados, 5);
        assert(esta_ordenado(dados, 5));
        const int esperado[] = {0, 1, 3, 9, 9};
        int vistos[5] = {0};
        for (int i = 0; i < 5; ++i) {
            assert(dados[i].nuvens == esperado[i]);
            assert(dados[i].id >= 1 && dados[i].id <= 5);
            assert(!vistos[dados[i].id - 1]);
            vistos[dados[i].id - 1] = 1;
            assert(dados[i].nuvens == entrada[dados[i].id - 1].nuvens);
        }
    }
}

static void testar_cenarios(void) {
    Imagem dados[5];
    gerar_cenario(dados, 5, 2);
    assert(esta_ordenado(dados, 5));
    assert(dados[0].nuvens == 0 && dados[4].nuvens == 100);
    gerar_cenario(dados, 5, 3);
    for (int i = 1; i < 5; ++i)
        assert(dados[i - 1].nuvens >= dados[i].nuvens);
    assert(dados[0].nuvens == 100 && dados[4].nuvens == 0);
    gerar_cenario(dados, 1, 2);
    assert(dados[0].nuvens == 0);
}

static void testar_fila_e_pilha(void) {
    FilaTrabalhos fila;
    PilhaResultados pilha;
    fila_iniciar(&fila);
    pilha_iniciar(&pilha);
    Imagem primeiro[] = {{1, 20260901, 10}};
    Imagem segundo[] = {{2, 20260902, 20}};
    assert(fila_enfileirar(&fila, primeiro, 1));
    assert(fila_enfileirar(&fila, segundo, 1));
    primeiro[0].nuvens = 99;
    Imagem *retirado = NULL;
    size_t n = 0;
    assert(fila_desenfileirar(&fila, &retirado, &n));
    assert(n == 1 && retirado[0].nuvens == 10);
    free(retirado);
    assert(fila_desenfileirar(&fila, &retirado, &n));
    assert(n == 1 && retirado[0].nuvens == 20);
    free(retirado);
    assert(!fila_desenfileirar(&fila, &retirado, &n));
    fila_limpar(&fila);

    Resultado a = {0}, b = {0}, atual;
    a.quantidade_dados = 10;
    b.quantidade_dados = 20;
    assert(pilha_empilhar(&pilha, a));
    assert(pilha_empilhar(&pilha, b));
    assert(pilha_consultar(&pilha, &atual) && atual.quantidade_dados == 20);
    assert(pilha_desempilhar(&pilha, &atual) && atual.quantidade_dados == 20);
    assert(pilha_desempilhar(&pilha, &atual) && atual.quantidade_dados == 10);
    assert(!pilha_desempilhar(&pilha, &atual));
    pilha_limpar(&pilha);
}

int main(void) {
    testar_ordenacoes();
    testar_cenarios();
    testar_fila_e_pilha();
    return 0;
}
