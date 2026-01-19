#!/bin/bash
echo -n "Enter the folder path: "
read folder
echo -n "Enter the string pattern to search: "
read pattern
grep -l "$pattern" "$folder"/*
