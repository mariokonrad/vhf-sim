#!/bin/bash
set -ev

curl \
	--max-time 300 \
	--upload-file build/vhf-sim-*-${PACKAGE_SYSTEM_DISTRIBUTION}.dmg https://free.keep.sh

