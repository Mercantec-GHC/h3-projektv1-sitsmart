#include "NetworkHelper.h"
WiFiServer server(80);

void NetworkHelper::begin() {
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

void NetworkHelper::updateStatus() {
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

void NetworkHelper::setupWebpage() {
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

void NetworkHelper::connectToWiFi(String ssid, String password) {
  ssid = "GalaxyS22";
  password = "password1";

  Serial.println("Attempting connect with: " + ssid + " and: " + password);
  while (!isConnected()) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  if (isConnected()) {
    Serial.println("connected");
    // TODO: SAVE TO SD CARD
    // httpClient = new HttpClient(wifi, apiUrl, 443);
  } else {
    Serial.println("failed connection");
    // TODO: REMOVE FROM FILE ON SD
  }
}

/*void NetworkHelper::sendData(String body, String action) {

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
void NetworkHelper::sendData() {
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

void NetworkHelper::getdata() {
  Serial.print("1");
  Serial.print("2");
  Serial.println(isConnected());
  httpClient->get("/api/TempHumidities");
  Serial.print("3");
  Serial.print("4");
  Serial.print("5");
  Serial.print("6");
  Serial.print("9");
  int statusCode = httpClient->responseStatusCode();
  Serial.println("10");
  String response = httpClient->responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

bool NetworkHelper::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}