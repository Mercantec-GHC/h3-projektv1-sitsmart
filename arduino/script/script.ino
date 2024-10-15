#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "Ultrasonic.h"

MKRIoTCarrier carrier;

Ultrasonic ultrasonic(A6);

void setup() {
  // put your setup code here, to run once:
  carrier.noCase();
  carrier.begin();
  Serial.begin(9600);

  carrier.display.fillScreen(0xFFFF);
}

float x, y, z;
int lastX, lastY, lastZ;
float lastHumidity, lastTemp;

void loop() {
  //////////////////////////////
  /// TEMPERATURE & HUMIDITY ///
  //////////////////////////////
  float humidity = carrier.Env.readHumidity();
  float temp = carrier.Env.readTemperature();
  // Remove decimals?

  // Print value if it has changed
  if (humidity != lastHumidity) {
    lastHumidity = humidity;
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  // Print value if it has changed
  if (temp != lastTemp) {
    lastTemp = temp;
    Serial.print("Temp: ");
    Serial.println(temp);
  }

  //////////////////////////////
  ///        DISTANCE        ///
  //////////////////////////////
  long distanceInCm = ultrasonic.MeasureInCentimeters();
  
  // check if distance is above 2 cm and below 100
  if (distanceInCm > 2 && distanceInCm < 100) {
    Serial.print(distanceInCm);
    Serial.println(" cm");
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
