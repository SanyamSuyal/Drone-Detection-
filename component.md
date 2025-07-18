# 📦 Components Guide  
## Drone Detection & City Blackout Defense System  
**Developer:** Sanyam Suyal  
**Version:** Gamma-G v2.0 (Final Build)  
**Date:** July 2025  
**GitHub:** [github.com/sanyamsuyal](https://github.com/sanyamsuyal)

---

## Overview

This document lists and explains all the hardware components used in the Drone Detection & City Blackout Defense System. Each component is carefully selected to fulfill specific roles in sensor detection, alerting, and city blackout control.

---

## Components List

| **Component**             | **Quantity** | **Purpose**                                                  | **Notes**                                             |
|---------------------------|--------------|--------------------------------------------------------------|-------------------------------------------------------|
| **Arduino Uno / Compatible Board** | 1            | Main microcontroller to control sensors, actuators, and logic | Must have PWM pins and analog input pins               |
| **Ultrasonic Sensor (HC-SR04 or equivalent)** | 1            | Measures distance by sending and receiving sound waves       | Requires 2 digital pins (Trig, Echo)                   |
| **Servo Motor (Standard 180°)**       | 1            | Rotates the ultrasonic sensor to scan the environment        | Connected to PWM pin; controls scanning angle          |
| **Relay Module (5V)**       | 1            | Controls city blackout by switching high-current lights       | Use relay rated for your city lighting load             |
| **Buzzer (Piezo or Active)**            | 1            | Audible alert during detected threat                           | Connect to digital output pin                           |
| **Infrared (IR) Sensors**   | 2            | Motion detection via infrared beam interruptions               | Connect to analog input pins (A0, A1)                   |
| **Red LEDs**               | 3            | Visual alert indicator flashing during alerts                  | Connected to digital output pins for red flashing LEDs  |
| **City Lights LEDs (or LED Array)** | 3            | Simulated city lighting controlled via relay                   | Can be replaced with real city light circuits           |
| **Laser Servo Motors (Optional)**  | 2            | For laser aiming system pan and tilt servos (optional module) | Requires 2 PWM pins; optional based on configuration    |

---



## Additional Notes

- **Power Supply:** Ensure all components have appropriate voltage and current ratings; servos and relays may require separate power lines with common ground.
- **Safety:** Handle relay connections carefully; never exceed rated voltages or currents.
- **Wiring:** Use proper gauge wires, and label each connection clearly for maintenance and troubleshooting.
- **Expansion:** This modular system allows addition of more sensors or alert modules as needed.

---

## Developer’s Note

> _"This system is a result of meticulous design and coding, bringing together hardware and software in harmony for effective drone detection and city blackout control."_  
> — **Sanyam Suyal**

---

If you want me to help generate a bill of materials (BOM) or sourcing guide next, just let me know!
