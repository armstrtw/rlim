#include <iostream>
#include <vector>
#include <string>

#include <tslib/tseries.hpp>

#include "lim.cpp"
#include "get.relation.hpp"

using std::vector;
using std::string;
using std::cout;
using std::endl;

using namespace tslib;
using namespace rlim;

typedef TSeries<double,double,int,TSdataSingleThreaded,PosixDate> simple_ts_type;

int main() {
  const XmimClientHandle myFuukinLim = limConnect();
  const string ticker("IBM");
  vector<string> colnames;

  colnames.push_back("open");
  colnames.push_back("high");
  colnames.push_back("low");
  colnames.push_back("close");

  XmimUnits xunits = XMIM_DAYS;

  XmimDate from_date;
  from_date.year = 1900;
  from_date.month = 01;
  from_date.day = 01;

  simple_ts_type ans = getRelation<double,double,int,TSdataSingleThreaded,PosixDate>(myFuukinLim,
                                                                                     ticker,
                                                                                     colnames,
                                                                                     from_date,
                                                                                     xunits,
                                                                                     1);

  cout << ans << endl;

  return 0;
}
