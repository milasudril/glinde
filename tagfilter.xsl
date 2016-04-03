<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output mode="html" omit-xml-declaration="yes" indent="no" media-type="text/html" />

<xsl:template match="node()|@*">
<xsl:copy>
<xsl:apply-templates select="node()|@*"/>
</xsl:copy>
</xsl:template>

<xsl:template match="div[@class='contents']/p[1]" />

<xsl:template match="div[@class='contents']">
<xsl:apply-templates select="*[not(@class='textblock' or text()='Detailed Description')]"/>
</xsl:template>

<xsl:template match="body">
<xsl:apply-templates select="div[@id='top']" />
<article>
<xsl:apply-templates select="div[@class='header']" />
<xsl:apply-templates select="div[@class='contents']/div[@class='textblock']" />
<xsl:apply-templates select="div[@class='contents']" />
</article>
</xsl:template>

<xsl:template match="div[@id='top']">
<xsl:apply-templates select="header" />
<nav><xsl:apply-templates select="div/ul" /></nav>
</xsl:template>

<xsl:template match="div[@class='title']">
<h1><xsl:apply-templates select="./node()" /></h1>
</xsl:template>

<xsl:template match="div[@class='header']">
<xsl:apply-templates select="div[@class='headertitle']/div[@class='title']" />
</xsl:template>

<xsl:template match="div[@class='navpath']/ul">
<ul class="navpath">
<xsl:apply-templates />
</ul>
</xsl:template>

<xsl:template match="div[@class='navpath']">
<xsl:apply-templates />
</xsl:template>

<!--Remove the *align attribute-->
<xsl:template match="@align" />
<xsl:template match="@valign" />

<!--Remove extra separator rows from tables-->
<xsl:template match="tr[starts-with(@class,'separator:')]" />

<!--Remove empty 'a' tags-->
<xsl:template match="a[not(a) and not(normalize-space())]" />

<!--Groupheaders-->
<xsl:template match="h2[@class='groupheader']">
<h2>
<xsl:choose>
	<xsl:when test="./a[1]/@name">
		<xsl:attribute name="id">
			<xsl:value-of select="./a[1]/@name"/>
		</xsl:attribute>
	</xsl:when>
	<xsl:when test="./a[1]/@id">
		<xsl:attribute name="id">
			<xsl:value-of select="./a[1]/@id"/>
		</xsl:attribute>
	</xsl:when>
	<xsl:otherwise>
		<xsl:attribute name="id">
			<xsl:value-of select="preceding-sibling::a[1]/@id "/>
		</xsl:attribute>
	</xsl:otherwise>
</xsl:choose>
<xsl:apply-templates select="./node()"/>
</h2>
</xsl:template>

<xsl:template match="h3">
<h3>
<xsl:choose>
	<xsl:when test="./a[1]/@name">
		<xsl:attribute name="id">
			<xsl:value-of select="./a[1]/@name"/>
		</xsl:attribute>
	</xsl:when>
	<xsl:when test="./a[1]/@id">
		<xsl:attribute name="id">
			<xsl:value-of select="./a[1]/@id"/>
		</xsl:attribute>
	</xsl:when>
	<xsl:otherwise>
		<xsl:attribute name="id">
			<xsl:value-of select="preceding-sibling::a[1]/@id "/>
		</xsl:attribute>
	</xsl:otherwise>
</xsl:choose>
<xsl:apply-templates select="./node()"/>
</h3>
</xsl:template>

<!--Header rows-->
<xsl:template match="tr[@class='heading']">
<xsl:apply-templates select="td[1]/h2[1]" />
</xsl:template>

<xsl:key name="memitem" match="table[@class='memberdecls']/tr" use="@class" />

<xsl:template match="table[@class='memberdecls']">
<!--Headers should go outside tables-->
<xsl:apply-templates select="tr[@class='heading']" />
<table class="memberdecls">
<!--xsl:apply-templates select="tr[not(@class='heading')]" /-->
<tr>
<xsl:for-each select="tr[not(@class='heading')]">
<xsl:choose>
<xsl:when test="starts-with(@class,'separator') and not(position()=last())">
<xsl:text disable-output-escaping="yes">&lt;/tr&gt;
&lt;tr&gt;</xsl:text>
</xsl:when>
<xsl:otherwise>
<xsl:apply-templates />
</xsl:otherwise>
</xsl:choose>
</xsl:for-each>
</tr>
</table>
</xsl:template>

<!--Member items-->
<xsl:template match="div[@class='memitem']">
<h3 class="memproto">
<xsl:attribute name="id">
<xsl:value-of select="preceding-sibling::a[1]/@id "/>
</xsl:attribute>
<xsl:apply-templates select="div[@class='memproto']"/>
</h3>
<xsl:apply-templates select="div[@class='memproto']/table[@class='mlabels']/tr/td[@class='mlabels-right']/*" />
<xsl:apply-templates select="div[@class='memdoc']" />
</xsl:template>

<!--Member title-->
<xsl:template match="div[@class='memtemplate']">
<span class="memtemplate"><xsl:apply-templates /></span>
</xsl:template>

<xsl:template match="table[@class='memname']/tr/td">
<span>
<xsl:if test="@class">
<xsl:attribute name="class">
<xsl:value-of select="@class"/>
</xsl:attribute>
</xsl:if>
<xsl:apply-templates /></span>
</xsl:template>

<xsl:template match="table[@class='memname']/tr">
<span><xsl:apply-templates select="td"/></span>
</xsl:template>

<xsl:template match="table[@class='memname']">
<span>
<xsl:for-each select="tr[not(@class='memtemplate')]">
<xsl:apply-templates/>
</xsl:for-each>
</span>
</xsl:template>

<xsl:template match="div[@class='memproto']">
<xsl:apply-templates />
</xsl:template>

<!--Handle tag soup from doxygen-->
<xsl:template match="table[@class='mlabels']">
<xsl:apply-templates select="tr/td[@class='mlabels-left']/*" />
<!--xsl:apply-templates select="tr/td[@class='mlabels-right']/*" /-->
</xsl:template>

<!--Render labels as a paragraph-->
<xsl:template match="span[@class='mlabels']">
<p>This entity is <xsl:apply-templates /></p>
</xsl:template>

<!--Member documentation-->
<xsl:template match="div[@class='memdoc']">
<xsl:apply-templates />
</xsl:template>

<!--Parameter description-->
<xsl:template match="dl[@class='params']" >
<h4>Parameters</h4>
<xsl:apply-templates select="dd[1]/table" />
</xsl:template>

<!--Return-value description-->
<xsl:template match="dl[@class='section return']" >
<h4>Return value</h4>
<xsl:for-each select="dd/.">
<xsl:choose>
<xsl:when test="not(name()='p')">
<p><xsl:apply-templates /></p>
</xsl:when>
<xsl:otherwise>
<p><xsl:apply-templates /></p>
</xsl:otherwise>
</xsl:choose>
</xsl:for-each>
</xsl:template>


<!--fieldtable-->
<xsl:template match="td[@class='fieldname']">
<th class="fieldname"><xsl:apply-templates /></th>
</xsl:template>

<xsl:template match="td[@class='fielddoc']">
<td class="fielddesc"><xsl:apply-templates /></td>
</xsl:template>

<xsl:template match="table[@class='fieldtable']">
<table class="fieldtable">
<xsl:for-each select="tr[position()>1]">
<tr><xsl:apply-templates /></tr>
</xsl:for-each>
</table>
</xsl:template>

</xsl:stylesheet>
