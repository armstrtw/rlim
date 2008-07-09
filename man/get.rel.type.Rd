\name{get.rel.type}
\alias{get.rel.type}
\title{read data from LIM}
\description{
read what type of object a symbol is
}
\usage{
get.rel.type(ticker)
}
\arguments{
  \item{ticker}{futures symbol, stock ticker, currency ticker, etc}
}
\value{
a character vector describing the type of object
}
\references{LIM}
\author{Whit}
\examples{
get.rel.type("TY")
get.rel.type("TY_1982M")
get.rel.type("EURJPY")
get.rel.type("CPI_ACT")
}
\keyword{ts}

