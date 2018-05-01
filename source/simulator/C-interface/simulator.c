/*
 * Compile this file by:
 *   gcc -rdynamic -o tjsim simulator.c -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "../abstmachine.h"
#include "../dataformats.h"

AM_DataType AM_regs_test[10];
AM_DataTypePtr AM_reg_test(int i) {return AM_regs_test + i;}

// pretend that the argument register has been setup
void mimicstate(void) {
    DF_TermPtr t1 = malloc(sizeof(DF_Term));
    DF_Tag *tag = malloc(sizeof(DF_Tag));
    // tag of this term
    tag->categoryTag = DF_TM_TAG_INT;
    t1->tag = *tag;
    t1->dummy = 5;
    AM_regs_test[0] = (AM_DataType)(*t1);
}

void setargs(AM_DataTypePtr **args) {
    AM_DataTypePtr *args1 = *args;
    *args1 = AM_reg_test(0);
}

int main() { 

    /* Suppose compiler generate the following instruction for external function:
         call_extern funName libName
       The following two variables simulate what the simulator may dynamically
       get from the instruction. */
    char funName[20];
    char libName[20];
    int numargs;
    printf("Enter the name of the external function:\n");
    scanf("%s",funName);
    printf("Enter the name of the library the function belongs to:\n");
    scanf("%s",libName);
    //printf("Enter number of args:\n");
    //scanf("%d",numargs);


    /* This should be the dedicated library path in teyjus directory. Here I'm 
       using the relative path "./lib" */
    char libpath[255];
    strcpy(libpath, "./lib/");
    strcat(libpath, libName);

    /* Dynamically open the shared library given by libName */
    void *handle = dlopen(libpath, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    /* All wrapper functions will be void functions with no args,
       so we can directly define the signature of the function pointer */
    void (*fun_wrapper)(AM_DataTypePtr*);
    /* Get a function pointer to wrapper function given by funName */
    fun_wrapper = dlsym(handle, strcat(funName, "_wrapper"));
    char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    printf("Function %s from %s linked with simulator.\n", funName, libName);
    /* Call the wrapper function */
    mimicstate();
    AM_DataTypePtr *args;
    setargs(&args);
    (*fun_wrapper)(args);
    return 0;
}
