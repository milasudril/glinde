//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"string.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"string.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_STRING_H
#define GLINDE_STRING_H

#include "arraydynamic.h"

namespace Glinde
	{
	class String:private ArrayDynamic<char>
		{
		public:
			using ArrayDynamic<char>::begin;
			using ArrayDynamic<char>::end;

			const char* end() const noexcept
				{return ArrayDynamic<char>::end()-1;}

			char* end() noexcept
				{return ArrayDynamic<char>::end()-1;}

			unsigned int length() const noexcept
				{return ArrayDynamic<char>::length()-1;}


			String()
				{ArrayDynamic<char>::append('\0');}

			explicit String(const char* c_str)
				{
				ArrayDynamic<char>::append('\0');
				append(c_str);
				}

			const char* beginAfter(char ch) const noexcept;

			char* beginAfter(char ch) noexcept;


			String& append(const char* c_str)
				{
				assert(c_str!=begin());
				return appendImpl(c_str);
				}

			String& append(const String& str)
				{
				ArrayDynamic<char>::truncate().append(str);
				return *this;
				}

			String& append(char ch)
				{
				ArrayDynamic<char>::truncate().append(ch).append('\0');
				return *this;
				}

			String& truncate() noexcept
				{
				ArrayDynamic<char>::truncate();
				(*end())='\0';
				return *this;
				}

			String& truncate(char ch) noexcept;

			String& clear() noexcept
				{
				ArrayDynamic<char>::clear();
				ArrayDynamic<char>::append('\0');
				return *this;
				}

		private:
			String& appendImpl(const char* c_str);
		};

		bool operator==(const String& a,const char* b) noexcept;
		bool operator==(const String& a,const String& b) noexcept;

		inline bool operator!=(const String& a,const String& b) noexcept
			{return !(a==b);}

		inline bool operator!=(const String& a,const char* b) noexcept
			{return !(a==b);}
	}

#endif
