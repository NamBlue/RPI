#include <stdio.h>
#include <wiringPi.h>
#define LINESIZE 256

int main (int argc, char *argv[])
{
	setup();
	run();
	end();
	printf("Press enter to exit:");
	getchar();
	return 0;
}

int setup(void) {
	int col1 = 0;

	wiringPiSetupGpio();
   
   for (size_t i = 1; i < 4; i++)
      {
      printf("Please enter the GPIO pin number for Column %d: \n" i);
      col1 = getUserInt();
      pinMode(col1, OUTPUT);

      printf("Please enter the GPIO pin number for Column %d: \n", i);
      col1 = getUserInt();
      pinMode(col1, OUTPUT);
      }

	return 0;	
}

int getUserInt(void) {
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

int run(void) {
	return 0;
}

int end(void) {
	return 0;
}