#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int x = 0;

void setup()
{
  Serial.begin(9600);

  delay(250);                       // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
}

void loop()
{
  x += 1;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, x % SCREEN_HEIGHT);
  display.printf("espeugeot32", x);
  display.display();
  delay(1);
}
