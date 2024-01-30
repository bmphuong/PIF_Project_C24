#include <stdio.h>
#include <stdint.h>

int main()
{
    uint64_t a[16];
    uint64_t n = 0;
    //input n
    do
    {
        printf("input n: ");
        scanf("%d", &n);
    } while (n<=0 || n>16);
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
    //print the address
    for (int i = 0; i < n; i++)
    {
        printf("\naddress of a[%d] is: %p", i, &a[i]);
    }
    return 0;
}