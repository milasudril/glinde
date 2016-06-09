#ifdef __WAND__
target[name[faceindirect.h] type[include]]
#endif

#ifndef GLINDE_FACEINDIRECT_H
#define GLINDE_FACEINDIRECT_H

#include "arrayfixed.h"

namespace Glinde
	{
	struct FaceIndirect:public ArrayFixed<unsigned int,3>
		{
		using ArrayFixed<unsigned int,3>::ArrayFixed;
		};
	}


#endif
