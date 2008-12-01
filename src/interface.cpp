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
#include <map>
#include <vector>
#include <string>

#include <Rinternals.h>
#include <R_ext/Rdynload.h>

#include <tslib/tseries.hpp>
#include <Rtype.hpp>
#include <Rutilities.hpp>
#include <R.tseries.data.backend.hpp>
#include <Rvector.hpp>
#include <lim.tslib/lim.tslib.hpp>

#include "interface.hpp"

using namespace tslib;

using std::map;
using std::vector;
using std::string;
using std::cerr;
using std::endl;

typedef TSeries<double,double,R_len_t,R_Backend_TSdata,PosixDate> ts_type;

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

const XmimClientHandle limConnect() {
  XmimClientHandle handle = -1;

  char* limServer = getenv("LIM_SERVER");
  char* limPort_char = getenv("LIM_PORT");

  if(limServer == NULL || limPort_char == NULL) {
    cerr << "please make sure LIM_SERVER and LIM_PORT are defined in your environment." << endl;
  } else {
    int limPort = atoi(limPort_char);
    if(XmimConnect(limServer, limPort, &handle)!= XMIM_SUCCESS) {
      cerr << "failed to connect to lim" << endl;
    }
  }
  return handle;
}

SEXP getRelation(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP units_sexp, SEXP numUnits_sexp) {
  ts_type ans;

  const char* relation_name = CHAR(Rtype<STRSXP>::scalar(relation_name_sexp));

  vector<string> colnames;
  sexp2string(colnames_sexp,inserter(colnames,colnames.begin()));

  XmimUnits xmim_units = getUnits(CHAR(Rtype<STRSXP>::scalar(units_sexp)));
  int numUnits = Rtype<INTSXP>::scalar(numUnits_sexp);

  if(colnames.size()) {
    ans = lim_tslib_interface::getRelation<double,double,R_len_t,R_Backend_TSdata,PosixDate>(handle, relation_name, colnames, xmim_units, numUnits);
  } else {
    ans = lim_tslib_interface::getRelationAllCols<double,double,R_len_t,R_Backend_TSdata,PosixDate>(handle, relation_name, xmim_units, numUnits);
  }

  return ans.getIMPL()->R_object;
}

SEXP getPerpetualSeries(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP rollDay_sexp, SEXP rollPolicy_sexp, SEXP units_sexp, SEXP numUnits_sexp) {
  ts_type ans;

  const char* relation_name = CHAR(Rtype<STRSXP>::scalar(relation_name_sexp));

  vector<string> colnames;
  sexp2string(colnames_sexp,inserter(colnames,colnames.begin()));

  const char* rollDay = CHAR(Rtype<STRSXP>::scalar(rollDay_sexp));
  const char* rollPolicy = CHAR(Rtype<STRSXP>::scalar(rollPolicy_sexp));
  XmimUnits xmim_units = getUnits(CHAR(Rtype<STRSXP>::scalar(units_sexp)));
  int numUnits = Rtype<INTSXP>::scalar(numUnits_sexp);

  if(!colnames.size()) {
    return R_NilValue;
  } else {
    ans = lim_tslib_interface::getPerpetualSeries<double,double,R_len_t,R_Backend_TSdata,PosixDate>(handle, relation_name, colnames, rollDay, rollPolicy, xmim_units, numUnits);
  }

  return ans.getIMPL()->R_object;
}

SEXP getFuturesSeries(SEXP relname_sexp, SEXP units_sexp, SEXP numUnits_sexp) {
  const char* relname = CHAR(Rtype<STRSXP>::scalar(relname_sexp));
  const XmimUnits xmim_units = getUnits(CHAR(Rtype<STRSXP>::scalar(units_sexp)));
  const int numUnits = Rtype<INTSXP>::scalar(numUnits_sexp);
  map<string,ts_type> ans_map;
  vector<XmimDate> ex_dates;

  lim_tslib_interface::getFuturesSeries<double,double,R_len_t,R_Backend_TSdata,PosixDate>(handle,ans_map,relname,xmim_units,numUnits);
  
  // extract individual contract names
  vector<string> contractNames;
  for(map<string,ts_type>::iterator iter = ans_map.begin(); iter != ans_map.end(); iter++) {
    contractNames.push_back(iter->first);
  }

  RAbstraction::RVector<VECSXP> ans(ans_map.size());
  ans.setClass("com");

  lim_tslib_interface::getExpirationDates(handle, back_inserter(ex_dates), contractNames.begin(),contractNames.end());
  RAbstraction::RVector<REALSXP> expirationDates(ex_dates.size());
  int i = 0;
  for(vector<XmimDate>::iterator iter = ex_dates.begin(); iter != ex_dates.end(); iter++, i++) {
    expirationDates[i] = PosixDate<double>::toDate(iter->year,iter->month,iter->day,0,0,0,0);
  }
  // create and add dates class to dates object
  vector<string> dts_class;
  dts_class.push_back("POSIXt");
  dts_class.push_back("POSIXct");
  expirationDates.setClass(dts_class.begin(),dts_class.end());
  
  i = 0;
  for(map<string,ts_type>::iterator iter = ans_map.begin(); iter != ans_map.end(); iter++, i++) {
    SET_VECTOR_ELT(ans.getSEXP(), i, iter->second.getIMPL()->R_object);
    //ans(i) = iter->second.getIMPL()->R_object;
  }

  // set names of ans: FIXME: extract names from map
  ans.setNames(contractNames.begin(),contractNames.end());
  // set expirationDates of ans
  ans.setAttribute("expirationDates",expirationDates.getSEXP());

  return ans.getSEXP();
}

SEXP getAllChildren(SEXP relname_sexp) {
  vector<string> ans;
  const char* relname = CHAR(Rtype<STRSXP>::scalar(relname_sexp));
  lim_tslib_interface::getAllChildren(handle, back_inserter(ans), relname);
  return string2sexp(ans.begin(),ans.end());
}

const XmimUnits getUnits(const char* units) {
  static map<string, XmimUnits> units_map = init_units();

  map<string, XmimUnits>::iterator iter = units_map.find(units);

  if(iter == units_map.end()) {
    cerr << "WARNING: invalid units: " << units << endl;
    return XMIM_UNITS_INVALID;
  }
  return iter->second;
}

map<string, XmimUnits> init_units() {

  map<string, XmimUnits> ans;

  ans["millisecond"] = XMIM_MILLISECONDS;
  ans["milliseconds"] = XMIM_MILLISECONDS;
  ans["second"] = XMIM_SECONDS;
  ans["seconds"] = XMIM_SECONDS;
  ans["minute"] = XMIM_MINUTES;
  ans["minutes"] = XMIM_MINUTES;
  ans["hour"] = XMIM_HOURS;
  ans["hours"] = XMIM_HOURS;
  ans["day"] = XMIM_DAYS;
  ans["days"] = XMIM_DAYS;
  ans["week"] = XMIM_WEEKS;
  ans["weeks"] = XMIM_WEEKS;
  ans["month"] = XMIM_MONTHS;
  ans["months"] = XMIM_MONTHS;
  ans["quarter"] = XMIM_QUARTERS;
  ans["quarters"] = XMIM_QUARTERS;
  ans["year"] = XMIM_YEARS;
  ans["years"] = XMIM_YEARS;

  return ans;
}
