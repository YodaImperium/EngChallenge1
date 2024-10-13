// Credits:
// All interaction with the TFT Display is based on Example TFT_eSPI/Test and Diagnostics/Colour_Test
// All time retrieving functions are based on Example ESP32/Time/SimpleTime
// The Wifi functionality is implemented according to the Connecting to eduroam example provided on Moodle

#include <WiFi.h>
#include "esp_eap_client.h"

#include <SPI.h>
#include <TFT_eSPI.h>

#include "time.h"
#include "esp_sntp.h"


TFT_eSPI tft = TFT_eSPI();

// WIFI authentication vars
const char* USER = "zcabplb@ucl.ac.uk";
const char* PASS = "";

// time ntp servers and time zone localisation
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

// prints current local time to both tft display and console 
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B, %d, %Y, %H:%M:%S");

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 4, 4);
  tft.println(&timeinfo, "%A");
  tft.println(&timeinfo, "%B, %d");
  tft.println(&timeinfo, "%Y");
  tft.println(&timeinfo, "%H:%M:%S");
}

// see Example ESP32/Time/SimpleTime
// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  //printLocalTime();
}

void setup() {
  tft.init();
  Serial.begin(115200);


  // Display Setup
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 4, 4);
  tft.setTextColor(TFT_WHITE);

  // WIFI Setup
  WiFi.mode(WIFI_STA);
  esp_eap_client_set_identity((uint8_t *)USER, strlen(USER));
  esp_eap_client_set_username((uint8_t *)USER, strlen(USER));
  esp_eap_client_set_password((uint8_t *)PASS, strlen(PASS));
  esp_wifi_sta_enterprise_enable();
  WiFi.begin("eduroam");

  Serial.print("Connecting");

  while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");

  Serial.print("Connected to WiFi network with IP Adress: ");
  Serial.println(WiFi.localIP());

  // Time Setup
  sntp_set_time_sync_notification_cb(timeavailable);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

}

void loop() {

  delay(1000);
  printLocalTime();

}
