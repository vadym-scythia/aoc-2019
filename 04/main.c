#include <stdio.h>

#define pairsCount 5

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
 
        int groupArr[pairsCount] = { number1 * 10 + number2, number2 * 10 + number3, number3 * 10 + number4,
                                     number4 * 10 + number5, number5 * 10 + number6, };
        
        for (int i = 0; i < pairsCount; i++)
        {
            int firstNumber = groupArr[i] % 100 / 10, secondNumber = groupArr[i] % 10,
                rightNeighbour = -1, leftNeighbour = -1;

            if (firstNumber == secondNumber)
            {
                if (i == 0)
                    rightNeighbour = groupArr[i + 1] % 10;
                else if (i == pairsCount - 1)
                    leftNeighbour = groupArr[i - 1] % 100 / 10;
                else 
                {
                    rightNeighbour = groupArr[i + 1] % 10;
                    leftNeighbour = groupArr[i - 1] % 100 / 10;
                }

                if (!(firstNumber == rightNeighbour || firstNumber == leftNeighbour))
                    goto endWithCounter;
            }
        }

        goto end;

        endWithCounter: counter++;
        end: leftRange++;
    }

    printf("result: %d", counter);

    return 0;
}