\name{load.coms}
\alias{load.coms}
\title{read data from LIM}
\description{
read the day series and perpetual series for a futures contract
}
\usage{
load.coms(contract, rollDay = "5 Days before Expiration Day")
}
\arguments{
  \item{contract}{name of generic futures contract}
  \item{rollDay}{policy used to compute roll days }
}
\value{
  \item{as}{adjusted series, this is the backwards price adjusted series}
  \item{ds}{day series, the unadjusted perpetual series}
}
\references{LIM}
\author{Whit}
\examples{
x <- load.coms("TY")
head(x$as)
head(x$ds)
}
\keyword{ts}

