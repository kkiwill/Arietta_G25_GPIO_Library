#include <iostream>
#include <string>
#include "GPIO_Class.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <map>

#define GPIO_BASE_DIR  "/sys/class/gpio"
#define EXPORT_FILE    "/sys/class/gpio/export"
#define UNEXPORT_FILE  "/sys/class/gpio/unexport"

using namespace std;

//##############################
// Initialisation du port GPIO
//##############################
GPIO::GPIO()
{
}

GPIO::GPIO(int pin)
{
	// Pin valeur et nom
	map <int, string> PinName;
	PinName[0]  = "A0";
	PinName[1]  = "A1";
	PinName[5]  = "A5";
	PinName[6]  = "A6";
	PinName[7]  = "A7";
	PinName[8]  = "A8";
	PinName[21] = "A21";
	PinName[22] = "A22";
	PinName[23] = "A23";
	PinName[24] = "A24";
	PinName[25] = "A25";
	PinName[26] = "A26";
	PinName[27] = "A27";
	PinName[28] = "A28";
	PinName[29] = "A29";
	PinName[30] = "A30";
	PinName[31] = "A31";
	
	PinName[43] = "B11";
	PinName[44] = "B12";
	PinName[45] = "B13";
	PinName[46] = "B14";
	
	PinName[64] = "C0";	
	PinName[65] = "C1";	
	PinName[66] = "C2";	
	PinName[67] = "C3";	
	PinName[68] = "C4";	
	PinName[81] = "C17";	
	PinName[91] = "C27";	
	PinName[92] = "C28";	
	PinName[95] = "C31";
	
	iPin = pin;
	sPin = PinName[pin];
	
	GPIO::Init();
}

GPIO::GPIO(std::string pin)
{
	// Pin nom et valeur
	map <string, int> PinName;
	PinName["A0"]  = 0;
	PinName["A1"]  = 1;
	PinName["A5"]  = 5;
	PinName["A6"]  = 6;
	PinName["A7"]  = 7;
	PinName["A8"]  = 8;
	PinName["A21"] = 21;
	PinName["A22"] = 22;
	PinName["A23"] = 23;
	PinName["A24"] = 24;
	PinName["A25"] = 25;
	PinName["A26"] = 26;
	PinName["A27"] = 27;
	PinName["A28"] = 28;
	PinName["A29"] = 29;
	PinName["A30"] = 30;
	PinName["A31"] = 31;
	
	PinName["B11"] = 43;
	PinName["B12"] = 44;
	PinName["B13"] = 45;
	PinName["B14"] = 46;
	
	PinName["C0"]  = 64;	
	PinName["C1"]  = 65;	
	PinName["C2"]  = 66;	
	PinName["C3"]  = 67;	
	PinName["C4"]  = 68;	
	PinName["C17"] = 81;	// Button
	PinName["C27"] = 91;	
	PinName["C28"] = 92;	
	PinName["C31"] = 95;
	
	iPin = PinName[pin];
	sPin = pin;
	
	GPIO::Init();
}

bool GPIO::Init()
{
	int fd;
	char b[64];
	
	//printf("Test val : %d\r\n", PinName["PA23"]);
	// Prepare GPIO pin connected to sensors data pin to be used with GPIO sysfs
	fd = open(EXPORT_FILE, O_WRONLY);
	if (fd < 0)
	{
		perror(EXPORT_FILE);
		return false;
	}
	snprintf(b, sizeof(b), "%d", iPin);
	if (pwrite(fd, b, strlen(b), 0) < 0) {
		//fprintf(stderr, "Unable to export pin=%d (already in use?): %s\n",
		//pin, strerror(errno));
		return false;
	}
	close(fd);

	return true;
}

void GPIO::pinMode(DHT_IOMODE_t iomode)
{
	int fd,res;
	char b[64];
	
	// Open gpio direction file for fast reading/writing when requested
	sysfs_filename(b, sizeof(b), "direction");
	fd = open(b, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Open %s: %s\n", b, strerror(errno));
	}
	
	if (iomode == INPUT)
		res = pwrite(fd, "in", 3, 0);
	else
		res = pwrite(fd, "out", 4, 0);

	if (res < 0) {
		//fprintf(stderr, "Unable to pwrite to gpio direction for pin %s: %s\n", sPin, strerror(errno));
	}
	
	close(fd);
}

void GPIO::digitalWrite(PIN_STATE_t value)
{
	int fd;
	char d[1];
	char b[64];
	
	// Ouverture du fichier
	sysfs_filename(b, sizeof(b), "value");
	fd = open(b, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Open %s: %s\n", b, strerror(errno));
	}
	
	// Ecriture de la valeur
	d[0] = (value == LOW ? '0' : '1');
	if (pwrite(fd, d, 1, 0) != 1) {
			fprintf(stderr, "Unable to pwrite %c to gpio value: %s\n", d[0], strerror(errno));
	}
	
	close(fd);
}

PIN_STATE_t GPIO::digitalRead(void)
{
	int fd;
	char d[1];
	char b[64];
	
	// Ouverture du fichier
	sysfs_filename(b, sizeof(b), "value");
	fd = open(b, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Open %s: %s\n", b, strerror(errno));
	}
	
	if (pread(fd, d, 1, 0) != 1) {
		fprintf(stderr, "Unable to pread gpio value: %s\n",strerror(errno));
	}

	close(fd);
	
	return (d[0] == '0' ? LOW : HIGH);
}

void GPIO::CleanGPIO(void)
{
	int fd;
	char b[8];

	// free GPIO pin connected to sensors data pin to be used with GPIO sysfs
	fd = open(UNEXPORT_FILE, O_WRONLY);
	if (fd < 0) {
		perror(UNEXPORT_FILE);
		return;
	}
	snprintf(b, sizeof(b), "%d", iPin);
	if (pwrite(fd, b, strlen(b), 0) < 0) {
		//fprintf(stderr, "Unable to unexport pin=%d: %s\n", data_pin, strerror(errno));
		return;
	}
	close(fd);
}
		
void GPIO::sysfs_filename(char *filename, int len, const char *function)
{
	snprintf(filename, len, "%s/pio%s/%s", GPIO_BASE_DIR, sPin.c_str(), function);
}

//##############################################
// Fonction lecture du boutton de l'Arietta G25
//##############################################
PIN_STATE_t readButton()
{	
	int i;
	GPIO button("C17");
	i = button.digitalRead();
	button.CleanGPIO();
	if(i==0)
	{
		return LOW;
	}
	else
	{
		return HIGH;
	}
}

//############################################
// Fonction Delay en Milliseconde
//############################################
void Delay(clock_t iMilli)
{
	struct timespec _stop;
	clock_t iStart, iStop;
	
	clock_gettime(CLOCK_REALTIME, &_stop);
	iStart  = _stop.tv_sec*1000+(_stop.tv_nsec/1000000);
	
	iStop = iStart;
	while ((iStop-iStart) < iMilli)
	{
		clock_gettime(CLOCK_REALTIME, &_stop);
		iStop = _stop.tv_sec*1000+(_stop.tv_nsec/1000000);
	}
}
