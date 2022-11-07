#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    size_t used;
    size_t size;
} IntArray;

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

int main(int argc, char **argv[])
{
    // optional todo: read and update the input from the file.
    char *programInString = "1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,13,19,1,9,19,23,1,6,23,27,2,27,9,31,2,6,31,35,1,5,35,39,1,10,39,43,1,43,13,47,1,47,9,51,1,51,9,55,1,55,9,59,2,9,59,63,2,9,63,67,1,5,67,71,2,13,71,75,1,6,75,79,1,10,79,83,2,6,83,87,1,87,5,91,1,91,9,95,1,95,10,99,2,9,99,103,1,5,103,107,1,5,107,111,2,111,10,115,1,6,115,119,2,10,119,123,1,6,123,127,1,127,5,131,2,9,131,135,1,5,135,139,1,139,10,143,1,143,2,147,1,147,5,0,99,2,0,14,0\0";
    IntArray tempIntArr, programInIntArr;

    for (int noun = 0; noun <= 99; noun++)
    {
        for (int verb = 0; verb <= 99; verb++)
        {
            char *pointerToProgramInString = programInString;
            initIntArray(&tempIntArr, 1);

            // fetch
            while (*pointerToProgramInString != '\0')
            {
                if (*pointerToProgramInString != ',')
                {
                    if (*(pointerToProgramInString + 1) == ',' || *(pointerToProgramInString + 1) == '\0')
                    {
                        insertIntArray(&tempIntArr, *pointerToProgramInString - '0');
                        pointerToProgramInString++;
                    }
                    else if (*(pointerToProgramInString + 2) == ',' || *(pointerToProgramInString + 2) == '\0')
                    {
                        insertIntArray(&tempIntArr, 10 * (*pointerToProgramInString - '0') + *(pointerToProgramInString + 1) - '0');
                        pointerToProgramInString += 2;
                    }
                    else if (*(pointerToProgramInString + 3) == ',' || *(pointerToProgramInString + 3) == '\0')
                    {
                        insertIntArray(&tempIntArr, 100 * (*pointerToProgramInString - '0') + 10 * (*(pointerToProgramInString + 1) - '0') + *(pointerToProgramInString + 2) - '0');
                        pointerToProgramInString += 3;
                    }
                }
                else
                    pointerToProgramInString++;
            }

            tempIntArr.array[1] = noun;
            tempIntArr.array[2] = verb;

            initIntArray(&programInIntArr, tempIntArr.used);

            for (int i = 0; i < programInIntArr.size; i++)
                insertIntArray(&programInIntArr, tempIntArr.array[i]);

            freeIntArray(&tempIntArr);

            // decode
            for (int i = 0; i <= programInIntArr.size; i += 4)
            {
                if (programInIntArr.array[i] == 99)
                {
                    // execute
                    break;
                }

                if (programInIntArr.array[i] == 1)
                {
                    // execute
                    int operandAddress1 = programInIntArr.array[i + 1];
                    int operandAddress2 = programInIntArr.array[i + 2];
                    int resultAddress = programInIntArr.array[i + 3];
                    int result = programInIntArr.array[operandAddress1] + programInIntArr.array[operandAddress2];
                    programInIntArr.array[resultAddress] = result;
                }

                if (programInIntArr.array[i] == 2)
                {
                    // execute
                    int operandAddress1 = programInIntArr.array[i + 1];
                    int operandAddress2 = programInIntArr.array[i + 2];
                    int resultAddress = programInIntArr.array[i + 3];
                    int result = programInIntArr.array[operandAddress1] * programInIntArr.array[operandAddress2];
                    programInIntArr.array[resultAddress] = result;
                }
            }

            // the answer is 4925 (100 * noun + verb)
            if (programInIntArr.array[0] == 19690720){
                printf("noun: %d\r\n", noun);
                printf("verb: %d", verb);
            }

            freeIntArray(&programInIntArr);
        }
    }

    return 0;
}
