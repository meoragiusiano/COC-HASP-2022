#include "HASP2022_Downlink.h"
#include "HASP2022_Temp.h"
#include "HASP2022_PMT.h"
#include<iostream>
#include<ctime>

using namespace std;

int PMTHit = 0;
long temp = 0;
String timestamp1;
String timestamp2;

String ReadTimestamp()
{
    time_t t = time(nullptr);
    timestamp1 = to_string(t) + " seconds since 01-Jan-1970\n";
    timestamp2 = to_string(asctime(localtime(&result))); //day month hr:min:sec year
    return timestamp1 + timestamp2;
}

void Downlink() {
    PMTHit = ReadPMT();
    temp = ReadTemp(0);

    Serial5.print(ReadTimestamp());
    Serial5.print(to_string(PMTHit));
    Serial5.print(to_string(temp));
}