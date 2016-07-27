#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#define LINESIZE 256
/* 1000MS per Second so 1000MS/60FPS = 16.7MS_PER_UPDATE */
#define MS_PER_UPDATE 16

typedef struct led
{
    int pinh, pinl;
    int state;
} led;

typedef struct board 
{
    led led1, led2, led3, led4, led5, led6, led7, led8, led9;
    int positioner, selector;
} board;


board matrix;

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
	int io1 = 0, int io2;

   wiringPiSetupGpio(void); 

   printf("Please enter the GPIO pin number for the LED Positioner: \n");
   io1 = getUserInt();
   pinMode(io1, INPUT);
   printf("LED Posistioner set to pin %d\n", io1);

   printf("Please enter the GPIO pin number for the LED Selector: \n");
   io2 = getUserInt();
   pinMode(io2, INPUT);
   printf("LED Selector set to pin %d\n", io2);

   matrix.positioner = io1;
   matrix.selector = io2;

   for (size_t i = 1; i < 4; i++)
      {
      printf("Please enter the GPIO pin number for Column %d: \n", i);
      io1 = getUserInt();
      pinMode(io1, OUTPUT);
      printf("Column %d set to pin %d\n", i, io1);

      printf("Please enter the GPIO pin number for Row %d: \n", i);
	   io2 = getUserInt();
   	pinMode(io2, OUTPUT);
      printf("Row %d set to pin %d\n", i, io2);
      
      if (i == 1)
         {
         matrix.led1.pinh = io1;
         matrix.led4.pinh = io1;
         matrix.led7.pinh = io1;
         matrix.led1.pinl = io2;
         matrix.led2.pinl = io2;
         matrix.led3.pinl = io2;
         }
      if (i == 2)
         {
         matrix.led2.pinh = io1;
         matrix.led5.pinh = io1;
         matrix.led8.pinh = io1;
         matrix.led4.pinl = io2;
         matrix.led5.pinl = io2;
         matrix.led6.pinl = io2;
         }
      if (i == 3)
         {
         matrix.led3.pinh = io1;
         matrix.led6.pinh = io1;
         matrix.led9.pinh = io1;
         matrix.led7.pinl = io2;
         matrix.led8.pinl = io2;
         matrix.led9.pinl = io2;
         }
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