#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

unsigned long tempo;
bool conectado;

String nome_recep = "esp_pedro";
String nome_trans = "esp-protese";
String macaddress = "08:B6:1F:B8:50:62";
uint8_t address[6] = { 0x08, 0xB6, 0x1F, 0xB8, 0x50, 0x62 };

void Bt_Status(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_OPEN_EVT) {
    conectado = true;
  }
  else if (event == ESP_SPP_CLOSE_EVT) {
    conectado = false;
  }
}

void setup() {
  conectado = false;
  Serial.begin(115200);
  
  SerialBT.register_callback(status);
  SerialBT.begin(nome_recep, true);
  SerialBT.connect(macadress);
}

void loop() {
  if (!conectado) {
    if (millis() - tempo >= 10000){
      tempo = millis();
      Serial.println("restartando...");
      SerialBT.end();
      SerialBT.begin(myName, true);
      SlaveConnect();
    }
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}
