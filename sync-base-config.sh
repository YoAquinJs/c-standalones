#!/bin/bash

# Dependencies: rsync

SCRIPT_DIR="$(dirname "$(realpath "$0")")"

standalones="$SCRIPT_DIR/sta"
base_project="$SCRIPT_DIR/base-project"

for project in "$standalones"/*; do
    if [ ! -d "$project" ]; then
        echo "file '$project' found in standalones"
        continue
    fi

    rsync -av "$base_project/" "$project/" \
        --exclude '.gitignore' \
        --exclude 'src/' \
        --exclude 'tests/'
done
