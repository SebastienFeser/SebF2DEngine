#pragma once

#include <iostream>
#include <string>

class Logger
{
public:

	static void Log(const std::string& msg)
	{
		GetInstance().LogImpl(msg);

	}

private:
	void LogImpl(const std::string& msg)
	{
		std::cout << msg << std::endl;
	}
	static Logger& GetInstance()
	{
		static Logger instance;
		return instance;
	}
	Logger() = default;

	//To avoid compiler copy/assignation
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
};


