#!/bin/bash

cc -o buildjust buildjust.c

if [ $? -eq 0 ]; then
    clear
    ./buildjust
else
    echo "Build failed"
fi