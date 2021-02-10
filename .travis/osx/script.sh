#!/bin/bash
set -ev

cmake --build build
(cd build ; cpack --verbose -G DragNDrop)

