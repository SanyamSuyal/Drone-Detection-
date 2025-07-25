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
#define USE_ULTRASONIC_SERVO         true    // Servo radar scan for first sensor
#define USE_SECOND_ULTRASONIC_SERVO  true    // Enable second servo + ultrasonic sensor for 360 coverage
#define USE_RELAY_BLACKOUT           true    // Control blackout relay
#define USE_BUZZER                   true    // Buzzer alarm
#define USE_RED_LEDS                 true    // Red LED flashing
#define USE_IR_SENSORS               true    // IR sensor triggers
#define USE_LASER_SERVO              false   // Optional: laser targeting

// ====== PIN DEFINITIONS ======
#define trigPin1 9
#define echoPin1 10
#define servoPin1 6

#define trigPin2 12                  // New ultrasonic trig pin for second sensor
#define echoPin2 13                  // New ultrasonic echo pin for second sensor
#define servoPin2 11                 // New servo pin for second servo

#define relayPin 7
#define buzzerPin 8
#define ir1Pin A0
#define ir2Pin A1

int redLEDs[] = {2, 3, 4};             // Flashing red LEDs
const int numRedLEDs = sizeof(redLEDs) / sizeof(redLEDs[0]);

int cityLEDs[] = {5, 6, 11};           // City lights (can be relayed)
const int numCityLEDs = sizeof(cityLEDs) / sizeof(cityLEDs[0]);

#define laserServoPanPin 3             // Optional laser panning
#define laserServoTiltPin 5            // Optional laser tilt

// ====== GLOBALS ======
Servo ultrasonicServo1;
Servo ultrasonicServo2;                 // Second servo
Servo laserServoPan;
Servo laserServoTilt;

bool alertActive = false;

unsigned long lastDetectionTime = 0;
const unsigned long blackoutDuration = 5000; // 5 sec active time

// First servo scan config (0 - 180 degrees)
int scanStartAngle1 = 0;
int scanEndAngle1 = 180;
int scanStep1 = 10;

// Second servo scan config (180 - 360 degrees)
// Since servo physically rotates 0-180°, we'll map this to 180-360° for logic purposes
// Servo moves 0-180°, we treat that as 180-360 degrees sector
int scanStartAngle2 = 0;   // servo degrees, corresponds to 180 degrees logical sector start
int scanEndAngle2 = 180;   // servo degrees

// Function prototypes for second sensor scanning
int scanWithSecondUltrasonicServo();
int readUltrasonicDistance(int trigPin, int echoPin);

void setup() {
  Serial.begin(9600);
  Serial.println("🛠️ Sanyam Suyal's Drone Defense System Booting...");

  // First ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  // Second ultrasonic sensor pins (new)
  if (USE_SECOND_ULTRASONIC_SERVO) {
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
  }

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
    ultrasonicServo1.attach(servoPin1);
    ultrasonicServo1.write(scanStartAngle1);
  }

  if (USE_SECOND_ULTRASONIC_SERVO) {
    ultrasonicServo2.attach(servoPin2);
    ultrasonicServo2.write(scanStartAngle2);
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

  int detectedDistance1 = 999;
  int detectedDistance2 = 999;

  // ==== First Ultrasonic Sensor Scan ====
  if (USE_ULTRASONIC_SERVO) {
    detectedDistance1 = scanWithUltrasonicServo();
    if (detectedDistance1 < 50) {
      detected = true;
      Serial.print("🔍 Sensor 1 Detected at ");
      Serial.print(detectedDistance1);
      Serial.println(" cm");
    } else {
      Serial.println("✅ Sensor 1 US not triggered");
    }
  } else {
    detectedDistance1 = readUltrasonicDistance(trigPin1, echoPin1);
    if (detectedDistance1 < 50) {
      detected = true;
      Serial.println("🔍 Sensor 1 Static Detection Triggered!");
    } else {
      Serial.println("✅ Sensor 1 US not triggered");
    }
  }

  // ==== Second Ultrasonic Sensor Scan (if enabled) ====
  if (USE_SECOND_ULTRASONIC_SERVO) {
    detectedDistance2 = scanWithSecondUltrasonicServo();
    if (detectedDistance2 < 50) {
      detected = true;
      Serial.print("🔍 Sensor 2 Detected at ");
      Serial.print(detectedDistance2);
      Serial.println(" cm");
    } else {
      Serial.println("✅ Sensor 2 US not triggered");
    }
  }

  // ==== IR Sensors ====
  if (USE_IR_SENSORS) {
    if (digitalRead(ir1Pin) == LOW || digitalRead(ir2Pin) == LOW) {
      detected = true;
      Serial.println("⚠️ IR Sensor Triggered!");
    } else {
      Serial.println("✅ IR Sensor not triggered");
    }
  }

  // ==== Determine closest distance from both sensors ====
  detectedDistance = (detectedDistance1 < detectedDistance2) ? detectedDistance1 : detectedDistance2;

  // ==== Alert Logic ====
  if (detected) {
    if (!alertActive) {
      alertActive = true;
      lastDetectionTime = millis();
      startAlert();
    }
    if (USE_LASER_SERVO) aimLaserAtDistance(detectedDistance);
  } 
  else if (alertActive && millis() - lastDetectionTime > blackoutDuration) {
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
  int closestAngle = scanStartAngle1;

  for (int angle = scanStartAngle1; angle <= scanEndAngle1; angle += scanStep1) {
    ultrasonicServo1.write(angle);
    delay(300);
    int d = readUltrasonicDistance(trigPin1, echoPin1);
    if (d > 0 && d < closestDistance) {
      closestDistance = d;
      closestAngle = angle;
    }
  }

  ultrasonicServo1.write(closestAngle);
  return closestDistance;
}

// Scanning for second servo + ultrasonic sensor (same logic as first, different pins)
int scanWithSecondUltrasonicServo() {
  int closestDistance = 999;
  int closestAngle = scanStartAngle2;

  for (int angle = scanStartAngle2; angle <= scanEndAngle2; angle += scanStep1) {
    ultrasonicServo2.write(angle);
    delay(300);
    int d = readUltrasonicDistance(trigPin2, echoPin2);
    if (d > 0 && d < closestDistance) {
      closestDistance = d;
      closestAngle = angle;
    }
  }

  ultrasonicServo2.write(closestAngle);
  return closestDistance;
}

// Generic ultrasonic reading function used for both sensors
int readUltrasonicDistance(int trigPin, int echoPin) {
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
    digitalWrite(cityLEDs[i], LOW);  // Turn OFF city lights
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
