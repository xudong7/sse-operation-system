#include <stdio.h>
#include <ucontext.h>

int x = 0;
ucontext_t context, *cp = &context;

void func(void) {
    x++;
    setcontext(cp); // switch context, goto 13 line "getcontext(cp)"
}

int main() {
    getcontext(cp);
    if (!x) {
        printf("getcontext!\n");
        func();
    } else {
        printf("setcontext!\n");
    }
    printf("finish!\n");
    return 0;
}