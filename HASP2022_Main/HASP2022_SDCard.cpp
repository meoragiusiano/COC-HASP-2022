#include "HASP2022_SDCard.h"

extern int PIN_CS;

extern bool SaveFilesAlways;
extern bool CheckSDAlways;
extern bool CheckSDInitially;
extern bool DeleteFiles;
extern int LineLimit;
extern int PMTHitThreshold;
extern String FileName;
extern String FileExt;

extern File CurrFile;
extern long ElapsedSeconds;
extern int FilesNum;
extern int CurrLines;
extern bool SDOpenStatus;
extern bool FileOpenStatus;
extern bool NeedNewFile;

void SaveData(String data) {
  if (!FileOpenStatus) 
  {
    OpenDataFile();
    if ((CheckSDAlways) && (!FileOpenStatus)) 
    {
      SDOpenStatus = false;
      
      return;
    }
  }

  WriteDataFile(data);
  CurrLines++;

  if (SaveFilesAlways) 
    CloseDataFile();
  if (CurrLines >= LineLimit)
  {
    NeedNewFile = true;
    if (!SaveFilesAlways) 
      CloseDataFile();
  }
}

void WriteDataFile(String data) {
  int dataLength = data.length() + 1;
  String timeStr = String(ElapsedSeconds);
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
  }
}

void OpenDataFile() {
  if (NeedNewFile) 
    FilesNum++;
  
  String currName = FileName;
  currName += FilesNum;
  currName += FileExt;
  int nameLength = currName.length() + 1;
  
  if (nameLength < 100)
  {
    char buff[100];
    
    currName.toCharArray(buff, nameLength);
    buff[nameLength - 1] = '\0';
    if ((NeedNewFile) && (DeleteFiles))
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
        }
      }
    }
    CurrFile = SD.open(buff, FILE_WRITE);

    if ((CheckSDAlways) && (!CurrFile))
    {
      Serial.println("Unable to open data file (SD full or disconnected)!");
      if (NeedNewFile)
        FilesNum--;
      
      return;
    }

    FileOpenStatus = true;
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

void CloseDataFile() {
  CurrFile.close();
  FileOpenStatus = false;
}

bool CheckSD() {
  if (CheckSDAlways || CheckSDInitially)
  {
    bool prevSDStatus = SDOpenStatus;
    OpenSD();
    if (!SDOpenStatus) 
    {
      PrintSDStatus();
      
      return false;
    }
    else
    {
      if (!prevSDStatus)
        PrintSDStatus();
    }
    CheckSDInitially = false;
  }

  return true;
}

void OpenSD() {
  SD.begin(PIN_CS) ? SDOpenStatus = true : SDOpenStatus = false;
}

void PrintSDStatus() {
  SDOpenStatus ? Serial.println("SD opened successfully!") : Serial.println("SD open failed!");
}
