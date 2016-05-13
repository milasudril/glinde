<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" method="xml" encoding="utf-8" omit-xml-declaration="yes"/>


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

<xsl:template match="ref">
<xsl:choose>
	<xsl:when test="@kind='type'">
		<a><xsl:attribute name="href"><xsl:value-of select="concat('recordtype-',translate(@name,$uppercase,$lowercase),'.html')" /></xsl:attribute>
		<xsl:value-of select="@name"/></a>
	</xsl:when>
	<xsl:when test="@kind='item'">
		<a><xsl:attribute name="href"><xsl:value-of select="concat('#',@name)" /></xsl:attribute>
		<xsl:value-of select="@name"/></a>
	</xsl:when>
</xsl:choose>
</xsl:template>

<xsl:template match="record-type">
<item><xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
<xsl:apply-templates select="description_brief"/></item>
</xsl:template>

</xsl:stylesheet>
