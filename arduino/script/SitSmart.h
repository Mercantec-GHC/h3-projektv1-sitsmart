#ifndef SITSMART_H
#define SITSMART_H

#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Ultrasonic.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <Arduino.h>
#include <bsec.h>
#include "Images.h"

class SitSmart {
  private:
    MKRIoTCarrier carrier;
    float temp;
    float humidity;
    const char* ssid;
    const char* password;
    HttpClient* httpClient;
    WiFiSSLClient wifi;
    String apiUrl = "sit-api.mercantec.tech";
    int status = WL_IDLE_STATUS; 
    String deviceId = "8d414a937e634a16945e5d17adc5e04a";
    float x, y, z;
    int lastX, lastY, lastZ, lastLength;
    int lastMillis;
    float lastHumidity, lastTemp;
        
    // API Variables
    String postData[10];

  public:
    SitSmart(const char* ssid, const char* password);
    void begin();
    void connectToWiFi();
    void drawLogo(uint16_t color);
    void handleTempHumid();
    void handleDistance();
    void handleMovement();
    void readData();
    void sendData(String body);
    void addRequestToBatch(String request);
    int getIndexOfInStringArray(String arr[10], String wantedValue);
    void sendAllRequests();
    void sendAsyncRequest(String body);
};

#endif // SITSMART_H
