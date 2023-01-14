// Placed in the public domain by Earle F. Philhower, III, 2022
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
 
#ifndef STASSID
#define STASSID "Mikrotik-2G";
#define STAPSK "afzd5862";
#endif


const char* ssid = STASSID;
const char* password = STAPSK;


String wszystkieZaluzjeStatus = "u gory";
String wejscioweZaluzjeStatus = "u gory";
String formattedDate;
String dayStamp;
String timeStamp;
String secretKey = "PhjCVzvU7GPvgC33CMxnRsj8CY9p4n2K";
String tempLog = "";

int godzina, minuta, sekunda;
int najwczesniejszySwit = 4;
int najpozniejszySwit = 7;
int najwczesniejszyWieczor = 16;
int najpozniejszyWieczor = 22;
int czasWlaczeniaRelaya = 3;

bool wszystkie_do_gory_called = false;
bool wszystkie_na_dol_called = false;
bool wejscie_do_gory_called = false;
bool wejscie_na_dol_called = false;
bool brama_garazowa_called = false;
bool brama_wjazdowa_called = false;
bool schedule_dzisiaj_opuszczone = false;
bool schedule_dzisiaj_podniesione = false;
bool czujnikStatus = false;
bool czasUstawionyPoprawnie = false;
bool ustaw_czas_called = false;
bool isSecondCoreUnlocked = false;
bool isSecondCoreInitUnlocked = false;

const int OUT1 = D0;
const int OUT2 = D1;
const int OUT3 = D2;
const int OUT4 = D3;
const int OUT5 = D4;
const int OUT6 = D5;
const int OUT7 = D6;
const int OUT8 = D7;
const int OUTX = D8;
const int ErrorWifi = D9;
const int ErrorCzas = D10;
const int czujnik = A0;
const int czujnikBariera = 400;

WiFiServer server(80);WiFiUDP ntpUDP;NTPClient timeClient(ntpUDP);

void setup1(){
  Serial.begin(115200);  Serial.println("");

  pinMode(ErrorCzas, OUTPUT);  pinMode(LED_BUILTIN, OUTPUT);
  while (!isSecondCoreInitUnlocked){    delay(500);  }

  Serial.println("Second Core: Unlocked");

  timeClient.begin();  timeClient.setTimeOffset(3600);
  //synchronizacja czasu
  if (!timeClient.update()) {    Serial.println("Boot time: read time FAILED");    digitalWrite(ErrorCzas, HIGH);
  } else {    Serial.println("Boot time: " + timeClient.getFormattedTime());    czasUstawionyPoprawnie = true;    digitalWrite(ErrorCzas, LOW);  }

}
void setup() {
  pinMode(OUTX, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(OUT5, OUTPUT);
  pinMode(OUT6, OUTPUT);
  pinMode(OUT7, OUTPUT);
  pinMode(OUT8, OUTPUT);

  digitalWrite(OUT1, HIGH); //turns on main relays
  digitalWrite(OUT2, HIGH); //turns on main relays
  digitalWrite(OUT3, HIGH); //turns on main relays
  digitalWrite(OUT4, HIGH); //turns on main relays
  digitalWrite(OUT5, HIGH); //turns on main relays
  digitalWrite(OUT6, HIGH); //turns on main relays
  digitalWrite(OUT7, HIGH); //turns on main relays
  digitalWrite(OUT8, HIGH); //turns on main relays
  digitalWrite(OUTX, HIGH); //turns on main relays
 
  pinMode(ErrorWifi, OUTPUT);
  delay(2000);
  digitalWrite(ErrorWifi, HIGH);

  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.println();  Serial.print("Connecting to ");  Serial.println(ssid);

  WiFi.mode(WIFI_STA);  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {    delay(500);    Serial.print(F("."));  }

  digitalWrite(ErrorWifi, LOW);
  isSecondCoreInitUnlocked = true;
  Serial.println();  Serial.println("WiFi connected: " + WiFi.SSID() + " (" + WiFi.RSSI() + " dB)");  Serial.println("Mac address: " + WiFi.macAddress());

  timeClient.begin();  timeClient.setTimeOffset(3600);

  server.begin();
  Serial.println("IP Address: " + WiFi.localIP().toString());  Serial.println("Secret Key: " + secretKey);
}
void relays(String ktore, String zadanie) {
  if (ktore == "wszystkie") {
    if (zadanie == "do_gory") {
      wszystkie_do_gory_called = false;
      wszystkieZaluzjeStatus = "u gory";
      wejscioweZaluzjeStatus = "u gory";
      //digitalWrite
      digitalWrite(OUT1, LOW);  //nieparzyste do gory
      digitalWrite(OUT3, LOW);  //nieparzyste do gory
      digitalWrite(OUT5, LOW);  //nieparzyste do gory
      digitalWrite(OUT7, LOW);  //nieparzyste do gory
      delay(czasWlaczeniaRelaya * 1000);
      digitalWrite(OUT1, HIGH);  //nieparzyste do gory
      digitalWrite(OUT3, HIGH);  //nieparzyste do gory
      digitalWrite(OUT5, HIGH);  //nieparzyste do gory
      digitalWrite(OUT7, HIGH);  //nieparzyste do gory
      
      //digitalWrite
    } else {
      wszystkie_na_dol_called = false;
      wszystkieZaluzjeStatus = "na dole";
      wejscioweZaluzjeStatus = "na dole";
      //digitalWrite
      digitalWrite(OUT8, LOW);  //nieparzyste do gory
      digitalWrite(OUT2, LOW);  //parzyste na dol
      digitalWrite(OUT4, LOW);  //parzyste na dol
      digitalWrite(OUT6, LOW);  //parzyste na dol
      delay(czasWlaczeniaRelaya * 1000);
      //digitalWrite
      digitalWrite(OUT8, HIGH);  //nieparzyste do gory
      digitalWrite(OUT2, HIGH);  //parzyste na dol
      digitalWrite(OUT4, HIGH);  //parzyste na dol
      digitalWrite(OUT6, HIGH);  //parzyste na dol
    }
  }
  if (ktore == "wejscie") {
    if (zadanie == "do_gory") {
      wejscie_do_gory_called = false;
      wejscioweZaluzjeStatus = "u gory";
      //digitalWrite

      digitalWrite(OUT7, LOW);  //nieparzyste do gory
      delay(czasWlaczeniaRelaya * 1000);
      //digitalWrite
      digitalWrite(OUT7, HIGH);  //nieparzyste do gory
    } else {
      wejscie_na_dol_called = false;
      wejscioweZaluzjeStatus = "na dole";
      //digitalWrite
      digitalWrite(OUT8, LOW);  //parzyste na dol
      delay(czasWlaczeniaRelaya * 1000);
      digitalWrite(OUT8, HIGH);  //parzyste na dol
      //digitalWrite
    }
  }
  Serial.println("Relays: " + ktore + " " + zadanie);
}
void brama_proc(String obj) {
  if (obj == "brama") {
    brama_wjazdowa_called = false;
    //digitalWrite
    digitalWrite(OUT7, LOW);
    delay(czasWlaczeniaRelaya * 1000);
    digitalWrite(OUT7, HIGH);
    //digitalWrite
  }
  if (obj == "garaz") {
    brama_garazowa_called = false;
    //digitalWrite
    digitalWrite(OUT8, LOW);
    delay(czasWlaczeniaRelaya * 1000);
    digitalWrite(OUT8, HIGH);
    //digitalWrite
  }
  Serial.println("Relays: " + obj);
}
//klasa led
void schedule() {
  ////////////////////////////check time
  if (czasUstawionyPoprawnie) {    if (analogRead(czujnik) > czujnikBariera) {      czujnikStatus = true;
    } else {      czujnikStatus = false;    }
/////////////////////////////PARSE TIME
    String now = timeClient.getFormattedTime();    godzina = now.substring(0, 2).toInt();    minuta = now.substring(3, 5).toInt();    sekunda = now.substring(6, 8).toInt();

////////////////////////////check wifi
    if (WiFi.status() == WL_CONNECTED) {      digitalWrite(ErrorWifi, LOW);
    } else  {        if ((minuta % 2) == 0){digitalWrite(ErrorWifi, HIGH);      WiFi.disconnect();      WiFi.begin(ssid, password);      delay(15000);      Serial.print(".");      }          }


    ///////////////////////////reset
    if (godzina >= 1 && godzina <2 && (minuta % 5) == 0 && sekunda >= 45) {  //miedzy 1 a 3 co 5 min
      schedule_dzisiaj_opuszczone = false;      schedule_dzisiaj_podniesione = false;  tempLog="";  }

    //////////////////////////Jesli nie podniesione
 if (!schedule_dzisiaj_podniesione) {
      if (godzina >= najpozniejszySwit && godzina < najwczesniejszyWieczor && (minuta % 5) == 0 && sekunda >= 15 && sekunda <= 50) {  //godzina 6-16 ci 5 min przedziale 15-30s
        relays("wszystkie", "do gory");        Serial.print(" " + String(godzina) + ":" + String(minuta) + " h");        Serial.println();// 9> (16
        schedule_dzisiaj_podniesione = true;        tempLog +="Automatic up HARD_";tempLog += String(godzina) + ":" + String(minuta)+" ";
        wszystkieZaluzjeStatus = "u gory";
        wejscioweZaluzjeStatus = "u gory";
      }
           if (godzina >= najwczesniejszySwit && godzina < najpozniejszySwit && (minuta % 5) == 0 && czujnikStatus) { //miedzy 6 a 9
           relays("wszystkie", "do gory");        Serial.print(" " + String(godzina) + ":" + String(minuta));        Serial.println(); // 6> (9
        schedule_dzisiaj_podniesione = true;        tempLog +="Automatic up Soft_";tempLog += String(godzina) + ":" + String(minuta)+" ";
        wszystkieZaluzjeStatus = "u gory";
        wejscioweZaluzjeStatus = "u gory";
      }
    }
    ////////////////////////////JESLI NIE OPUSZCZONE
    if (!schedule_dzisiaj_opuszczone) {
      if (godzina >= najpozniejszyWieczor && godzina < najwczesniejszySwit && (minuta % 5) == 0 && sekunda >= 15 && sekunda <= 30) {  //godzina po 23 do ranka
        relays("wszystkie", "na dol");        Serial.print(" " + String(godzina) + ":" + String(minuta) + " h");        Serial.println(); //23> (6)
        schedule_dzisiaj_opuszczone = true;        tempLog +="Automatic down HARD_"; tempLog += String(godzina) + ":" + String(minuta)+" ";
        wszystkieZaluzjeStatus = "na dole";
        wejscioweZaluzjeStatus = "na dole";
      }
      if (godzina >= najwczesniejszyWieczor && godzina < najpozniejszyWieczor && (minuta % 5) == 0 && !czujnikStatus) { //od popoludnia do 23
        relays("wszystkie", "na dol");        Serial.print(" " + String(godzina) + ":" + String(minuta));        Serial.println(); //16> (23
         schedule_dzisiaj_opuszczone = true;         tempLog +="Automatic down Soft_";tempLog += String(godzina) + ":" + String(minuta)+" ";
         wszystkieZaluzjeStatus = "na dole";
         wejscioweZaluzjeStatus = "na dole";
      }
    }
   
  } else {
    ///////if time error then
    if (!timeClient.update()) {      Serial.println("Fetch time: read time FAILED");      digitalWrite(ErrorCzas, HIGH);
    } else {      Serial.println("Fetch time: SUCCESS!");      czasUstawionyPoprawnie = true;      digitalWrite(ErrorCzas, LOW);    }
  }
}
void ustaw_czas() {}
void fakeThread() {}

void loop1(){
  if(isSecondCoreUnlocked){  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  delay(500);

  if (wszystkie_do_gory_called) { relays("wszystkie", "do_gory"); }
  if (wszystkie_na_dol_called) { relays("wszystkie", "na_dol"); }
  if (wejscie_do_gory_called) { relays("wejsciowe", "do_gory"); }
  if (wejscie_na_dol_called) { relays("wejsciowe", "na_dol"); }
  if (brama_wjazdowa_called) { brama_proc("brama"); }
  if (brama_garazowa_called) { brama_proc("garaz"); }
  if (ustaw_czas_called) { ustaw_czas(); }

  schedule();
  }
}
void loop() {
  isSecondCoreUnlocked=true;  WiFiClient client = server.available();  if (!client) {
    //fakeThread(); not used due to 2 cores
    return;  }

  client.setTimeout(5000);

  //READ REQUEST AND FIND STRINGS
  String req = client.readStringUntil('\r');
  Serial.println(req);
  if (req.indexOf("wszystkie/na_dol") != -1) { wszystkie_na_dol_called = true; wszystkieZaluzjeStatus = "na dole";wejscioweZaluzjeStatus ="na dole";}
  if (req.indexOf("wszystkie/do_gory") != -1) { wszystkie_do_gory_called = true; wszystkieZaluzjeStatus = "u gory";wejscioweZaluzjeStatus ="u gory";}
  if (req.indexOf("wejsciowe/na_dol") != -1) { wejscie_na_dol_called = true; wejscioweZaluzjeStatus = "na dole";}
  if (req.indexOf("wejsciowe/do_gory") != -1) { wejscie_do_gory_called = true; wejscioweZaluzjeStatus = "u gory";}
  if (req.indexOf("brama/" + secretKey) != -1) { brama_wjazdowa_called = true; }
  if (req.indexOf("garaz/" + secretKey) != -1) { brama_garazowa_called = true; }
  if (req.indexOf("ustaw_czas/") != -1) { ustaw_czas_called = true; }  //not used

  while (client.available()) {    client.read();  }

  // HTML Post
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><link rel=\"icon\" href=\"data:,\"></head><body>\r\n "));
  client.print("Polaczono do WiFi: " + WiFi.SSID());
  client.print(";<br>");
  client.print("Status wszystkich rolet: " + wszystkieZaluzjeStatus);
  client.print(";<br>");
  client.print("Status wejsciowych rolet: " + wejscioweZaluzjeStatus);
  client.print(";<br>");
  client.print("Status czujnika zmierzchu: ");
  client.print(String(czujnikStatus));
  client.print(";<br>");
  client.print("Lokalny adres ip: ");
  client.print(WiFi.localIP());
  client.print(";<br>");
  client.print("Otwiera od godziny: " + String(najwczesniejszySwit));
  client.print(";<br>");
  client.print("Otwiera do godziny: " + String(najpozniejszySwit));
  client.print(";<br>");
  client.print("Zamyka od godziny: " + String(najwczesniejszyWieczor));
  client.print(";<br>");
  client.print("Zamyka do godziny: " + String(najpozniejszyWieczor));
  client.print(";<br>");
  client.print("Czy czas ustawiony poprawnie: " + String(czasUstawionyPoprawnie));
  client.print(";<br>");
  client.print(tempLog);
  client.print("</body></html>");
  /*if(czasUstawionyPoprawnie){
    client.print("Czas: " + godzina+":"+minuta+":"+sekunda);
  client.print(";<br>");
  }*/
}
