#!/bin/bash

# Check if the build directory exists
if [ -d "build" ]; then
    echo "Build directory already exists. Removing..."
    rm -rf build
fi

# Create a new build directory
mkdir build

# Navigate to the build directory
cd build

# Run CMake and build the project
cmake ..

# Check if a specific test name is passed as an argument
if [ ! -z "$1" ]; then
    echo "Building test: $1"
    make $1
    echo "Running test: $1"
    ./$1
else
    echo "No specific test provided. Making and running all tests..."
    make
    ctest
fi

# Return to the original directory
cd ..