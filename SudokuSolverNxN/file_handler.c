//
//  file_handler.c
//  SudokuSolverNxN
//
//  Created by Lauri Koskenniemi on 04/10/2018.
//  Copyright © 2018 Lauri Koskenniemi. All rights reserved.
//

// All methods for ssstdv1.0
#include "file_handler.h"



int getPathFromUser(File *file) {
    char path[80];
    memset(path, '\0', 80);
    printf("Give file path:");
    fflush(stdin);
    scanf("%s", path);
    printf("Filename: %s\n", path);
    
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    } else {
        memset(file->path, '\0', 80);
        strcpy(file->path, path);
    }
    fclose(f);
    return 1;
}



int getSudokuNameFromUser(File *file, char *dest) {
    char sudoku[20];
    printf("Choose sudoku:\n");
    for (int i = 0; i < file->sudoku_count; i++) {
        printf("%s\n", file->sudoku_names[i]);
    }
    
    fflush(stdin);
    scanf("%s", sudoku);
    for (int i = 0; i < file->sudoku_count; i++) {
        if (strcmp(sudoku, file->sudoku_names[i]) == 0) {
            strcpy(dest, sudoku);
            return 1;
        }
    }
    
    return 0;
}



int readVersion(File *file) {
    FILE *f = fopen(file->path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    }
    
    char version[16];
    while (fgets(version, 15, f)) {
        if (strncmp(version, "ssstdv", 6) == 0) {
            int i = 6;
            while (i < 16 && version[i] != ':') {
                i++;
            }
            strncpy(file->version, version, i);
            fclose(f);
            return 1;
        }
    }
    
    fclose(f);
    
    return 0;
}



int readSudokuNames(File *file) {
    FILE *f = fopen(file->path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    }
    
    char buffer[20];
    fgets(buffer, 20, f);
    while (!feof(f)) {
        while (!feof(f) && buffer[0] != '@') {
            fgets(buffer, 20, f);
        }
        if (buffer[0] == '@') {
            char *name = calloc(20, sizeof(char));
            int i = 1;
            while (!isspace(buffer[i]) && buffer[i] != '(' && i < 20) {
                name[i - 1] = buffer[i];
                i++;
            }
            name[19] = '\0';
            
            file->sudoku_count++;
            file->sudoku_names = realloc(file->sudoku_names, sizeof(char *) * file->sudoku_count);
            file->sudoku_names[file->sudoku_count - 1] = (char *)calloc(20, sizeof(char));
            strcpy(file->sudoku_names[file->sudoku_count - 1], name);
            free(name);
        }
        
        fgets(buffer, 20, f);
    }
    
    fclose(f);
    
    return 1;
}



void deleteSudokuNames(File *file) {
    for (int i = 0; i < file->sudoku_count; i++) {
        free(file->sudoku_names[i]);
    }
    file->sudoku_count = 0;
    free(file->sudoku_names);
}



int readSudokuSize(File *file, char *sudoku_name) {
    FILE *f = fopen(file->path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    }
    
    char buffer[20];
    fgets(buffer, 20, f);
    while (!feof(f)) {
        while (!feof(f) && !strstr(buffer, sudoku_name)) {
            fgets(buffer, 20, f);
        }
        char *s = strstr(buffer, sudoku_name);
        if (s != NULL) {
            char *m = strchr(s, '(');
            int i = 0;
            m++;
            while (*(m + i) != ')') {
                i++;
            }
            char size_str[10];
            for (int j = 0; j < i; j++) {
                size_str[j] = *(m + j);
            }
            int size = atoi(size_str);
            
            if (size != 0) {
                fclose(f);
                return size;
            }
        }
        
        fgets(buffer, 20, f);
    }
    
    fclose(f);
    
    return 0;
}



char *readSudokuComment(File *file, char *sudoku_name) {
    FILE *f = fopen(file->path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    }
    
    char buffer[20];
    fgets(buffer, 20, f);
    while (!feof(f)) {
        while (!feof(f) && !(strstr(buffer, sudoku_name) && buffer[0] == '@')) {
            fgets(buffer, 20, f);
        }
        
        while (!strchr(buffer, '}')) {
            fgets(buffer, 20, f);
        }
        
        if (!strchr(buffer, '[')) {
            fclose(f);
            return NULL;
        } else {
            fseek(f, -20, SEEK_CUR);
            int s = fgetc(f);
            int i = 0;
            
            while (s != '[') {
                s = fgetc(f);
                i++;
            }
            int start = i;
            
            while (s != ']') {
                s = fgetc(f);
                i++;
            }
            int end = i;
            
            char *comment = calloc(end - start, sizeof(char));
            fseek(f, -(end - start), SEEK_CUR);
            
            for (int j = 0; j < (end - start) - 1; j++) {
                comment[j] = fgetc(f);
            }
            comment[(end - start) - 1] = '\0';
            
            fclose(f);
            return comment;
        }
    }
    
    fclose(f);
    return NULL;
}



char *readSudoku(File *file, char *sudoku_name) {
    FILE *f = fopen(file->path, "r");
    if (!f) {
        printf("Could not open the file!\n");
        return 0;
    }
    
    char buffer[20];
    fgets(buffer, 20, f);
    while (!feof(f)) {
        while (!feof(f) && !(strstr(buffer, sudoku_name) && buffer[0] == '@')) {
            fgets(buffer, 20, f);
        }
        
        if (strchr(buffer, '{')) {
            int s = fgetc(f);
            int i = 0;
            while (s != '}') {
                s = fgetc(f);
                i++;
            }
            
            fseek(f, -i-1, SEEK_CUR);
            char *sudoku = calloc(i, sizeof(char));
            for (int j = 0; j < i; j++) {
                sudoku[j] = fgetc(f);
            }
            sudoku[i - 1] = '\0';
            
            fclose(f);
            return sudoku;
        } else {
            fclose(f);
            return NULL;
        }
    }
    
    fclose(f);
    return NULL;
}



int ask(char *text, int options) {
    int run = 1;
    while (run) {
        printf("%s", text);
        fflush(stdin);
        int option = 0;
        scanf("%d", &option);
        
        if (option > 0 && option <= options) {
            run = 0;
            return option;
        }
    }
    
    return 0;
}
