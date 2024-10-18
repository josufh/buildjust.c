#!/bin/bash

OUTPUT_DIR="bin"

if [ ! -d "$OUTPUT_DIR" ]; then
  mkdir -p "$OUTPUT_DIR"
fi

cc -o ./"$OUTPUT_DIR/example" example.c

if [ $? -eq 0 ]; then
    #clear
    ./bin/example
else
    echo "Build failed"
fi