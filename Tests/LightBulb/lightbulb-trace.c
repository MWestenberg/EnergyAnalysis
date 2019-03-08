//
//  trace.c
//  LightBulb
//
//  Created by Mark Westenberg on 07/12/2018.
//  Copyright © 2018 Mark Westenberg. All rights reserved.
//


#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


void turnOn();
void turnOff();
void delay() { sleep(5);}



void loop(int d[], size_t n)
{
    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0) {
            turnOn();
            delay();
        }
        turnOff();
        delay();
    }
}



int main(int argc, const char * argv[]) {
    int d[17] = { 120, 60, 30, 50, 20, 44, 12,55, 22,56,75, 5, 2, 6, 74, 67, 33};
    size_t n = (sizeof(d) / sizeof(d[0]));
    
    loop(d, n);
    
    return 0;
}
