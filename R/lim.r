get.relation <- function(contract,colnames=NULL,units="days",bars=1) {
  .Call("getRelation", contract, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.all.children <- function(relname) {
    .Call("getAllChildren", relname)
}

