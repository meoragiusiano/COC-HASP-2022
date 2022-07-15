#ifndef _HASP2022_SDCard_h_
#define _HASP2022_SDCard_h_

#include "Arduino.h"
#include <SD.h>

void SaveData(String);
void WriteDataFile(String);
void OpenDataFile();
void CloseDataFile();
bool CheckSD();
void OpenSD();
void PrintSDStatus();

#endif
