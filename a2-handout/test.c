#include <stdio.h>

int main() {
    int x = 5;
    printf("x = %d\n", x);
    void* px = &x;
    printf("px = %p\n", px);
    int l = *(int *)px;
    printf("l = %d\n", l);
}
