Análise de Estruturas de Dados para Sensores Industriais 

Este projeto visa resolver problemas de escalabilidade no sistema de monitoramento de temperatura de uma empresa de automação industrial, comparando duas abordagens de estruturas de dados em C++.

## 👥 Autores 
* **João Pedro Jung**
* **Pedro Quitanilha Bezerra**
* **Mateus Francelino Alexandre**

---

## 🛑 O Problema
O sistema legado da empresa utiliza listas lineares ordenadas para armazenar leituras de centenas de sensores que enviam dados continuamente[cite: 6, 10, 11].
**Gargalo:** A complexidade **O(N)** para inserções torna o sistema inviável à medida que o volume de dados cresce[cite: 11].
**Consequência:** Latência e atraso na detecção de anomalias térmicas críticas[cite: 11].

---

## 🚀 A Solução
O trabalho implementa e compara o desempenho de duas versões do sistema:

### 1. Versão Básica (Vetor Ordenado)
* Utiliza `std::vector` do C++].
* Baseada na lógica do **Insertion Sort**.
* **Custo de Inserção:** O(N) devido ao deslocamento de memória.
* **Consultas:** Rápidas (O(1) ou O(logN)) devido à contiguidade de memória.

### [cite_start]2. Versão Aprimorada (Árvore Rubro-Negra / Red-Black Tree) 
* Estrutura escolhida por ser uma árvore binária de busca auto-balanceada
* **Por que Rubro-Negra e não AVL?** Em cenários industriais com muitas escritas (sensores enviando dados o tempo todo), a Rubro-Negra realiza menos rotações que a AVL, oferecendo o melhor compromisso entre atualização e consulta.
* **Complexidade:** Garante O(logN) para inserção, remoção e busca no pior caso.



**Operações Suportadas em ambas:** `insert`, `remove`, `printSorted`, `min/max`, `rangeQuery` e `median`.

---

## 📊 Resultados Experimentais

Os testes de estresse foram realizados variando o tamanho da entrada (N) de 1.000 a 1.000.000, utilizando temperaturas aleatórias (-10°C a 100°C).

**Ambiente de Teste:** CPU AMD Ryzen 5 5500, Compilador g++ (std=c++17).

### Tabela de Comparação de Inserção 

| N (Entradas) | Vetor Ordenado (ms) | Árvore Rubro-Negra (ms) | Análise |
| :--- | :--- | :--- | :--- |
| **1.000** | 35μs | 26μs | Vetor vence ligeiramente |
| **1.000** | 376μs | 268μs | Árvore mais rápida |
| **10.000** | 7.314μs | 3.353μs | Árvore mais rápida |
| **100.000** | 299.268μs | 40.249μs | Vetor inviável |

> **Conclusão:** Para a escala da empresa (milhares de sensores), a Árvore Rubro-Negra é a única solução viável, evitando o travamento do sistema, enquanto o Vetor degrada quadraticamente.

---

## Referências

1. **CORMEN, T. H. et al.** *Algoritmos: Teoria e Prática*. 3. ed. Elsevier, 2012.
2. **SEDGEWICK, R.; WAYNE, K.** *Algorithms*. 4. ed. Addison-Wesley, 2011.
3. Material de aula da disciplina de Estrutura de Dados - Lig Prog.

---

## 📂 Organização do Projeto

A estrutura de diretórios do projeto é organizada da seguinte forma:

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
└── Makefile (ou CMakeLists.txt) text'''



