#ifdef __WAND__
target[name[sdk.o] type[object]]
#endif

#include "sdk.h"
#include "sdkresource.h"
#include "blob.h"

#include "world.h"
GLINDE_BLOB(sdk_world,"world.h");

#include "stringkey.h"
GLINDE_BLOB(sdk_stringkey,"stringkey.h");

using namespace Glinde;

static const SdkResource SDK[]
	{
		 {"world.h",Range<const uint8_t>(sdk_world_begin,sdk_world_end)}
		,{"stringkey.h",Range<const uint8_t>(sdk_stringkey_begin,sdk_stringkey_end)}
	};

Range<const SdkResource> Glinde::sdk() noexcept
	{
	return Range<const SdkResource>(SDK,sizeof(SDK)/sizeof(SdkResource));
	}
