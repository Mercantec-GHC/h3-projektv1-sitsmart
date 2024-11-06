#ifndef SITSMART_H
#define SITSMART_H

#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Ultrasonic.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <bsec.h>
#include "Images.h"

class SitSmart {
  private:
    MKRIoTCarrier carrier;
    HttpClient* httpClient;
    WiFiSSLClient wifi;
    WiFiClient client;


    float temp;
    float humidity;
    float x, y, z;
    float lastHumidity, lastTemp;
    
    int lastX, lastY, lastZ, lastLength;
    int lastMillis;
    
    const char* fileName = "savedval.txt";
    String deviceId = "8d414a937e634a16945e5d17adc5e04a";
    
    String postData[10];


    const char* apiUrl = /*"sit-api.mercantec.tech";*/"sitsmart-nk0s.onrender.com";
    String ssidAP = "SitSmartHotspot";

    int status = WL_IDLE_STATUS;


  public:
    void begin();
    void drawLogo(uint16_t color);
    void handleTempHumid();
    void handleDistance();
    void handleMovement();
    void handleData();
    String sendData(String body, String action);
    void addRequestToBatch(String request);
    int getIndexOfInStringArray(String arr[10], String wantedValue);
    void sendAllRequests();
    void writeToSD(String input, bool clearSDFile);
    String readFromSD();
    void updateStatus();
    void setupWebpage();
    void beginNetwork();
    void connectToWiFi(String ssid, String password);
    bool isConnected();
};

#endif // SITSMART_H
