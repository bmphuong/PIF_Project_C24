#include <stdio.h>

int main()
{
    int n, a[16];
    float avr = 0;
    //input n
    do
    {
        printf("Input 0 < n <= 16: ");
        scanf("%d", &n);
    } while (n <= 0 || n>16);
    //input the array
    for (int i = 0; i < n; i++)
    {
        printf("a[%d] = ", i);
        scanf("%d", &a[i]);
    }
    //caculate the average
    for (int i = 0; i < n; i++)
        avr += a[i];
    printf("\naverage = %5.3f", avr/n);
    return 0;
}