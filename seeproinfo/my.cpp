
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
HMODULE  GetProcessModuleHandle(DWORD pid) {
	MODULEENTRY32 moduleEntry;
	HANDLE handle = NULL;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);//
	if (handle== INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
		return NULL;
	}
	//ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
	if (!Module32First(handle, &moduleEntry)) {
		CloseHandle(handle);
		return NULL;
	}
	do {
		wprintf(L"   ->%d\n",moduleEntry.th32ModuleID);
	} while (Module32Next(handle, &moduleEntry));

	CloseHandle(handle);
	return 0;
}
BOOL FindProcess()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	BOOL    bRet = FALSE;
	do
	{
		WCHAR ss[1000];
		wcscpy(ss, pe32.szExeFile);
		printf("进程PID:%d 文件名:", pe32.th32ProcessID);
		wprintf(L"%s\n", pe32.szExeFile);
		GetProcessModuleHandle(pe32.th32ProcessID);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bRet;
}

int main() {
	FindProcess();
	system("pause");
}