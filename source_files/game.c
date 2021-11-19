#include "game.h"

void ArrayFree(int **field, FieldProperty f) {
    free(field[0]);
    free(field);
}

void AutoReveal(Field f, Coordinate coor) {
    int surr_mine_num = CountSurrondingMine(f.datafield, coor, f.property);
    Coordinate coorcopy = coor;
    
    if (surr_mine_num == 0) {
        f.userfield[coor.y][coor.x] = FREE;  // Show free if no mine arround.
        // Recursive for only the four coordinates right beside the given one.
        if (coor.y > 0 && f.userfield[coor.y-1][coor.x] == COVERED) {
            coorcopy.y = coor.y - 1;
            AutoReveal(f, coorcopy);
        }
        if (coor.y < f.property.height - 1 && f.userfield[coor.y+1][coor.x] == COVERED) {
            coorcopy.y = coor.y + 1;
            AutoReveal(f, coorcopy);
        }
        // The y-coor has changed! Reset the copied coor first!
        coorcopy = coor;
        if (coor.x < f.property.width - 1 && f.userfield[coor.y][coor.x+1] == COVERED) {
            coorcopy.x = coor.x + 1;
            AutoReveal(f, coorcopy);
        }
        if (coor.x > 0 && f.userfield[coor.y][coor.x-1] == COVERED) {
            coorcopy.x = coor.x - 1;
            AutoReveal(f, coorcopy);
        }
    } else {
        f.userfield[coor.y][coor.x] = surr_mine_num;  // Show number if has mine around.
    }
}

bool CoorCheck(Coordinate coor, FieldProperty f) {
    if (coor.x >= 0 && coor.x < f.width &&
        coor.y >= 0 && coor.y < f.height) 
        return true;
    return false;
}

int CountChosenElements(int **user_f, FieldProperty f, int checkwhat) {
    int i, j, cnt = 0;
    // Transverse the whole array for "checkwhat".
    for (i = 0; i < f.height; ++i)
        for (j = 0; j < f.width; ++j)
            if (user_f[i][j] == checkwhat)
                cnt += 1;
    return cnt;
}

// Sum of the value of surrounding elements devided by value of mine.
// Only mines in data field has value (value of mine).
int CountSurrondingMine(int **data_f, Coordinate coor, FieldProperty f) {
    int i, j, cnt = 0;
    Coordinate coorcopy;
    // All 9 elements, if within the field, check if contains mine.
    for (j = -1; j < 2; ++j) {
        for (i = -1; i < 2; ++i) {
            coorcopy.x = coor.x + i;
            coorcopy.y = coor.y + j;
            if (CoorCheck(coorcopy,f) == true && 
                data_f[coorcopy.y][coorcopy.x] == MINE)
                cnt += 1;
        }
    }
    return cnt;
}

void DisplayField(int **field, FieldProperty f) {
    int i = 0, j = 0;
    
    printf("   ");
    // Print the x-axis.
    for (i = 0; i < f.width; ++i)  {
        printf("%3d ",i);
    }
    printf("\n");
    for (j = 0; j <= f.width; ++j) {
        printf("---|");
    }
    printf("\n");
    // Print the y-axis, then row after row.
    for (i = 0; i < f.height; ++i) {
        printf("%2d |",i);
        for (j = 0; j < f.width; ++j) {
            switch (field[i][j]) {
                case FREE: printf("   |"); break;
                case FLAGGED: printf(" ? |"); break;
                case COVERED: printf(" # |"); break;
                case EXPLODED: printf(" ! |"); break;
                case MINE: printf(" @ |"); break;
                // Number of mines around is printed by default
                default: printf(" %d |",field[i][j]);
                    break;
            }
        }
        printf("\n");
        for (j = 0; j <= f.width; ++j) {
            printf("---|");
        }
        printf("\n");
    }
}

void DisplayLastRunningTime() {
    FILE *fp;
    char c;
    
    fp = fopen("time.txt", "r");
    // No file means no file has been created so is the first time run.
    if (fp == NULL)
        printf("THE EXISTENCE OF THE UNIVERSE!");
    else {
        while (fscanf(fp, "%c", &c) == 1) {
            printf("%c", c);
        }
        fclose(fp);
    }
}

void DisplayResults(Field f, bool result, TimeComsumption t) {
    DisplayField(f.datafield, f.property);
    printf("-----------------------------------------\n");
    if (result == true) {
        printf("         SUCCEED ! :))\n");
        printf(" YOU TOOK %.f SECONDS TO WIN!\n\n", difftime(t.end, t.str));
    } else {
        printf("            FAILED ! :( \n");
        printf(" YOU TOOK %.f SECONDS TO KILL YOURSELF!\n\n", difftime(t.end, t.str));
    }
}

int **InitializeField(FieldProperty f, enum state state) {
    int **field;
    int i,j;
    
    field = (int **)malloc(f.height * sizeof(int*));
    field[0] = (int *)malloc(f.height * f.width * sizeof(int));
    for (i = 1; i < f.height; ++i) {
        field[i] = field[i-1] + f.width;
    }
    for (i = 0; i < f.height; ++i) {
        for (j = 0; j < f.width; ++j)
        field[i][j] = state;  // Assign state.
    }
    return field;
};

void RandomlyInsertMine(int **field, FieldProperty f, Coordinate coor) {
    int i, j, n;

    srand((unsigned)time(0));
    for (n = 0; n < f.num_of_mines; ++n) {
        do {
            i = rand() % f.width;  // The remainder divided by width <= width, >= 0.
            j = rand() % f.height;
        } while (field[j][i] == MINE || (j == coor.y && i == coor.x));  // Exclude the first coor
        field[j][i] = MINE;
    }
}

action ReadAction (void) {
    action act;

    while (1) {
            printf("--- 1.SWEEP --- 2.FLAG --- 3.DEFLAG --- 4.EXPAND ---\n");
            printf("PLEASE SELECT:>");
            scanf("%d", &act);
            if (act == TOFLAG || act == TOSWEEP || act == TODEFLAG || act == TOEXPAND) {
                break;
            }
            printf("UNDEFINED OPERATION!\a\n");
        }
    return act;
}

FieldProperty ReadFieldProperty() {
    FieldProperty f;

    printf("Please enter how big you want the field to be?\n");
    printf("Width: "); scanf("%d", &f.width);
    printf("Height: "); scanf("%d", &f.height);

    // Only accept mine number that will never succeed at the first click!
    printf("please enter how many mines you want :)\n");
    while (1) {
        scanf("%d",&f.num_of_mines);
        if (f.num_of_mines < f.width * f.height - 1 && 
            f.num_of_mines > f.height * f.width / 16 + 1) {
            break;
        } else if (f.num_of_mines <= f.height * f.width / 16 + 1) {
            printf("YOU WANT TOO FEW MINES!\n"
                   "PLEASE TRY MORE MINES!\n");
        } else {
            printf("YOU WAT TOO MUCH MINES!\n"
                   "PLEASE ENTER AGAIN!\n");
        }
    }
    return f;
};

Coordinate ReadTheCoordinate(FieldProperty f) {
    Coordinate coor;

    while (1) {
        printf("x-coordinate: ");
        scanf("%d", &coor.x);
        printf("y-coordinate: ");
        scanf("%d", &coor.y);
        if (CoorCheck(coor, f) != 0) {
            break;  // Terminates if valid valued entered.
        }
        printf("NOT VALID ENTRY!\a\n");
        printf("PLEASE ENTER THE COORDINATE AGAIN:\n");
    }
    return coor;
}

bool RecordLastRunningTime() {
    FILE *fp;
    int status;
    time_t rawtime;
    struct tm *timeinfo; // Default structure contains date and time

    fp = fopen("time.txt", "w");
    if (fp == NULL) {
        return false;
    }
    time(&rawtime);  // The time in sec from 1970/1/1
    timeinfo = localtime(&rawtime); // Converts sec into tm structure(local time)
    fprintf(fp, "%s", asctime(timeinfo)); 
    status = fclose(fp);
    if (status != 0) {
        return false;  // Fails to record.
    }
    return true;
}

int StartOrNot() {
    int start;

    printf("    Do you want to start a new game?\n\n\n");
    while (1) {
        printf("        1--START :)     0--QUIT :(\n\n");
        scanf("%d", &start);
        if (start == 0 || start == 1) {
            break;
        }
        printf("PLEASE ENTER 1 OR 0!\n");
    }
    return start;
};

// If the chosen coor contains a mine, fail. Otherwise, not yet fail.
bool Sweep(Field f, Coordinate coor, int *cleared) {
    if (f.datafield[coor.y][coor.x] == FREE) {
        AutoReveal(f, coor);
        // # of the elements - flag ones - covered ones = the reaveled ones
        *cleared = f.property.width * f.property.height -
                  CountChosenElements(f.userfield, f.property, FLAGGED) -
                  CountChosenElements(f.userfield, f.property, COVERED);
    }
    else if (f.datafield[coor.y][coor.x] == MINE) {
        f.datafield[coor.y][coor.x] = EXPLODED;
        return false;
    } else printf("ERROR!\a\n");
    return true;
}

// If all mine-free elements are revealed, TRUE.
// If mine exploded, FALSE. Otherwise, stay in loop.
bool SweepingFlaggingEnding(Field f) {
    int user_side_remain = f.property.num_of_mines;
    int cleared = f.property.width * f.property.height -
                  CountChosenElements(f.userfield, f.property, FLAGGED) -
                  CountChosenElements(f.userfield, f.property, COVERED);
    Coordinate coor;
    action act;
    
    while (1) {
        // Check first in case user succeeds at the first click!
        if (cleared == f.property.width * f.property.height - f.property.num_of_mines)
            return true;
        // Display
        system("clear");
        DisplayField(f.userfield, f.property);
        printf("MINE REMAINING: %d\n", user_side_remain);
        // Read information from user.
        act = ReadAction();
        printf("PLEASE ENTER THE COORDINATE:\n");
        coor = ReadTheCoordinate(f.property);
        
        // 4 operations. One is chosen each time.
        if (act == TOSWEEP) {
            if (Sweep(f, coor, &cleared) == false)
                return false;
        }
        
        if (act == TOFLAG) {
            if (f.userfield[coor.y][coor.x] == COVERED) {
                f.userfield[coor.y][coor.x] = FLAGGED;
                user_side_remain -= 1;
            } else printf("\a");
        }
        
        if (act == TODEFLAG) {
            if (f.userfield[coor.y][coor.x] == FLAGGED) {
                f.userfield[coor.y][coor.x] = COVERED;
                user_side_remain += 1;
            } else printf("\a");
        }

        if (act == TOEXPAND) {
            Coordinate coorcopy;

            if (f.userfield[coor.y][coor.x] != COVERED) {
                // Sweep all the surrondings except the flagged ones.
                for (int j = -1; j < 2; ++j) {
                    for (int i = -1; i < 2; ++i) {
                        // Make sure original coor will never change.
                        coorcopy.y = coor.y + j;
                        coorcopy.x = coor.x + i;
                        if (CoorCheck(coorcopy, f.property) == true &&
                            f.userfield[coorcopy.y][coorcopy.x] != FLAGGED) {    
                            if (Sweep(f, coorcopy, &cleared) == false) {
                            return false;
                            }
                        }
                    }
                }
            } else printf("\a");
        }
    }
}
