# ColorSorterProjectCode
EK210 color sorter project code
@'

// ColorSorter.ino — your full sorter loop (sensor → classify → servo → stepper → LCD)

#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <mwc_stepper.h>
#include <LiquidCrystal_I2C.h>

#define EN_PIN 3
#define DIR_PIN 2
#define STEP_PIN 5

#define RPM 60
#define PULSE 400
#define ClOCKWISE 1

MWCSTEPPER nema23(EN_PIN, DIR_PIN, STEP_PIN);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

Servo myservo;
int servoPin = 9;

int red = 0, ora = 0, green = 0, pur = 0, yel = 0;

char array1[] = "    Skittle Count:    ";
char arrayY[] = "Y = ";
char arrayR[] = " R = ";
char arrayO[] = "O = ";
char arrayG[] = "G = ";
char arrayP[] = " P = ";

LiquidCrystal_I2C lcd(0x27, 20, 4); // change if your LCD address differs

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print(array1);

  lcd.setCursor(0,1); lcd.print(arrayY); lcd.print(yel);
  lcd.setCursor(0,2); lcd.print(arrayO); lcd.print(ora); lcd.print(arrayR); lcd.print(red);
  lcd.setCursor(0,3); lcd.print(arrayG); lcd.print(green); lcd.print(arrayP); lcd.print(pur);

  nema23.init();
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);

  if (!tcs.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  } else {
    Serial.println("Found TCS34725 sensor");
  }
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.print(b);
  Serial.print(" C: "); Serial.println(c);

  lcd.setCursor(0,1); lcd.print(arrayY); lcd.print(yel);
  lcd.setCursor(0,2); lcd.print(arrayO); lcd.print(ora); lcd.print(arrayR); lcd.print(red);
  lcd.setCursor(0,3); lcd.print(arrayG); lcd.print(green); lcd.print(arrayP); lcd.print(pur);

  delay(2500);

  if (r > 1500 && r < 2000 && c >= 5700 && c <= 7500) {        // Red
    writeServo(0); red++; runOnce();
  } else if (r > b && c >= 8800 && c <= 11000) {               // Orange
    writeServo(25); ora++; runOnce();
  } else if (r > 3000 & r < 5000 && b > 2000 & b < 3300 && g > r && c >= 11000 && c <= 20000) { // Yellow
    writeServo(50); yel++; runOnce();
  } else if (r < b && c >= 8400 && c <= 11100) {               // Green
    writeServo(70); green++; runOnce();
  } else if (r > 1000 && r < 1500 && g > r && c >= 4800 && c < 6700) { // Purple
    writeServo(95); pur++; runOnce();
  } else {
    writeServo(50); runOnce();
  }

  delay(1000);
}

void runOnce() {
  delay(15);
  runStepper(ClOCKWISE, RPM, PULSE);
  delay(1000);
}

// SG90 PWM (software pulse)
void writeServo(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2500);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(20000 - pulseWidth);
}

void runStepper(int direction, int rpm, int pulse) {
  nema23.set(direction, rpm, pulse);
  for (size_t i = 0; i < pulse; i++) {
    nema23.run();
  }
}
'@ | Set-Content -Encoding utf8 .\firmware\ColorSorter.ino
