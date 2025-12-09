#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <set> 

using namespace std::chrono;

class ITemperaturaStorage {
public:
    virtual void insert(double value) = 0;
    virtual void remove(double value) = 0;
    virtual void printSorted() = 0;
    virtual std::vector<double> getMin(int n) = 0;
    virtual std::vector<double> getMax(int n) = 0;
    virtual std::vector<double> rangeQuery(double x, double y) = 0;
    virtual double median() = 0;
    virtual void clear() = 0;
    virtual int size() = 0;
    virtual ~ITemperaturaStorage() {}
};

class VersaoBasica : public ITemperaturaStorage {
private:
    std::vector<double> dados;

public:
    void insert(double value) override {
        auto it = std::lower_bound(dados.begin(), dados.end(), value);
        dados.insert(it, value);
    }

    void remove(double value) override {
        auto it = std::find(dados.begin(), dados.end(), value);
        if (it != dados.end()) {
            dados.erase(it);
        }
    }

    void printSorted() override {
        std::cout << "Dados Ordenados (Versao Basica): ";
        for (double d : dados) std::cout << d << " ";
        std::cout << "\n";
    }

    std::vector<double> getMin(int n) override {
        std::vector<double> result;
        int qtd = std::min((int)dados.size(), n);
        for(int i = 0; i < qtd; i++) result.push_back(dados[i]);
        return result;
    }

    std::vector<double> getMax(int n) override {
        std::vector<double> result;
        int qtd = std::min((int)dados.size(), n);
        for(int i = 0; i < qtd; i++) result.push_back(dados[dados.size() - 1 - i]);
        return result;
    }

    std::vector<double> rangeQuery(double x, double y) override {
        std::vector<double> result;
        for(double d : dados) {
            if (d >= x && d <= y) result.push_back(d);
            if (d > y) break;
        }
        return result;
    }

    double median() override {
        if (dados.empty()) return 0.0;
        if (dados.size() % 2 != 0) {
            return dados[dados.size() / 2];
        } else {
            return (dados[dados.size() / 2 - 1] + dados[dados.size() / 2]) / 2.0;
        }
    }

    void clear() override { dados.clear(); }
    int size() override { return dados.size(); }
};

class VersaoAprimorada : public ITemperaturaStorage {
private:
    std::multiset<double> dados;

public:
    void insert(double value) override {
        dados.insert(value); 
    }

    void remove(double value) override {
        auto it = dados.find(value);
        if (it != dados.end()) {
            dados.erase(it);
        }
    }

    void printSorted() override {
        std::cout << "Dados Ordenados (Versao Aprimorada): ";
        for (double d : dados) std::cout << d << " ";
        std::cout << "\n";
    }

    std::vector<double> getMin(int n) override {
        std::vector<double> result;
        int count = 0;
        for(auto it = dados.begin(); it != dados.end() && count < n; ++it, ++count) {
            result.push_back(*it);
        }
        return result;
    }

    std::vector<double> getMax(int n) override {
        std::vector<double> result;
        int count = 0;
        for(auto it = dados.rbegin(); it != dados.rend() && count < n; ++it, ++count) {
            result.push_back(*it);
        }
        return result;
    }

    std::vector<double> rangeQuery(double x, double y) override {
        std::vector<double> result;
        auto itLow = dados.lower_bound(x);
        auto itUp = dados.upper_bound(y);
        for(auto it = itLow; it != itUp; ++it) {
            result.push_back(*it);
        }
        return result;
    }

    double median() override {
        if (dados.empty()) return 0.0;
        size_t n = dados.size();
        auto it = dados.begin();
        std::advance(it, n / 2);
        
        if (n % 2 != 0) {
            return *it;
        } else {
            double val2 = *it;
            --it;
            double val1 = *it;
            return (val1 + val2) / 2.0;
        }
    }

    void clear() override { dados.clear(); }
    int size() override { return dados.size(); }
};

struct DadosSolo {
    double temperatura;
    double humidade;
    double salinidade;
    std::chrono::system_clock::time_point tempoColeta;

    std::string toString() const {
        return "Temp: " + std::to_string(std::round(temperatura * 10) / 10) + "C | Hum: " +
               std::to_string(std::round(humidade * 10) / 10) + "% | Sal: " +
               std::to_string(std::round(salinidade * 10) / 10) + " dS/m";
    }
};

struct DecisaoIrrigacao {
    std::chrono::system_clock::time_point tempoDecisao;
    bool deveIrrigar;
    std::string motivo;
};

constexpr double LIMITE_UMIDADE_MIN = 40.0;
constexpr double LIMITE_TEMPERATURA_MEDIA_CRITICA = 28.0;
constexpr double LIMITE_SALINIDADE_MAX = 2.0;

double ultimaTemperaturaMediaQuatroHoras = 0.0;

DadosSolo coletarDadosSolo_Simulado() {
    double temp = 20.0 + (std::rand() % 150) / 10.0;
    double hum = 30.0 + (std::rand() % 500) / 10.0;
    double sal = 0.5 + (std::rand() % 20) / 10.0;

    if (std::rand() % 10 < 3) {
        hum = 20.0 + (std::rand() % 150) / 10.0;
    }
    return {temp, hum, sal, std::chrono::system_clock::now()};
}

void apresentarNoSerial(int contador, const DadosSolo& dados, const DecisaoIrrigacao& decisao) {
    std::cout << "[Coleta " << std::setw(2) << std::setfill('0') << contador << "] " << dados.toString();
    std::cout << " -> DECISAO: " << (decisao.deveIrrigar ? "*IRRIGAR*" : "Nao Irrigar");
    std::cout << " (Motivo: " << decisao.motivo << ")\n";
}

DecisaoIrrigacao tomarDecisaoIrrigacao(const DadosSolo& dados) {
    DecisaoIrrigacao decisao = {std::chrono::system_clock::now(), false, "Condicoes ideais ou OK."};

    if (dados.humidade < LIMITE_UMIDADE_MIN) {
        decisao.deveIrrigar = true;
        decisao.motivo = "Umidade muito baixa (" + std::to_string(std::round(dados.humidade * 10) / 10) + "%)";
        return decisao;
    }

    if (ultimaTemperaturaMediaQuatroHoras > LIMITE_TEMPERATURA_MEDIA_CRITICA) {
        decisao.deveIrrigar = true;
        decisao.motivo = "ALTA DEMANDA HIDRICA (Media 4h: " + std::to_string(std::round(ultimaTemperaturaMediaQuatroHoras * 10) / 10) + "C)";
        return decisao;
    }

    if (dados.salinidade > LIMITE_SALINIDADE_MAX) {
        if (dados.humidade > 55.0) {
             decisao.motivo = "Salinidade alta. Monitorar.";
             decisao.deveIrrigar = false;
             return decisao;
        }
    }
    return decisao;
}

void armazenarDecisao(const DadosSolo& dados, const DecisaoIrrigacao& decisao, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (arquivo.is_open()) {
        auto agora = std::chrono::system_clock::to_time_t(decisao.tempoDecisao);
        std::string tempoStr = std::ctime(&agora);
        if (!tempoStr.empty()) tempoStr.pop_back();

        arquivo << "------------------------------------------\n";
        arquivo << "Horario: " << tempoStr << "\n";
        arquivo << "Dados: " << dados.toString() << "\n";
        arquivo << "IRRIGAR: " << (decisao.deveIrrigar ? "SIM" : "NAO") << "\n";
        arquivo.close();
    }
}

void processarCicloDeDados(ITemperaturaStorage& storage) {
    if (storage.size() < 2) return;

    std::cout << "\n**************************************************\n";
    std::cout << "* PROCESSAMENTO DE DADOS (FIM DO CICLO)          *\n";
    std::cout << "**************************************************\n";

    storage.printSorted();

    std::vector<double> menores = storage.getMin(3);
    std::vector<double> maiores = storage.getMax(3);

    std::cout << "3 Menores temps: ";
    for(double v : menores) std::cout << v << " ";
    std::cout << "\n3 Maiores temps: ";
    for(double v : maiores) std::cout << v << " ";
    std::cout << "\n";

    double mediana = storage.median();
    std::cout << "Mediana atual: " << std::fixed << std::setprecision(1) << mediana << " C\n";

    std::vector<double> intervalo = storage.rangeQuery(25.0, 30.0);
    std::cout << "Leituras entre 25.0C e 30.0C: " << intervalo.size() << " ocorrencias.\n";

    if (storage.size() >= 3) {
        ultimaTemperaturaMediaQuatroHoras = mediana; 
    } else {
        ultimaTemperaturaMediaQuatroHoras = mediana;
    }
    
    std::cout << "Media/Referencia p/ Irrigacao: " << ultimaTemperaturaMediaQuatroHoras << " C\n";
    std::cout << "**************************************************\n\n";

    storage.clear();
}

void rodarBenchmark(ITemperaturaStorage* storage, int N) {
    std::cout << "\n--- TESTE DE DESEMPENHO (N=" << N << ") ---\n";

    std::vector<double> valoresGerados;
    valoresGerados.reserve(N);

    auto inicio_insercao = high_resolution_clock::now();
    
    for (int i = 0; i < N; ++i) {
        double temp = 20.0 + (std::rand() % 150) / 10.0;
        storage->insert(temp);
        valoresGerados.push_back(temp);
    }
    
    auto fim_insercao = high_resolution_clock::now();
    auto duracao_insercao = duration_cast<microseconds>(fim_insercao - inicio_insercao);
    
    std::cout << "Insercao de " << N << " elementos: " << duracao_insercao.count() << " us\n";

    auto inicio_mediana = high_resolution_clock::now();
    double mediana = storage->median();
    auto fim_mediana = high_resolution_clock::now();
    auto duracao_mediana = duration_cast<microseconds>(fim_mediana - inicio_mediana);

    std::cout << "Mediana (1 operacao): " << duracao_mediana.count() << " us\n";

    double x = 25.0;
    double y = 35.0;
    
    auto inicio_range = high_resolution_clock::now();
    std::vector<double> resultados_range = storage->rangeQuery(x, y);
    auto fim_range = high_resolution_clock::now();
    auto duracao_range = duration_cast<microseconds>(fim_range - inicio_range);

    std::cout << "Range Query (25.0-35.0): " << duracao_range.count() << " us (" << resultados_range.size() << " resultados)\n";

    if (!valoresGerados.empty()) {
        double valor_remover = valoresGerados.front();
        
        auto inicio_remocao = high_resolution_clock::now();
        storage->remove(valor_remover);
        auto fim_remocao = high_resolution_clock::now();
        auto duracao_remocao = duration_cast<microseconds>(fim_remocao - inicio_remocao);
        
        std::cout << "Remocao (1 elemento): " << duracao_remocao.count() << " us\n";
    }

    storage->clear();
    std::cout << "------------------------------------------\n";
}

void iniciarSimulacaoContinua(ITemperaturaStorage& tempStorage) {
    const int NUM_COLETAS_CICLO = 16;
    int contadorColetas = 0;
    const std::string NOME_ARQUIVO_LOG = "log_irrigacao.txt";

    std::cout << "--- Sistema de Coleta Inteligente Iniciado ---\n";
    std::cout << "Estrutura de dados ativa: " << (dynamic_cast<VersaoAprimorada*>(&tempStorage) ? "Arvore (Otimizada)" : "Lista (Basica)") << "\n";
    std::cout << "==============================================\n";

    while (true) {
        DadosSolo novoDado = coletarDadosSolo_Simulado();
        
        tempStorage.insert(novoDado.temperatura);
        
        contadorColetas++;

        DecisaoIrrigacao decisao = tomarDecisaoIrrigacao(novoDado);
        apresentarNoSerial(contadorColetas, novoDado, decisao);
        armazenarDecisao(novoDado, decisao, NOME_ARQUIVO_LOG);

        if (contadorColetas >= NUM_COLETAS_CICLO) {
            processarCicloDeDados(tempStorage);
            contadorColetas = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    std::srand(std::time(0));
    
    std::vector<int> volumes = {100, 1000, 10000, 100000};
    
    // FASE 1: BENCHMARKS
    
    std::cout << "========================================================\n";
    std::cout << "INICIANDO TESTES: VERSAO BASICA (LISTA ORDENADA - O(N))\n";
    std::cout << "========================================================\n";
    
    VersaoBasica storageBasica;
    for (int N : volumes) {
        rodarBenchmark(&storageBasica, N);
    }

    std::cout << "========================================================\n";
    std::cout << "INICIANDO TESTES: VERSAO APRIMORADA (RBT - O(log N))\n";
    std::cout << "========================================================\n";

    VersaoAprimorada storageAprimorada;
    for (int N : volumes) {
        rodarBenchmark(&storageAprimorada, N);
    }
    
    std::cout << "\n\n**************************************************\n";
    std::cout << "* BENCHMARKS CONCLUIDOS. INICIANDO SIMULACAO AO VIVO. *\n";
    std::cout << "**************************************************\n\n";

    // FASE 2: SIMULAÇÃO CONTÍNUA (USANDO A VERSÃO OTIMIZADA)
    VersaoAprimorada producaoStorage; 
    iniciarSimulacaoContinua(producaoStorage);

    return 0;
}
