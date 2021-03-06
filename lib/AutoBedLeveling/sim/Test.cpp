#include "Test.h"

using namespace std;

const char testData[][255] =
{
    {"G00"},
    {"G17"},
    {"G21"},
    {"G40"},
    {"G49"},
    {"G54"},
    {"G80"},
    {"G90"},
    {"G94"},
    {"G00 F3000 Z3.00"},
    {"M03 S9000"},
    {"G00 F3000 X7.00 Y28.00 Z3.00"},
    {"G00 F1500 Z0.00"},
    {"G01 F600 Z-0.20"},

    {"G01 F600 X7.00 Y28.00 Z0.00"},

    {"G01 F600 X5.09 Y20.00 Z0.00"},
    {"G01 F600 X2.71 Y10.00 Z0.00"},

    {"G01 F600 X2.00 Y7.00 Z0.00"},

    {"G01 F600 X10.00 Y5.46 Z0.00"},
    {"G01 F600 X20.00 Y3.54 Z0.00"},

    {"G01 F600 X28.00 Y2.00 Z0.00"},

    {"G01 F600 X26.10 Y10.00 Z0.00"},
    {"G01 F600 X23.71 Y20.00 Z0.00"},

    {"G01 F600 X23.00 Y23.00 Z0.00"},

    {"G01 F600 X20.00 Y23.96 Z0.00"},
    {"G01 F600 X10.00 Y27.06 Z0.00"},

    {"G01 F600 X7.00 Y28.00 Z0.00"},

    {"G00 F3000 Z3.00"},
    {"G00 F3000 X0.00 Y0.00 Z0.00"},
    {"M05"},
    {"M30"},
    {"%"}
};

const double zMap[4][4] = {
    {0.0, 0.1, 0.2, 0.3},
    {0.1, 0.2, 0.3, 0.4},
    {0.2, 0.3, 0.4, 0.5},
    {0.3, 0.4, 0.5, 0.6}
};

string ReadStringUntil(char marker){

    return "Line\n";
}
bool Available(){
    return false;
}

SDCard SD;

File SDCard::open(const char fileName[])
{
    File myFile;
    myFile.fp = fopen(fileName, "r");
    return myFile;
}

uint32_t File::available()
{
    uint32_t availableCount = 0;
    if (fgets(buffer, 255, fp) != NULL) {
        availableCount = 1;
    }
    return availableCount;
}

char* File::readUntil(char _delimiter)
{
    return buffer;
}

File::operator bool()
{
    return true;
}

void File::close()
{
    // close the file
    fclose(fp);
}

void DummySerial::println(char _data[])
{
    printf("%s", _data);
}
