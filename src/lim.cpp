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

#include <cstdlib>
#include <string>
#include <iostream>
#include <ctime>
#include <set>
#include <map>

#include <xmim_api.h>
#include "lim.hpp"

using std::string;
using std::cerr;
using std::endl;

const XmimClientHandle rlim::limConnect() {
  XmimClientHandle handle = static_cast<XmimClientHandle>(NULL);

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


/* look up the month as an int
   that corresponds to the contract letter

   For example: TY_2004M
   we would strip off the last character: M
   and look it up in the table below to find the contract month
   which is 6 in this case */
const int rlim::getContractMonth(const char letter) {
  int month = -1;

  switch(letter) {
  case 'F' : month = 1; break;
  case 'G' : month = 2; break;
  case 'H' : month = 3; break;
  case 'J' : month = 4; break;
  case 'K' : month = 5; break;
  case 'M' : month = 6; break;
  case 'N' : month = 7; break;
  case 'Q' : month = 8; break;
  case 'U' : month = 9; break;
  case 'V' : month = 10; break;
  case 'X' : month = 11; break;
  case 'Z' : month = 12; break;
  }
  return month;
}

/* convert integer month to futures contract letter */
const char rlim::getContractLetter(const int m) {
  // contract months
  static char contracts[] = {'F','G','H','J','K','M','N','Q','U','V','X','Z'};

  /* check input */
  /* if number is outside of months range */
  if( m < 1 || m > 12) {
    return '\0';
  }
  
  return contracts[m-1];
}

/* create a yyyymm string from integers year and month */
void rlim::makeContractName(const int year, const int month, std::string& ans) {

  char year_buff[5];
  snprintf(year_buff,5,"%d",year);
  string year_str(year_buff);

  string month_str(1,getContractLetter(month));

  ans.assign(year_str + month_str);
}



