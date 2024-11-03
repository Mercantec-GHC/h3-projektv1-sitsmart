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
  abc = true;
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
              String givenSSID = currentLine.substring(currentLine.indexOf("=")+1, currentLine.indexOf("&pass"));
              String givenPass = currentLine.substring(currentLine.lastIndexOf("=")+1, currentLine.lastIndexOf(" HTTP"));

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
  Serial.println("connect to wifi");
  Serial.println("Attempting login with: " + ssid + " and: " + password);
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

void NetworkHelper::sendData(String body) {
  Serial.println("a");
  /*httpClient->beginRequest();
  httpClient->post("/api/TempHumidities");
  httpClient->sendHeader("Content-Type", "application/json");
  httpClient->sendHeader("Content-Length", body.length());
  httpClient->sendHeader("accept", "text/plain");
  httpClient->beginBody();
  Serial.println(isConnected());
  httpClient->print(body);
  httpClient->endRequest();

  int statusCode = httpClient->responseStatusCode();
  String response = httpClient->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.print("WiFi: ");
  Serial.println(isConnected());*/
}

bool NetworkHelper::isConnected() {
  //Serial.println(String(abc));
  return WiFi.status() == WL_CONNECTED;
}