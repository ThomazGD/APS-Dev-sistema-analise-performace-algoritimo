# Resultados exploratórios incluídos no repositório

Esta página descreve os arquivos atualmente em `resultados/`. **São medições de exemplo, não a análise final da monografia.** Os registros ordenados foram gerados pelo programa e são fictícios.

Execução da bateria personalizada em 21/10/2026: Windows NT 10.0.26200.0, AMD Ryzen 5 4600G, 15,89 GiB de RAM física, GCC MSYS2 UCRT64 16.2.0, C11 com `-O2`. Foram usados cinco tamanhos (10, 100, 1000, 5000 e 10000), três cenários e três repetições, totalizando **135 medições** e **45 linhas de resumo**. Todas as linhas de `tempos.csv` registraram `correto=1`. Esses números podem variar mesmo no mesmo computador.

Medianas em milissegundos retiradas de `resultados/resumo.csv`:

| Cenário | Registros | Insertion | Merge | Quick | Menor mediana |
| --- | ---: | ---: | ---: | ---: | --- |
| Aleatório | 10 | 0,000200 | 0,000400 | 0,000400 | Insertion |
| Aleatório | 100 | 0,002300 | 0,004700 | 0,004200 | Insertion |
| Aleatório | 1000 | 0,161000 | 0,051600 | 0,029800 | Quick |
| Aleatório | 5000 | 3,358900 | 0,394500 | 0,137600 | Quick |
| Aleatório | 10000 | 14,564600 | 0,586100 | 0,278600 | Quick |
| Ordenado | 10 | 0,000100 | 0,000200 | 0,000100 | Empate Insertion/Quick |
| Ordenado | 100 | 0,000200 | 0,002100 | 0,001500 | Insertion |
| Ordenado | 1000 | 0,000700 | 0,019700 | 0,007900 | Insertion |
| Ordenado | 5000 | 0,003500 | 0,112200 | 0,037500 | Insertion |
| Ordenado | 10000 | 0,007000 | 0,240000 | 0,073200 | Insertion |
| Invertido | 10 | 0,000100 | 0,000200 | 0,000100 | Empate Insertion/Quick |
| Invertido | 100 | 0,002800 | 0,001700 | 0,001000 | Quick |
| Invertido | 1000 | 0,267100 | 0,022600 | 0,008100 | Quick |
| Invertido | 5000 | 6,945200 | 0,123100 | 0,038900 | Quick |
| Invertido | 10000 | 25,319600 | 0,239900 | 0,074300 | Quick |

Nesta execução, Insertion Sort foi competitivo nas entradas pequenas e apresentou medianas muito baixas na entrada já ordenada. Nas entradas aleatória e invertida maiores, Quick Sort teve a menor mediana; Insertion Sort cresceu muito mais rápido. Essa é uma **observação dos dados medidos**, não uma garantia para outros computadores ou distribuições. Tempos de poucos microssegundos são especialmente sensíveis a ruído, e os valores de nuvens possuem muitos empates. Para a monografia, discuta também média, dispersão, complexidade teórica e os gráficos, sem concluir apenas por uma linha da tabela.

Os SVGs `grafico_aleatorio.svg`, `grafico_ordenado.svg` e `grafico_invertido.svg` visualizam as mesmas medianas. `tempos.csv` permite recontar os resultados por repetição e semente; `resumo.csv` contém média, mediana, mínimo e máximo.

**Resultado histórico da versão anterior:** o teste isolado de memória com limite de processo de **64 MiB** concluiu 1.024.000 registros em três vetores (36.864.000 bytes solicitados) e registrou falha de alocação na tentativa de 2.048.000 registros (73.728.000 bytes solicitados). O arquivo `escala_falha.csv` presente no repositório pertence a essa execução antiga. Trata-se de um limite artificial do processo, não de esgotamento da RAM física. A opção `13` atual já não impõe esse teto; será preciso gerar novos dados para analisá-la. Esse teste **não mede tempo de ordenação**. O arquivo `escala_memoria.csv` pertence a um teste separado com orçamento de segurança de 8 MiB; sua última linha informa `limite_seguranca`, não falha de memória.
