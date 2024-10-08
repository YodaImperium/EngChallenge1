#include <WiFi.h>
#include "esp_eap_client.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* USER = "zcabplb@ucl.ac.uk";
const char* PASS = "";

const char* url = "http://worldtimeapi.org/api/timezone/Europe/London";

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  esp_eap_client_set_identity((uint8_t *)USER, strlen(USER));
  esp_eap_client_set_username((uint8_t *)USER, strlen(USER));
  esp_eap_client_set_password((uint8_t *)PASS, strlen(PASS));
  esp_wifi_sta_enterprise_enable();
  WiFi.begin("eduroam");

  Serial.print("Connecting");
  String res;

  while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");

  Serial.print("Connected to WiFi network with IP Adress: ");
  Serial.println(WiFi.localIP());

  if (WiFi.status()== WL_CONNECTED) {

    res = httpGETRequest(url);
    Serial.println(res);

    JSONVar time_info = JSON.parse(res);

  }

}

void loop() {
  // put your main code here, to run repeatedly:

}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}