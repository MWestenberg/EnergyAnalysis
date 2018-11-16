#include "Logging.h"






Logging::Level Logging::GetLevel()
{
	return m_loglevel;
}

void Logging::SetLevel(Level level)
{
	m_loglevel = level;
}

void Logging::LogError(const std::string& message)
{
	if (m_loglevel >= ERROR)
		std::cout << "[ERROR]: " << message;
}

void Logging::LogWarning(const std::string& message)
{
	if (m_loglevel >= WARNING)
		std::cout << "[WARNING]: " << message;
}

void Logging::LogInfo(const std::string& message)
{

	if (m_loglevel >= INFO)
		std::cout << "[INFO]: " << message;
}