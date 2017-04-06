//@	{"targets":[{"name":"test","type":"application","pkgconfig_libs":["glew"]}]}

#define GLFWMM_NO_MEMBER_ADVERTISMENT

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

	void mouseButton(GLFWmm::WindowBase& src,int button,GLFWmm::WindowBase::Action action
		,unsigned int modifiers)
		{
		printf("Click: %d %d %x\n",button,static_cast<int>(action),modifiers);
		}
	
	void cursorEnter(GLFWmm::WindowBase& src,GLFWmm::WindowBase::CursorDirection dir)
		{
		printf("Cursor enter: %d\n",static_cast<int>(dir));
		}

	void cursorPos(GLFWmm::WindowBase& src,double x,double y)
		{
		printf("Cursor position: %.7g,%.7g\n",x,y);
		}

	void drop(GLFWmm::WindowBase& src,int n,const char* const* files)
		{
		printf("Drop:\n");
		while(n!=0)
			{
			printf("   %s\n",*files);
			++files;
			--n;
			}
		}

	void scroll(GLFWmm::WindowBase& src,double x,double y)
		{
		printf("Scroll: %.7g,%.7g\n",x,y);
		}

	void key(GLFWmm::WindowBase& src,int scancode,GLFWmm::WindowBase::Action action
		,unsigned int modifiers)
		{
		printf("Key: %d %d %x\n",scancode,static_cast<int>(action),modifiers);
		}

	void codepoint(GLFWmm::WindowBase& src,uint32_t cp)
		{
		printf("Codepoint: %x\n",cp);
		}
	};

struct Hints
	{
	static constexpr bool visible=1;
	static constexpr bool srgb=1;

	static constexpr bool resizable=1;
	static constexpr bool decorated=1;

#ifdef SPECIFIC_REQUIREMENTS
	static constexpr auto clientAPI=GLFWmm::Session::ClientAPI::OPENGL;
	static constexpr auto clientAPIProfile=GLFWmm::Session::ClientAPIProfile::CORE;
	static constexpr int clientAPIVersionMajor=4;
	static constexpr int clientAPIVersionMinor=5;
	static constexpr bool clientAPIForwardCompat=1;
#endif
	};

int main()
	{
	GLFWmm::Session session;
	session.creationHints(Hints{});
	MyCallback cb;
	GLFWmm::Window<MyCallback> test(0.5f,0.5f,"Hello, World",cb,session);
	test.contextCapture();
	printf("Version: %s\n",glGetString(GL_VERSION));
	while(!test.shouldClose())
		{
		session.eventsPoll();
		test.buffersSwap();
		}
	return 0;
	}