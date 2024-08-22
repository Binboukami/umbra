#pragma once

/** Build as shared library by default **/
#ifdef __WIN32__
    #ifdef BUILD_SHARED_LIB
        #define UMBRA_API __declspec(dllexport)
    #else
        #define UMBRA_API __declspec(dllimport)
    #endif
#else
    #define UMBRA_API __attribute__((visibility("default"))) // Make library symbols visible
#endif