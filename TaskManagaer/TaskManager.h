
#pragma once
#include <vector>
#include "Process.h"
#include <windows.h>
#include <iostream>
#include <Psapi.h>
#include <tlhelp32.h>

class TaskManager
{
private:
	std::vector<Process> processList;
public:
	void getRunningProcesses();
	void displayProcessList();
};

/*
* Generates a list of running processes and their details.
*/
void TaskManager::getRunningProcesses() {
	// fetch the snapshot of all running processes
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE) {
		// TODO: Handle any errors
		return;
	}

	// initialize the PROCESSENTRY32 structure for iterating
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about our processes
	if (Process32First(hSnapshot, &pe32)) {
		do {
			Process proc;
			proc.processId = pe32.th32ProcessID;
			proc.name = pe32.szExeFile;
			this->processList.push_back(proc);
		} while (Process32Next(hSnapshot, &pe32));
		CloseHandle(hSnapshot);
	}


	// TODO: Handle system level processes

	for (auto &process : processList) {
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process.processId);
		if (hProcess == NULL) {
			// TODO: Error message
			continue; // process failed to open, skip this one
		}

		PROCESS_MEMORY_COUNTERS_EX memoryInfo;
		GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&memoryInfo, sizeof(memoryInfo));

		process.memoryUsage = (double)(memoryInfo.PrivateUsage / (1024 * 1024));

		FILETIME create, exit, kernel, user;
		if (GetProcessTimes(hProcess, &create, &exit, &kernel, &user)) {
			ULARGE_INTEGER time, sysTime;
			time.LowPart = user.dwLowDateTime;
			time.HighPart = user.dwHighDateTime;
			sysTime.LowPart = kernel.dwLowDateTime;
			sysTime.HighPart = kernel.dwHighDateTime;

			process.cpuUsage = (time.QuadPart + sysTime.QuadPart) / 1000.0;
		}
	}
}

void TaskManager::displayProcessList() {
	// clear console
	//system("cls");

	for (auto process : this->processList) {
		std::wcout << "[ " << process.processId << " ] " << process.name
			<< " " << process.cpuUsage << "ms " << process.memoryUsage << "MB " << std::endl;
	}
}

