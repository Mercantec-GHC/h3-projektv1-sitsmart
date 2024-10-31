#include "SitSmart.h"

SitSmart::SitSmart(const char* ssid, const char* password) 
  : ssid(ssid), password(password) {}

void SitSmart::begin() {
  carrier.withCase();
  carrier.begin();
  drawLogo(0x021D30);
  Serial.begin(9600); 
  
  sdInitialized = true;
  
  connectToWiFi();

  writeToSD("abab", true);
  readFromSD();
}

void SitSmart::connectToWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, password);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("You're connected to ");
    Serial.println(ssid);
  }

  httpClient = new HttpClient(wifi, apiUrl, 443);
}

// Draw logo
void SitSmart::drawLogo(uint16_t color) {
  carrier.display.fillScreen(0xFFFF);
  carrier.display.drawBitmap(44, 60, ErgoLogo, 152, 72, color);
  //carrier.display.drawBitmap(44, 60, epd_bitmap_nowifi, 152, 72, color);
  carrier.display.drawBitmap(48, 145, ErgoText, 144, 23, color);
}

void SitSmart::handleTempHumid() {
  //////////////////////////////
  /// TEMPERATURE & HUMIDITY ///
  //////////////////////////////
  float humidity = carrier.Env.readHumidity();
  float temp = carrier.Env.readTemperature();
  // Remove decimals?

  // Print value if it has changed
  if (humidity != lastHumidity || temp != lastTemp) {
    lastHumidity = humidity;
    Serial.print("Humidity: ");
    Serial.println(humidity);
    lastTemp = temp;
    Serial.print("Temp: ");
    Serial.println(temp);


    int intTemp = temp*100;
    int intHumid = humidity*100;


    String postBody = "{ \"temp\": " + String(intTemp) + ", \"humidity\": " + String(intHumid) + ", \"sitSmartDeviceId\": \"" + String(deviceId) + "\" }";
    addRequestToBatch(postBody);
  }
}

void SitSmart::handleDistance() {
  //////////////////////////////
  ///        DISTANCE        ///
  //////////////////////////////
  Ultrasonic ultrasonic(A6);
  long distanceInCm = ultrasonic.MeasureInCentimeters();
  
  // 
  int time = millis();
  if (distanceInCm < 100 && time - lastMillis > 10000) {
    Serial.print(distanceInCm);
    Serial.println(" cm");

    String body = "{ \"distance\": " + String(distanceInCm) + ", \"sitSmartDeviceId\": \"" + String(deviceId) + "\" }";
    //addRequestToBatch(body);

    lastMillis = time;
    lastLength = distanceInCm;
  }
}

void SitSmart::handleMovement() {
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
}

void SitSmart::readData() {
  handleTempHumid();

  //handleDistance(); // Udkommenterer kald til handleDistance

  handleMovement();

  delay(100);
}

void SitSmart::sendData(String body) {
  httpClient->beginRequest();
  httpClient->post("/api/TempHumidities");
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("Content-Length", body.length());
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  Serial.println(WiFi.status() == WL_CONNECTED);
  httpClient->print(body);
  httpClient->endRequest();

  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.print("WiFi: ");
  Serial.println(WiFi.status() == WL_CONNECTED);
}

void SitSmart::addRequestToBatch(String request) {
  int firstEmptySpace = getIndexOfInStringArray(postData, "");

  if (firstEmptySpace != -1) {
    postData[firstEmptySpace] = request;
  }

  if (firstEmptySpace == 10 - 1) {
    Serial.println("Saved requests full! Sending all requests");
    sendAllRequests();
  }
}

int SitSmart::getIndexOfInStringArray(String arr[10], String wantedValue) {
  int wantedpos = -1;

  for (int i=0; i < 10; i++) {
    if (arr[i] == wantedValue) {
      wantedpos = i;
      break;
    }
  }
  return wantedpos;
}

void SitSmart::sendAllRequests() {
  for (int i=0; i < 10; i++) {
    Serial.println(postData[i]);
    //sendData(postData[i]);
    postData[i] = "";
  }
  Serial.println("Sent all requests");
}

void SitSmart::writeToSD(String input, bool clearSDFile) {
  Serial.println("Forsøger at skrive til SD-kort...");
  Serial.print("Filnavn: ");
  Serial.println(fileName);
  
  if (clearSDFile) {
    if (SD.exists(fileName)) {
      Serial.println("Fil findes - forsøger at slette");
      if (SD.remove(fileName)) {
        Serial.println("Fil slettet succesfuldt");
      } else {
        Serial.println("Kunne ikke slette fil");
      }
    }
  }
  
  File myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    Serial.println("Fil åbnet succesfuldt");
    myFile.println(input);
    myFile.close();
    Serial.println("Data skrevet til SD-kort");
  } else {
    Serial.println("Kunne ikke åbne fil til skrivning");
    Serial.print("SD-kort status: ");
    if (!SD.begin(SD_CS)) {
      Serial.println("SD-kort ikke tilgængeligt");
    } else {
      Serial.println("SD-kort er tilgængeligt");
    }
  }
}

void SitSmart::readFromSD() {
  Serial.println("Forsøger at læse fra SD-kort...");
  Serial.print("Filnavn: ");
  Serial.println(fileName);
  
  File myFile = SD.open(fileName);
  if (myFile) {
    Serial.println("Fil åbnet succesfuldt");
    String content = "";
    while (myFile.available()) {
      content += (char)myFile.read();
    }
    Serial.println("Læst fra SD-kort: " + content);
    myFile.close();
  } else {
    Serial.println("Kunne ikke åbne fil til læsning");
    if (SD.exists(fileName)) {
      Serial.println("Filen findes på SD-kortet");
    } else {
      Serial.println("Filen findes ikke på SD-kortet");
    }
  }
}
