#!/bin/bash
if [ -e "filter" ]; then
    rm filter
    printf "removed old executable.\n"
fi
make filter
printf "ready now run './filter'\n"