#!/usr/bin/python3

import sys
import json
import platform
import os
import os.path
import subprocess
import re
import urllib.parse
import socket

def cxxversionGet():
	with subprocess.Popen(['g++','-dM','-E','-x','c++',os.devnull]\
		,stdout=subprocess.PIPE) as predef_macros:
		for line in predef_macros.stdout.readlines():
			strings=line.decode('utf-8').split(' ');
			if strings[0]=='#define' and strings[1]=='__cplusplus':
				m=re.match('[0-9]*',strings[2])
				return int(re.match('[0-9]*',strings[2]).group(0))
	return 0

def cxxNameGet(version):
	if version>=0 and version<201103:
		return '-std=c++03'
	if version>=201103 and version<201402:
		return '-std=c++11'
	return '-std=c++14'

def flagstringGet(profile,do_link,cxxversion):
	ret=''
	optlevel=max(0,min(int(4*profile.get('optlevel',0)),3))
	debug=0
	if optlevel > 0:
		ret+='-O%d^ '%optlevel

	v=profile.get('cxx_minversion',0)
	if cxxversion < v:
		print('(!) Requesting C++ version %d (default is %d)'%(v\
			,cxxversion))
		ret+=cxxNameGet(v)+'^ '

	if profile.get('debug',1):
		debug=1
		ret+='-g^ '
	else:
		ret+='-DNDEBUG^ '

	if profile.get('fastmath',0):
		ret+='-ffast-math^ '

	if profile.get('native',0):
		ret+='-march=native^ '

	if profile.get('m32',0):
		ret+='-m32^ '

	if profile.get('pic',0):
		ret+='-fpic^ '

	if profile.get('warnall',1):
		ret+='-Wall^ '

	if profile.get('warnsize',0):
		ret+='-Wconversion^ -Wtype-limits^ '

	if profile.get('warnerror',0):
		ret+='-Werror^ '

	if do_link:
		if platform.system()=='Windows':
			ret+='-mwindows^ '
			if profile.get('console',1):
				ret+='-mconsole^ '
		if profile.get('shared',0):
			ret+='-shared^ '
		if not debug:
			ret+='-s^ '
	else:
		ret+='-c^ '

	return ret


def wand_exec(data,profile):
	cxxversion=cxxversionGet()
	flagstring_compile=flagstringGet(data[profile],0,cxxversion)
	flagstring_link=flagstringGet(data[profile],1,cxxversion)

	arg='config_set[components[Wand[actions[treeexport]] Handler^ C++[envblock[LC_ALL[C]] '\
		'commands[ '\
		'compile_obj[g++^ '+flagstring_compile+'"%0"^ -o^ "%1"] '\
		'compile_app[g++^ '+flagstring_link+'"%0"^ %4^-o^ "%1"] '\
		']]'\
		'Wand[paths[targets[__targets_'+socket.gethostname()+'-'\
			+urllib.parse.quote(profile,safe='')+']]]]]'

	subprocess.run(['wand',arg],check=True)

def touch(target,reference):
	ref=os.path.getmtime(reference)
	if ref > os.path.getmtime(target):
		os.utime(target)

def profileChoose(data,default):

	print('\nChoose compilation profile (default is %s):\n'%default)
	for key in data:
		print('  %s'%key)

	print('\n> ',end='',flush=True)
	for line in sys.stdin:

		profile=line.strip();
		if profile:
			if profile in data:
				return profile
			print('> ',end='',flush=True)
		else:
			break

	return default


with open('config_in.json') as config:
	data=json.load(config)

profile='debug'
if len(sys.argv)<=1:
	profile=profileChoose(data,profile)
else:
	profile=sys.argv[1]
print('Compiling using %s profile'%profile)


with open('config.json','w') as config:
	json.dump(data[profile],config)

touch('console.cpp','charmap.png')
touch('actionprogram.cpp','config.json')

wand_exec(data,profile)

os.remove('config.json')
