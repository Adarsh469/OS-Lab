#!/bin/bash

echo "Choose shape:"
echo "1. Cylinder"
echo "2. Sphere"
read choice

pi=3.14

case $choice in
1)
    echo "Enter radius:"
    read r
    echo "Enter height:"
    read h
    vol=$(echo "$pi * $r * $r * $h" | bc)
    echo "Volume of Cylinder = $vol"
    ;;
2)
    echo "Enter radius:"
    read r
    vol=$(echo "4/3 * $pi * $r * $r * $r" | bc -l)
    echo "Volume of Sphere = $vol"
    ;;
*)
    echo "Invalid choice"
    ;;
esac
