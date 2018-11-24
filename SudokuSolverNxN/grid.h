//
//  grid.h
//  SudokuSolverNxN
//
//  Created by Lauri Koskenniemi on 26/09/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

/*
 * Create all grid function here. Might be divided to different c files. (For example grid creation and deleting, id and value manipulation)
 *
 */

#ifndef grid_h
#define grid_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    int value;
    int *cand_values;
    int cand_count;
} Number;

typedef struct {
    Number *numbers;
} Cell;

typedef struct {
    Cell *cell;
    int n;
    int m;
} Grid;


//grid.c
void initGrid(Grid *grid, int n);
void deleteGrid(Grid *grid);
void copyGrid(Grid *from, Grid *to);
// WORKS ON NxN (N=MxM) GRID ONLY!!!
void printGrid(Grid *grid);
//LIMITED TO 9x9 GRID ONLY!!!
void print9Grid(Grid *grid);

//UPDATE 'makeGrid' TO NEW STANDARD! (ssstdv1.0)
//update: Create grid from string, not file. User input in different function (ssstdv1.0 file handler).
// LIMITED TO 9x9 GRID ONLY!!!
int make9Grid(Grid *grid);
int makeGrid(Grid *grid);
//NEW 'makeGrid' FOR STANDARD! (ssstdv1.0)
int newMakeGrid(Grid *grid, char *sudoku);

int saveGridToFile(Grid *grid);
Grid *saveGrid(Grid *grid);
void makeNumberCandidates(Grid *grid);
void printCandidates(Grid *grid);

//grid_methods.c
int getCellID(Grid *grid, int id);
int getNumberID(Grid *grid, int id);
int getID(Grid *grid, int cell_id, int number_id);
int getRow(Grid *grid, int cell_id, int number_id);
int getColumn(Grid *grid, int cell_id, int number_id);
void changeNumberValue(Grid *grid, int id, int num);
int getNumberValue(Grid *grid, int id);
int numberInCell(Grid *grid, int cell_id, int value);
int numberInRow(Grid *grid, int row_index, int value);
int numberInColumn(Grid *grid, int column_index, int value);

#endif /* grid_h */
