#ifdef __WAND__
target
	[
	name[window.o]
	type[object]
	dependency[GL;external]
	dependency[GLEW;external]
	dependency[glfw;external]
	]
#endif

#include "window.h"
#include "errormessage.h"
#include "logwriter.h"
#include "debug.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>


using namespace Glinde;

class Window::EHWrapper
	{
	friend class Window;
	static void on_resize(GLFWwindow* window,int width,int height)
		{
		auto self=reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		self->eventHandlerGet().onResize(*self,width,height);
		self->resized(width,height);
		}

	static void key_handler(GLFWwindow* window,int key,int scancode,int action
		,int mods)
		{
		auto self=reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		switch(action)
			{
			case GLFW_REPEAT:
			case GLFW_PRESS:;
				self->eventHandlerGet().onKeyDown(*self,scancode);
				break;

			case GLFW_RELEASE:
				self->eventHandlerGet().onKeyUp(*self,scancode);
				break;

			default:
				GLINDE_DEBUG_PRINT("Unknown keyboard action %d",action);
			}
		}

	static void mouse_handler(GLFWwindow* window,int button,int action,int mods)
		{
		auto self=reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		switch(action)
			{
			case GLFW_PRESS:
				self->eventHandlerGet().onMouseDown(*self,button);
				break;

			case GLFW_RELEASE:
				self->eventHandlerGet().onMouseUp(*self,button);
				break;

			default:
				GLINDE_DEBUG_PRINT("Unknown mouse action %d",action);
			}
		}

	static void on_mouse_move(GLFWwindow* window,double x,double y)
		{
		auto self=reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		self->eventHandlerGet().onMouseMove(*self,x,y);
		}

	static void on_close(GLFWwindow* window)
		{
		auto self=reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		self->eventHandlerGet().onClose(*self);
		}
	};

static size_t instance_count=0;

[[noreturn]] static void glfwErrorRaise(int code,const char* message)
	{
	throw ErrorMessage("The following error occured when trying to initialize "
		"the window system:\n%s",message);
	}

static void init()
	{
	logWrite(Log::MessageType::INFORMATION,"Initializing GLFW version %s"
		,glfwGetVersionString());

	glfwSetErrorCallback(glfwErrorRaise);
	GLINDE_ASSERT_CALL(glfwInit(),==,GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
	}

static void deinit()
	{
	logWrite(Log::MessageType::INFORMATION,"Terminating GLFW");
	glfwTerminate();
	}

void Window::eventsPoll() noexcept
	{
	glfwPollEvents();
	}


Window::Window(const char* title,unsigned int width,unsigned int height
	,EventHandler& handler)
	{
	if(instance_count==0)
		{init();}

	logWrite(Log::MessageType::INFORMATION,"Creating an OpenGL window");
//	HACK: Create a window that is one unit wider, so we are able to force a
//	resize message to be sent (See (1) below).
	auto window=glfwCreateWindow(width + 1,height,title,NULL,NULL);
	assert(window!=NULL);
	glfwMakeContextCurrent(window);
	glewExperimental=1;
	logWrite(Log::MessageType::INFORMATION,"Initializing GLEW %s"
		,glewGetString(GLEW_VERSION));
	auto glew_result=glewInit();
	if(glew_result != GLEW_OK)
		{
		throw ErrorMessage("Faild to initialize GLEW. %s"
			,glewGetErrorString(glew_result));
		}

	logWrite(Log::MessageType::INFORMATION,"Got an OpenGL context with the "
		"following characteristics:\n\n"
		"  Vendor: %s\n"
		"  Renderer: %s\n"
		"  Version: %s\n"
		"  GLSL version: %s\n"
		,glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_VERSION)
		,glGetString(GL_SHADING_LANGUAGE_VERSION));

	r_handler=&handler;
	m_handle=window;
	glfwSetWindowUserPointer(window,this);
	glfwSetWindowSizeCallback(window,EHWrapper::on_resize);
	glfwSetKeyCallback(window,EHWrapper::key_handler);
	glfwSetMouseButtonCallback(window,EHWrapper::mouse_handler);
	glfwSetCursorPosCallback(window,EHWrapper::on_mouse_move);
	glfwSetWindowCloseCallback(window, EHWrapper::on_close);

//	HACK(1): Trigg window size event so we can set our viewport
	glfwSetWindowSize(window,width,height);

	++instance_count;
	}

Window::~Window()
	{
	--instance_count;
	glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
	if(instance_count==0)
		{deinit();}
	}

void Window::buffersSwap() noexcept
	{
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
	}

bool Window::shouldClose() noexcept
	{
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
	}

void Window::stickyKeysSet() noexcept
	{
	glfwSetInputMode(static_cast<GLFWwindow*>(m_handle),GLFW_STICKY_KEYS,1);
	}

void Window::sizeGet(int& width,int& height) noexcept
	{
	glfwGetWindowSize(static_cast<GLFWwindow*>(m_handle),&width,&height);
	}

void Window::cursorPositionSet(double x, double y) noexcept
	{
	glfwSetCursorPos(static_cast<GLFWwindow*>(m_handle),x,y);
	}

void Window::cursorHide() noexcept
	{
	glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

void Window::cursorShow() noexcept
	{
	glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

void Window::cursorHideAndGrab() noexcept
	{
	glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
