#!/bin/bash

echo "Enter filename:"
read filename

if [ -f "$filename" ]; then
    date >> "$filename"
    echo "Date and time appended to $filename"
else
    echo "File does not exist"
fi
