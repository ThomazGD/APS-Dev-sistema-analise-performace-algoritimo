# Como usar o programa

No Windows x64, execute `aps.exe` na raiz da pasta extraída do ZIP do repositório; não é necessário instalar um compilador. Para compilar a partir do código, siga o [README](../README.md) e execute `build/aps.exe` também a partir da raiz. A interface é textual e reúne entrada, processamento e saída no mesmo menu.

| Opção | Função |
| --- | --- |
| `1` | Gerar a quantidade informada de registros fictícios em cenário aleatório, ordenado ou invertido; o tamanho depende da memória e do tipo `int` usado no ID. |
| `2` | Ler um CSV no formato `id,data,nuvens`. |
| `3` | Mostrar a base atual. |
| `4` | Ordenar cópias da base atual com os três métodos e mostrar tempos e registros. |
| `5` e `6` | Enfileirar uma cópia da base atual e executar o teste mais antigo (FIFO). |
| `7` e `8` | Consultar ou remover o resultado mais recente (pilha LIFO). |
| `9` | Bateria padrão: 100, 1000, 5000 e 10000 registros, três cenários e cinco repetições. |
| `10` | Bateria personalizada: de 2 a 6 tamanhos crescentes e de 1 a 10 repetições. |
| `11` | Teste de memória que para no orçamento de segurança escolhido. |
| `12` | Alternar entre amostra de 20 registros e listagem completa antes e depois da ordenação. |
| `13` | Teste em processo separado, sem teto artificial, até falha de alocação. Disponível no Windows. |
| `0` | Encerrar e liberar a fila e a pilha. |

Para testar a leitura externa, escolha `2` e informe `dados/exemplo.csv`. O arquivo é **fictício**. O cabeçalho exigido é `id,data,nuvens`; cada linha contém um ID positivo, uma data numérica no formato AAAAMMDD e nuvens entre 0 e 100, separados por vírgulas e sem aspas. A validação de datas é apenas numérica e não confere se o dia existe no calendário.

Para demonstrar todos os valores, gere uma base pequena com `1`, ative `12` e escolha `4`. A tela mostrará a entrada inteira e a saída inteira de cada algoritmo. Com 100000 registros, isso produz cerca de 400000 linhas; o modo de amostra permite inspecionar casos grandes sem sobrecarregar o terminal. A listagem acontece **fora** do intervalo medido.

As opções `9` e `10` substituem `resultados/tempos.csv`, `resultados/resumo.csv` e os três arquivos `grafico_*.svg`. O primeiro CSV tem cada medição e sua semente; o segundo tem média, mediana, mínimo e máximo por cenário, tamanho e algoritmo. As opções `11` e `13` geram, respectivamente, `resultados/escala_memoria.csv` e `resultados/escala_falha.csv`. Salve uma cópia dos arquivos antes de repetir um experimento que deseja preservar.

O teste `13` tenta tamanhos dobrados **dentro de um processo separado**, sem limite artificial em MiB. Uma linha `falha_alocacao` no CSV identifica a primeira tentativa que não coube; a linha `sucesso` anterior é o maior tamanho concluído. O Windows pode ficar lento ou encerrar o processo antes de `malloc` reportar falha; nesse caso, não se deve afirmar que foi medida uma falha de alocação. A opção não ordena os vetores grandes. `limite_bytes=0` no CSV significa que nenhum teto foi configurado.
