#pragma once
#include <string>
#include <windows.h>

struct Process {
	DWORD processId;
	std::wstring name;
	double cpuUsage;
	double memoryUsage;
};
