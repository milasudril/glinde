#!/bin/bash

rm -rf __doc
doxygen

for i in __doc/html/*.html; do
	echo "Processing $i"
	xsltproc --novalid tagfilter.xsl "$i" 2>/dev/null > "temp.html"
	mv temp.html "$i"
done
