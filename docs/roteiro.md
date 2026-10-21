# Guia da APS e roteiro de trabalho

Resumo de planejamento baseado no guia `APS_3_4_CC_2026_EDados.pdf` (2026/2). Preencher a monografia com pesquisa, referências verificadas e **tempos realmente medidos sobre dados simulados**; este roteiro não substitui o documento original.

## Requisitos do sistema

- Contexto: dados catalogados de imagens de satélite da floresta amazônica.
- Implementar **pelo menos três** técnicas de ordenação e comparar seu desempenho pelo **tempo total da ordenação**. Aquisição, geração, cópia e exibição dos dados ficam fora da medição.
- Aceitar dados externos de arquivo e dados internos gerados pelo programa **ou** informados pelo usuário. O projeto atual já oferece arquivo e geração aleatória.
- Mostrar valores antes e depois de ordenar e apresentar comparação de desempenho.
- Testar tamanhos variados e cenários com dados ordenados, parcialmente ordenados e aleatórios; apresentar tabelas, gráficos e discussão das vantagens e desvantagens.
- Cuidar do refinamento, tratamento de erros e complexidade das técnicas.
- O guia escrito especifica Java ou Microsoft Visual C#. **A escolha confirmada para este projeto é C**; registrar essa divergência e a orientação recebida pelo grupo na entrega acadêmica.

## Entrega escrita

Ordem indicada pelo guia: capa (curso, tema, nomes e RAs), índice, objetivo, introdução, referencial teórico, desenvolvimento, resultados e discussão, considerações finais, referências bibliográficas, código-fonte e ficha de APS.

Formato indicado: papel A4, Arial 12, espaçamento 1,5, margens direita e esquerda de 2,5 cm, encadernação espiral e capa transparente. O guia não especifica aqui as margens superior e inferior.

| Parte | Extensão indicada |
| --- | --- |
| Introdução | 2 a 4 páginas |
| Referencial teórico | 3 a 5 páginas |
| Desenvolvimento | 10 a 15 páginas |
| Resultados e discussão | 6 a 10 páginas |
| Considerações finais | 1 a 3 páginas |
| Referências bibliográficas | mínimo de 2 páginas |
| Código-fonte | sem limite |

O guia prevê grupo de quatro alunos; quantidade diferente depende de aprovação da coordenação. Exige ficha padrão da APS com atividades em ordem cronológica. A data de entrega será informada pelo coordenador auxiliar.

## Conteúdo da monografia

1. Introdução: contexto do catálogo de imagens da Amazônia e objetivo da comparação.
2. Referencial teórico: funcionamento, complexidade, estabilidade e memória de Insertion Sort, Merge Sort e Quick Sort.
3. Desenvolvimento: modelo dos registros, obtenção dos dados, implementação em C, critério de ordenação, tratamento de erros e metodologia de medição.
4. Resultados e discussão: tabelas e gráficos de tempos medidos, vantagens e limitações por cenário.
5. Considerações finais: conclusões apoiadas nos experimentos.
6. Referências bibliográficas: listar somente fontes efetivamente consultadas.
7. Código-fonte: incluir a versão final utilizada nos testes.

## Preparação da monografia e melhorias opcionais

- Executar a bateria definitiva e registrar tamanho, cenário, semente, compilador, otimização e equipamento, conforme [preparação da entrega](entrega.md).
- Usar os CSVs e gráficos exportados para discutir os resultados, como exemplificado em [resultados-exemplo](resultados-exemplo.md).
- Distinguir no texto a parada pelo orçamento da opção `11` e a falha no processo isolado da opção `13`.
- Como melhorias opcionais do software: permitir entrada manual, acrescentar cenário parcialmente ordenado, ampliar testes de preservação dos registros e validar datas de calendário no CSV.

O plano de ensino da disciplina foi lido. A aplicação atual usa uma fila de testes pendentes e uma pilha de resultados, além de vetores, listas encadeadas, ponteiros, recursividade e análise de complexidade. A função de cada estrutura está em `docs/arquitetura.md`. Os demais tópicos do currículo não precisam ser incluídos artificialmente; serão reavaliados conforme a evolução do sistema.
