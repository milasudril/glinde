//@ {"targets":[{"name":"handle.hpp","type":"include"}]}

#ifndef PAGECOMPOSER_HANDLE_HPP
#define PAGECOMPOSER_HANDLE_HPP

namespace PageComposer
	{
	/**Class for holding handles retrieved by libraries that use `typedef struct`
	* things.
	*/
	template<class TypeTag>
	class Handle
		{
		public:
			Handle() noexcept{}

			explicit Handle(void* h) noexcept:m_handle(h)
				{}

			const void* handle() const noexcept
				{return m_handle;}

			void* handle() noexcept
				{return m_handle;}

		private:
			void* m_handle;
		};
	}

#endif