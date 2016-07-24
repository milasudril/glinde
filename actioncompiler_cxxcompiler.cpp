//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_cxxcompiler.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "actioncompiler_cxxcompiler.h"
#include "actioncompiler_cxxpptokenizer.h"
#include "pipein.h"
#include "readbuffer.h"
#include "errormessage.h"
#include "logwriter.h"
#include "variant.h"

using namespace Glinde;
using namespace Glinde::ActionCompiler;

static String compilerIdentify()
	{
	PipeIn cxxpipe(PipeIn::Stream::STDOUT,"g++",{"--version"});

	ReadBuffer buffer(cxxpipe);
	String version;
	while(!buffer.eof())
		{
		version.append(buffer.byteRead());
		}

	auto result=cxxpipe.exitStatusGet();
	if(result!=0)
		{
		throw ErrorMessage("It was not possible to identify the C++ compiler, "
			"but the corresponding command exists. "
			"The compiler returned status code #0;.",{result});
		}

	return std::move(version);
	}

static uint64_t cxxVersionDefaultGet()
	{
#if _WIN32 || _WIN64
	static const char* NULLFILE="NUL";
#else
	static const char* NULLFILE="/dev/null";
#endif
	PipeIn cxxpipe(PipeIn::Stream::STDOUT,"g++",{"-E","-dM","-x","c++",NULLFILE});
	CxxPPTokenizer tokenizer(cxxpipe);
	CxxPPTokenizer::Token token;
	enum class Mode:uint8_t{NORMAL,DEFINE,VALUE};
	auto mode=Mode::NORMAL;
	String symbol_name;
	while(tokenizer.read(token))
		{
		switch(mode)
			{
			case Mode::NORMAL:
				switch(token.type)
					{
					case CxxPPTokenizer::Token::Type::DIRECTIVE:
						if(token.value=="define")
							{
							mode=Mode::DEFINE;
							}
						break;
					default:
						break;
					}
				break;
			case Mode::DEFINE:
				symbol_name=token.value;
				mode=Mode::VALUE;
				break;
			case Mode::VALUE:
				if(strcmp(symbol_name.begin(),"__cplusplus")==0)
					{return atoll(token.value.begin());}
				mode=Mode::NORMAL;
				break;
			}
		}
	return 0;
	}

CxxCompiler::CxxCompiler(const CxxOptions& options):m_version(compilerIdentify())
	,m_options(options),m_cxxversion_default(cxxVersionDefaultGet())
	{
	}



static size_t stringPointersGet(const char** buffer,const Range<const String>& strings)
	{
//	WARNING: Strings must not be temporary here!!!
	auto ptr_src=strings.begin();
	auto ptr_end=strings.end();
	while(ptr_src!=ptr_end)
		{
		*buffer=ptr_src->begin();
		++buffer;
		++ptr_src;
		}
	return strings.length();
	}

static void compilerRun(const Range<const String>& arg_strings)
	{
	ArraySimple<const char*> args(arg_strings.length());
	stringPointersGet(args.begin(),arg_strings);
	PipeIn cxxpipe(PipeIn::Stream::STDERR,"g++",args);
	ReadBuffer buffer(cxxpipe);
	String line;
	while(!buffer.eof())
		{
		auto ch_in=buffer.byteRead();
		if(ch_in!='\n')
			{line.append(ch_in);}
		else
			{
			logWrite(Log::MessageType::LINE_QUOTE,"#0;",{line.begin()});
			line.clear();
			}
		}

	auto result=cxxpipe.exitStatusGet();
	if(result!=0)
		{
		throw ErrorMessage(
"The compiler returned status code #0;. See STDERR for more information.",{result});
		}
	}

static const char* cxxNameGet(uint64_t version)
	{
	if(version<201103)
		{return "-std=c++03";}
	if(version>=201103 && version<201402)
		{return "-std=c++11";}
	return "-std=c++14";
	}

static
ArrayDynamic<String> optionStringCreate(const CxxOptions& options
	,uint64_t cxxMinversionDefault)
	{
	ArrayDynamic<String> ret;

	auto optlevel=std::min(static_cast<int>(4*options.optlevelGet()),3);
	if(optlevel>0)
		{
		char buffer[4];
		sprintf(buffer,"-O%d",optlevel);
		ret.append(String(buffer));
		}

	if(options.cxxMinversion() > cxxMinversionDefault)
		{
		logWrite(Log::MessageType::WARNING
			,"Requesting C++ version #0; (default is #1;)"
			,{options.cxxMinversion(),cxxMinversionDefault});
		ret.append(String(cxxNameGet(options.cxxMinversion())));
		}

	if(options.debug())
		{ret.append(String("-g"));}
	else
		{ret.append(String("-DNDEBUG"));}

	if(options.fastmath())
		{ret.append(String("-ffast-math"));}

	if(options.native())
		{ret.append(String("-march=native"));}

	if(options.m32())
		{ret.append(String("-m32"));}

	if(options.pic())
		{ret.append(String("-fpic"));}


	if(options.warnAll())
		{ret.append(String("-Wall"));}

	if(options.warnSize())
		{ret.append(String("-Wconversion")).append(String("-Wtype-limits"));}

	if(options.warnError())
		{ret.append(String("-Werror"));}


	if(options.compileOnly())
		{ret.append(String("-c"));}
	else
		{
#if _WIN32 || _WIN64
		ret.append("-mwindows");
		if(options.console())
			{ret.append(String("-mconsole"));}
#endif
		if(options.shared())
			{ret.append(String("-shared"));}

		if(!options.debug())
			{ret.append(String("-s"));}
		}

	return std::move(ret);
	}

static
ArrayDynamic<String> mangle(const char* prefix,const Range<const String>& strings)
	{
	ArrayDynamic<String> ret;
	auto ptr=strings.begin();
	auto ptr_end=strings.end();
	while(ptr!=ptr_end)
		{
		ret.append(String(prefix).append(*ptr));
		++ptr;
		}
	return std::move(ret);
	}

void CxxCompiler::compile(const char* source,const char* dest
	,const CxxOptions& opts) const
	{
	CxxOptions opts_temp(m_options);
	opts_temp|=opts;
	compilerRun(
		optionStringCreate( opts_temp.optlevelSet(opts.optlevelGet())
			.compileOnlyEnable() ,m_cxxversion_default )
		.append(mangle("-I",opts_temp.includeDirGet()) ).append(String("-o"))
		.append(String(dest)).append(String(source)));
	}

void CxxCompiler::link(const Range<const String>& files,const char* dest
	,const CxxOptions& opts) const
	{
	CxxOptions opts_temp(m_options);
	opts_temp|=opts;
	compilerRun(
		optionStringCreate( opts_temp.optlevelSet(opts.optlevelGet())
			.compileOnlyDisable(),m_cxxversion_default )
		.append(mangle("-I",opts_temp.includeDirGet()))
		.append(mangle("-L",opts_temp.libDirGet())).append(String("-o"))
		.append(String(dest)).append(files));
	}
