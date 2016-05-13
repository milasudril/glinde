#!/bin/bash

#Prepare record type documentation
echo '<?xml version="1.0"?>' > recordtype-.xml
echo '<typeindex>' >> recordtype-.xml
for rectype in recordtypes/*.xml; do
	xsltproc --stringparam docparams docparams.xml recordtype.xsl "$rectype" > recordtype-`basename "$rectype"`
	xsltproc --stringparam docparams docparams.xml recordtypeindex.xsl "$rectype" >> recordtype-.xml
done
echo '</typeindex>' >> recordtype-.xml
xsltproc --stringparam docparams docparams.xml rectyperef.xsl recordtype-.xml > rectyperef.xml
tidydox
#clean up
rm recordtype-*.xml

#fix stylesheets
kiss doccolors.txt < doccolors.css > __doc/html/doccolors.css
kiss doccolors.txt < dochighlight.css > __doc/html/dochighlight.css
