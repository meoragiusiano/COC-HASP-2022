//// HASP 2022 TESTING CODE ////

#include <SD.h>

int PIN_CS = 10;  //Pin defined for "Chip Select" on Teensy 4.0
int PIN_PMT = 20; //Pin defined for PMT analog input on Teensy 4.0

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
File CurrFile;
int FilesNum = 0;
int CurrLines = 0;
int SCKRate = 1;
bool SDOpen = false;
bool FileOpen = false;
bool NeedNewFile = true;

void SaveData(String);
void WriteDataFile(String);
void OpenDataFile();
void CloseDataFile();
void OpenSD();
long SimulateThermistor();
int ReadPMT();
int SimulatePMT();

void setup() {
  Serial.begin(9600);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_PMT, INPUT);
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
  int currTemp = 0;
  //currPMTHit = SimulatePMT();
  currPMTHit = ReadPMT();
  currTemp = SimulateThermistor();
  if (currPMTHit > PMTHitThreshold)
  {
    String data = String(currPMTHit);
    data.concat("\t- ");
    data.concat(String(currTemp));
    SaveData(data);
  }
}

void SaveData(String data) {
  if (!FileOpen) 
  {
    OpenDataFile();
    if ((SDCheckMode) && (!FileOpen)) 
    {
      SDOpen = false;
      
      return;
    }
  }

  WriteDataFile(data);
  CurrLines++;

  if (SlowSaveMode) CloseDataFile();
  if (CurrLines >= LineLimit)
  {
    NeedNewFile = true;
    if (!SlowSaveMode) CloseDataFile();
  }
}

void WriteDataFile(String data) {
  int dataLength = data.length() + 1;
  long elapsedSeconds = (millis() / 1000) + TimeOffset;
  String timeStr = String(elapsedSeconds);
  int timeLength = timeStr.length() + 1; 
  
  if ((dataLength < 100) && (timeLength < 100))
  {
    char buff[100];
    
    data.toCharArray(buff, dataLength);
    buff[dataLength - 1] = '\0';
    CurrFile.write(buff);

    CurrFile.write("\t- ");

    timeStr.toCharArray(buff, timeLength);
    buff[timeLength - 1] = '\0';
    CurrFile.write(buff);
    CurrFile.write(" sec\n");
  }
  else
  {
    CurrFile.write("Unable to write data (data exceeded the buffer)!\n");
    //Serial.println("Unable to write data (data exceeded the buffer)!");
  }
}

void OpenDataFile() {
  if (NeedNewFile) FilesNum++;
  
  String currName = FileName;
  currName.concat(FilesNum);
  currName.concat(FileExt);
  int nameLength = currName.length() + 1;
  
  if (nameLength < 100)
  {
    char buff[100];
    
    currName.toCharArray(buff, nameLength);
    buff[nameLength - 1] = '\0';
    if ((NeedNewFile) && (FileCheckMode))
    {
      if (SD.exists(buff) == 1) 
      {
        if (SD.remove(buff) == 1)
        {
          Serial.print("Deleted existing data file #");
          Serial.print(FilesNum);
          Serial.println("!");
        }
        else
        {
          Serial.print("Unable to delete existing data file #");
          Serial.print(FilesNum);
          Serial.println("!");

          //return;
        }
      }
    }
    CurrFile = SD.open(buff, FILE_WRITE);

    if ((SDCheckMode) && (!CurrFile))
    {
      Serial.println("Unable to open data file (SD full or disconnected)!");
      if (NeedNewFile) FilesNum--;
      
      return;
    }

    FileOpen = true;
    if (NeedNewFile)
    {
      CurrFile.write("-------- HASP 2022 - College of the Canyons - DataFile #");
      String fileNumStr = String(FilesNum);
      fileNumStr.toCharArray(buff, fileNumStr.length() + 1);
      CurrFile.write(buff);
      CurrFile.write(" --------\n\n");
      CurrLines = 0;
      NeedNewFile = false;
      Serial.print("Created file #");
      Serial.print(FilesNum);
      Serial.println("!");
    }
  }
  else
  {
    Serial.println("Unable to open data file (name exceeded the buffer)!");
  }
}

void OpenSD() {
  if (SD.begin() == 1) 
  {
    SDOpen = true;
    Serial.println("SD opened successfully!");
  }
  else
  {
    SDOpen = false;
    Serial.println("SD open failed!");
  }
}

void CloseDataFile() {
  CurrFile.close();
  FileOpen = false;
}

long SimulateThermistor() {
  return random(50, 200);
}

int ReadPMT() {
  return analogRead(PIN_PMT);
}

int SimulatePMT() {
  return random(5, 100);
}
