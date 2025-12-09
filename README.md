 Análise de Estruturas de Dados para Sensores Industriais
Disciplina: João Pedro Jung , Pedro Quitanilha Bezerra, Mateus Francelino Tilápia
Linguagem: C++


1. Resumo
Este projeto visa resolver problemas de escalabilidade no sistema de monitoramento de temperatura de uma empresa de automação industrial. O sistema legado, baseado em listas ordenadas simples, apresenta degradação de performance com o aumento do volume de dados. O trabalho implementa e compara duas abordagens: uma Versão Básica (Vetor Ordenado com Insertion Sort) e uma Versão Aprimorada (Árvore Rubro-Negra / Red-Black Tree), analisando o desempenho assintótico e prático para operações de inserção, remoção, busca e estatísticas em tempo real.


2. Descrição do Problema e Solução
O Problema
O sistema atual utiliza listas lineares que exigem reordenação a cada nova leitura de sensor. Com centenas de sensores enviando dados continuamente, a complexidade O(N) para inserções torna o sistema inviável, causando latência (atraso) na detecção de anomalias térmicas.
A Solução Escolhida: Árvore Rubro-Negra
Para a versão aprimorada, escolhemos a Árvore Rubro-Negra (Red-Black Tree).
Justificativa: É uma árvore binária de busca auto-balanceada que utiliza bits de cor (vermelho ou preto) para manter o equilíbrio.
Por que Rubro-Negra e não AVL? Embora a AVL seja mais estritamente balanceada (melhor para buscas), a Rubro-Negra realiza menos rotações durante operações de escrita (inserção/remoção). Em um cenário industrial onde sensores enviam dados o tempo todo (muitas escritas), a Rubro-Negra oferece o melhor compromisso entre velocidade de atualização e velocidade de consulta. É a mesma estrutura usada internamente no std::map do C++.


3. Descrição Técnica
As duas implementações suportam as operações: insert, remove, printSorted, min/max, rangeQuery e median.
Abordagem 1: Vetor Ordenado (Versão Básica)
Utiliza um std::vector do C++.
Inserção: Utiliza a lógica do Insertion Sort. Encontra a posição correta e desloca todos os elementos. Custo: O(N).
Consultas: Rápidas (O(1) ou O(logN)) pois os dados estão contíguos na memória.
Problema: O custo de deslocar memória na inserção cresce linearmente.
Abordagem 2: Árvore Rubro-Negra (Versão Aprimorada)
Implementação de nós ligados contendo ponteiros, valor e atributo de cor.
Propriedades:
Todo nó é vermelho ou preto.
A raiz é sempre preta.
Não existem dois nós vermelhos consecutivos (filhos de vermelho são pretos).
Todo caminho da raiz até uma folha (NULL) tem o mesmo número de nós pretos.
Complexidade: Garante operações de inserção, remoção e busca em O(logN) no pior caso.
Mediana: Implementada via percurso in-order até o índice central.


4. Metodologia de Comparação
Para validar a solução, realizamos testes de estresse variando o tamanho da entrada (N).
Ambiente de Teste:
CPU:  AMD Ryzen 5 5500
Compilador: g++ (std=c++17)
Dados:
Temperaturas aleatórias (-10°C a 100°C) 
Volumes de teste (N): 1.000, 10.000, 100.000, 1.000.000.
Métrica:
Tempo de execução medido com std::chrono::high_resolution_clock.
5. Resultados Experimentais
Tabela de Tempos de Execução (Média de 10 execuções)

N (Entradas)OperaçãoVetor Ordenado (ms)Árvore Rubro-Negra (ms)Análise1000Inserção35μs26μsVetor vence 1.000Inserção376μs268μsÁrvore mais rápida10.000Inserção7314μs3353μsÁrvore mais rápida100.000Inserção299268μs40249μsVetor inviável
Interpretação (TL;DR)
Inserção: A Árvore Rubro-Negra mantém desempenho logarítmico estável. O Vetor degrada quadraticamente (inserir 10x mais dados custa 100x mais tempo).
Busca: O Vetor é ligeiramente mais rápido em buscas puras devido à localidade de referência (cache do processador), mas a Árvore é suficientemente rápida (O(logN)) para atender aos requisitos de tempo real.
Conclusão: Para a escala da empresa (milhares de sensores), a Árvore Rubro-Negra é a única solução viável, pois evita o travamento do sistema durante picos de recebimento de dados.



6. Organização do Código
ProjetoIrrigacao/
├── include/
│   ├── ITemperaturaStorage.hpp   # Interface (Contrato)
│   ├── BasicStorage.hpp          # Implementação com Vector
│   ├── OptimizedStorage.hpp      # Implementação com Multiset
│   ├── DomainModels.hpp          # Structs (DadosSolo, Decisao)
│   └── IrrigationSystem.hpp      # Lógica de controle e simulação
├── src/
│   ├── IrrigationSystem.cpp      # Implementação da lógica
│   └── main.cpp                  # Ponto de entrada
└── Makefile (ou CMakeLists.txt)
Como Executar
Compile: g++ src/*.cpp -o monitor_temp
Execute: ./monitor_temp


7. Referências
CORMEN, T. H. et al. Algoritmos: Teoria e Prática. 3. ed. Elsevier, 2012.
SEDGEWICK, R.; WAYNE, K. Algorithms. 4. ed. Addison-Wesley, 2011.
Material de aula da disciplina de Estrutura de Dados - Lig Prog.



