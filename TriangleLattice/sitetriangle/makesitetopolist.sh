#!/bin/bash 

LIMIT=$1

rm makesitetopolist.log
for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
do
  time ./makesitetopolist.e $a >>makesitetopolist.log
done
