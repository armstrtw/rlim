\name{day.series}
\alias{day.series}
\title{get data from LIM}
\description{
reads the perpetual series but does not backwards adjust it
}
\usage{
day.series(contract, colnames = c("open", "high", "low", "close",
"volume", "openinterest"), rollDay = "5 Days before Expiration Day",
rollPolicy = "Actual Prices", units = "days", bars = 1)
}
\arguments{
  \item{contract}{futures contract}
  \item{colnames}{what cols you want for the com}
  \item{rollDay}{which day to roll the contract}
  \item{rollPolicy}{how to adjust the prices backwards, forwards, no
    adjustments}
  \item{units}{minutes or days}
  \item{bars}{how many minutes or days}
}
\value{
a tseries object
}
\author{Whit}
\examples{
x <- day.series("TY")
tail(x)
}
\keyword{ts}


