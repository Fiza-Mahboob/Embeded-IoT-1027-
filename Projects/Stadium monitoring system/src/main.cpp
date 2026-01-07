#define BLYNK_TEMPLATE_ID "TMPL6c64A_Zfc"
#define BLYNK_TEMPLATE_NAME "Smart stadium"
#define BLYNK_AUTH_TOKEN "IkPtIhwJH7Br2IdBfKuPiZLjMtqEooly"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>
/*************** BLYNK CONFIG ***************/


#define BLYNK_PRINT Serial


/*************** WIFI ***************/
char ssid[] = "NTU FSD";
char pass[] = "";

/* ---------------- OLED ---------------- */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ---------------- Pins ---------------- */
#define IR_PIN 27
#define BUZZER_PIN 26
#define SERVO_PIN 2
#define DHTPIN 32
#define LDR_PIN 34

/* ---------------- LEDs ---------------- */
#define NUM_LEDS 4
int LED_PINS[NUM_LEDS] = {18, 16, 17, 19};

/* ---------------- Limits ---------------- */
#define MAX_PERSONS 20

/* ---------------- Objects ---------------- */
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
Servo doorServo;
BlynkTimer timer;

/* ---------------- Variables ---------------- */
int personCount = 0;
bool stadiumFull = false;

/* --- Accurate IR debounce --- */
unsigned long lastTriggerTime = 0;
const unsigned long debounceTime = 800;

/* ---------------- Functions ---------------- */
void longBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void shortBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}

void updateOLED(float temp, float hum) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Stadium");

  display.setTextSize(2);
  display.setCursor(0, 15);
  display.print("Count:");
  display.println(personCount);

  display.setTextSize(1);
  display.setCursor(0, 40);

  if (stadiumFull) {
    display.println("STADIUM FULL");
  } else {
    display.print("T:");
    display.print(temp);
    display.print("C H:");
    display.print(hum);
    display.println("%");
  }

  display.display();
}

bool lightsOn = false;

void checkLighting() {
  int ldrValue = analogRead(LDR_PIN);
  lightsOn = (ldrValue > 2000);

  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], lightsOn ? HIGH : LOW);
  }
}

/* -------- Send Data to Blynk -------- */
void sendToBlynk(float temp, float hum) {
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, personCount);
  Blynk.virtualWrite(V3, lightsOn ? 255 : 0);
}

/* ---------------- SETUP ---------------- */
void setup() {
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  dht.begin();

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setTextColor(SSD1306_WHITE);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("System Starting...");
  display.display();
  delay(2000);

  /* -------- Blynk -------- */
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(5000L, []() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      sendToBlynk(t, h);
    }
  });
}

/* ---------------- LOOP ---------------- */
void loop() {
  Blynk.run();
  timer.run();

  int irState = digitalRead(IR_PIN);

  if (!stadiumFull && irState == LOW &&
      (millis() - lastTriggerTime > debounceTime)) {

    personCount++;
    lastTriggerTime = millis();
    shortBeep();

    if (personCount >= MAX_PERSONS) {
      stadiumFull = true;
      doorServo.write(90);
      longBeep();
    }
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    temperature = 0;
    humidity = 0;
  }

  updateOLED(temperature, humidity);
  checkLighting();

  delay(300);
}
