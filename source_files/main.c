//  minesweeper
//
//  Created by Zhang Li.
//
#include "game.h"

int main() {
    Field field;
    bool result;
    Coordinate coor;
    TimeComsumption critical_times;

    system("clear");   
    // Tells the user last logging time
    printf("--Hello, We HAVE NOT Met SINCE ");
    DisplayLastRunningTime();
    printf("\n                 ：）））\n\n\n\n");

    while (1) {
        if (StartOrNot()) {
            // Initializes the game.
            // Get the first coor then insert the mine!
            field.property = ReadFieldProperty();
            field.userfield = InitializeField(field.property, COVERED);
            field.datafield = InitializeField(field.property, FREE);
            time(&critical_times.str);
            system("clear");
            DisplayField(field.userfield, field.property);
            printf("MINE REMAINING: %d\n",field.property.num_of_mines);
            printf("PLEASE ENTER YOUR FIRST COORDINATE TO SWEEP:\n");
            coor = ReadTheCoordinate(field.property);    
            RandomlyInsertMine(field.datafield, field.property, coor);
            AutoReveal(field, coor);
            
            // Plays the game.
            result = SweepingFlaggingEnding(field);
            system("clear");
            time(&critical_times.end);
            DisplayResults(field, result, critical_times);
            
            // Frees the memory every time when the previous fields are not useful.
            ArrayFree(field.userfield, field.property);
            ArrayFree(field.datafield, field.property);
        } else break;  // Breaks out the loop if user chose to quit.
    }
    printf("                 GOODBYE\n\n");
    // Save running time before quit
    if (RecordLastRunningTime() == false) {
        printf ("WARNING:ERROR IN SAVING YOUR LOGGING TIME!"
                "YOU MAY NOT ABLE TO KNOW WHEN YOU LAST PLAYES NEXT TIME YOU LOG IN!\n");
    }
    return 0;
}