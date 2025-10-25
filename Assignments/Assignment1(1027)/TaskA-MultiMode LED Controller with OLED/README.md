ESP32 Multi-Mode LED Controller with OLED Display

Name: Fiza Mahboob
Reg. No: 23-NTU-CS-1027
Course: Embedded IoT Systems
Semester: 5th (BSCS-A)
Instructor: Sir Nasir

🔗 Wokwi Simulation Link
[Click to Open Simulation on Wokwi](https://wokwi.com/projects/445812600209838081)

Project Overview
This project demonstrates how to control multiple LEDs using an ESP32 board with mode switching and reset functionality.
An OLED display shows the current LED mode in real time.
Modes include Both Off, Alternate Blink, Both On, and PWM Fade, controlled using two push buttons.

Pin Map
Component	    ESP32 Pin	     Description
LED1	        GPIO 18	         Main LED 1
LED2	        GPIO 17	         Main LED 2
LED3	        GPIO 16	         Indicator LED for button press
Mode Button	    GPIO 34	         Cycles through LED modes
Reset Button	GPIO 35	         Resets LEDs to “BOTH OFF”
OLED SDA	    GPIO 21	         Data line for I2C
OLED SCL	    GPIO 22	         Clock line for I2C
Buzzer	        GPIO 25	         Optional buzzer output

Screenshots
![Circuit Diagram](Screenshots/Circuit%20Diagram.jpg)
![Mode 1 - Both Off](Screenshots/Mode1-Both%20LEDs%20OFF.jpg)
![Mode 2 - Alternate Blink LED1](Screenshots/Mode2-ALT%20Blink%20LED1.jpg)
![Mode 2 - Alternate Blink LED2](Screenshots/Mode2-ALT%20Blink%20LED2.jpg)
![Mode 3 - Both On](Screenshots/Mode3-Both%20LEDs%20ON.jpg)
![Mode 4 - PWM Fade](Screenshots/Mode4-PWM%20Fade.jpg)
![Reset - Both Off](Screenshots/RESET%20to%20Off.jpg)

Key Features
Button debouncing using millis() timer.
OLED mode display using Adafruit_SSD1306 library.
PWM fading using analogWrite().
Clean, modular code with well-commented logic.

How to Run
Open project in Wokwi.
Upload code to ESP32 Dev Board.
Observe mode changes on OLED when pressing the Mode Button.
Use the Reset Button to return to “Both Off” mode.

Conclusion
This project integrates multiple hardware peripherals with ESP32 to demonstrate event-driven embedded programming.
It reinforces core IoT concepts such as button debouncing, PWM control, I2C communication, and non-blocking programming.