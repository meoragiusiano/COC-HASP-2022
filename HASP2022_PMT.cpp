#include "HASP2022_PMT.h"

int ReadPMT(const int pin) {
  return analogRead(pin);
}

int SimulatePMT() {
  return random(5, 100);
}
