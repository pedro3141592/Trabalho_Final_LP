# [cite_start]Análise de Estruturas de Dados para Sensores Industriais [cite: 1]

[cite_start]Este projeto visa resolver problemas de escalabilidade no sistema de monitoramento de temperatura de uma empresa de automação industrial, comparando duas abordagens de estruturas de dados em C++[cite: 5, 7].

## [cite_start]👥 Autores [cite: 2]
* **João Pedro Jung**
* **Pedro Quitanilha Bezerra**
* **Mateus Francelino Tilápia**

---

## 🛑 O Problema
[cite_start]O sistema legado da empresa utiliza listas lineares ordenadas para armazenar leituras de centenas de sensores que enviam dados continuamente[cite: 6, 10, 11].
* [cite_start]**Gargalo:** A complexidade **O(N)** para inserções torna o sistema inviável à medida que o volume de dados cresce[cite: 11].
* [cite_start]**Consequência:** Latência e atraso na detecção de anomalias térmicas críticas[cite: 11].

---

## 🚀 A Solução
[cite_start]O trabalho implementa e compara o desempenho de duas versões do sistema[cite: 7]:

### 1. Versão Básica (Vetor Ordenado)
* [cite_start]Utiliza `std::vector` do C++[cite: 21].
* [cite_start]Baseada na lógica do **Insertion Sort**[cite: 22].
* [cite_start]**Custo de Inserção:** O(N) devido ao deslocamento de memória[cite: 23, 25].
* [cite_start]**Consultas:** Rápidas (O(1) ou O(logN)) devido à contiguidade de memória[cite: 24].

### [cite_start]2. Versão Aprimorada (Árvore Rubro-Negra / Red-Black Tree) [cite: 12]
* [cite_start]Estrutura escolhida por ser uma árvore binária de busca auto-balanceada[cite: 14].
* [cite_start]**Por que Rubro-Negra e não AVL?** Em cenários industriais com muitas escritas (sensores enviando dados o tempo todo), a Rubro-Negra realiza menos rotações que a AVL, oferecendo o melhor compromisso entre atualização e consulta[cite: 15, 16].
* [cite_start]**Complexidade:** Garante O(logN) para inserção, remoção e busca no pior caso[cite: 33].



[cite_start]**Operações Suportadas em ambas:** `insert`, `remove`, `printSorted`, `min/max`, `rangeQuery` e `median`[cite: 19].

---

## 📊 Resultados Experimentais

[cite_start]Os testes de estresse foram realizados variando o tamanho da entrada (N) de 1.000 a 1.000.000, utilizando temperaturas aleatórias (-10°C a 100°C)[cite: 36, 41, 42].

[cite_start]**Ambiente de Teste:** CPU AMD Ryzen 5 5500, Compilador g++ (std=c++17)[cite: 38, 39].

### [cite_start]Tabela de Comparação de Inserção [cite: 46, 47]

| N (Entradas) | Vetor Ordenado (ms) | Árvore Rubro-Negra (ms) | Análise |
| :--- | :--- | :--- | :--- |
| **1.000** | 35μs | 26μs | Vetor vence ligeiramente |
| **1.000** | 376μs | 268μs | Árvore mais rápida |
| **10.000** | 7.314μs | 3.353μs | Árvore mais rápida |
| **100.000** | 299.268μs | 40.249μs | Vetor inviável |

> [cite_start]**Conclusão:** Para a escala da empresa (milhares de sensores), a Árvore Rubro-Negra é a única solução viável, evitando o travamento do sistema, enquanto o Vetor degrada quadraticamente[cite: 49, 51].

---

## 📂 Organização do Projeto

[cite_start]A estrutura de diretórios do projeto é organizada da seguinte forma [cite: 52-63]:

```text
ProjetoIrrigacao/
├── include/
│   ├── ITemperaturaStorage.hpp   # Interface (Contrato)
│   ├── BasicStorage.hpp          # Implementação com Vector
│   ├── OptimizedStorage.hpp      # Implementação com Multiset (RB Tree)
│   ├── DomainModels.hpp          # Structs (DadosSolo, Decisao)
│   └── IrrigationSystem.hpp      # Lógica de controle e simulação
├── src/
│   ├── IrrigationSystem.cpp      # Implementação da lógica
│   └── main.cpp                  # Ponto de entrada
└── Makefile (ou CMakeLists.txt)
