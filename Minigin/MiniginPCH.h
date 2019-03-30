#pragma once

#include "targetver.h"

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <tchar.h>
#include <memory> // smart pointers
#include <vector>

struct StatusCode
{
	enum class Status : unsigned int { FAIL = 0, SUCCESS = 1, WARNING = 2 };
	Status status;
	std::string message;
	void* attachedObject;
	explicit StatusCode(Status status = Status::SUCCESS, const std::string& message = "", void* attachedObject = nullptr)
		: status{ status }, message{ message }, attachedObject{ attachedObject }{}
};

#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Log.h" // Various logging functions