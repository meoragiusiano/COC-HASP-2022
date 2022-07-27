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
  int v_cum = 0;
  for (int i = 0; i < 2; i++)
  {
    v_cum += analogRead(PINS[i]);
  }

  return v_cum / 2.0;
}
