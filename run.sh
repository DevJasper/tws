#!/bin/bash

# Check if the "build" directory exists
if [ -d "build" ]; then
    # If "build" directory exists, run the build command
    cmake --build build
    build/example/app
else
    # If "build" directory does not exist, create it and run the build command
    cmake -B build -S . -G Ninja
    cmake --build build
    build/example/app
fi
