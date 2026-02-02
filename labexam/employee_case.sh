#!/bin/bash

echo "Enter employee file:"
read file

echo "1. Display employees with salary > 1000"
echo "2. Convert employee names to uppercase"
read choice

case $choice in
1)
    while read name salary
    do
        if [ "$salary" -gt 1000 ]; then
            echo "$name $salary"
        fi
    done < "$file"
    ;;
2)
    while read name salary
    do
        uppername=$(echo "$name" | tr 'a-z' 'A-Z')
        echo "$uppername $salary"
    done < "$file"
    ;;
*)
    echo "Invalid option"
    ;;
esac
