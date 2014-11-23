#!/bin/bash

set -e

function main {
    while read line; do
        echo $line |
        sed 's/...passed/\x1b[1;32m   PASSED\x1b[m/g' |
        sed 's/...FAILED/\x1b[1;31m   FAILED\x1b[m/g'
    done
}

main
