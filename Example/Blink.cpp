#include "libGPIO.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    GPIO led("A23");

    led.pinMode(OUTPUT);
    while(1)
    {
	Delay(250);
	led.digitalWrite(HIGH);
	Delay(250);
	led.digitalWrite(LOW);
    }

}
