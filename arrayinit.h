#ifdef __WAND__
target[name[arrayinit.h] type[include]]
#endif

#ifndef GLINDA_ARRAYINIT_H
#define GLINDA_ARRAYINIT_H

/**\file arrayinit.h Array initialization routines
 */

#include <new>
#include <cstddef>

namespace Glinda
	{
	/** \brief Array initialization routines.
	 *
	 * This namespace contains functions for in-place initialization of array
	 * elements. Possible usecases are
	 *
	 *  * Storing array elements in a location not returned by `new`
	 *  * Creating an array containing objects that cannot be default constructed
	 *
	*/
	namespace ArrayInit
		{
		/** \brief Explicit element destruction.
		 *
		 * This function destroys all elements in the range [`begin`, `end`) in
		 * reverse order by explicitly calling the destructor.
		 *
		 * \note The memory for the array itself is not freed when the function
		 * returns.
		 *
		 */
		template<class T>
		void destroy(T* begin,T* end)
			{
			while(end!=begin)
				{
				--end;
				end->~T();
				}
			}

		/** \brief Element initialization from template.
		 *
		 * This function initializes all elmements in the range* [`begin, `end`)
		 * by copying the template `x` to all elements.
		 *
		 * \note When the array is no longer needed, its elements should be
		 * destroyed by destroy(T*,T*).
		 *
		 * \warning Calling this function on a range that is already initialized
		 * may cause memory leaks.
		 *
		 */
		template<class T,class Template>
		void create(T* begin,T* end,const Template& x)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T(x);
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		/** \brief Element initialization through default constructor.
		 *
		 * This function initializes all elmements in the range [`begin`, `end`)
		 * by calling the default constructor.
		 *
		 * \note When the array is no longer needed, its elements should be
		 * destroyed by destroy(T*,T*).
		 *
		 * \warning Calling this function on a range that is already initialized
		 * may cause memory leaks.
		 *
		 */
		template<class T>
		void create(T* begin,T* end)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T();
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		/** \brief Function type that can be used as Initializer for
		 * create(T*,T*,Initializer&&).
		 *
		 * This function type that can be used as Initializer for
		 * create(T*,T*,Initializer&&).
		 *
		 * \param k the element index
		 * \return the constructed element
		 *
		 */
		template<class T>
		using ElementInitializer = T (*)(size_t k);

		/** \brief Element initialization through initializer functor.
		 *
		 * This function initializes all elmements in the range [`begin`, `end`)
		 * by calling `init`, which has to behave as an \ref ElementInitializer.
		 * Example usage is as follows:
		 *
		 * \code{.cpp}
		 * int values[10];
		 * create(values, values+10, [](size_t k)
		 *     {
		 *     return static_cast<int>(k);
		 *     });
		 * \endcode
		 *
		 * This piece of code will fill `values` with the sequence of integers
		 * from 0 to 10.
		 *
		 * \note When the array is no longer needed, its elements should be
		 * destroyed by destroy(T*,T*).
		 *
		 * \note This function requires that T is move constructable.
		 *
		 * \warning Calling this function on a range that is already initialized
		 * may cause memory leaks.
		 *
		 */
		template<class T,class Initializer>
		void create(T* begin,T* end,Initializer&& init)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T(init(position-begin));
					++position;
					}
				}
			catch(...)
				{
				destroy(position,begin);
				throw;
				}
			}

		/** \brief Element initialization through source array.
		 *
		 * This function initializes all elmements in the range [`begin`, `end`)
		 * by calling converting all elements in `source` into `T`s.
		 *
		 * \note When the array is no longer needed, its elements should be
		 * destroyed by destroy(T*,T*).
		 *
		 * \warning Calling this function on a range that is already initialized
		 * may cause memory leaks.
		 *
		 * \warning The number of elements in the source array must be at least
		 * the same as the number of elements in the target array.
		 *
		 */
		template<class T,class U>
		void copy(T* begin,T* end,const U* source)
			{
			auto position=begin;
			try
				{
				while(position!=end)
					{
					new(position)T(*source);
					++position;
					}
				}
			catch(...)
				{
				destroy(position,end);
				throw;
				}
			}
		}
	}

#endif
