#include "HASP2022_PMT.h"

extern int PIN_PMT;

int ReadPMT() {
  return analogRead(PIN_PMT);
}
