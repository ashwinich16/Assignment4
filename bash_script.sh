#!/bin/bash

# List of files to create
files=(
    "main.cpp"
    "BibParser.cpp"
    "BibParser.h"
    "publication.cpp"
    "publication.h"
)

# Create each file if it does not already exist
for file in "${files[@]}"; do
    if [[ -f "$file" ]]; then
        echo "$file already exists. Skipping..."
    else
        touch "$file"
        echo "Created $file"
    fi
done
