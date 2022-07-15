#include "HASP2022_Downlink.h"
#include "HASP2022_PMT.h"

extern long ElapsedSeconds;
extern int DownlinkInterval;
extern int FilesNum;
extern int PMTHits;
extern bool CanSendDownlink;

void CheckDownlink() {
  if (((ElapsedSeconds % DownlinkInterval) == 0) && (!CanSendDownlink))
  {
    String data = "0";
    data.concat(ByteString(FilesNum));
    data.concat(ByteString(ReadPMT()));
    data.concat("\n");
    SendDownlink(data);
    PMTHits = 0;
    CanSendDownlink = true;
  }
  else if ((ElapsedSeconds % DownlinkInterval) != 0)
    CanSendDownlink = false;
}

void SendDownlink(String data) {
  String dataPrint = "COC DOWNLINK: ";
  dataPrint.concat(data);
  
  Serial.print(dataPrint);
  Serial5.print(dataPrint);

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

  for (int i = 0; i < (4 - len); i++) {
    numString = "0" + numString;
  }

  return numString;
}
