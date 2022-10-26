#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
 
char ssid[] = "Spothot";
char password[] = "12345678";

WiFiClientSecure client;
HTTPClient http;
 
//const String endpoint = "https://gateway.apiportal.ns.nl/reisinformatie-api/api/v3/disruptions[?type][&isActive]";
const String endpoint = "https://gateway.apiportal.ns.nl/reisinformatie-api/api/v3/disruptions?type=calamity&isActive=true";

const String key = "e2def5ae092e4b53add2c6350926fc7d";

// The fingerprint of the site you want to connect to.
#define HOST_FINGERPRINT "BA 78 CA 7E 98 B7 32 35 46 1F 62 CA E1 BF 27 1E A8 75 DE 01"
// The finger print will change every few months.
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  // If you want to check the fingerprint
  client.setFingerprint(HOST_FINGERPRINT);
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    http.begin(client, endpoint); //Specify the URL
    http.addHeader("Ocp-Apim-Subscription-Key", key, true);

    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(30000);
 
}