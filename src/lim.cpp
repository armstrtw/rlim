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

#include <string>
#include <iostream>
#include <ctime>
#include <set>
#include <map>

#include <xmim_api.h>
#include "lim.hpp"

using std::set;
using std::map;
using std::string;
using std::cout;
using std::endl;

namespace rlim {

  const std::string safe_getenv(const char* env_var) {
    std::string ans;

    const char* var = getenv(env_var);

    // only set ans if var is not NULL
    if(var) {
      ans.assign(var);
    }

    return ans;
  }


  const XmimClientHandle limConnect() {
    XmimClientHandle handle;

    const string lim_server = safe_getenv("LIM_SERVER");
    const int lim_port = atoi(safe_getenv("LIM_PORT").c_str());

    cout << "connecting to LIM on server: " << lim_server <<  " on port " << lim_port << endl;

    // connect to LIM
    if (lim_server.size() && XmimConnect (const_cast<char*>(lim_server.c_str()), lim_port, &handle) != XMIM_SUCCESS) {
      cout << "problem connecting to LIM." << endl;
      cout << "make sure LIM_SERVER and LIM_PORT are set in your environment." << endl;
      handle = static_cast<XmimClientHandle>(0);
    }        
    return handle;
  }

  const XmimRelType getRelationType(const XmimClientHandle& handle, const char* relname) {
    XmimRelType reltype;

    if(XmimGetRelType (handle, const_cast<char*>(relname), &reltype) != XMIM_SUCCESS) {
      XmimPrintError("getRelationType");
      return XMIM_REL_INVALID;
    } else {
      return reltype;
    }
  }

  /* Returns a list of the names of all the contracts available from the LIM */
  void getContracts(const XmimClientHandle& handle, std::set<string>& ans, const char* relname, const XmimUnits units, const int bars) {
    XmimRelType reltype;
    set<string> tickers;

    getAllChildren(handle, tickers, relname);

    for(set<string>::iterator it = tickers.begin(); it != tickers.end(); it++ ) {
      reltype = getRelationType(handle, it->c_str());
      if(reltype != XMIM_REL_FUTURES_CONTRACT || getRelationNROWS(handle, it->c_str(), units, bars) < 1) {
        tickers.erase(it);
      }
    }

    ans.clear();

    // insert elements into answer
    std::copy(tickers.begin(),tickers.end(),
              inserter(ans,ans.begin()));
  }

  /* Returns a list of the names of all the contracts available from the LIM */
  void getAllChildren(const XmimClientHandle& handle, std::set<string>& ans, const char* relname) {

    int num_relnames;
    XmimString* relnames;

    if (XmimGetRelChildren (handle, const_cast<char*>(relname), &num_relnames, &relnames) != XMIM_SUCCESS) {
      return;
    }

    for(int i = 0; i < num_relnames; i++) {
      ans.insert(string(relnames[i]));
    }
  }

  /* look up the month as an int
     that corresponds to the contract letter

     For example: TY_2004M
     we would strip off the last character: M
     and look it up in the table below to find the contract month
     which is 6 in this case */
  const int getContractMonth(const char letter) {
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
  const char getContractLetter(const int m) {
    // contract months
    static char contracts[] = {'F','G','H','J','K','M','N','Q','U','V','X','Z'};

    /* check input */
    /* if number is outside of months range */
    if( m < 1 || m > 12) {
      return '\0';
    }
  
    return contracts[m-1];
  }

  void getRollDates(const XmimClientHandle& handle, map<string,XmimDate>& ans, const char* relname, const char* rollDay) {

    int numContracts, numPeriods;
    XmimDate *rollDates, *contracts;
    std::string contractName;

    if(XmimVaGetRolloverDates (XMIM_CLIENT_HANDLE, handle,
                               XMIM_RELATION, const_cast<char*>(relname),
                               XMIM_COLUMN_LIST, NULL,
                               XMIM_UNITS, 1, XMIM_DAYS,
                               XMIM_ROLLOVER_DAY, const_cast<char*>(rollDay),
                               XMIM_NUM_CONTRACTS, &numContracts,
                               XMIM_NUM_PERIODS, &numPeriods,
                               XMIM_ROLL_DATES, &rollDates,
                               XMIM_CONTRACTS, &contracts,
                               XMIM_END_ARGS)!=XMIM_SUCCESS) {

      XmimPrintError("XmimVaGetRecordsRollover");
      return;
    }

    for(int i = 0; i < numPeriods; i++) {
      // FIXME: why is it i+1 in contracts[]
      makeContractName(contracts[i].year,contracts[i].month,contractName);
      ans[contractName] = rollDates[i+1];
    }
  }

  /* create a yyyymm string from integers year and month */
  void makeContractName(const int year, const int month, std::string& ans) {

    char year_buff[5];
    snprintf(year_buff,5,"%d",year);
    string year_str(year_buff);

    string month_str(1,getContractLetter(month));

    ans.assign(year_str + month_str);
  }


  // get the number of rows for a given individual futures contract
  // some contracts have no rows (stupid LIM), so we don't want to include those in our allocation
  // of storage space
  const int getRelationNROWS(const XmimClientHandle& handle, const char* relname, const XmimUnits xunits, const int bars) {

    XmimDateTime *dates;
    XmimDate from_date;
    int num_records;
    float values;

    from_date.month = 1;
    from_date.day = 1;
    from_date.year = 1900;

    if( XmimVaGetRecords(XMIM_CLIENT_HANDLE, handle,
                         XMIM_RELATION, const_cast<char*>(relname),
                         XMIM_FROM_DATE, from_date,
                         XMIM_NUM_RECORDS, &num_records,
                         XMIM_VALUES, &values,
                         XMIM_DATE_TIMES, &dates,
                         XMIM_UNITS, bars, xunits,
                         XMIM_FILL_OPTION,  XMIM_FILL_NAN, XMIM_FILL_NAN, XMIM_SKIP_ALL_NAN,
                         XMIM_END_ARGS) != XMIM_SUCCESS) {
      return -1;
    }

    return num_records;
  }

  const XmimDate get_expiration_date(const XmimClientHandle& handle, const char* contract) {
    XmimDate ex_date;

    if(XmimVaGetRelation (XMIM_CLIENT_HANDLE, handle,
                          XMIM_RELATION,        contract,
                          XMIM_EXPIRATION_DATE, &ex_date,
                          XMIM_END_ARGS) != XMIM_SUCCESS) {
      XmimPrintError("XmimVaGetRelation");

      // set these to something nonsensical
      ex_date.year  = 0;
      ex_date.month = 0;
      ex_date.day   = 0;
    }

    return ex_date;
  }

  // get the number of rows for a given individual futures contract
  // some contracts have no rows (stupid LIM), so we don't want to include those in our allocation
  // of storage space
  const XmimDate getFirstNoticeDate(const XmimClientHandle& handle, const char* contract) {
    XmimDate first_notice_date;

    if (XmimVaGetRecords(XMIM_CLIENT_HANDLE, handle,
                         XMIM_RELATION, contract,
                         XMIM_FIRST_NOTICE_DATE, &first_notice_date,
                         XMIM_END_ARGS) != XMIM_SUCCESS) {
      XmimPrintError("XmimVaGetRecords");
    }
    return first_notice_date;
  }

  const bool isFuturesContract(const XmimClientHandle& handle, const char* relname) {
    XmimRelType reltype;

    if(XmimGetRelType(handle, const_cast<char*>(relname), &reltype)!=XMIM_SUCCESS) {
      XmimPrintError("XmimGetRelType");
      return false;
    }

    return (reltype==XMIM_REL_FUTURES_CONTRACT) ? true : false;
  }

} // namespace rlim
