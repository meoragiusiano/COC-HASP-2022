#include "HASP2022_Downlink.h"

void SendDownlink(String data) {
  String dataPrint = "DOWNLINK: ";
  dataPrint.concat(data);
  
  Serial.print(dataPrint);
  Serial5.print(dataPrint);
}
