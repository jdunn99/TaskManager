#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
const int MAX_DISPLAY_ROWS = 10; // Maximum number of rows to display in the console


 struct Process {
    DWORD pid;
    std::wstring name;
    double cpuUsage; // CPU usage in percentage
    double memoryUsage; // Memory usage in MB
};

 void SetConsoleCursorPosition(int x, int y) {
     COORD coord;
     coord.X = static_cast<SHORT>(x);
     coord.Y = static_cast<SHORT>(y);
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
 }

 std::vector<Process> GetRunningProcesses() {
     std::vector<Process> processList;
     HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

     if (hSnapshot != INVALID_HANDLE_VALUE) {
         PROCESSENTRY32 processEntry;
         processEntry.dwSize = sizeof(PROCESSENTRY32);

         if (Process32First(hSnapshot, &processEntry)) {
             do {
                 Process process;
                 process.pid = processEntry.th32ProcessID;
                 process.name = processEntry.szExeFile;
                 processList.push_back(process);
             } while (Process32Next(hSnapshot, &processEntry));
         }
         CloseHandle(hSnapshot);
     }
     else {
         std::wcerr << L"Failed to create snapshot. Error code: " << GetLastError() << std::endl;
     }

     return processList;
 }



 void DisplayProcesses(const std::vector<Process>& processList, int selectedIndex) {
     system("cls"); // Clear the console

     std::wcout << L"Scroll using Up and Down arrow keys. Press Enter to view details. Press Esc to exit." << std::endl;
     std::wcout << L"-----------------------------------" << std::endl;

     for (int i = 0; i < processList.size(); ++i) {
         if (i == selectedIndex) {
             std::wcout << L"-> [" << processList[i].pid << L"] " << processList[i].name
                 << L" CPU: " << std::fixed << std::setprecision(2) << processList[i].cpuUsage
                 << L"% Mem: " << processList[i].memoryUsage << L"MB" << std::endl;
         }
         else {
             std::wcout << L"   [" << processList[i].pid << L"] " << processList[i].name
                 << L" CPU: " << std::fixed << std::setprecision(2) << processList[i].cpuUsage
                 << L"% Mem: " << processList[i].memoryUsage << L"MB" << std::endl;
         }
     }
 }

 int main() {
     std::vector<Process> processList = GetRunningProcesses();

     int selectedIndex = 0;
     int startIndex = 0;

     while (true) {
         DisplayProcesses(processList, selectedIndex);

         // Handle keyboard input
         int key = _getch();
         if (key == 0xE0) { // Arrow key
             key = _getch();
             if (key == 72) { // Up arrow
                 if (selectedIndex > 0) {
                     selectedIndex--;
                     if (selectedIndex < startIndex) {
                         startIndex--;
                     }
                 }
             }
             else if (key == 80) { // Down arrow
                 if (selectedIndex < processList.size() - 1) {
                     selectedIndex++;
                     if (selectedIndex >= startIndex + MAX_DISPLAY_ROWS) {
                         startIndex++;
                     }
                 }
             }
         }
         else if (key == 13) { // Enter key
             // Display process details
             if (selectedIndex < processList.size()) {
                 std::cout << "Process ID: " << processList[selectedIndex].pid << std::endl;
                 // Implement further details if needed
                 // Example: Display detailed process information using WinAPI functions
             }
         }
         else if (key == 27) { // Esc key
             break;
         }
     }

     return 0;
 }

