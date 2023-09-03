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
char * readFileOneLine(char *path) {
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

// big_int
void init_big_int(big_int *a, size_t size) {
    a->digits = (int *)calloc(size, sizeof(int));
    a->size = size;
    a->sign = 1;
}

void free_big_int(big_int *a) {
    free(a->digits);
    a->size = 0;
    a->sign = 1;
}

void set_big_int_from_string(big_int *a, int sign, const char *s) {
    size_t len = strlen(s);
    init_big_int(a, len);

    a->sign = sign;
    for (size_t i = 0; i < len; i++)
        a->digits[len - i - 1] = s[i] - '0';
}

int set_int_from_big_int(big_int *a) {
    if (a->size > 9) {
        perror("WRNG: Too big value of bit_int for 64-bit int.");
    }

    int result = 0;
    for (size_t i = 0; i < a->size; i++)
        result += a->digits[i] * pow(10, i);
    if (a->sign != 1)
        result *= -1;

    return result;
}

void subtract_abs_big_int(big_int *a, big_int *b, big_int *result) {
    // Assumes |a| >= |b|
    int borrow = 0;
    init_big_int(result, a->size);

    for (size_t i = 0; i < a->size; i++) {
        int diff = a->digits[i] - borrow - (i < b->size ? b->digits[i] : 0);
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = diff;
    }
}

int compare_abs_big_int(big_int *a, big_int *b) {
    if (a->size != b->size) {
        return a->size > b->size ? 1 : -1;
    }
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return a->digits[i] > b->digits[i] ? 1 : -1;
        }
    }
    return 0;  // They are equal
}

void add_big_int(big_int *a, big_int *b, big_int *result) {
    // If signs are the same, we just add
    if (a->sign == b->sign) {
        int carry = 0;
        size_t maxSize = (a->size > b->size) ? a->size : b->size;
        init_big_int(result, maxSize + 1);

        for (size_t i = 0; i < maxSize; i++) {
            int sum = carry;
            if (i < a->size) sum += a->digits[i];
            if (i < b->size) sum += b->digits[i];

            result->digits[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry) {
            result->digits[maxSize] = carry;
        }
        result->sign = a->sign;  // or b->sign since they are the same
    } else {
        // Different signs, so we subtract
        big_int *larger;
        big_int *smaller;

        int comparison = compare_abs_big_int(a, b);
        if (comparison > 0) {
            larger = a;
            smaller = b;
        } else if (comparison < 0) {
            larger = b;
            smaller = a;
        } else {
            // If both are of same magnitude and opposite signs, result is 0
            init_big_int(result, 1);
            return;
        }

        subtract_abs_big_int(larger, smaller, result);
        result->sign = larger->sign;  // sign of the larger magnitude number
    }
}

void multiply_big_int(big_int *a, big_int *b, big_int *result) {
    big_int temp;
    init_big_int(result, a->size + b->size);

    for (size_t i = 0; i < b->size; i++) {
        int carry = 0;
        for (size_t j = 0; j < a->size || carry; j++) {
            long long cur = result->digits[i + j] +
                            (long long)b->digits[i] * (j < a->size ? a->digits[j] : 0) + carry;
            result->digits[i + j] = (int)(cur % 10);
            carry = (int)(cur / 10);
        }
    }

    result->sign = a->sign * b->sign; // result will be positive if signs are same, otherwise negative
}

void print_big_int(big_int *a) {
    if (a->sign == -1) {
        printf("-");
    }

    int found_first_digit = 0;
    for (int i = a->size - 1; i >= 0; i--) {
        if (!found_first_digit && a->digits[i] != 0) {
            found_first_digit = 1;
        }
        if (found_first_digit) {
            printf("%d", a->digits[i]);
        }
    }
    if (!found_first_digit) {
        printf("0");
    }
    printf("\n");
}

// big_int_arr
void init_big_int_arr(big_int_arr *p, size_t initSize)
{
    p->array = malloc(initSize * sizeof(big_int_arr));
    p->used = 0;
    p->size = initSize;
}

void insert_big_int_arr(big_int_arr *p, big_int *element)
{
    if (p->size == p->used) {
        p->size *= 2;
        p->array = realloc(p->array, p->size * sizeof(int));
    }

    p->array[p->used++] = *element;
}

void free_big_int_arr(big_int_arr *p)
{
    for (int i = 0; i < p->used; i++)
        free_big_int(&(p->array[i]));
    free(p->array);
    p->array = NULL;
    p->size = p->used = 0;
}

// icc
void init_icc(int_code_comp *p_icc, size_t program_size, size_t memory_size) {
    p_icc->program = malloc(sizeof(big_int_arr));
    p_icc->memory = malloc(sizeof(big_int_arr));
    init_big_int_arr(p_icc->program, program_size);
    init_big_int_arr(p_icc->memory, memory_size);
    for (int i = 0; i < p_icc->memory->size; i++) {
        big_int big_int;
        set_big_int_from_string(&big_int, 1, "0");
        insert_big_int_arr(&(p_icc->memory->array[i]), &big_int);
    }
}

int is_program_addr(int_code_comp *p_icc, size_t address) {
    if (address > p_icc->program->used)
        return 0;
    
    return 1;
}

// my_strndup
char *my_strndup(const char *s, size_t n) {
    size_t len = strnlen(s, n);  // find the length of the string up to 'n'
    char *new_str = (char *)malloc(len + 1);  // +1 for the null terminator

    if (new_str == NULL) {
        return NULL;  // memory allocation failed
    }

    new_str[len] = '\0';  // null terminator
    return (char *)memcpy(new_str, s, len);  // copy the string and return
}