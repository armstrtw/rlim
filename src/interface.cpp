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

#include <iostream>
#include <iterator>
#include <vector>
#include <string>

#include <R_ext/Rdynload.h>

#include <tslib/tseries.hpp>
#include <Rtype.hpp>
#include <Rutilities.hpp>

#include "lim.hpp"
#include "R.tseries.data.backend.hpp"
#include "get.relation.hpp"

#include "interface.hpp"

using namespace tslib;

using std::string;
using std::vector;

typedef TSeries<double,double,int,R_Backend_TSdata,PosixDate> ts_type;

// so we don't have to pass it around in every function call
static XmimClientHandle handle;

// initialize the connection to the LIM server when loaded:
// (remember to change this function name if the package name changes)
void R_init_RLIM(DllInfo *info) {
  handle = rlim::limConnect();
}

void R_unload_RLIM(DllInfo *info) {
  XmimDisconnect(handle);
}

SEXP getRelation(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP units_sexp, SEXP bars_sexp) {
  ts_type ans;

  string relation_name = Rtype<STRSXP>::scalar(relation_name_sexp);

  vector<string> colnames;
  sexp2string(colnames_sexp,inserter(colnames,colnames.begin()));

  XmimUnits xmim_units = getUnits(Rtype<STRSXP>::scalar(units_sexp));
  int bars = Rtype<INTSXP>::scalar(bars_sexp);

  if(colnames.size()) {
    ans = rlim::getRelation<double,double,int,R_Backend_TSdata,PosixDate>(handle,relation_name.c_str(),colnames,xmim_units,bars);
  } else {
    ans = rlim::getRelationAllCols<double,double,int,R_Backend_TSdata,PosixDate>(handle,relation_name.c_str(),xmim_units,bars);
  }

  return ans.getIMPL()->R_object;
}

SEXP getFuturesSeries(SEXP relation_name_sexp, SEXP units_sexp, SEXP bars_sexp) {
  SEXP ans, r_class;
  string relation_name = Rtype<STRSXP>::scalar(relation_name_sexp);
  XmimUnits xmim_units = getUnits(Rtype<STRSXP>::scalar(units_sexp));
  int bars = Rtype<INTSXP>::scalar(bars_sexp);

  std::set<std::string> contractNames;
  rlim::getContracts(handle, contractNames, relation_name.c_str(), xmim_units, bars);

  std::vector<std::string> colnames;
  colnames.push_back("open");
  colnames.push_back("high");
  colnames.push_back("low");
  colnames.push_back("close");
  colnames.push_back("volume");
  colnames.push_back("OpenInterest");

  PROTECT(ans = allocVector(VECSXP,contractNames.size()));
  int i = 0;
  for(std::set<std::string>::iterator iter = contractNames.begin(); iter != contractNames.end(); iter++, i++) {
    ts_type this_contract = rlim::getRelation<double,double,int,R_Backend_TSdata,PosixDate>(handle,const_cast<char*>(iter->c_str()),colnames,xmim_units,bars);
    SET_VECTOR_ELT(ans,i,this_contract.getIMPL()->R_object);
  }

  // set names of ans
  setAttrib(ans, R_NamesSymbol, string2sexp(contractNames.begin(),contractNames.end()));

  // set class of answer
  PROTECT(r_class = allocVector(STRSXP, 1));
  SET_STRING_ELT(r_class, 0, mkChar("com"));
  classgets(ans, r_class);

  UNPROTECT(2); // ans, r_class
  return ans;
}

SEXP getAllChildren(SEXP relname_sexp) {
  SEXP ans_sexp;
  std::set<std::string> ans;

  std::string relname = Rtype<STRSXP>::scalar(relname_sexp);

  rlim::getAllChildren(handle, ans, relname.c_str());
  return string2sexp(ans.begin(),ans.end());
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
