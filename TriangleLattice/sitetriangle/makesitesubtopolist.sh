#!/bin/bash 

LIMIT=$1

rm makesitesubtopolist.log
for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
do
  time ./makesitesubtopolist.e $a >>makesitesubtopolist.log
done
