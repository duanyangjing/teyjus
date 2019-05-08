#include "cinterface.h"
#include "builtins/evalexp.h"
#include "abstmachine.h"
#include "dataformats.h"
#include "mcstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int TJ_getInt(int i)
{
  if (i < 0 || i >= AM_NUM_OF_REG) {
    printf("Argument index must be in range [%d,%d)\n", 0, AM_NUM_OF_REG);
    exit(-1);
  }
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);
  int val = BIEVAL_evalInt(t);

  return val;
}

float TJ_getReal(int i)
{
  if (i < 0 || i >= AM_NUM_OF_REG) {
    printf("Argument index must be in range [%d,%d)\n", 0, AM_NUM_OF_REG);
    exit(-1);
  }
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);
  float val = BIEVAL_evalFloat(t);

  return val;
}

const char* TJ_getStr(int i)
{
  if (i < 0 || i >= AM_NUM_OF_REG) {
    printf("Argument index must be in range [%d,%d)\n", 0, AM_NUM_OF_REG);
    exit(-1);
  }
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);
  DF_StrDataPtr sdata = BIEVAL_evalStr(t);
  char* s = DF_strDataToCString(sdata);
  /* char* val = malloc((strlen(s)+1) * sizeof(char)); */
  /* strcpy(val, s); */

  return s;
}

/*
 * Position i originally holds the unbound variable, which is moved to reg 2.
 * Actual return value has a corresponding term created at reg 1.
 * Then the unification code is called.
 */
void TJ_returnInt(int i, int val)
{
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);
  /* if (!DF_isFV(t)) { */
  /*   printf("Can only put return data into free variables\n"); */
  /* } */

  DF_mkInt((MemPtr)AM_reg(1), val);
  DF_copyAtomic((DF_TermPtr)(AM_reg(i)), (MemPtr)AM_reg(2));
  AM_preg = AM_eqCode;
}

void TJ_returnReal(int i, float val)
{
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);

  DF_mkFloat((MemPtr)AM_reg(1), val);
  DF_copyAtomic((DF_TermPtr)(AM_reg(i)), (MemPtr)AM_reg(2));
  AM_preg = AM_eqCode;
}

void TJ_returnStr(int i, const char* s)
{
  DF_TermPtr t = (DF_TermPtr)AM_reg(i);

  DF_mkStr((MemPtr)AM_reg(1), val);
  DF_copyAtomic((DF_TermPtr)(AM_reg(i)), (MemPtr)AM_reg(2));
  AM_preg = AM_eqcode;
}

void noop(){}
