#ifndef FIREBASE_CONNECTION_H
#define FIREBASE_CONNECTION_H

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <WiFiManager.h> // Include the WiFiManager library
#include <LiquidCrystal_I2C.h>

#define FIREBASE_HOST "https://optipond-80d47-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "GDiJQOAEkloBHGSS6Wjb8zUTpR5xoG3oaVzYk2tA"

// Forward declaration
void saveConfigCallback();

void connectToWiFi() {
  // Create an instance of WiFiManager
  WiFiManager wifiManager;
  
  
  // Set callback functions to handle saving custom parameters
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setDebugOutput(false);

  // Try to connect to previously saved WiFi credentials
  if (!wifiManager.autoConnect()) {
    // Reset and try again
    ESP.reset();
    delay(5000);
  }


  // If connected, print the assigned IP address
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void connectToFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  
}

void saveConfigCallback() {
  // Called when WiFiManager enters configuration mode to save custom parameters
  Serial.println("Configuring WiFi...");
  // You can add custom logic here to save the WiFi credentials to EEPROM or any other storage
}




#endif
