//// HASP 2022 TESTING CODE ////

#include "HASP2022_SDCard.h"
#include "HASP2022_PMT.h"
#include "HASP2022_Temp.h"
#include "HASP2022_Downlink.h"

int PIN_CS = BUILTIN_SDCARD;
int PIN_PMT = 20;
int PINS_TEMP_INSIDE[] = {22, 23};
int PINS_TEMP_OUTSIDE[] = {35, 36};

bool SaveFilesAlways = false;
//FALSE (DEFAULT) - Program will only save the currently-opened data file after it crosses the Line Limit (faster)
//TRUE - Program will save the currently-opened data file after every iteration (slower)
bool CheckSDAlways = false;
//FALSE (DEFAULT) - Program will open the SD card only in the beggining according to CheckSDInitially, will never check whether the SD is actually connected, and will just keep attempting to save to it regardless
//TRUE - Program will check whether the SD is still connected every time the currently-opened data file is saved, will halt the saving process if it detects the SD is disconnected, and resume once it gets recconected
bool CheckSDInitially = true;
//FALSE - At the start, program will only run begin() function of the SD card in the setup() function
//TRUE (DEFAULT) - At the start, program will keep trying to open the SD card until it is successfully opened
bool DeleteFiles = false;
//FALSE (DEFAULT) - Upon attempting to create a new data file, program will not check whether there already is a file with the same name on the SD card
//TRUE - Upon attempting to create a new data file, program will check whether there is already a file with the same name on the SD card, and will attempt to delete it if it's present prior to creating the new file
int LineLimit = 1000000;
int PMTHitThreshold = 10;
int DownlinkInterval = 900;
int TimeOffset = 0;
String FileName = "COC_HASP2022_DataFile_";
String FileExt = ".txt";

File CurrFile;
long ElapsedSeconds = 0;
long ThermResistance = 10000;
int FilesNum = 0;
int CurrLines = 0;
int PMTHits = 0;
bool SDOpen = false;
bool FileOpen = false;
bool NeedNewFile = true;
bool CanSendDownlink = false;

void setup() {
  Serial.begin(9600);
  Serial5.begin(9600);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_PMT, INPUT);
  for (int i = 0; i < 2; i++)
    pinMode(PINS_TEMP_INSIDE[i], INPUT);
  for (int i = 0; i < 2; i++)
    pinMode(PINS_TEMP_OUTSIDE[i], INPUT);
  if (!CheckSDInitially)
    OpenSD();
}

void loop() {
  CheckDownlink();
  if ((CheckSDAlways || CheckSDInitially) && (!SDOpen))
  {
    OpenSD();
    if (!SDOpen)
      return;
    CheckSDInitially = false;
  }
  
  ElapsedSeconds = (millis() / 1000) + TimeOffset;
  int currPMTHit = ReadPMT();

  if (currPMTHit > PMTHitThreshold)
  {
    String data = String(currPMTHit);
    data.concat("\t- ");
    data.concat(String(ReadTemp(0)));
    data.concat("\t- ");
    data.concat(String(ReadTemp(1)));
    SaveData(data);
  }
}
