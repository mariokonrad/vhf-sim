#!/bin/bash

script_dir=$(dirname `readlink -e $0`)

if [ $# != 1 ] ; then
	echo "error: invalid number of parameters"
	exit 1
fi

if [ ! -d ${script_dir}/$1 ] ; then
	echo "error: unsupported distribution"
	exit 1
fi

docker run \
	--interactive \
	--tty \
	--rm \
	--read-only \
	--volume $(pwd):$(pwd) \
	--workdir $(pwd) \
	--env HOME=$(pwd) \
	--user $(id -u):$(id -g) \
	$1:build \
	bash

