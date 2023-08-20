#include "stdio.h"
#include "../libs/util.h"

int main(int argc, char **argv) {
    int const pixels_wide = 25, pixels_tall = 6; 
    int layer_size = pixels_wide * pixels_tall;
    char *input = readFileOneLine(argv[1]);
    
    int ** arr = NULL;
    int num_cols = 0, num_layer = 0, zeros_amount = 0, fewest_zeros_amount = 0, num_fewest_zero_layer = 0;
    arr = malloc((num_layer + 1) * sizeof(int *));

    for(int i = 0; i < strlen(input); i++) {
        if (i % layer_size == 0) {
            if (i != 0) {
                arr = realloc(arr, (num_layer + 1) * sizeof(int *));
                if (fewest_zeros_amount == 0) {
                    fewest_zeros_amount = zeros_amount;
                    num_fewest_zero_layer = num_layer;
                } else if (fewest_zeros_amount > zeros_amount) {
                    fewest_zeros_amount = zeros_amount;
                    num_fewest_zero_layer = num_layer - 1;
                }

                zeros_amount = 0;
            }
                
            arr[num_layer] = malloc(layer_size * sizeof(int));
            num_layer++;
        }

        int digit = input[i] - '0';
        if (digit == 0)
            zeros_amount++;

        arr[num_layer - 1][num_cols] = digit;
        num_cols++;
        if (num_cols == layer_size)
            num_cols = 0;
    }

    for (int i = 0; i < num_layer; i++) {
        for (int j = 0; j < layer_size; j++)
            printf("%d", arr[i][j]);
        printf("\n");
    }

    printf("fewest_zeros_amount: %d\n", fewest_zeros_amount);
    printf("num_fewest_zero_layer: %d\n", num_fewest_zero_layer);

    int num_1 = 0, num_2 = 0;

    for (int j = 0; j < layer_size; j++) {
        if (arr[num_fewest_zero_layer][j] == 1) {
            num_1++;
        }

        if (arr[num_fewest_zero_layer][j] == 2) {
            num_2++;
        }
    }

    printf("result1: %d\n", num_1 * num_2);

    // Part 2.
    // change if layer_size is not as in original puzzle
    int arr2[150], flag = 1;

    for (int i = 0; i < layer_size; i++) {
        int incr = i, layer = 0;
        while (flag) {
            if (arr[layer][i] == 0 || arr[layer][i] == 1) {
                arr2[i] = arr[layer][i];
                break;
            } else {
                incr += layer_size;
                layer++;
            }
        }
    }

    printf("result2:\n");
    for (int i = 0; i < layer_size; i++) {
        if (i % 25 == 0 && i != 0) 
            printf("\n");
        printf("%d ", arr2[i]);
    }

    for (int i = 0; i < pixels_tall; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}