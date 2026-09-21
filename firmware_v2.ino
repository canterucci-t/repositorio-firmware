// Projeto Motiva - Firmware 2.0 (chegou pela atualizacao OTA)
// Faz tudo do 1.0 e agora tambem: ordena, calcula a mediana e usa histerese.
// LED verde = NORMAL | LED vermelho = ALERTA

String versao = "2.0";

int ledVermelho = 25;
int ledVerde = 26;
int ledAzul = 27;

int leituras[5];
int ordenado[5];
int contador = 0;
unsigned long inicio = 0;
bool alerta = false;

void comecarSessao() {
  inicio = millis();
  contador = 0;
  Serial.println("========================================");
  Serial.println("MONITORAMENTO DE VEGETACAO - FW " + versao);
  Serial.println("========================================");
}

void lerValor() {
  leituras[contador] = random(10, 21);  // numero de 10 a 20
  Serial.println("Leitura " + String(contador + 1) + ": " + String(leituras[contador]) + " cm");
  contador++;
}

float media() {
  int soma = 0;
  for (int i = 0; i < 5; i++) {
    soma = soma + leituras[i];
  }
  return soma / 5.0;
}

// copia as leituras e coloca em ordem crescente
void ordenar() {
  for (int i = 0; i < 5; i++) {
    ordenado[i] = leituras[i];
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (ordenado[j] > ordenado[j + 1]) {
        int troca = ordenado[j];
        ordenado[j] = ordenado[j + 1];
        ordenado[j + 1] = troca;
      }
    }
  }
}

void mostrar(String titulo, int lista[]) {
  Serial.print(titulo);
  for (int i = 0; i < 5; i++) {
    Serial.print(String(lista[i]) + " ");
  }
  Serial.println();
}

// histerese: 16 ou mais = ALERTA, 14 ou menos = NORMAL, no meio = nao muda
void verificarEstado(int mediana) {
  if (mediana >= 16) {
    alerta = true;
  }
  if (mediana <= 14) {
    alerta = false;
  }

  if (alerta) {
    Serial.println("Estado: ALERTA (LED vermelho)");
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
  } else {
    Serial.println("Estado: NORMAL (LED verde)");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
  }
}

void fimDaSessao() {
  ordenar();
  int mediana = ordenado[2];  // o valor do meio
  mostrar("Ordem original:  ", leituras);
  mostrar("Ordem crescente: ", ordenado);
  Serial.println("Media da sessao: " + String(media(), 1) + " cm");
  Serial.println("Mediana da sessao: " + String(mediana) + " cm");
  verificarEstado(mediana);
  Serial.println("Proxima sessao em 48 segundos.");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  digitalWrite(ledVerde, HIGH);  // comeca NORMAL
  Serial.println("Rodando o FIRMWARE " + versao + " (atualizado pelo GitHub!)");
  comecarSessao();
}

void loop() {
  // uma leitura a cada 2 segundos
  if (contador < 5 && millis() - inicio >= contador * 2000) {
    lerValor();
    if (contador == 5) {
      fimDaSessao();
    }
  }
  // nova sessao 48 segundos depois do comeco da anterior
  if (millis() - inicio >= 48000) {
    comecarSessao();
  }
}
