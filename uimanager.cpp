#ifdef __WAND__
target
	[
	name[uimanager.o]
	type[object]
	dependency[GL;external]
	dependency[GLEW;external]
	dependency[glfw;external]
	]
#endif

#include "uimanager.h"
#include "errormessage.h"
#include "debug.h"
#include "logwriter.h"
#include "window.h"
#include "timer.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

using namespace Glinde;

[[noreturn]] static void glfwErrorRaise(int code,const char* message)
	{
	throw ErrorMessage("The following error occured when trying to initialize "
		"the window system:\n%s",message);
	}

UIManager::UIManager()
	{
	logWrite(LogMessageType::INFORMATION,"Initializing GLFW version %s"
		,glfwGetVersionString());

	glfwSetErrorCallback(glfwErrorRaise);
	GLINDE_ASSERT_CALL(glfwInit(),==,GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
	}

UIManager::~UIManager()
	{
	glfwTerminate();
	}

void UIManager::eventsPoll()
	{
	glfwPollEvents();
	}
