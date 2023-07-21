#include <SoftwareSerial.h>

#define Lora_pinRx 7
#define Lora_pinTx 8
#define ESP_pinRx 4
#define ESP_pinTx 2
#define pinLed 9

SoftwareSerial LoRa_Receptor(Lora_pinRx, Lora_pinTx);
SoftwareSerial ESP_WIFI(ESP_pinRx, ESP_pinTx);

unsigned long looptime = 0;
unsigned long timer = 0;
void setup() {
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  LoRa_Receptor.begin(9600);
  ESP_WIFI.begin(9600);
  Serial.println("LoRa Receiver");
}

void loop() {
  looptime = millis();

  LoRa_Receptor.listen();
  if (LoRa_Receptor.available()) {
    String received_msg = LoRa_Receptor.readString();
    Serial.println("Received message LoRa: " + received_msg); // terminal
    digitalWrite(pinLed, HIGH); //led liga
    timer = looptime; // Atualiza o tempo anterior toda vez que receber uma mensagem
    Serial.println("Seding message to ESP... ");
    ESP_WIFI.listen();
    ESP_WIFI.print(received_msg);// envia para esp
    Serial.println("Seding message to ESP... ");
  }
  // Verifica se passou 1 segundo desde que o LED foi ligado
  if ((looptime - timer >= 2000)) {
    timer = looptime;
    digitalWrite(pinLed, LOW); // Desliga o LED após 1 segundo
  }



}
