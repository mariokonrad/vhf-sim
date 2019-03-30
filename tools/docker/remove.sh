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

docker rmi -f $1:build
