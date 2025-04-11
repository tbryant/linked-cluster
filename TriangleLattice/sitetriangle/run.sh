#!/bin/bash 

LIMIT=$1

for ((a=1; a <= LIMIT ; a++))  # Double parentheses, and "LIMIT" with no "$".
do
  time ./count.e $a 
done
