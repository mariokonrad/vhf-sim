#!/bin/bash
set -ev

if [ "$QT_BASE" = "55"  ]; then sudo add-apt-repository ppa:beineri/opt-qt551-trusty                -y; sudo apt-get update -qq ; fi
if [ "$QT_BASE" = "510" ]; then sudo add-apt-repository ppa:beineri/opt-qt-5.10.1-$TRAVIS_DIST_NAME -y; sudo apt-get update -qq ; fi
if [ "$QT_BASE" = "512" ]; then sudo add-apt-repository ppa:beineri/opt-qt-5.12.3-$TRAVIS_DIST_NAME -y; sudo apt-get update -qq ; fi

