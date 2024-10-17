#!/bin/bash

cc -o ./bin/example example.c

if [ $? -eq 0 ]; then
    clear
    ./bin/example
else
    echo "Build failed"
fi