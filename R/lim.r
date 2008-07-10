get.relation <- function(contract,colnames=NULL,units="days",bars=1) {
  .Call("getRelation", contract, colnames, units, as.integer(bars), PACKAGE="RLIM")
}

get.contracts <- function(genericContract, units="days",  bars=1) {
    .Call("getContracts", genericContract, units, as.integer(bars), PACKAGE="RLIM")
}
