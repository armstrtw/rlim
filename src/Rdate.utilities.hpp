///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008  Whit Armstrong                                    //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
///////////////////////////////////////////////////////////////////////////

#ifndef RDATE_UTILITIES_HPP
#define RDATE_UTILITIES_HPP

#include <Rinternals.h>

void addPOSIXattributes(SEXP x);
void addFtsClass(SEXP x);
void addDates(SEXP r_object,SEXP r_dates);
void setDates(SEXP x, SEXP dates);
SEXP getDatesSEXP(const SEXP x);


#endif // RDATE_UTILITIES_HPP
