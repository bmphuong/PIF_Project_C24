#include <stdio.h>

int max (int * a, int n);   //prototype of max function
int min (int * a, int n);   //prototype of min function
int main()
{
    int n, a[16];
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
    //print the array
    printf("the array is: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    //print the max, min
    printf("\nmax = %d", max(a, n));
    printf("\nmin = %d", min(a, n));
    return 0;
}
// max function
int max (int * a, int n)
{
    int max = a[0];
    for (int i = 0; i < n; i++)
        if (a[i] > max)
            max = a[i];
    return max;
}
// min function
int min (int * a, int n)
{
    int min = a[0];
    for (int i = 0; i < n; i++)
        if (a[i] < min)
            min = a[i];
    return min;
}