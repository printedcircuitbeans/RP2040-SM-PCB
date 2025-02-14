//Libraries
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <TinyGPSPlus.h>
#include "MAX30100_PulseOximeter.h"
#include <Wire.h>


//Screen setup
#define TFT_CS 3
#define TFT_RST 2
#define TFT_DC 20
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

TinyGPSPlus gps;
#define GPS_SERIAL Serial1

MAX30100 sensor;
PulseOximeter pox;

int Bt1 = 6;
int Bt2 = 7;
int Bt3 = 20;
int Bt4 = 21;
unsigned long mtime;
int ScrMOS = 10;
bool scrn0N = 1;
int i = 0;
int dgWheel = 0;

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);

  GPS_SERIAL.begin(9600);

  pox.begin();


  pinMode(Bt1, INPUT);
  pinMode(Bt2, INPUT);
  pinMode(Bt4, INPUT);
  pinMode(ScrMOS, OUTPUT);
  digitalWrite(ScrMOS, scrn0N);

  sensor.setMode(MAX30100_MODE_SPO2_HR);
  sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_50MA);
}
void drawBioUI(int16_t x, int16_t y, int16_t hb, int16_t oxy) {
  tft.fillCircle(x + 30, y + 30, 20, ST7735_RED);
  tft.fillCircle(x + 30, y + 30, 10, ST7735_BLACK);
  tft.fillCircle(x + 80, y + 30, 20, ST7735_CYAN);
  tft.fillCircle(x + 80, y + 30, 10, ST7735_BLACK);

  tft.setTextColor(ST7735_WHITE, 0);
  tft.setCursor(x + 26, y + 27);
  tft.print(hb);
  tft.setCursor(x + 76, y + 27);
  tft.print(oxy);
}
void loop() {


  mtime = millis();
  while (digitalRead(Bt1)) {
    i = millis() - mtime;
    if (scrn0N && digitalRead(Bt1) && i >= 1000) {
      scrn0N = 0;
      delay(50);
      tft.fillScreen(ST7735_BLACK);
    } else if (digitalRead(Bt1) && i >= 100) {
      scrn0N = 1;
      delay(50);
    }
  }

  digitalWrite(ScrMOS, scrn0N);
  if (scrn0N) {
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);






    while (GPS_SERIAL.available()) {
      char c = GPS_SERIAL.read();
      gps.encode(c);  // Feed data to TinyGPS++
    }

    tft.println("----------------");
    tft.print("Time :");
    tft.print(gps.time.hour());
    tft.print(" : ");
    tft.print(gps.time.minute());
    tft.print(" : ");
    tft.println(gps.time.second());

    /*
    tft.print("Latitude: ");
    tft.println(gps.location.lat(), 12);
    tft.print("Longitude: ");
    tft.println(gps.location.lng(), 12);
    tft.print("sattlock: ");
    tft.println(gps.satellites.value(), 2);
*/

    tft.setCursor(0, 50);
    if (digitalRead(Bt2) && dgWheel != 0) {
      dgWheel--;
      delay(50);
    }
    if (digitalRead(Bt4) && dgWheel != 3) {
      dgWheel++;
      delay(50);
    }
    tft.println(" BIOMETRIC SENSOR   ");
    tft.println(" BME 680 GAS        ");
    tft.println(" SD CARD FS BASIC   ");
    tft.println(" GPS DATA DSP       ");
    tft.println(dgWheel);
    tft.drawRect(0, (dgWheel * 8) + 49, 120, 9, ST7735_WHITE);
    mtime = millis();
  }
  if (digitalRead(Bt1)) {
    if (dgWheel == 0) {
      delay(100);
      tft.fillScreen(ST7735_BLACK);
      sensor.resume();
      while (digitalRead(Bt1) != 1) {

        pox.update();
        delay(20);
        drawBioUI(10, 50, pox.getHeartRate(), pox.getSpO2());
      }
      sensor.shutdown();
      tft.fillScreen(ST7735_BLACK);
    }
  }
}
