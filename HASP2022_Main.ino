//// HASP 2022 TESTING CODE ////
//Authors:
//Svarun Soda

#include "HASP2022_SDCard.h"
#include "HASP2022_PMT.h"
#include "HASP2022_Temp.h"

//Pin defined for "Chip Select" on Teensy 4.0
#define PIN_CS 10
//DEFAULT: 10
//Pin defined for PMT analog input on Teensy 4.0
#define PIN_PMT 20
//DEFAULT: 20

//Enables SlowSave mode
//FALSE (DEFAULT) - Program will only save the currently-opened data file after it crosses the Line Limit (faster)
//TRUE - Program will save the currently-opened data file after every iteration (slower)
bool SlowSaveMode = false;
//Enables SDCheck mode
//FALSE (DEFAULT) - Program will open the SD card only in the beggining, will never check whether the SD is actually connected, and will just keep attempting to save to it regardless
//TRUE - Program will check whether the SD is still connected every time the currently-opened data file is saved, will halt the saving process if it detects the SD is disconnected, and resume once it gets recconected
bool SDCheckMode = false;
//Defines the line limit for each data file
//DEFAULT: 1000000
int LineLimit = 1000000;
//Defines the threshold above which the data from the PMT will be written to the data file
int PMTHitThreshold = 60;
//Increments the timestamp in the data files by this amnount
int TimeOffset = 0;
//Prefix names of the saved data files  ([prefix + file number + suffix] HAS to be smaller than the char "buff" present in the saving function!)
String FileName = "COC_HASP2022_DataFile_";
//Suffix names of the saved data files
//DEFAULT: ".txt"
String FileExt = ".txt";

//Rest of global variables (do not change)
File CurrFile;
int FilesNum = 0;
int CurrLines = 0;
bool SDOpen = false;
bool FileOpen = false;
bool NeedNewFile = true;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_PMT, INPUT);
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
  //currPMTHit = SimulatePMT();
  currPMTHit = ReadPMT(PIN_PMT);
  currTemp = SimulateThermistor();
  if (currPMTHit > PMTHitThreshold)
  {
    String data = String(currPMTHit);
    data.concat("\t- ");
    data.concat(String(currTemp));
    SaveData(data);
  }
}
