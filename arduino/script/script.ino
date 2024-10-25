#include "SitSmart.h"
#include "Images.h"

// SEPERATE INTO OWN CLASS
float x, y, z;
int lastX, lastY, lastZ, lastLength;
float lastHumidity, lastTemp;

// WIFI Connection variables
String deviceId = "8d414a937e634a16945e5d17adc5e04a";

// API Variables
const int postDataMaxLength = 10;
String postData[postDataMaxLength];

SitSmart sitSmart("GalaxyS22", "password!");

void setup() {
  sitSmart.begin();
}

void loop() {
  //////////////////////////////
  /// TEMPERATURE & HUMIDITY ///
  //////////////////////////////
  float humidity = sitSmart.carrier.Env.readHumidity();
  float temp = sitSmart.carrier.Env.readTemperature();
  // Remove decimals?

  // Print value if it has changed
  if (humidity != lastHumidity || temp != lastTemp) {
    lastHumidity = humidity;
    Serial.print("Humidity: ");
    Serial.println(humidity);
    lastTemp = temp;
    Serial.print("Temp: ");
    Serial.println(temp);


    String postBody = "{\"sitSmartDeviceId\":\"" + String(deviceId) +"\",\"temp\":" + String(temp*100) + ",\"humidity\":" + String(humidity*100) + " }";
    addRequestTothing(postBody);
    sendData(temp*100, humidity*100);
  }

  //////////////////////////////
  ///        DISTANCE        ///
  //////////////////////////////
  long distanceInCm = sitSmart.ultrasonic.MeasureInCentimeters();
  
  // check if distance is above 2 cm and below 100
  if (distanceInCm > 2 && distanceInCm < 100 && distanceInCm != lastLength) {
    /*Serial.print(distanceInCm);
    Serial.println(" cm");*/

    lastLength = distanceInCm;
  }

  //////////////////////////////
  ///        MOVEMENT        ///
  //////////////////////////////
  // false is no movement, true is movement
  if (sitSmart.carrier.IMUmodule.accelerationAvailable())
  {
    // Get values, *100 to round up and remove decimals to make it to an int
    sitSmart.carrier.IMUmodule.readAcceleration(x, y, z);
    int valueX = x*100;
    int valueY = y*100;
    int valueZ = z*100;

    // Check if difference in numeric value is more than 2  
    if (abs(valueX-lastX) > 2 || abs(valueY-lastY) > 2 || abs(valueZ-lastZ) > 2) {
      /*Serial.print("x: ");
      Serial.print(valueX);

      Serial.print(" - y: ");
      Serial.print(valueY);

      Serial.print(" - z: ");
      Serial.println(valueZ);*/

      // SEND VALUE
      
      // Save values for next comparison
      lastX = valueX;
      lastY = valueY;
      lastZ = valueZ;
    }
  }

  delay(100);
}

void sendData(int temp, int humid) {
  
  /*httpClient->beginRequest();
  httpClient->post("/api/TempHumidities");
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("Content-Length", sizeof(postData[0]));
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  httpClient->print(postData[0]);
  httpClient->endRequest();

  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);*/
}

void addRequestTothing(String request) {
  int firstEmptySpace = getIndexOfInStringArray(postData, "");

  if (firstEmptySpace != -1) {
    postData[firstEmptySpace] = request;
  }

  if (firstEmptySpace == postDataMaxLength - 1) {
    Serial.println("Saved requests full! Sending all requests");
    sendAllRequests();
  }

  Serial.println("Added request: " + postData[firstEmptySpace] + " at " + String(firstEmptySpace));
}


int getIndexOfInStringArray(String arr[postDataMaxLength], String wantedValue) {
  int wantedpos = -1;

  for (int i=0; i < postDataMaxLength; i++) {
    if (arr[i] == wantedValue) {
      wantedpos = i;
      break;
    }
  }
  return wantedpos;
}

void sendAllRequests() {
  Serial.println("begun clear");
  Serial.println(postData[0]);
  Serial.println(postData[9]);
  for (int i=0; i < postDataMaxLength; i++) {
    Serial.println(postData[i]);
    postData[i] = "";
    Serial.println(postData[i]);
  }
  Serial.println("cleared list");
}



