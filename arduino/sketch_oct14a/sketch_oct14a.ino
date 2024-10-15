#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
//#include "arduino_secrets.h"
#include <SD.h>
#include <Arduino.h>
#include <bsec.h>

MKRIoTCarrier carrier;
float x, y, z;

char ssid[] = "Jacob - iPhone (2)"; // You wifi/mobile data nam
char pass[] = "kodener2";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

//const char* serverAddress = "172.20.10.4";
WiFiSSLClient wifi;

// Specify IP address or hostname
String hostName = "www.google.com";
int pingResult;
void setup() {
    carrier.begin();
  carrier.display.fillScreen(ST77XX_BLACK); // Start med en ren skærm
  carrier.leds.setBrightness(255); // Sæt LED'ernes maksimale lysstyrke
  // Initialiser kommunikation
  Serial.begin(9600);
 
while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    carrier.display.setCursor(60, 110);
    carrier.display.print("Connecting to wifi");

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  carrier.display.setCursor(30, 125);
  carrier.display.print("Connected to wifi!");
  printCurrentNet();
  //printWiFiData();
carrier.noCase();
  carrier.begin();
}


void loop(){
  if (carrier.IMUmodule.accelerationAvailable())
    {
      carrier.IMUmodule.readAcceleration(x, y, z);
      delay(100);
      Serial.println(x);
    }
}
void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
}