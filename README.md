# PWM-Based Buck-Boost Converter with ESC and Sensor Feedback

This project implements a **smart DC-DC buck-boost converter** using Arduino. It is designed to dynamically control output voltage based on **real-time voltage and current sensor inputs**, and includes motor speed control via an **Electronic Speed Controller (ESC)**. The use of both **manual software-based PWM** and **hardware Timer2-driven PWM** enables precise control for power regulation.

---

## 📘 Project Overview

In electric vehicle (EV) or DC motor control scenarios, there are moments when voltage levels must be **stepped up (boost)** or **stepped down (buck)** to ensure performance. This project automates that decision-making process using feedback from voltage and current sensors. In cases where the motor faces load resistance (like getting stuck), the boost mode helps increase power. A **manual override (relay)** is also included to enable boost mode on demand.

---

## 🔩 Hardware Used

| Component                     | Description/Role                        |
|------------------------------|------------------------------------------|
| Arduino Uno/Nano             | Core controller for logic and PWM       |
| Voltage Sensor (e.g., ZMPT101B) | Measures system voltage               |
| Current Sensor (e.g., ACS712)   | Monitors system current               |
| ESC (Electronic Speed Controller) | Controls motor speed via PWM        |
| Brushless DC Motor           | Driven by ESC                           |
| Potentiometer (10kΩ)         | User input to adjust PWM/ESC speed      |
| Relay Module / Manual Switch | Manual override for boost activation    |
| Power Transistor/MOSFETs     | Used for buck-boost circuit switching   |
| Buck and Boost Converter Modules | Step-down/step-up regulation        |

---

## 🔧 Features

- ✅ **Automatic Buck and Boost Switching**
- ⚙️ **100kHz Boost PWM using Timer2 (hardware-driven)**
- 🔁 **Software PWM for Buck using delay()**
- ⚡ **Voltage and Current Sensing for Decision Making**
- 🧲 **ESC (Electronic Speed Controller) Support**
- 🔘 **Manual Override with Relay/Selection Pin**
- 🔄 **Independent Control Loops for Buck and Boost**

---

## 🧠 Logic Summary

### Inputs Monitored:

- **Voltage Sensor** → A1
- **Current Sensor** → A2
- **Manual Relay Switch** → D2 (override)

### Decision Control Logic:

If Voltage < 1.8V OR Current > 2.0A → BOOST Mode
Else → BUCK Mode

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

🔌 PWM Behavior
Boost Mode:
Activated by sensor thresholds or manual relay override.

Boost PWM signal generated at 100kHz on pin D3 (OC2B) via Timer2.

An additional PWM pulse train is generated on pin D5 using delayMicroseconds() inside a for loop.

Buck Mode:
Operates by varying on-time and off-time using values from the potentiometer on A0.

Output PWM is generated manually on pin D5 using delay().

ESC Control:
ESC connected to D9.

Potentiometer sets PWM signal from 1000 to 2000 µs, adjusting motor speed.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

💻 Coding Highlights
Timer2 Configuration for Boost PWM (100kHz)

TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // Fast PWM on OC2B
TCCR2B = _BV(WGM22) | _BV(CS20);                // No prescaler
OCR2A = 159;  // Top value for 100kHz frequency
OCR2B = 40;   // 25% duty cycle

This setup allows independent and uninterrupted PWM for boost mode, while the rest of the code runs its main logic and controls other peripherals.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

📈 Main Loop Flow

1. Read sensor values (voltage, current, potentiometer)
2. Convert raw sensor values to volt/amp using 5.0/1023.0 scale
3. Map potentiometer value for:
   - ESC speed (1000–2000 µs)
   - Buck PWM on-time (1–8 ms)
4. If (voltage < 1.8 OR current > 2.0 OR selectionPin == LOW):
     → Run Boost: trigger high-frequency PWM + extra pulse loop
   Else:
     → Run Buck: Manual PWM using onTime / offTime
5. ESC controlled continuously using `Servo.writeMicroseconds()`

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


✅ Getting Started
Wire up the components according to the table above.

Upload the .ino code using Arduino IDE.

Open Serial Monitor (optional) for debug prints.

Adjust the potentiometer to control buck PWM & motor speed.

Manually toggle the relay or test conditions to trigger Boost mode.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


🚀 Real-World Applications
Electric Vehicle (EV) power management

Smart power supply units

DC motor control under varying loads

Educational demonstration of sensor-based power control systems

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


📄 License
MIT License – open for personal, academic, and non-commercial use.

