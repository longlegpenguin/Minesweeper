#include <stdio.h>
#include <stdlib.h>  // For memory allocation and system function
#include <time.h>  // For default time structure and time function
#include <stdbool.h>  // For functions return boolean value

enum state {FREE = 0, FLAGGED = 9, COVERED = 10, EXPLODED = 11, MINE = 12} ;

typedef enum {TOSWEEP = 1,TOFLAG = 2, TODEFLAG = 3, TOEXPAND = 4} action;

typedef struct {
    int width;
    int height;
    int num_of_mines;
} FieldProperty;

typedef struct {
    int **userfield;
    int **datafield;
    FieldProperty property;
} Field;

typedef struct {
    int x;  // x coordinate
    int y;  // y coordinate
} Coordinate;

typedef struct {
    time_t str, end;  // Starting and ending time of the game.
} TimeComsumption;

// Frees the memory allocated to a 2D array.
void ArrayFree(int **field, FieldProperty f);

// Automatically reveals the states of the surroundings if a coordinate 
// is free of mine , until no more can be revealed.
void AutoReveal (Field f, Coordinate coor);

// Checks if a given coordinate is within the scope of field.
// Returns TRUE if within. Return FALSE if not in the scope.
bool CoorCheck (Coordinate coor, FieldProperty f);

// Counts the occurance of one type of element in the entire array.
// Returns the number of occurance.
int CountChosenElements(int **user_f, FieldProperty f, int checkwhat);

// Returns the number of mines around a chosen coordinate
int CountSurrondingMine(int **data_f, Coordinate coor, FieldProperty f);

// Prints the 2D array in field manner
void DisplayField(int **field, FieldProperty f);

// Reads and prints the latest running time from file.
void DisplayLastRunningTime(void);

// Displays the (results and time) of the game.
void DisplayResults(Field f, bool result, TimeComsumption t);

// Allocates the memory for the 2D array of appropriate size.
// Initializes every element with chosen state.
// Returns the address of the array.
// Memory must be freed (by the ArrayFree function) at the end of the program.
int **InitializeField(FieldProperty f, enum state state);

// Randomly set appreciate number of elements in the field to mine  
// The coordinate provided in argument will be excluded.
void RandomlyInsertMine(int **field, FieldProperty f, Coordinate coor);

// Reads and returns what the user want to do with the field next.
action ReadAction (void);

// Reads and size of field and number of mines from the user and returns in structure format.
FieldProperty ReadFieldProperty(void);

// Returns the coordinates read from user in struct form.
Coordinate ReadTheCoordinate(FieldProperty f);

// Writes the system time into an external file.
// Returns FALSE if failed to save.
bool RecordLastRunningTime(void);

// Asks if the user want to start or quit.
// Returns 1 if to start. Return 0 if to quit.
int StartOrNot(void);

// Sweeps a chosen coor. Returns FALSE if failed, TRUE if not yet.
// An address of variable must be provide to save the number of cleared elements.
bool Sweep(Field f, Coordinate coor, int* cleared);

// The main process of the game.
// Returns TRUE if user succeed. Return FALSE if user failed.
bool SweepingFlaggingEnding(Field f);
