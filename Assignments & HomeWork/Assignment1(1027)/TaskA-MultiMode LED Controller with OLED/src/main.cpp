/*
  Project Title : ESP32 Multi-Mode LED Controller with OLED Display
  Name          : Fiza Mahboob
  Reg. No.      : 23-NTU-CS-1027
  Date          : 2025-10-05
*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------- OLED Configuration ---------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// -------- Pin Definitions -----------
const int LED1_PIN = 18;
const int LED2_PIN = 17;
const int LED3_PIN = 16; 
const int BTN_MODE  = 34;
const int BTN_RESET = 35;
const int BUZZER_PIN = 25; 

// -------- Debounce Configuration --------
const unsigned long DEBOUNCE_DELAY = 25; // Debounce time in ms
// OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- Operating Modes -----------
enum Mode { BOTH_OFF, ALT_BLINK, BOTH_ON, PWM_FADE };
Mode currentMode = BOTH_OFF;

// ---------- Timing Variables -----------
unsigned long lastToggle = 0;       // Tracks LED blink timing
const unsigned long blinkInterval = 400; // Time interval for alternating blink

// --------- Button State Tracking -----------
unsigned long lastModePress = 0;
unsigned long lastResetPress = 0;
int lastModeState = HIGH;   
int lastResetState = HIGH;
bool modePressed = false;
bool resetPressed = false;

// --------- Fade Control Variables ------------
int fadeVal = 0; 
int fadeDir = 1; 

// --------- Function Prototypes -------------
void setLedsToOff();
void setLedsToHigh();
void updateOLED(const char *msg);
void handleModeChange();

void setup() {
  Serial.begin(115200);
  
  // Configure pin modes
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize I2C and OLED
  Wire.begin(21, 22); 
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED failed!");
    for (;;); // Stop execution if OLED setup fails
  }
 
  // Display initial mode
  updateOLED("BOTH OFF");
  setLedsToOff(); 
  Serial.println("Setup complete");
}

void loop() {
  unsigned long now = millis();

  // -------- MODE Button Handler --------
  int readingMode = digitalRead(BTN_MODE);
  
   // Detect change in state
  if (readingMode != lastModeState) {
    lastModePress = now;
  }
  
    // Debounce check
  if ((now - lastModePress) > DEBOUNCE_DELAY) {
    if (readingMode == LOW && !modePressed) {
      modePressed = true;
      digitalWrite(LED3_PIN, HIGH);
      handleModeChange();
      Serial.println("Mode button pressed");
    }
    else if (readingMode == HIGH) {
      modePressed = false;
      digitalWrite(LED3_PIN, LOW);
    }
  }
  
  lastModeState = readingMode;

  // -------- RESET Button Handler -----------
  int readingReset = digitalRead(BTN_RESET);
  
  if (readingReset != lastResetState) {
    lastResetPress = now;
  }
  
  if ((now - lastResetPress) > DEBOUNCE_DELAY) {
    if (readingReset == LOW && !resetPressed) {
      resetPressed = true;
      digitalWrite(LED3_PIN, HIGH);
      currentMode = BOTH_OFF;
      updateOLED("BOTH OFF");
      setLedsToOff();
      Serial.println("Reset button pressed");
    }
    else if (readingReset == HIGH) {
      resetPressed = false;
      digitalWrite(LED3_PIN, LOW);
    }
  }
  
  lastResetState = readingReset;

  // ------- LED behavior based on Mode -------
  switch (currentMode) {
    case BOTH_OFF:
      break;

    case ALT_BLINK:
    // Alternate LED blinking
      if (now - lastToggle >= blinkInterval) {
        lastToggle = now;
        static bool toggle = false;
        toggle = !toggle;
        
        analogWrite(LED1_PIN, toggle ? 255 : 0);
        analogWrite(LED2_PIN, toggle ? 0 : 255);
        digitalWrite(LED3_PIN, LOW); 
      }
      break;

    case BOTH_ON:
    // Both LEDs stay continuously on
      break;

    case PWM_FADE:
    // Smooth fading of both LEDs using PWM
      fadeVal += fadeDir * 1; 
      
      if (fadeVal >= 180) { fadeVal = 180; fadeDir = -1; }
      if (fadeVal <= 0)   { fadeVal = 0;   fadeDir = 1;  }
      
      analogWrite(LED1_PIN, fadeVal);
      analogWrite(LED2_PIN, fadeVal);
      digitalWrite(LED3_PIN, LOW);
      
      delay(20); 
      break;
  }
}

// -------- FUNCTION DEFINITIONS ----------

// Turns off all LEDs
void setLedsToOff() {
  analogWrite(LED1_PIN, 0); 
  analogWrite(LED2_PIN, 0); 
  digitalWrite(LED3_PIN, LOW); 
}

// Turns on all LEDs at full brightness
void setLedsToHigh() {
  analogWrite(LED1_PIN, 255); 
  analogWrite(LED2_PIN, 255); 
  digitalWrite(LED3_PIN, HIGH); 
}

// Handles switching between LED operation modes
void handleModeChange() {
    currentMode = (Mode)((currentMode + 1) % 4);
    
    switch (currentMode) {
      case BOTH_OFF:    
        updateOLED("BOTH OFF");   
        setLedsToOff();
        break;
      case ALT_BLINK:   
        updateOLED("ALT BLINK");  
        setLedsToOff();
        lastToggle = millis();
        break;
      case BOTH_ON:     
        updateOLED("BOTH ON");    
        setLedsToHigh();
        break;
      case PWM_FADE:    
        updateOLED("PWM FADE");   
        fadeVal = 0; 
        fadeDir = 1; 
        break;
    }
}

// Updates the OLED display with current mode information
void updateOLED(const char *msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}