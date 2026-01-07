# Assignment 1 - Task A  
### Control LEDs and Display Mode Using Buttons  
**Author:** Fareena Shahbaz  
**Reg #:** 23-NTU-CS-FL-1024  
**Course:** Embedded IoT Systems (Fall 2025)  
**University:** National Textile University, Faisalabad  

---

##  Wokwi Project Link
You can view and simulate this project on Wokwi:  
[Wokwi Simulation Link](https://wokwi.com/projects/445497187511394305)

---

##  Overview
This project demonstrates how to control multiple LEDs and a buzzer using two buttons and display the current operational mode on an OLED screen.  
The system is implemented using the **ESP32** microcontroller and uses **non-blocking debounce logic** to ensure reliable button input.

---

##  Main Components and Technologies

| Component | Description |
|------------|-------------|
| **ESP32** | The main microcontroller used to control LEDs, buttons, and OLED |
| **LEDs (x3)** | Visual indicators for different modes and button feedback |
| **Buttons (Mode, Reset)** | Used to switch between LED modes and reset the system |
| **Buzzer** | Optional alert indicator |
| **OLED Display (SSD1306)** | Displays the current LED operation mode |
| **Arduino IDE** | Platform used for coding, compiling, and uploading firmware |

---

## Working Principle

The system operates in **four different modes**, which are cycled through using the **MODE button**:  

| Mode | Description |
|------|-------------|
| **BOTH_OFF** | Turns all LEDs off |
| **ALT_BLINK** | LEDs 1 and 2 blink alternately every 400 ms |
| **BOTH_ON** | All LEDs stay continuously ON |
| **PWM_FADE** | LEDs smoothly fade in and out using PWM |

The **RESET button** sets the system back to the `BOTH_OFF` mode.  
All current modes are displayed on the **OLED** in real-time.

---

## Debouncing Logic
Both buttons use **software debouncing** with a stable time of `25 ms`.  
This prevents false triggers from noisy input signals and ensures smooth mode transitions.

---

##  File Description

| File | Description |
|------|-------------|
| `main.cpp` | Contains the complete implementation of the LED control system, OLED handling, debounce logic, and PWM control |

---

## Functions Overview

| Function | Purpose |
|-----------|----------|
| `setLedsToOff()` | Turns off all LEDs |
| `setLedsToHigh()` | Turns on all LEDs to full brightness |
| `handleModeChange()` | Cycles through operational modes and updates the display |
| `updateOLED()` | Updates OLED with the current mode text |

---

## How to Run

1. **Open** the project in Arduino IDE.  
2. **Select Board:** `ESP32 Dev Module`  
3. **Connect** ESP32 to your computer.  
4. **Upload** the code.  
5. Use:
   - **Mode Button (GPIO 34):** To change LED operation mode  
   - **Reset Button (GPIO 35):** To reset system to BOTH_OFF  

---

## Output Display Examples

| Mode | OLED Text | LED Behavior |
|------|------------|---------------|
| BOTH_OFF | “BOTH OFF” | All LEDs off |
| ALT_BLINK | “ALT BLINK” | LEDs blink alternately |
| BOTH_ON | “BOTH ON” | All LEDs ON |
| PWM_FADE | “PWM FADE” | LEDs fade in/out smoothly |

---

## Summary
This assignment demonstrates **multi-mode LED control**, **debounce logic**, and **OLED interfacing** using the **ESP32**.  
It is a foundational step in developing **interactive IoT systems** with multiple hardware components working together.

---

