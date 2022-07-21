#include "HASP2022_PMT.h"

extern int PIN_PMT;

int ReadPMTDigital() {
  return digitalRead(PIN_PMT);
}

int ReadPMTAnalog() {
  return analogRead(PIN_PMT);
}
