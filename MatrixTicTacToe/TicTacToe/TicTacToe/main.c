#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#define LINESIZE 256
/* 1000MS per Second so 1000MS/60FPS = 16.7MS_PER_UPDATE */
#define MS_PER_UPDATE 16

int main (int argc, char *argv[])
{
	setup();
	run();
	end();
	printf("End of program, enter to exit:");
	getchar();
	return 0;
}

int setup(void) {
	int pinNum = 0;

   wiringPiSetupGpio(void); 

   printf("Please enter the GPIO pin number for the LED Positioner: \n");
   pinNum = getUserInt();
   pinMode(pinNum, INPUT);
   printf("LED Posistioner set to pin %d\n", pinNum);

   printf("Please enter the GPIO pin number for the LED Selector: \n");
   pinNum = getUserInt();
   pinMode(pinNum, INPUT);
   printf("LED Selector set to pin %d\n", pinNum);

   for (size_t i = 1; i < 4; i++)
      {
      printf("Please enter the GPIO pin number for Column %d: \n", i);
      pinNum = getUserInt();
      pinMode(pinNum, OUTPUT);
      printf("Column %d set to pin %d\n", i, pinNum);

      printf("Please enter the GPIO pin number for Row %d: \n", i);
	   pinNum = getUserInt();
   	pinMode(pinNum, OUTPUT);
      printf("Row %d set to pin %d\n", i, pinNum);
      }
	return 0;	
}

int getUserInt(void) 
{
	char input[LINESIZE];
	int output = 0;

	while (1) {
		if (!fgets(input, LINESIZE, stdin)) {
			clearerr(stdin);
			printf("%s\n", "Error occurred getting user input!");
		}
		if (sscanf(input, "%d", &output) == 1) {
			return output;
		} else {
			printf("%s\n", "Invalid input try again: ");
		}
	}
}

void processInput(void) 
{
	return;
}

void update(void) 
{
	return;
}

void render(void) 
{
	return;
}

long getCurrentTime(void) 
{
	int timeout = 0;
	clock_t current_time;

	current_time = clock();

	while (current_time == ((time_t)-1) && (timeout <= 2000))
	{		
		printf("\nError obtaining the current time, retrying..");
		delay(10);
		timeout += 10;
	}

	long time = current_time;
	return time;	
}

int run(void) 
{
	int exit = 0;
	long previous = getCurrentTime();
	long lag = 0;

	while (exit < 500) 
	{
		long current = getCurrentTime();
		long elapsed = current - previous;
		previous = current;
		/* time is in 1/10 of a millisecond so need to divide be 10 to get miliseconds */
		lag += (elapsed / 10);

		processInput();

		printf("\nLag = %d \n Current Time = %d", lag , current/10);
		while(lag >= MS_PER_UPDATE) 
		{
			update();
			lag -= MS_PER_UPDATE;
		}

		exit++;
		render();
	}
	return 0;
}

int end(void) 
{
	return 0;
}