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
#include <string.h>
#include <stdio.h> 
#include "../C-interface/interface.h"

// DJ: Looks like only these two helper functions are needed from io.c

/* get string from an lpwam string term pointer */
static char* BIIO_getStringFromTerm(DF_TermPtr tmPtr)
{
    HN_hnorm(tmPtr);
    tmPtr = DF_termDeref(tmPtr);
    if (DF_isStr(tmPtr)) return MCSTR_toCString(DF_strValue(tmPtr));
    else return NULL;
}

/* Given an lpwam VAR term pointer, and an integer value,
   bind the variable term to the given integer. */
static void BIIO_bindVarToInt(DF_TermPtr varPtr, int integer)
{
    HN_hnorm(varPtr);
    varPtr = DF_termDeref(varPtr);
    if (!DF_isFV(varPtr)) EM_error(BI_ERROR_NON_VAR_TERM, varPtr);

    TR_trailTerm(varPtr);
    DF_mkInt((MemPtr)varPtr, integer);
}

/* type getenv string -> string -> o.
        getenv Name Value.
   Calls the Unix getenv function to unify Value with the value of
   the environment variable given in Name. Name must be instantiated.
 */
void BIIO_getEnv()
{
  //NOTE: os dependent; need to add code for other os besides UNIX.
  char     *str, *envstr;
  int      length, size;
  MemPtr   strDataHead = AM_hreg;
  MemPtr   strData     = strDataHead + DF_STRDATA_HEAD_SIZE;
  MemPtr   nhreg;

  str = BIIO_getStringFromTerm((DF_TermPtr)AM_reg(1));
  if (!str) EM_error(BI_ERROR_UNBOUND_VARIABLE, "string");

  envstr = getenv(str);
  if (envstr == NULL) EM_error(BI_ERROR_UNSET_ENV_VAR, str);

  length = strlen(envstr);
  size   = MCSTR_numWords(length);
  nhreg  = strData + size;
  
  AM_heapError(nhreg);
  DF_mkStrDataHead(strDataHead);
  MCSTR_toString((MCSTR_Str)strData, envstr, length);
  AM_hreg = nhreg;   
 
  DF_mkStr((MemPtr)AM_reg(1), (DF_StrDataPtr)strDataHead);
  AM_preg = AM_eqCode;
  return;
}


/* type  system  string -> int -> o
   system Command ReturnCode.
*/
void BIIO_system()
{
  char * command = NULL;
  int result = -1;

  //Grab the command; it must be bound.
  command = BIIO_getStringFromTerm((DF_TermPtr)AM_reg(1));
  if (!command) EM_error(BI_ERROR_UNBOUND_VARIABLE, "string");

  //Execute
  result = system(command);

  //Store result.
  BIIO_bindVarToInt((DF_TermPtr)AM_reg(2), result);

  AM_preg = AM_cpreg;
  return;
}


/* type  time  int -> int -> o
   time X Y
   Uses the Unix gettimeofday function to get the number of seconds and
   microseconds in X and Y since 00:00 Universal Coordinated Time,
   January 1, 1970. The arguments are expected to be uninstantiated
   variables
*/
void BIIO_unixTime()
{
  //to be filled in
  EM_error(BI_ERROR_NOT_IMPLEMENTED);
}
