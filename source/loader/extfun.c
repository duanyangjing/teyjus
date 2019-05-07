//////////////////////////////////////////////////////////////////////////////
//Copyright 2008
//  Andrew Gacek, Steven Holte, Gopalan Nadathur, Xiaochu Qi, Zach Snow
//////////////////////////////////////////////////////////////////////////////
// This file is part of Teyjus.                                             //
//                                                                          //
// Teyjus is free software: you can redistribute it and/or modify           //
// it under the terms of the GNU General Public License as published by     //
// the Free Software Foundation, either version 3 of the License, or        //
// (at your option) any later version.                                      //
//                                                                          //
// Teyjus is distributed in the hope that it will be useful,                //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU General Public License for more details.                             //
//                                                                          //
// You should have received a copy of the GNU General Public License        //
// along with Teyjus.  If not, see <http://www.gnu.org/licenses/>.          //
//////////////////////////////////////////////////////////////////////////////
#include "extfun.h"
#include "strings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ld_message.h"
#include "loader.h"
#include "../system/memory.h"
#include "../system/error.h"
#include "../simulator/mctypes.h"


TwoBytes LD_EXTFUN_numExtfuns;
struct extfun {
    char* funname;
    char* libname;
    void* addr;
};

typedef struct extfun* extfunptr;
extfunptr* LD_EXTFUN_Extfuns;


// load the shared library and return addr to function symbol
void* dyload(char* cfun, char* clib)
{
    char libPath[128];
    // assume this is a dedicated library path
    strcpy(libPath,
	   "/home/grad06/jingx061/Projects/teyjus-fork/teyjus/lib/");
    strcat(libPath, clib);
    // shared library extension
    strcat(libPath, ".so");

    // Dynamically open (load) the shared library 
    void *handle = dlopen(libPath, RTLD_LAZY);
    if (!handle) {
	fprintf(stderr, "%s\n", dlerror());
        EM_THROW(EM_FAIL);
    }

    printf("loaded shared library %s\n", libPath);
    dlerror();
    //int (*funStub)(DF_TermPtr*, BI_BuiltinTabIndex);
    // Get a function pointer to wrapper function given by funName
    void* funStub = dlsym(handle, cfun);
    char *error = dlerror();
    if (error) {
        fprintf(stderr, "%s\n", error);
        EM_THROW(EM_FAIL);
    }
    // Need to set up args array to pass to the wrapper
    /* DF_TermPtr lOp = (DF_TermPtr)AM_reg(1); */
    /* DF_TermPtr rOp = (DF_TermPtr)AM_reg(2); */
    /* DF_TermPtr args[2] = {lOp, rOp}; */
    /* int success = (*funStub)(args, BI_number); */
    return funStub;
}

extfunptr LD_EXTFUN_LoadExtfun(MEM_GmtEnt* ent) {
    extfunptr loc = malloc(sizeof(struct extfun));
    
    int fnamelen = LD_FILE_GET4();
    char* funname = malloc((fnamelen + 1) * sizeof(char));
    LD_FILE_GetString(funname, fnamelen);
    int lnamelen = LD_FILE_GET4();
    char* libname = malloc((lnamelen + 1) * sizeof(char));
    LD_FILE_GetString(libname, lnamelen);

    loc->funname = funname;
    loc->libname = libname;
    loc->addr = NULL;
    return loc;
}

void LD_EXTFUN_LoadExtfuns(MEM_GmtEnt* gmtEnt) {
    int i;
    TwoBytes count = LD_EXTFUN_numExtfuns = LD_FILE_GET2();
    LD_detail("Loading %d extfuns\n", count);
    LD_EXTFUN_Extfuns = (extfunptr*)malloc(count*sizeof(void*));
  
    for(i = 0; i < count; i++) {
        LD_EXTFUN_Extfuns[i] = LD_EXTFUN_LoadExtfun(gmtEnt);
    }
    return;
}


int LD_EXTFUN_isLoaded(int i) {
    return LD_EXTFUN_Extfuns[i]->addr != NULL;
}

WordPtr LD_EXTFUN_GetFunAddr() {
    int i = (int) LD_FILE_GET2();
    if (i < 0 || i > LD_EXTFUN_numExtfuns)
	EM_THROW(LD_LoadError);
    
    if (LD_EXTFUN_isLoaded(i)) return LD_EXTFUN_Extfuns[i]->addr;
    char* funname = LD_EXTFUN_Extfuns[i]->funname;
    char* libname = LD_EXTFUN_Extfuns[i]->libname;
    void* addr = dyload(funname, libname);
    LD_EXTFUN_Extfuns[i]->addr = addr;
    return (WordPtr)addr;
}

void LD_EXTFUN_Cleanup()
{
    int i;
    for (i = 0; i < LD_EXTFUN_numExtfuns; i++) {
	extfunptr ef = LD_EXTFUN_Extfuns[i];
	free(ef);
    }
}
