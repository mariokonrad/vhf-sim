#!/bin/bash
set -ev

export PATH=/opt/cmake/bin:${PATH}

set +e
if [ "$QT_BASE" = "55"  ]; then source /opt/qt55/bin/qt55-env.sh   ; export QTHOME=/opt/qt55  ; fi
if [ "$QT_BASE" = "510" ]; then source /opt/qt510/bin/qt510-env.sh ; export QTHOME=/opt/qt510 ; fi
if [ "$QT_BASE" = "512" ]; then source /opt/qt512/bin/qt512-env.sh ; export QTHOME=/opt/qt512 ; fi
set -e

cmake --build build
(cd build ; cpack -G DEB)

