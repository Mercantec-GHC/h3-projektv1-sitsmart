#include "SitSmart.h"

SitSmart sitSmart;
NetworkHelper networkHelper;

void setup() {
  Serial.println("ab");
  sitSmart.begin();
  Serial.println("ba");
  
}

void loop() {
  sitSmart.readData();
}

