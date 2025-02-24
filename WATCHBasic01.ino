//Libraries
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <TinyGPSPlus.h>
#include "MAX30100_PulseOximeter.h"
#include <Wire.h>
#include <Fonts/FreeMonoBold12pt7b.h>

//Screen setup
#define TFT_CS 3
#define TFT_RST 2
#define TFT_DC 12
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

TinyGPSPlus gps;
#define GPS_SERIAL Serial1

MAX30100 sensor;
PulseOximeter pox;

const int Bt1 = 6;
const int Bt2 = 7;
const int Bt3 = 20;
const int Bt4 = 21;

float gpsramlat[120];
float gpsramlng[120];
float gpsminy = 0;
float gpsminx = 0;
float gpsmaxx = 0;
float gpsmaxy = 0;
int gpsconstx = 0;
int gpsconsty = 0;

int hour = 0;
int minut = 0;
int second = 0;
unsigned long mtime;
int ScrMOS = 10;
bool scrn0N = 1;
int i = 0;
int ii = 0;

int dgWheel = 0;
int STAT2 = 23;
int STAT1 = 24;


void gpsSleep() {
  uint8_t standbyCmd[] = { 0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0xDC };
  Serial1.write(standbyCmd, sizeof(standbyCmd));
}


void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);

  GPS_SERIAL.begin(9600);
  pinMode(Bt1, INPUT);
  pinMode(Bt2, INPUT);
  pinMode(Bt3, INPUT);
  pinMode(Bt4, INPUT);
  pinMode(STAT1, INPUT);
  pinMode(STAT2, INPUT);
  pinMode(ScrMOS, OUTPUT);
  digitalWrite(ScrMOS, scrn0N);
}

void loop() {
  mtime = millis();
  while (digitalRead(Bt1)) {
    i = millis() - mtime;
    if (scrn0N && digitalRead(Bt1) && i >= 1000) {
      gpsSleep();
      scrn0N = 0;
      delay(50);
      tft.fillScreen(ST7735_BLACK);
    } else if (digitalRead(Bt1) && i >= 100) {
      scrn0N = 1;
      Serial1.write(0xFF);  // Wakey wakey eggs and bakey gps!
      delay(50);
    }
  }

  digitalWrite(ScrMOS, scrn0N);
  if (scrn0N) {
    tft.setFont(&FreeMonoBold12pt7b);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.fillRect(0, 0, 160, 50, ST7735_BLACK);
    tft.setCursor(10, 30);
    tft.print(hour);
    tft.print(":");
    tft.print(minut);
    tft.print(":");
    tft.println(second);
    tft.setFont();

    if (digitalRead(STAT1) && !digitalRead(STAT2)) {
      tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
      tft.println("Charge Complete     ");
      tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    }
    if (digitalRead(STAT2) && !digitalRead(STAT1)) {
      tft.setTextColor(ST7735_RED, ST7735_BLACK);
      tft.println("Charging            ");
      tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    }
    if (digitalRead(STAT1) && digitalRead(STAT2)) {
      tft.setTextColor(ST7735_BLUE, ST7735_BLACK);
      tft.println("Charger disconnected ");
      tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    }
    while (GPS_SERIAL.available()) {
      char c = GPS_SERIAL.read();
      gps.encode(c);  // Feed data to TinyGPS++
    }
    if (gps.date.isValid()) {
      hour = gps.time.hour() + 1;
      minut = gps.time.minute();
      second = gps.time.second();
    }
    if (digitalRead(Bt2) && dgWheel != 0) {
      dgWheel--;
      delay(50);
    }
    if (digitalRead(Bt4) && dgWheel != 3) {
      dgWheel++;
      delay(50);
    }


    if (digitalRead(Bt1) && dgWheel == 0) {  //the ultimate UI skullfucking
      delay(100);
      i = 0;

      while (!digitalRead(Bt1)) {
        delay(500);
        tft.fillScreen(ST7735_BLACK);
        tft.setCursor(0, 0);
        tft.println("GPS UTILITY BASIC");
        tft.println(gps.location.lat());
        tft.println(gps.location.lng());
        tft.print(gpsminx);
        tft.print(",");
        tft.print(gpsminy);
        tft.print(",");
        tft.print(gpsmaxx);
        tft.print(",");
        tft.println(gpsmaxy);
        gpsramlat[i] = gps.location.lat();
        gpsramlng[i] = gps.location.lng();

        for (ii = 0; ii != 120; ii++) {
          if (gpsramlat[i] < gpsminx) {  //this shit will only resize in one direction
            gpsminx = gpsramlat[i];
          }
          if (gpsramlat[i] > gpsmaxx) {
            gpsmaxx = gpsramlat[i];
          }
          if (gpsramlng[i] < gpsminy) {
            gpsminy = gpsramlng[i];
          }
          if (gpsramlng[i] > gpsmaxy) {
            gpsmaxy = gpsramlng[i];
          }
        }
        ii = 0;
        while (i != ii) {
          tft.drawLine((gpsramlat[ii] - gpsminx) * (128 / (gpsmaxx - gpsminx)), (gpsramlng[ii] - gpsminy) * (128 / (gpsmaxy - gpsminy)), (gpsramlat[ii + 1] - gpsminx) * (128 / (gpsmaxx - gpsminx)), (gpsramlng[ii + 1] - gpsminy) * (128 / (gpsmaxy - gpsminy)), ST7735_ORANGE);
          ii++;
        }

        tft.drawRect(0, 32, 128, 128, ST7735_WHITE);
      }
      tft.fillScreen(ST7735_BLACK);
    }
  }
}
