# Arquitetura do projeto em C

O objetivo central é comparar três algoritmos de ordenação aplicados a registros que simulam um catálogo de imagens de satélite. Cada registro (`Imagem`) contém identificador, data e percentual de nuvens. A chave atual é `nuvens`, em ordem crescente. O programa ordena metadados; não processa pixels nem afirma usar imagens reais.

## Fluxo dos dados

```text
CSV ou gerador -> vetor atual de Imagem
                       | comparacao imediata -> copia -> 3 ordenacoes -> resultado -> pilha
                       | enfileirar -> fila FIFO -> proximo teste -> 3 ordenacoes -> resultado -> pilha
```

`src/dados.c` lê e gera registros aleatórios, ordenados ou invertidos. O menu de `src/main.c` escolhe a entrada, chama a comparação e mostra resultados. `src/ordenacao.c` implementa Insertion Sort, Merge Sort e Quick Sort. `src/tempo.c` fornece o cronômetro. `src/estruturas.c` implementa a fila e a pilha. As interfaces e os tipos compartilhados ficam em `include/aps.h`.

`src/experimentos.c` executa a bateria: três cenários, com tamanhos e repetições padrão ou escolhidos pelo usuário. Cada repetição gera uma entrada; os três métodos recebem cópias dessa mesma entrada. Os tempos individuais vão para CSV, e um segundo CSV contém média, mediana, mínimo e máximo. O módulo também gera três gráficos SVG de medianas em escala logarítmica. A geração, a cópia, a validação e a gravação ficam fora do intervalo cronometrado.

`src/escala.c` testa a capacidade de três vetores dinâmicos em tamanhos crescentes. A opção controlada pode parar pelo orçamento de segurança; a opção isolada inicia outro processo com limite próprio de memória e continua até uma falha de alocação. O código força o uso das páginas alocadas antes de considerar uma etapa concluída. Esse módulo mede capacidade de memória, não tempo de ordenação; tamanhos extremos não entram automaticamente na bateria dos três algoritmos.

O vetor atual usa armazenamento estático para até 1000 registros e armazenamento dinâmico acima disso. Os vetores de trabalho e o auxiliar do Merge Sort são dinâmicos. A fila faz cópias dinâmicas dos dados enfileirados. A interface atual limita cada entrada a 100000 registros, verifica falhas de alocação e não tenta esgotar a memória do computador.

## Fila e pilha

- **Fila encadeada (FIFO):** guarda testes pendentes. Cada nó possui uma cópia do vetor de entrada; o teste mais antigo é o próximo a executar. `enfileirar` e `desenfileirar` levam O(1) para ligar ou retirar o nó, mais O(n) para copiar os n registros ao enfileirar. Isso permite alternar o vetor atual sem modificar testes já agendados.
- **Pilha encadeada (LIFO):** guarda um resumo de cada comparação concluída (tamanho, três tempos e três verificações). `empilhar`, `consultar` e `desempilhar` levam O(1). Assim, o usuário consulta ou remove primeiro o resultado mais recente. Remover não desfaz a ordenação nem reconstitui o vetor original; apenas retira o resumo do histórico.
- Os nós usam alocação dinâmica e são liberados ao remover ou encerrar o programa. Uma falha de alocação é reportada. Nenhum nó da fila ou pilha é manipulado dentro do intervalo cronometrado.

## Medição e algoritmos

Para cada comparação, `main.c` copia o mesmo vetor original para um vetor de trabalho antes de iniciar cada cronômetro. Somente a chamada do algoritmo é medida. Verificação da ordem e impressão ocorrem depois da medição. Merge Sort recebe um vetor auxiliar previamente alocado. Os três algoritmos são os métodos comparados; fila e pilha organizam o fluxo do sistema e não contam como métodos de ordenação.

| Método | Melhor caso | Caso médio | Pior caso | Memória extra | Estável |
| --- | --- | --- | --- | --- | --- |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Sim |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Sim |
| Quick Sort com partição em três grupos | O(n log n) | O(n log n) | O(n²) | O(log n) de pilha de chamadas nesta implementação | Não |

Quick Sort usa recursão na partição menor e iteração na maior, o que limita a profundidade da pilha de chamadas. O fato de ser recursivo não substitui a pilha encadeada explícita usada para o histórico.

## Relação com o plano de ensino

O plano da disciplina também aborda TADs, ponteiros, listas, recursividade, árvores, grafos, busca, tabela hash e outros algoritmos de ordenação. Este projeto aplica TADs, alocação dinâmica, listas encadeadas, pilha, fila, recursividade, análise de complexidade e ordenação. Árvores, grafos, tabela hash e outros métodos aparecem no currículo, mas não são requisitos explícitos da APS e ainda não têm uma função necessária no fluxo atual.

## Limites atuais

A bateria já produz medições repetidas e gráficos com tamanhos configuráveis. O histórico interativo fica somente na memória durante a execução. O teste isolado registra o maior tamanho concluído e a primeira tentativa que falhou; seu limite é artificial e por processo, não a RAM física total. A exibição pode ser completa ou limitada a 20 registros. O programa ainda precisa de validações adicionais para importação e testes de casos extremos.
