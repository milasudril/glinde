//@ {
//@	 "targets":[{"name":"session.o","type":"object","pkgconfig_libs":["glfw3"]}]
//@	}

#include "session.hpp"
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cassert>

using namespace GLFWmm;

void Session::error_handler_default(int code,const char* message)
	{
	fprintf(stderr,"%s (%x)\n",message,code);
	abort();
	}

namespace
	{
	class Version
		{
		public:
			explicit Version(int x) noexcept:version(x)
				{}
			int value() const noexcept
				{return version;}

		private:
			int version;
		};

	class VersionIterator
		{
		public:
			using iterator_category=std::random_access_iterator_tag;
			using value_type=Version;
			using difference_type=int;
			using pointer=int;
			using reference=int;

			explicit VersionIterator(int x) noexcept:pos(x)
				{}

			Version operator*() const noexcept
				{return Version(pos);}

			int position() const noexcept
				{return pos;}

			VersionIterator& operator++() noexcept
				{
				++pos;
				return *this;
				}

			VersionIterator& operator+=(int x)
				{
				pos+=x;
				return *this;
				}

		private:
			int pos;
		};

	inline int operator-(VersionIterator a,VersionIterator b) noexcept
		{return a.position()-b.position();}

	inline VersionIterator operator+(VersionIterator a,int b) noexcept
		{return VersionIterator(a.position()+b);}
	}

template<class Callback>
static int unbounded_maximize(Callback&& cb)
	{
	const int lower_bound=1;
	auto upper_bound=lower_bound;

	while(cb(upper_bound))
		{upper_bound<<=1;}
	if(upper_bound==1)
		{return 0;}
	
	return std::lower_bound(VersionIterator(lower_bound)
		,VersionIterator(upper_bound)
		,Version(0),[&cb](Version a,Version)
			{
			return cb(a.value());
			}).position() - 1;
	}

void Session::gl_version_maximize()
	{
	glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,0);
	glfwWindowHint(GLFW_VISIBLE,0);
	auto version_major=unbounded_maximize([](int x)
		{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,x);
		if(x>=4)
			{
			glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
			}
		else
			{
			glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,0);
			}

		auto ret=glfwCreateWindow(100,100,"",NULL,NULL);
		if(ret==nullptr)
			{return 0;}
		
		glfwDestroyWindow(ret);
		return 1;
		});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,version_major);
	if(version_major>=4)
		{
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
		}
	else
		{
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,0);
		}
	auto version_minor=version_major==3?
		unbounded_maximize([](int x)
			{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,x);
			if(x>=2)
				{
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
				}
			else
				{
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,0);
				}

			auto ret=glfwCreateWindow(100,100,"",NULL,NULL);
			if(ret==nullptr)
				{return 0;}
			
			glfwDestroyWindow(ret);
			return 1;
			})
		:unbounded_maximize([](int x)
			{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,x);

			auto ret=glfwCreateWindow(100,100,"",NULL,NULL);
			if(ret==nullptr)
				{return 0;}
			
			glfwDestroyWindow(ret);
			return 1;
			});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,version_minor);
	if(version_major>=4 || (version_major>=3 && version_minor>=2))
		{
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
		}
	m_gl_ver_major_max=version_major;
	m_gl_ver_minor_max=version_minor;
	glfwWindowHint(GLFW_VISIBLE,1);
	}
