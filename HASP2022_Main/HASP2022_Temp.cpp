#include "HASP2022_Temp.h"

extern int PINS_TEMP_INSIDE[];
extern int PINS_TEMP_OUTSIDE[];

extern long ThermResistance;

long ReadTemp(int tempType) {
  long temp = 0;
  
  if (tempType == 0)
    temp = GetTemp(PINS_TEMP_INSIDE);
  else if (tempType == 1)
    temp = GetTemp(PINS_TEMP_OUTSIDE);

  return temp;
}

//"GetTemp" adaptation from: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
long GetTemp(int PINS[2]) {
  int Vo;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  long tempSum = 0;
  
  for (int i = 0; i < 2; i++)
  {
    Vo = analogRead(PINS[i]);
    R2 = ThermResistance * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    T = T - 273.15;
    tempSum += T;
  }

  return tempSum / 2;
}
