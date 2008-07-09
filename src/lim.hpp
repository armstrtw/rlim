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

#ifndef LIM_HPP
#define LIM_HPP

#include <string>
#include <map>
#include <set>

#include <xmim_api.h>

namespace rlim {

  const XmimClientHandle limConnect();
  const XmimRelType getRelationType(const XmimClientHandle& handle, const std::string& relname);
  void getContracts(const XmimClientHandle& handle, std::set<std::string>& ans, const std::string& genericContract, const XmimUnits units, const int bars);
  void getAllChildren(const XmimClientHandle& handle, std::set<std::string>& ans,  const std::string& ticker);
  const int getContractMonth(const char letter);
  const char getContractLetter(const int m);
  void getRollDates(const XmimClientHandle& handle, std::map<std::string,XmimDate>& ans, const std::string& ticker, const std::string& rollDay);
  const std::string makeContractName(const int year, const int month);
  const int getRelationNROWS(const XmimClientHandle& handle, const std::string& ticker, const XmimUnits xunits, const int bars);
  const XmimDate get_expiration_date(const XmimClientHandle& handle, const char* contract);
  const XmimDate getFirstNoticeDate(const XmimClientHandle& handle, const char* contract);
  const bool isFuturesContract(const std::string& relname);

} // namespace rlim

#endif // LIM_HPP
