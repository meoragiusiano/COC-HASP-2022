#include "HASP2022_Temp.h"

extern int PINS_TEMP_INSIDE[];
//extern int PINS_TEMP_OUTSIDE[];
extern int NumInTempPins;
//extern int NumOutTempPins;

extern long ThermResistance;

//"ReadTemp" adaptation from: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
long ReadTemp(int tempType) {
  int Vo;
  float logR2, R2, T;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  long tempSum = 0;

  if (tempType == 0)
  {
    for (int i = 0; i < NumInTempPins; i++) 
    {                     
      Vo = analogRead(PINS_TEMP_INSIDE[i]);
      R2 = ThermResistance * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
      T = T - 273.15;
      tempSum += (T * 9.0) / 5.0 + 32.0;
    }
  }
  else if (tempType == 1)
  {
    /*for (int i = 0; i < NumOutTempPins; i++) 
    {                     
      Vo = analogRead(PINS_TEMP_OUTSIDE[i]);
      R2 = ThermResistance * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
      T = T - 273.15;
      tempSum += (T * 9.0) / 5.0 + 32.0;
    }*/
  }

  return tempSum / NumInTempPins;
}

long SimulateThermistor() {
  return random(50, 200);
}
