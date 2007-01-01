#ifndef DEF_PERSONNAGE
#define DEF_PERSONNAGE
#include <stdint.h>
#include <time.h>
#include <iostream>

using namespace std;

typedef enum {
   INPUT,
   OUTPUT
}
DHT_IOMODE_t;

typedef enum {
   LOW = 0 ,
   HIGH = 1
}
PIN_STATE_t;

class GPIO
{
    public:
    GPIO();
    GPIO(int pin);
    GPIO(std::string pin);
    void digitalWrite(PIN_STATE_t value);
    void pinMode(DHT_IOMODE_t iomode);
    bool Init();
    PIN_STATE_t digitalRead(void);
    void CleanGPIO(void);
	
    private:
    uint8_t iPin;
    std::string sPin;

    void sysfs_filename(char *filename, int len, const char *function);
};

PIN_STATE_t readButton();
void Delay(clock_t iMilli);
#endif


