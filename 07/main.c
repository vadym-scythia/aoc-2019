#include <stdio.h>
#include <string.h>
#include "../libs/util.h"

typedef struct
{
    int output;
    int currentOperationAddress;
    int input[2];
    int isFirstCycle;
    IntArray* program;
} Amplifier;

void initAmplifier(Amplifier* amplifier, IntArray* initProgram);
void fetch(char *ptrToProgramString, IntArray *program);
void decodeAndExecute(Amplifier* amplifier); 
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

    IntArray programInIntArr, tempProgram;
    initIntArray(&tempProgram, 1);
    char *ptrToProgramString = readFileOneLine(argv[1]);
    fetch(ptrToProgramString, &tempProgram);
    initIntArray(&programInIntArr, tempProgram.used);
    for (int i = 0; i < programInIntArr.size; i++)
        insertIntArray(&programInIntArr, tempProgram.array[i]);
    freeIntArray(&tempProgram);

    IntArray** settingSequences = generateSettingSequences();
    int result = 0;
    for (int i = 0; i < 120; i++)
    {
        int output = 0;
        Amplifier a, b, c, d, e;
        initAmplifier(&a, &programInIntArr);
        initAmplifier(&b, &programInIntArr);
        initAmplifier(&c, &programInIntArr);
        initAmplifier(&d, &programInIntArr);
        initAmplifier(&e, &programInIntArr);
        while (e.program->array[e.currentOperationAddress] != 99)
        {
            if (a.isFirstCycle == 1)
            {
                a.input[0] = settingSequences[i]->array[0];
                a.input[1] = output;
            }
            else
                a.input[0] = output;
            decodeAndExecute(&a);
            output = a.output;
            a.isFirstCycle = 0;

            if (b.isFirstCycle == 1)
            {
                b.input[0] = settingSequences[i]->array[1];
                b.input[1] = output;
            }
            else
                b.input[0] = output;
            decodeAndExecute(&b);
            output = b.output;
            b.isFirstCycle = 0;

            if (c.isFirstCycle == 1)
            {
                c.input[0] = settingSequences[i]->array[2];
                c.input[1] = output;
            }
            else
                c.input[0] = output;
            decodeAndExecute(&c);
            output = c.output;
            c.isFirstCycle = 0;

            if (d.isFirstCycle == 1)
            {
                d.input[0] = settingSequences[i]->array[3];
                d.input[1] = output;
            }
            else
                d.input[0] = output;
            decodeAndExecute(&d);
            output = d.output;
            d.isFirstCycle = 0;

            if (e.isFirstCycle == 1)
            {
                e.input[0] = settingSequences[i]->array[4];
                e.input[1] = output;
            }
            else
                e.input[0] = output;
            decodeAndExecute(&e);
            output = e.output;
            e.isFirstCycle = 0;
        }
        if (output > result)
            result = output;
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

void decodeAndExecute(Amplifier* amplifier) 
{
    int inputOpCalls = 0;
    for (int i = 0; i < amplifier->program->used; i++)
    {
        printf("%d,", amplifier->program->array[i]);
    }
    printf("\n");
    while (amplifier->currentOperationAddress <= amplifier->program->used)
    {
        int operation = amplifier->program->array[amplifier->currentOperationAddress] % 100, firstMode = amplifier->program->array[amplifier->currentOperationAddress] % 1000 / 100,
            secondMode = amplifier->program->array[amplifier->currentOperationAddress] % 10000 / 1000, thirdMode = amplifier->program->array[amplifier->currentOperationAddress] % 100000 / 10000;
       
        if (operation == 99)
        {
            break;
        }

        if (operation == 1)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            int operand3 = amplifier->program->array[amplifier->currentOperationAddress + 3];
            int result = operand1 + operand2;
            amplifier->program->array[operand3] = result;
            amplifier->currentOperationAddress += 4;
            continue;
        }

        if (operation == 2)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            int operand3 = amplifier->program->array[amplifier->currentOperationAddress + 3];
            int result = operand1 * operand2;
            amplifier->program->array[operand3] = result;
            amplifier->currentOperationAddress += 4;
            continue;
        }

        if (operation == 3)
        {
            if ((amplifier->isFirstCycle == 1 && inputOpCalls > 1) || (amplifier->isFirstCycle == 0 && inputOpCalls > 0))
                break;
            int operand1 = amplifier->program->array[amplifier->currentOperationAddress + 1];
            int result = amplifier->input[inputOpCalls];
            amplifier->program->array[operand1] = result;
            amplifier->currentOperationAddress += 2;
            inputOpCalls++;
            continue;
        }

        if (operation == 4)
        {
            int result = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            printf("%d\r\n", result);
            amplifier->currentOperationAddress += 2;
            amplifier->output = result;
            continue;
        }

        if (operation == 5)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            if (operand1 != 0)
                amplifier->currentOperationAddress = operand2;
            else
                amplifier->currentOperationAddress += 3;
            continue;
        }

        if (operation == 6)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            if (operand1 == 0)
                amplifier->currentOperationAddress = operand2;
            else 
                amplifier->currentOperationAddress += 3;
            continue;
        }

        if (operation == 7)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            int operand3 = amplifier->program->array[amplifier->currentOperationAddress + 3];
            if (operand1 < operand2)
                amplifier->program->array[operand3] = 1;
            else
                amplifier->program->array[operand3] = 0;
            amplifier->currentOperationAddress += 4;
            continue;
        }

        if (operation == 8)
        {
            int operand1 = firstMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 1] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 1]];
            int operand2 = secondMode == 1 ? amplifier->program->array[amplifier->currentOperationAddress + 2] : amplifier->program->array[amplifier->program->array[amplifier->currentOperationAddress + 2]];
            int operand3 = amplifier->program->array[amplifier->currentOperationAddress + 3];
            if (operand1 == operand2)
                amplifier->program->array[operand3] = 1;
            else
                amplifier->program->array[operand3] = 0;
            amplifier->currentOperationAddress += 4;
            continue;
        }
        amplifier->currentOperationAddress = amplifier->currentOperationAddress;
    }
}

IntArray** generateSettingSequences()
{
    IntArray** store = (IntArray**) malloc(sizeof(IntArray*) * 120);
    IntArray* ptrSequence = (IntArray*) malloc(sizeof(IntArray));
    initIntArray(ptrSequence, 5);
    for (int i = 0; i < 5; ++i) 
    {
        insertIntArray(ptrSequence, i + 5);
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
        printf("%d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
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

void initAmplifier(Amplifier* amplifier, IntArray* initProgram)
{
    amplifier->output = 0;
    amplifier->currentOperationAddress = 0;
    amplifier->isFirstCycle = 1;
    amplifier->program = copyIntArray(initProgram);
}