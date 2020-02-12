# Včelárska váha na platforme Arduino / ESP
* Včelárska váha má využitie predovšetkým pri vážení úľov a včelých produktov. 
* Cieľom je mať prehľad o stave úľa kedykoľvek bez nutnosti cestovania na fyzickú kontrolu úľa.
* Váha môže byť pripojená do internetu a byť zviazaná s webaplikáciou, do ktorej odosiela dáta.
* Webaplikácia ponúka používateľské rozhranie s prehľadom údajov v reálnom čase, spojenie s MySQL databázou, kde ukladá dáta z mikrokontrolérov.
* Druhotnou implementáciou váhy môže byť aj komunikácia inými technológiami na prenos rádovo v metroch až kilometroch, pričom výstup môže byť napríklad do UART monitoru, alebo môžu byť dáta odosielané do internetu inou technológiou.
* Váha pre váženie používa tenzometer a analógovo-digitálny prevodník - HX711 s rozlíšením 24-bitov, ktorý slúži na veľmi presné merania.
* Možnosť vážiť gramy, až desiatky kilogramov, existujú tenzometre na 1kg, 5kg, 10kg, 20kg, ktoré sa predávajú priamo s prevodníkom HX711.
# Hardvér pre prenos údajov do webového rozhrania
* Arduino + Ethernet Wiznet W5100 - HTTP
* ESP8266 (WiFi) - HTTP / HTTPS
* ESP32 (WiFi) - HTTP / HTTPS
* Arduino + Sigfox Wisol SFM10R1 (868MHz) - HTTPS callback
# Hardvér pre prenos údajov medzi včelárskymi váhami (nie do webového rozhrania)
* RF vysielač a prijímač (315/433MHz) - max. 80 metrov
* nRF24L01+ / nRF24L01 + PA + LNA (2,4GHz) - max. 1100 metrov
* IR (vlnová dĺžka 700 - 1000nm) - max. 10 metrov
* RS232/RS485 - max. 1200 metrov
* LoRa - max. stovky kilometrov
![Prehľad v reálnom čase - admin rozhranie](https://i.imgur.com/UuNIDZg.png)
![História meraní včelárskej váhy](https://i.imgur.com/pxd80Br.png)
![Schéma zapojenia](https://i.imgur.com/BgRQWAG.png)
