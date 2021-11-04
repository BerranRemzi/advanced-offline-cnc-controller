#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern const char testData[255][255];
    extern const double zMap[4][4];

#ifdef __cplusplus
}
string ReadStringUntil(char marker);
bool Available();

class File {
public:
    uint32_t available();
    char* readUntil(char _delimiter);
    operator bool();
    void close();

    FILE* fp = NULL;
    char buffer[255] = {};
};

class SDCard {
public:
    File open(const char fileName[]);
};

class DummySerial {
public:
    static void println(char _data[]);
};

extern SDCard SD;
extern DummySerial Serial;

#endif
