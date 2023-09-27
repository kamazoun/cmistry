#include<stdio.h>

#define ELEMENTS 78

int main()
{
    int numb;
    numb = ELEMENTS;
    printf("The number of elements is %d", numb);

    char symbol;
    while(symbol = getchar(), symbol != '1'){
        printf("%c", symbol);
    }

    int x = 1;
    int y = 0;
    int t;
    t = x, x = y, y = t;
    printf("%d", x);

    printf("\n");
    return 1;
}