//project 1 - RGB color sensor
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Create an instance of the TCS34725 sensor
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup() {
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

  // Check if there is significant color detected
  if (c > 50) { // Adjust the threshold as needed
    // Normalize the values
    float sum = c;
    float red = r / sum;
    float green = g / sum;
    float blue = b / sum;

    Serial.print("R: "); Serial.print(r); Serial.print(" ");
    Serial.print("G: "); Serial.print(g); Serial.print(" ");
    Serial.print("B: "); Serial.print(b); Serial.print(" ");
    Serial.print("C: "); Serial.print(c); Serial.println(" ");

    // Calculate the color temperature and lux
    uint16_t colorTemp = tcs.calculateColorTemperature(r, g, b);
    uint16_t lux = tcs.calculateLux(r, g, b);
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.println(lux, DEC);
  } // <-- MISSING CLOSING BRACE (Fixed)

  // Add a small delay before the next reading
  delay(2000);
}


