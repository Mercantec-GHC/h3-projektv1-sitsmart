#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

class NetworkHelper {
  private:
    const char* apiUrl = "sit-api.mercantec.tech"//"sitsmart-nk0s.onrender.com";
    String ssidAP = "SitSmartHotspot";

    int status = WL_IDLE_STATUS;

    HttpClient* httpClient;
    WiFiSSLClient wifi;
    WiFiClient client;

  public:
    void updateStatus();
    void setupWebpage();
    void begin();
    void connectToWiFi(String ssid, String password);
    void sendData(/*String body, String action*/);
    bool isConnected();
    void getdata();
};

#endif // NETWORKHELPER_H
