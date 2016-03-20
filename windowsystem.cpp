#ifdef __WAND__
target
	[
	name[windowsystem.o]
	type[object]
	dependency[GL;external]
	dependency[GLEW;external]
	dependency[glfw;external]
	]
#endif

#include "windowsystem.h"
#include "errormessage.h"
#include "debug.h"
#include "logwriter.h"
#include "window.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>



using namespace Glinda;

[[noreturn]] static void glfwErrorRaise(int code,const char* message)
	{
	throw ErrorMessage("The following error occured when trying to initialize "
		"the window system:\n%s",message);
	}

WindowSystem::WindowSystem():r_window(nullptr)
	{
	logWrite(LogMessageType::INFORMATION,"Initializing GLFW version %s"
		,glfwGetVersionString());

	glfwSetErrorCallback(glfwErrorRaise);
	GLINDA_ASSERT_CALL(glfwInit(),==,GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
	}

WindowSystem::~WindowSystem()
	{
	glfwTerminate();
	}

void WindowSystem::inputLoopRun() noexcept
	{
	glfwSwapInterval(1);
	if(r_window!=nullptr)
		{r_window->stickyKeysSet();}

	while(r_window!=nullptr)
		{
		if(r_window->shouldClose())
			{break;}

		glfwPollEvents();
		r_window->redraw();
		r_window->buffersSwap();
		}
	}

