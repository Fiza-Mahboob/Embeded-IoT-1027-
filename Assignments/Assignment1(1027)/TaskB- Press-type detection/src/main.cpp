
  // Project Title : ESP32 Press Type Detection(Short & Long Press)
  // Name          : Fiza Mahboob
  // Reg. No.      : 23-NTU-CS-1027
  // Date          : 2025-10-05

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- OLED Display Configuration ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// --- Pin Assignments ---
const int LED_PIN = 18;      // LED output pin
const int BTN_PIN = 34;      // Button input pin (connected to GND)
const int BUZZER_PIN = 25;   // Buzzer output pin

// --- Timing Parameters ---
const unsigned long DEBOUNCE_DELAY = 50;       // Debounce time for stable button input
const unsigned long LONG_PRESS_TIME = 1500;    // Long press threshold (in milliseconds)

// --- OLED Display Object ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- State Variables ---
bool buttonPressed = false;         // Tracks current button state
bool ledState = false;              // Stores LED ON/OFF state
unsigned long pressStartTime = 0;   // Time when button was first pressed
bool longPressTriggered = false;    // Prevents repeated long-press action

// --- Function Prototypes ---
void updateOLED(const char *msg);
void playBuzzerTone();

void setup() {
  Serial.begin(115200);

  // Configure I/O pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);   // Using internal pull-up (LOW when pressed)

  // Initialize OLED with I2C
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed!");
    while (true);  // Stop execution if OLED fails
  }

  digitalWrite(LED_PIN, LOW);   // Start with LED turned OFF
  updateOLED("LED OFF");
  Serial.println("System initialized - Task B");
}

void loop() {
  unsigned long now = millis();
  int reading = digitalRead(BTN_PIN);

  // Detect when button is pressed (FALLING edge)
  if (reading == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = now;
    longPressTriggered = false;
    Serial.println("Button pressed");
  }

  // Handle long press detection
  if (buttonPressed && reading == LOW && !longPressTriggered) {
    if ((now - pressStartTime) >= LONG_PRESS_TIME) {
      longPressTriggered = true;
      playBuzzerTone();
      updateOLED("BUZZER ON");
      Serial.println("Long press detected");
    }
  }

  // Detect when button is released (RISING edge)
  if (reading == HIGH && buttonPressed) {
    unsigned long pressDuration = now - pressStartTime;

    // Execute short press action if not a long press
    if (pressDuration < LONG_PRESS_TIME && !longPressTriggered) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);

      if (ledState) {
        updateOLED("LED ON");
        Serial.println("Short press → LED ON");
      } else {
        updateOLED("LED OFF");
        Serial.println("Short press → LED OFF");
      }
    }
    // Restore OLED after long press ends
    else if (longPressTriggered) {
      if (ledState) updateOLED("LED ON");
      else updateOLED("LED OFF");
    }

    buttonPressed = false;
    delay(DEBOUNCE_DELAY);
  }
}

// --- OLED Text Update Function ---
// Clears the screen and displays the provided message
void updateOLED(const char *msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.println(msg);
  display.display();
}

// --- Buzzer Activation Function ---
// Produces a 1000Hz tone for 0.5 seconds
void playBuzzerTone() {
  tone(BUZZER_PIN, 1000, 500);
}
