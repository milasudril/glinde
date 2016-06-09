#ifdef __WAND__
target[name[enginedefault.o] type[object]]
#endif

#include "enginedefault.h"
#include "window.h"
#include "timerdummy.h"
#include "message.h"
#include "world.h"
#include "archive.h"
#include "thread.h"

using namespace Glinde;

static const TimerDummy s_timer(30.0);

class EngineDefault::WorldLoader:public Thread::Runner
	{
	public:
		WorldLoader(EngineDefault& engine,const char* filename):
			r_engine(engine),m_filename(filename)
			{}

		void run() noexcept;

	private:
		EngineDefault& r_engine;
		String m_filename;
	};

void EngineDefault::WorldLoader::run() noexcept
	{
	World* world=nullptr;
	try
		{
		logWrite(LogMessageType::INFORMATION,"Loading world from \"%s\""
			,m_filename.begin());

		world=new World(m_filename.begin());
		}
	catch(const ErrorMessage& msg)
		{
		logWrite(LogMessageType::ERROR,"%s",msg.messageGet());
		}

	r_engine.worldLoadedPost(world);
	}

EngineDefault::EngineDefault():m_world(nullptr),m_world_loader_task(nullptr)
	,m_con(25,80),m_log(*this)
	,m_message_count(0),m_stop(0)
	{
	logWriterAttach(&m_log);
	framecounterSet(0).windowSet(nullptr).timerSet(nullptr);
	}

EngineDefault::~EngineDefault()
	{
	logWriterAttach(nullptr);
	if(m_world_loader_task!=nullptr)
		{
		delete m_world_loader_task;
		delete m_world_loader;
		}
	if(m_world!=nullptr)
		{delete m_world;}
	}

EngineDefault& EngineDefault::timerSet(const Timer* timer) noexcept
	{
	r_timer= (timer==nullptr)? &s_timer : timer;
	return *this;
	}


enum MessageID:uint32_t
	{
	 INVALID
	,STOP
	,CONSOLE_WRITE_STRING
	,CONSOLE_WRITE_CHAR
	,CONSOLE_COLORMASK_SET
	,WORLD_LOAD
	,WORLD_LOADED
	};

void EngineDefault::run()
	{
	auto dt=r_timer->delayGet();
	Message msg;
	do
		{
		if(msg.timeGet() <= m_frame_current)
			{
			messageProcess(msg);
			msg.clear();
			while(m_messages.get(msg))
				{
				if(msg.timeGet() > m_frame_current)
					{break;}

				messageProcess(msg);
				msg.clear();
				}
			}

		Window::eventsPoll();
		if(r_window!=nullptr)
			{
			r_window->redraw();
			r_window->buffersSwap();
			}
		r_timer->wait();
		++m_frame_current;
		}
	while(!m_stop);
	m_stop=0;
	}

void EngineDefault::stop()
	{
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::STOP
		,m_frame_current
		});
	}

void EngineDefault::consoleWrite(const char* string)
	{
	auto l=strlen(string) + 1;
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::CONSOLE_WRITE_STRING
		,m_frame_current
		,Range<const char>(string,l)
		});
	}

void EngineDefault::consoleWrite(char ch)
	{
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::CONSOLE_WRITE_CHAR
		,m_frame_current
		,ch
		});
	}

void EngineDefault::consoleColorMaskSet(uint8_t color_mask)
	{
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::CONSOLE_COLORMASK_SET
		,m_frame_current
		,color_mask
		});
	}


EngineDefault& EngineDefault::worldLoadAsync(const char* file)
	{
	auto l=strlen(file) + 1;
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::WORLD_LOAD
		,m_frame_current
		,Range<const char>(file,l)
		});
	return *this;
	}

void EngineDefault::worldLoadedPost(World* world)
	{
	m_messages.post(Message
		{
		 __sync_fetch_and_add(&m_message_count,1)
		,MessageID::WORLD_LOADED
		,m_frame_current
		,world
		});
	}


void EngineDefault::messageProcess(const Message& msg)
	{
	switch(msg.typeGet())
		{
		case MessageID::STOP:
			m_stop=1;
			break;

		case MessageID::CONSOLE_WRITE_STRING:
			m_con.writeUTF8(msg.dataGetRange<char>().begin());
			break;

		case MessageID::CONSOLE_WRITE_CHAR:
			m_con.write(msg.dataGetValue<char>());
			break;

		case MessageID::CONSOLE_COLORMASK_SET:
			m_con.colorMaskSet(msg.dataGetValue<uint8_t>());
			break;

		case MessageID::WORLD_LOAD:
			if(m_world_loader_task!=nullptr)
				{delete m_world_loader_task;}
			m_world_loader=new WorldLoader(*this,msg.dataGetRange<char>().begin());
			m_world_loader_task=new Thread(*m_world_loader);
			break;

		case MessageID::WORLD_LOADED:
			{
			if(m_world_loader_task!=nullptr)
				{
				delete m_world_loader_task;
				delete m_world_loader;
				m_world_loader_task=nullptr;
				}
			auto world_new=msg.dataGetPointer<World>();
			if(world_new!=nullptr)
				{
				if(m_world!=nullptr)
					{delete m_world;}
				m_world=world_new;
				logWrite(LogMessageType::INFORMATION,"World successfully loaded");
				}
			}
			break;
		}
	}


