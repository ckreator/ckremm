#!/bin/bash
# $1 should be make target

usage() {
	echo "Oops, please use the script right"
	echo "------------------------------------"
	echo "Usage: ./rerun.sh <make target>"
	echo "------------------------------------"
}

if [ -z $1 ]
then
	usage
	exit 1
fi

rm -rf bin/* tmp/*

make clean

make $1

./bin/$1
