#!/bin/bash                

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/flatbuffers"

./flatc --cpp ../GUI/flatbuffer/config.fbs

cd ../GUI

cmake -B build -S .
cmake --build build



