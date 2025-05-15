#include "stdio.h"

int main() {

    double data[10] = {0};

    printf("sizeof(double) = %ld\n", sizeof(double));
    printf("address of data is %p\n", data);
    for(int i = 0; i < 1000; i++) {
        printf("data[%d] = %f, addr = %p\n", i, data[i], &data[i]);
    }
    printf("success!");
    return 0;
}
