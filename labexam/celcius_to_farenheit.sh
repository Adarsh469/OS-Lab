#!/bin/bash

echo "Enter temperature in Celsius:"
read c

f=$(( (c * 9 / 5) + 32 ))

echo "Temperature in Fahrenheit is: $f"
