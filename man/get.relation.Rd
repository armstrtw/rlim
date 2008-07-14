\name{RLIM}
\alias{RLIM}
\alias{get.relation}
\alias{get.all.children}
\alias{get.futures.series}
\alias{get.ohlc}
\title{Read data from lim}
\description{
reads any tseries type object form LIM
}
\usage{
get.relation(relname,colnames=NULL,units="days",bars=1)
get.ohlc(relname,colnames=c("open","high","low","close"),units="days",bars=1)
get.futures.series(relname, units="days", bars=1)
get.all.children(relname)
}

\arguments{
  \item{relname}{ contract, symbol, or ticker}
  \item{colnames}{ what cols do you want to read}
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

ty.contracts <- get.all.children("TY")
}
\keyword{ts}

