#ifdef __WAND__
target[name[errormessage.o] type[object]]
#endif

#include "errormessage.h"
#include "stringformat.h"

using namespace Glinde;

ErrorMessage::ErrorMessage(const char* format_string
	,const std::initializer_list<Variant>& args) noexcept
	{
	format(m_buffer,format_string,args);
	}
