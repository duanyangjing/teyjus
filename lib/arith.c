// compile as shared library by:
//   gcc -shared -fpic -o arith.so arith.c

#include <stdio.h>
#include <math.h>
#include "../source/simulator/cinterface.h"

int fact(int x) 
{
    if (x == 0) return 1;
    int prod = 1;
    int i;
    for (i = 1; i <= x; i++) {
        prod *= i;
    }
    return prod;
}

void fact_wrapper() 
{
    int a1 = TJ_getInt(1);
    int ret = fact(a1);
    TJ_returnInt(2, ret);
}


// ----------------- sine --------------
double sine(double x) 
{
    return sin(x);
}

void sine_wrapper() 
{
    // teyjus terms to c float
    float a1 = TJ_getReal(1);
    
    // call function
    float ret = (float)sine((double)a1);
    TJ_returnReal(2, ret);
}


int add(int x, int y) 
{
    return x + y;
}

void add_wrapper() 
{
    // teyjus terms to c int
    int a1 = TJ_getInt(1);
    int a2 = TJ_getInt(2);
    // call function
    int ret = add(a1, a2);
    TJ_returnInt(3, ret);
}
