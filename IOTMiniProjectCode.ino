#include <WiFi.h>
#include <PubSubClient.h>

// Create WiFi and MQTT client instances
WiFiClient wificlient;
PubSubClient client(wificlient);

// ==========================
// Wi-Fi SSID , PASSWORD Setup
// ==========================
#define ssid "Omar 17"
#define pass "111112223"

// ==========================
// Pin Definitions
// ==========================
#define LED_GREEN 14   // LED controlled via mobile app (MQTT)
#define LED_RED   23   // LED indicates button (IR sensor) status
#define BTN_PIN    0   // Simulated IR sensor (button input)

long long IR_TIME;
char message[100];

// ==========================
// MQTT Callback Function
// Called when a message arrives from the subscribed topic
// ==========================
void callback(char topic[], byte* payload, unsigned int length) {
  Serial.println("topic: ");
  Serial.println(topic);

  Serial.println("message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message[i] = (char)payload[i];
  }
  message[length] = '\0';  // Null-terminate the message
  Serial.println();

  // Handle received message to control LED_GREEN
  if (strcmp(message, "on")) {
    digitalWrite(LED_GREEN, LOW);  // LED ON (active LOW)
    Serial.println("led ON");
  }
  else if (strcmp(message, "off")) {
    digitalWrite(LED_GREEN, HIGH); // LED OFF
    Serial.println("led OFF");
  }
}

// ==========================
// Setup Function
// Initializes WiFi, MQTT, and I/O pins
// ==========================
void setup() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  WiFi.begin(ssid, pass);
  Serial.begin(9600);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi");
    Serial.println(WiFi.status());
    delay(500);
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Connect to MQTT broker
  client.setServer("broker.emqx.io", 1883);
  client.setCallback(callback);
  while (!client.connect("omarmmomarom")) {
    Serial.println("Connecting to broker");
    delay(500);
  }
  Serial.println("Connected to broker");

  // Subscribe to topic
  while (!client.subscribe("/OPERA/omar/led")) {
    Serial.println("Subscribing to /OPERA/omar/led ");
    delay(500);
  }
  Serial.println("Subscribed to /OPERA/omar/led ");

  // Initialize IR sensor timer
  IR_TIME = millis();
}

// ==========================
// Loop Function
// Publishes IR sensor (button) state to MQTT
// ==========================
void loop() {
  client.loop();  // Keep MQTT connection alive

  // Check every 250 ms for button status
  if (millis() - IR_TIME >= 250) {
    if (digitalRead(BTN_PIN) == LOW) {
      client.publish("/sectorb5/omar/led", "on");
      Serial.println("IR_Sensor on");
      digitalWrite(LED_RED, HIGH);  // Turn on feedback LED
    }
    else if (digitalRead(BTN_PIN) == HIGH) {
      client.publish("/sectorb5/omar/led", "off");
      Serial.println("IR_Sensor off");
      digitalWrite(LED_RED, LOW);   // Turn off feedback LED
    }
    // Reset timer
    IR_TIME = millis();
  }
}
