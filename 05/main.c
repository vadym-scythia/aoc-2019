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
    // "3,0,4,0,99\0"
    // "1002,4,3,4,33\0"
    // "1101,100,-1,4,0\0"
    char *programInString = "3,225,1,225,6,6,1100,1,238,225,104,0,1,192,154,224,101,-161,224,224,4,224,102,8,223,223,101,5,224,224,1,223,224,223,1001,157,48,224,1001,224,-61,224,4,224,102,8,223,223,101,2,224,224,1,223,224,223,1102,15,28,225,1002,162,75,224,1001,224,-600,224,4,224,1002,223,8,223,1001,224,1,224,1,224,223,223,102,32,57,224,1001,224,-480,224,4,224,102,8,223,223,101,1,224,224,1,224,223,223,1101,6,23,225,1102,15,70,224,1001,224,-1050,224,4,224,1002,223,8,223,101,5,224,224,1,224,223,223,101,53,196,224,1001,224,-63,224,4,224,102,8,223,223,1001,224,3,224,1,224,223,223,1101,64,94,225,1102,13,23,225,1101,41,8,225,2,105,187,224,1001,224,-60,224,4,224,1002,223,8,223,101,6,224,224,1,224,223,223,1101,10,23,225,1101,16,67,225,1101,58,10,225,1101,25,34,224,1001,224,-59,224,4,224,1002,223,8,223,1001,224,3,224,1,223,224,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1108,226,226,224,102,2,223,223,1005,224,329,101,1,223,223,107,226,226,224,1002,223,2,223,1005,224,344,1001,223,1,223,107,677,226,224,102,2,223,223,1005,224,359,101,1,223,223,7,677,226,224,102,2,223,223,1005,224,374,101,1,223,223,108,226,226,224,102,2,223,223,1006,224,389,101,1,223,223,1007,677,677,224,102,2,223,223,1005,224,404,101,1,223,223,7,226,677,224,102,2,223,223,1006,224,419,101,1,223,223,1107,226,677,224,1002,223,2,223,1005,224,434,1001,223,1,223,1108,226,677,224,102,2,223,223,1005,224,449,101,1,223,223,108,226,677,224,102,2,223,223,1005,224,464,1001,223,1,223,8,226,677,224,1002,223,2,223,1005,224,479,1001,223,1,223,1007,226,226,224,102,2,223,223,1006,224,494,101,1,223,223,1008,226,677,224,102,2,223,223,1006,224,509,101,1,223,223,1107,677,226,224,1002,223,2,223,1006,224,524,1001,223,1,223,108,677,677,224,1002,223,2,223,1005,224,539,1001,223,1,223,1107,226,226,224,1002,223,2,223,1006,224,554,1001,223,1,223,7,226,226,224,1002,223,2,223,1006,224,569,1001,223,1,223,8,677,226,224,102,2,223,223,1006,224,584,101,1,223,223,1008,677,677,224,102,2,223,223,1005,224,599,101,1,223,223,1007,226,677,224,1002,223,2,223,1006,224,614,1001,223,1,223,8,677,677,224,1002,223,2,223,1005,224,629,101,1,223,223,107,677,677,224,102,2,223,223,1005,224,644,101,1,223,223,1108,677,226,224,102,2,223,223,1005,224,659,101,1,223,223,1008,226,226,224,102,2,223,223,1006,224,674,1001,223,1,223,4,223,99,226\0";
    IntArray tempIntArr, programInIntArr;

    char *pointerToProgramInString = programInString;
    initIntArray(&tempIntArr, 1);

    // fetch
    while (*pointerToProgramInString != '\0')
    {
        if (*pointerToProgramInString == ',')
        {
            pointerToProgramInString++;
            continue;
        }

        int negativeModifier = 1;
        if (*pointerToProgramInString == '-')
        {
            negativeModifier = -1;
            pointerToProgramInString++;
        }

        if (*(pointerToProgramInString + 1) == ',' || *(pointerToProgramInString + 1) == '\0')
        {
            insertIntArray(&tempIntArr, negativeModifier * (*pointerToProgramInString - '0'));
            pointerToProgramInString++;
        }
        else if (*(pointerToProgramInString + 2) == ',' || *(pointerToProgramInString + 2) == '\0')
        {
            insertIntArray(&tempIntArr, negativeModifier * (10 * (*pointerToProgramInString - '0') + *(pointerToProgramInString + 1) - '0'));
            pointerToProgramInString += 2;
        }
        else if (*(pointerToProgramInString + 3) == ',' || *(pointerToProgramInString + 3) == '\0')
        {
            insertIntArray(&tempIntArr, negativeModifier * (100 * (*pointerToProgramInString - '0') + 10 * (*(pointerToProgramInString + 1) - '0') + *(pointerToProgramInString + 2) - '0'));
            pointerToProgramInString += 3;
        }
        else if (*(pointerToProgramInString + 4) == ',' || *(pointerToProgramInString + 3) == '\0')
        {
            insertIntArray(&tempIntArr, negativeModifier * (1000 * (*pointerToProgramInString - '0') + 100 * (*(pointerToProgramInString + 1) - '0') + 10 * (*(pointerToProgramInString + 2) - '0') + (*(pointerToProgramInString + 3) - '0')));
            pointerToProgramInString += 4;
        }
        else if (*(pointerToProgramInString + 5) == ',' || *(pointerToProgramInString + 3) == '\0')
        {
            insertIntArray(&tempIntArr, negativeModifier * (10000 * (*pointerToProgramInString - '0') + 1000 * (*(pointerToProgramInString + 1) - '0') + 100 * (*(pointerToProgramInString + 2) - '0') + 10 * (*(pointerToProgramInString + 3) - '0') + *(pointerToProgramInString + 4) - '0'));
            pointerToProgramInString += 5;
        }
    }

    initIntArray(&programInIntArr, tempIntArr.used);
    for (int i = 0; i < programInIntArr.size; i++)
        insertIntArray(&programInIntArr, tempIntArr.array[i]);
    freeIntArray(&tempIntArr);

    // decode
    int operationAddress = 0;
    while (operationAddress <= programInIntArr.size)
    {
        //printf("operation before = %d", programInIntArr.array[operationAddress]);
        //printf(",on 223 = %d\r\n", programInIntArr.array[223]);
        int operation = programInIntArr.array[operationAddress] % 100, firstMode = programInIntArr.array[operationAddress] % 1000 / 100,
            secondMode = programInIntArr.array[operationAddress] % 10000 / 1000, thirdMode = programInIntArr.array[operationAddress] % 100000 / 10000;
       
        if (operation == 99)
        {
            // execute
            break;
        }

        if (operation == 1)
        {
            // execute
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            int operand3 = programInIntArr.array[operationAddress + 3];
            int result = operand1 + operand2;
            programInIntArr.array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 2)
        {
            // execute
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            int operand3 = programInIntArr.array[operationAddress + 3];
            int result = operand1 * operand2;
            programInIntArr.array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 3)
        {
            // execute
            int operand1 = programInIntArr.array[operationAddress + 1];
            int result;
            scanf("%d", &result);
            programInIntArr.array[operand1] = result;
            operationAddress += 2;
            continue;
        }

        if (operation == 4)
        {
            // execute
            int result = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            printf("%d\r\n", result);
            operationAddress += 2;
            continue;
        }
    }

    freeIntArray(&programInIntArr);

    return 0;
}
