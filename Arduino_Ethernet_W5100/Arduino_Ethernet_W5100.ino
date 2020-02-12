/*|---------------------------------|*/
/*|Projekt: Včelárska váha          |*/
/*|Autor: Martin Chlebovec          |*/
/*|E-mail: martinius96@gmail.com    |*/
/*|Web: https://arduino.php5.sk     |*/
/*|Licencia pouzitia: MIT           |*/
/*|---------------------------------|*/

#include <avr\wdt.h>
#include <SPI.h>
#include <Ethernet.h>
#include "HX711.h"
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.arduino.php5.sk";
IPAddress ip(192, 168, 1, 101);
EthernetClient client;
HX711 scale;
void setup() {
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      //raw from ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   //average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   //average of 5 readings from the ADC

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  //average of 5 readings from the ADC
  // by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);  // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();  // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 //raw reading from ADC

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
  float hodnota = scale.get_units(10);
  String hodnota_odosielanie = String(hodnota));
  delay(50);
  if (client.connect(server, 80)) {
    String hodnota = String(vzdialenost);
    Serial.println("Pripojenie uspesne na webserver, vykonavam request... ");
    client.print("GET /vaha/data.php?hodnota=");
    client.print(hodnota_odosielanie);
    client.println(" HTTP/1.1");
    client.println("Host: www.arduino.php5.sk");
    client.println("Connection: close");
    client.println();
    Serial.println("Data uspesne odoslane na web");
    client.stop();
  } else {
    Serial.println("Pripojenie zlyhalo...");
  }
}
scale.power_down();
for (int i = 0; i <= 300; i++) {
  delay(1000);
  wdt_reset();
}
scale.power_up();
}
