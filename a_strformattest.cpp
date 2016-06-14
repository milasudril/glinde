#ifdef __WAND__
target[name[a_strformattest] type[application]]
#endif

#include "stringformat.h"
#include "range.h"
#include <cstdio>
#include <cmath>

int main()
	{
	char buffer[4096];

	Glinde::format(Glinde::Range<char>(buffer,4096)
		,"#0;, #1;, #2b;, #2o;, #2d;, #2;, #2:12;, #2h;, #3;, #3h;, #4;, #4nh;, "
		 "#5;\n#6,16;\n#6,15; #7;\n"
		,{
		 &buffer
		,"A string",int64_t(-144),uint64_t(235),'B',true,acos(-1.0),2.72f
		});

	printf("%s",buffer);
	}
