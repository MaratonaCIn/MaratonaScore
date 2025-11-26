#pragma once

// DLL Export/Import macros for all platforms
#ifdef _WIN32
    // Windows (MSVC/MinGW)
    #ifdef MARATONASCORE_EXPORTS
        #define MARATONASCORE_API __declspec(dllexport)
    #elif defined(MARATONASCORE_DLL)
        #define MARATONASCORE_API __declspec(dllimport)
    #else
        #define MARATONASCORE_API
    #endif
#else
    // Linux/macOS (GCC/Clang with -fvisibility=hidden)
    #ifdef MARATONASCORE_EXPORTS
        #define MARATONASCORE_API __attribute__((visibility("default")))
    #else
        #define MARATONASCORE_API
    #endif
#endif
