// SH1106 OLED I2C display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bluetooth Low Energy (LE) server
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;

#define SERVICE_UUID "F8A50714-F9B3-4222-A26B-7349CCB98229"
#define CHARACTERISTIC_UUID_TX "F8A50714-F9B3-4222-A26B-7349CCB98229"

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

void BLESetup()
{
  // Create the BLE device
  BLEDevice::init("ESPeugeot32");

  // Create the BLE server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE characteristic
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
                                                   BLECharacteristic::PROPERTY_NOTIFY);

  // BLE2902 needed to notify
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Setting up and clearing the display...");
  delay(250);                       // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  Serial.println("Cleared and set!");

  Serial.println("Setting up ESPeugeot32 BLE...");
  BLESetup();
  Serial.println("Waiting for a client connection to notify.");
}

void loop()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  if (deviceConnected)
  {
    txValue = random(-10, 20);

    char txString[8];
    dtostrf(txValue, 1, 2, txString);

    pCharacteristic->setValue(txString);

    pCharacteristic->notify();
    display.printf("Sent value %s.", String(txString));
    Serial.printf("Sent value %s.", String(txString));
  }
  else
  {
    display.printf("No device connected.");
    Serial.printf("No deveice connected.");
  }
  display.display();
  delay(500);
}
