#!/bin/bash

rm -rf __doc
doxygen >/dev/null

cp -n dynsections.js __doc/html
cp starspink.png __doc/html
cp __doc/html/files.html .

if kiss doxstyle.txt < doxstyle.css > __doc/html/doxstyle.css; then
	for i in __doc/html/*.html; do
		temp=`mktemp`
		{
		echo "<!DOCTYPE html>" ;
		sed 's/\&\&/\&amp;\&amp;/g' "$i" | xsltproc --novalid tagfilter.xsl - \
			| xsltproc --html members-merge.xsl - 2>/dev/null;
		}  > temp
		mv temp "$i"
	done
else
	echo "Kiss failed"
fi
