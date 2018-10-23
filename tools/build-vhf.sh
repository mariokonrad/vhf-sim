#!/bin/bash -e

set +o history

script_dir=$(dirname `readlink -e $0`)
project_dir=${script_dir}/..

if [ $# != 1 ] ; then
	echo "error: invalid number of arguments"
	exit 1
fi

build_dir=${project_dir}/build-${1}

if [ -d ${build_dir} ] ; then
	echo "error: unclean build. abort"
	exit 1
fi

mkdir ${build_dir}
cd ${build_dir}
cmake ${project_dir} -DPACKAGE_SYSTEM_DISTRIBUTION="${1}"
cmake --build .
cpack
