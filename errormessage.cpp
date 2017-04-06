//@	{"targets":[{"name":"errormessage.o","type":"object"}]}

#include "errormessage.hpp"
#include "stringformat.hpp"

using namespace Glinde;

ErrorMessage::ErrorMessage(const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	format(m_buffer,format_string,args);
	}
