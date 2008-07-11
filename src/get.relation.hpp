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

#ifndef GET_RELATION_HPP
#define GET_RELATION_HPP


#include <vector>
#include <string>
#include <xmim_api.h>
#include <tslib/tseries.hpp>
#include "xmim2fts.hpp"

using namespace tslib;

namespace rlim {

  template<class TDATE,
           class TDATA,
           class TSDIM,
           template<typename,typename,typename> class TSDATABACKEND,
           template<typename> class DatePolicy>
  const TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy> getRelation(const XmimClientHandle& handle,
                                                                        const char* relname,
                                                                        const std::vector<std::string>& colnames,
                                                                        const XmimUnits xunits,
                                                                        const int bars)
  {
    int num_columns = colnames.size();
    int num_records = 0;
    float* values = NULL;
    XmimDateTime* dates = NULL;
    XmimReturnCode retCode;
    XmimDate from_date;

    from_date.year  = 1900;
    from_date.month = 1;
    from_date.day   = 1;

    // return empty fts if no colnames supplied
    if(!num_columns) {
      return TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>();
    }

    XmimString* xmim_columns = new XmimString[num_columns];
    int i = 0;
    for(vector<std::string>::const_iterator it = colnames.begin(); it != colnames.end(); it++, i++) {
      xmim_columns[i] = strdup(it->c_str());
    }

    retCode = XmimVaGetRecords(XMIM_CLIENT_HANDLE, handle,
                               XMIM_RELATION, const_cast<char*>(relname),
                               XMIM_COLUMN_ARRAY, num_columns, xmim_columns,
                               XMIM_FROM_DATE, from_date,
                               XMIM_UNITS, bars, xunits,
                               XMIM_NUM_RECORDS, &num_records,
                               XMIM_DATE_TIMES, &dates,
                               XMIM_VALUES, &values,
                               XMIM_FILL_OPTION, XMIM_FILL_NAN, XMIM_FILL_NAN, XMIM_SKIP_ALL_NAN,
                               XMIM_END_ARGS);

    // free our colnames array
    for(i = 0; i < num_columns; i++)
      free(xmim_columns[i]);
    delete []xmim_columns;

    // return empty fts if we encounter an error
    if(retCode!=XMIM_SUCCESS) {
      XmimPrintError("XmimVaGetRecords");
      return TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>();
    }

    return xmim2fts<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>(num_records, num_columns, values, dates, colnames);
  }

  template<class TDATE,
           class TDATA,
           class TSDIM,
           template<typename,typename,typename> class TSDATABACKEND,
           template<typename> class DatePolicy>
  const TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy> getRelationAllCols(const XmimClientHandle& handle,
                                                                               const char* relname,
                                                                               const XmimUnits xunits,
                                                                               const int bars)
  {
    int num_columns = 0;
    int num_records = 0;

    float* values = NULL;
    XmimDateTime* dates = NULL;
    XmimReturnCode retCode;

    XmimDate from_date;
    from_date.year  = 1900;
    from_date.month = 1;
    from_date.day   = 1;

    XmimString* xmim_columns = NULL;
    std::vector<std::string> colnames;

    retCode = XmimVaGetRecordsAllColumns(XMIM_CLIENT_HANDLE, handle,
                                         XMIM_RELATION, const_cast<char*>(relname),
                                         XMIM_COLUMN_ARRAY, &num_columns, &xmim_columns,
                                         XMIM_FROM_DATE, from_date,
                                         XMIM_UNITS, bars, xunits,
                                         XMIM_NUM_RECORDS, &num_records,
                                         XMIM_DATE_TIMES, &dates,
                                         XMIM_VALUES, &values,
                                         XMIM_FILL_OPTION, XMIM_FILL_NAN, XMIM_FILL_NAN, XMIM_SKIP_ALL_NAN,
                                         XMIM_END_ARGS);

    // return empty fts if we encounter an error
    if(retCode!=XMIM_SUCCESS) {
      XmimPrintError("XmimVaGetRecords");
      return TSeries<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>();
    }

    // copy colnames
    // it's not necessary to free memory that LIM allocated
    // see Ch 13.4 of devel guide (page 198 of the May 30, 2008 version)
    // http://customers.lim.com/pdfdocs/data_dev_guide.pdf
    for(int i = 0; i < num_columns; i++) {
      colnames.push_back(xmim_columns[i]);
    }
    return xmim2fts<TDATE,TDATA,TSDIM,TSDATABACKEND,DatePolicy>(num_records, num_columns, values, dates, colnames);
  }

} // namespace rlim

#endif // GET_RELATION_HPP
