/*
 * Haptic echolocation system
 * The closer the obstacle, the faster the vibrations
 * Inspired by the echolocation system of bats
 */

// === PIN CONFIGURATION ===
#define TRIG_PIN 2      // HC-SR04 Trigger Pin
#define ECHO_PIN 3      // HC-SR04 Echo Pin
#define VIBRATION_PIN 4 // Vibrator motor control pin

// === DISTANCE PARAMETERS ===
const int MIN_DISTANCE = 5;     // Minimum distance in cm
const int MAX_DISTANCE = 150;   // Maximum distance in cm
const int MEASUREMENT_INTERVAL = 30; // Time between each measurement (ms)

// === VIBRATION PARAMETERS ===
const int MIN_VIBRATION_INTERVAL = 20;   // Minimum interval between vibrations (ms)
const int MAX_VIBRATION_INTERVAL = 1000; // Maximum interval between vibrations (ms)
const int MIN_VIBRATION_DURATION = 15; // Minimum vibration duration (ms)
const int MAX_VIBRATION_DURATION = 80; // Maximum vibration duration (ms)

// === GLOBAL VARIABLES ===
unsigned long lastMeasurement = 0;
unsigned long lastVibration = 0;
unsigned long vibrationStart = 0;
bool isVibrating = false;

int distance = 0;
int vibrationInterval = 0;
int vibrationDuration = 0;

void setup() {
  // Pin configuration
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  // Serial communication initialization
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("   HAPTIC ECHOLOCATION ACTIVE");
  Serial.println("=================================");
  Serial.println("Distance | Interval | Duration");
  Serial.println("---------|----------|---------");

  // Startup signal: 3 short vibrations
  for (int i = 0; i < 3; i++) {
    digitalWrite(VIBRATION_PIN, HIGH);
    delay(100);
    digitalWrite(VIBRATION_PIN, LOW);
    delay(100);
  }

  Serial.println("System ready!");
}

void loop() {
  // Measure distance at regular intervals
  if (millis() - lastMeasurement >= MEASUREMENT_INTERVAL) {
    distance = measureDistance();
    calculateVibrationParameters();
    displayInformation();
    lastMeasurement = millis();
  }

  // Manage vibrations
  manageVibrations();
}

int measureDistance() {
  // Send an ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo return time
  long duration = pulseIn(ECHO_PIN, HIGH, 25000); // Timeout 25ms

  // Calculate the distance (speed of sound = 343 m/s)
  int dist = duration * 0.034 / 2;

  // Filter out outlier values
  if (dist <= 0 || dist > MAX_DISTANCE) {
    dist = MAX_DISTANCE;
  }
  if (dist < MIN_DISTANCE) {
    dist = MIN_DISTANCE;
  }

  return dist;
}

void calculateVibrationParameters() {
  // Calculate the interval: closer = faster vibrations
  vibrationInterval = map(distance, MIN_DISTANCE, MAX_DISTANCE,
                          MIN_VIBRATION_INTERVAL, MAX_VIBRATION_INTERVAL);

  // Calculate the duration: closer = longer vibrations
  vibrationDuration = map(distance, MIN_DISTANCE, MAX_DISTANCE,
                      MAX_VIBRATION_DURATION, MIN_VIBRATION_DURATION);

  // Ensure consistent values
  vibrationInterval = constrain(vibrationInterval, MIN_VIBRATION_INTERVAL, MAX_VIBRATION_INTERVAL);
  vibrationDuration = constrain(vibrationDuration, MIN_VIBRATION_DURATION, MAX_VIBRATION_DURATION);
}

void manageVibrations() {
  unsigned long currentTime = millis();

  if (!isVibrating) {
    // Start a new vibration if the interval has passed
    if (currentTime - lastVibration >= vibrationInterval) {
      digitalWrite(VIBRATION_PIN, HIGH);
      isVibrating = true;
      vibrationStart = currentTime;
      lastVibration = currentTime;
    }
  } else {
    // Stop the vibration if the duration has passed
    if (currentTime - vibrationStart >= vibrationDuration) {
      digitalWrite(VIBRATION_PIN, LOW);
      isVibrating = false;
    }
  }
}

void displayInformation() {
  // Formatted display of information
  Serial.print(distance);
  Serial.print(" cm      | ");
  Serial.print(vibrationInterval);
  Serial.print(" ms      | ");
  Serial.print(vibrationDuration);
  Serial.print(" ms");

  // Visual proximity indicator
  if (distance <= 10) {
    Serial.println("  [VERY CLOSE!]");
  } else if (distance <= 30) {
    Serial.println("  [CLOSE]");
  } else if (distance <= 60) {
    Serial.println("  [MEDIUM]");
  } else {
    Serial.println("  [FAR]");
  }
}

/*
 * === OPERATING LOGIC ===
 *
 * Distance 5cm     → Vibrate every 20ms for 80ms (very fast)
 * Distance 30cm    → Vibrate every 200ms for 50ms (fast)
 * Distance 60cm    → Vibrate every 500ms for 30ms (medium)
 * Distance 150cm   → Vibrate every 1000ms for 15ms (slow)
 *
 * The closer the obstacle, the more the vibrations are:
 * - Frequent (short interval)
 * - Intense (long duration)
 */