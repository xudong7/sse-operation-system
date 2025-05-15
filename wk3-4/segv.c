#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int *ptr;

    puts("before invalid access");
    
    ptr = (int *)0x1234567899999;
    *ptr = 0;

    puts("after invalid access");
    exit(EXIT_SUCCESS);
}