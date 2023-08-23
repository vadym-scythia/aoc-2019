#include "stdio.h"
#include "../libs/util.h"

void fetch(char *ptrToProgramString, IntArray *program);
void decodeAndExecute(Amplifier* amplifier); 

int main() {
    printf("Hello, World!");

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