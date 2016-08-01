#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#define LINESIZE 256
/* 1000MS per Second so 1000MS/60FPS = 16.7MS_PER_UPDATE */
#define MS_PER_UPDATE 16

/*Functions declared here*/
int setup(void);
int initialize(void);
int run(void);
void processInput(void);
void update(void);
void render(void);
int end(void);
int getUserInt(void);
long getCurrentTime(void);

/*Typedefs declared here*/
typedef enum ledStates
{
	OFF,
	SELECTED,
	CROSS,
	CIRCLE,
	/*SELECTEDX and O represent the state where it is CIRCLE or CROSS and also it is the currently selected led*/
	SELECTEDX,
	SLECTEDO
} ledState;

typedef enum boardStates
{
	IDLE,
	POSITIONING,
	SELECTING
} boardState;

typedef struct led
{
    int pinHigh, pinLow;
    ledState state;
} led;

typedef struct board 
{
	boardState state;
    led ledArr[9];
	/*The two controls for user input*/
    int positioner, selector;
} board;


/*Globals declared here*/
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
	int io1 = 0, io2 = 0;

    wiringPiSetupGpio(); 

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
            matrix.ledArr[0].pinHigh = io1;
            matrix.ledArr[3].pinHigh = io1;
            matrix.ledArr[6].pinHigh = io1;
            matrix.ledArr[0].pinLow = io2;
            matrix.ledArr[1].pinLow = io2;
            matrix.ledArr[2].pinLow = io2;
        }
        if (i == 2)
        {
            matrix.ledArr[1].pinHigh = io1;
            matrix.ledArr[4].pinHigh = io1;
            matrix.ledArr[7].pinHigh = io1;
            matrix.ledArr[3].pinLow = io2;
            matrix.ledArr[4].pinLow = io2;
            matrix.ledArr[5].pinLow = io2;
        }
        if (i == 3)
        {
            matrix.ledArr[2].pinHigh = io1;
            matrix.ledArr[5].pinHigh = io1;
            matrix.ledArr[8].pinHigh = io1;
            matrix.ledArr[6].pinLow = io2;
            matrix.ledArr[7].pinLow = io2;
            matrix.ledArr[8].pinLow = io2;
        }
    }

	return 0;	
}

int initialize(void)
{
    return 0;
}

/*Support function used to get an int from stdin*/
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
	if (digitalRead(matrix.positioner)) 
    {
		matrix.state = POSITIONING;
		return;
    }
    else if (digitalRead(matrix.selector)) 
    {
        matrix.state = SELECTING;
        return;
    }
    else 
    {
        matrix.state = IDLE;
        return;
    }
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