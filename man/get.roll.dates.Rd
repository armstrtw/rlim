\name{get.roll.dates}
\alias{get.roll.dates}
\title{get data from LIM}
\description{
read the roll dates for a futures contract from LIM
}
\usage{
get.roll.dates(genericContract, rollDay = "5 Days before Expiration Day")
}
\arguments{
  \item{genericContract}{a futures contract}
  \item{rollDay}{what type of roll day (see LIM documentation)}
}
\value{
vector of POSIXct dates w/ names to match the contract names
}
\references{LIM}
\author{Whit}
\examples{
x <- get.roll.dates("TY")
x
}
\keyword{ts}

