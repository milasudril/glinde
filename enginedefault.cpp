//@	{
//@	    "dependencies_extra":[],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"enginedefault.o",
//@	            "type":"object"
//@	        }
//@	    ]
//@	}
#include "enginedefault.h"
#include "windowgame.h"
#include "timerdummy.h"
#include "worlddefault.h"
#include "archive.h"
#include "thread.h"
#include "variant.h"
#include "messagequeue.h"
#include "sitedefault.h"

using namespace Glinde;

static const TimerDummy s_timer(30.0);

class EngineDefault::WorldLoader
	{
	public:
		WorldLoader(MessageQueue& messages
			,Message::Processor<WorldDefault*,EngineDefault::WorldOwner>& notifier
			,const char* filename):
			r_messages(messages),r_notifier(notifier),m_filename(filename)
			{}

		void operator()() noexcept;

	private:
		MessageQueue& r_messages;
		Message::Processor<WorldDefault*,EngineDefault::WorldOwner>& r_notifier;
		String m_filename;
	};

void EngineDefault::WorldLoader::operator()() noexcept
	{
	try
		{
		logWrite(Log::MessageType::INFORMATION,"Loading world from \"#0;\""
			,{m_filename.begin()});

		r_messages.post(Message{Message::Time(0),r_notifier
			,new WorldDefault(m_filename.begin(),r_messages)} );
		}
	catch(const ErrorMessage& msg)
		{
		logWrite(Log::MessageType::ERROR,"#0;",{msg.messageGet()});
		r_messages.post(Message{Message::Time(0),r_notifier
			,static_cast<WorldDefault*>(nullptr)} );
		}
	}


void EngineDefault::WorldOwner::operator()(uint64_t time,WorldDefault* world_new)
	{
	if(world_new!=nullptr)
		{
		if(m_world!=nullptr)
			{delete m_world;}
		m_world=world_new;
		logWrite(Log::MessageType::INFORMATION,"World successfully initialized",{});
		}
	}

EngineDefault::WorldOwner::WorldOwner():m_world(nullptr)
	{}

EngineDefault::WorldOwner::~WorldOwner()
	{
	if(m_world!=nullptr)
		{
		delete m_world;
		}
	}




EngineDefault::EngineDefault(MessageQueue& messages):r_messages(messages)
	,m_world_status(EngineDefault::WorldOwner{})
	,m_world_loader(nullptr)
	,m_con(25,80),m_log(*this)
	,m_message_count(0),m_stop(0)
	{
	m_con_index=logWriterAttach(m_log);
	framecounterSet(0).windowSet(nullptr).timerSet(nullptr);
	}

EngineDefault::~EngineDefault()
	{
	cleanup();
	}

void EngineDefault::cleanup()
	{
	if(m_world_loader!=nullptr)
		{delete m_world_loader;}
	logWriterDetach(m_con_index);
	}


EngineDefault& EngineDefault::timerSet(const Timer* timer) noexcept
	{
	r_timer= (timer==nullptr)? &s_timer : timer;
	return *this;
	}



void EngineDefault::run()
	{
	auto dt=r_timer->delayGet();
	Message msg;
	do
		{
		if(msg.timeGet() <= m_frame_current)
			{
			msg.process();
			msg.clear();
			while(r_messages.get(msg))
				{
				if(msg.timeGet() > m_frame_current)
					{break;}

				msg.process();
				msg.clear();
				}
			}

		Window::eventsPoll();
		auto world=m_world_status.callback().world();
		if(world!=nullptr)
			{
			auto site=world->siteGet();
			if(site!=nullptr)
				{
				site->update(m_frame_current,dt,0);
			//	logWrite(Log::MessageType::INFORMATION,"#0;",{dt*m_frame_current});
				}
			}
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
	m_stop=1;
	}

void EngineDefault::consoleWrite(const char* string)
	{
	m_con.writeUTF8(string);
	}

void EngineDefault::consoleWrite(char ch)
	{
	m_con.write(ch);
	}

void EngineDefault::consoleColorMaskSet(uint8_t color_mask)
	{
	m_con.colorMaskSet(color_mask);
	}


EngineDefault& EngineDefault::worldLoadAsync(const char* file)
	{
	if(m_world_loader!=nullptr)
		{delete m_world_loader;}
	m_world_loader=new Thread<WorldLoader>
		{WorldLoader(r_messages,m_world_status,file)};
	return *this;
	}



