#!/bin/bash
Nbondlist=`seq 1 $1`
Nsitelist=`seq 1 $2`


if  [[ -a makesubtopolist$2.log ]]; then
    echo "moving makesubtopolist$2.log to makesubtopolist$2.log.old"
    mv makesubtopolist$2.log makesubtopolist$2.log.old
fi

for Nsite in $Nsitelist
  do
  for Nbond in $Nbondlist
    do
    if  [[ -a graphs/bondgraphlist${Nbond}_${Nsite}.dat ]]; then
	nice time ./makesubtopolist.e $Nbond $Nsite >> makesubtopolist$2.log	
    fi
  done
done

