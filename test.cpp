//@	{"targets":[{"name":"test","type":"application"}]}

#include "window.hpp"
#include "session.hpp"
#include <cstdio>
#include <cstdlib>

struct MyCallback
	{
	void positionChanged(GLFWmm::WindowBase& src,int x,int y)
		{
		printf("Position changed: %d %d\n",x,y);
		}

	void sizeChanged(GLFWmm::WindowBase& src,int w,int h)
		{
		printf("Size changed: %d %d\n",w,h);
		}

	void closing(GLFWmm::WindowBase& src)
		{
		printf("Closing\n");
		}

	void refresh(GLFWmm::WindowBase& src)
		{
		printf("Refresh\n");
		}

	void focusChanged(GLFWmm::WindowBase& src,bool status)
		{
		printf("Focus changed: %d\n",status);
		}

	void iconified(GLFWmm::WindowBase& src,bool status)
		{
		printf("Iconified: %d\n",status);
		}

	void framebufferSizeChanged(GLFWmm::WindowBase& src,int w,int h)
		{
		printf("Fb Size changed: %d %d\n",w,h);
		}
	};

struct Hints
	{
	static constexpr bool visible=1;
	static constexpr bool srgb=1;
	static constexpr auto clientAPI=GLFWmm::WindowBase::ClientAPI::OPENGL;
	static constexpr auto clientAPIProfile=GLFWmm::WindowBase::ClientAPIProfile::CORE;
	static constexpr int clientAPIVersionMajor=4;
	static constexpr int clientAPIVersionMinor=5;
	static constexpr bool clientAPIForwardCompat=1;
	};

int main()
	{
	GLFWmm::Session session;
	MyCallback cb;
	GLFWmm::WindowBase::creationHints(Hints{},session);

	GLFWmm::Window<MyCallback> test(0.5f,0.5f,"Hello, World",cb,session);
	while(!test.shouldClose())
		{
		session.eventsPoll();
		}
	return 0;
	}