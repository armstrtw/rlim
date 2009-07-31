get.relation <- function(relname, colnames=NULL, units="days", bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.perpetual.series <- function(relname, colnames=c("open","high","low","close","volume","OpenInterest"),
                                 rollDay="open_interest crossover",
                                 rollPolicy="backward adjusted prices",
                                 units="days",
                                 bars=1) {

    ans <- .Call("getPerpetualSeries", relname, colnames, rollDay, rollPolicy, units, as.integer(bars))
    class(ans) <- c("coms",class(ans))
    ans
}

get.ohlc <- function(relname, colnames=c("open","high","low","close"), units="days", bars=1) {
    .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.futures.series <- function(relname, units="days", bars=1, rollPolicy="open_interest crossover") {
    .Call("getFuturesSeries", relname, units, as.integer(bars), rollPolicy)
}

get.contract.names <- function(relname, units="days") {
    .Call("getContractNames", relname, units)
}
