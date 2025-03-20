#!/bin/bash

# Dependencies: make, bear

SCRIPT_DIR="$(dirname "$(realpath "$0")")"

standalones="sta"

usage(){
    echo "Usage: $0 project_name"
    echo "  project_name Project in $standalones/"
}

if [ $# -ne 1 ]; then
    usage
    exit 1
fi

project="$SCRIPT_DIR/$standalones/$1"

if [ ! -d "$project" ]; then
    echo "project '$project' not found"
    exit 1
fi

cd "$project" || exit 1

make clean-all && bear -- make
