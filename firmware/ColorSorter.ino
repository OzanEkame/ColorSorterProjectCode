//project 1 - RGB color sensor
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include<mwc_stepper.h>
#include <LiquidCrystal_I2C.h>

#define EN_PIN 3
#define DIR_PIN 2
#define STEP_PIN 5

#define RPM 60
#define PULSE 400
#define ClOCKWISE 1

MWCSTEPPER nema23(EN_PIN, DIR_PIN, STEP_PIN);

// Create an instance of the TCS34725 sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);


Servo myservo; 

int servoPin = 9;


int red = 0;
int ora = 0;
int green = 0;
int pur = 0;
int yel = 0;


char array1[] = "    Skittle Count:    ";  
char arrayY[] = "Y = " ;     
char arrayR[] = " R = " ;     
char arrayO[] = "O = " ;  
char arrayG[] = "G = "  ;
char arrayP[] = " P = "  ;

LiquidCrystal_I2C lcd(0x27, 20, 4); // CHANGE THE 0X27 ADDRESS TO YOUR SCREEN ADDRESS IF NEEDED

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print(array1); 

  lcd.setCursor(0,1);
  lcd.print(arrayY);
  lcd.print(yel);          

 
  lcd.setCursor(0,2);
  lcd.print(arrayO);
  lcd.print(ora);

   lcd.print(arrayR);
  lcd.print(red);


   lcd.setCursor(0,3);
  lcd.print(arrayG);
  lcd.print(green);

  lcd.print(arrayP);
  lcd.print(pur);

  nema23.init();
  pinMode( servoPin , OUTPUT);
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found TCS34725 sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt the program
  }
  
}

void loop() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  

  Serial.print("R: "); Serial.print(r);
    Serial.print(" G: "); Serial.print(g);
    Serial.print(" B: "); Serial.print(b);
    Serial.print(" C: "); Serial.println(c);
    
    lcd.setCursor(0,1);
    lcd.print(arrayY);
    lcd.print(yel);          

 
    lcd.setCursor(0,2);
    lcd.print(arrayO);
    lcd.print(ora);

    lcd.print(arrayR);
    lcd.print(red);


    lcd.setCursor(0,3);
    lcd.print(arrayG);
    lcd.print(green);

    lcd.print(arrayP);
    lcd.print(pur);

    delay (2500);

    

   
    // Red Color 
     if (r > 1500 && r < 2000 &&c >=5700 && c<= 7500) {
        writeServo(0);
        Serial.println("Red");
        red++;
        delay(15);
       runStepper(ClOCKWISE, RPM, PULSE);
  delay(1000);
    }

    // Orange Color 
    else if (r > b && c >=8800 && c<= 11000) {  // Fixed comparison
        writeServo(25);
        Serial.println("Orange");
        ora++;
        delay(15);
        runStepper(ClOCKWISE, RPM, PULSE);
  delay(1000);
    }

    // Yellow Color
    else if (r >3000 & r<5000 && b>2000 & b<3300 &&g>r &&  c >=11000 && c <=20000) {
        writeServo(50);
        Serial.println("Yellow");
        yel++;
        delay(15);
        runStepper(ClOCKWISE, RPM, PULSE);
  delay(1000);
    }

    // Green Color
    else if (r < b && c >=8400 && c<= 11100) {
        writeServo(70);
        Serial.println("Green");
        green++;
        delay(15);
        runStepper(ClOCKWISE, RPM, PULSE);
        delay(1000);
    }

    // Purple Color (Fixed condition)
    else if (r > 1000 && r < 1500 && g>r && c >=4800 && c< 6700) {
        writeServo(95);
        Serial.println("Purple");
        pur++;
        delay(15);
        runStepper(ClOCKWISE, RPM, PULSE);

  delay(1000);
    }
else {
  writeServo(50);
  delay(15);
  runStepper(ClOCKWISE, RPM, PULSE);
}
 
    delay(1000);  // Delay before next reading
}

// Function to generate PWM signal for SG90
void writeServo(int angle) {
 // Map angle (0 to 180) to pulse width (500 to 2500 microseconds)
 int pulseWidth = map(angle, 0,180, 500, 2500);
 // Send the PWM signal
 digitalWrite(servoPin, HIGH);
 delayMicroseconds(pulseWidth); // HIGH pulse duration based on angle
 digitalWrite(servoPin, LOW);

 // Wait for the rest of the 20ms period (20ms - pulseWidth)
 delayMicroseconds(20000 - pulseWidth);
}

void runStepper(int direction, int rpm, int pulse) {
  nema23.set(direction, rpm, pulse);
  for (size_t i = 0; i < pulse; i++) {
    nema23.run();
  }
}

