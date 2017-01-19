//@	{"targets":[{"name":"test","type":"application","pkgconfig_libs":["glfw3"]}]}

#include "window.hpp"
#include <cstdio>

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

int main()
	{
	glfwInit();
	MyCallback cb;
	GLFWmm::Window<MyCallback> test(0.5f,0.5f,"Hello, World",cb);
	while(!test.shouldClose())
		{
		glfwPollEvents();
		}
	glfwTerminate();
	return 0;
	}