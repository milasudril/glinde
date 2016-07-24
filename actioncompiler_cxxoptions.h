//@	{
//@	    "dependencies_extra":[
//@	        {
//@	            "ref":"actioncompiler_cxxoptions.o",
//@	            "rel":"implementation"
//@	        }
//@	    ],
//@	    "targets":[
//@	        {
//@	            "dependencies":[],
//@	            "name":"actioncompiler_cxxoptions.h",
//@	            "type":"include"
//@	        }
//@	    ]
//@	}
#ifndef GLINDE_ACTIONCOMPILER_CXXOPTIONS_H
#define GLINDE_ACTIONCOMPILER_CXXOPTIONS_H

#include "string.h"
#include <algorithm>

namespace Glinde
	{
	class ResourceObject;

	namespace ActionCompiler
		{
		class CxxOptions
			{
			public:
				CxxOptions();

				explicit CxxOptions(const ResourceObject& object);

				CxxOptions& operator|=(const CxxOptions& opts) noexcept
					{
					m_include_dir.append(opts.m_include_dir);
					m_lib_dir.append(opts.m_lib_dir);
					m_options|=opts.m_options;
					m_cxx_minversion=std::max(m_cxx_minversion,opts.m_cxx_minversion);
					return *this;
					}



				float optlevelGet() const noexcept
					{return m_optlevel;}

				CxxOptions& optlevelSet(float optlevel) noexcept
					{
					m_optlevel=std::max(0.0f,std::min(optlevel,1.0f));
					return *this;
					}

				bool debug() const noexcept
					{return m_options&OPTION_DEBUG;}

				CxxOptions& debugEnable() noexcept
					{
					m_options|=OPTION_DEBUG;
					return *this;
					}

				bool fastmath() const noexcept
					{return m_options&OPTION_FASTMATH;}

				CxxOptions& fastmathEnable() noexcept
					{
					m_options|=OPTION_FASTMATH;
					return *this;
					}

				bool native() const noexcept
					{return m_options&OPTION_NATIVE;}

				CxxOptions& nativeEnable() noexcept
					{
					m_options|=OPTION_NATIVE;
					return *this;
					}

				bool m32() const noexcept
					{return m_options&OPTION_M32;}

				CxxOptions& m32Enable() noexcept
					{
					m_options|=OPTION_M32;
					return *this;
					}

				bool pic() const noexcept
					{return m_options&OPTION_PIC;}

				CxxOptions& picEnable() noexcept
					{
					m_options|=OPTION_PIC;
					return *this;
					}



				bool warnAll() const noexcept
					{return m_options&OPTION_WARNALL;}

				CxxOptions& warnAllEnable() noexcept
					{
					m_options|=OPTION_WARNALL;
					return *this;
					}


				bool warnSize() const noexcept
					{return m_options&OPTION_WARNSIZE;}

				CxxOptions& warnSizeEnable() noexcept
					{
					m_options|=OPTION_WARNSIZE;
					return *this;
					}


				bool warnError() const noexcept
					{return m_options&OPTION_WARNERROR;}

				CxxOptions& warnErrorEnable() noexcept
					{
					m_options|=OPTION_WARNERROR;
					return *this;
					}



				bool shared() const noexcept
					{return m_options&OPTION_SHARED;}

				CxxOptions& sharedEnable() noexcept
					{
					m_options|=OPTION_SHARED;
					return *this;
					}


				bool compileOnly() const noexcept
					{return m_options&OPTION_COMPILEONLY;}

				CxxOptions& compileOnlyEnable() noexcept
					{
					m_options|=OPTION_COMPILEONLY;
					return *this;
					}

				CxxOptions& compileOnlyDisable() noexcept
					{
					m_options&=~OPTION_COMPILEONLY;
					return *this;
					}


				bool console() const noexcept
					{return m_options&OPTION_CONSOLE;}

				CxxOptions& consoleEnable() noexcept
					{
					m_options|=OPTION_CONSOLE;
					return *this;
					}


				CxxOptions& includeDirAdd(const char* dir) noexcept
					{
					m_include_dir.append(String(dir));
					return *this;
					}

				Range<const String> includeDirGet() const noexcept
					{return m_include_dir;}


				CxxOptions& libDirAdd(const char* dir) noexcept
					{
					m_include_dir.append(String(dir));
					return *this;
					}

				Range<const String> libDirGet() const noexcept
					{return m_lib_dir;}



				uint64_t cxxMinversion() const noexcept
					{return m_cxx_minversion;}

				uint64_t& cxxMinversion() noexcept
					{return m_cxx_minversion;}



				CxxOptions& dllSupportEnable() noexcept
					{
#if !( _WIN32 || _WIN64 ) && (__x86_64__ || __amd64 || __x86_64)
					picEnable();
#endif
					return *this;
					}

			private:
				float m_optlevel;
				uint32_t m_options;
				uint64_t m_cxx_minversion;
				ArrayDynamic<String> m_include_dir;
				ArrayDynamic<String> m_lib_dir;
				static constexpr uint32_t OPTION_DEBUG=1;
				static constexpr uint32_t OPTION_FASTMATH=2;
				static constexpr uint32_t OPTION_NATIVE=4;
				static constexpr uint32_t OPTION_M32=8;
				static constexpr uint32_t OPTION_PIC=0x10;
				static constexpr uint32_t OPTION_WARNALL=0x20;
				static constexpr uint32_t OPTION_WARNSIZE=0x40;
				static constexpr uint32_t OPTION_WARNERROR=0x80;
				static constexpr uint32_t OPTION_COMPILEONLY=0x20000000;
				static constexpr uint32_t OPTION_SHARED=0x40000000;
				static constexpr uint32_t OPTION_CONSOLE=0x80000000;
			};
		}
	}

#endif
