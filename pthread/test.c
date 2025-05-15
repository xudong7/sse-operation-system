#include <stdio.h>
#include <pthread.h>

volatile int balence = 0;

void *mythread(void *arg) {
    int i;
    for (i = 0; i < 2000000; i++) {
        balence = balence + 1;
    }
    printf("%d\n", balence);
    return NULL;
}

void test_1() {
    pthread_t p1, p2;
    pthread_create(&p1, NULL, mythread, (void *)"A");
    pthread_join(p1, NULL);
    pthread_create(&p2, NULL, mythread, (void *)"B");
    pthread_join(p2, NULL);
    printf("final balence: %d\n", balence);
}

void test_2() {
    pthread_t p1, p2;
    pthread_create(&p1, NULL, mythread, (void *)"A");
    pthread_create(&p2, NULL, mythread, (void *)"B");
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("final balence: %d\n", balence);
}

int main(int argc, char *argv[]) {
    // test_1(); // no error
    test_2(); // error: multi threads access the same variable
    return 0;
}