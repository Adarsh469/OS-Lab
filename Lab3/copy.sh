#!/bin/bash
echo -n "Enter the extension (e.g., .text): "
read ext
echo -n "Enter the destination folder name: "
read folder

mkdir -p "$folder"
cp *"$ext" "$folder/"
echo "Files with $ext copied to $folder."
