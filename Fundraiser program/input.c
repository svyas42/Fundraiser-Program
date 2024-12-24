/**
    @file input.c
    @author Sachi Vyas (smvyas)
    A program that: Helps us reads a single line of input from the given input stream (stdin or a file) and returns it as a 
    string inside a block of dynamically allocated memory.
 */
#include "input.h"
#include "group.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
/** Temporary buffer size */
#define BUFFER_SIZE 100
/**
    Reads a line from the given file
    @param fp the pointer for the file
    @return a pointer to a dynamically allocated memory containing the line
 */
char *readLine( FILE *fp ) 
{
    //What buffer size should I set?
    size_t bufferSize = BUFFER_SIZE;
    size_t position = 0;
    char *buffer = malloc(bufferSize);
    if (buffer == NULL) {
        return NULL;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n') {
        if (position >= bufferSize - 1) {
            bufferSize *= DOUBLE_SIZE;
            char *buffer2 = realloc(buffer, bufferSize);
            if (buffer2 == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = buffer2;
        }
        buffer[position++] = (char) ch;
    }

    if (position == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }
    buffer[position] = '\0';
    return buffer;
}
