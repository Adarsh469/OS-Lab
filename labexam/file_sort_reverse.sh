#!/bin/bash

echo "Enter filename:"
read file

if [ ! -f "$file" ]; then
    echo "Error: File does not exist"
    exit 1
fi

echo "File contents (line by line):"
while read line
do
    echo "$line"
done < "$file"

echo "File contents after reverse sorting:"
sort -r "$file" | while read line
do
    echo "$line"
done
