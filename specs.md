
Special textures
================

Console
-------

Example (VGA console):

	{
	 rows,i32:80
	,cols,i32:25
	,charmap,s:"charmap.png" # The rasterized glyphs. Must be have 32x16 character cells
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
	,command_processor,s:"CmdProc"  #Created and defined by action program
	}


Camera
------

	{
	 fov_y,f32:0.78 #Radians
	,aspect_ratio,f32:1.5
	,orientation,f32:[...] #Rotation quaternion
	,pivot,f32:[x,y,x,0] #Pivot point relative to position
	,position,f32:[x,y,z,1]
	}


