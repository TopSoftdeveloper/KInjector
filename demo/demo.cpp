/*
	yapi demo

	Copyright (c) 2010-2018 <http://ez8.co> <orca.zhang@yahoo.com>
	This library is released under the MIT License.

	Please see LICENSE file or visit https://github.com/ez8-co/yapi for details.
*/
#include "stdafx.h"
#include "../yapi.hpp"

using namespace yapi;

std::string getCurrentDirectory() {
	// Buffer to store the current directory
	char buffer[MAX_PATH];

	// Get the current directory
	DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);

	if (length > 0 && length <= MAX_PATH) {
		return std::string(buffer); // Return as std::string
	}
	else {
		// Handle error (you can throw an exception or return an empty string)
		return "";
	}
}

int main()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (_tcsicmp(pe32.szExeFile, _T("explorer.exe")))
				continue;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			//YAPICall MessageBoxA(hProcess, _T("user32.dll"), "MessageBoxA");
			//MessageBoxA(NULL, "MessageBoxA : Hello World!", "From ez8.co", MB_OK);
			
			//YAPI(hProcess, _T("user32.dll"), MessageBoxW)(NULL, L"MessageBoxW: Hello World!", L"From ez8.co", MB_OK);
			
			//YAPICall GetCurrentProcessId(hProcess, _T("kernel32.dll"), "GetCurrentProcessId");
			//DWORD pid = GetCurrentProcessId();
			//_tprintf(_T("[%d]%s => %d\n"), pe32.th32ProcessID, pe32.szExeFile, pid);

			YAPICall LoadLibraryA(hProcess, _T("kernel32.dll"), "LoadLibraryA");
			//DWORD64 x86Dll = LoadLibraryA(".\\x86.dll");

			std::string path = getCurrentDirectory() + "\\hook.dll";
			printf((char*)"X64: %s\n", path.c_str());

			DWORD64 x64Dll = LoadLibraryA.Dw64()(path.c_str());
			//_tprintf(_T("X86: %I64x\n"), x86Dll);
			_tprintf(_T("X64: %I64x\n"), x64Dll);

		} while (Process32Next(hSnapshot, &pe32));
	}

    return 0;
}
