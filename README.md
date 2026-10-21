# APS — Análise de desempenho de algoritmos de ordenação

Projeto em **C11** com menu no terminal para comparar o desempenho de Insertion Sort, Merge Sort e Quick Sort. Os registros **simulam** metadados de imagens de satélite: ID, data (AAAAMMDD) e cobertura de nuvens (0 a 100). A chave de ordenação é o percentual de nuvens, em ordem crescente. Nenhuma imagem real ou dado do INPE/TerraBrasilis é utilizado.

Para uma demonstração rápida: compile o programa, execute `build/aps.exe`, use `1` para gerar dados, `4` para comparar os métodos, `9` para gerar tabelas e gráficos e `13` para registrar uma falha de alocação em processo isolado. As opções completas estão em [uso do programa](docs/uso.md).

## Estrutura

- `include/aps.h`: estrutura de dados e interfaces.
- `src/main.c`: menu e comparação de desempenho.
- `src/ordenacao.c`: Insertion Sort, Merge Sort e Quick Sort.
- `src/dados.c`: geração, leitura de CSV, exibição e verificação.
- `src/tempo.c`: medição do tempo.
- `src/experimentos.c`: bateria configurável, tabelas CSV e gráficos SVG.
- `src/escala.c`: teste controlado de capacidade de três vetores em memória.
- `src/estruturas.c`: fila encadeada de testes pendentes e pilha encadeada de resultados.
- `dados/exemplo.csv`: catálogo fictício para testar a importação.
- `docs/uso.md`: menu, arquivos de entrada e saídas.
- `docs/metodologia.md`: geração de dados, medição e limites da comparação.
- `docs/resultados-exemplo.md`: leitura dos resultados exploratórios já gerados.
- `docs/entrega.md`: preparação do repositório e da execução final.
- `docs/arquitetura.md`: organização dos módulos e das estruturas de dados.
- `docs/roteiro.md`: roteiro da monografia.

## Compilar e executar

No Windows, instale o [MSYS2](https://www.msys2.org/), abra o terminal **MSYS2 UCRT64** e instale o GCC com `pacman -S mingw-w64-ucrt-x86_64-gcc`. Confirme com `gcc --version`.

No terminal UCRT64, entre na pasta do projeto e compile:

```bash
# Entre na pasta em que o repositorio foi clonado.
cd "/d/caminho/para/o/projeto"
mkdir -p build
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/dados.c src/ordenacao.c src/tempo.c src/estruturas.c src/experimentos.c src/escala.c -lm -o build/aps.exe
./build/aps.exe
```

Se GCC já estiver disponível no PowerShell, o comando equivalente é:

```powershell
# Execute dentro da pasta do repositorio.
New-Item -ItemType Directory -Path build -Force | Out-Null
$env:Path = 'C:\msys64\ucrt64\bin;' + $env:Path
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/dados.c src/ordenacao.c src/tempo.c src/estruturas.c src/experimentos.c src/escala.c -lm -o build\aps.exe
.\build\aps.exe
```

Ou com CMake e um compilador C configurado:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

O executável fica em `build/aps.exe` ou `build/Release/aps.exe`, conforme o gerador. Execute a partir da raiz do projeto para usar `dados/exemplo.csv` como caminho relativo. No PowerShell, use `& '.\build\aps.exe'` se necessário.

Com CMake configurado, execute `ctest --test-dir build -C Release --output-on-failure` para verificar ordenação, fila FIFO, cópia dos dados enfileirados e pilha LIFO.

## Uso e medição

Escolha `1` para gerar de 1 a 100000 registros, informando o cenário (aleatório, ordenado ou invertido), ou `2` para carregar o CSV de exemplo; depois `4` para comparar. Cada algoritmo recebe uma cópia da mesma entrada. A cópia, a leitura, a geração, a alocação de memória e a exibição ficam fora do intervalo medido. O programa mostra até 20 registros por padrão e verifica a ordem das chaves. A opção `12` alterna para a listagem completa antes e depois de cada algoritmo, inclusive na opção `3`; acione `12` novamente para voltar à amostra. Uma entrada grande pode produzir centenas de milhares de linhas no terminal.

Para comparar várias entradas na ordem escolhida, use `5` para colocar a entrada atual na fila, carregue ou gere outra entrada e repita; `6` executa o teste mais antigo. As opções `7` e `8` consultam e removem o resultado mais recente da pilha. A fila guarda uma cópia dos registros ao enfileirar, portanto trocar a entrada atual não altera um teste pendente. Os dados e resultados guardados são liberados ao sair. Veja [a arquitetura](docs/arquitetura.md) para o fluxo e a análise das estruturas.

A opção `9` executa uma bateria com 100, 1000, 5000 e 10000 registros em cada cenário (aleatório, ordenado e invertido), cinco repetições por combinação. Ela grava 180 tempos individuais em `resultados/tempos.csv`, 36 resumos com média e mediana em `resultados/resumo.csv` e três gráficos SVG em `resultados/`. Executar a opção novamente substitui esses arquivos. Os gráficos usam escala logarítmica e representam valores abaixo de 0,001 ms no piso visual; os tempos exatos permanecem no CSV.

A opção `10` permite escolher de 2 a 6 tamanhos crescentes (até 100000 registros) e de 1 a 10 repetições. Ela exporta os mesmos arquivos da bateria padrão e também substitui os resultados anteriores. Use tamanhos pequenos e grandes para comparar os três algoritmos; Insertion Sort pode levar muito tempo com 100000 registros invertidos.

A opção `11` testa a capacidade de alocação de **três vetores** com tamanhos que dobram a cada etapa. Informe um orçamento de 1 a 1024 MiB; no Windows, o programa usa o menor valor entre esse orçamento e um quarto da memória física disponível. Os vetores são preenchidos para confirmar que a memória foi efetivamente usada. O arquivo `resultados/escala_memoria.csv` registra os tamanhos bem-sucedidos e se a parada ocorreu por falha real de alocação ou pelo limite de segurança.

A opção `13` continua até uma **falha real de alocação** em um processo separado, limitado a 64 a 1024 MiB por um Job Object do Windows. Se não for possível aplicar o limite antes de iniciar o processo, o teste é cancelado. O relatório `resultados/escala_falha.csv` distingue o maior tamanho concluído da primeira tentativa que falhou. O limite é o do processo isolado, não o da RAM total do computador. Os testes de memória não executam Insertion Sort nos tamanhos extremos.

No Windows, o cronômetro usa QueryPerformanceCounter (tempo decorrido). Nos demais sistemas, usa clock (tempo de CPU); resultados dessas plataformas não devem ser comparados diretamente. A comparação manual faz uma medição por algoritmo; a bateria padrão faz cinco repetições por combinação, e a personalizada permite de uma a dez.

CSV: cabeçalho `id,data,nuvens`, separado por vírgulas, sem aspas, até 100000 registros. A data recebe apenas validação numérica básica, sem conferir o calendário. Os dados gerados são fictícios e usam semente fixa 42. Até 1000 registros, a entrada atual fica em um vetor estático; acima disso, em um vetor alocado dinamicamente. A comparação interativa fica limitada a 100000; os testes das opções `11` e `13` exploram tamanhos maiores de memória separadamente da ordenação.

## Escopo desta base

Já inclui os três algoritmos, importação CSV, geração de três cenários com tamanho configurável, menu, medição, fila de testes, pilha de resultados, bateria repetida configurável, CSV de resultados, gráficos SVG, listagem completa e dois modos de teste de memória. Consulte [metodologia](docs/metodologia.md) antes de interpretar os tempos e [status do desenvolvimento](docs/status_desenvolvimento.md) para conhecer os limites atuais.
