#ifdef __WAND__
target[name[string.h] type[include]]
dependency[string.o]
#endif

#ifndef GLINDA_STRING_H
#define GLINDA_STRING_H

#include "arraydynamic.h"

namespace Glinda
	{
	class String:private ArrayDynamic<char>
		{
		public:
			using ArrayDynamic<char>::begin;
			using ArrayDynamic<char>::end;
			using ArrayDynamic<char>::length;

			String()
				{}

			explicit String(const char* c_str)
				{append(c_str);}

			const char* beginAfter(char ch) const noexcept;

			char* beginAfter(char ch) noexcept;


			String& append(const char* c_str)
				{
				assert(c_str!=begin());
				return appendImpl(c_str);
				}

			String& append(const String& str)
				{
				ArrayDynamic<char>::append(str);
				return *this;
				}

			String& append(char ch)
				{
				ArrayDynamic<char>::append(ch);
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
				if(begin()!=nullptr)
					{
					memClear();
					ArrayDynamic<char>::clear();
					}
				return *this;
				}

		private:
			String& appendImpl(const char* c_str);
			void memClear();
		};

		bool operator==(const String& a,const char* b) noexcept;
		bool operator==(const String& a,const String& b) noexcept;

		inline bool operator!=(const String& a,const String& b) noexcept
			{return !(a==b);}

		inline bool operator!=(const String& a,const char* b) noexcept
			{return !(a==b);}
	}

#endif
