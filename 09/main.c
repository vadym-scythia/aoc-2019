#include "stdio.h"
#include "../libs/util.h"

void fetch(char *p_program_string, IntArray **p_program);
void decodeAndExecute(IntArray *p_program); 

int main(int argc, char **argv) {
    char *p_program_string = readFileOneLine(argv[1]);
    IntArray *p_program;

    fetch(p_program_string, &p_program);
    decodeAndExecute(p_program);
    freeIntArray(p_program);

    return 0;
}

void fetch(char *p_program_string, IntArray **pp_program)
{
    IntArray temp_program;
    initIntArray(&temp_program, 1);

    while (*p_program_string != '\0')
    {
        if (*p_program_string == ',' || *p_program_string == ' ')
        {
            p_program_string++;
            continue;
        }

        int negativeModifier = 1;
        if (*p_program_string == '-')
        {
            negativeModifier = -1;
            p_program_string++;
        }

        if (*(p_program_string + 1) == ',' || *(p_program_string + 1) == '\0')
        {
            insertIntArray(&temp_program, negativeModifier * (*p_program_string - '0'));
            p_program_string++;
        }
        else if (*(p_program_string + 2) == ',' || *(p_program_string + 2) == '\0')
        {
            insertIntArray(&temp_program, negativeModifier * (10 * (*p_program_string - '0') + *(p_program_string + 1) - '0'));
            p_program_string += 2;
        }
        else if (*(p_program_string + 3) == ',' || *(p_program_string + 3) == '\0')
        {
            insertIntArray(&temp_program, negativeModifier * (100 * (*p_program_string - '0') + 10 * (*(p_program_string + 1) - '0') + *(p_program_string + 2) - '0'));
            p_program_string += 3;
        }
        else if (*(p_program_string + 4) == ',' || *(p_program_string + 4) == '\0')
        {
            insertIntArray(&temp_program, negativeModifier * (1000 * (*p_program_string - '0') + 100 * (*(p_program_string + 1) - '0') + 10 * (*(p_program_string + 2) - '0') + (*(p_program_string + 3) - '0')));
            p_program_string += 4;
        }
        else if (*(p_program_string + 5) == ',' || *(p_program_string + 5) == '\0')
        {
            insertIntArray(&temp_program, negativeModifier * (10000 * (*p_program_string - '0') + 1000 * (*(p_program_string + 1) - '0') + 100 * (*(p_program_string + 2) - '0') + 10 * (*(p_program_string + 3) - '0') + *(p_program_string + 4) - '0'));
            p_program_string += 5;
        }
    }

    initIntArray(*pp_program, temp_program.used);
    for (int i = 0; i < (*pp_program)->size; i++)
        insertIntArray(*pp_program, temp_program.array[i]);
    freeIntArray(&temp_program);
}

void decodeAndExecute(IntArray *p_program) 
{
    int operationAddress = 0;
    while (operationAddress <= p_program->size)
    {
        int operation = p_program->array[operationAddress] % 100, firstMode = p_program->array[operationAddress] % 1000 / 100,
            secondMode = p_program->array[operationAddress] % 10000 / 1000, thirdMode = p_program->array[operationAddress] % 100000 / 10000;
       
        if (operation == 99)
        {
            // execute
            break;
        }

        if (operation == 1)
        {
            // execute
            // todo: extract operand getting to separate function
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            int operand3 = p_program->array[operationAddress + 3];
            int result = operand1 + operand2;
            p_program->array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 2)
        {
            // execute
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            int operand3 = p_program->array[operationAddress + 3];
            int result = operand1 * operand2;
            p_program->array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 3)
        {
            // execute
            int operand1 = p_program->array[operationAddress + 1];
            int result;
            scanf("%d", &result);
            p_program->array[operand1] = result;
            operationAddress += 2;
            continue;
        }

        if (operation == 4)
        {
            // execute
            int result = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            printf("%d\r\n", result);
            operationAddress += 2;
            continue;
        }

        if (operation == 5)
        {
            // execute
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            if (operand1 != 0)
                operationAddress = operand2;
            else
                operationAddress += 3;
            continue;
        }

        if (operation == 6)
        {
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            if (operand1 == 0)
                operationAddress = operand2;
            else 
                operationAddress += 3;
            continue;
        }

        if (operation == 7)
        {
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            int operand3 = p_program->array[operationAddress + 3];
            if (operand1 < operand2)
                p_program->array[operand3] = 1;
            else
                p_program->array[operand3] = 0;
            operationAddress += 4;
            continue;
        }

        if (operation == 8)
        {
            int operand1 = firstMode == 1 ? p_program->array[operationAddress + 1] : p_program->array[p_program->array[operationAddress + 1]];
            int operand2 = secondMode == 1 ? p_program->array[operationAddress + 2] : p_program->array[p_program->array[operationAddress + 2]];
            int operand3 = p_program->array[operationAddress + 3];
            if (operand1 == operand2)
                p_program->array[operand3] = 1;
            else
                p_program->array[operand3] = 0;
            operationAddress += 4;
            continue;
        }
    }
}
