//// HASP 2022 TESTING CODE ////

#include "HASP2022_SDCard.h"
#include "HASP2022_PMT.h"
#include "HASP2022_Temp.h"

#define PIN_CS 10

bool SlowSaveMode = false;
bool SDCheckMode = false;
int LineLimit = 1000000;
int PMTHitThreshold = 60;
int TimeOffset = 0;
String FileName = "COC_HASP2022_DataFile_";
String FileExt = ".txt";

File CurrFile;
int FilesNum = 0;
int CurrLines = 0;
bool SDOpen = false;
bool FileOpen = false;
bool NeedNewFile = true;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_CS, OUTPUT);
  if (!SDCheckMode) OpenSD();
}

void loop() {
  if ((SDCheckMode) && (!SDOpen))
  {
    OpenSD();
    if (!SDOpen) return;
  }

  int currPMTHit = 0;
  int currTemp = 0;
  currPMTHit = SimulatePMT();
  currTemp = SimulateThermistor();
  if (currPMTHit > PMTHitThreshold)
  {
    String data = String(currPMTHit);
    data.concat("\t- ");
    data.concat(String(currTemp));
    SaveData(data);
  }
}
