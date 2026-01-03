#include <WiFi.h>
#include <WiFiClient.h>

// WiFi credentials (match with Slave's AP credentials)
const char* ssid = "SpectrumSetup-212D";
const char* password = "Jaibalayya";
const char* slaveIP = "192.168.4.1"; // Slave ESP32 IP
const int slavePort = 1234;

WiFiClient client;

// Pin definitions
#define BUTTON_PIN 25
#define BUZZER_PIN 26

// Debounce variables
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50 ms debounce delay
int lastButtonState = HIGH; // Initial state of the button
int buttonState;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);

  // Connect to Slave's AP
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Slave's WiFi AP...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Slave's WiFi AP");
}

void loop() {
  // Read the button state with debounce logic
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Check if button is pressed (LOW state)
      if (buttonState == LOW) {
        sendAlert();
      }
    }
  }
  lastButtonState = reading;
  delay(10);
}

void sendAlert() {
  // Attempt connection to Slave ESP32
  if (client.connect(slaveIP, slavePort)) {
    Serial.println("Connected to Slave");
    client.println("ALERT: Button Pressed");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    client.stop();
  } else {
    Serial.println("Failed to connect to Slave");
  }
}
