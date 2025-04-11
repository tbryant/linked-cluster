#!/bin/bash
Nbondlist=`seq 1 $1`
Nsitelist=`seq 1 $2`


if  [[ -a maketopolist$2.log ]]; then
    echo "moving maketopolist$2.log to maketopolist$2.log.old"
    mv maketopolist$2.log maketopolist$2.log.old
fi

for Nsite in $Nsitelist
  do
  for Nbond in $Nbondlist
    do
    if  [[ -a graphs/bondgraphlist${Nbond}_${Nsite}.dat ]]; then
# 	if [ -f "graphs/bondgraph_topolist${Nbond}_${Nsite}.dat" ]; then
# 	    echo "graphs/bondgraph_topolist${Nbond}_${Nsite}.dat already exists"
# 	else
      time ./maketopolist.e $Nbond $Nsite >> maketopolist$2.log
#	fi
    fi
  done
done


# rm maketopolist.log
# for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
# do
#   time ./maketopolist.e $a >> maketopolist.log
# done

