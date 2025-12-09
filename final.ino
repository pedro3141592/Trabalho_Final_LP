const int PIN_TEMP = A0;
const int PIN_UMID = A1;
const int PIN_SAL  = A2;
const int PIN_BOMBA = 13;

const float LIMITE_UMIDADE_MIN = 40.0;
const float LIMITE_TEMP_MEDIA_CRITICA = 28.0;
const float LIMITE_SALINIDADE_MAX = 2.0;

const int MAX_AMOSTRAS = 10; 
float temperaturas[MAX_AMOSTRAS];
int qtdAmostras = 0;

unsigned long ultimoTempo = 0;
const long INTERVALO_LEITURA = 2000;

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_BOMBA, OUTPUT);
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_UMID, INPUT);
  pinMode(PIN_SAL, INPUT);
  
  Serial.println("--- INICIANDO SISTEMA NO ARDUINO ---");
  Serial.println("Simulando Versao Basica (Array Ordenado)");
}

float lerTemperatura() {
  int leitura = analogRead(PIN_TEMP);
  float voltagem = leitura * 5.0;
  voltagem /= 1024.0; 
  return (voltagem - 0.5) * 100; 
}

float lerUmidade() {
  int leitura = analogRead(PIN_UMID);
  return map(leitura, 0, 1023, 0, 100); 
}

float lerSalinidade() {
  int leitura = analogRead(PIN_SAL);
  return (float)leitura * (5.0 / 1023.0);
}

void inserirOrdenado(float novaTemp) {
  if (qtdAmostras < MAX_AMOSTRAS) {
    int i = qtdAmostras - 1;
    while (i >= 0 && temperaturas[i] > novaTemp) {
      temperaturas[i + 1] = temperaturas[i];
      i--;
    }
    temperaturas[i + 1] = novaTemp;
    qtdAmostras++;
  } else {
    Serial.println("[AVISO] Memoria cheia. Reiniciando ciclo de coleta...");
    qtdAmostras = 0; 
    temperaturas[0] = novaTemp;
    qtdAmostras++;
  }
}

void loop() {
  unsigned long tempoAtual = millis();
  
  if (tempoAtual - ultimoTempo >= INTERVALO_LEITURA) {
    ultimoTempo = tempoAtual;
    
    float temp = lerTemperatura();
    float umid = lerUmidade();
    float sal  = lerSalinidade();
    
    inserirOrdenado(temp);
    
    float soma = 0;
    for(int i=0; i<qtdAmostras; i++) soma += temperaturas[i];
    float media = (qtdAmostras > 0) ? (soma / qtdAmostras) : 0.0;
    
    bool irrigar = false;
    String motivo = "";

    if (umid < LIMITE_UMIDADE_MIN) {
      irrigar = true;
      motivo = "Umidade Baixa";
    } else if (media > LIMITE_TEMP_MEDIA_CRITICA) {
      irrigar = true;
      motivo = "Alta Temp Media";
    }

    if (sal > LIMITE_SALINIDADE_MAX && umid > 55.0) {
      irrigar = false;
      motivo = "Salinidade Alta (Bloqueio)";
    }
    
    if (irrigar) {
      digitalWrite(PIN_BOMBA, HIGH);
    } else {
      digitalWrite(PIN_BOMBA, LOW);
    }
    
    Serial.print("T: "); Serial.print(temp); 
    Serial.print("C | U: "); Serial.print(umid); 
    Serial.print("% | S: "); Serial.print(sal);
    Serial.print(" | Media(Hist): "); Serial.print(media);
    Serial.print(" -> DECISAO: "); 
    Serial.println(irrigar ? "IRRIGAR" : "NAO IRRIGAR");
    
    Serial.print("   [Dados Ordenados]: ");
    for(int i=0; i<qtdAmostras; i++) {
      Serial.print(temperaturas[i]);
      Serial.print(" ");
    }
    Serial.println("\n");
  }
}