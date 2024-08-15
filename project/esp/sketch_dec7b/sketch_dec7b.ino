#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include "connection.h"

FirebaseData firebaseData;

String nodeID = "-nmL-12312mn";

void setup() {
  Serial.begin(9600);

  connectToWiFi();
  connectToFirebase();

  nodeID = "-nmL-12312mn";

  Serial.println("Setup complete");
}

void loop() {
  if (Serial.available() > 0) {
    String serialData = Serial.readStringUntil('\n');

    Serial.println("Received: " + serialData);

    if (serialData.startsWith("reading:")) {
      serialData.remove(0, 8);

      int separatorIndex = serialData.indexOf(' ');
      if (separatorIndex != -1) {
        float phValue = serialData.substring(0, separatorIndex).toFloat();
        int waterLevel = serialData.substring(separatorIndex + 1).toInt();

        updateDataInFirebase(nodeID, phValue, waterLevel);
      } else {
        Serial.println("Invalid data format received");
      }
    }
  }

  delay(100);
}

void updateDataInFirebase(String nodeID, float phValue, int waterLevel) {
  if (nodeID.length() == 0) {
    Serial.println("Node ID is empty");
    return;
  }

  String pathWater = "/Reading/" + nodeID + "/waterPercentage";
  String pathPH = "/Reading/" + nodeID + "/phValue";

  if (Firebase.setInt(firebaseData, pathWater, waterLevel)) {
    Serial.println("Water percentage updated successfully");
  } else {
    Serial.println("Failed to update water percentage");
    Serial.println("Error: " + firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, pathPH, phValue)) {
    Serial.println("pH value updated successfully");
  } else {
    Serial.println("Failed to update pH value");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}
