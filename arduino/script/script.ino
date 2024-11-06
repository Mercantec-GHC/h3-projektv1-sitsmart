#include "SitSmart.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

SitSmart sitSmart;


void setup() {
  sitSmart.begin();
  /*

  Serial.print("Status code: ");
  Serial.println(statusCode);

  String input = "{\"sitSmartDeviceId\": \"8d414a937e634a16945e5d17adc5e04a\",";
  input += "\"temp\": 22,";
  input += "\"humidity\": 40 }";
  Serial.println(input);
  const size_t contentLength = input.length();

  Serial.print("1");
  httpClient.beginRequest();
  Serial.print("2");
  httpClient.post("/api/TempHumidities");
  Serial.print("3");
  httpClient.sendHeader("Content-Type", "application/json");
  Serial.print("4");
  httpClient.sendHeader("Content-Length", contentLength);
  Serial.print("5");
  httpClient.sendHeader("accept", "text/plain");
  Serial.print("6");
  httpClient.beginBody();

  constexpr size_t chunkSize = 64;

  Serial.print("7");
  for (size_t i = 0; i < contentLength; i = i + chunkSize) {
    if (contentLength - i < chunkSize) {
      String chunk = input.substring(i);
      httpClient.print(chunk);
    } else {
      String chunk = input.substring(i, chunkSize + i);
      httpClient.print(chunk);
    }
  }

  httpClient.endRequest();
  Serial.print("8");
  statusCode = httpClient.responseStatusCode();
  //String response = httpClient->responseBody();
  Serial.print("9");

  Serial.print("Status code: ");
  Serial.println(statusCode);
*/
  Serial.println("acab");
}

void loop() {
  //sitSmart.readData();
}

