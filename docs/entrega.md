# Preparação da entrega e do repositório

O repositório deve permitir que outra pessoa compile e execute o projeto sem o executável produzido no computador do grupo. Inclua `src/`, `include/`, `tests/`, `dados/exemplo.csv`, `CMakeLists.txt`, `README.md`, a pasta `docs/` e, se forem usados na monografia, os CSVs e gráficos de `resultados/`. Não é necessário subir `build/` nem arquivos `.exe`; eles estão no `.gitignore`.

## Execução de referência para a monografia

1. Compile em C11 com as instruções do [README](../README.md) e rode os testes (`ctest` com CMake ou o teste compilado manualmente).
2. Escolha **antes** os tamanhos e o número de repetições. A opção `9` usa 100, 1000, 5000 e 10000 registros com cinco repetições; a opção `10` permite uma bateria personalizada. Registre essa escolha.
3. Execute a bateria uma vez no ambiente escolhido. Guarde `tempos.csv`, `resumo.csv` e os três SVGs. Reexecutar `9` ou `10` substitui esses arquivos.
4. Se for demonstrar o limite de memória, execute `13`, anote o limite do processo e guarde `escala_falha.csv`. Não confunda o maior tamanho concluído com a primeira tentativa que falhou. A opção `11` é outro experimento e pode parar apenas pelo orçamento de segurança.
5. Registre data, sistema operacional, CPU, RAM, versão do GCC e opções de compilação. No MSYS2 UCRT64, `gcc --version` informa a versão; no Windows, as informações da máquina podem ser consultadas em **Configurações > Sistema > Sobre**. Anote também que os dados são simulados e que a chave é `nuvens`.
6. Confira se cada linha de `tempos.csv` tem `correto=1` e se os gráficos abrem. Use os números da **mesma execução** nas tabelas, gráficos e discussão da monografia.

Os arquivos atuais de `resultados/` são exploratórios e estão descritos em [resultados-exemplo](resultados-exemplo.md). Se vocês gerarem a execução definitiva, atualizem essa página com os novos números e o ambiente real antes de publicar o repositório. A comparação com limite de memória usa um processo isolado; não afirme que a RAM total do computador se esgotou.

## Entrega acadêmica

O software e o repositório são apenas a parte prática. O guia da APS também pede uma monografia com introdução, referencial teórico, desenvolvimento, resultados e discussão, considerações finais, referências e o código-fonte, além da ficha de atividades. O [roteiro](roteiro.md) resume essa estrutura, mas o PDF oficial continua sendo a referência para formatação e entrega. O grupo confirmou que a implementação será em C; o guia escrito recebido originalmente menciona Java ou Microsoft Visual C#.
