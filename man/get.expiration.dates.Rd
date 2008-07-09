\name{get.expiration.dates}
\alias{get.expiration.dates}
\title{read data from LIM}
\description{
read the expiration dates of the individual contracts of a generic
futures contract
}
\usage{
get.expiration.dates(genericContract, units = "days", bars = 1)
}
\arguments{
  \item{genericContract}{the name of a generic futures contract}
  \item{units}{days or minutes}
  \item{bars}{how many units}
}
\value{
a named vector of POSIXct dates corresponding to the expiration dates of the
individual futures contracts
}
\references{LIM}
\author{Whit}
\examples{
x <- get.expiration.dates("TY")
x
}
\keyword{ts}
