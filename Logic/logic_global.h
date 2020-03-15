#ifndef LOGIC_GLOBAL_H
#define LOGIC_GLOBAL_H

#if defined(LOGIC_LIBRARY)
#  define LOGICSHARED_EXPORT __declspec(dllexport)
#else
#  define LOGICSHARED_EXPORT __declspec(dllimport)
#endif

#endif // LOGIC_GLOBAL_H
