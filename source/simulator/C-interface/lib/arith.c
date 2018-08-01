// compile as shared library by:
//   gcc -shared -fpic -o arith.so arith.c ../../builtins/evalexp.c
//gcc -shared -fpic -o arith.so arith.c ../../abstmachine.c ../../builtins/evalexp.c ../../hnorm.c ../../../system/error.c ../../hnormlocal.c ../../dataformats.c

#include <stdio.h>
#include <math.h>
#include "../../builtins/evalexp.h"
#include "../interface.h"

/*
 * goal fact X Y has argregs (A1 A2) set up already,
 * these wrapper should later be auto-generated.
 * assume last arg reg stores the return
*/
/* void fact_wrapper(AM_DataTypePtr *args) { */
/*     // teyjus terms to c int */
/*     DF_TermPtr t1 = (DF_TermPtr)args[0]; */
/*     int a1 = BIEVAL_evalInt(t1); */
    
/*     // call function */
/*     int ret = fact(a1); */
/*     printf("fact result: %d\n", ret); */
/*     //if (ret == a2) AM_preg = AM_eqCode; */
/*     //else EM_THROW(EM_FAIL); */
/* } */

void fact_wrapper() {
    // teyjus terms to c int
    DF_TermPtr t1 = (DF_TermPtr)AM_reg(1);
    DF_TermPtr t2 = (DF_TermPtr)AM_reg(2);
    int a1 = BIEVAL_evalInt(t1);
    int a2 = BIEVAL_evalInt(t2);
    
    // call function
    int ret = fact(a1);
    printf("fact result: %d\n", ret);
    if (ret == a2) AM_preg = AM_cpreg;
    else EM_THROW(EM_FAIL);
}

int fact(int i) {
    if (i == 0) return 1;
    return i * fact(i-1);
}


// ----------------- sine --------------
double sine(double x) {       
    return sin(x);
}

/* Void sine_wrapper(AM_DataTypePtr *args) { */
/*     // teyjus terms to c float */
/*     DF_TermPtr t1 = (DF_TermPtr)args[0]; */
/*     float a1 = BIEVAL_evalFloat(t1); */
    
/*     // call function */
/*     float ret = (float)sine((double)a1); */
/*     printf("sine result: %f\n", ret); */
/*     //if (ret == a2) AM_preg = AM_eqCode; */
/*     //else EM_THROW(EM_FAIL); */
/* } */

void sine_wrapper() {
    // teyjus terms to c float
    DF_TermPtr t1 = (DF_TermPtr)AM_reg(1);
    DF_TermPtr t2 = (DF_TermPtr)AM_reg(2);
    float a1 = BIEVAL_evalFloat(t1);
    float a2 = BIEVAL_evalFloat(t2);
    
    // call function
    float ret = (float)sine((double)a1);
    printf("sine result: %f\n", ret);
    if (ret == a2) AM_preg = AM_cpreg;
    else EM_THROW(EM_FAIL);
}



// ----------------- add ------------------
/* void add_wrapper(AM_DataTypePtr *args) { */
/*     // teyjus terms to c int */
/*     DF_TermPtr t1 = (DF_TermPtr)args[0]; */
/*     int a1 = BIEVAL_evalInt(t1); */
/*     DF_TermPtr t2 = (DF_TermPtr)args[1]; */
/*     int a2 = BIEVAL_evalInt(t2); */
/*     // call function */
/*     int ret = add(a1, a2); */
/*     printf("add result: %d\n", ret); */
/*     //if (ret == a2) AM_preg = AM_eqCode; */
/*     //else EM_THROW(EM_FAIL); */
/* } */

void add_wrapper() {
    // teyjus terms to c int
    DF_TermPtr t1 = (DF_TermPtr)AM_reg(1);
    int a1 = BIEVAL_evalInt(t1);
    DF_TermPtr t2 = (DF_TermPtr)AM_reg(2);
    int a2 = BIEVAL_evalInt(t2);
    // call function
    int ret = add(a1, a2);
    printf("add result: %d\n", ret);
    if (ret == a2) AM_preg = AM_cpreg;
    else EM_THROW(EM_FAIL);
}

int add(int x, int y) {
    return x + y;
}
