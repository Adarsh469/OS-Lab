#!/bin/bash

echo "Enter filename:"
read file

sed 's|//$|$|' "$file"
