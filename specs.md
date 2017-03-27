

Console
-------

Example (VGA console):

{
 rows,i32:80
,cols,i32:25
,charmap,s:"charmap.png" # The rasterized glyphs
,colortab,f32: # Must have 16 entries
	[
#Dark colors
	 0.00,0.00,0.00,1.00
	,0.00,0.00,0.67,1.00
	,0.00,0.67,0.00,1.00
	,0.00,0.67,0.67,1.00
	,0.67,0.00,0.00,1.00
	,0.67,0.00,0.67,1.00
	,0.67,0.33,0.00,1.00
	,0.67,0.67,0.67,1.00
#Bright colors
	,0.33,0.33,0.33,1.00
	,0.33,0.33,1.00,1.00
	,0.33,1.00,0.33,1.00
	,0.33,1.00,1.00,1.00
	,1.00,0.33,0.33,1.00
	,1.00,0.33,1.00,1.00
	,1.00,1.00,0.33,1.00
	,1.00,1.00,1.00,1.00
	]
,colormode,s:"srgb" #see image.cpp
,command_processor,s:"CmdProc"  #Created, and defined by action program
}

