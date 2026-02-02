#!/bin/bash

echo "Enter a number:"
read n

if [ "$n" -le 1 ]; then
    echo "$n is NOT a prime number"
    exit 0
fi

flag=0

for ((i=2; i<=n/2; i++))
do
    if [ $((n % i)) -eq 0 ]; then
        flag=1
        break
    fi
done

if [ $flag -eq 0 ]; then
    echo "$n is a Prime number"
else
    echo "$n is NOT a Prime number"
fi
