#include "SitSmart.h"

SitSmart::SitSmart(const char* ssid, const char* password) 
  : ssid(ssid), password(password) {}

void SitSmart::begin() {
  carrier.noCase();
  carrier.begin();
  Serial.begin(9600); 
  connectToWiFi();
}

void SitSmart::connectToWiFi() {
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
  }

  Serial.print("You're connected to ");
  Serial.println(ssid);

  httpClient = new HttpClient(wifi, apiUrl, 443);
}

// Draw logo
void SitSmart::drawLogo(uint16_t color) {
  carrier.display.fillScreen(0xFFFF);
  carrier.display.drawBitmap(44, 60, ErgoLogo, 152, 72, color);
  //carrier.display.drawBitmap(44, 60, epd_bitmap_nowifi, 152, 72, color);
  carrier.display.drawBitmap(48, 145, ErgoText, 144, 23, color);
}
