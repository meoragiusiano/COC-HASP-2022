#include "HASP2022_SDCard.h"

extern int PIN_CS;

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
    OpenDataFile();

  WriteDataFile(data);
  CurrLines++;

  if (CurrLines >= LineLimit)
  {
    CloseDataFile();
    NeedNewFile = true;
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
    CurrFile.write("\t");
    timeStr.toCharArray(buff, timeLength);
    buff[timeLength - 1] = '\0';
    CurrFile.write(buff);
    CurrFile.write("\n");
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
    if ((DeleteFiles) && (NeedNewFile))
    {
      if (SD.exists(buff) == 1) 
        SD.remove(buff);
    }
    CurrFile = SD.open(buff, FILE_WRITE);

    FileOpenStatus = true;
    if (NeedNewFile)
    {
      CurrFile.write("-------- HASP 2022 - College of the Canyons - DataFile #");
      String fileNumStr = String(FilesNum);
      fileNumStr.toCharArray(buff, fileNumStr.length() + 1);
      CurrFile.write(buff);
      CurrFile.write(" --------\n---- (PMT Digital, PMT Analog, Temperature Inside (K), Temperature Outside (K), Timestamp (sec)) ----\n\n");
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
  if (CheckSDInitially)
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
