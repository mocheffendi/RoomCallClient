#include <TimeLib.h>
#include <TelnetStream.h>
#include <sntp.h>
#include <TZ.h>

#define TIME_ZONE TZ_Europe_London

void log()
{
    static int i = 0;

    char timeStr[20];
    sprintf(timeStr, "%02d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());

    TelnetStream.print(i++);
    TelnetStream.print(" ");
    TelnetStream.print(timeStr);
    TelnetStream.print(" A0: ");
    TelnetStream.println(analogRead(A0));
}

void TelnetLog(String str)
{
    char timeStr[20];
    sprintf(timeStr, "%02d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
    TelnetStream.print(" ");
    TelnetStream.print(timeStr);
    TelnetStream.println(str);
}