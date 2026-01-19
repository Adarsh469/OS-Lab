#!/bin/bash
echo -n "Enter the file or directory name: "
read name

if [ -d "$name" ]; then
    echo "$name is a directory."
elif [ -f "$name" ]; then
    echo "$name is a regular file."
else
    echo "$name does not exist or is a special file."
fi
