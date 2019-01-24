#include "Log.h"


Log::Log()
{
	std::cout.imbue(std::locale(""));
}


Log::Level Log::GetLevel()
{
	return m_loglevel;
}

void Log::SetLevel(Level level)
{
	m_loglevel = level;
}

void Log::LogError(const std::string& message)
{
	if (m_loglevel >= ERROR)
		std::cout << "ERROR: " << message << "\n";
}

void Log::LogWarning(const std::string& message)
{
	if (m_loglevel >= WARNING)
		std::cout << "WARNING: " << message << "\n";
}

void Log::LogInfo(const std::string& message)
{
	std::cout << std::fixed;
	if (m_loglevel >= INFO)
		std::cout << std::setprecision(15) << message;
}

void Log::LogDebug(const std::string& message)
{

	if (m_loglevel >= DEBUG)
		std::cout << message;
}

void Log::LogConsole(const std::string & message)
{
	std::cout << std::fixed;
	std::cout << std::setprecision(15) << message;
}

