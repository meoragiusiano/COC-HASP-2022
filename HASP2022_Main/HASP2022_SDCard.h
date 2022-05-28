#ifndef _HASP2022_SDCard_h_
#define _HASP2022_SDCard_h_

#include "Arduino.h"
#include <SdFat.h>

void SaveData(String);
void WriteDataFile(String);
void OpenDataFile();
void CloseDataFile();
void OpenSD();

#endif
