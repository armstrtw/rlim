\name{load.coml}
\alias{load.coml}
\title{read data from lim}
\description{
read in a list of all the futures contracts
}
\usage{
load.coml(contract, colnames = c("open", "high", "low", "close", "volume", "openinterest"), rollDay = "5 Days before Expiration Day", from.date = NULL, units = "days", bars = 1)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{contract}{generic futures contract }
  \item{colnames}{colnames of futures contract you want}
  \item{rollDay}{method to use to calc roll days}
  \item{from.date}{start date (not not implemented)}
  \item{units}{minutes or days }
  \item{bars}{how many minutes or days}
}
\value{
a list of futures contracts
}
\references{LIM}
\author{Whit}
\examples{
x <- load.coml("TY")
head(x[[1]])
}
\keyword{ts}

