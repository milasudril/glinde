//@	{
//@	 "targets":[{"name":"eventhandler.h","type":"include"}]
//@ ,"dependencies_extra":[{"ref":"eventhandler.o","rel":"implementation"}]
//@	,"author":["Torbjörn Rathsman"]
//@	,"year":2016
//@	,"license":"Public domain"
//@	,"description":"Definition of event handler for map00"
//@	}

#ifndef MAP00_EVENTHANDLER_H
#define MAP00_EVENTHANDLER_H

#include <glinde/site.h>

namespace Map00
	{
	class EventHandler:public Glinde::Site::EventHandler
		{
		public:
			void onInit(Glinde::Site& site);

			void onEnter(Glinde::Site& site,Glinde::WorldObject& who)
				{}

			void onLeave(Glinde::Site& site,Glinde::WorldObject& who)
				{}
		};
	}

#endif
