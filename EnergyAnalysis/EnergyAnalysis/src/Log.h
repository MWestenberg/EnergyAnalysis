#pragma once
#include <iostream>
#include <string>
#include "Constants.h"
#include <iomanip>
#include <locale>

class Log
{
public:
	
	Log();
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

	template<class T>
	void RecursiveCommas(std::ostream& os, T n);

private:
	Level m_loglevel = LOGLEVEL;

};



template<class T>
inline void Log::RecursiveCommas(std::ostream & os, T n)
{
	T rest = n % 1000; //"last 3 digits"
	n /= 1000;         //"begining"

	if (n > 0) {
		RecursiveCommas(os, n); //printing "begining"

		//and last chunk
		os << '.' << std::setfill('0') << std::setw(3) << rest;
	}
	else
		os << rest; //first chunk of the number
}
