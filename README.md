# 🔴 Drone Detection & Automated Blackout System  
**Coded by Sanyam Suyal**

## 💡 Overview
This project is a smart **anti-drone defense system** using an Arduino Uno, ultrasonic sensor, relay module, servo motor, buzzer, and indicator lights. The system detects drones within a defined proximity and automatically performs a **simulated blackout** by switching off white lights, turning on red warning lights, and sounding a buzzer alarm.

This is ideal for simulating security zones, restricted airspaces, or sensitive installations.

---

## 🧠 Features

- 🔍 **Drone Detection** using an Ultrasonic Sensor with dynamic 180° scanning via a Servo.
- 🚨 **Automatic Blackout Activation**:
  - White light OFF
  - All red lights ON
  - Buzzer ON (alarm)
- 🔄 **Servo Sweeping** for wider detection area.
- ⚙️ **Modular Code** – Easily remove/add features like buzzer, lights, or servo rotation by toggling one variable.

---

---


---

## 📂 Project Structure

```bash
Drone-Detection-System/
├── Drone_Detector.ino      # Main Arduino code
├── circuit_diagram.png     # Wiring diagram
└── README.md               # Documentation
