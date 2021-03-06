﻿#include <stdio.h>
#include <wiringPi.h>
#define DELAY 30


int main (int argc, char *argv[]) {
	size_t i, x = 0, y = 0, ms = 0;

	wiringPiSetupGpio();

	pinMode(23, OUTPUT);
	pinMode(18, INPUT);
	pinMode(17, INPUT);
	printf ("%s\n%s", "Welcome to our Light Controller program!"
		, "Press any key to read from input or ctrl+d to exit:");
	while (getchar() != EOF) 
	{
		x = digitalRead(18);
		y = digitalRead(17);
		printf ("%d%d\n", x, y);

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

		for (i = 0; i < 50; i++) 
		{
			if (!(x + y))
			break;

			digitalWrite(23, HIGH);

			if (x + y == 2)
			continue;	
			delay(ms);

			digitalWrite(23, LOW);
			delay(ms);
		}
	}
	return 0;
}

