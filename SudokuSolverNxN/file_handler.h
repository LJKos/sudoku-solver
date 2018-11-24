//
//  file_handler.h
//  SudokuSolverNxN
//
//  Created by Lauri Koskenniemi on 04/10/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

// All methods for ssstdv1.0
#ifndef file_handler_h
#define file_handler_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char path[80];
    char version[16];
    int sudoku_count;
    char **sudoku_names;
} File;



int getPathFromUser(File *file);
int getSudokuNameFromUser(File *file, char *dest);
int readVersion(File *file);
int readSudokuNames(File *file);
int readSudokuSize(File *file, char *sudoku_name);
char *readSudokuComment(File *file, char *sudoku_name);
void deleteSudokuNames(File *file);
char *readSudoku(File *file, char *sudoku_name);
int ask(char *text, int options);

#endif /* file_handler_h */
