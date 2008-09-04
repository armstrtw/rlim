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


#include <map>
#include <string>
#include <xmim_api.h>
#include <Rinternals.h>

const XmimUnits getUnits(const char* units);
std::map<std::string,XmimUnits> init_units();

// exported functions
extern "C" {
  void R_init_RLIM(DllInfo *info);
  void R_unload_RLIM(DllInfo *info);
  SEXP getRelation(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP units_sexp, SEXP bars_sexp);
  SEXP getPerpetualSeries(SEXP relation_name_sexp, SEXP colnames_sexp, SEXP rollDay_sexp, SEXP rollPolicy_sexp, SEXP units_sexp, SEXP bars_sexp);
  SEXP getFuturesSeries(SEXP relation_name_sexp, SEXP units_sexp, SEXP bars_sexp);
  SEXP getAllChildren(SEXP relname_sexp);
}
