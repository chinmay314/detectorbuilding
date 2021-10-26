#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
#pragma region constants
const int tPin = A0;
const int sPin = A5;
const double vRef = 5.0;
const int r1 = 10000;
const int R = 3;
const int G = 5;
const int B = 6;
const int fade = 10;
#pragma endregion
#pragma region variables
bool ROn = false;
bool GOn = false;
bool BOn = false;
int Gval = 255;
int Rval = 255;
int Bval = 255;
int time = 0;
int fadeTime = 0;
long salV = 0;
long tempV = 0;
double temp;
#pragma endregion

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
  pinMode(sPin, INPUT);
  pinMode(tPin, INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
}

void loop()
{
  time = millis();
  fadeTime = time + 1000;
  lcd.setCursor(0, 0);
  salV = analogRead(sPin);

  int count = 0;
  while (time < fadeTime) //fade code
  {
    tempV = tempV + analogRead(tPin);
    if (ROn)
    { //LED fade code
      if (Rval > 5)
      {
        Rval = Rval - fade;
      }
    }
    else
    {
      if (Rval < 255)
      {
        Rval = Rval + fade;
      }
    }
    if (GOn)
    {
      if (Gval > 5)
      {
        Gval = Gval - fade;
      }
    }
    else
    {
      if (Gval < 255)
      {
        Gval = Gval + fade;
      }
    }
    if (BOn)
    {
      if (Bval > 5)
      {
        Bval = Bval - fade;
      }
    }
    else
    {
      if (Bval < 255)
      {
        Bval = Bval + fade;
      }
    }

    analogWrite(G, Gval);
    analogWrite(R, Rval);
    analogWrite(B, Bval);
    delay(10);
    count = count + 1;
    time = millis();
  }

  double Vout = calculateVoltage(salV);
  tempV = tempV / count; //average reads
  temp = calculateVoltage(tempV);
  Serial.print("Voltage = ");
  Serial.print(Vout);
  Serial.print(", ");
  Serial.print("Salinity (ppm) = ");
  float salinity = 1 / ((0.002006061 * Vout) - 0.002220208);
  Serial.println(salinity);

  // lcd.print("Voltage = ");
  // lcd.print(calculateVoltage(salV));
  // lcd.setCursor(0, 2);
  // lcd.print("TempV = ");
  // lcd.print(temp);
  lcd.print(salinity);
  lcd.print(" ppm");

  if (calculateVoltage(salV) > 3.33)
  { //hot margin in C
    ROn = true;
    GOn = false;
    BOn = false;
  }
  else if (calculateVoltage(salV) < 1.66)
  { //cold margin in C
    ROn = false;
    GOn = false;
    BOn = true;
  }
  else
  {
    ROn = false;
    GOn = true;
    BOn = false;
  }
}
