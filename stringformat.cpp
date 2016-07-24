//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"stringformat.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "stringformat.h"
#include "variant.h"
#include "range.h"
#include <algorithm>
#include <cstring>

using namespace Glinde;

template<class T>
T string2uint(const Range<const char>& str_range)
	{
	auto ptr_begin=str_range.begin();
	auto ptr_end=str_range.end();
	T ret=0;
	auto digit=1;
	while(ptr_begin!=ptr_end)
		{
		--ptr_end;
		auto ch_in=*ptr_end;
		if( !(ch_in>='0'&& ch_in<='9') )
			{return ret;}
		ret+=digit*(ch_in - '0');
		digit*=10;
		}
	return ret;
	}


static const char* digits="0123456789ABCDEF";

template<class T>
static char* convert_impl(char* buffer,typename std::make_unsigned<T>::type x
	,unsigned int radix) noexcept
	{
	auto begin=buffer;
	while(x)
		{
		typename std::make_unsigned<T>::type rem=x%radix;
		x/=radix;
		*buffer = digits[rem];
		++buffer;
		}
	if(buffer==begin)
		{
		*buffer=digits[0];
		++buffer;
		}
	*buffer='\0';
	std::reverse(begin,buffer);
	return buffer;
	}

template<class T>
static char* convert(char* result,T value,unsigned int radix) noexcept
	{
	if(std::is_signed<T>::value)
		{
		if(value < 0)
			{
			*result='-';
			++result;
			value=-value;
			}

		typename std::make_unsigned<T>::type x=value;
		return convert_impl<T>(result,x,radix);
		}
	else
		{return convert_impl<T>(result,value,radix);}
	}

///Pointer

static size_t format(const Range<char>& range,const void* ptr
	,const Range<const char>& argstring) noexcept
	{
	auto pos=range.begin();
	if(ptr==nullptr)
		{
		auto n=std::min(static_cast<size_t>(5),range.length());
		memcpy(pos,"null",n);
		return n;
		}
	else
		{
		char buffer[2 + 2*sizeof(ptr)+1]={"0x"};
		auto pos_out=convert(buffer+2,reinterpret_cast<uintptr_t>(ptr),16);
		auto n=std::min(static_cast<size_t>(pos_out-buffer),range.length());
		memcpy(pos,buffer,n);
		return n;
		}
	return 0;
	}

///String constant

static size_t format(const Range<char>& range,const char* cstr
	,const Range<const char>& argstring) noexcept
	{
	auto ptr=range.begin();
	auto ptr_end=range.end();
	while(*cstr!='\0' && ptr!=ptr_end)
		{
	//	TODO: Add support for various transformation filters: UPPER/lowecase... .
		*ptr=*cstr;
		++ptr;
		++cstr;
		}
	return static_cast<size_t>(ptr-range.begin());
	}

///Integer printing stuff

static unsigned int radixGet(const Range<const char>& argstring)
	{
	auto args_begin=argstring.begin();
	auto args_end=argstring.end();
	enum class State:unsigned int{NORMAL,ARGS_BEGIN,RADIX,DONE};
	auto state=State::NORMAL;
	auto radix=10u;
	const char* radix_pos=nullptr;
	while(args_begin!=args_end && state!=State::DONE)
		{
		auto ch_in=*args_begin;
		switch(state)
			{
			case State::NORMAL:
				switch(ch_in)
					{
					case 'b':
						radix=2;
						state=State::DONE;
						break;
					case 'o':
						radix=8;
						state=State::DONE;
						break;
					case 'd':
						radix=10;
						state=State::DONE;
						break;
					case 'h':
						radix=16;
						state=State::DONE;
						break;
					case ':':
						state=State::ARGS_BEGIN;
						break;
					default:
						return radix;
					}
				break;

			case State::ARGS_BEGIN:
				if(ch_in>='0' && ch_in<='9')
					{
					radix_pos=args_begin;
					state=State::RADIX;
					}
				else
					{return radix;}
				break;

			case State::RADIX:
				if(!(ch_in>='0' && ch_in<='9'))
					{
					state=State::DONE;
					radix=string2uint<unsigned int>(Range<const char>(radix_pos,args_begin));
					}
				break;

			case State::DONE:
				break;
			}
		++args_begin;
		}
	if(args_begin==args_end && radix_pos!=nullptr)
		{radix=string2uint<unsigned int>(Range<const char>(radix_pos,args_begin));}
	return std::max(2u,std::min(radix,16u));
	}

static size_t format(const Range<char>& range,int64_t x
	,const Range<const char>& argstring) noexcept
	{
	auto radix=radixGet(argstring);

	char buffer[1 + 8*sizeof(x) + 1];
	auto pos=convert(buffer,x,radix);
	auto n=std::min(static_cast<size_t>(pos-buffer),range.length());
	memcpy(range.begin(),buffer,n);
	return n;
	}

static size_t format(const Range<char>& range,uint64_t x
	,const Range<const char>& argstring) noexcept
	{
	auto radix=radixGet(argstring);

	char buffer[8*sizeof(x) + 1];
	auto pos=convert(buffer,x,radix);
	auto n=std::min(static_cast<size_t>(pos-buffer),range.length());
	memcpy(range.begin(),buffer,n);
	return n;
	}


///Char

static size_t format(const Range<char>& range,char x
	,const Range<const char>& argstring) noexcept
	{
	if(range.length())
		{
		if(argstring.length())
			{
			if(argstring[0]=='n')
				{
				return format(range,static_cast<int64_t>(x)
					,Range<const char>(argstring.begin() + 1,argstring.end()));
				}
			}
		range[0]=x;
		return 1;
		}
	return 0;
	}


///Bool

static size_t format(const Range<char>& range,bool x
	,const Range<const char>& argstring) noexcept
	{
	auto pos=range.begin();
	if(x)
		{
		auto n=std::min(static_cast<size_t>(4),range.length());
		memcpy(pos,"true",n);
		return n;
		}
	else
		{
		auto n=std::min(static_cast<size_t>(5),range.length());
		memcpy(pos,"false",n);
		return n;
		}
	}


///Floating point stuff

static std::pair<char,unsigned int>
floatParams(const Range<const char>& argstring
	,unsigned int digits_default,unsigned int digits_max)  noexcept
	{
	auto n_digits=digits_default;
	auto separator='.';
	enum class State:unsigned int{NORMAL,DIGITS};

	auto args_begin=argstring.begin();
	auto args_end=argstring.end();
	auto state=State::NORMAL;
	const char* digits_pos=nullptr;
	while(args_begin!=args_end)
		{
		auto ch_in=*args_begin;
		switch(state)
			{
			case State::NORMAL:
				state=State::DIGITS;
				if(ch_in!=',' && ch_in!='.')
					{return {separator,n_digits};}
				separator=ch_in;
				digits_pos=args_begin + 1;
				break;
			case State::DIGITS:
				if(!(ch_in>='0' && ch_in<='9'))
					{
					auto n_digits=string2uint<unsigned int>(Range<const char>(digits_pos,args_begin));
					return
						{
						separator,std::max(1u,std::min(n_digits,digits_max))
						};
					}
				break;
			}
		++args_begin;
		}

	if(args_begin==args_end && digits_pos!=nullptr)
		{n_digits=string2uint<unsigned int>(Range<const char>(digits_pos,args_begin));}
	return {separator,std::max(1u,std::min(n_digits,digits_max))};
	}

static size_t format(const Range<char>& range,double x
	,const Range<const char>& argstring) noexcept
	{
	auto params=floatParams(argstring,15,16);
	char buffer[32];
	char format[6];
	sprintf(format,"%%.%ug",params.second);
	sprintf(buffer,format,x);

	auto pos=range.begin();
	auto pos_end=range.end();
	auto ptr_source=buffer;
	while(pos!=pos_end && *ptr_source!='\0')
		{
		auto ch_in=*ptr_source;
		if(ch_in==',' || ch_in=='.')
			{ch_in=params.first;}
		*pos=ch_in;
		++ptr_source;
		++pos;
		}
	return pos - range.begin();
	}


static size_t format(const Range<char>& range,float x
	,const Range<const char>& argstring) noexcept
	{
	auto params=floatParams(argstring,7,8);
	char buffer[32];
	char format[6];
	sprintf(format,"%%.%ug",params.second);
	sprintf(buffer,format,x);

	auto pos=range.begin();
	auto pos_end=range.end();
	auto ptr_source=buffer;
	while(pos!=pos_end && *ptr_source!='\0')
		{
		auto ch_in=*ptr_source;
		if(ch_in==',' || ch_in=='.')
			{ch_in=params.first;}
		*pos=ch_in;
		++ptr_source;
		++pos;
		}
	return pos - range.begin();
	}



static char* process(const Range<char>& range,const Variant& var
	,const Range<const char>& argstring) noexcept
	{
	auto pos=range.begin();
	switch(var.typeGet())
		{
		case Variant::POINTER:
			pos+=format(range,static_cast<const void*>(var),argstring);
			break;
		case Variant::STRING:
			pos+=format(range,static_cast<const char*>(var),argstring);
			break;
		case Variant::INT:
			pos+=format(range,static_cast<int64_t>(var),argstring);
			break;
		case Variant::UINT:
			pos+=format(range,static_cast<uint64_t>(var),argstring);
			break;
		case Variant::CHAR:
			pos+=format(range,static_cast<char>(var),argstring);
			break;
		case Variant::BOOL:
			pos+=format(range,static_cast<bool>(var),argstring);
			break;
		case Variant::DOUBLE:
			pos+=format(range,static_cast<double>(var),argstring);
			break;
		case Variant::FLOAT:
			pos+=format(range,static_cast<float>(var),argstring);
			break;

		default:
			{
		//	User-specified type...
			auto obj=std::pair<const void*,const Variant::Formatter*>(var);
			pos+=obj.second->format(range,obj.first,argstring);
			}
		}
	return pos;
	}

void Glinde::format(const Range<char>& buffer,const char* format_string
	,const Range<const Variant>& args)
	{
	auto write_pos=buffer.begin();
	auto write_end=buffer.end();

	enum class State:unsigned int
		{
		 NORMAL
		,PLACEHOLDER_BEGIN
		,PLACEHOLDER_INDEX
		,PLACEHOLDER_ARGSTRING
		};

	auto state=State::NORMAL;
	const char* numpos=nullptr;
	const char* argpos=nullptr;
	size_t index_current=0;

	while(write_pos!=write_end && *format_string!='\0')
		{
		auto ch_in=*format_string;
		switch(state)
			{
			case State::NORMAL:
				switch(ch_in)
					{
					case '#':
						state=State::PLACEHOLDER_BEGIN;
						break;
					default:
						*write_pos=ch_in;
						++write_pos;
					}
				break;

			case State::PLACEHOLDER_BEGIN:
				if(ch_in>='0' && ch_in<='9')
					{
					numpos=format_string;
					state=State::PLACEHOLDER_INDEX;
					}
				else
					{
					*write_pos=ch_in;
					++write_pos;
					state=State::NORMAL;
					}
				break;

			case State::PLACEHOLDER_INDEX:
				if(!(ch_in>='0'&& ch_in<='9'))
					{
					index_current=string2uint<size_t>(Range<const char>(numpos,format_string));
					if(index_current>=args.length()) //Ignore out-of-bounds index
						{state=State::NORMAL;}
					else
					if(ch_in==';')
						{
						state=State::NORMAL;
						write_pos=process(Range<char>(write_pos,write_end)
							,args[index_current],Range<const char>(format_string,format_string));
						}
					else
						{
						state=State::PLACEHOLDER_ARGSTRING;
						argpos=format_string;
						}
					}
				break;

			case State::PLACEHOLDER_ARGSTRING:
				if(ch_in==';')
					{
					state=State::NORMAL;
					write_pos=process(Range<char>(write_pos,write_end)
						,args[index_current],Range<const char>(argpos,format_string));
					}
				break;
			}
		++format_string;
		}
	if(write_pos!=write_end)
		{
		*write_pos='\0';
		}
	*(write_end - 1)='\0';
	}
