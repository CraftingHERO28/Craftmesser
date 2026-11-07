#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// --- DISPLAY EINSTELLUNGEN ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- DHT22 / AM2302 EINSTELLUNGEN ---
#define DHTPIN 2          // Data-Pin des AM2302 an Arduino Pin D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- MH-Z19B (CO2) EINSTELLUNGEN ---
// Arduino RX (Pin D3) -> MH-Z19 TX
// Arduino TX (Pin D4) -> MH-Z19 RX
SoftwareSerial co2Serial(3, 4);

// Befehl zum Abfragen des CO2-Werts vom MH-Z19B
const byte cmdReadCO2[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

void setup() {
  // Seriellen Monitor für Debugging starten
  Serial.begin(9600);
  
  // Software-Serial für MH-Z19B starten (läuft auf 9600 Baud)
  co2Serial.begin(9600);
  
  // DHT22 starten
  dht.begin();

  // OLED Display initialisieren (I2C-Adresse meist 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Display nicht gefunden!"));
    for(;;); // Stoppt falls Display fehlt
  }

  // Begrüßung auf dem Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 25);
  display.println(F("Raummesser Start..."));
  display.display();
  delay(2000);
}

void loop() {
  // 1. DHT22 auslesen
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // 2. MH-Z19B CO2-Wert auslesen
  int co2 = readCO2();

  // 3. Ausgabe auf dem Seriellen Monitor (zum Überprüfen am PC)
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" C | ");
  Serial.print("Feuchte: "); Serial.print(hum); Serial.print(" % | ");
  Serial.print("CO2: "); Serial.print(co2); Serial.println(" ppm");

  // 4. Display aktualisieren
  display.clearDisplay();

  // Überschrift
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println(F("-- UMWELTMESSER --"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // CO2-Anzeige (groß hervorheben)
  display.setCursor(0, 16);
  display.print(F("CO2: "));
  display.setTextSize(2);
  if (co2 > 0) {
    display.print(co2);
    display.setTextSize(1);
    display.print(F(" ppm"));
  } else {
    display.print(F("Lade..."));
  }

  // Temperatur & Luftfeuchtigkeit
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print(F("Temp:   "));
  if (!isnan(temp)) {
    display.print(temp, 1);
    display.print(F(" C"));
  } else {
    display.print(F("Error"));
  }

  display.setCursor(0, 52);
  display.print(F("Feuchte: "));
  if (!isnan(hum)) {
    display.print(hum, 1);
    display.print(F(" %"));
  } else {
    display.print(F("Error"));
  }

  // Display-Inhalt zeichnen
  display.display();

  // 2 Sekunden warten bis zur nächsten Messung
  delay(2000);
}

// Funktion zum Auslesen des MH-Z19B über UART
int readCO2() {
  byte response[9];
  co2Serial.write(cmdReadCO2, 9);
  memset(response, 0, 9);
  
  int i = 0;
  while (co2Serial.available() && i < 9) {
    response[i] = co2Serial.read();
    i++;
  }

  // Prüfen ob die Antwort gültig ist
  if (response[0] == 0xFF && response[1] == 0x86) {
    int co2High = (int) response[2];
    int co2Low = (int) response[3];
    return (co2High * 256) + co2Low;
  }
  
  return -1; // Fehler beim Lesen
}