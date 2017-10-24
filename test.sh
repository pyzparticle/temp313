#!/bin/bash

count=0
if [ $# -eq 1 ]; then
    while [ $count -lt $1 ]; do
        echo "sample #${count}"
        ./blocks
        ((count++))
    done 
    exit
fi

echo "usage: $0 [repeat count]"
