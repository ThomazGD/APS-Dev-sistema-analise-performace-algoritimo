# Estado do desenvolvimento da APS

Esta página acompanha somente o **software em C**. A produção da monografia é uma etapa separada.

## Já implementado

| Requisito ou recurso | Estado |
| --- | --- |
| Entrada externa | Leitura de CSV no formato `id,data,nuvens`; há arquivo de exemplo. |
| Entrada interna | Geração de registros simulados por tamanho e cenário. |
| Técnicas de ordenação | Insertion Sort, Merge Sort e Quick Sort. |
| Comparação justa | Cada método recebe cópia da mesma entrada; somente a chamada de ordenação é cronometrada. |
| Saída interativa | Menu, amostra ou listagem completa dos valores antes/depois, tempos e indicação de ordem correta. |
| Estruturas da disciplina | Fila encadeada de testes pendentes e pilha encadeada de resultados. |
| Cenários | Aleatório, ordenado e invertido; geração e bateria sem o antigo teto de 100000 registros, limitadas por memória e pela representação do ID como `int`. |
| Bateria | Padrão com quatro tamanhos e cinco repetições; opção personalizada com 2 a 6 tamanhos e 1 a 10 repetições. |
| Exportação | CSV com tempos individuais, CSV com resumos e três gráficos SVG. |
| Memória | Vetor estático para até 1000 registros; vetor dinâmico acima disso; falha de alocação é reportada. |
| Escala de memória | Teste controlado e teste isolado até falha real de alocação, ambos com relatórios CSV. |
| Verificação | Testes automatizados das ordenações, cenários, fila e pilha; compilação sem avisos do GCC. |

## Refinamentos recomendados antes de uma versão final

1. **Reprodutibilidade:** a bateria permite escolher tamanhos e repetições, mas a semente ainda é definida pelo programa. O ambiente da execução exploratória está em [resultados-exemplo](resultados-exemplo.md); registrar novamente compilador, otimização, sistema e memória do computador usado nos resultados definitivos.
2. **Validação da entrada externa:** melhorar mensagens para CSV inválido, validação real das datas e testes de cabeçalho, linhas longas, valores repetidos e arquivo acima do limite.
3. **Verificação dos registros:** além de conferir se as chaves estão em ordem, confirmar que cada algoritmo preserva todos os registros da entrada, inclusive com chaves repetidas.

## Decisões já tomadas

- Usar C é a escolha confirmada para este projeto. A interface de terminal segue a orientação do professor; a diferença entre C e as linguagens citadas no guia escrito está registrada no [roteiro](roteiro.md).
- Usar dados simulados é permitido pela resposta do professor; os arquivos e gráficos devem ser apresentados como resultados de simulação.
- O projeto utilizará somente dados fictícios gerados pelo programa ou carregados do CSV de exemplo. Os conjuntos públicos baixados anteriormente foram retirados.
- Entrada manual registro por registro não é necessária para cumprir o item de dados internos, pois já há geração pelo programa.
- Não é necessário inserir árvores, grafos ou tabela hash sem uma função ligada à comparação de ordenações.
