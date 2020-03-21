/*|----------------------------------|*/
/*|Projekt: Včelárska váha           |*/
/*|Hardvér: Arduino + Ethernet W5100 |*/
/*|Autor: Martin Chlebovec           |*/
/*|E-mail: martinius96@gmail.com     |*/
/*|Web: https://arduino.php5.sk      |*/
/*|Licencia pouzitia: MIT            |*/
/*|Revízia: 14. Marec 2020           |*/
/*|----------------------------------|*/

#include <avr\wdt.h>
#include <SPI.h>
#include <Ethernet.h>
#include "HX711.h"

float calibration_factor = -50.10;
unsigned long zero_factor =  -90709;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char* host = "www.arduino.php5.sk";
IPAddress ip(192, 168, 1, 101);
EthernetClient client;
#define DOUT  3
#define CLK  2
HX711 scale;
void setup() {
  Serial.begin(115200);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.set_offset(zero_factor); //Zero out the scale using a previously known zero_factor
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  Serial.print("HX711 ready");
  wdt_enable(WDTO_8S);
}

void loop() {
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  wdt_reset();
  float hodnota = (scale.get_units(10), 2);
  String hodnota_odosielanie = String(hodnota);
  delay(50);
  wdt_reset();
  String data = "hodnota=" + hodnota_odosielanie;
  String url = "/vaha/data.php";
  if (client.connect(host, 80)) {
    client.println("POST " + url + " HTTP/1.0");
    client.println("Host: " + (String)host);
    client.println("User-Agent: EthernetW5100");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    Serial.println("Data uspesne odoslane na web");
  } else {
    Serial.println("Pripojenie zlyhalo...");
  }
  client.stop();
  scale.power_down();
  for (int i = 0; i <= 300; i++) {
    delay(1000);
    wdt_reset();
  }
  scale.power_up();
}
