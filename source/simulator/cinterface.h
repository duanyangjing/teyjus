#ifndef CINTERFACE_H
#define CINTERFACE_H

/* Get data from argument register i */
int TJ_getInt(int i);
float TJ_getReal(int i);
const char* TJ_getStr(int i);

/*
 * Unify the return value of a C function with an existing term at 
 * argument i (the one that is to hold the return value).
 */
void TJ_returnInt(int i, int val);
void TJ_returnReal(int i, float val);
void TJ_returnStr(int i, char* s);

void noop();


#endif //CINTERFACE_H
