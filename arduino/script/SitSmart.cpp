#include "SitSmart.h"

void SitSmart::begin() {
  Serial.begin(9600); 

  carrier.noCase();
  carrier.begin();
  drawLogo(0x021D30);
    
  /*writeToSD("abab", true);
  readFromSD();*/

  //beginNetwork();
  Serial.println("connect");
  connectToWiFi("", "");
  Serial.println("send data");
  sendData();
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
    int valueX = x * 100;
    int valueY = y * 100;
    int valueZ = z * 100;

    // Check if difference in numeric value is more than 2  
    if (abs(valueX-lastX) > 25 || abs(valueY-lastY) > 25 || abs(valueZ-lastZ) > 25) {
      Serial.println("new");
      Serial.println(" x" + String(valueX));
      Serial.println(" y" + String(valueY));
      Serial.println(" z" + String(valueZ));

      lastX = valueX;
      lastY = valueY;
      lastZ = valueZ;
    }
  }
}

void SitSmart::readData() {
  // Check if device has wifi connection
  /*if (!isConnected()) { 
    // If no connection use accesspoint to get wifi
    updateStatus();
    setupWebpage();
  } else {
    // If connection handle all sensors
    handleTempHumid();
    handleDistance();
    //handleMovement();
    
    delay(100);
  }*/
  carrier.Buttons.update();
  if (carrier.Buttons.onTouchDown(TOUCH2))
  {
    Serial.println("abac");
    //sendData("{ \"temp\": " + String(22) + ", \"humidity\": " + String(40) + ", \"sitSmartDeviceId\": \"" + String(deviceId) + "\" }", "/api/TempHumidities");
  }
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
  //for (int i=0; i < 10; i++) {
    Serial.println(postData[1]);
    //sendData(postData[1], "/api/TempHumidities");
    postData[1] = "";
  //}
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
    myFile.println(input);
    myFile.close();
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
  Serial.println(fileName);
  
  File myFile = SD.open(fileName);
  if (myFile) {
    String content = "";
    while (myFile.available()) {
      content += (char)myFile.read();
    }
    Serial.println(content);
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

// NETWORKING
WiFiServer server(80);

void SitSmart::beginNetwork() {
  Serial.println("network begin");
  if (WiFi.status() == WL_NO_MODULE) {
    while (true);
  }

  // 10.10.10.1
  WiFi.config(IPAddress(10, 10, 10, 1));

  status = WiFi.beginAP(ssidAP.c_str());

  if (status != WL_AP_LISTENING) {
    while (true);
  }

  server.begin();
}

void SitSmart::updateStatus() {
  if (status != WiFi.status()) {
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP");
    } else {
      Serial.println("Device disconnected from AP");
    }
  }

  client = server.available();
}

void SitSmart::setupWebpage() {
  if (client) {
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html><body>");
            client.print("<form method='get' action='/connect'>");
            client.print("<label for='ssid'>SSID:</label>");
            client.print("<input type='text' id='ssid' name='ssid'><br><br>");
            client.print("<label for='pass'>Password:</label>");
            client.print("<input type='text' id='pass' name='pass'><br><br>");
            client.print("<input type='submit' value='Connect'>");
            client.print("</form></body></html>");
            client.println();
            break;

          } else if (currentLine.startsWith("GET /connect")) {
              Serial.println(currentLine);
              // GET /connect?ssid=asdasg&21wag&pass=Merc1234%21 HTTP/1.1
              
              String givenSSID = currentLine.substring(currentLine.indexOf("=")+1, currentLine.indexOf("&pass"));
              String givenPass = currentLine.substring(currentLine.lastIndexOf("=")+1, currentLine.lastIndexOf(" HTTP"));
              
              // TODO: MAKE BETTER HEX TO ASCII TRANSLATOR
              givenSSID.replace("%21", "!");
              givenPass.replace("%21", "!");
              
              Serial.println("ababababab");
              Serial.println("ssid: " + givenSSID);
              Serial.println("pass: " + givenPass);
              
              connectToWiFi(givenSSID, givenPass);
              
              currentLine = "";
          }
          currentLine = "";
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // close the connection:
    client.stop();

  }
}

void SitSmart::connectToWiFi(String ssid, String password) {
  ssid = "MAGS-OLC";
  password = "Merc1234!";

  Serial.println("Attempting connect with: " + ssid + " and: " + password);
  while (!isConnected()) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  if (isConnected()) {
    Serial.println("connected");
    //String response = sendData("{}", "/api/SitSmartDevices");
    String response = "{\"id\": \"ba4dde5fd6f444868b8fe98f7bb06df3\", \"tempHumiditys\": null, \"movements\": null, \"distanceObjects\": null, \"users\": null }";
    response = response.substring(response.indexOf("\"id\":")+5);
    Serial.println(response);
    response = response.substring(1);
    Serial.println(response);
/*
    String id = "abc";
    File myFile = SD.open(fileName, FILE_WRITE);
    
    if (myFile) {
      myFile.println("ssid=abc;password=abc;device=\"" + id + "\";");
      myFile.close();
      Serial.println("Wrote to sd card");
    } else {
      Serial.println("Kunne ikke åbne fil til skrivning");
    }*/
  } else {
    Serial.println("failed connection");
    // TODO: REMOVE FROM FILE ON SD
  }
}

/*void sendData(String body, String action) {

  httpClient->beginRequest();
  httpClient->post(action);
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("Content-Length", body.length());
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  httpClient->print(body);
  httpClient->endRequest();

  int statusCode = httpClient->responseStatusCode();
  //String response = httpClient->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.print("WiFi: ");
  Serial.println(isConnected());
}*/
// String body = "{ \"temp\": 22, \"humidity\": 40, \"sitSmartDeviceId\": \"8d414a937e634a16945e5d17adc5e04a\" }";
void SitSmart::sendData() {
  String postData = "{\"sitSmartDeviceId\": \"8d414a937e634a16945e5d17adc5e04a\",";
  postData += "\"temp\": 22,";
  postData += "\"humidity\": 40 }";
  Serial.println(postData);
  
  Serial.print("1");
  httpClient->beginRequest();
  Serial.print("2");
  httpClient->post("/api/TempHumidities");
  Serial.print("3");
  httpClient->sendHeader("Content-Type", "application/json");
  Serial.print("4");
  httpClient->sendHeader("Content-Length", postData.length());
  Serial.print("5");
  httpClient->sendHeader("accept", "text/plain");
  Serial.print("6");
  httpClient->beginBody();
  Serial.print("7");
  httpClient->print(postData);
  Serial.print("8");
  httpClient->endRequest();
  Serial.print("9");
  int statusCode = httpClient->responseStatusCode();
  Serial.println("10");
  String response = httpClient->responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

bool SitSmart::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
