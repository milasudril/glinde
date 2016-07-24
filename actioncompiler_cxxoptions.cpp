//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_cxxoptions.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "actioncompiler_cxxoptions.h"
#include "resourceobject.h"

using namespace Glinde;
using namespace ActionCompiler;

CxxOptions::CxxOptions()
	{
	m_optlevel=0.0f;
	m_cxx_minversion=0;
	m_options=0;
	debugEnable().warnAllEnable().consoleEnable();
	}

CxxOptions::CxxOptions(const ResourceObject& object):CxxOptions()
	{
	if(object.objectExists("optlevel"))
		{
		optlevelSet(static_cast<float>(
			static_cast<double>(object.objectGet("optlevel"))));
		}

	if(object.objectExists("cxx_minversion"))
		{
		cxxMinversion()=static_cast<uint64_t>(
			static_cast<long long int>(object.objectGet("cxx_minversion")));
		}

	if(object.objectExists("debug"))
		{
		if( static_cast<long long int>(object.objectGet("debug")))
			{debugEnable();}
		}

	if(object.objectExists("native"))
		{
		if( static_cast<long long int>(object.objectGet("native")))
			{nativeEnable();}
		}

	if(object.objectExists("m32"))
		{
		if( static_cast<long long int>(object.objectGet("m32")))
			{m32Enable();}
		}

	if(object.objectExists("pic"))
		{
		if( static_cast<long long int>(object.objectGet("pic")))
			{picEnable();}
		}


	if(object.objectExists("warnall"))
		{
		if( static_cast<long long int>(object.objectGet("warnall")))
			{warnAllEnable();}
		}

	if(object.objectExists("warnall"))
		{
		if( static_cast<long long int>(object.objectGet("warnall")))
			{warnAllEnable();}
		}

	if(object.objectExists("warnerror"))
		{
		if( static_cast<long long int>(object.objectGet("warnerror")))
			{warnErrorEnable();}
		}


	if(object.objectExists("console"))
		{
		if( static_cast<long long int>(object.objectGet("console")))
			{consoleEnable();}
		}
	}
