#ifndef DARK_LOGIC_GLOBAL_H
#define DARK_LOGIC_GLOBAL_H

#if defined(DARK_LOGIC_LIBRARY)
#  define LOGICSHARED_EXPORT __declspec(dllexport)
#else
#  define LOGICSHARED_EXPORT __declspec(dllimport)
#endif

#endif // DARK_LOGIC_GLOBAL_H
