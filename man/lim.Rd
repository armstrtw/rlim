\name{lim}
\alias{lim}
\title{load data from LIM}
\description{
load a futures contract, an fx series, or a fundamental series
}
\usage{
lim(ticker)
}
\arguments{
  \item{ticker}{ticker for the type of data you want}
}
\value{
a tseries or list of futures contracts
}
\references{LIM}
\author{Whit}
\examples{
ty <- lim("TY")
eurusd <- lim("EURUSD")
cpi <- lim("CPI_ACT")
}
\keyword{ts}
