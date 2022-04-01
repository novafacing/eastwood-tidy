#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

if [[ "$#" != 1 ]]; then
    echo "usage: $0 [path/to/llvm-project]"
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "$1: directory not found"
    exit 1
fi
# Generate patch command
GET_NAME_ARGS=$'-type f -name $(printf REPL | awk -F\'.\' \'BEGIN{OFS=FS};NF{NF-=1};1\' | awk -F\'/\' \'{print $NF}\') -exec patch {} REPL \;'
echo "GET_NAME_ARGS: ${GET_NAME_ARGS}"
GET_NAME="find $1 ${GET_NAME_ARGS}"
echo "GET_NAME: ${GET_NAME}"

find "${SCRIPT_DIR}" -type f -name '*.patch' -print0 | xargs -0 -n1 -I REPL bash -c "${GET_NAME}"
