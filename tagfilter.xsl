<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" omit-xml-declaration="yes"/>
	<xsl:template match="node()|@*">
		<xsl:if test="not(name(.)='tr' and starts-with(@class,'separator:'))">
			<xsl:copy>
				<xsl:apply-templates select="node()|@*"/>
			</xsl:copy>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
