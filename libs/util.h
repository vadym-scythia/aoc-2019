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
typedef struct {
    int *array;
    size_t used;
    size_t size;
} IntArray;

void initIntArray(IntArray *ptr, size_t initSize);
void insertIntArray(IntArray *ptr, int element);
void freeIntArray(IntArray *ptr);

// big_int
typedef struct {
    int *digits;
    size_t size;
    int sign; // 1 or -1
} big_int;

void init_big_int(big_int *a, size_t size);
void free_big_int(big_int *a);
void set_big_int_from_string(big_int *a, int sign, const char *s);
int set_int_from_big_int(big_int *a);
void subtract_abs_big_int(big_int *a, big_int *b, big_int *result);
int compare_abs_big_int(big_int *a, big_int *b);
void add_big_int(big_int *a, big_int *b, big_int *result);
void multiply_big_int(big_int *a, big_int *b, big_int *result);
void print_big_int(big_int *a);

// big_int_arr
typedef struct {
    big_int *array;
    size_t used;
    size_t size;
} big_int_arr;

void init_big_int_arr(big_int_arr *p, size_t initSize);
void insert_big_int_arr(big_int_arr *p, big_int *element);
void free_big_int_arr(big_int_arr *ptr);

// int_code_comp
typedef struct {
    big_int_arr *program;
    big_int_arr *memory;
} int_code_comp;

void init_icc(int_code_comp *p, size_t program_size, size_t memory_size);
int is_program_addr(int_code_comp *p, size_t address);

// my_strndup
char *my_strndup(const char *s, size_t n);