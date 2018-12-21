//=============================================================================//
//							PRECOMPILED HEADER								   //	
//=============================================================================//

// LLVM global annotation function
#define LLVM_GLOBAL_ANNOTATIONS "llvm.global.annotations"
#define LLVM_LIFETIME_FUNCTION "llvm."
// The name given to functions that control external hardware components and are energy annotated
#define ENERGY_ATTR "Energy"

#define PRINT_FUNCTION "printf"

#define NUM_OF_TOKENS 4

// External hardware functions are prefixed with _ and are removed after annotation
#define ENERGY_FUNCTION_PREFIX "_";
// the name of the energy function as defined by the user
#define ENERGY_FUNCTION_NAME "name"
// temporal power draw of energy in joules
#define ENERGY_TEMPORAL_CONSUMPTION "pd"
// single power draw of energy in Joules
#define ENERGY_CONSUMPTION "ec"
// time unit used in WCET
#define ENERGY_TIME_UNIT "t"
// Function name of the main entry point
#define MODULE_ENTRY_POINT "main"
// Log level: ERROR, WARNING, INFO, DEBUG
#define LOGLEVEL Log::INFO