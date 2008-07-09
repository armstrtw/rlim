\name{dates.com}
\alias{dates.com}
\title{read dates from a series of futures contracts}
\description{
grabs the dates for each individual futures contract in a list of futures contracts
}
\usage{
dates.com(x)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{x}{a list of individual futures contracts}
}
\value{
a list of dates, each list element corresponding to an individual futures contract
}
\author{Whit}
\examples{
x <- load.coml("TY")
# FIXME: make dates generic and replace call to
# dates.com with call to dates
x.dates <- dates.com(x)
}
\keyword{ts}

