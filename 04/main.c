#include <stdio.h>

int main()
{
    const int minRange = 100000, maxRange = 999999;
    int leftRange = 123257, rightRange = 647015, counter = 0;

    while (leftRange <= rightRange)
    {
        if (leftRange < minRange || leftRange > maxRange || rightRange < minRange || rightRange > maxRange)
        {
            leftRange++;
            continue;
        }

        int number1 = leftRange % 1000000 / 100000, number2 = leftRange % 100000 / 10000, number3 = leftRange % 10000 / 1000,
            number4 = leftRange % 1000 / 100, number5 = leftRange % 100 / 10, number6 = leftRange % 10;

        if (!(number1 <= number2 && number2 <= number3 && number3 <= number4 && number4 <= number5 && number5 <= number6))
        {
            leftRange++;
            continue;
        }

        if (!(number1 == number2 || number2 == number3 || number3 == number4 || number4 == number5 || number5 == number6))
        {
            leftRange++;
            continue;
        }

        counter++;
        leftRange++;
    }

    printf("result: %d", counter);

    return 0;
}