\name{p.series}
\alias{p.series}
\title{read data from LIM}
\description{
read a perpetual series from LIM
}
\usage{

p.series(contract, colnames = c("open", "high", "low", "close",
                 "volume", "openinterest"), rollDay = "5 Days before Expiration Day", rollPolicy = "Actual Prices", units ="days", bars = 1)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{contract}{name of generic futures contract}
  \item{colnames}{colnames of futures series}
  \item{rollDay}{day on which to roll the contract}
  \item{rollPolicy}{policy to use when adjusting prices}
  \item{units}{days or minutes}
  \item{bars}{how many units}
}
\value{
a tseries representing the perpetual contract
}
\references{LIM}
\author{Whit}
\examples{
x <- p.series("TY")
tail(x)
}
\keyword{ts}
