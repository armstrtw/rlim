get.relation <- function(relname, colnames=NULL, units="days", bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.perpetual.series <- function(relname, colnames=c("open","high","low","close","volume","OpenInterest"),
                                 rollDay="open_interest crossover",
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

get.coml <- function(relname,
                     colnames=c("open","high","low","close","volume","OpenInterest"),
                     rollDay="open_interest crossover",
                     units="days",
                     bars=1) {

    ans <- list()

    ans[["ps"]] <- get.perpetual.series(relname,colnames=colnames,rollDay=rollDay,rollPolicy="backward adjusted prices",units=units,bars=bars)
    ans[["ds"]] <- get.perpetual.series(relname,colnames=colnames,rollDay=rollDay,rollPolicy="actual prices",units=units,bars=bars)

    class(ans) <- "coml"
    ans
}
