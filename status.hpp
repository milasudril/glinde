//@	{"targets":[{"name":"status.hpp","type":"include"}]}

#ifndef GLINDE_STATUS_HPP
#define GLINDE_STATUS_HPP

namespace Glinde
	{
	enum class Status:unsigned int{WAITING,READY,READY_AFTER_KEY};
	}

#endif