#include "stdio.h"
#include "../libs/util.h"

// todo:
// add big numbers support

// This virtual machine is the IntCode computer, that supports big integers. 
void fetch(char *p_program_string, big_int_arr **p_program);
void decode_execute(int_code_comp *p_program); 

int main(int argc, char **argv) {
    // big_int a, b, sum, product;

    // set_big_int_from_string(&a, 1, "12345");
    // set_big_int_from_string(&b, -1, "6789");

    // printf("%d", set_int_from_big_int(&a));


    // big_int_arr arr;
    // init_big_int_arr(&arr, 2);
    // insert_big_int_arr(&arr, &a);
    // insert_big_int_arr(&arr, &b);

    // add_big_int(&(arr.array[0]), &(arr.array[1]), &sum);
    // multiply_big_int(&(arr.array[0]), &(arr.array[1]), &product);

    // printf("Sum: ");
    // print_big_int(&sum);
    // printf("Product: ");
    // print_big_int(&product);

    // char *s1 = "Hello!";
    // char *s2 = my_strndup(s1, 1);

    char *p_program_string = readFileOneLine(argv[1]);
    int_code_comp *p_icc;
    init_icc(p_icc, 1, 1000);

    fetch(p_program_string, &p_icc->program);
    decode_execute(p_icc);
    free(p_icc);

    return 0;
}

void fetch(char *p_program_string, big_int_arr **pp_program)
{
    big_int_arr temp_program;
    init_big_int_arr(&temp_program, 1);

    while (*p_program_string != '\0')
    {
        if (*p_program_string == ',' || *p_program_string == ' ') {
            p_program_string++;
            continue;
        }

        int sign = 1;
        if (*p_program_string == '-') {
            sign = -1;
            p_program_string++;
        }

        if (*(p_program_string + 1) == ',' || *(p_program_string + 1) == '\0') {
            // Could be instruction or data.
            big_int int_code;
            set_big_int_from_string(&int_code, sign, my_strndup(p_program_string, 1));
            insert_big_int_arr(&temp_program, &int_code);
            p_program_string++;
        }
        else if (*(p_program_string + 2) == ',' || *(p_program_string + 2) == '\0') {
            big_int int_code;
            set_big_int_from_string(&int_code, sign, my_strndup(p_program_string, 2));
            insert_big_int_arr(&temp_program, &int_code);
            p_program_string += 2;
        }
        else if (*(p_program_string + 3) == ',' || *(p_program_string + 3) == '\0') {
            big_int int_code;
            set_big_int_from_string(&int_code, sign, my_strndup(p_program_string, 3));
            insert_big_int_arr(&temp_program, &int_code);
            p_program_string += 3;
        }
        else if (*(p_program_string + 4) == ',' || *(p_program_string + 4) == '\0') {
            big_int int_code;
            set_big_int_from_string(&int_code, sign, my_strndup(p_program_string, 4));
            insert_big_int_arr(&temp_program, &int_code);
            p_program_string += 4;
        }
        else if (*(p_program_string + 5) == ',' || *(p_program_string + 5) == '\0') {
            big_int int_code;
            set_big_int_from_string(&int_code, sign, my_strndup(p_program_string, 5));
            insert_big_int_arr(&temp_program, &int_code);
            p_program_string += 5;
        }
    }

    // for (int i = 0; i < temp_program.used; i++)
    //     insertIntArray(*pp_program, temp_program.array[i]);
    freeIntArray(&temp_program);
}

void decode_execute(int_code_comp *p_icc) {
    IntArray *p_program = p_icc->program;
    IntArray *p_memory = p_icc->memory; 
    int operation_addr = 0;
    int relative_base = 0;
    while (operation_addr <= p_program->size) {
        // decode
        int operation = p_program->array[operation_addr] % 100, operand1_mode = p_program->array[operation_addr] % 1000 / 100,
            operand2_mode = p_program->array[operation_addr] % 10000 / 1000, operand3_mode = p_program->array[operation_addr] % 100000 / 10000;

        // execute
        // stop       
        if (operation == 99) {
            break;
        }

        // add
        if (operation == 1) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base),
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base),
            operand3 = p_program->array[operation_addr + 3],
            result = operand1 + operand2;
            set_result(&p_icc, operand3, result);
            operation_addr += 4;
            continue;
        }

        // multiply 
        if (operation == 2) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base),
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base),
            operand3 = p_program->array[operation_addr + 3],
            result = operand1 * operand2;
            set_result(&p_icc, operand3, result);
            operation_addr += 4;
            continue;
        }

        // input
        if (operation == 3) {
            int result, operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base);
            scanf("%d", &result);    
            set_result(&p_icc, operand1, result);
            operation_addr += 2;
            continue;
        }

        // output 
        if (operation == 4) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base);
            printf("%d\r\n", operand1);
            operation_addr += 2;
            continue;
        }

        // jump-if-true: if the first parameter is non-zero, it sets the instruction pointer to the value from the second parameter. Otherwise, it does nothing.
        if (operation == 5) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base), 
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base);
            if (operand1 != 0)
                operation_addr = operand2;
            else
                operation_addr += 3;

            continue;
        }

        // jump-if-false: if the first parameter is zero, it sets the instruction pointer to the value from the second parameter. Otherwise, it does nothing.
        if (operation == 6) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base),
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base);
            if (operand1 == 0)
                operation_addr = operand2;
            else 
                operation_addr += 3;

            continue;
        }

        // less than: if the first parameter is less than the second parameter, it stores 1 in the position given by the third parameter. Otherwise, it stores 0.
        if (operation == 7) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base),
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base), 
            operand3 = p_program->array[operation_addr + 3];
            if (operand1 < operand2)
                set_result(&p_icc, operand3, 1);
            else
                set_result(&p_icc, operand3, 0);

            operation_addr += 4;
            continue;
        }

        // equals: if the first parameter is equal to the second parameter, it stores 1 in the position given by the third parameter. Otherwise, it stores 0.
        if (operation == 8) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base),
            operand2 = get_operand(&p_icc, operand2_mode, operation_addr + 2, relative_base),
            operand3 = p_program->array[operation_addr + 3];
            if (operand1 == operand2)
                set_result(&p_icc, operand3, 1);
            else
                set_result(&p_icc, operand3, 0);
            
            operation_addr += 4;
            continue;
        }

        if (operation == 9) {
            int operand1 = get_operand(&p_icc, operand1_mode, operation_addr + 1, relative_base);
            relative_base += operand1;
            operation_addr += 2;
            continue;
        }
    }
}

int get_operand(int_code_comp **p_icc, int operand_mode, int operation_addr, int relative_base) {
    big_int_arr *p_memory_type;
    int address, operand;

    if (operand_mode == 0) {
        // from big_int to int for adresses
        if (is_program_addr((*p_icc), set_int_from_big_int(&(*p_icc)->program->array[operation_addr]))) {
            p_memory_type = (*p_icc)->program;
            address = operation_addr;
        }
        else {
            p_memory_type = (*p_icc)->memory;
            address = set_int_from_big_int(&(*p_icc)->program->array[operation_addr]) - (*p_icc)->program->used;
        }
        
        operand = set_int_from_big_int(&p_memory_type->array[address]);
    }
    else if (operand_mode == 1) {
        if (is_program_addr((*p_icc), operation_addr)) {
            p_memory_type = (*p_icc)->program;
            address = operation_addr;
        }
        else {
            p_memory_type = (*p_icc)->memory;
            address = set_int_from_big_int(&(*p_icc)->program->array[(operation_addr)]) - (*p_icc)->program->used;
        }
        
        operand = set_int_from_big_int(&p_memory_type->array[address]);
    }
    else if (operand_mode == 2) {
        if (is_program_addr((*p_icc), relative_base + set_int_from_big_int(&(*p_icc)->program->array[operation_addr]))) {
            p_memory_type = (*p_icc)->program;
            address = operation_addr;
        }
        else {
            p_memory_type = (*p_icc)->memory;
            address = set_int_from_big_int(&(*p_icc)->program->array[operation_addr]) - (*p_icc)->program->used;
        }
        
        operand = set_int_from_big_int(&p_memory_type->array[relative_base + set_int_from_big_int(&p_memory_type->array[address])]);
    }

    return operand;
}

void set_result(int_code_comp **p_icc, int operand, int result) {
    IntArray *p_memory_type;
    int address;

    if (is_program_addr(*p_icc, operand)) {
        p_memory_type = (*p_icc)->program;
        address = operand;
    }
    else {
        p_memory_type = (*p_icc)->memory;
        address = operand - (*p_icc)->program->used;
    }

    p_memory_type->array[address] = result;
}
