#include "Logging.h"



void Logging::SetLevel(Level level)
{
	m_loglevel = level;
}

void Logging::Error(const char* message)
{
	if (m_loglevel >= ERROR)
		std::cout << "[ERROR]: " << message << std::endl;
}

void Logging::Warning(const char* message)
{
	if (m_loglevel >= WARNING)
		std::cout << "[WARNING]: " << message << std::endl;
}

void Logging::Info(const char* message)
{
	if (m_loglevel >= INFO)
		std::cout << "[INFO]: " << message << std::endl;
}