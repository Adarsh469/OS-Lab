#!/bin/bash
echo -n "Enter Basic Salary: "
read basic
echo -n "Enter TA: "
read ta

bonus=$(echo "0.1 * $basic" | bc -l)
gs=$(echo "$basic + $ta + $bonus" | bc -l)

echo "The Gross Salary is: $gs"
