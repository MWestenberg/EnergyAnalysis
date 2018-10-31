#include <stdio.h>

void method1(){
    printf("m1\n");
};
void method2(){printf("m2\n");};


void loop(int *arr, size_t size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] %2 == 0) method1();
        else method2();
    }
}


int main(int argc, const char * argv[]) {
    int a[] = { 1 , 2 , 3 , 4  };
    int s = sizeof a / sizeof *a;
    loop(a, s);
    return 0;
}
