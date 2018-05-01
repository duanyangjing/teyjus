// gcc -shared -fpic -o lib.so lib.c

#include <stdio.h>

int fact_helper(int i) {
    if (i == 0) return 1;
    return i * fact_helper(i-1);
}

int fact(int i) {
	printf("fact %d is %d\n", i, fact_helper(i));

	return 0;
}
