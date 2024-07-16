#pragma once
#include <sstream>
#include <mutex>

#include <iostream>

namespace prizma
{
    enum class LogLevel
    {
        Debug,
        Work
    };

	enum class LogType
	{
        Debug,
		Info,
		Bytes,
		Endl,
		Warning,
		Error
	};

	static std::string CurrentTime();
	static std::mutex printMutex;
    static LogLevel logLevel;

	class Log
	{
	public:
		explicit Log(LogType type = LogType::Info) : m_type(type) {}
		~Log() noexcept;

		template<typename T>
		Log& operator<<(const T& t)
		{
			m_stream << t;
			return *this;
		}

	private:
		std::stringstream m_stream;
		LogType m_type;
	};
}

void SetLogLevel(prizma::LogLevel level);
prizma::Log INFO();
prizma::Log DEBUG();
void BYTES(uint8_t* data, size_t size);
prizma::Log ENDL();
prizma::Log WARN();
prizma::Log ERR();