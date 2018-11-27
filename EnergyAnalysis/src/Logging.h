#pragma once
#include <iostream>
#include <string>
#include "Constants.h"

class Logging
{
public:
	
	//The log Levels
	enum Level
	{
		ERROR = 0, WARNING, INFO, DEBUG
	};

	// Return the current log level: ERROR, WARNING, INFO
	Level GetLevel();		

	//Set the log level. Allowed Levels are : ERROR, WARNING, INFO
	void SetLevel(Level level);

	//Logs only errors when level is set to ERROR
	void LogError(const std::string& message);

	//Logs only warnings when level is set to WARNING
	void LogWarning(const std::string& message);

	//Logs only information when level is set to INFO
	void LogInfo(const std::string& message);

	// Log debug messages. Solely for debugging purposes. Extensive logging.
	void LogDebug(const std::string& message);

	//Standard log can be used to print report and other information that needs to be shown to the user.
	// This will always print no matter what level
	void LogConsole(const std::string& message);
private:
	Level m_loglevel = LOGLEVEL;

};

