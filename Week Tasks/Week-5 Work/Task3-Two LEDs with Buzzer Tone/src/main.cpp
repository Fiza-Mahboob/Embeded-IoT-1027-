//TASK3-Two LED ON with Buzzer Tone
#include <Arduino.h>

// --- Buzzer configuration ---
#define BUZZER_PIN 27
#define BUZZER_CH  0
#define BUZZER_FREQ 2000
#define BUZZER_RES 10

// --- LED 1 configuration ---
#define LED1_PIN 18
#define LED1_CH  1
#define LED_FREQ 5000
#define LED_RES 8

// --- LED 2 configuration ---
#define LED2_PIN 19
#define LED2_CH  2

void setup() {
  // Setup buzzer PWM
  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);

  // Setup LEDs PWM
  ledcSetup(LED1_CH, LED_FREQ, LED_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);

  ledcSetup(LED2_CH, LED_FREQ, LED_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);

  // --- Buzzer patterns ---
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(BUZZER_CH, 2000 + i * 400);
    delay(150);
    ledcWrite(BUZZER_CH, 0);
    delay(150);
  }

  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(BUZZER_CH, f);
    delay(20);
  }
  ledcWrite(BUZZER_CH, 0);
  delay(500);

  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(BUZZER_CH, melody[i]);
    delay(250);
  }
  ledcWrite(BUZZER_CH, 0);
}

void loop() {
  // LED1 brightens while LED2 dims
  for (int d = 0; d <= 255; d++) {
    ledcWrite(LED1_CH, d);
    ledcWrite(LED2_CH, 255 - d);
    delay(10);
  }

  // LED1 dims while LED2 brightens
  for (int d = 255; d >= 0; d--) {
    ledcWrite(LED1_CH, d);
    ledcWrite(LED2_CH, 255 - d);
    delay(10);
  }
}
