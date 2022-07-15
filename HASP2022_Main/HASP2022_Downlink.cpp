#include "HASP2022_Downlink.h"
#include "HASP2022_SDCard.h"

extern long ElapsedSeconds;
extern int DownlinkInterval;
extern int FilesNum;
extern int PMTHits;
extern bool SDOpenStatus;
extern bool CanSendDownlink;

void CheckDownlink() {
  if (((ElapsedSeconds % DownlinkInterval) == 0) && (!CanSendDownlink))
  {
    String data = SDOpenStatus;
    data += ByteString(FilesNum);
    data += ByteString(PMTHits);
    data += "\n";
    
    SendDownlink(data);
    PMTHits = 0;
    CanSendDownlink = true;
  }
  else if ((ElapsedSeconds % DownlinkInterval) != 0)
  {
    CanSendDownlink = false;
  }
}

void SendDownlink(String data) {
  String dataPrint = "DOWNLINK: " + data;
  
  Serial.print(dataPrint);
  Serial5.print(data);

  //char buff[100];
  //dataPrint.toCharArray(buff, dataPrint.length() + 1);
  //buff[dataPrint.length() - 1] = '\0';
  //Serial5.write(buff);
}

String ByteString(int num) {
  String numString = String(num);
  int len = numString.length();
  
  if (len > 4)
    return "9999";
  for (int i = 0; i < (4 - len); i++)
    numString = "0" + numString;

  return numString;
}
