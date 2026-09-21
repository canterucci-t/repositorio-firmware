# Projeto Motiva – S2-CP02 | Atualização OTA

ESP32 no Wokwi roda o **Firmware 1.0**, consulta o `version.json` no GitHub, baixa o `firmware_v2.bin`, atualiza via OTA e reinicia no **Firmware 2.0**.

## Integrantes
| Nome | RM |
|---|---|
| Erick Gimenez | RM564748 |
| Henrique Boscoli | RM563651 |
| Joao Henrique | RM563578 |
| Sergio Mirabelo | RM562161 |
| Tomazzo Canterucci | RM565566 |

## Links
- Wokwi: https://wokwi.com/projects/475802110355551233
- Repositório: https://github.com/canterucci-t/repositorio-firmware

## Arquivos
- `firmware_v1.ino` – 5 leituras (10–20 cm) a cada 2 s, sessão a cada 48 s, média, LED azul, OTA após 3 sessões
- `firmware_v2.ino` – tudo do 1.0 + ordenação, mediana e histerese (LED verde = NORMAL, vermelho = ALERTA)
- `firmware_v2.bin` – FW 2.0 compilado (baixado pelo OTA)
- `version.json` – versão disponível + URL do .bin
- `wokwi/diagram.json` – ESP32 + LED RGB (R=25, G=26, B=27) + 3 resistores 220 Ω

## Arquitetura
ESP32 (FW 1.0) → Wi-Fi Wokwi-GUEST → GitHub (version.json + firmware_v2.bin)
Fluxo: consultar versão → comparar → baixar .bin → atualizar → reiniciar

Histerese (mediana): ≥ 16 cm → ALERTA · ≤ 14 cm → NORMAL · entre 14 e 16 → mantém o estado.

## Como executar
1. Os arquivos ficam no repositório público https://github.com/canterucci-t/repositorio-firmware.
2. No Wokwi (ESP32): cole `firmware_v1.ino` em `sketch.ino` e `wokwi/diagram.json` em `diagram.json`.
3. Rode: após 3 sessões o ESP32 baixa o FW 2.0 e reinicia.
