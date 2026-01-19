#!/bin/bash

file_count=$(find . -type f -name "*.txt" | wc -l)

if [ "$file_count" -eq 0 ]; then
    echo "No .txt files were found in this directory."
    exit 0
fi

echo "Found $file_count files. Beginning conversion..."

find . -type f -name "*.txt" -exec sh -c '
    old_file="$1"
    new_file="${1%.txt}.text"
    mv "$old_file" "$new_file"
' _ {} \;

echo "All $file_count files have been renamed."
