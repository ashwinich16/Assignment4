#!/bin/bash

# Create necessary directories
mkdir -p build

# Compile the program in debug and optimized modes
g++ -g -o build/debug Publication.cpp BibParser.cpp main.cpp
g++ -O2 -o build/optimized Publication.cpp BibParser.cpp main.cpp

echo "Build complete. Executables are located in the build/ directory."
