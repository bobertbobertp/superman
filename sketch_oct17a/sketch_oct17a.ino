// Define Motor Control Pins (Change these to match your actual wiring)
const int motorA_In1 = 4;
const int motorA_In2 = 5;
const int motorB_In1 = 6;
const int motorB_In2 = 7;

// PWM Properties
const int pwmFreq = 5000;    // 5 kHz frequency
const int pwmResolution = 8; // 8-bit resolution (values from 0 to 255)

// Define the requested speed steps (0.1 to 1.0)
const float speeds[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
const int numSpeeds = sizeof(speeds) / sizeof(speeds[0]);

void setup() {
  Serial.begin(9600);

  // Configure Motor Pins as PWM Outputs using newer ESP32 LEDC API
  ledcAttach(motorA_In1, pwmFreq, pwmResolution);
  ledcAttach(motorA_In2, pwmFreq, pwmResolution);
  ledcAttach(motorB_In1, pwmFreq, pwmResolution);
  ledcAttach(motorB_In2, pwmFreq, pwmResolution);
}

void loop() {
  // Step through each speed forward
  for (int i = 0; i < numSpeeds; i++) {
    float currentSpeed = speeds[i];
    Serial.print("Setting motor speed to: ");
    Serial.println(currentSpeed);

    setMotorSpeedForward(currentSpeed);
    
    delay(2000); // Run at this speed for 2 seconds
  }

  // Stop motors for 3 seconds before repeating
  Serial.println("Stopping motors...");
  stopMotors();
  delay(3000);
}

// Function to control speed (0.0 to 1.0) moving Forward
void setMotorSpeedForward(float speed) {
  // Constrain speed between 0.0 and 1.0 for safety
  speed = constrain(speed, 0.0, 1.0);

  // Map the 0.0 - 1.0 float value to an 8-bit PWM duty cycle (0 - 255)
  int pwmValue = (int)(speed * 255);

  // Motor A Forward: IN1 gets PWM, IN2 gets LOW
  ledcWrite(motorA_In1, pwmValue);
  ledcWrite(motorA_In2, 0);

  // Motor B Forward: IN3 gets PWM, IN4 gets LOW
  ledcWrite(motorB_In1, pwmValue);
  ledcWrite(motorB_In2, 0);
}

// Function to stop both motors
void stopMotors() {
  ledcWrite(motorA_In1, 0);
  ledcWrite(motorA_In2, 0);
  ledcWrite(motorB_In1, 0);
  ledcWrite(motorB_In2, 0);
}