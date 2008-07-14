get.relation <- function(relname, colnames=NULL, units="days", bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.perpetual.series <- function(relname, colnames=c("open","high","low","close","volume","OpenInterest"),
                                 rollDay="1 day after open_interest crossover",
                                 rollPolicy="Actual Prices",
                                 units="days",
                                 bars=1) {
    .Call("getPerpetualSeries", relname, colnames, rollDay, rollPolicy, units, as.integer(bars))
}

get.ohlc <- function(relname, colnames=c("open","high","low","close"), units="days", bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.futures.series <- function(relname, units="days", bars=1) {
    .Call("getFuturesSeries", relname, units, as.integer(bars))
}

get.all.children <- function(relname) {
    .Call("getAllChildren", relname)
}
