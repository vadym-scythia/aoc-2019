#include "stdio.h"
#include "../libs/util.h"

void fetch(char *p_program_string, IntArray **p_program);
void decode_execute(int_code_comp *p_program); 

int main(int argc, char **argv) {
    char *p_program_string = readFileOneLine(argv[1]);
    int_code_comp *p_icc;
    init_icc(p_icc, 1, 1);

    fetch(p_program_string, &p_icc->program);
    decode_execute(p_icc);
    free(p_icc);

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

    for (int i = 0; i < temp_program.used; i++)
        insertIntArray(*pp_program, temp_program.array[i]);
    freeIntArray(&temp_program);
}

void decode_execute(int_code_comp *p_icc) 
{
    IntArray *p_program = p_icc->program;
    IntArray *p_memory = p_icc->memory; 
    int operationAddress = 0;
    int relative_base = 0;
    while (operationAddress <= p_program->size)
    {
        // decode
        int operation = p_program->array[operationAddress] % 100, operand1_mode = p_program->array[operationAddress] % 1000 / 100,
            operand2_mode = p_program->array[operationAddress] % 10000 / 1000, operand3_mode = p_program->array[operationAddress] % 100000 / 10000;
       
        if (operation == 99) {
            // execute
            break;
        }

        if (operation == 1) {
            // execute
            // todo: extract operand getting to separate function
            int operand1, operand2, operand3, result;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];
            
            operand3 = p_program->array[operationAddress + 3];
            result = operand1 + operand2;
            p_program->array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 2) {
            // execute
            int operand1, operand2, operand3, result;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];

            operand3 = p_program->array[operationAddress + 3];
            result = operand1 * operand2;
            p_program->array[operand3] = result;
            operationAddress += 4;
            continue;
        }

        if (operation == 3) {
            // execute
            int operand1 = p_program->array[operationAddress + 1];
            int result;
            scanf("%d", &result);
            p_program->array[operand1] = result;
            operationAddress += 2;
            continue;
        }

        if (operation == 4) {
            // execute
            int result;
            
            if (operand1_mode == 0)
                result = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                result = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                result = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            printf("%d\r\n", result);
            operationAddress += 2;
            continue;
        }

        if (operation == 5) {
            // execute
            int operand1, operand2;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];

            if (operand1 != 0)
                operationAddress = operand2;
            else
                operationAddress += 3;

            continue;
        }

        if (operation == 6) {
            int operand1, operand2;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];

            if (operand1 == 0)
                operationAddress = operand2;
            else 
                operationAddress += 3;

            continue;
        }

        if (operation == 7) {
            int operand1, operand2, operand3;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];

            operand3 = p_program->array[operationAddress + 3];
            if (operand1 < operand2)
                p_program->array[operand3] = 1;
            else
                p_program->array[operand3] = 0;

            operationAddress += 4;
            continue;
        }

        if (operation == 8) {
            int operand1, operand2, operand3;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            if (operand2_mode == 0)
                operand2 = p_program->array[p_program->array[operationAddress + 2]];
            else if (operand2_mode == 1)
                operand2 = p_program->array[operationAddress + 2];
            else if (operand2_mode == 2)
                operand2 = p_program->array[relative_base + p_program->array[operationAddress + 2]];
            
            operand3 = p_program->array[operationAddress + 3];
            if (operand1 == operand2)
                p_program->array[operand3] = 1;
            else
                p_program->array[operand3] = 0;
            
            operationAddress += 4;
            continue;
        }

        if (operation == 9) {
            int operand1;
            
            if (operand1_mode == 0)
                operand1 = p_program->array[p_program->array[operationAddress + 1]];
            else if (operand1_mode == 1)
                operand1 = p_program->array[operationAddress + 1];
            else if (operand1_mode == 2)
                operand1 = p_program->array[relative_base + p_program->array[operationAddress + 1]];

            relative_base += operand1;
            operationAddress += 2;
            continue;
        }
    }
}
