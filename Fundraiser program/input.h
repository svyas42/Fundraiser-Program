/**
    @file input.h
    @author Sachi Vyas (smvyas)
    A program that: Prototype for the input.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
/**
    Reads a line from the given file
    @param fp the pointer for the file
    @return a pointer to a dynamically allocated memory containing the line
 */
char *readLine( FILE *fp );
