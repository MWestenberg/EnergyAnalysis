#pragma once
#include <iostream>
#include <string>
#include "Constants.h"

class Logging
{
public:
	
	enum Level
	{
		ERROR, WARNING, INFO, DEBUG
	};

	Level GetLevel();		
	void SetLevel(Level level);
	void LogError(const std::string& message);
	void LogWarning(const std::string& message);
	void LogInfo(const std::string& message);
	void LogDebug(const std::string& message);

private:
	Level m_loglevel = LOGLEVEL;

};

