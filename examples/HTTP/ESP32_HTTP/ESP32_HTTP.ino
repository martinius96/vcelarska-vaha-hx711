/*|----------------------------------|*/
/*|Projekt: Včelárska váha           |*/
/*|Hardvér: ESP32 (DevKit)           |*/
/*|Autor: Martin Chlebovec           |*/
/*|E-mail: martinius96@gmail.com     |*/
/*|Web: https://arduino.php5.sk      |*/
/*|Licencia pouzitia: MIT            |*/
/*|Revízia: 14. Marec 2020           |*/
/*|----------------------------------|*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "HX711.h"

float calibration_factor = -50.10;
unsigned long zero_factor =  -90709;

const char * ssid = "WIFI_MENO";
const char * password = "WIFI_HESLO";
const char * host = "www.arduino.php5.sk";
const int httpPort = 80; //http port

WiFiClient client;
#define DOUT  23
#define CLK  22
HX711 scale;
void setup() {
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.set_offset(zero_factor); //Zero out the scale using a previously known zero_factor
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi uspesne pripojene");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
  Serial.println("HX711 Ready");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  float hodnota = (scale.get_units(10), 2);
  String hodnota_odosielanie = String(hodnota);
  String data = "hodnota=" + hodnota_odosielanie;
  String url = "/vaha/data.php";
  client.stop();
  if (client.connect(host, httpPort)) {
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println("User-Agent: ESP32");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    Serial.println("Data uspesne odoslane na webserver");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    String line = client.readStringUntil('\n');
  } else {
    Serial.println("Nepodarilo sa pripojit na webserver");
  }
  client.stop();
  scale.power_down();
  for (int i = 0; i <= 300; i++) {
    delay(1000);
  }
  scale.power_up();
}
