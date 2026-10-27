# Preparação da entrega e do repositório

O repositório inclui o código-fonte (`src/`, `include/`, `tests/`), o CSV de exemplo, CMake, documentação e resultados. Inclui também **`aps.exe` na raiz**, compilado para Windows x64, para demonstrar o programa sem instalar um compilador. A pasta `build/` e outros `.exe` continuam excluídos pelo `.gitignore`. Após alterar o código, recompile e atualize o `aps.exe` publicado; ele não muda automaticamente quando os arquivos `.c` são enviados ao GitHub.

## Execução de referência para a monografia

1. Compile em C11 com as instruções do [README](../README.md) e rode os testes (`ctest` com CMake ou o teste compilado manualmente).
2. Escolha **antes** os tamanhos e o número de repetições. A opção `9` usa 100, 1000, 5000 e 10000 registros com cinco repetições; a opção `10` permite uma bateria personalizada. Registre essa escolha.
3. Execute a bateria uma vez no ambiente escolhido. Guarde `tempos.csv`, `resumo.csv` e os três SVGs. Reexecutar `9` ou `10` substitui esses arquivos.
4. Se for demonstrar a falha de memória, execute `13` e guarde `escala_falha.csv`. A opção não impõe teto artificial e pode deixar o computador lento ou sem resposta. Não confunda o maior tamanho concluído com a primeira tentativa que falhou. Se o processo terminar antes de registrar `falha_alocacao`, relate isso como interrupção. A opção `11` é outro experimento e pode parar apenas pelo orçamento de segurança.
5. Registre data, sistema operacional, CPU, RAM, versão do GCC e opções de compilação. No MSYS2 UCRT64, `gcc --version` informa a versão; no Windows, as informações da máquina podem ser consultadas em **Configurações > Sistema > Sobre**. Anote também que os dados são simulados e que a chave é `nuvens`.
6. Confira se cada linha de `tempos.csv` tem `correto=1` e se os gráficos abrem. Use os números da **mesma execução** nas tabelas, gráficos e discussão da monografia.

Os arquivos atuais de `resultados/` são exploratórios e estão descritos em [resultados-exemplo](resultados-exemplo.md). O arquivo antigo `escala_falha.csv` veio de uma versão com limite de processo de 64 MiB; gere um novo relatório para documentar a versão atual. Se vocês gerarem a execução definitiva, atualizem essa página com os novos números e o ambiente real antes de publicar o repositório. Uma falha de alocação não prova, sozinha, que a RAM física inteira se esgotou.

## Entrega acadêmica

O software e o repositório são apenas a parte prática. O guia da APS também pede uma monografia com introdução, referencial teórico, desenvolvimento, resultados e discussão, considerações finais, referências e o código-fonte, além da ficha de atividades. O [roteiro](roteiro.md) resume essa estrutura, mas o PDF oficial continua sendo a referência para formatação e entrega. O grupo confirmou que a implementação será em C; o guia escrito recebido originalmente menciona Java ou Microsoft Visual C#.
