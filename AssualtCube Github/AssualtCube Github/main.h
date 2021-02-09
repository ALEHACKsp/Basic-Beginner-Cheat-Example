#include <Windows.h>        //Common windows include
#include <iostream>         //For Printing text
#include <tlhelp32.h>       //For GetModuleBase
#include <memoryapi.h>      //For Read&Write
#include "offsets.h"        //For our offsets
#include <thread>           //Used for making threads

DWORD ProcID;
HANDLE ACHandle;
bool InfiniteHealth = false;

//This gets the module base from the game, you can feed it the PID and the name of the module and it will return the address of the module if it is valid.
DWORD GetModuleBase(DWORD processId, TCHAR* szModuleName)
{
    DWORD moduleBase = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (wcscmp((const wchar_t*)moduleEntry.szModule, (const wchar_t*)szModuleName) == 0) {
                    moduleBase = (DWORD)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBase;
}

//We use templates like these below because typing the entire statment could be annoying, so we shorten them.
//ReadProcessMemory template, for easy use you can call this with: RPM<DWORD>(Handle, Address);
template<typename TYPE>
TYPE RPM(HANDLE proc, DWORD address) {
	TYPE buffer;
	ReadProcessMemory(proc, (LPCVOID)address, &buffer, sizeof(buffer), 0);
	return buffer;
}

//WriteProcessMemory template, for easy use you can call this with: WPM<bool>(Handle, Address, Value)
template<typename TYPE>
void WPM(HANDLE proc, DWORD address, TYPE dataToWrite) {
	TYPE buffer = dataToWrite;
	WriteProcessMemory(proc, (LPVOID)address, &buffer, sizeof(buffer), 0);
}