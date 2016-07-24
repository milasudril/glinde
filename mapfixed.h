//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"mapfixed.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_MAPFIXED_H
#define GLINDE_MAPFIXED_H

#include <cstddef>
#include <cassert>

namespace Glinde
	{
	/**\brief An associative container with compile-time keys.
	*/
	template<class Key,class Value, Key ... args>
	struct MapFixed
		{
		public:
			template<Key key>
			Value& get() noexcept
				{
				return m_values[Find<key>::value];
				}

			template<Key key>
			constexpr const Value& get() const noexcept
				{
				return m_values[Find<key>::value];
				}

			static constexpr size_t length() noexcept
				{return sizeof...(args);}

			Value& operator[](const Key& key) noexcept
				{
				auto i=find(key);
				assert(i!=static_cast<size_t>(-1));
				return m_values[i];
				}

			const Value& operator[](const Key& key) const noexcept
				{
				auto i=find(key);
				assert(i!=static_cast<size_t>(-1));
				return m_values[i];
				}

		private:
			static size_t find(const Key& key) noexcept
				{
				auto pos=keys + length();
				auto begin=keys;
				while(pos!=begin)
					{
					--pos;
					if(*pos==key)
						{return pos-begin;}
					}
				return static_cast<size_t>(-1);
				}

			static constexpr Key keys[sizeof...(args)]={Key(args)...};
			Value m_values[sizeof...(args)];

			template<Key key,bool done=0,size_t N=sizeof...(args)>
			struct Find
				{
				static constexpr size_t value=Find<key,keys[N - 1]==key,N - 1>::value;
				};

			template<Key key,size_t N>
			struct Find<key,1,N>
				{
				static constexpr size_t value=N;
				};

			template<Key key>
			struct Find<key,0,0>
				{
			//	value property intentionally ommitted in order to force compile-time
			//	error when key is not found.
				};
		};
		template<class Key,class Value, Key ... args>
		constexpr Key MapFixed<Key,Value,args...>::keys[sizeof...(args)];
	}

#endif
