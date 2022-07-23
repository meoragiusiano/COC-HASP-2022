#ifndef _HASP2022_Downlink_h_
#define _HASP2022_Downlink_h_

#include "Arduino.h"

void CheckDownlink();
void SendDownlink(String);
String ByteString(int, int);

#endif
