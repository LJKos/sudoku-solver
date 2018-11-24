//
//  main.c
//  SudokuSolver
//
//  Created by Lauri Koskenniemi on 30/08/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

/*
 * NxN sudoku solver (N = MxM)
 *
 */
#include <time.h>

#include "grid.h"
#include "file_handler.h"



int leastCandidates(Grid *grid) {
    int least_cell = -1;
    int least_number = -1;
    int least_count = -1;
    
    for (int cell_i = 0; cell_i < grid->n; cell_i++) {
        for (int num_i = 0; num_i < grid->n; num_i++) {
            if (grid->cell[cell_i].numbers[num_i].cand_count > 0 && (grid->cell[cell_i].numbers[num_i].cand_count < least_count || least_count == -1)) {
                least_cell = cell_i;
                least_number = num_i;
                least_count = grid->cell[cell_i].numbers[num_i].cand_count;
            }
        }
    }
    
    if (least_count != -1) {
        return getID(grid, least_cell, least_number);
    } else {
        return -1;
    }
}



int onePossibleAdd(Grid *grid, int least_id) {
    if (least_id != -1) {
        Number *n = &grid->cell[getCellID(grid, least_id)].numbers[getNumberID(grid, least_id)];
        if (n->cand_count == 1) {
            n->value = n->cand_values[0];
        }
    } else {
        return 0;
    }
    
    return 1;
}



int soleCandidate(Grid *grid) {
    int possible = 0;
    int possible_number = 0;
    int possible_cell = 0;
    int possible_value = 0;
    
    for (int cell_i = 0; cell_i < grid->n; cell_i++) {
        for (int value = 1; value < grid->n + 1; value++) {
            if (!numberInCell(grid, cell_i, value)) {
                for (int num_i = 0; num_i < grid->n; num_i++) {
                    if (grid->cell[cell_i].numbers[num_i].value == -1) {
                        if (!numberInRow(grid, getRow(grid, cell_i, num_i), value) && !numberInColumn(grid, getColumn(grid, cell_i, num_i), value)) {
                            possible++;
                            possible_number = num_i;
                            possible_cell = cell_i;
                            possible_value = value;
                        }
                    }
                }
                
                if (possible == 1) {
                    grid->cell[possible_cell].numbers[possible_number].value = possible_value;
                    return 1;
                } else {
                    possible = 0;
                }
            }
        }
    }
    
    return 0;
}



unsigned long long int backtrack(Grid *grid, int limit) {
    Grid *added = calloc(1, sizeof(Grid));
    initGrid(added, grid->n);
    printCandidates(added);
    
    int value = 1;
    int id = 0;
    unsigned long long int tries = 0;
    int run = 1;
    
    while (id < grid->n * grid->n && run) {
        //double bt_runtime_in = (double)clock();
        
        tries++;
        int cell = getCellID(grid, id);
        int number = getNumberID(grid, id);
        int row = getRow(grid, cell, number);
        int column = getColumn(grid, cell, number);
        
        if (getNumberValue(grid, id) == -1) {
            if (!numberInCell(grid, cell, value) && !numberInRow(grid, row, value) && !numberInColumn(grid, column, value)) {
                changeNumberValue(grid, id, value);
                changeNumberValue(added, id, value);
                if (limit != 0 && tries >= limit) {
                    run = 0;
                }
                value = 1;
                id++;
            } else if (value < grid->n) {
                value++;
            } else {
                int last_found = 0;
                int last_number = -1;
                int count = id - 1;
            
                while (!last_found) {
                    while (last_number == -1 && count > -1) {
                        if (getNumberValue(added, count) != -1) {
                            last_number = getNumberValue(added, count);
                        } else {
                            count--;
                        }
                    }
                    
                    for (int i = count; i <= id; i++) {
                        if (i > -1) {
                            
                            if (getNumberValue(added, i) != -1) {
                                changeNumberValue(grid, i, -1);
                            }
                            
                            changeNumberValue(added, i, -1);
                        }
                    }
                
                    if (last_number < grid->n) {
                        value = last_number + 1;
                        id = count;
                        last_found++;
                        changeNumberValue(added, id, -1);
                    } else {
                        changeNumberValue(added, count, -1);
                        last_number = -1;
                        count--;
                    }
                }
            }
        } else {
            id++;
        }
        /*
        if (tries % 1000 == 0) {
            printf("\ntries: %llu\n\n", tries);
        }
         */
        
        //double bt_runtime_out = (double)clock();
        //printf("time used: %lf seconds\n", (bt_runtime_out - bt_runtime_in) / CLOCKS_PER_SEC);
    }
    
    deleteGrid(added);
    free(added);
    
    return tries;
}



int main(int argc, const char * argv[]) {
    int run = 1;
    
    Grid *grid = calloc(1, sizeof(Grid));
    File *file = calloc(1, sizeof(File));
    //printf("%lu\n", sizeof(Cell));
    getPathFromUser(file);
    readSudokuNames(file);
    
    while (run) {
        char sudoku_name[20];
        int val = 0;
        while (1) {
            if (getSudokuNameFromUser(file, sudoku_name)) {
                break;
            } else {
                val = ask("1: Write name\n2: Change file path\n3: Quit\n", 3);
                if (val == 1) {
                    continue;
                } else if (val == 2) {
                    getPathFromUser(file);
                    readSudokuNames(file);
                } else if (val == 3) {
                    printf("Quitting...\n");
                    run = 0;
                    break;
                }
            }
        }
        
        if (!run) {
            break;
        }
        
        printf("Sudoku size: %d\n", readSudokuSize(file, sudoku_name));
        char *s = readSudoku(file, sudoku_name);
        printf("%s\n", s);
        free(s);
        char *comment = readSudokuComment(file, sudoku_name);
        if (comment != NULL) {
            printf("Comment: %s\n", comment);
        }
        printf("\n");
        
        val = ask("1: Solve this sudoku\n2: Change sudoku\n", 2);
        if (val == 1) {
            // solving code here...
            
            val = ask("1: Backtrack\n2: Sole candidate\n3: Least candidates\n4: Sole and least cand.\n5: sole and least cand. to backtrack\n", 5);
            
            initGrid(grid, readSudokuSize(file, sudoku_name));
            char *sudoku = readSudoku(file, sudoku_name);
            newMakeGrid(grid, sudoku);
            free(sudoku);
            printGrid(grid);
            printf("\n");
            
            if (val == 1) {
                
                double solve_in = (double)clock();
                unsigned long long tries = backtrack(grid, 0);
                double solve_out = (double)clock();
                printGrid(grid);
                printf("\n");
                printf("Tries %llu\n", tries);
                printf("Solving time: %f\n", (solve_out - solve_in) / CLOCKS_PER_SEC);
                
            } else if (val == 2) {
                
                while (1) {
                    if (!soleCandidate(grid)) {
                        break;
                    }
                }
                printGrid(grid);
                
            } else if (val == 3) {
                
                while (1) {
                    makeNumberCandidates(grid);
                    int id = leastCandidates(grid);
                    
                    if (id != -1) {
                        Number *n = &grid->cell[getCellID(grid, id)].numbers[getNumberID(grid, id)];
                        if (n->cand_count == 1) {
                            n->value = n->cand_values[0];
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                printGrid(grid);
                
            } else if (val == 4) {
                
                while (1) {
                    if (!soleCandidate(grid)) {
                        makeNumberCandidates(grid);
                        int id = leastCandidates(grid);
                        
                        if (id != -1) {
                            Number *n = &grid->cell[getCellID(grid, id)].numbers[getNumberID(grid, id)];
                            if (n->cand_count == 1) {
                                n->value = n->cand_values[0];
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }
                printGrid(grid);
                
            } else if (val == 5) {
                
                while (1) {
                    if (!soleCandidate(grid)) {
                        makeNumberCandidates(grid);
                        int id = leastCandidates(grid);
                        
                        if (id != -1) {
                            Number *n = &grid->cell[getCellID(grid, id)].numbers[getNumberID(grid, id)];
                            if (n->cand_count == 1) {
                                n->value = n->cand_values[0];
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }
                
                backtrack(grid, 2000000);
                printGrid(grid);
            }
            
            deleteGrid(grid);
            
            // solving code here...^
        } else if (val == 2) {
            printf("2\n");
        }
        
        val = ask("1: Continue\n2: Quit\n", 2);
        if (val == 1) {
            val = ask("1: Choose sudoku from this file\n2: Choose new file path\n", 2);
            if (val == 1) {
                printf("1\n");
            } else if (val == 2) {
                printf("2\n");
                deleteSudokuNames(file);
                getPathFromUser(file);
                readSudokuNames(file);
            }
        } else if (val == 2) {
            printf("Quitting...\n");
            run = 0;
        }
    }
    
    deleteSudokuNames(file);
    free(grid);
    free(file);
    printf("Grids and sudokus deleted.\n");
    
    return 0;
}
