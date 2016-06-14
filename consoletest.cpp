#ifdef __WAND__
target[name[consoletest] type[application] ]
#endif

#include "errormessage.h"
#include "logwriter.h"
#include "enginedefault.h"
#include "consolerenderer.h"
#include "rendereroverlay.h"
#include "variant.h"

#include "window.h"
#include "timerreal.h"

#include "glprogram.h"
#include "glvertexarray.h"
#include "glvertexbuffer.h"
#include "glelementarraybuffer.h"
#include "gltexture.h"
#include "glframebuffer.h"


using namespace Glinde;

class EventHandlerTest:public Window::EventHandler
	{
	public:
		EventHandlerTest(Engine& engine):r_engine(engine){}

		virtual void onKeyDown(Window& source,uint8_t scancode){}
		virtual void onKeyUp(Window& source,uint8_t scancode){}
		virtual void onTextInput(Window& source,uint32_t codepoint){}

		virtual void onMouseMove(Window& source,double x,double y){}
		virtual void onMouseDown(Window& source,int button){}
		virtual void onMouseUp(Window& source,int button){}

		virtual void onResize(Window& source,int width,int height){}

		virtual void onClose(Window& source)
			{r_engine.stop();}

	private:
		Engine& r_engine;
	};


class WindowTest:public Window
	{
	public:
		WindowTest(const char* title,int width,int height
			,EventHandler& handler,const Console& console):Window(title,width,height,handler)
			,r_console(&console),m_con_renderer(console),w(width),h(height)
			{
			}

		~WindowTest()
			{
			}

		void redraw() noexcept
			{
			m_con_renderer.render();
			m_renderer.viewportSizeSet(w,h);
			m_renderer.render(m_con_renderer.textureGet());
			}

		void resized(int width,int height)  noexcept
			{
			w=width;
			h=height;
			}

	private:
		const Console* r_console;

		ConsoleRenderer m_con_renderer;
		RendererOverlay m_renderer;
		int w;
		int h;
	};


int main()
	{
	deathtrapHandlerActivate();
	logWrite(Log::MessageType::INFORMATION,"Glinde is starting up",{});
	try
		{
		EngineDefault e;
		logWrite(Log::MessageType::INFORMATION,"Engine initialized",{});
		EventHandlerTest test(e);
		WindowTest mainwin("Console",800,600,test,e.consoleGet());
		TimerReal world_clock(30);
		logWrite(Log::MessageType::INFORMATION
			,"World clock ticks every #0; second",{world_clock.delayGet()});
		e.windowSet(&mainwin).timerSet(&world_clock);
		e.worldLoadAsync("test.zip");
		e.run();
		}
	catch(const ErrorMessage& message)
		{
		logWrite(Log::MessageType::ERROR,"#0;",{message.messageGet()});
		return -1;
		}
	return 0;
	};
