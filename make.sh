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

# Check if the -t flag for tests is passed
if [ "$1" == "-t" ]; then
    # Check if a specific test name is passed as the second argument
    if [ ! -z "$2" ]; then
        echo "Building: $2"
        make $2
        echo "Running test: $2"
        ./$2
    else
        echo "Building..."
        make
        echo "Running all tests since no specific test is provided..."
        ctest
    fi
else
    echo "Building..."
    make
fi

# Return to the original directory
cd ..