#include <stdio.h>
#include <wiringPi.h>
#define DELAY 30


int main (int argc, char *argv[]) {
	size_t x = 0;

	wiringPiSetupGpio();

	pinMode(18, OUTPUT);
	pinMode(17, OUTPUT);

	printf ("Hello world!\n");
	while(x != 50) {
		digitalWrite(18, HIGH);
		delay(DELAY);
		digitalWrite(18, LOW);
		delay(DELAY);
		digitalWrite(17, HIGH);
		delay(DELAY);
		digitalWrite(17, LOW);
		delay(DELAY);
		x++;
	}
	getchar();
	return 0;
}

