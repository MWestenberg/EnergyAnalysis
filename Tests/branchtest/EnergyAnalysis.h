#define LLVM

#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef LLVM
 #define ENERGY_ANNOTATION(x) __attribute__((annotate(x)))
 #define ENERGY(name, rettype, retval, energy) rettype _ ## name() ENERGY_ANNOTATION(energy) { return retval; }; rettype name()
 #define ENERGY_ARG(name, rettype, retval, energy, arg, ...) rettype _ ## name() ENERGY_ANNOTATION(energy) { return retval;}; rettype name(arg,...)
#else
 #define ENERGY(name, rettype, energy) rettype name()
 #define LOOP(name, tripCount) rettype name()
#endif


