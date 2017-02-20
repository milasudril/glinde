//@	{"targets":[{"name":"membercheck.hpp","type":"include"}]}

#ifndef GLFWMM_MEMBERCHECK_HPP
#define GLFWMM_MEMBERCHECK_HPP

#ifndef GLFWMM_NO_MEMBER_ADVERTISMENT
#define ADVERTISE_MEMBER(membername) [[deprecated("Hint: add " #membername " to current class.")]]
#else
#define ADVERTISE_MEMBER(membername) 
#endif

namespace GLFWmm
	{
	struct has_not_member{};
	struct has_member:public has_not_member{};
	template<class X> struct member_test
		{typedef int type;};
	}

#endif
