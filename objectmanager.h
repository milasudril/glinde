//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"objectmanager.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLIDNE_OBJECTMANAGER_H
#define GLINDE_OBJECTMANAGER_H

#include "arraydynamic.h"
#include "worldobject.h"
#include <utility>

namespace Glinde
	{
	class ObjectManager
		{
		public:
			class value_type
				{
				public:
					value_type()=default;
					value_type(const value_type&)=default;
					value_type(value_type&&)=default;

					value_type(WorldObject&& object,uint32_t id):
						m_object(std::move(object)),m_id(id)
						{}

					WorldObject& object() noexcept
						{return m_object;}

					const WorldObject& object() const noexcept
						{return m_object;}

					uint32_t id() const noexcept
						{return m_id;}
				private:
					WorldObject m_object;
					uint32_t m_id;
					friend void swap(value_type&,value_type&);
					value_type& operator=(const value_type&)=default;
					value_type& operator=(value_type&&)=default;
				};

			ObjectManager():m_id_next(0)
				{
				}

			ObjectManager& insert(WorldObject&& obj)
				{
				auto id=idGet();
				assert(id<=m_idmap.length());
				if(id==m_idmap.length())
					{m_idmap.append(m_objects.length());}
				else
					{m_idmap[id]=m_objects.length();}

				m_objects.append({ std::move(obj), id}  );
				return *this;
				}

			ObjectManager& remove(uint32_t id) noexcept
				{
				if(id>=m_idmap.length())
					{return *this;}
				auto pos=m_idmap[id];
				if(pos==static_cast<uint32_t>(-1))
					{return *this;}

				swap(m_objects[pos],*(m_objects.end()-1));
				m_idmap[ m_objects[pos].id() ]=pos;
				m_ids.append(id);
				m_idmap[id]=static_cast<uint32_t>(-1);
				return *this;
				}

			WorldObject* objectGet(uint32_t id) noexcept
				{
				if(id>=m_idmap.length())
					{return nullptr;}
				auto pos=m_idmap[id];
				if(pos==static_cast<uint32_t>(-1))
					{return nullptr;}

				return &m_objects[pos].object();
				}

			const value_type* begin() const noexcept
				{return m_objects.begin();}

			const value_type* end() const noexcept
				{return m_objects.end();}

			value_type* begin() noexcept
				{return m_objects.begin();}

			value_type* end() noexcept
				{return m_objects.end();}

			Range<const value_type> objectsGet() const noexcept
				{return m_objects;}

			Range<value_type> objectsGet() noexcept
				{return m_objects;}


			uint32_t objectsCount() const noexcept
				{return m_objects.length();}

		private:
			ArrayDynamic<value_type> m_objects;
			ArrayDynamic<uint32_t> m_ids;
			ArrayDynamic<uint32_t> m_idmap;
			uint32_t m_id_next;

			uint32_t idGet() noexcept
				{
				if(m_ids.length()==0)
					{
					auto ret=m_id_next;
					++m_id_next;
					return ret;
					}
				auto ret=*(m_ids.end() - 1);
				m_ids.truncate();
				return ret;
				}
		};

	inline void swap(ObjectManager::value_type& a,ObjectManager::value_type& b)
		{
		ObjectManager::value_type temp(a);
		a=std::move(b);
		b=std::move(temp);
		}
	}

#endif
