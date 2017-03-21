//@	[
//@		[
//@		"linux"
//@			,{
//@	 		"targets":
//@				[{"name":"wallclocktime.o","type":"object"}]
//@			 }
//@		]
//@	]

#include "wallclocktime.hpp"
#include <time.h>
#include <type_traits>

using namespace Glinde;

WallclockTime::WallclockTime() noexcept:m_value(time(NULL))
	{}


std::enable_if_t<sizeof(time_t)==sizeof(int64_t),Timestamp>
WallclockTime::timestampUTC() const noexcept
	{
	Timestamp ret;
	tm st;
	gmtime_r(&m_value,&st);
	ret.sec(st.tm_sec)
		.min(st.tm_min)
		.hour(st.tm_hour)
		.day(st.tm_mday)
		.month(st.tm_mon+1)
		.year(st.tm_year+1900);
	return ret;	
	}

std::enable_if_t<sizeof(time_t)==sizeof(int64_t),Timestamp>
WallclockTime::timestampLocal() const noexcept
	{
	Timestamp ret;
	tm st;
	localtime_r(&m_value,&st);
	ret.sec(st.tm_sec)
		.min(st.tm_min)
		.hour(st.tm_hour)
		.day(st.tm_mday)
		.month(st.tm_mon+1)
		.year(st.tm_year+1900);
	return ret;	
	}