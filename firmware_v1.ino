// Projeto Motiva - Firmware 1.0
// Faz 5 leituras a cada sessao e mostra a media.
// Depois de 3 sessoes, procura a versao nova no GitHub e atualiza.

#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>

String versao = "1.0";
String linkJson = "https://raw.githubusercontent.com/canterucci-t/repositorio-firmware/main/version.json";

int ledVermelho = 25;
int ledVerde = 26;
int ledAzul = 27;

int leituras[5];
int contador = 0;
int sessoes = 0;
unsigned long inicio = 0;
bool atualizado = false;

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

// pega um valor do arquivo json
String pegarValor(String texto, String nome) {
  int posicao = texto.indexOf(nome);
  int comeco = texto.indexOf("\"", texto.indexOf(":", posicao)) + 1;
  int fim = texto.indexOf("\"", comeco);
  return texto.substring(comeco, fim);
}

void procurarAtualizacao() {
  Serial.println("Procurando atualizacao...");

  // 1) conectar no wifi
  WiFi.begin("Wokwi-GUEST", "", 6);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    tentativas++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Erro: sem internet");
    return;
  }
  Serial.println("Wi-Fi conectado");

  // 2) baixar o version.json
  WiFiClientSecure cliente;
  cliente.setInsecure();
  HTTPClient http;
  http.begin(cliente, linkJson);
  if (http.GET() != 200) {
    Serial.println("Erro: nao consegui abrir o version.json");
    return;
  }
  String texto = http.getString();
  http.end();

  // 3) comparar as versoes
  String versaoNova = pegarValor(texto, "version");
  String linkBin = pegarValor(texto, "url");
  Serial.println("Versao atual: " + versao + " | Versao no GitHub: " + versaoNova);

  if (versaoNova.toFloat() <= versao.toFloat()) {
    Serial.println("Ja esta na versao mais nova");
    atualizado = true;
    return;
  }

  // 4) baixar o .bin e atualizar
  Serial.println("Versao nova encontrada! Baixando...");
  if (httpUpdate.update(cliente, linkBin) != HTTP_UPDATE_OK) {
    Serial.println("Erro ao baixar ou atualizar: " + httpUpdate.getLastErrorString());
  }
  // se der certo, o ESP32 reinicia sozinho
}

void setup() {
  Serial.begin(115200);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  digitalWrite(ledAzul, HIGH);  // azul = versao 1.0
  Serial.println("Rodando o FIRMWARE " + versao);
  comecarSessao();
}

void loop() {
  // uma leitura a cada 2 segundos
  if (contador < 5 && millis() - inicio >= contador * 2000) {
    lerValor();
    if (contador == 5) {
      Serial.println("Media da sessao: " + String(media(), 1) + " cm");
      Serial.println("Proxima sessao em 48 segundos.");
      sessoes++;
      if (sessoes >= 3 && !atualizado) {
        procurarAtualizacao();
      }
    }
  }
  // nova sessao 48 segundos depois do comeco da anterior
  if (millis() - inicio >= 48000) {
    comecarSessao();
  }
}
