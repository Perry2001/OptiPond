#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include "connection.h"

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);


  connectToWiFi();
  connectToFirebase();

  Serial.println("Setup complete");
}

void loop() {
  // Check if there is data available to read from the serial port
  if (Serial.available() > 0) {
    // Read the incoming line
    String serialData = Serial.readStringUntil('\n');

    // Print the received line
    Serial.println("Received: " + serialData);

    // Parse the received data
    if (serialData.startsWith("reading:")) {
      // Remove the "reading:" prefix
      serialData.remove(0, 8);

      // Parse the float and int values
      float phValue = serialData.substring(0, serialData.indexOf(' ')).toFloat();
      int waterLevel = serialData.substring(serialData.indexOf(' ') + 1).toInt();

      // Push the values to Firebase
      pushDataToFirebase(phValue, waterLevel);
    }
  }

  // Your other code can go here

  // Delay to avoid excessive looping
  delay(100);
}

void pushDataToFirebase(float phValue, int waterLevel) {
  String path = "/Reading";

  // Push the phValue and waterLevel to Firebase under the specified path
  if (Firebase.pushFloat(firebaseData, path + "/phValue", phValue) &&
      Firebase.pushInt(firebaseData, path + "/WaterPercentage", waterLevel)) {
    Serial.println("Data pushed to Firebase successfully");
  } else {
    Serial.println("Failed to push data to Firebase");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}
