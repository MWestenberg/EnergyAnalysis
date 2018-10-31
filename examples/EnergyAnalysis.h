#define LLVM

#ifdef LLVM
#define ENERGY_ANNOTATION(x) __attribute__((annotate(x)))
#define ENERGY(name, rettype, energy) void _ ## name() ENERGY_ANNOTATION(energy) {}; rettype name()
#define ENERGY_ARG(name, rettype, energy, arg, ...) void _ ## name() ENERGY_ANNOTATION(energy) {}; rettype name(arg,...)
#else
#define ENERGY(name, rettype, energy) rettype name()
#endif


#define STRINGIFY_2(value) #value
#define STRINGIFY(value) STRINGIFY_2(value)
#define my_stuff(value) __attribute__((annotate(STRINGIFY(value))))
