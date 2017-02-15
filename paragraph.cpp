//@	{
//@	 "targets":
//@		[
//@			{"name":"paragraph.o","type":"object","pkgconfig_libs":["pangocairo"] }
//@		]
//@	}

#include "paragraph.hpp"
#include <pango/pangocairo.h>

using namespace PageComposer;

inline static const PangoFontDescription* font(const Handle<Paragraph::Font>& f)
	{return reinterpret_cast<const PangoFontDescription*>( f.handle() );}

inline static PangoFontDescription* font(Handle<Paragraph::Font>& f)
	{return reinterpret_cast<PangoFontDescription*>( f.handle() );}


inline static const PangoLayout* layout(const Handle<Paragraph::Layout>& f)
	{return reinterpret_cast<const PangoLayout*>( f.handle() );}

inline static PangoLayout* layout(Handle<Paragraph::Layout>& f)
	{return reinterpret_cast<PangoLayout*>(f.handle());}
