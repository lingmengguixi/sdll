// seeproinfo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
#define ArraySize(ptr)    (sizeof(ptr) / sizeof(ptr[0]))
BOOL FindProcessPid(LPCTSTR ProcessName, DWORD& dwPid);
HMODULE  GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName) {
	MODULEENTRY32 moduleEntry;
	HANDLE handle = NULL;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);//
	if (!handle) {
		CloseHandle(handle);
		return NULL;
	}
	ZeroMemory(&moduleEntry, sizeof(MODULEENTRY32));
	if (!Module32First(handle, &moduleEntry)) {
		CloseHandle(handle);
		return NULL;
	}
	do {
		if (_tcscmp(moduleEntry.szModule, moduleName) == 0) {
			return moduleEntry.hModule;
		}
		else {
			wprintf(L"%s", moduleName);
		}
	} while (Module32Next(handle, &moduleEntry));

	CloseHandle(handle);
	return 0;
}
BOOL FindProcessPid(LPCTSTR ProcessName, DWORD& dwPid)
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
		wcscpy(ss,pe32.szExeFile);
		if (!wcscmp(ProcessName,ss))
		{
			dwPid = pe32.th32ProcessID;
			printf("%d\n", pe32.th32ModuleID);
			bRet = TRUE;
			break;
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bRet;
}
int main1()
{
	TCHAR aa[100000];
	
	LPCTSTR ListApps[]{
	L"erl.exe", L"chrome.exe"
	};
	// StopMyService();
	DWORD dwPid = 0;

	for (int i = 0; i < ArraySize(ListApps); i++)
	{
		if (FindProcessPid(ListApps[i], dwPid))
		{
			printf("[%s] [%d]\n", ListApps[i], dwPid);
			GetProcessModuleHandle(dwPid, aa);
		}
		else
		{
			printf("[%s] [Not Found]\n", ListApps[i]);
		}
	}

	system("pause");
	return 0;
}
