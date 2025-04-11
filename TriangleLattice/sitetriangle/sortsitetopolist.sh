#!/bin/bash 

LIMIT=$1

rm sortsitetopolist.log
for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
do
  time ./sortsitetopolist.e $a >>sortsitetopolist.log
done
