\name{get.relation}
\alias{get.relation}
\title{read data from lim}
\description{
reads any tseries type object form LIM
}
\usage{
get.relation(contract, colnames = NULL, units = "days", bars = 1)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{contract}{ contract, symbol, or ticker}
  \item{colnames}{ what cols do you want to read}
  \item{from.date}{ starting date (not implemented yet)}
  \item{units}{ minutes or days}
  \item{bars}{ how many minutes or days}
}
\value{
tseries object
}
\references{LIM}
\author{Whit}
\examples{
x <- get.relation("IBM")
tail(x)
}
\keyword{ts}

