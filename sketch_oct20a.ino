/*************************************************************
 * ESP32 IoT Switch using Blynk + Servo + Buzzer
 * When the button in Blynk app is pressed:
 * - Buzzer beeps with short delays
 * - Servo rotates after a few seconds (simulating switch flip)
 *************************************************************/

// ---- Blynk Template Details ----
#define BLYNK_TEMPLATE_ID "temp-ID"
#define BLYNK_TEMPLATE_NAME "temp-name"
#define BLYNK_AUTH_TOKEN "Your-blynk-device-auth-token" // Replace with your Auth Token

// ---- Library Includes ----
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ---- WiFi Credentials ----
char ssid[] = "your-wifi-name";       // Replace with your WiFi SSID
char pass[] = "your-wifi-passkey";   // Replace with your WiFi Password

// ---- Hardware Pins ----
#define SERVO_PIN  15   // GPIO pin connected to servo signal wire
#define BUZZER_PIN 16   // GPIO pin connected to buzzer
#define BUTTON_VPIN V1  // Virtual pin in Blynk for button widget

Servo servo;  // Servo object

// ---- Variables ----
bool isSwitchActive = false;  // Keeps track of button state

// ---- Function triggered when Blynk button changes state ----
BLYNK_WRITE(BUTTON_VPIN)
{
  int buttonState = param.asInt(); // Reads button state from app

  if (buttonState == 1) { // Button pressed
    if (!isSwitchActive) {
      isSwitchActive = true;
      beepAndFlipSwitch();
    }
  } else {
    isSwitchActive = false; // Button released (optional)
  }
}

// ---- Function to control buzzer + servo ----
void beepAndFlipSwitch() {
  Serial.println("Blynk Button Pressed - Starting sequence...");

  // Beep buzzer three times with delay
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }

  // Wait 2 seconds before servo moves
  delay(2000);

  // Move servo to simulate switch flip
  servo.write(90);   // Turn servo
  delay(1000);
  servo.write(0);    // Return to original
  Serial.println("Switch toggled!");
}

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);
  delay(1000);

  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  servo.attach(SERVO_PIN);
  servo.write(0); // Default position

  // Connect to WiFi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connecting to Blynk...");
}

void loop() {
  Blynk.run();
}
