//// HASP 2022 TESTING CODE ////

#include "HASP2022_SDCard.h"
#include "HASP2022_PMT.h"
#include "HASP2022_Temp.h"
#include "HASP2022_Downlink.h"

int PIN_CS = BUILTIN_SDCARD;
int PIN_PMT = 20;
int PINS_TEMP_INSIDE[] = {35, 36};
int PINS_TEMP_OUTSIDE[] = {22, 23};

bool CheckSDInitially = true;
//FALSE - At the start, program will only try to open the SD card once
//TRUE (DEFAULT) - At the start, program will keep trying to open the SD card until it is successfully opened
bool DeleteFiles = false;
//FALSE (DEFAULT) - Upon attempting to create a new data file, program will not check whether there already is a file with the same name on the SD card
//TRUE - Upon attempting to create a new data file, program will check whether there is already a file with the same name on the SD card, and will attempt to delete it if it's present prior to creating the new file
int LineLimit = 1000000;
int DownlinkInterval = 5;
int TimeOffset = 0;
String FileName = "COC_HASP2022_DataFile_";
String FileExt = ".txt";

File CurrFile;
long ElapsedSeconds = 0;
long ThermResistance = 10000;
int FilesNum = 0;
int CurrLines = 0;
int PMTHits = 0;
bool SDOpenStatus = false;
bool FileOpenStatus = false;
bool NeedNewFile = true;
bool CanSendDownlink = false;

void setup() {
  Serial.begin(9600);
  Serial5.begin(4800);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_PMT, INPUT);
  for (int i = 0; i < 2; i++)
    pinMode(PINS_TEMP_INSIDE[i], INPUT);
  for (int i = 0; i < 2; i++)
    pinMode(PINS_TEMP_OUTSIDE[i], INPUT);
  if (!CheckSDInitially)
  {
    OpenSD();
    PrintSDStatus();
  }
}

void loop() {
  ElapsedSeconds = (millis() / 1000) + TimeOffset;
  CheckDownlink();
  
  int PMTHitDigital = ReadPMTDigital();
  int PMTHitAnalog = ReadPMTAnalog();

  if (CheckSD())
  {
    String data = "";
    data += String(PMTHitDigital);
    data += "\t";
    data += String(PMTHitAnalog);
    data += "\t";
    data += String(ReadTemp(0));
    data += "\t";
    data += String(ReadTemp(1));
    SaveData(data);
  }
  if (PMTHitDigital == 1)
    PMTHits++;
}
