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

  // Create a JSON object to hold the data
  FirebaseJson json;
  json.add("id", "");  // Placeholder for the unique ID
  json.add("waterPercentage", waterLevel);
  json.add("phValue", phValue);

  // Push the JSON object to Firebase under the "Readings" parent node
  if (Firebase.pushJSON(firebaseData, path, json)) {
    // If the push was successful, get the unique identifier
    String uniqueID = firebaseData.pushName();

    // Update the JSON object with the actual unique ID
    json.clear();
    json.add("id", uniqueID);
    json.add("waterPercentage", waterLevel);
    json.add("phValue", phValue);

    // Set the updated JSON object to Firebase under the generated uniqueID
    if (Firebase.setJSON(firebaseData, path + "/" + uniqueID, json)) {
      Serial.println("Data pushed to Firebase successfully");
    } else {
      Serial.println("Failed to push data to Firebase");
      Serial.println("Error: " + firebaseData.errorReason());
    }
  } else {
    Serial.println("Failed to push data to Firebase");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}
