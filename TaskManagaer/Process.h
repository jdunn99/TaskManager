#pragma once
#include <string>
#include <windows.h>

struct Process {
	DWORD process_id;
	std::string name;
	double cpu_usage;
	double memory_usage;
};

