#ifdef __WAND__
target[name[errormessage.o] type[object]]
#endif

#include "errormessage.h"
#include <cstdio>
#include <cstdarg>

using namespace Glinie;

ErrorMessage::ErrorMessage(const char* format,...) noexcept
	{
	va_list list;
	va_start(list,format);
	vsnprintf(m_buffer,SIZE_BUFFER-1,format,list);
	va_end(list);

	m_buffer[SIZE_BUFFER-1]=0;
	}
