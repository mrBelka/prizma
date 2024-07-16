#include <Logger.hpp>
#include <iomanip>
#include <iostream>

namespace prizma
{
	std::string CurrentTime()
    {
      time_t rawtime;
      struct tm* timeinfo;
      time(&rawtime);
      timeinfo = localtime(&rawtime);

      char buffer[10];
      strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);

      return std::string(buffer);
    }

	Log::~Log() noexcept
	{
		std::lock_guard<std::mutex> lock(printMutex);

		switch (m_type)
		{
        case LogType::Debug:
			if(logLevel == LogLevel::Debug)
                std::cerr << CurrentTime() << " \x1B[97m[DEBUG]\033[0m " << m_stream.str() << std::endl;
			break;
		case LogType::Info:
			std::cerr << CurrentTime() << " \x1B[97m[INFO]\033[0m " << m_stream.str() << std::endl;
			break;
		case LogType::Bytes:
			std::cerr << std::hex << m_stream.str();
			break;
		case LogType::Endl:
			std::cerr << std::endl;
			break;
		case LogType::Warning:
			std::cerr << CurrentTime() << " \x1B[93m[WARN]\033[0m " << m_stream.str() << std::endl;
			break;
		case LogType::Error:
			std::cerr << CurrentTime() << " \x1B[91m[ERR]\033[0m " << m_stream.str() << std::endl;
			break;
		}
	}
}

void SetLogLevel(prizma::LogLevel level)
{
    prizma::logLevel = level;
}

prizma::Log INFO()
{
	return prizma::Log(prizma::LogType::Info);
}

prizma::Log DEBUG()
{
	return prizma::Log(prizma::LogType::Debug);
}

void BYTES(uint8_t* data, size_t size)
{
	for(int i=0;i<size;i++)
        printf("%02hhX ", data[i]);
	printf("\n");
}

prizma::Log ENDL()
{
	return prizma::Log(prizma::LogType::Endl);
}

prizma::Log WARN()
{
	return prizma::Log(prizma::LogType::Warning);
}

prizma::Log ERR()
{
	return prizma::Log(prizma::LogType::Error);
}