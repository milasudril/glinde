<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output mode="html" omit-xml-declaration="yes" indent="no" media-type="text/html" />

<xsl:template match="node()|@*">
<xsl:copy>
<xsl:apply-templates select="node()|@*"/>
</xsl:copy>
</xsl:template>

<xsl:template match="table[@class='memberdecls']/tr">
<tr>
<th class="membername">
<xsl:for-each select="td[not(@class='mdescRight' or @class='mdescLeft')]">
<span><xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute><xsl:apply-templates /></span>
</xsl:for-each>
</th>
<td class="memberdesc">
<xsl:for-each select="td[@class='mdescRight']">
<xsl:apply-templates />
</xsl:for-each>
</td>
</tr>
</xsl:template>
</xsl:stylesheet>
