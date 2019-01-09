//=============================================================================//
//							PRECOMPILED HEADER								   //	
//=============================================================================//

// LLVM global annotation function
#define LLVM_GLOBAL_ANNOTATIONS "llvm.global.annotations"
#define LLVM_LIFETIME_FUNCTION "llvm.lifetime"
// The name given to functions that control external hardware components and are energy annotated
#define ENERGY_ATTR "Energy"

#ifdef _WIN32
#define MICROSECONDS "\xE6s"
#else
#define MICROSECONDS "\xC2\xB5s"
#endif


//Instruction costs are expressed in number of cycles. How long 1 cycle requires depends on the 
//Speed of the controller. E.g. a 16 Mhz controller can do 16 million cycles per second.
//This means 1 cycle = to 0.0625 microseconds. This is calculated in the WCETANalysisVisitor.
//The following setting is the default Speed in Mhz.
//If you would change this to 16.000.000 you also need to change the division
#define DEFAULT_SPEED 16;
//Because the speed is set in Mhz we need to know the division that is required to convert
// from microseconds into seconds. 
#define MICROSECONDS_TO_SECONDS(x) (x / 1000000)

//The user enters time for external components in miliseconds.
//This Macro converts that to microseconds.
// if you use seconds change it to 1.000.000
#define USERDELAY_TO_MICROSECONDS(x) (x * 1000000)

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
// function name to retrieve loop trip counts from undefined loops.
#define LOOP_TRIPCOUNT "LOOP_TRIPCOUNT"
// Log level: ERROR, WARNING, INFO, DEBUG
#define LOGLEVEL Log::ERROR