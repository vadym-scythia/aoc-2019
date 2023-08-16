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

    printf("result: %d", num_1 * num_2);

    for (int i = 0; i < pixels_tall; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}