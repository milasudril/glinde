#ifdef __WAND__
target[name[arrayinit.h] type[include]]
#endif

#ifndef GLINDA_ARRAYINIT_H
#define GLINDA_ARRAYINIT_H

namespace Glinda
	{
	namespace ArrayInit
		{
		template<class T,class Template>
		void create(T* begin,T* end,const Template& x)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(*position)T(x);
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		template<class T,class Initializer>
		void create(T* begin,T* end,Initializer&& init)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(*position)T(init(position-begin));
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		template<class T,class U>
		void copy(T* begin,T* end,const U* source)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(*position)T(*source);
					++position;
					}
				}
			catch(...)
				{
				destroy(position,end);
				throw;
				}
			}

		template<class T>
		void destroy(T* begin,T* end)
			{
			while(end!=begin)
				{
				--end;
				end->~T();
				}
			}
		}
	}

#endif
