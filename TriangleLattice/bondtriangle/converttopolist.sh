#!/bin/bash
Nbondlist=`seq 1 $1`
Nsitelist=`seq 1 $2`

for Nsite in $Nsitelist
  do
  for Nbond in $Nbondlist
    do
    if  [[ -a graphs/bondgraph_topolist${Nbond}_${Nsite}.big ]]; then
	./converttopolist.e $Nbond $Nsite 
    fi
  done
done


# rm maketopolist.log
# for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
# do
#   time ./maketopolist.e $a >> maketopolist.log
# done

