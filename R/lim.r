get.relation <- function(relname,colnames=NULL,units="days",bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.ohlc <- function(relname,colnames=c("open","high","low","close"),units="days",bars=1) {
  .Call("getRelation", relname, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.futures.series <- function(relname, units="days", bars=1) {
    .Call("getFuturesSeries",relname, units, as.integer(bars))
}

get.all.children <- function(relname) {
    .Call("getAllChildren", relname)
}
