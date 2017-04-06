//@	{"targets":[{"name":"include.hpp","type":"include"}]}

#ifndef GLFWMM_SIZE_HPP
#define GLFWMM_SIZE_HPP

namespace GLFWmm
	{
	struct Size
		{
		Size():width(0),height(0){}

		int width;
		int height;
		float ratio() const noexcept
			{return static_cast<float>(width)/static_cast<float>(height);}
		};

	inline bool operator==(const Size& a,const Size& b) noexcept
		{return a.width==b.width && a.height==b.height;}

	inline bool operator!=(const Size& a,const Size& b) noexcept
		{return !(a==b);}
	}

#endif
