//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"sdk.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "sdk.h"
#include "sdkresource.h"
#include "blob.h"

#include "site.h"
GLINDE_BLOB(sdk_site,"site.h");

#include "world.h"
GLINDE_BLOB(sdk_world,"world.h");

#include "stringkey.h"
GLINDE_BLOB(sdk_stringkey,"stringkey.h");

#include "mapfixed.h"
GLINDE_BLOB(sdk_mapfixed,"mapfixed.h");

#include "log.h"
GLINDE_BLOB(sdk_log,"log.h");

#include "variant.h"
GLINDE_BLOB(sdk_variant,"variant.h");

using namespace Glinde;

static const SdkResource SDK[]
	{
		 {"site.h",Range<const uint8_t>(sdk_site_begin,sdk_site_end)}
		,{"world.h",Range<const uint8_t>(sdk_world_begin,sdk_world_end)}
		,{"mapfixed.h",Range<const uint8_t>(sdk_mapfixed_begin,sdk_mapfixed_end)}
		,{"stringkey.h",Range<const uint8_t>(sdk_stringkey_begin,sdk_stringkey_end)}
		,{"log.h",Range<const uint8_t>(sdk_log_begin,sdk_log_end)}
		,{"variant.h",Range<const uint8_t>(sdk_variant_begin,sdk_variant_end)}
	};

Range<const SdkResource> Glinde::sdk() noexcept
	{
	return Range<const SdkResource>(SDK,sizeof(SDK)/sizeof(SdkResource));
	}
