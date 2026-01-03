#define BLYNK_TEMPLATE_ID "TMPL2kz0tmNZU"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "8lyVsCLo292MkbHB_LHFhAzu2CWKK1Jq"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Blynk Auth Token and Wi-Fi Credentials
char auth[] = "8lyVsCLo292MkbHB_LHFhAzu2CWKK1Jq";  // Replace with Blink token
char ssid[] = "SpectrumSetup-212D";                // Replace with your WiFi SSID
char pass[] = "Jaibalayya";                        // Replace with Wi-Fi password

// Sensor Pins
#define IR_PIN 15          // GPIO pin connected to IR sensor
#define ULTRASONIC_TRIG 4  // GPIO pin for Ultrasonic Trig
#define ULTRASONIC_ECHO 5  // GPIO pin for Ultrasonic Echo
#define SHOCK_SENSOR_PIN 19  // Pin connected to shock sensor

// Thresholds
const int distanceThreshold = 20; // Trigger alert for distance less than 20 cm
bool alertSent = false;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Sensor setup
  pinMode(IR_PIN, INPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(SHOCK_SENSOR_PIN, INPUT);

  Serial.println("System Initialized. Monitoring for intrusions...");
}

void loop() {
  Blynk.run();          // Run Blynk
  detectIntrusion();    // Monitor sensors
}

void detectIntrusion() {
  // IR Sensor Detection
  if (digitalRead(IR_PIN) == HIGH) {
    sendAlert("IR Motion Detected!");
    delay(1000); // Delay to avoid multiple alerts
  }

  // Ultrasonic Sensor Detection
  long duration, distance;
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  if (distance < distanceThreshold && !alertSent) {
    sendAlert("Ultrasonic Proximity Alert! Object Detected < 20 cm.");
    alertSent = true;
    delay(1000); // Delay to avoid multiple alerts
  } else if (distance >= distanceThreshold) {
    alertSent = false; // Reset alert status when object is far
  }

  // Updated Shock Sensor Detection
  int sensorValue = digitalRead(SHOCK_SENSOR_PIN);  // Read sensor output
  if (sensorValue == HIGH) {  // Check if shock is detected
    sendAlert("Shock Detected! Possible Intrusion.");
    Serial.println("Shock detected");
    delay(100);  // Debounce delay to avoid repeated messages
  }
}

void sendAlert(String message) {
  // Log the event to the Blynk app
  Blynk.logEvent("intrusion_alert", message); // Ensure "intrusion_alert" is defined in Blynk
  Serial.println(message);
}
