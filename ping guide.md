# 🔒 Drone Detection & City Blackout Defense System - Pin Guide

---

## 🚀 Project Overview

**👨‍💻 Developer:** Sanyam Suyal  
**🛠️ Version:** Gamma-G v2.0 (Final Build)  
**📅 Date:** July 2025  
**🌐 GitHub:** [github.com/sanyamsuyal](https://github.com/sanyamsuyal)

A cutting-edge system featuring:  
- 🔄 Rotating Ultrasonic Detection with Servo Radar  
- 📡 IR Motion Detection Support  
- ⚡ Auto Blackout via Relay  
- 🚨 Red Alert LED Flashing + Buzzer Siren  
- 🎯 Modular Laser Aiming System (Optional)

---

## 📌 Pin Connections & Descriptions

| **Pin**         | **Type**         | **Component**                      | **Purpose**                                     |
|-----------------|------------------|----------------------------------|------------------------------------------------|
| `9`             | 🔌 Digital Out   | Ultrasonic Sensor Trigger         | Sends trigger pulses to ultrasonic sensor      |
| `10`            | 🔌 Digital In    | Ultrasonic Sensor Echo            | Receives echo signals from ultrasonic sensor   |
| `7`             | 🔌 Digital Out   | Relay Module                     | Controls city blackout relay switch             |
| `8`             | 🔌 Digital Out   | Buzzer                          | Emits alert siren sound                          |
| `A0`            | 🎛️ Analog In    | IR Sensor 1                     | Detects motion via infrared                      |
| `A1`            | 🎛️ Analog In    | IR Sensor 2                     | Detects motion via infrared                      |
| `6`             | 🎛️ PWM Out     | Servo Motor (Ultrasonic Radar)  | Rotates ultrasonic sensor for scanning          |
| `2, 3, 4`       | 🔌 Digital Out  | Red Alert LEDs                  | Flashes red LEDs during alerts                    |
| `5, 6, 11`      | 🔌 Digital Out  | City Lights LEDs               | Controls city lighting (ON/OFF via relay)        |
| `3`             | 🎛️ PWM Out     | Laser Servo Pan (Optional)     | Pans laser aiming servo                           |
| `5`             | 🎛️ PWM Out     | Laser Servo Tilt (Optional)   | Tilts laser aiming servo                          |

---

## 💡 Important Notes

- Analog pins `A0` and `A1` are used for IR sensors and must be wired correctly to detect motion signals.
- Relay on pin `7` switches the blackout mode by turning city lights OFF (pins 5, 6, 11).
- Servo on pin `6` rotates ultrasonic sensor in a sweeping arc (0°–180°).
- Laser servo pins (`3` and `5`) are **optional**, enabled only if `USE_LASER_SERVO` is set to `true`.
- Red LEDs (pins 2, 3, 4) flash rapidly to indicate active alerts.
- Buzzer (pin 8) provides audible alarms.
- City LEDs (pins 5, 6, 11) are normally ON, turned OFF during blackout mode.

---

## 🛠️ Wiring Diagram Summary

```plaintext
+------------------------+--------------------+---------------------------------+
| Component              | Arduino Pin        | Description                     |
+------------------------+--------------------+---------------------------------+
| Ultrasonic Trigger     | Digital Pin 9      | Trigger signal to ultrasonic   |
| Ultrasonic Echo        | Digital Pin 10     | Echo signal from ultrasonic    |
| Relay Module           | Digital Pin 7      | Blackout relay switch control  |
| Buzzer                 | Digital Pin 8      | Alert sound                    |
| IR Sensor 1            | Analog Pin A0      | Motion detection via IR        |
| IR Sensor 2            | Analog Pin A1      | Motion detection via IR        |
| Ultrasonic Servo Motor | PWM Pin 6          | Servo rotation for scanning    |
| Red Alert LEDs         | Digital Pins 2,3,4 | Flashing alert LEDs            |
| City Lights LEDs       | Digital Pins 5,6,11| City light control             |
| Laser Servo Pan (Opt.) | PWM Pin 3          | Laser servo pan control        |
| Laser Servo Tilt (Opt.)| PWM Pin 5          | Laser servo tilt control       |
+------------------------+--------------------+---------------------------------+
