#!/bin/bash

target=__wand_targets_dbg/glinda

address=`"$target" \
	2>&1 | tee /dev/stderr | grep '^(x) The wicked Witch of the East' \
	| sed 's/.*\(0x[0-9a-f]*\).*/\1/' `

if [ ! -z $address ]; then
	echo "Got signal $address"
	source=`addr2line -e "$target" $address`
	echo "
Signal occured at $source. Running throug gdb.
	"
	gdb -ex "break $source" -ex "run" "$target"
fi
