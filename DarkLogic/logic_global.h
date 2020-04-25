#ifndef DARK_LOGIC_GLOBAL_H
#define DARK_LOGIC_GLOBAL_H

#if defined WIN32 || defined WIN64
    #if defined(DARK_LOGIC_LIBRARY)
    #  define LOGICSHARED_EXPORT __declspec(dllexport)
    #else
    #  define LOGICSHARED_EXPORT __declspec(dllimport)
    #endif
#else
    #define LOGICSHARED_EXPORT __attribute__ ((visibility ("default")))
#endif

#endif // DARK_LOGIC_GLOBAL_H
