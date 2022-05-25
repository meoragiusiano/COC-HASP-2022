//// HASP 2022 TESTING CODE ////

#include <SD.h>

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

void SaveData(String);
void WriteDataFile(String);
void OpenDataFile();
void CloseDataFile();
void OpenSD();
long SimulateThermistor();
int SimulatePMT();

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
  long value = random(50, 200);
  
  return value;
}

int SimulatePMT() {
  int value = random(5, 100);
  
  return value;
}
