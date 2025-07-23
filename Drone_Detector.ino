/*
  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
  ┃   🔒 Drone Detection & City Blackout Defense System      ┃
  ┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃
  ┃    Developer : Sanyam Suyal                              ┃
  ┃    Version   : Gamma-G v2.0 (Final Build)                ┃
  ┃    Date      : July 2025                                 ┃
  ┃    GitHub    : github.com/sanyamsuyal                    ┃
  ┃    Description:                                          ┃
  ┃     ▸ Rotating Ultrasonic Detection with Servo Radar     ┃
  ┃     ▸ IR Motion Detection Support                        ┃
  ┃     ▸ Auto Blackout via Relay                            ┃
  ┃     ▸ Red Alert LED Flashing + Buzzer Siren              ┃
  ┃     ▸ Modular Laser Aiming System (Optional)             ┃
  ┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

#include <Servo.h>

// ====== CONFIGURATION FLAGS ======
#define USE_ULTRASONIC_SERVO   true    // Servo radar scan
#define USE_RELAY_BLACKOUT     true    // Control blackout relay
#define USE_BUZZER             true    // Buzzer alarm
#define USE_RED_LEDS           true    // Red LED flashing
#define USE_IR_SENSORS         true    // IR sensor triggers
#define USE_LASER_SERVO        false   // Optional: laser targeting

// ====== PIN DEFINITIONS ======
#define trigPin 9
#define echoPin 10
#define relayPin 7
#define buzzerPin 8
#define ir1Pin A0
#define ir2Pin A1

int redLEDs[] = {2, 3, 4};             // Flashing red LEDs
const int numRedLEDs = sizeof(redLEDs) / sizeof(redLEDs[0]);

int cityLEDs[] = {5, 6, 11};           // City lights (can be relayed)
const int numCityLEDs = sizeof(cityLEDs) / sizeof(cityLEDs[0]);

#define ultrasonicServoPin 6           // Servo for ultrasonic scanner
#define laserServoPanPin 3             // Optional laser panning
#define laserServoTiltPin 5            // Optional laser tilt

// ====== GLOBALS ======
Servo ultrasonicServo;
Servo laserServoPan;
Servo laserServoTilt;

long duration;
int distance;
bool alertActive = false;

unsigned long lastDetectionTime = 0;
const unsigned long blackoutDuration = 5000; // 5 sec active time

int scanStartAngle = 0;
int scanEndAngle = 180;
int scanStep = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("🛠️ Sanyam Suyal's Drone Defense System Booting...");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  if (USE_RELAY_BLACKOUT) pinMode(relayPin, OUTPUT);
  if (USE_BUZZER) pinMode(buzzerPin, OUTPUT);
  if (USE_IR_SENSORS) {
    pinMode(ir1Pin, INPUT);
    pinMode(ir2Pin, INPUT);
  }

  if (USE_RED_LEDS) {
    for (int i = 0; i < numRedLEDs; i++) {
      pinMode(redLEDs[i], OUTPUT);
      digitalWrite(redLEDs[i], LOW);
    }
  }

  for (int i = 0; i < numCityLEDs; i++) {
    pinMode(cityLEDs[i], OUTPUT);
    digitalWrite(cityLEDs[i], HIGH); // City lights ON
  }

  if (USE_ULTRASONIC_SERVO) {
    ultrasonicServo.attach(ultrasonicServoPin);
    ultrasonicServo.write(scanStartAngle);
  }

  if (USE_LASER_SERVO) {
    laserServoPan.attach(laserServoPanPin);
    laserServoTilt.attach(laserServoTiltPin);
    laserServoPan.write(90);
    laserServoTilt.write(90);
  }

  if (USE_RELAY_BLACKOUT) digitalWrite(relayPin, LOW);
  if (USE_BUZZER) digitalWrite(buzzerPin, LOW);
}

void loop() {
  bool detected = false;
  int detectedDistance = 999;

  // ======= Ultrasonic Detection, with "not triggered" log =======
  if (USE_ULTRASONIC_SERVO) {
    detectedDistance = scanWithUltrasonicServo();
    if (detectedDistance < 50) {
      detected = true;
      Serial.print("🔍 Detected at ");
      Serial.print(detectedDistance);
      Serial.println(" cm");
    } else {
      Serial.println("✅ US not triggered");
    }
  } else {
    detectedDistance = readUltrasonicDistance();
    if (detectedDistance < 50) {
      detected = true;
      Serial.println("🔍 Static Detection Triggered!");
    } else {
      Serial.println("✅ US not triggered");
    }
  }

  // ======= IR Sensor Detection, with "not triggered" log =======
  if (USE_IR_SENSORS) {
    if (digitalRead(ir1Pin) == LOW || digitalRead(ir2Pin) == LOW) {
      detected = true;
      Serial.println("⚠️ IR Sensor Triggered!");
    } else {
      Serial.println("✅ IR Sensor not triggered");
    }
  }

  if (detected) {
    if (!alertActive) {
      alertActive = true;
      lastDetectionTime = millis();
      startAlert();
    }
    if (USE_LASER_SERVO) aimLaserAtDistance(detectedDistance);
  } else if (alertActive && millis() - lastDetectionTime > blackoutDuration) {
    alertActive = false;
    stopAlert();
    if (USE_LASER_SERVO) resetLaser();
  }

  if (alertActive && USE_RED_LEDS) flashRedLEDs();
  else if (USE_RED_LEDS) turnOffRedLEDs();

  delay(50);
}

// === MODULES ===

int scanWithUltrasonicServo() {
  int closestDistance = 999;
  int closestAngle = scanStartAngle;

  for (int angle = scanStartAngle; angle <= scanEndAngle; angle += scanStep) {
    ultrasonicServo.write(angle);
    delay(300);
    int d = readUltrasonicDistance();
    if (d > 0 && d < closestDistance) {
      closestDistance = d;
      closestAngle = angle;
    }
  }

  ultrasonicServo.write(closestAngle);
  return closestDistance;
}

int readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void startAlert() {
  Serial.println("🚨 ALERT: Activating BLACKOUT Mode");

  if (USE_RELAY_BLACKOUT) digitalWrite(relayPin, HIGH); // blackout relay ON
  if (USE_BUZZER) digitalWrite(buzzerPin, HIGH);

  for (int i = 0; i < numCityLEDs; i++) {
    digitalWrite(cityLEDs[i], LOW);  // Turn OFF white lights
  }
}

void stopAlert() {
  Serial.println("✅ STATUS: All Clear. Resuming City Lights.");

  if (USE_RELAY_BLACKOUT) digitalWrite(relayPin, LOW);
  if (USE_BUZZER) digitalWrite(buzzerPin, LOW);

  for (int i = 0; i < numCityLEDs; i++) {
    digitalWrite(cityLEDs[i], HIGH); // Turn ON city lights
  }
}

void flashRedLEDs() {
  static unsigned long lastFlash = 0;
  static bool ledState = false;
  unsigned long now = millis();

  if (now - lastFlash > 300) {
    ledState = !ledState;
    for (int i = 0; i < numRedLEDs; i++) {
      digitalWrite(redLEDs[i], ledState ? HIGH : LOW);
    }
    lastFlash = now;
  }
}

void turnOffRedLEDs() {
  for (int i = 0; i < numRedLEDs; i++) {
    digitalWrite(redLEDs[i], LOW);
  }
}

void aimLaserAtDistance(int distance) {
  int tiltAngle = map(distance, 10, 50, 120, 60);
  tiltAngle = constrain(tiltAngle, 60, 120);
  laserServoTilt.write(tiltAngle);

  static int panAngle = 60;
  static bool increasing = true;
  if (increasing) {
    panAngle++;
    if (panAngle > 120) increasing = false;
  } else {
    panAngle--;
    if (panAngle < 60) increasing = true;
  }
  laserServoPan.write(panAngle);
}

void resetLaser() {
  laserServoPan.write(90);
  laserServoTilt.write(90);
}
