//// HASP 2022 TESTING CODE ////

#include "HASP2022_SDCard.h"
#include "HASP2022_PMT.h"
#include "HASP2022_Temp.h"

int PIN_CS = 10;  //Pin defined for "Chip Select" on Teensy 4.1
int PIN_PMT = 20; //Pin defined for PMT analog input on Teensy 4.1
int PINS_TEMP_INSIDE[] = {26, 27};
//int PINS_TEMP_OUTSIDE[] = {24, 25};
int NumInTempPins = 2;
//int NumOutTempPins = 2;

bool SlowSaveMode = false;
//FALSE (DEFAULT) - Program will only save the currently-opened data file after it crosses the Line Limit (faster)
//TRUE - Program will save the currently-opened data file after every iteration (slower)
bool SDCheckMode = false;
//FALSE (DEFAULT) - Program will open the SD card only in the beggining according to FirstSDCheck, will never check whether the SD is actually connected, and will just keep attempting to save to it regardless
//TRUE - Program will check whether the SD is still connected every time the currently-opened data file is saved, will halt the saving process if it detects the SD is disconnected, and resume once it gets recconected
bool FirstSDCheck = true;
//FALSE - At the start, program will only run begin() function of the SD card in the setup() function
//TRUE (DEFAULT) - At the start, program will keep trying to open the SD card until it is successfully opened
bool FileCheckMode = false;
//FALSE (DEFAULT) - Upon attempting to create a new data file, program will not check whether there already is a file with the same name on the SD card
//TRUE - Upon attempting to create a new data file, program will check whether there is already a file with the same name on the SD card, and will attempt to delete it if it's present prior to creating the new file
int LineLimit = 1000000;
//DEFAULT: 1000000
int PMTHitThreshold = 0;
//Defines the threshold above which the data from the PMT will be written to the data file
int TimeOffset = 0;
//Increments the timestamp in the data files by this amnount
String FileName = "COC_HASP2022_DataFile_";
//[prefix + file number + suffix] HAS to be smaller than the char "buff" present in the saving function!
String FileExt = ".txt";
//DEFAULT: ".txt"

//Rest of global variables (do not change)
SdExFat SDCard;
ExFatFile CurrFile;
long ThermResistance = 10000;
int FilesNum = 0;
int CurrLines = 0;
int SCKRate = 0;
bool SDOpen = false;
bool FileOpen = false;
bool NeedNewFile = true;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_PMT, INPUT);
  for (int i = 0; i < NumInTempPins; i++) 
  {
    pinMode(PINS_TEMP_INSIDE[i], INPUT);
  }
  /*for (int i = 0; i < NumOutTempPins; i++) 
  {
    pinMode(PINS_TEMP_OUTSIDE[i], INPUT);
  }*/
  if (!FirstSDCheck) OpenSD();
}

void loop() {
  if ((SDCheckMode || FirstSDCheck) && (!SDOpen))
  {
    OpenSD();
    if (!SDOpen) return;
    FirstSDCheck = false;
  }

  int currPMTHit = 0;
  long currTemp = 0;
  //currPMTHit = SimulatePMT();
  currPMTHit = ReadPMT();
  //currTemp = SimulateThermistor();
  currTemp = ReadTemp(0);
  if (currPMTHit > PMTHitThreshold)
  {
    String data = String(currPMTHit);
    data.concat("\t- ");
    data.concat(String(currTemp));
    SaveData(data);
  }
}
