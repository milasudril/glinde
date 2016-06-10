#ifdef __WAND__
target[name[sdk.h] type[include]]
dependency[sdk.o]
#endif

#ifndef GLINDE_SDK_H
#define GLINDE_SDK_H

#include "range.h"

namespace Glinde
	{
	class SdkResource;
	Range<const SdkResource> sdk() noexcept;
	}

#endif
