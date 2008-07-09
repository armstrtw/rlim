\name{get.all.contract.names}
\alias{get.all.contract.names}
\title{read data from LIM}
\description{
reads the names of all the individual futures contract names
}
\usage{
get.all.contract.names(ticker, units = "days", bars = 1)
}
\arguments{
  \item{ticker}{generic futures contract name, TY for instance}
  \item{units}{days or minutes}
  \item{bars}{not all tickers avaialble at certain frequencies}
}
\value{
vector of strings of the individual contract names
}
\references{LIM}
\author{Whit}
\examples{
x <- get.all.contract.names("TY")
x[1]
}
\keyword{ts}
