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

<xsl:template match="description_detailed">
<xsl:apply-templates />
</xsl:template>

<xsl:template match="description_brief">
<xsl:apply-templates />
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

<xsl:template name="item_type">
<xsl:param name="typename"/>
<xsl:choose>
	<xsl:when test="$typename='array' or $typename='string' or $typename='integer' or $typename='float'">
		<xsl:value-of select="$typename"/>
	</xsl:when>
	<xsl:otherwise>
		<a><xsl:attribute name="href"><xsl:value-of select="concat('recordtype-',translate($typename,$uppercase,$lowercase),'.html')"/></xsl:attribute>
<xsl:value-of select="$typename"/></a>
	</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template match="item" mode="brieftable">
<tr><th class="membername"><xsl:call-template name="item_type">
<xsl:with-param name="typename" select="@type"/>
</xsl:call-template>
<xsl:if test="@subtype"> &lt; <xsl:call-template name="item_type">
<xsl:with-param name="typename" select="@subtype"/>
</xsl:call-template> &gt;</xsl:if><xsl:text> </xsl:text>
<a><xsl:attribute name="href"><xsl:value-of select="concat('#',@name)"/></xsl:attribute>
<xsl:value-of select="@name"/></a>
</th><td><xsl:apply-templates select="description_brief"/></td></tr>
</xsl:template>

<xsl:template match="fields" mode="brief">
<table id="fields">
<xsl:apply-templates select="item" mode="brieftable"/>
</table>
</xsl:template>

<xsl:template match="item">
<section>
<h2>
<xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
<xsl:call-template name="item_type">
<xsl:with-param name="typename" select="@type"/>
</xsl:call-template>
<xsl:if test="@subtype"> &lt; <xsl:call-template name="item_type">
<xsl:with-param name="typename" select="@subtype"/>
</xsl:call-template> &gt;</xsl:if><xsl:text> </xsl:text><xsl:value-of select="@name"/>
</h2><xsl:apply-templates select="description_detailed"/>
</section>
</xsl:template>

<xsl:template match="fields">
<xsl:apply-templates select="item"/>
</xsl:template>



<xsl:template match="record-type">
<docset type="record-type" human-name="Record type reference" indexpage="rectyperef">
<navpath>
<navitem href="index.html">
<xsl:value-of select="document($docparams)/docparams/project/@title"/>
</navitem>
<navitem href="rectyperef.html">Record type reference</navitem>
</navpath>
<h1 id="top">record type <xsl:value-of select="@name" /></h1>
<section class="summary">
<xsl:apply-templates select="description_detailed"/>
<p>The following table summarizes all members in this type.</p>
<xsl:apply-templates select="fields" mode="brief"/>
</section>

<xsl:apply-templates select="fields"/>

</docset>
</xsl:template>

</xsl:stylesheet>
