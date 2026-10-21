# Metodologia dos experimentos

## Dados e cenários

Cada `Imagem` possui `id`, `data` e `nuvens`. Os dados são **simulados**; não há leitura de pixels, imagens reais ou catálogos do INPE/TerraBrasilis. A ordenação usa apenas `nuvens` (0 a 100) como chave crescente. A data dos registros gerados é fixa (`20260916`); ela não é a variável experimental. Repetições de valores de nuvens são esperadas, principalmente em bases grandes, pois há apenas 101 valores possíveis.

O gerador produz três cenários: `aleatorio` usa `rand() % 101`; `ordenado` distribui valores não decrescentes de 0 a 100; `invertido` distribui valores não crescentes de 100 a 0. O cenário invertido pode conter empates, mas mantém a ordem decrescente. A bateria usa tamanhos padrão de 100, 1000, 5000 e 10000 registros ou uma lista crescente escolhida no menu. Para cada combinação de cenário, tamanho e repetição, os três algoritmos recebem **cópias da mesma entrada**.

## Medição e comparação

O cronômetro começa imediatamente antes da função de ordenação e para quando ela retorna. Geração, leitura de CSV, alocação, cópia, verificação, impressão e gravação de arquivos não entram no tempo medido. O Merge Sort recebe um vetor auxiliar já alocado. No Windows, `QueryPerformanceCounter` mede tempo decorrido; em outros sistemas, `clock()` mede tempo de CPU, portanto os números de plataformas diferentes não são diretamente comparáveis.

A bateria grava cada tempo em milissegundos com seis casas decimais em `tempos.csv`. O campo `correto` vale 1 quando a saída está em ordem não decrescente. `resumo.csv` agrega média, mediana, mínimo e máximo por combinação. Com número par de repetições, a mediana é a média dos dois valores centrais. Os gráficos SVG usam as medianas e escala logarítmica; tempos abaixo de 0,001 ms recebem um piso **apenas visual**, enquanto o CSV preserva o valor medido.

A semente da bateria é calculada como `42 + cenario_indice * 1000000 + tamanho * 10 + repeticao_indice`, em que os índices começam em zero. Ela é registrada em `tempos.csv`. Para cenários ordenado e invertido, a semente não altera os valores. Para resultados comparáveis, mantenha compilador, otimização, computador e carga de trabalho semelhantes; os tempos de uma única máquina não devem ser apresentados como desempenho universal.

## Alocação e teste de escala

No menu interativo, a base atual usa vetor estático até 1000 registros e vetor dinâmico acima disso. O programa também usa vetores dinâmicos para a cópia e o auxiliar de Merge Sort. A bateria aloca vetores suficientes para o maior tamanho solicitado.

O teste da opção `11` cresce por duplicação até o orçamento escolhido ou um quarto da memória física disponível no Windows, o que for menor. Ele pode parar sem uma falha de alocação. A opção `13` inicia um segundo processo com limite de memória próprio (Windows Job Object) e cresce até `malloc` falhar. Depois de alocar, o teste gera dados e toca páginas dos três vetores para confirmar o uso de memória. O relatório diferencia a última etapa concluída da primeira tentativa que falhou. Esse limite é **do processo isolado**, não da RAM física total. Insertion Sort não é executado nos tamanhos extremos, pois seu custo quadrático tornaria o teste de capacidade impraticável.

## Limites de interpretação

O programa verifica a ordem das chaves; a bateria ainda não faz uma verificação geral de preservação de todos os registros. A importação CSV aceita apenas o formato simples descrito em [uso](uso.md), e a data recebe validação numérica básica. O domínio de 101 valores provoca muitos empates, que influenciam o comportamento dos algoritmos. Esses limites devem acompanhar qualquer análise dos resultados.
