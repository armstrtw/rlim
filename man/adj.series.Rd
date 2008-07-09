\name{adj.series}
\alias{adj.series}
\title{read data from LIM}
\description{
reads the perpetual price series from LIM and backwards adjusts the prices
}
\usage{
adj.series(contract, rollDay = "5 Days before Expiration Day")
}
\arguments{
  \item{contract}{ generic contract name (TY for instance) }
  \item{rollDay}{ the policy to use to roll the contract as given by LIM}
}
\details{
rollDay is a string representing a roll policy
}
\value{
a (typically) six column tseries of the price an vol/oi data for
a futures contract
}
\author{Whit}
\examples{
x <- adj.series("TY")
tail(x)
}
\keyword{ts}

