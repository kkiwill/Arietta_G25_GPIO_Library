#include "GPIO_Class.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    GPIO led("A23");

    led.pinMode(OUTPUT);
    while(1)
    {
	led.digitalWrite(readButton());
    }

}
