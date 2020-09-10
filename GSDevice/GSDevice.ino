/*  
 *   GSDevice - A Google Sheets controlled device
 *   https://youtu.be/aKgtz3-gwrY
 *   
 *   Created by Taste The Code - https://tastethecode.com
 *   Blagojce Bill Kolicoski
 *   
 */

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

// Fill ssid and password with your network credentials
const char* ssid = "YourWiFiName";
const char* password = "your_wifi_password";

const char* host = "script.google.com";
// Replace with your own script id to make server side changes
const char *GScriptId = "AKfycbyCyv8ukzqb1WXvN3m0UnKn44QmuLNqQofcBXQs1847Xh1Df8Y";
const char *cellAddress = "A5";

const int httpsPort = 443;

String url3 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress;

String payload = "";

HTTPSRedirect* client = nullptr;

void setup() {
  //init the relay control pin
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  
  Serial.begin(115200);
  Serial.flush();
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably, 
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  static int error_count = 0;
  static int connect_count = 0;
  const unsigned int MAX_CONNECT = 20;
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(false);
    client->setContentTypeHeader("application/json");
  }

  if (client != nullptr){
    if (!client->connected()){
      client->connect(host, httpsPort);
    }
  }
  else{
    Serial.println("Error creating client object!");
    error_count = 5;
  }
  
  if (connect_count > MAX_CONNECT){
    //error_count = 5;
    connect_count = 0;
    flag = false;
    delete client;
    return;
  }

  Serial.print("GET Data from cell: ");
  Serial.println(cellAddress);
  if (client->GET(url3, host)){
    //get the value of the cell
    payload = client->getResponseBody();
    payload.trim();
    Serial.println(payload);
    if (payload == "on" || payload == "On" || payload == "ON" || payload == "1" || payload == "true" || payload == "TRUE") {
      digitalWrite(16, LOW);
    } else {
      digitalWrite(16, HIGH);
    }
    ++connect_count;
  }
  else{
    ++error_count;
    Serial.print("Error-count while connecting: ");
    Serial.println(error_count);
  }
  
  if (error_count > 3){
    Serial.println("Halting processor..."); 
    delete client;
    client = nullptr;
    Serial.flush();
    ESP.deepSleep(0);
  }
  
  // Add some delay in between checks
  delay(1000);
                          
}
