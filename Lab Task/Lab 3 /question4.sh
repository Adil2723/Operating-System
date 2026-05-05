#!/bin/bash

echo "Enter the path to check:"
read path

if [ -e "$path" ]; then
    echo "Path exists: $path"
    
    if [ -f "$path" ]; then
        echo "It is a file"
    fi
    
    if [ -d "$path" ]; then
        echo "It is a directory"
    fi
    
    if [ -r "$path" ]; then
        echo "Readable: Yes"
    else
        echo "Readable: No"
    fi
    
    if [ -w "$path" ]; then
        echo "Writable: Yes"
    else
        echo "Writable: No"
    fi
    
    if [ -x "$path" ]; then
        echo "Executable: Yes"
    else
        echo "Executable: No"
    fi
    
else
    echo "Path does not exist"
fi
