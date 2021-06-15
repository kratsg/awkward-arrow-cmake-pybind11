#!/bin/bash

# bash "scrict mode"
set -e
#set -u
set -o pipefail

# Assuming running from top level
if [ ! -d examples ]; then
    mkdir examples
fi
if [ ! -f examples/test.csv ]; then
    curl -sL https://raw.githubusercontent.com/apache/arrow/master/cpp/examples/minimal_build/test.csv -o examples/test.csv
fi

pushd examples
# example_arrow has paths hardcoded
./../build/example_arrow
popd
