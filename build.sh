#!/bin/bash

cc -o ./bin/buildjust buildjust.c

if [ $? -eq 0 ]; then
    clear
    ./bin/buildjust
else
    echo "Build failed"
fi