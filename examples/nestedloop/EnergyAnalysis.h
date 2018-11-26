#define LLVM

#ifdef LLVM
 #define ENERGY_ANNOTATION(x) __attribute__((annotate(x)))
 #define ENERGY(name, rettype, energy) void _ ## name() ENERGY_ANNOTATION(energy) {}; rettype name()
 #define ENERGY_ARG(name, rettype, energy, arg, ...) void _ ## name() ENERGY_ANNOTATION(energy) {}; rettype name(arg,...)
 void LOOP_TRIPCOUNT(unsigned int c);
#else
 #define ENERGY(name, rettype, energy) rettype name()
 #define LOOP(name, tripCount) rettype name()
#endif


