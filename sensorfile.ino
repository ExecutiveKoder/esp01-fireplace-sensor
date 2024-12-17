#include <ESP8266WiFi.h>

// Replace these with your Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Define the relay pin
const int relayPin = 2; // GPIO2 on ESP-01

WiFiServer server(80);

void setup() {
  // Set the relay pin as output
  pinMode(relayPin, OUTPUT);

  // Ensure the relay is OFF at startup
  digitalWrite(relayPin, HIGH); // HIGH usually corresponds to OFF for active LOW relays

  // Start serial communication for debugging
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("New Client.");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Handle the request
  if (request.indexOf("/relay/on") != -1) {
    digitalWrite(relayPin, LOW); // LOW usually corresponds to ON for active LOW relays
    Serial.println("Relay turned ON");
  } else if (request.indexOf("/relay/off") != -1) {
    digitalWrite(relayPin, HIGH); // HIGH usually corresponds to OFF for active LOW relays
    Serial.println("Relay turned OFF");
  }

  // Prepare the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>ESP01 Relay Control</h1>");
  client.println("<p><a href=\"/relay/on\">Turn Relay ON</a></p>");
  client.println("<p><a href=\"/relay/off\">Turn Relay OFF</a></p>");
  client.println("</html>");

  // Close the connection
  delay(1);
  Serial.println("Client disconnected.");
}
