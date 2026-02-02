#!/bin/bash

echo "Enter filename:"
read file

if [ ! -f "$file" ]; then
    echo "File does not exist"
    exit 1
fi

if [ -x "$file" ]; then
    echo "File already has execute permission"
else
    echo "Execute permission not found. Granting permission..."
    chmod +x "$file"
fi

echo "New permissions:"
ls -l "$file"
