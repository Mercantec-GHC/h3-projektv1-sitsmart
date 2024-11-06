#include "SitSmart.h"
WiFiServer server(80);

void SitSmart::begin() {
  Serial.begin(9600); 

  carrier.noCase();
  carrier.begin();
  drawLogo(0x021D30);
    
  beginNetwork();
}

// Draw logo
void SitSmart::drawLogo(uint16_t color) {
  carrier.display.fillScreen(0xFFFF);
  carrier.display.drawBitmap(44, 60, ErgoLogo, 152, 72, color);
  carrier.display.drawBitmap(48, 145, ErgoText, 144, 23, color);
}

// Hande temp and humidity sensor
void SitSmart::handleTempHumid() {
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

// Handle distance sensor
void SitSmart::handleDistance() {
  Ultrasonic ultrasonic(A6);
  long distanceInCm = ultrasonic.MeasureInCentimeters();
  
  // Get time since last request
  int time = millis();

  // If the change in distance is big enough and it has been more than 10 seconds since last request
  if (distanceInCm < 100 && time - lastMillis > 10000) {
    // Create request body and add to batch
    String body = "{ \"distance\": " + String(distanceInCm) + ", \"sitSmartDeviceId\": \"" + String(deviceId) + "\" }";
    addRequestToBatch(body);

    lastMillis = time;
    lastLength = distanceInCm;
  }
}

// Handle movement sensor
void SitSmart::handleMovement() {
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

// Handles reading all data, connection to wifi and sending data to wifi
void SitSmart::handleData() {
  // Check if device has wifi connection
  if (!isConnected()) { 
    // Check if a wifi has been saved
    if (SD.exists(fileName)) {
      // Get saved data
      String sdContent = readFromSD();

      // Get SSID from saved data
      String savedSSID = sdContent.substring(sdContent.indexOf("ssid") + 7);
      savedSSID = savedSSID.substring(0, savedSSID.indexOf("\""));
      
      // Get password from saved data
      String savedPass = sdContent.substring(sdContent.lastIndexOf("=")+2);
      savedPass = savedPass.substring(0, savedPass.indexOf("\""));

      // Attempt to connect to wifi
      connectToWiFi(savedSSID, savedPass);
    }

    // If connecting with saved data failed or no saved data, open own hotspot
    if (!isConnected()) {
      // If no connection use accesspoint to get wifi
      updateStatus();
      setupWebpage();
    }
  } else {
    // If connection handle all sensors
    handleTempHumid();
    handleDistance();
    //handleMovement();
    
    delay(100);
  }
}

// adds request to batch
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

// Gets index of item within array
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

// sends all requests from saved batch
void SitSmart::sendAllRequests() {
  for (int i=0; i < 10; i++) {
    Serial.println(postData[i]);
    sendData(postData[i], "/api/TempHumidities");
    postData[i] = "";
  }
  Serial.println("Sent all requests");
}


// Writes to sd card, can also clear and rewrite entire file
void SitSmart::writeToSD(String input, bool clearSDFile) {  
  if (clearSDFile) {
    if (SD.exists(fileName)) {
      if (SD.remove(fileName)) {
        Serial.println("File successfully deleted");
      } else {
        Serial.println("Error when deleting file");
      }
    }
  }
  
  File SDFile = SD.open(fileName, FILE_WRITE);
  if (SD.exists(fileName)) {
    SDFile.println(input);
    SDFile.close();
  } else {
    Serial.print("SD status: ");
    if (!SD.begin(SD_CS)) {
      Serial.println("Not avaliable");
    } else {
      Serial.println("Avaliable");
    }
  }
}

// Reads data from sd card and returns
String SitSmart::readFromSD() {
  String content = "";
  
  File SDFile = SD.open(fileName);
  if (SD.exists(fileName)) {
    while (SDFile.available()) {
      content += (char)SDFile.read();
    }
    SDFile.close();
  } else {
    if (SD.exists(fileName)) {
      Serial.println("Filen findes på SD-kortet");
    } else {
      Serial.println("Filen findes ikke på SD-kortet");
    }
  }

  return content;
}

// NETWORKING

// Initializes network and variables
void SitSmart::beginNetwork() {
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

// Updates status, used for making hotspot webpage
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

// Makes the page where user can give ssid and password to local network
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
              // Example: GET /connect?ssid=asdasg&21wag&pass=Merc1234%21 HTTP/1.1
              
              String givenSSID = currentLine.substring(currentLine.indexOf("=")+1, currentLine.indexOf("&pass"));
              String givenPass = currentLine.substring(currentLine.lastIndexOf("=")+1, currentLine.lastIndexOf(" HTTP"));
              
              // TODO: MAKE BETTER HEX TO ASCII TRANSLATOR
              givenSSID.replace("%21", "!");
              givenPass.replace("%21", "!");
              
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

// Attempt to connect to wifi with given ssid and password, saves to sd card if connection is made
void SitSmart::connectToWiFi(String ssid, String password) {

  Serial.println("Attempting connect with: " + ssid + " and: " + password);
  while (!isConnected()) {
    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  if (isConnected()) {
    Serial.println("Connected");
    String id = "";

    if (SD.exists(fileName)) {
      String sdContent = readFromSD();

      // Only get id
      id = sdContent.substring(sdContent.indexOf("\"id\"=") + 7);
      id = id.substring(0, id.indexOf("\"")-1);
    }
    
    if (!SD.exists(fileName) || id.length() < 25) {
      // Create new device in database and get ID
      String response = sendData("{}", "/api/SitSmartDevices");

      // EXAMPLE RESPONSE; NOT TO BE USED IN PRODUCTION
      response = "{\"id\": \"ba4dde5fd6f444868b8fe98f7bb06df3\", \"tempHumiditys\": null, \"movements\": null, \"distanceObjects\": null, \"users\": null }";

      // GET ID FROM RESPONSE
      response = response.substring(response.indexOf("\"id\":")+7);
      id = response.substring(0, response.indexOf("\""));
    }


    String contentToSD = "{ \"id\"=\"" + id + "\",\"ssid\"=\"" + ssid + "\",\"password\"=\"" + password + "\" }";
    Serial.println("Writing to SD: " + contentToSD);

    writeToSD(contentToSD, true);
  } else {
    Serial.println("failed connection");
  }
}

// Sends given request and returns response, definetly works ;)
String SitSmart::sendData(String body, String action) {
  httpClient->beginRequest();
  httpClient->post(action);
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("Content-Length", body.length());
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  httpClient->print(body);
  httpClient->endRequest();
  
  // Gets response and prints in console
  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  return response;

}

// Check if wifi is connected
bool SitSmart::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
