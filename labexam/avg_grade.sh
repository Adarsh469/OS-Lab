#!/bin/bash

echo "Enter number of subjects:"
read n

sum=0
marks=()

for ((i=1; i<=n; i++))
do
    echo "Enter marks for subject $i:"
    read m
    marks+=($m)
    sum=$((sum + m))
done

avg=$((sum / n))

echo "Marks entered:"
for m in "${marks[@]}"
do
    echo "$m"
done

echo "Average = $avg"

if [ $avg -ge 75 ]; then
    echo "Grade: A"
elif [ $avg -ge 60 ]; then
    echo "Grade: B"
elif [ $avg -ge 50 ]; then
    echo "Grade: C"
else
    echo "Grade: Fail"
fi
