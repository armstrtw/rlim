## get relation type
get.rel.type <- function(ticker) {
  .Call("getRelationType",as.character(ticker), PACKAGE="RLIM")
}

## perpetual series, defaults to actual prices
p.series <- function(contract,colnames=c("open","high","low","close","volume","openinterest"),rollDay="5 Days before Expiration Day",rollPolicy="Actual Prices",units="days",bars=1) {
  .Call("perpetualSeries", contract, colnames, rollDay, rollPolicy, as.character(units),as.integer(bars),PACKAGE="RLIM")
}



## ds is just the actual prices, so no change here
day.series <- p.series

## adjusted series is the backwards adjusted series
adj.series <- function(contract, rollDay="5 Days before Expiration Day") {
  p.series(contract,rollDay=rollDay,rollPolicy="Backward Adjusted Prices")
}

get.futures.contract <- function(	contract,
                                 colnames=c("open","high","low","close","volume","openinterest"),
                                 from.date=NULL,
                                 units="days",
                                 bars=1) {
  .Call("getLimRelation",contract,colnames,from.date, units, as.integer(bars),PACKAGE="RLIM")
}

## setting colnames to NULL will allow it to load all cols
get.relation <- function(contract,
                         colnames=NULL,
                         units="days",
                         bars=1)
{
  .Call("getRelation", contract, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.all.contract.names <- function(ticker,units="days",bars=1) {
  .Call("getContracts",ticker,as.character(units),as.integer(bars),PACKAGE="RLIM")
}

get.all.children <- function(ticker) {
  .Call("getAllChildren",ticker,PACKAGE="RLIM")
}

## load com as a list of contracts (type 1)
load.coml <- function(contract,
                      colnames=c("open","high","low","close","volume","openinterest"),
                      rollDay="5 Days before Expiration Day",
                      from.date=NULL,
                      units="days",
                      bars=1) {

  .Call("fullFutHistory",contract,colnames,rollDay,from.date,units,as.integer(bars),PACKAGE="RLIM")
}

## load com as a list of two series adj series and day series (type 2)
load.coms <- function(contract,rollDay="5 Days before Expiration Day") {

  ans <- list()
  ans.as <- adj.series(contract,rollDay=rollDay)

  if(!is.null(ans.as)) {
    ans[["as"]] <- ans.as
    ans[["ds"]] <- day.series(contract,rollDay=rollDay)
    class(ans) <- "coms"
    ans
  } else {
    stop("load.coms: bad name passed to LIM.")
  }

}

## tries to determine type and call the right
## lim function
lim <- function(ticker) {
  ticker.type <- get.rel.type(ticker)
  if("FUTURES"%in%ticker.type) {
    load.coms(ticker)
  } else if("NORMAL"%in%ticker.type) {
    ## hack because LIM loads all the fundamental categories as well
    get.relation(ticker,colnames=c("open","high","low","close"))
  }
}

get.roll.dates <- function(genericContract,rollDay="5 Days before Expiration Day") {
  .Call("getRollDates",genericContract,rollDay,PACKAGE="RLIM")
}

do.roll <- function(contract.list) {

  dates.list <- lapply(contract.list,dates)
  contract.dates <- sort(unique(unlist(dates.list)))
  class(contract.dates) <- c("POSIXt","POSIXct")

  ## FIXME: do all contracts have the same ncol
  ## probably, but we might need a check

  ## just take the ncol of the 1st contract
  new.ncol <- ncol(contract.list[[1]])

  ans <- fts(data=matrix(NA,ncol=new.ncol,nrow=length(contract.dates)),
             dates=contract.dates) 
  colnames(ans) <- colnames(contract.list[[1]])

  ## find the active contract
  ## when this condition is true, we are one futures contract
  ## earlier than the active contract (the one that just got rolled)
  roll.dates <- attr(contract.list,"rollDates")
  ## match roll dates to the contracts we have

  roll.dates <- good.roll.dates(contract.list)
  roll.index <- length(roll.dates)
  last.data.date <- contract.dates[length(contract.dates)]
  while(roll.dates[roll.index] > last.data.date)
    roll.index <- roll.index - 1

  ## bring us to the current contract
  roll.index <- roll.index + 1

  while(roll.index > 1) {
    mask <- dates(contract.list[[roll.index]])>roll.dates[roll.index-1]
    replace.dates <- dates(contract.list[[roll.index]])[mask]
    ans[replace.dates,] <- contract.list[[roll.index]][mask,]
    roll.index <- roll.index - 1
  }

  ## now do the first contract
  mask <- dates(contract.list[[1]])<=roll.dates[1]
  replace.dates <- dates(contract.list[[1]])[mask]
  ans[replace.dates,] <- contract.list[[1]][mask,]

  ans

}

calc.price.adjust <- function(contract.list) {

  roll.dates <- good.roll.dates(contract.list)
  if(length(roll.dates)!=length(contract.list)) stop("rollDate mismatch")

  active.index <- find.active.contract(contract.list)

  ans <- matrix(NA,nrow=active.index-1)

  ## calculate basis shift for each roll date
  for(i in (active.index-1):1)
    ans[i,] <- as.numeric(contract.list[[i+1]][roll.dates[i],"close"]) - as.numeric(contract.list[[i]][roll.dates[i],"close"])


}

calc.price.adjust <- function(contract.list) {

  roll.dates <- good.roll.dates(contract.list)
  if(length(roll.dates)!=length(contract.list)) stop("rollDate mismatch")

  active.index <- find.active.contract(contract.list)

  ans <- matrix(NA,nrow=active.index-1)

  ## calculate basis shift for each roll date
  for(i in 1:(active.index-1))
    ans[i,] <- as.numeric(contract.list[[i+1]][roll.dates[i],"close"]) - as.numeric(contract.list[[i]][roll.dates[i],"close"])

  ## make it an fts w/ the roll dates
  ## except the last roll date
  attr(ans,"dates") <- roll.dates[-length(roll.dates)]
  class(ans) <- c("fts","matrix")
  ans
}


## weed out the bad ones (that don't match our contracts)
good.roll.dates <- function(contract.list) {
  contract.names <- names(contract.list)
  attr(contract.list,"rollDates")[substring(contract.names,4,nchar(contract.names))]
}

## roll dates that have been hit
rolled.rolled.dates <- function(x) {

  rd <- attr(x,"rollDates")
  max.index <- min(c(length(x),length(rd)))
  ans <- vector("logical",length(rd))
  ans[] <- FALSE

  for(i in 1:max.index) {
    if(rd[i]%in%dates(x[[i]])) ans[i] <- TRUE
  }
  rd[ans]
}

basis <- function(x) {
  x.rd <- rolled.rolled.dates(x)
  ans <- vector("double",length(x.rd))
  for(i in 1:length(x.rd)) {
    ans[i] <- as.numeric(x[[i]][x.rd[i],"close"]) - as.numeric(x[[i+1]][x.rd[i],"close"])
  }
  fts(data=x,
      dates=x.rd)
}

## TESTING: cbind(good.roll.dates(x),lapply(lapply(x,dates),min),lapply(lapply(x,dates),max))


find.active.contract <- function(contract.list) {

  dates.list <- lapply(contract.list,dates)
  contract.dates <- sort(unique(unlist(dates.list)))
  class(contract.dates) <- c("POSIXt","POSIXct")

  roll.dates <- good.roll.dates(contract.list)
  roll.index <- length(roll.dates)
  last.data.date <- contract.dates[length(contract.dates)]
  while(roll.dates[roll.index] > last.data.date)
    roll.index <- roll.index - 1

  roll.index+1
}

dates.com <- function(x) {
  lapply(x,dates)
}


get.expiration.dates <- function(genericContract,units="days",bars=1) {
  sort(.Call("get_expiration_dates",genericContract,as.character(units),as.integer(bars),PACKAGE="RLIM"))
}

get.option.expiration.dates <- function(genericContract) {
  ans <- .Call("get_option_expiration_dates",genericContract,PACKAGE="RLIM")

  ## take out the perpetual contracts
  ## FIXME: add this to api
  ans[nchar(names(ans)) > 8]
}

