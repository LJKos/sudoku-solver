//
//  grid.c
//  SudokuSolverNxN
//
//  Created by Lauri Koskenniemi on 26/09/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

#include "grid.h"

void initGrid(Grid *grid, int n) {
    grid->n = n;
    grid->m = sqrt(n);
    grid->cell = calloc(n, sizeof(Cell));
    
    for (int i = 0; i < n; i++) {
        grid->cell[i].numbers = calloc(n, sizeof(Number));
        
        for (int j = 0; j < n; j++) {
            grid->cell[i].numbers[j].value = -1;
            grid->cell[i].numbers[j].cand_count = 0;
            grid->cell[i].numbers[j].cand_values = calloc(1, sizeof(int));
            grid->cell[i].numbers[j].cand_values[0] = 0;
        }
    }
}



void deleteGrid(Grid *grid) {
    for (int i = 0; i < grid->n; i++) {
        for (int j = 0; j < grid->n; j++) {
            free(grid->cell[i].numbers[j].cand_values);
        }
        
        free(grid->cell[i].numbers);
    }
    
    free(grid->cell);
}



void copyGrid(Grid *from, Grid *to) {
    for (int i = 0; i < from->n; i++) {
        for (int j = 0; j < from->n; j++) {
            to->cell[i].numbers[j].value = from->cell[i].numbers[j].value;
        }
    }
}



// WORKS ON NxN (N=MxM) GRID ONLY!!!
void printGrid(Grid *grid) {
    for (int id = 0; id < grid->n * grid->n; id++) {
        if (getNumberValue(grid, id) == -1) {
            printf("  ");
        } else {
            printf("%2d", getNumberValue(grid, id));
        }
        
        if (((id + 1) / grid->m) % grid->n == 0 && (id + 1) % grid->n == 0 && id + 1 < grid->n * grid->n) {
            printf("\n");
            for (int i = 0; i < grid->m; i++) {
                for (int j = 0; j < grid->m; j++) {
                    printf("--");
                }
                
                if (i != grid->m - 1) {
                    printf("+");
                }
            }
        } else if ((id + 1) % grid->m == 0 && (id + 1) % grid->n != 0) {
            printf("|");
        }
        
        if ((id + 1) % grid->n == 0) {
            printf("\n");
        }
    }
}



//LIMITED TO 9x9 GRID ONLY!!!
void print9Grid(Grid *grid) {
    for (int i = 0; i < grid->n * grid->n; i++) {
        if (getNumberValue(grid, i) == -1) {
            printf(" ");
        } else {
            printf("%d", getNumberValue(grid, i));
        }
        
        if (((i + 1) / grid->m) % grid->n == 0 && (i + 1) % grid->n == 0 && i + 1 < grid->n * grid->n) {
            printf("\n");
            for (int i = 0; i < grid->m; i++) {
                for (int j = 0; j < grid->m; j++) {
                    printf("-");
                }
                
                if (i != grid->m - 1) {
                    printf("+");
                }
            }
        } else if ((i + 1) % grid->m == 0 && (i + 1) % grid->n != 0) {
            printf("|");
        }
        
        if ((i + 1) % grid->n == 0) {
            printf("\n");
        }
    }
}


//NOTICE UPDATE (may not require changes for 9x9 only)
// LIMITED TO 9x9 GRID ONLY!!!
int make9Grid(Grid *grid) {
    char name[80];
    printf("Give file path:");
    scanf("%s", name);
    printf("Filename: %s\n", name);
    
    FILE *f = fopen(name, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    } else {
        char line[grid->n + 1];
        char buffer[2];
        int i = 0;
        while (fgets(line, grid->n + 1, f) != NULL && i < grid->n) {
            int j = 0;
            while (line[j] != '\n' && j < grid->n) {
                if (isdigit(line[j]) && line[j] != '0') {
                    buffer[0] = line[j];
                    changeNumberValue(grid, j + i * grid->n, atoi(buffer));
                }
                j++;
            }
            if (line[j] == '\n') {
                continue;
            }
            i++;
        }
    }
    fclose(f);
    
    return 1;
}


//NOTICE UPDATE!!!
int makeGrid(Grid *grid) {
    char name[80];
    printf("Give file path:");
    scanf("%s", name);
    printf("Filename: %s\n", name);
    
    FILE *f = fopen(name, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    } else {
        char line[grid->n + 1];
        char buffer[2];
        int i = 0;
        
        int char_offset = 'a' - 10;
        
        while (fgets(line, grid->n + 1, f) != NULL && i < grid->n) {
            int j = 0;
            while (line[j] != '\n' && j < grid->n) {
                if (isdigit(line[j]) && line[j] != '0') {
                    buffer[0] = line[j];
                    changeNumberValue(grid, j + i * grid->n, atoi(buffer));
                } else if (strchr("abcdefghijklmnopqrstuvwxyz", line[j]) != NULL) {
                    int value = line[j] - char_offset;
                    changeNumberValue(grid, j + i * grid->n, value);
                }
                j++;
            }
            if (line[j] == '\n') {
                continue;
            }
            i++;
        }
    }
    fclose(f);
    
    return 1;
}



int newMakeGrid(Grid *grid, char *sudoku) {
    unsigned long len = strlen(sudoku);
    int char_offset = 'a' - 10;
    char buffer[2];
    int index = 0;
    
    for (int i = 0; i < len; i++) {
        if (isdigit(sudoku[i]) && sudoku[i] != '0') {
            buffer[0] = sudoku[i];
            changeNumberValue(grid, index, atoi(buffer));
            index++;
        } else if (strchr("abcdefghijklmnopqrstuvwxyz", sudoku[i]) != NULL) {
            int value = sudoku[i] - char_offset;
            changeNumberValue(grid, index, value);
            index++;
        } else if (sudoku[i] == ' ') {
            index++;
        }
    }
    
    return 1;
}



int saveGridToFile(Grid *grid) {
    
    //code here
    
    return 0;
}



Grid *saveGrid(Grid *grid) {
    Grid *g = calloc(1, sizeof(Grid));
    initGrid(g, grid->n);
    
    for (int id = 0; id < grid->n * grid->n; id++) {
        int value = getNumberValue(grid, id);
        if (value != -1) {
            changeNumberValue(g, id, value);
        }
    }
    
    return g;
}



void makeNumberCandidates(Grid *grid) {
    for (int cell_i = 0; cell_i < grid->n; cell_i++) {
        for (int num_i = 0; num_i < grid->n; num_i++) {
            grid->cell[cell_i].numbers[num_i].cand_count = 0;
            Number *n = &grid->cell[cell_i].numbers[num_i];
            n->cand_values = realloc(n->cand_values, sizeof(int));
        }
    }
    
    for (int cell_i = 0; cell_i < grid->n; cell_i++) {
        for (int num_i = 0; num_i < grid->n; num_i++) {
            if (grid->cell[cell_i].numbers[num_i].value == -1) {
                for (int value = 1; value < grid->n + 1; value++) {
                    if (!numberInCell(grid, cell_i, value)) {
                        if (!numberInRow(grid, getRow(grid, cell_i, num_i), value) && !numberInColumn(grid, getColumn(grid, cell_i, num_i), value)) {
                            Number *n = &grid->cell[cell_i].numbers[num_i];
                            n->cand_count++;
                            n->cand_values = realloc(n->cand_values, sizeof(int) * n->cand_count);
                            n->cand_values[n->cand_count - 1] = value;
                        }
                    }
                }
            }
        }
    }
}



void printCandidates(Grid *grid) {
    for (int id = 0; id < grid->n * grid->n; id++) {
        Number *n = &grid->cell[getCellID(grid, id)].numbers[getNumberID(grid, id)];
        if (n->cand_count < 1) {
            continue;
        }
        printf("id:%3d, (cell:%2d, number:%2d), candidates:", id, getCellID(grid, id), getNumberID(grid, id));
        for (int i = 0; i < n->cand_count; i++) {
            printf("%2d", n->cand_values[i]);
        }
        
        printf("\n");
    }
    printf("\n");
}
