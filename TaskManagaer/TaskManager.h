
#pragma once
#include <vector>
#include "Process.h"
#include <windows.h>
#include <iostream>
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
			proc.process_id = pe32.th32ProcessID;
			proc.name = pe32.szExeFile;
			this->processList.push_back(proc);
		} while (Process32Next(hSnapshot, &pe32));
		CloseHandle(hSnapshot);
	}

	
}

void TaskManager::displayProcessList() {
	// clear console
	system("cls");

	for (auto process : this->processList) {
		std::wcout << "[ " << process.process_id << " ] " << process.name << "\n";
	}
}

