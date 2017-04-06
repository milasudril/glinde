//@	{"targets":[{"name":"blob.hpp","type":"include"}]}

#ifndef GLINDE_BLOB_HPP
#define GLINDE_BLOB_HPP

#include <cstdint>
extern "C"
{
#if __gnu_linux__

#define GLINDE_BLOB(identifier,filename) \
asm(".pushsection .data\n" \
	"\t.local " #identifier "_begin\n" \
	"\t.type " #identifier "_begin, @object\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.local " #identifier "_end\n" \
	"\t.type " #identifier "_end, @object\n" \
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.popsection\n"); \
\
extern const uint8_t identifier##_begin[];\
extern const uint8_t identifier##_end[]

#elif _WIN32

#define GLINDE_BLOB(identifier,filename) \
asm(".data\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.text\n"); \
\
extern const uint8_t identifier##_begin[];\
extern const uint8_t identifier##_end[]

#else
	#error "Cannot include binary files"
#endif
}
#endif
