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

#include <Rtype.hpp>

#include "lim.hpp"
#include "R.tseries.data.backend.hpp"

using std::string;
using std::cout;
using std::endl;

// so we don't have to pass it around in every function call
static XmimClientHandle handle;

// initialize the connection to the LIM server when loaded:
// (remember to change this function name if the package name changes)
void R_init_RLIM(DllInfo *info) {
  handle = limConnect();
}

void R_unload_RLIM(DllInfo *info) {
  XmimDisconnect(handle);
}

SEXP getRelation(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP units_sexp, SEXP bars_sexp) {
  string relation_name = Rtype<STRSXP>::scalar(relation_name_sexp);

  vector<string> colnames;
  sexp2string(colnames_sexp,colnames);

  XmimDate from_date;
  from_date.year  = 1900;
  from_date.month = 1;
  from_date.day   = 1;
    
  XmimUnits xmim_units = getUnits(Rtype<STRSXP>::scalar(units_sexp));
  int bars = Rtype<INTSXP>::scalar(bars_sexp);

  rlim::getRelation<double,double,int,R_Backend_TSdata,PosixDate>(handle,
                                                                  relation_name,
                                                                  
                                                                  xmim_units,
                                                                  bars)
}


XmimUnits getUnits(string units) {
  if(units=="day" || units=="days") {
    return XMIM_DAYS;
  } else if(units=="minute" || units=="minutes") {
    return XMIM_MINUTES;
  } else {
    Rprintf("WARNING: invalid units: %s\n",units.c_str());
    return XMIM_UNITS_INVALID;
  }
}


// convert a vector of R strings to a
// c++ vector of strings
void sexp2string(const SEXP str_sexp,std::vector<std::string>& ans) {

  if(str_sexp==R_NilValue) {
    return;
  }

  for(int i=0; i < length(str_sexp); i++) {
    ans.push_back(const_cast<char *>(CHAR(STRING_ELT(str_sexp,i))));
  }
}