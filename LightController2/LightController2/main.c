#include <stdio.h>
#include <wiringPi.h>
#define DELAY 30


int main (int argc, char *argv[]) {
	size_t i, x = 0, y = 0, z = 0, ms = 0;

	wiringPiSetupGpio();

	pinMode(23, OUTPUT);
	pinMode(24, INPUT);
	pinMode(18, INPUT);
	pinMode(17, INPUT);
	digitalWrite(23, LOW);

	printf ("%s\n%s", "Welcome to our Light Controller program 2!"
		, "Flip the switches to have fun or press the button to exit!");
	while (z != 1) 
	{
		x = digitalRead(18);
		y = digitalRead(17);
		z = digitalRead(24);


		if (x == 0 && y == 0)
		{
			ms = 0;
		} 
		else if ( x == 0, y == 1) 
		{
			ms = 50;
		}
		else if ( x == 1, y == 0) 
		{
			ms = 25;
		}
		else if ( x == 1, y == 1) 
		{
			ms = 1;
		}


		if (!(x + y))
			continue;

		digitalWrite(23, HIGH);

		if (x + y == 2)
		continue;	
		delay(ms);

		digitalWrite(23, LOW);
		delay(ms);

	}
	return 0;
}

