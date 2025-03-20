#!/bin/bash

# Dependencies: rsync

SCRIPT_DIR="$(dirname "$(realpath "$0")")"

standalones="sta"
base_project="base-project"

usage(){
    echo "Usage: $0 project_name"
    echo "  project_name Different than '$base_project'"
}

if [ $# -ne 1 ]; then
    usage
    exit 1
fi

name="$1"

if [ "$name" == "$base_project" ]; then
    usage
    exit 1
fi

project="$SCRIPT_DIR/$standalones/$name"

if [ -d "$project" ]; then
    echo "project '$project' already exists"
    exit 1
fi

rsync -av "$SCRIPT_DIR/$base_project/" "$project/"

./gen-clangd-compile-commands.sh "$name"
