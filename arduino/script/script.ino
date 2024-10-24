#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "Ultrasonic.h"
#include "Images.h"
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <Arduino.h>
#include <bsec.h>

MKRIoTCarrier carrier;
WiFiSSLClient wifi;

Ultrasonic ultrasonic(A6);

// SEPERATE INTO OWN CLASS
float x, y, z;
int lastX, lastY, lastZ, lastLength;
float lastHumidity, lastTemp;

// WIFI Connection variables
char ssid[] = "MAGS-OLC";
char pass[] = "Merc1234!";
int status = WL_IDLE_STATUS;

// API Variables
String apiUrl = "ergo.mercantec.tech";
HttpClient* httpClient;

void setup() {
  Serial.begin(9600);
  carrier.noCase();
  carrier.begin();

  // Draw logo on display
  carrier.display.fillScreen(0xFFFF);
  drawLogo(0x021D30);

  // Check for WIFI
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");

    // don't continue
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

  Serial.print("You're connected to ");
  Serial.println(ssid);

  httpClient = new HttpClient(wifi, apiUrl);
}

void loop() {
  //////////////////////////////
  /// TEMPERATURE & HUMIDITY ///
  //////////////////////////////
  float humidity = carrier.Env.readHumidity();
  float temp = carrier.Env.readTemperature();
  // Remove decimals?

  // Print value if it has changed
  if (humidity != lastHumidity || temp != lastTemp) {
    lastHumidity = humidity;
    Serial.print("Humidity: ");
    Serial.println(humidity);
    lastTemp = temp;
    Serial.print("Temp: ");
    Serial.println(temp);

    sendData(temp*100, humidity*100);
  }

  //////////////////////////////
  ///        DISTANCE        ///
  //////////////////////////////
  long distanceInCm = ultrasonic.MeasureInCentimeters();
  
  // check if distance is above 2 cm and below 100
  if (distanceInCm > 2 && distanceInCm < 100 && distanceInCm != lastLength) {
    Serial.print(distanceInCm);
    Serial.println(" cm");

    lastLength = distanceInCm;
  }

  //////////////////////////////
  ///        MOVEMENT        ///
  //////////////////////////////
  // false is no movement, true is movement
  if (carrier.IMUmodule.accelerationAvailable())
  {
    // Get values, *100 to round up and remove decimals to make it to an int
    carrier.IMUmodule.readAcceleration(x, y, z);
    int valueX = x*100;
    int valueY = y*100;
    int valueZ = z*100;

    // Check if difference in numeric value is more than 2  
    if (abs(valueX-lastX) > 2 || abs(valueY-lastY) > 2 || abs(valueZ-lastZ) > 2) {
      Serial.print("x: ");
      Serial.print(valueX);

      Serial.print(" - y: ");
      Serial.print(valueY);

      Serial.print(" - z: ");
      Serial.println(valueZ);

      // SEND VALUE
      
      // Save values for next comparison
      lastX = valueX;
      lastY = valueY;
      lastZ = valueZ;
    }
  }

  delay(100);
}

// Draw logo
void drawLogo(uint16_t color) {
  carrier.display.drawBitmap(44, 60, ErgoLogo, 152, 72, color);
  //carrier.display.drawBitmap(44, 60, epd_bitmap_nowifi, 152, 72, color);
  carrier.display.drawBitmap(48, 145, ErgoText, 144, 23, color);
}

void sendData(int temp, int humid) {
  String postData = "{\"sitSmartDeviceId\":\"8d414a937e634a16945e5d17adc5e04a\",";
  postData += "\"temp\":" + String(temp) + ",";
  postData += "\"humidity\":" + String(humid) + "}";
  Serial.println(postData);

  httpClient->beginRequest();
  httpClient->post("/api/TempHumidities");
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  httpClient->print(postData);
  httpClient->endRequest();

  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}



