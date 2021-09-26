#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int vOutPin = A5;
const double vRef = 5.0;
const int r1 = 10000;

double calculateVoltage(int vDigital)
{
  return vDigital * 5.0 / 1024;
}

double calculateResistance(int vDigital)
{
  double vAnalog = vDigital * 5.0 / 1024;
  return vAnalog * r1 / (vRef - vAnalog);
}

void setup()
{
  pinMode(vOutPin, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
}

void loop()
{
  lcd.setCursor(0, 0);
  int vOut = analogRead(vOutPin);
  Serial.print(vOut);
  Serial.print(", ");
  Serial.print(calculateVoltage(vOut));
  Serial.print(", ");
  Serial.println(calculateResistance(vOut));
  lcd.print("Voltage = ");
  lcd.print(calculateVoltage(vOut));
  delay(1000);
}
