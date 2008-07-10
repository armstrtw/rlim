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

#include "Rdate.utilities.hpp"

void addPOSIXattributes(SEXP x) {
  SEXP r_dates_class;

  PROTECT(r_dates_class = allocVector(STRSXP, 2));
  SET_STRING_ELT(r_dates_class, 0, mkChar("POSIXt"));
  SET_STRING_ELT(r_dates_class, 1, mkChar("POSIXct"));
  classgets(x, r_dates_class);
  UNPROTECT(1);
}

void addFtsClass(SEXP x) {
  SEXP r_tseries_class;
  PROTECT(r_tseries_class = allocVector(STRSXP, 2));
  SET_STRING_ELT(r_tseries_class, 0, mkChar("fts"));
  SET_STRING_ELT(r_tseries_class, 1, mkChar("matrix"));
  classgets(x, r_tseries_class);
  UNPROTECT(1);
}

void addDates(SEXP r_object,SEXP r_dates) {
  if(r_dates==R_NilValue) {
    return;
  }
  setAttrib(r_object,install("dates"),r_dates);
}

void setDates(SEXP x, SEXP dates) {
  setAttrib(x,install("dates"),dates);
}

SEXP getDatesSEXP(const SEXP x) {
  return getAttrib(x,install("dates"));
}
