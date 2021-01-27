#!/bin/bash
set -ev

# Qt
if [ "$QT_BASE" = "55"  ]; then sudo apt-get install -qq qt55base  qt55multimedia  qt55translations  qt55tools  ; fi
if [ "$QT_BASE" = "510" ]; then sudo apt-get install -qq qt510base qt510multimedia qt510translations qt510tools ; fi
if [ "$QT_BASE" = "512" ]; then sudo apt-get install -qq qt512base qt512multimedia qt512translations qt512tools ; fi

# cmake
curl -o cmake.tar.gz -L https://cmake.org/files/v3.15/cmake-3.15.5-Linux-x86_64.tar.gz
mkdir /opt/cmake
tar -xf cmake.tar.gz -C /opt/cmake --strip-components=1
rm -f cmake.tar.gz

