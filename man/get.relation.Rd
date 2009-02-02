\name{RLIM}
\alias{RLIM}
\alias{get.relation}
\alias{get.perpetual.series}
\alias{get.futures.series}
\alias{get.coms}
\alias{get.ohlc}
\title{Read data from lim}
\description{
reads any tseries type object form LIM
}
\usage{
get.relation(relname,colnames=NULL,units="days",bars=1)
get.perpetual.series(relname,colnames=c("open","high","low","close","volume","OpenInterest"),
rollDay="open_interest crossover",rollPolicy="Actual Prices",units="days",bars=1)
get.coms(relname, colnames=c("open","high","low","close","volume","OpenInterest"), rollDay="open_interest crossover", units="days", bars=1)
get.ohlc(relname,colnames=c("open","high","low","close"),units="days",bars=1)
get.futures.series(relname, units="days", bars=1, rollPolicy="open_interest crossover")
}

\arguments{
  \item{relname}{ contract, symbol, or ticker}
  \item{colnames}{ what cols do you want to read}
  \item{rollDay}{string describing when to roll the contract}
  \item{rollPolicy}{string describing how to adjust the prices when a
  roll occurs}
  \item{units}{ minutes or days}
  \item{bars}{ how many minutes or days}
}
\value{
  an fts object
}
\author{ Whit Armstrong }
\examples{

## load all columns
ibm.all <- get.relation("IBM")

## load only the open/high/low/close columns
ibm.ohlc <- get.ohlc("IBM")

ty <- get.futures.series("TY")

ty.p <- get.perpetual.series("TY")

ty.p <- get.perpetual.series("TY",rollDay="open_interest crossover")
ty.p1 <- get.perpetual.series("TY",rollDay="1 day after open_interest crossover")

ty.adj <- get.perpetual.series("TY",rollDay="open_interest crossover",rollPolicy="backward adjusted prices")

}
\keyword{ts}

