//Libraries
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <TinyGPSPlus.h>




//Screen setup
#define TFT_CS 3
#define TFT_RST 2
#define TFT_DC 20
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

TinyGPSPlus gps;
#define GPS_SERIAL Serial1

int Bt1 = 6;
int Bt2 = 7;
int Bt3 = 20;
int Bt4 = 21;
int ScrMOS = 10;
bool scrn0N = 1;

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);

  GPS_SERIAL.begin(9600);

  pinMode(Bt1, INPUT);
  pinMode(Bt2, INPUT);
  pinMode(Bt4, INPUT);
  pinMode(ScrMOS, OUTPUT);
  digitalWrite(ScrMOS, scrn0N);
}

void loop() {

  if (digitalRead(Bt1)) {
    delay(100);
    if (scrn0N && digitalRead(Bt1)) {
      scrn0N = 0;
      tft.fillScreen(ST7735_BLACK);
    } else if (digitalRead(Bt1)) {
      scrn0N = 1;
    }
    delay(50);
  }
  digitalWrite(ScrMOS, scrn0N);

  if (scrn0N) {
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    while (GPS_SERIAL.available()) {
      char c = GPS_SERIAL.read();
      gps.encode(c);  // Feed data to TinyGPS++
    }
    tft.println("---------");
    tft.print("Time :");
    tft.print(gps.time.hour());
    tft.print(" : ");
    tft.print(gps.time.minute());
    tft.print(" : ");
    tft.println(gps.time.second());

    tft.print("Latitude: "); tft.println(gps.location.lat(), 12);
    tft.print("Longitude: "); tft.println(gps.location.lng(), 12);
    tft.print("sattlock: "); tft.println(gps.satellites.value(), 2);
  }
}
