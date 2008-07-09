\name{do.roll}
\alias{do.roll}
\title{roll a futures contract list into a tseries}
\description{
calculates the roll dates for a list of individual futures contracts
}
\usage{
do.roll(contract.list)
}
\arguments{
  \item{contract.list}{list of individual futures contracts}
}
\value{
tseries object
}
\author{Whit}
\examples{
x <- load.coml("TY")
y <- do.roll(x)
tail(y)
}
\keyword{ts}

