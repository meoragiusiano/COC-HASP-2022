#include "HASP2022_SDCard.h"

extern int PIN_CS;

extern bool SlowSaveMode;
extern bool SDCheckMode;
extern bool FileCheckMode;
extern int LineLimit;
extern int PMTHitThreshold;
extern int TimeOffset;
extern String FileName;
extern String FileExt;

extern SdExFat SDCard;
extern ExFatFile CurrFile;
extern int FilesNum;
extern int CurrLines;
extern int SCKRate;
extern bool SDOpen;
extern bool FileOpen;
extern bool NeedNewFile;

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
      if (SDCard.exists(buff) == 1) 
      {
        if (SDCard.remove(buff) == 1)
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
    CurrFile.open(buff, FILE_WRITE);

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
      Serial.print("Created new data file #");
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
  if (SDCard.begin(PIN_CS, SCKRate) == 1) 
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
