#!/bin/bash

echo "Enter filename:"
read file

if [ ! -f "$file" ]; then
    echo "Error: File does not exist"
    exit 1
fi

newfile="${file%.*}.out"
line=1

> "$newfile"

while read text
do
    if [ $((line % 2)) -eq 0 ]; then
        echo "$text" | sed 's|^/\*|#|' >> "$newfile"
    else
        echo "$text" >> "$newfile"
    fi
    line=$((line + 1))
done < "$file"

echo "Modified output saved in $newfile"
