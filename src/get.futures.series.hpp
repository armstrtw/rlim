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

#ifndef GET_FUTURES_SERIES_HPP
#define GET_FUTURES_SERIES_HPP


#include <map>
#include <vector>
#include <string>
#include <tslib/tseries.hpp>

using namespace tslib;

namespace rlim {

  template<class TDATE,
           class TDATA,
           class TSDIM,
           template<typename,typename,typename> class TSDATABACKEND,
           template<typename> class DatePolicy>
  const map< std::string, TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy> > getFuturesSeries(const XmimClientHandle& handle,
                                                                                                 const char* relname,
                                                                                                 const XmimUnits units,
                                                                                                 const int bars)
  {

    map< std::string, TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy> ans;

    std::set<std::string> contractNames;
    std::vector<std::string> colnames;
    colnames.push_back("open");
    colnames.push_back("high");
    colnames.push_back("low");
    colnames.push_back("close");
    colnames.push_back("volume");
    colnames.push_back("OpenInterest");

    getAllChildren(handle, contractNames, relname);
    
    for(std::set<std::string> iter = contractNames.begin(); iter != contractNames.end(); iter++) {
      ans[ *iter ] = getRelation<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>(handle,
                                                                             const_cast<char*>(iter->c_str()),
                                                                             colnames,
                                                                             units,
                                                                             bars);
    }

    return ans;
  } 
} // namespace rlim 

#endif // GET_FUTURES_SERIES_HPP
