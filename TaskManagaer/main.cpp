#include "TaskManager.h"

int main() {

    TaskManager taskManager;
    taskManager.getRunningProcesses(); // might as well put this in the constructor (?)

    taskManager.displayProcessList();

    return 0;
}

