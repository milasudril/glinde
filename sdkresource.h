#ifdef __WAND__
target[name[sdkresource.h] type[include]]
#endif

#ifndef GLINDE_SDKRESOURCE_H
#define GLINDE_SDKRESOURCE_H

#include "range.h"
#include <cstdint>

namespace Glinde
	{
	struct SdkResource
		{
		const char* name;
		Range<const uint8_t> data;
		};
	}

#endif
