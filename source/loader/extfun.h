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
#ifndef _EXTFUN_H_
#define _EXTFUN_H_

#include "../system/memory.h"
#include "../simulator/mctypes.h"
#include <dlfcn.h>

// Load the external function table
void LD_EXTFUN_LoadExtfuns(MEM_GmtEnt* gmtEnt);
// Read an index to the extfun table, dylink if needed, return the address of
// the function
WordPtr LD_EXTFUN_GetFunAddr();
// Clean up the external function table (it's not needed after loading)
void LD_EXTFUN_Cleanup();

#endif //_CODE_H_
