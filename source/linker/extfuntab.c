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
#include "stringspace.h"
#include "module.h"
#include "vector.h"
#include "file.h"
#include "VectorRW.h"
#include "linker_message.h"
#include "../include/standardlib.h"

typedef struct {
  char* funname;
  char* libname;
} extfun_t;

struct Vector extfuns;

void LK_EXTFUNS_Init()
{
  LK_VECTOR_Init(&extfuns, 10, sizeof(extfun_t));
}

void LoadExtfun(int fd, struct Module_st* CMData, void* entry)
{
  extfun_t* ef = (extfun_t*)entry;
  char* funname = LK_FILE_GetString(fd);
  char* libname = LK_FILE_GetString(fd);
  ef->funname = funname;
  ef->libname = libname;
}

void LK_EXTFUNS_Load(int fd, struct Module_st* CMData)
{
  LK_VECTOR_Read(fd, &extfuns, CMData, &(CMData->ExtfunsAdj), LoadExtfun);
}

void WriteExtfun(int fd, void* entry)
{
  extfun_t* ef = (extfun_t*)entry;
  LK_FILE_PutString(fd, ef->funname);
  LK_FILE_PutString(fd, ef->libname);
  free(ef->funname);
  free(ef->libname);
}

void LK_EXTFUNS_Write(int fd)
{
  debug("Writing String Tables at %lx\n",lseek(fd,0,SEEK_CUR));
  LK_VECTOR_Write(fd, &extfuns, WriteExtfun);
  LK_VECTOR_Free(&extfuns);
}
