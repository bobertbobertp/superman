// Define Motor Control Pins (Optimized for your ESP32-S3 layout)
const int motorA_In1 = 11;
const int motorA_In2 = 12;
const int motorB_In1 = 13;
const int motorB_In2 = 14;

// 🛠️ DIRECTION CALIBRATION SETTINGS
// If a motor spins backward when it should go forward, change its setting to true!
const bool INVERT_MOTOR_A = false; 
const bool INVERT_MOTOR_B = false; 

// PWM Properties
const int pwmFreq = 5000;    // 5 kHz frequency
const int pwmResolution = 8; // 8-bit resolution (values from 0 to 255)

// Refined speed steps (Starting at 0.3 so yellow motors don't stall or hum)
const float speeds[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const int numSpeeds = sizeof(speeds) / sizeof(speeds[0]); // FIXED: Now counts all 8 steps correctly

void setup() {
  Serial.begin(9600);

  // Configure Motor Pins as PWM Outputs using newer ESP32 LEDC API
  ledcAttach(motorA_In1, pwmFreq, pwmResolution);
  ledcAttach(motorA_In2, pwmFreq, pwmResolution);
  ledcAttach(motorB_In1, pwmFreq, pwmResolution);
  ledcAttach(motorB_In2, pwmFreq, pwmResolution);

  // Safety initialize: Make sure motors start completely stopped
  stopMotors();
}

void loop() {
  // 1. Step through each speed Forward
  Serial.println("--- Testing Forward Movement ---");
  for (int i = 0; i < numSpeeds; i++) {
    float currentSpeed = speeds[i];
    Serial.print("Forward Speed: ");
    Serial.println(currentSpeed);
    setMotorSpeedForward(currentSpeed);
    delay(1500); 
  }

  // Safe transition break before switching directions
  Serial.println("Braking...");
  stopMotors();
  delay(1000);

  // 2. Test Turning Right (Motor A Forward, Motor B Backward)
  Serial.println("--- Testing Sharp Right Turn ---");
  turnRight(0.6); 
  delay(2000);

  Serial.println("Braking...");
  stopMotors();
  delay(1000);

  // 3. Step through each speed Backward
  Serial.println("--- Testing Backward Movement ---");
  for (int i = 0; i < numSpeeds; i++) {
    float currentSpeed = speeds[i];
    Serial.print("Backward Speed: ");
    Serial.println(currentSpeed);
    setMotorSpeedBackward(currentSpeed);
    delay(1500);
  }

  // Stop before restarting the routine
  Serial.println("Routine complete. Sleeping for 3 seconds...\n");
  stopMotors();
  delay(3000);
}

// ================= MOTOR MOVEMENT FUNCTIONS =================

// Move Forward: Both motors spin forward
void setMotorSpeedForward(float speed) {
  speed = constrain(speed, 0.0, 1.0);
  int pwmValue = (int)(speed * 255);

  // Handle Motor A Forward / Inverted Forward
  if (!INVERT_MOTOR_A) {
    ledcWrite(motorA_In1, pwmValue);
    ledcWrite(motorA_In2, 0);
  } else {
    ledcWrite(motorA_In1, 0);
    ledcWrite(motorA_In2, pwmValue);
  }

  // Handle Motor B Forward / Inverted Forward
  if (!INVERT_MOTOR_B) {
    ledcWrite(motorB_In1, pwmValue);
    ledcWrite(motorB_In2, 0);
  } else {
    ledcWrite(motorB_In1, 0);
    ledcWrite(motorB_In2, pwmValue);
  }
}

// Move Backward: Both motors reverse
void setMotorSpeedBackward(float speed) {
  speed = constrain(speed, 0.0, 1.0);
  int pwmValue = (int)(speed * 255);

  // Handle Motor A Backward / Inverted Backward
  if (!INVERT_MOTOR_A) {
    ledcWrite(motorA_In1, 0);
    ledcWrite(motorA_In2, pwmValue);
  } else {
    ledcWrite(motorA_In1, pwmValue);
    ledcWrite(motorA_In2, 0);
  }

  // Handle Motor B Backward / Inverted Backward
  if (!INVERT_MOTOR_B) {
    ledcWrite(motorB_In1, 0);
    ledcWrite(motorB_In2, pwmValue);
  } else {
    ledcWrite(motorB_In1, pwmValue);
    ledcWrite(motorB_In2, 0);
  }
}

// Turn Right: Motor A goes forward, Motor B goes backward
void turnRight(float speed) {
  speed = constrain(speed, 0.0, 1.0);
  int pwmValue = (int)(speed * 255);

  // Motor A Forward
  if (!INVERT_MOTOR_A) {
    ledcWrite(motorA_In1, pwmValue);
    ledcWrite(motorA_In2, 0);
  } else {
    ledcWrite(motorA_In1, 0);
    ledcWrite(motorA_In2, pwmValue);
  }

  // Motor B Backward
  if (!INVERT_MOTOR_B) {
    ledcWrite(motorB_In1, 0);
    ledcWrite(motorB_In2, pwmValue);
  } else {
    ledcWrite(motorB_In1, pwmValue);
    ledcWrite(motorB_In2, 0);
  }
}

// Turn Left: Motor A goes backward, Motor B goes forward
void turnLeft(float speed) {
  speed = constrain(speed, 0.0, 1.0);
  int pwmValue = (int)(speed * 255);

  // Motor A Backward
  if (!INVERT_MOTOR_A) {
    ledcWrite(motorA_In1, 0);
    ledcWrite(motorA_In2, pwmValue);
  } else {
    ledcWrite(motorA_In1, pwmValue);
    ledcWrite(motorA_In2, 0);
  }

  // Motor B Forward
  if (!INVERT_MOTOR_B) {
    ledcWrite(motorB_In1, pwmValue);
    ledcWrite(motorB_In2, 0);
  } else {
    ledcWrite(motorB_In1, 0);
    ledcWrite(motorB_In2, pwmValue);
  }
}

// Stop: Cuts all voltage to the pins immediately
void stopMotors() {
  ledcWrite(motorA_In1, 0);
  ledcWrite(motorA_In2, 0);
  ledcWrite(motorB_In1, 0);
  ledcWrite(motorB_In2, 0);
}