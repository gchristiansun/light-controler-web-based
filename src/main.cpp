// D4, D5, dan D19.
// 3v3, GND

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FS.h>
#include <SPIFFS.h>
#include <DNSServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WebServer server(80);

const char* ssid = "Web IoT";
const char* password = "12345678";

const int espLed = 2;
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 19;
const int ledPin4 = 14;

bool dapurLedState = false;
bool tamuLedState = false;
bool makanLedState = false;
bool toiletLedState = false;


void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(espLed, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  // WiFi.begin(ssid, password);
  // Serial.print("connecting to wifi import java.util.*...");

  void getDapurLed();
  void getTamuLed();
  void getMakanLed();
  void getToiletLed();
  void setDapurLed();
  void setTamuLed();
  void setMakanLed();
  void setToiletLed();


  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED tidak ditemukan"));
    for (;;);
  }

  // Menampilkan nama sistem
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Web IoT - AP Mode");
  display.display();

  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();

  display.setCursor(0, 10);
  display.println("WiFi: AP Mode");
  display.setCursor(0, 20);
  display.print("IP: ");
  display.println(ip);
  display.display();

  // int i = 0;
  // int y = 0;
  // delay(500);
  // display.clearDisplay();
  // while (WiFi.status() != WL_CONNECTED) {
  //   digitalWrite(espLed, LOW);
  //   delay(1000);
  //   display.setTextSize(1);
  //   display.setTextColor(SSD1306_WHITE);
  //   display.setCursor(0, 0);
  //   display.print("Connecting");
  //   display.display();
  //   display.setCursor(60 + y, 0);
  //   display.print(".");
  //   display.display();
  //   delay(200);
  //   y += 3;
  //   i++;
  //   if(i == 3) {
  //     display.clearDisplay();
  //     i = 0;
  //     y = 0;
  //   }
  // }

  // Jika ESP sudah terkoneksi ke WiFi, maka lampu menyala
  // digitalWrite(espLed, HIGH);
  // Serial.println("");

  // Menampilkan status di OLED
  // delay(100);
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE);
  // display.setCursor(0, 0);
  // display.println("connected to wifi network!");
  // display.display();
  // display.setCursor(0, 20);
  // display.println(WiFi.localIP());
  // display.display();

  // Menampilkan IP di serial monitor
  // Serial.println(WiFi.localIP());

  // End point untuk get method
  server.on("/dapur", HTTP_GET, getDapurLed);
  server.on("/tamu", HTTP_GET, getTamuLed);
  server.on("/makan", HTTP_GET, getMakanLed);
  server.on("/toilet", HTTP_GET, getToiletLed);

  // End point untuk post method
  server.on("/dapur", HTTP_POST, setDapurLed);
  server.on("/tamu", HTTP_POST, setTamuLed);
  server.on("/makan", HTTP_POST, setMakanLed);
  server.on("/toilet", HTTP_POST, setToiletLed);

  if(!SPIFFS.begin(true)) {
    Serial.println("Gagal mount SPIFFS");
    return;
  }

  // Menyajikan file statis
  server.onNotFound([]() {
    String path = server.uri();
    if (SPIFFS.exists(path)) {
      File file = SPIFFS.open(path, "r");

      // Tentukan content type
      String contentType = "text/plain";
      if (path.endsWith(".html")) contentType = "text/html";
      else if (path.endsWith(".css")) contentType = "text/css";
      else if (path.endsWith(".js")) contentType = "application/javascript";
      else if (path.endsWith(".png")) contentType = "image/png";
      else if (path.endsWith(".jpg")) contentType = "image/jpeg";
      else if (path.endsWith(".ico")) contentType = "image/x-icon";

      server.streamFile(file, contentType);
      file.close();
    } else {
      server.send(404, "text/plain", "File Not Found");
    }
  });


  server.begin();

  // Memulai server
  Serial.println("Server started...");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

void setDapurLed() {
  dapurLedState = !dapurLedState;
  digitalWrite(ledPin1, dapurLedState ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", dapurLedState ? "ON" : "OFF");
  if(dapurLedState) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Dapur led on");
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Dapur led off");
    display.display();
  }
}

void setTamuLed() {
  tamuLedState = !tamuLedState;
  digitalWrite(ledPin2, tamuLedState ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", tamuLedState ? "ON" : "OFF");
  if(tamuLedState) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Ruang tamu led on");
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Ruang tamu led off");
    display.display();
  }
}

void setMakanLed() {
  makanLedState = !makanLedState;
  digitalWrite(ledPin3, makanLedState ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", makanLedState ? "ON" : "OFF");
  if(makanLedState) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Ruang makan led on");
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Ruang makan led off");
    display.display();
  }
}

void setToiletLed() {
  toiletLedState = !toiletLedState;
  digitalWrite(ledPin4, toiletLedState ? HIGH : LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", toiletLedState ? "ON" : "OFF");
  if(toiletLedState) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Toilet led on");
    display.display();
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Toiled led off");
    display.display();
  }
}

void getDapurLed() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", dapurLedState ? "ON" : "OFF");
}

void getTamuLed() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", tamuLedState ? "ON" : "OFF");
}

void getMakanLed() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", makanLedState ? "ON" : "OFF");
}

void getToiletLed() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", toiletLedState ? "ON" : "OFF");
}
