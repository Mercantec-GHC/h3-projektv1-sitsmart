#include "SitSmart.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

SitSmart sitSmart;


void setup() {
  sitSmart.begin();
}

void loop() {
  sitSmart.handleData();
}

