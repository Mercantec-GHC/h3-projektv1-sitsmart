

#ifdef SITSMART_H
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

class SitSmart {
  private:
    MKRIoTCarrier carrier;
    float temp;
    float humidity;
    const char* ssid;
    const char* password;
    HttpClient* httpClient;
    WiFiSSLClient wifi;
    String apiUrl = "ergo.mercantec.tech";
    int status = WL_IDLE_STATUS; 
    Ultrasonic ultrasonic(A6);

  public:
    SitSmart(const char* ssid, const char* password);
    void drawLogo();
};

#endif SITSMART_H
