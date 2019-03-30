#pragma once
#include "Singleton.h"
#include <fstream>
class Logger : public dae::Singleton<Logger>
{
public:
	void Log(const StatusCode& sc);
	template<class T>
	void SafeDelete(T &pObjectToDelete, bool ignoreWarning = false)
	{
		if (pObjectToDelete != nullptr)
		{
			delete(pObjectToDelete);
			pObjectToDelete = nullptr;
		}
		else if (!ignoreWarning)
		{
			Log(StatusCode{ StatusCode::Status::WARNING, "Attempted to delete a nullpointer of type " + std::string(typeid(T).name()) });
		}
	}

	Logger();
	~Logger();

	Logger(const Logger& other) = delete;
	Logger(Logger&& other) = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger& operator=(Logger&& other) = delete;

private:
	std::ofstream m_Output;
	std::vector<StatusCode> m_Logged;
};

