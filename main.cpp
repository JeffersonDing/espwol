/*
 * Created Date: Monday May 24th 2021
 * Author: Jefferson Ding
 * -----
 * Last Modified: Monday May 24th 2021 10:30:33 pm
 * Modified By: Jefferson Ding at <tyding2004@outlook.com>
 * -----
 * Copyright (c) 2021 Jefferson Ding
 */

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();

// WiFi parameters 
const char *ssid = "your ssid";
const char *password = "your psswd";

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

// Define powerbutton pin
#define POWER_BUTTON_PIN D0

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

int push(String command)
{
  digitalWrite(POWER_BUTTON_PIN, HIGH);
  Serial.println("Button Pushed...");
  delay(200);
  digitalWrite(POWER_BUTTON_PIN, LOW);
  return 1;
}

void setup(void)
{

  // Initiate D0 as an output
  pinMode(POWER_BUTTON_PIN, OUTPUT);

  // Start Serial
  Serial.begin(115200);

  // Function to be exposed
  rest.function("push", push);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("espwol");
  rest.set_name("j3of0");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{

  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  while (!client.available())
  {
    delay(1);
  }
  rest.handle(client);
}

