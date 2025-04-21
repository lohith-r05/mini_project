#include <Servo.h>  // Servo control

const int potPin = A0;               // Potentiometer
const int pwmPin = 5;                // Buck control pin
const int selectionPin = 2;          // Relay/manual switch input
const int boostPwmPin = 3;           // Boost PWM pin (OC2B)
const int escPin = 9;                // ESC signal
const int voltageSensorPin = A1;     // Voltage sensor input
const int currentSensorPin = A2;     // Current sensor input

Servo esc;  // ESC object

void setup() {
  Serial.begin(9600);  // Serial monitor
  pinMode(pwmPin, OUTPUT);
  pinMode(selectionPin, INPUT);
  pinMode(boostPwmPin, OUTPUT);

  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  // Fast PWM
  TCCR2B = _BV(WGM22) | _BV(CS20);                 // No prescaler
  OCR2A = 159;  // 100kHz
  OCR2B = 40;   // 25% duty cycle

  esc.attach(escPin);  // ESC setup
}

void loop() {
  int rawVoltage = analogRead(voltageSensorPin);  // Read voltage sensor
  float voltage = rawVoltage * (5.0 / 1023.0);     // Convert to volts

  int rawCurrent = analogRead(currentSensorPin);  // Read current sensor
  float current = rawCurrent * (5.0 / 1023.0);     // Convert to amps

  int potValue = analogRead(potPin);  // Read potentiometer
  int escSpeed = map(potValue, 0, 1023, 1000, 2000);
  esc.writeMicroseconds(escSpeed);  // Control ESC

  if (voltage < 1.8 || current > 2.0) {  // Boost condition
    for (int i = 0; i < 500; i++) {
      digitalWrite(pwmPin, HIGH);
      delayMicroseconds(9500);
      digitalWrite(pwmPin, LOW);
      delayMicroseconds(500);
    }
  } else {  // Buck condition
    int onTime = map(potValue, 0, 1023, 1, 8);     // ON time in milliseconds
    int offTime = 10 - onTime;                    // OFF time in milliseconds

    digitalWrite(pwmPin, HIGH);
    delay(onTime);
    digitalWrite(pwmPin, LOW);
    delay(offTime);
  }
}
