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

/****************************************************************************
 *                                                                          *
 * builtins/io.c implements the i/o builtin routines.                       *
 *                                                                          *
 ****************************************************************************/
#include <stdlib.h>
#include "../source/simulator/cinterface.h"

/* type getenv string -> string -> o.
        getenv Name Value.
   Calls the Unix getenv function to unify Value with the value of
   the environment variable given in Name. Name must be instantiated.
 */
void getEnv()
{
    //NOTE: os dependent; need to add code for other os besides UNIX.
  char* str = TJ_getStr(1);
  char* envstr = getenv(str);
  TJ_returnStr(2, envstr);
  
    /* char     *str, *envstr; */
    /* int      length, size; */
    /* MemPtr   strDataHead = AM_hreg; */
    /* MemPtr   strData     = strDataHead + DF_STRDATA_HEAD_SIZE; */
    /* MemPtr   nhreg; */

    /* str = getStringFromTerm((DF_TermPtr)AM_reg(1)); */
    /* if (!str) EM_error(BI_ERROR_UNBOUND_VARIABLE, "string"); */

    /* envstr = getenv(str); */
    /* if (envstr == NULL) EM_error(BI_ERROR_UNSET_ENV_VAR, str); */

    /* length = strlen(envstr); */
    /* size   = MCSTR_numWords(length); */
    /* nhreg  = strData + size; */
  
    /* AM_heapError(nhreg); */
    /* DF_mkStrDataHead(strDataHead); */
    /* MCSTR_toString((MCSTR_Str)strData, envstr, length); */
    /* AM_hreg = nhreg;    */
 
    /* DF_mkStr((MemPtr)AM_reg(1), (DF_StrDataPtr)strDataHead); */
    /* AM_preg = AM_eqCode; */
    /* return; */
}


/* type  system  string -> int -> o
   system Command ReturnCode.
*/
void syscmd()
{
  char* str = TJ_getStr(1);
  int ret = system(str);
  TJ_returnInt(2, ret);

  
  /* char * command = NULL; */
  /* int result = -1; */

  /* //Grab the command; it must be bound. */
  /* command = getStringFromTerm((DF_TermPtr)AM_reg(1)); */
  /* if (!command) EM_error(BI_ERROR_UNBOUND_VARIABLE, "string"); */

  /* //Execute */
  /* result = system(command); */

  /* //Store result. */
  /* bindVarToInt((DF_TermPtr)AM_reg(2), result); */

  /* AM_preg = AM_cpreg; */
  /* return; */
}
