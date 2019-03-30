#!/bin/bash -e

script_dir=$(dirname `readlink -e $0`)
project_dir=${script_dir}/..

if [ $# != 1 ] ; then
	echo "error: invalid number of arguments"
	echo ""
	echo "choose a target distribution:"
	find tools/docker -mindepth 1 -type d -printf "- %f\n"
	exit 1
fi

target=${1}
if [ ! -d ${script_dir}/docker/${target} ] ; then
	echo "error: specified target does not exist: ${target}"
	exit 1
fi

cd ${project_dir}
docker run \
	--interactive \
	--tty \
	--rm \
	--read-only \
	--volume $(pwd):$(pwd) \
	--workdir $(pwd) \
	--env HOME=$(pwd) \
	--user $(id -u):$(id -g) \
	${target}:build \
	bash -c "tools/build-vhf.sh ${target}"

