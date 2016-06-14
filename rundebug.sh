#!/bin/bash

target=$1

address=`"$target" \
	2>&1 | tee /dev/stderr | grep '^(x) The wicked Witch of the East' \
	| sed 's/.*\(0x[0-9A-F]*\).*/\1/' `

if [ ! -z $address ]; then
	echo "Got signal $address"
	source=`addr2line -e "$target" $address`
	echo "
Signal occured at $source. Running through gdb.
	"
	gdb -ex "break $source" -ex "run" "$target"
fi
