#!/bin/bash

echo "Enter filename:"
read file

line=1

> oddfile
> evenfile

while read text
do
    if [ $((line % 2)) -eq 0 ]; then
        echo "$text" >> evenfile
    else
        echo "$text" >> oddfile
    fi
    line=$((line + 1))
done < "$file"
