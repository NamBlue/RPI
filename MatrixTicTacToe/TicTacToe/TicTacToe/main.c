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

/*The current state of the LED*/
typedef enum ledStates
{
	OFF,
	SELECTED,
	CROSS,
	CIRCLE,
	/*SELECTEDX and O represent the state where it is CIRCLE or CROSS and also it is the currently selected led*/
	SELECTEDX,
	SELECTEDO
} ledState;

/*The current active player*/
typedef enum activePlayer
{
    X,
    O
} activePlayer;

/*The state of board*/
typedef enum boardStates
{
	IDLING,
	POSITIONING,
	SELECTING,
    EXITING
} boardState;

/*The LED object*/
typedef struct led
{
    int pinHigh, pinLow;
    ledState state;
} led;

/*The whole ciruit board object, also keeps track of the game state.*/
typedef struct board 
{
	boardState state;
    led ledArr[9];
    activePlayer player;
	/*The three controls for user input*/
    int positioner, selector, exit;
    /*Keeps track of the current frame of the game so render knows what frame it is in to render.*/
    long frame;
} board;


/*Globals declared here*/

board matrix;


/*Function definitions here*/

/*Entry point*/
int main (int argc, char *argv[])
{
    if (setup())
    {
        printf("\nError occurred in setup.");
        exit(1);
    }
    if (initialize())
    {
        printf("\nError occurred in initialize.");
        exit(1);
    }
    if (run())
    {
        printf("\nError occurred in run.");
        exit(1);
    }
    if (end())
    {
        printf("\nError occurred in end.");
        exit(1);
    }
	printf("\nEnd of program, enter to exit:");
	getchar();
	return 0;
}

/*Sets up all related hardware devices to interface with the GPIO pins*/
int setup(void) {
	int io1 = 0, io2 = 0;

    wiringPiSetupGpio(); 

    printf("Please enter the GPIO pin number for the LED Positioner: \n");
    matrix.positioner = getUserInt();
    pinMode(matrix.positioner, INPUT);
    printf("LED Posistioner set to pin %d\n", matrix.positioner);

    printf("Please enter the GPIO pin number for the LED Selector: \n");
    matrix.selector = getUserInt();
    pinMode(matrix.selector, INPUT);
    printf("LED Selector set to pin %d\n", matrix.selector);

    printf("Please enter the GPIO pin number for the Exit button: \n");
    matrix.exit = getUserInt();
    pinMode(matrix.exit, INPUT);
    printf("Exit button set to pin %d\n", matrix.exit);

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

/*Initializes the game to its starting state*/
int initialize(void)
{
    matrix.state = IDLING;
    matrix.frame = 0;

    for (size_t i = 0; i < 9; i++)
    {
        /*First led is always selected as the starting position*/
        if (i == 0) 
        {
            matrix.ledArr[i].state = SELECTED;
        } 
        else
        {
            matrix.ledArr[i].state = OFF;
        }
    }
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

    return 1;
}

/*Processes user input from the specified controls*/
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
    else if (digitalRead(matrix.exit)) 
    {
        matrix.state = EXITING;
        return;
    }
    else 
    {
        matrix.state = IDLING;
        return;
    }
}

/*Updates the game state and handles game logic*/
void update(void) 
{
    if (matrix.frame == 60)
    {
        matrix.frame = 0;
    }
    else
    {
        matrix.frame++;
    }

    if (matrix.state == IDLING)
        return;

    /*Still require a logic to win or reset the game on tie or exit*/
    /*Require decomposing the fuction*/
    if (matrix.state == POSITIONING) 
    {
        for (size_t i = 0; i < 9; i++) 
        {
            if (matrix.ledArr[i].state == SELECTED || 
                matrix.ledArr[i].state == SELECTEDX || 
                matrix.ledArr[i].state == SELECTEDO)
            {
                if (matrix.ledArr[i].state == SELECTEDX) 
                {
                    matrix.ledArr[i].state = CROSS;
                } 
                else if (matrix.ledArr[i].state == SELECTEDO)
                {
                    matrix.ledArr[i].state = CIRCLE;
                }
                else
                {
                    matrix.ledArr[i].state = OFF;
                }

                if (++i == 8)
                {
                    i = 0;
                }
                else
                {
                    i++;
                }

                if (matrix.ledArr[i].state == CROSS)
                {
                    matrix.ledArr[i].state = SELECTEDX;
                }
                else if (matrix.ledArr[i].state == CIRCLE)
                {
                    matrix.ledArr[i].state = SELECTEDO;
                }
                else
                {
                    matrix.ledArr[i].state = SELECTED;
                }
                return;
            }
        }
        printf("\nSomething wrong happened, cant find a selected pin.");
        return;
    }
    else if (matrix.state == SELECTING)
    {
        for (size_t i = 0; i < 9; i++)
        {
            if (matrix.ledArr[i].state == SELECTED)
            {
                if (matrix.player == X)
                {
                    matrix.ledArr[i].state = SELECTEDX;
                }
                else
                {
                    matrix.ledArr[i].state = SELECTEDO;
                }
            }
        }
        return;
    }
    else if (matrix.state == EXITING)
    {
        printf("\nThanks for playing!");
        exit(0);
    }
	return;
}

/*Renders the graphics on a 3x3 LED matrix*/
void render(void) 
{
    for (size_t i = 0; i < 9; i++)
    {
        /*Not redundant, for faster rendering.*/
        if (matrix.ledArr[i].state == OFF)
        {
            continue;
        }
        else if (matrix.ledArr[i].state == CROSS) 
        {
            /*Render for cross in solid light*/
            digitalWrite(matrix.ledArr[i].pinHigh, HIGH);
            digitalWrite(matrix.ledArr[i].pinLow, LOW);
        }
        else if (matrix.ledArr[i].state == CIRCLE)
        {
            /*Render for circle in fast blinking light*/
            if (!matrix.frame % 5)
            {
                digitalWrite(matrix.ledArr[i].pinHigh, HIGH);
                digitalWrite(matrix.ledArr[i].pinLow, LOW);
            }
            else
            {
                digitalWrite(matrix.ledArr[i].pinHigh, LOW);
                digitalWrite(matrix.ledArr[i].pinLow, LOW);
            }
        }
        else if (matrix.ledArr[i].state == SELECTED ||
            matrix.ledArr[i].state == SELECTEDX ||
            matrix.ledArr[i].state == SELECTEDO)
        {
            /*Render for selected in slow blinking light*/
            if (!matrix.frame % 20)
            {
                digitalWrite(matrix.ledArr[i].pinHigh, HIGH);
                digitalWrite(matrix.ledArr[i].pinLow, LOW);
            }
            else
            {
                digitalWrite(matrix.ledArr[i].pinHigh, LOW);
                digitalWrite(matrix.ledArr[i].pinLow, LOW);
            }
        }
    }
	return;
}

/*Support function to get the system time*/
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

/*Runs the game in a synchronized game loop*/
int run(void) 
{
    long previous = getCurrentTime();
	long lag = 0;

	while (1) 
	{
		long current = getCurrentTime();
		long elapsed = current - previous;
		previous = current;
		/* time is in 1/10 of a millisecond so need to divide by 10 to get miliseconds */
		lag += (elapsed / 10);

		processInput();

		while(lag >= MS_PER_UPDATE) 
		{
			update();
			lag -= MS_PER_UPDATE;
		}

		render();
	}
	return 0;
}

/*Cleanup done here*/
int end(void) 
{
	return 0;
}