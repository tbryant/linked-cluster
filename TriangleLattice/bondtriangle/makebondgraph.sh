#!/bin/bash
if [[ -a "graphs/bondgraph.log" ]]; then
    echo "mv graphs/bondgraph.log graphs/bondgraph.log.old"
    mv graphs/bondgraph.log graphs/bondgraph.log.old    
fi
for a in `seq 1 $2`
do
 time ./makebondgraph.e $1 $a
done 
