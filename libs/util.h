#include <stdlib.h>

//String and read file
typedef struct {
    char *array;
    size_t used;
    size_t size;
} String;

void initString(String *ptr, size_t initSize);
void insertString(String *ptr, char element[]);
char *readFileNewLine(char *path);
char *readFileOneLine(char *path);

//IntArray
typedef struct
{
    int *array;
    size_t used;
    size_t size;
} IntArray;

void initIntArray(IntArray *ptr, size_t initSize);
void insertIntArray(IntArray *ptr, int element);
void freeIntArray(IntArray *ptr);