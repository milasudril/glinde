//@	{
//@	"targets":[{"name":"renderlist.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"renderlist.o","rel":"implementation"}]
//@	}

#ifndef GLINDE_RENDERLIST_HPP
#define GLINDE_RENDERLIST_HPP

#include "arraydynamic.hpp"

namespace Glinde
	{
	class Display;
	class RenderObject;

	class RenderList
		{
		public:
			RenderList();
			void framebufferResize(int width,int height);
			void render(Display& disp) const noexcept;

			uint32_t append(RenderObject& obj,unsigned int z_index)
				{
				auto id=r_objects.length(); //Assume we never removes objects
				auto obj_index=r_objects.length(); //The new object will be stored last in the array
				m_id_to_obj_index.append(obj_index); //Append object index to id->object lookup table
				r_objects.append(RenderTask{0,z_index,id,&obj}); //Append the object using the new id
				m_dirty=1;
				return id;
				}

			RenderList& activate(uint32_t id) noexcept
				{
				assert(id<m_id_to_obj_index.length());
				r_objects[m_id_to_obj_index[id]].active=1;
				return *this;
				}

			RenderList& deactivate(uint32_t id) noexcept
				{
				assert(id<m_id_to_obj_index.length());
				r_objects[m_id_to_obj_index[id]].active=0;
				return *this;
				}
			

		private:
			struct RenderTask
				{
				bool active;
				unsigned int index;
				uint32_t id;
				RenderObject* r_obj;
				};
			
			mutable ArrayDynamic<RenderTask> r_objects;
			mutable bool m_dirty;
			mutable ArrayDynamic<uint32_t> m_id_to_obj_index;
		};
	}

#endif
