//
//  grid_methods.c
//  SudokuSolverNxN
//
//  Created by Lauri Koskenniemi on 04/10/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

#include "grid.h"

int getCellID(Grid *grid, int id) {
    int i = id / grid->n;
    int j = id % grid->n;
    int m = i / grid->m;
    int n = j / grid->m;
    return n + grid->m * m;
}



int getNumberID(Grid *grid, int id) {
    int i = id / grid->n;
    int j = id % grid->n;
    int m = i % grid->m;
    int n = j % grid->m;
    return n + grid->m * m;
}



int getID(Grid *grid, int cell_id, int number_id) {
    int cell_m = cell_id / grid->m;
    int number_m = number_id / grid->m;
    int row = number_m + grid->m * cell_m;
    
    int cell_n = cell_id % grid->m;
    int number_n = number_id % grid->m;
    int column = number_n + grid->m * cell_n;
    
    return column + grid->n * row;
}



int getRow(Grid *grid, int cell_id, int number_id) {
    int cell_m = cell_id / grid->m;
    int number_m = number_id / grid->m;
    return number_m + grid->m * cell_m;
}



int getColumn(Grid *grid, int cell_id, int number_id) {
    int cell_n = cell_id % grid->m;
    int number_n = number_id % grid->m;
    return number_n + grid->m * cell_n;
}



void changeNumberValue(Grid *grid, int id, int num) {
    int cell_id = getCellID(grid, id);
    int number_id = getNumberID(grid, id);
    
    grid->cell[cell_id].numbers[number_id].value = num;
}



int getNumberValue(Grid *grid, int id) {
    int cell_id = getCellID(grid, id);
    int number_id = getNumberID(grid, id);
    
    return grid->cell[cell_id].numbers[number_id].value;
}



int numberInCell(Grid *grid, int cell_id, int value) {
    for (int i = 0; i < grid->n; i++) {
        if (grid->cell[cell_id].numbers[i].value == value) {
            return 1;
        }
    }
    
    return 0;
}



int numberInRow(Grid *grid, int row_index, int value) {
    int cell_start = (row_index / grid->m) * grid->m;
    
    for (int cell_n = cell_start; cell_n < cell_start + grid->m; cell_n++) {
        int num_start = (row_index % grid->m) * grid->m;
        
        for (int num_n = num_start; num_n < num_start + grid->m; num_n++) {
            
            if (grid->cell[cell_n].numbers[num_n].value == value) {
                return 1;
            }
        }
    }
    
    return 0;
}



int numberInColumn(Grid *grid, int column_index, int value) {
    int cell_start = column_index / grid->m;
    
    for (int cell_m = cell_start; cell_m < cell_start + grid->n; cell_m += grid->m) {
        int num_start = column_index % grid->m;
        
        for (int num_m = num_start; num_m < num_start + grid->n; num_m += grid->m) {
            
            if (grid->cell[cell_m].numbers[num_m].value == value) {
                return 1;
            }
        }
    }
    
    return 0;
}
