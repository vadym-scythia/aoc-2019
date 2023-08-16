#include "util.h"
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 9
#define MAX_ONE_LINE_LENGTH 16000

//String and read file
void initString(String *ptr, size_t initSize) {
    ptr->array = malloc(initSize * sizeof(char));
    ptr->used = 0;
    ptr->size = initSize;
}

void insertString(String *ptr, char element[]) {
    if (ptr->size <= ptr->used + MAX_LINE_LENGTH) {
        ptr->size = ptr->used * 2;
        char *tmp = realloc(ptr->array, ptr->size * sizeof(char));

        if (tmp != NULL) {
            ptr->array = tmp;
        }
    }

    strcpy(ptr->array + ptr->used, element);
    ptr->used += sizeof(element);
}

char *readFileNewLine(char *path) {
    String result;
    initString(&result, 1000);
    char line[MAX_LINE_LENGTH] = {0};

    FILE *file = fopen(path, "r");

    if (!file) {
        perror(path);
    }

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[7] = ',';
        line[8] = '\0';
        insertString(&result, line);
    }

    if (fclose(file)) {
        perror(path);
    }

    result.array[result.used - 1] = '\0';

    return result.array;
}

// add buff
char *  readFileOneLine(char *path) {
    char *ptr_buff;
    char *tmp = malloc(MAX_ONE_LINE_LENGTH * sizeof(char));
    if (tmp != NULL)
        ptr_buff = tmp;
    else {
        perror(tmp);
        return NULL;
    }

    FILE *file = fopen(path, "r");
    if (!file || file == NULL) {
        perror(path);
    }

    fgets(ptr_buff, MAX_ONE_LINE_LENGTH, file);

    if (fclose(file)) {
        perror(path);
    }

    return ptr_buff;
}

//IntArray
void initIntArray(IntArray *ptr, size_t initSize)
{
    ptr->array = malloc(initSize * sizeof(int));
    ptr->used = 0;
    ptr->size = initSize;
}

void insertIntArray(IntArray *ptr, int element)
{
    if (ptr->size == ptr->used)
    {
        ptr->size *= 2;
        ptr->array = realloc(ptr->array, ptr->size * sizeof(int));
    }
    ptr->array[ptr->used++] = element;
}

void freeIntArray(IntArray *ptr)
{
    free(ptr->array);
    ptr->array = NULL;
    ptr->size = ptr->used = 0;
}