#include "HASP2022_Temp.h"

extern int PINS_TEMP_INSIDE[];
extern int PINS_TEMP_OUTSIDE[];

extern long ThermResistance;

long ReadTemp(int tempType) {
  long temp = 0;

  if (tempType == 0)
    temp = GetTemp(PINS_TEMP_INSIDE[0]);
  else if (tempType == 1)
    temp = GetTemp(PINS_TEMP_INSIDE[1]);
  else if (tempType == 2)
    temp = GetTemp(PINS_TEMP_OUTSIDE[0]);
  else if (tempType == 3)
    temp = GetTemp(PINS_TEMP_OUTSIDE[1]);

  return temp;
}

//"GetTemp" adaptation from: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
long GetTemp(int PIN) {
  int Vo;
  float logR2, R2;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  long temp = 0;

  Vo = analogRead(PIN);
  R2 = ThermResistance * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));

  return temp;
}
