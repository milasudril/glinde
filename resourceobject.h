#ifdef __WAND__
target[name[resourceobject.h] type[include] ]
dependency[resourceobject.o]
#endif

#ifndef GLINDA_RESOURCEOBJECT_H
#define GLINDA_RESOURCEOBJECT_H

#include <utility>
#include <cstddef>

namespace Glinda
	{
	class DataSource;

	class ResourceObject
		{
		public:
			ResourceObject(const ResourceObject&)=delete;
			ResourceObject& operator=(const ResourceObject&)=delete;

			explicit ResourceObject(DataSource&& reader);
			~ResourceObject();

			ResourceObject(ResourceObject&& tree) noexcept:m_handle(tree.m_handle)
				{tree.m_handle=nullptr;}

			ResourceObject& operator=(ResourceObject&& tree) noexcept
				{
				std::swap(tree.m_handle,m_handle);
				return *this;
				}

			enum class Type:unsigned int{OBJECT,ARRAY,STRING,INTEGER,FLOAT};

			Type typeGet() const noexcept;

			ResourceObject objectGet(const char* name);
			size_t objectCountGet() const noexcept;
			ResourceObject objectGet(size_t index);

			const char* stringGet() const noexcept;
			long long int integerGet() const noexcept;
			double floatGet() const noexcept;

			explicit operator const char*() const;
			explicit operator long long int() const;
			explicit operator double() const;

		private:
			ResourceObject(void* handle);
			void* m_handle;
		};
	};

#endif
