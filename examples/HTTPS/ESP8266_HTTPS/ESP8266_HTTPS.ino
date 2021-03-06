/*|----------------------------------|*/
/*|Projekt: Včelárska váha           |*/
/*|Hardvér: NodeMCU v3 (ESP8266)     |*/
/*|Autor: Martin Chlebovec           |*/
/*|E-mail: martinius96@gmail.com     |*/
/*|Web: https://arduino.php5.sk      |*/
/*|Licencia pouzitia: MIT            |*/
/*|Revízia: 14. Marec 2020           |*/
/*|----------------------------------|*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "HX711.h"

float calibration_factor = -50.10;
unsigned long zero_factor =  -90709;
const char * ssid = "MENO_WIFI";
const char * password = "HESLO_WIFI";
const char * host = "arduino.php5.sk"; //bez https a www
const int httpsPort = 443; //https port
const char fingerprint[] PROGMEM = "00 2c c1 3a 3c fd a2 0a a3 f1 19 1a ee ee 54 72 93 56 7d 1b";
#define DOUT  5
#define CLK  4
HX711 scale;
void setup() {
  Serial.begin(115200);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.set_offset(zero_factor); //Zero out the scale using a previously known zero_factor
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi uspesne pripojene");
  Serial.println("IP adresa: ");
  Serial.println(WiFi.localIP());
  Serial.println("HX711 ready");
}

void loop() {
  float hodnota = (scale.get_units(10), 2);
  String hodnota_odosielanie = String(hodnota);
  delay(50);
  WiFiClientSecure client;
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);
  String data = "hodnota=" + hodnota_odosielanie;
  String url = "/vaha/data.php";
  if (client.connect(host, httpsPort)) {
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println("User-Agent: ESP8266");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    Serial.println("Data uspesne odoslane na webserver"); while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    String line = client.readStringUntil('\n');
  } else {
    Serial.println("Problem s pripojenim na webserver");
  }
  client.stop();
  scale.power_down();
  for (int i = 0; i <= 300; i++) {
    delay(1000);
  }
  scale.power_up();
}
