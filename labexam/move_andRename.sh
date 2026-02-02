#!/bin/bash

echo "Enter file extension (example: txt):"
read ext

echo "Enter destination folder:"
read dest

mkdir -p "$dest"

for file in *.$ext
do
    name="${file%.*}"
    mv "$file" "$dest/$name.sh"
done
