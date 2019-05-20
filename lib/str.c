#include <string.h>
#include "../source/simulator/cinterface.h"
#include <stdlib.h>

char* strrev(const char* s)
{
  int len = strlen(s);
  char* rev = malloc((len+1) * sizeof(char));
  int j = 0;
  for (int i = len-1; i >= 0; i--) {
    rev[j] = s[i];
    j++;
  }
  rev[len] = '\0';
  return rev;
}

void strrev_wrapper()
{
  const char* s = TJ_getStr(1);
  char* rev = strrev(s);
  TJ_returnStr(2, rev);
}
