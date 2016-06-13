#ifdef __WAND__
target[name[site.h] type [include]]
#endif

#ifndef GLINDE_SITE_H
#define GLINDE_SITE_H

#include <cstdint>

namespace Glinde
	{
	class WorldObject;
	class Stringkey;

	class Site
		{
		public:
			class EventHandler
				{
				public:
					virtual void onInit(Site& site)=0;
					virtual void onEnter(Site& site,WorldObject& who)=0;
					virtual void onLeave(Site& site,WorldObject& who)=0;
				//	virtual void onCollide(Site& site,WorldObject& a,WorldObject& b)=0;

				};

			virtual ~Site()=default;
			virtual Site& eventHandlerSet(EventHandler& handler)=0;
			virtual Site& init()=0;
			virtual const Stringkey& idGet() const noexcept=0;

			virtual uint32_t itemSpawn(const Stringkey& mapspot,const Stringkey& classname) noexcept=0;

			class SpotVisitor
				{
				public:

				};

			virtual void spotsVisit(SpotVisitor&& visitor)=0;

#if 0
			virtual void exitTo(const Stringkey& sitename)=0;
			virtual void exitTo(const Stringkey& sitename,const Stringkey& mapspot)=0;

			virtual uint32_t itemSpawn(const Stringkey& mapspot,const Stringkey& classname)=0;
			virtual uint32_t itemSpawn(float x,float y,float z,const Stringkey& classname)=0;
#endif
		};
	}

#endif
