#! /bin/bash

BIN="bin"

echo "Removing old build..."
rm -rf $BIN
rm -f ./src/frontend/syntactic-analysis/bison-parser.* ./src/frontend/lexical-analysis/flex-scanner.*
