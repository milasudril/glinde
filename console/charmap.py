#@  {
#@  "targets":
#@      [
#@          {
#@			 "name":"charmap_srgb.png"
#@			,"dependencies":[{"ref":"charmap.png","rel":"misc"}]
#@			}
#@      ]
#@  }

import subprocess
import sys

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	subprocess.run(['pngcrush','-sRGB','-brute'
		,in_dir + '/charmap.png'
		,target_dir+'/'+in_dir+'/charmap_srgb.png']\
		,stdout=subprocess.DEVNULL
		,stderr=subprocess.DEVNULL)

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)