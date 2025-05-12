#pragma once

#include <Defines.hpp>
#include <IO/Logger.hpp>
#if defined(SYLVER_PLATFORM_UNIX)
#include <dlfcn.h>
#elif defined(SYLVER_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Sylver{
    template<typename T>
    class LibLoader{
        private:
#if defined(SYLVER_PLATFORM_UNIX)
            void* m_Handle{nullptr};
#elif defined(SYLVER_PLATFORM_WINDOWS)
            HMODULE m_Handle{};
#endif
            std::filesystem::path m_LibPath{};
            inline static std::string s_AllocSymbol{"allocate"};
            inline static std::string s_FreeSymbol{"free"};
        public:
            LibLoader(const std::filesystem::path& libPath): m_LibPath(libPath){}
            ~LibLoader(){}

            void Open(){
#if defined(SYLVER_PLATFORM_UNIX)
                if (!(m_Handle = dlopen(m_LibPath.c_str(), RTLD_NOW | RTLD_LAZY))) {
				    Logger::Error("Failed to load shared object with error: {}", dlerror());
			    }
#elif defined(SYLVER_PLATFORM_WINDOWS)
                if (!(m_Handle = LoadLibrary(m_LibPath.c_str()))) {
				    Logger::Error("Failed to load shared object: {}", m_LibPath.string());
			    }
#endif
            }

            std::shared_ptr<T> GetInstance(){
                using allocClass = T *(*)();
                using deleteClass = void (*)(T *);

	            auto allocFunc = reinterpret_cast<T>(
#if defined(SYLVER_PLATFORM_UNIX)
			        dlsym(m_Handle, s_AllocSymbol.c_str())
#elif defined(SYLVER_PLATFORM_WINDOWS)
			        GetProcAddress(m_Handle, s_AllocSymbol.c_str())
#endif
                );
	            auto deleteFunc = reinterpret_cast<T>(
#if defined(SYLVER_PLATFORM_UNIX)
			        dlsym(m_Handle, s_FreeSymbol.c_str())
#elif defined(SYLVER_PLATFORM_WINDOWS)
			        GetProcAddress(m_Handle, s_FreeSymbol.c_str())
#endif
                );

	            if (!allocFunc || !deleteFunc) {
		            Close();
                    Logger::Error("Failed to get instance of shared object: {}", m_LibPath.string());
	            }

	            return std::shared_ptr(
			        allocFunc(),
			        [deleteFunc](T *p){ deleteFunc(p); }
                );
            }

            void Close(){
#if defined(SYLVER_PLATFORM_UNIX)
                if(dlclose(m_Handle) != 0){
				    Logger::Error("Failed to close shared object with error: {}", dlerror());
                }
#elif defined(SYLVER_PLATFORM_WINDOWS)
                if(FreeLibrary(m_Handle) == 0){
				    Logger::Error("Failed to close shared object: {}", m_LibPath.string());
                }
#endif
            }
    };
}
