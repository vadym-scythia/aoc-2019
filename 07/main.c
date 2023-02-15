#include <stdio.h>
#include <string.h>
#include "../libs/util.h"

void fetch(char *ptrToProgramString, IntArray *program);
int decodeAndExecute(IntArray program, int input[]);
IntArray** generateSettingSequences();
IntArray* copyIntArray(const IntArray* src);
void permute(IntArray*** store, int* indexStore, IntArray* ptrIntArr, int left, int right);
void swap(int* a, int* b);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Please provide a file path as an only one command line argument!");
        return 1;
    }

    IntArray tempProgram;
    initIntArray(&tempProgram, 1);
    char *ptrToProgramString = readFileOneLine(argv[1]);
    fetch(ptrToProgramString, &tempProgram);

    IntArray programInIntArr;
    initIntArray(&programInIntArr, tempProgram.used);
    for (int i = 0; i < programInIntArr.size; i++)
        insertIntArray(&programInIntArr, tempProgram.array[i]);
    freeIntArray(&tempProgram);

    int input[2], amplifiersAmount = 5, result = 0;
    IntArray** settingSequences = generateSettingSequences();
    for (int i = 0; i < 120; i++)
    {
        int output = 0;
        for (int j = 0; j < amplifiersAmount; j++) 
        {
            input[0] = settingSequences[i]->array[j];
            input[1] = output;

            output = decodeAndExecute(programInIntArr, input);
            if (output > result)
                result = output;
        }
    } 

    freeIntArray(&programInIntArr);
    printf("result: %d", result);
    return 0;
}

void fetch(char *ptrToProgramString, IntArray *program)
{
    while (*ptrToProgramString != '\0')
    {
        if (*ptrToProgramString == ',')
        {
            ptrToProgramString++;
            continue;
        }

        int negativeModifier = 1;
        if (*ptrToProgramString == '-')
        {
            negativeModifier = -1;
            ptrToProgramString++;
        }

        if (*(ptrToProgramString + 1) == ',' || *(ptrToProgramString + 1) == '\0')
        {
            insertIntArray(program, negativeModifier * (*ptrToProgramString - '0'));
            ptrToProgramString++;
        }
        else if (*(ptrToProgramString + 2) == ',' || *(ptrToProgramString + 2) == '\0')
        {
            insertIntArray(program, negativeModifier * (10 * (*ptrToProgramString - '0') + *(ptrToProgramString + 1) - '0'));
            ptrToProgramString += 2;
        }
        else if (*(ptrToProgramString + 3) == ',' || *(ptrToProgramString + 3) == '\0')
        {
            insertIntArray(program, negativeModifier * (100 * (*ptrToProgramString - '0') + 10 * (*(ptrToProgramString + 1) - '0') + *(ptrToProgramString + 2) - '0'));
            ptrToProgramString += 3;
        }
        else if (*(ptrToProgramString + 4) == ',' || *(ptrToProgramString + 4) == '\0')
        {
            insertIntArray(program, negativeModifier * (1000 * (*ptrToProgramString - '0') + 100 * (*(ptrToProgramString + 1) - '0') + 10 * (*(ptrToProgramString + 2) - '0') + (*(ptrToProgramString + 3) - '0')));
            ptrToProgramString += 4;
        }
        else if (*(ptrToProgramString + 5) == ',' || *(ptrToProgramString + 5) == '\0')
        {
            insertIntArray(program, negativeModifier * (10000 * (*ptrToProgramString - '0') + 1000 * (*(ptrToProgramString + 1) - '0') + 100 * (*(ptrToProgramString + 2) - '0') + 10 * (*(ptrToProgramString + 3) - '0') + *(ptrToProgramString + 4) - '0'));
            ptrToProgramString += 5;
        }
    }
}

int decodeAndExecute(IntArray programInIntArr, int input[]) 
{
    int output = 0, inputOpCalls = 0, operationAddress = 0;
    while (operationAddress <= programInIntArr.size)
    {
        int operation = programInIntArr.array[operationAddress] % 100, firstMode = programInIntArr.array[operationAddress] % 1000 / 100,
            secondMode = programInIntArr.array[operationAddress] % 10000 / 1000, thirdMode = programInIntArr.array[operationAddress] % 100000 / 10000;
       
        if (operation == 99)
        {
            break;
        }

        if (operation == 1)
        {
            // todo: extract getting of operand to separate function
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
            int operand1 = programInIntArr.array[operationAddress + 1];
            int result = input[inputOpCalls];
            programInIntArr.array[operand1] = result;
            operationAddress += 2;
            inputOpCalls++;
            continue;
        }

        if (operation == 4)
        {
            int result = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            printf("%d\r\n", result);
            operationAddress += 2;
            output = result;
            continue;
        }

        if (operation == 5)
        {
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            if (operand1 != 0)
                operationAddress = operand2;
            else
                operationAddress += 3;
            continue;
        }

        if (operation == 6)
        {
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            if (operand1 == 0)
                operationAddress = operand2;
            else 
                operationAddress += 3;
            continue;
        }

        if (operation == 7)
        {
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            int operand3 = programInIntArr.array[operationAddress + 3];
            if (operand1 < operand2)
                programInIntArr.array[operand3] = 1;
            else
                programInIntArr.array[operand3] = 0;
            operationAddress += 4;
            continue;
        }

        if (operation == 8)
        {
            int operand1 = firstMode == 1 ? programInIntArr.array[operationAddress + 1] : programInIntArr.array[programInIntArr.array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? programInIntArr.array[operationAddress + 2] : programInIntArr.array[programInIntArr.array[operationAddress + 2]];
            int operand3 = programInIntArr.array[operationAddress + 3];
            if (operand1 == operand2)
                programInIntArr.array[operand3] = 1;
            else
                programInIntArr.array[operand3] = 0;
            operationAddress += 4;
            continue;
        }
    }
    return output;
}

IntArray** generateSettingSequences()
{
    IntArray** store = (IntArray**) malloc(sizeof(IntArray*) * 120);
    IntArray* ptrSequence = (IntArray*) malloc(sizeof(IntArray));
    initIntArray(ptrSequence, 5);
    for (int i = 0; i < 5; ++i) 
    {
        insertIntArray(ptrSequence, i);
    }
    int index = 0;
    permute(&store, &index, ptrSequence, 0, ptrSequence->used - 1);
    freeIntArray(ptrSequence);
    return store;
}

void permute(IntArray*** store, int* indexStore, IntArray* ptrIntArr, int left, int right) 
{
    int* arr = ptrIntArr->array;
    if (left == right)
    {
        //printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
        (*store)[(*indexStore)++] = copyIntArray(ptrIntArr);
    }
    else 
    {
        for (int i = left; i <= right; i++) 
        {
            swap((arr + left), (arr + i));
            permute(store, indexStore, ptrIntArr, left + 1, right);
            swap((arr + left), (arr + i));  // backtrack
        }
    }
}

void swap(int* a, int* b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

IntArray* copyIntArray(const IntArray* src) 
{
    IntArray* dest = (IntArray*) malloc(sizeof(IntArray));
    initIntArray(dest, src->used);
    memcpy(dest->array, src->array, src->used * sizeof(int));
    dest->used = src->used;
    return dest;
}