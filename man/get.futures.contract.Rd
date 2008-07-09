\name{get.futures.contract}
\alias{get.futures.contract}
\title{read futures contract}
\description{
reads a perpetual futures series from LIM
}
\usage{
get.futures.contract(contract, colnames = c("open", "high", "low", "close", "volume", "openinterest"), from.date = NULL, units = "days", bars = 1)
}
\arguments{
  \item{contract}{generic futures contract}
  \item{colnames}{ what cols do you want to read for the futures contract}
  \item{from.date}{ start date (not implemented yet) }
  \item{units}{ minutes or days}
  \item{bars}{ how many minutes or days for each bar }
}
\value{
a tseries object
}
\references{LIM}
\author{Whit}
\examples{
x <- get.futures.contract("TY")
tail(x)
}
\keyword{ts}

