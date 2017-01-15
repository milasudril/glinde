//@	{"targets":[{"name":"contextguard.hpp","type":"include"}]}

#ifndef ANGLE_CONTEXTGUARD_HPP
#define ANGLE_CONTEXTGUARD_HPP

namespace Angle
	{
	template<class ContextUser>
	class ContextGuard
		{
		public:
			ContextGuard(const ContextGuard&)=delete;
			ContextGuard& operator=(const ContextGuard&)=delete;

			explicit ContextGuard(ContextUser& user):r_user(user)
				{r_context=user.contextCapture();}

			~ContextGuard()
				{r_user.contextRelease(r_context);}

		private:
			ContextUser& r_user;
			typename ContextUser::ContextHandle r_context;
		};
	}

#endif
