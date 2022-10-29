#include <stdio.h>

int main(int argc, char **argv[])
{
    int mass;
    int fuel;
    int result;
    char filePath[300];
    FILE *fp;

    printf("Enter file path:");
    gets(filePath);

    if (strlen(filePath) < 1)
    {
        printf("Error in file path!");

        return 1;
    }

    fp = fopen(filePath, "r");

    if (fp == NULL)
    {
        printf("Error opening file!");

        return 1;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%d", &mass);
        fuel = ((int)mass / 3) - 2;
        result += fuel;
    }

    printf("Result: %d \r\n", result);
    fclose(fp);

    return 0;
}