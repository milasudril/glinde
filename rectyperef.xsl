<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" method="xml" encoding="utf-8"/>


<xsl:strip-space elements="*" />
<xsl:preserve-space elements="codeline programlisting highlight verbatim" />

<xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'" />
<xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

<xsl:template match="node()|@*">
<!--Default rule is identy-->
<xsl:copy>
<xsl:apply-templates select="node()|@*"/>
</xsl:copy>
</xsl:template>

<xsl:template match="description_brief">
<td><xsl:apply-templates/></td>
</xsl:template>

<xsl:template match="item">
<tr><th class="membername">
<a><xsl:attribute name="href"><xsl:value-of select="concat('recordtype-',translate(@name,$uppercase,$lowercase),'.html')"/></xsl:attribute>
<xsl:value-of select="@name"/></a></th><xsl:apply-templates /></tr>
</xsl:template>

<xsl:template match="typeindex">
<docset root="true">
<navpath>
<navitem href="index.html">
<xsl:value-of select="document($docparams)/docparams/project/@title"/>
</navitem>
</navpath>
<h1>Record type reference</h1>
<p>This is a list of all record types used within the project.</p>
<table>
<xsl:apply-templates match="item"/>
</table>
</docset>
</xsl:template>
</xsl:stylesheet>
