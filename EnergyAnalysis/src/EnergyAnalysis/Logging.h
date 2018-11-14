#pragma once
#include <iostream>

class Logging
{
public:
	
	enum Level
	{
		ERROR, WARNING, INFO
	};


	void SetLevel(Level level);
	void Error(const char* message);
	void Warning(const char* message);
	void Info(const char* message);

private:
	Level m_loglevel = INFO;

};

