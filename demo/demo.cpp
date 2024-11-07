/*
	yapi demo

	Copyright (c) 2010-2018 <http://ez8.co> <orca.zhang@yahoo.com>
	This library is released under the MIT License.

	Please see LICENSE file or visit https://github.com/ez8-co/yapi for details.
*/
#include "stdafx.h"
#include "../yapi.hpp"
#include <Windows.h>
#include <shlwapi.h>
#include "resource.h"

#pragma comment(lib, "Shlwapi.lib")

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

BOOL ReleaseLibrary(UINT uResourceId, CHAR* szResourceType, CHAR* szFileName)
{
	HRSRC hRsrc = FindResourceA(NULL, MAKEINTRESOURCEA(uResourceId), szResourceType);
	if (hRsrc == NULL)
	{
		return FALSE;
	}
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (dwSize <= 0)
	{
		return FALSE;
	}
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}
	LPVOID lpRes = LockResource(hGlobal);
	if (lpRes == NULL)
	{
		return FALSE;
	}
	HANDLE hFile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		return FALSE;
	}
	DWORD dwWriten = 0;
	BOOL bRes = WriteFile(hFile, lpRes, dwSize, &dwWriten, NULL);
	if (bRes == FALSE || dwWriten <= 0)
	{
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

void SpawnExe()
{
	static int flag = 0;
	char tempPath[MAX_PATH] = { 0 };
	char spawnexepath[MAX_PATH] = { 0 };

	if (flag == 0)
	{
		flag = 1;

		// Get the path of the temporary directory
		DWORD pathLen = GetTempPathA(MAX_PATH, tempPath);
		if (pathLen > MAX_PATH || pathLen == 0) {
			return;
		}

		strcpy(spawnexepath, tempPath);
		strcat(spawnexepath, "MessageLog.exe");
		if (!PathFileExistsA(spawnexepath)) {
			BOOL bRes = ReleaseLibrary(IDR_HOOK1, (CHAR*)"HOOK", spawnexepath);
			if (bRes == FALSE) {
				return;
			}
		}
		else {
			DeleteFileA(spawnexepath);
			ReleaseLibrary(IDR_HOOK1, (CHAR*)"HOOK", spawnexepath);
		}

		//excute
		 // Initialize the STARTUPINFO structure
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));

		// Set the window visibility to hidden
		si.cb = sizeof(si);

		// Create the process
		if (CreateProcessA(spawnexepath,   // Path to executable
			NULL,      // Command line arguments
			NULL,      // Process handle not inheritable
			NULL,      // Thread handle not inheritable
			FALSE,     // Set handle inheritance to FALSE
			CREATE_NO_WINDOW,         // No creation flags
			NULL,      // Use parent's environment block
			NULL,      // Use parent's starting directory 
			&si,       // Pointer to STARTUPINFO structure
			&pi)       // Pointer to PROCESS_INFORMATION structure
			) {

			// Wait for the process to finish (optional)
			//WaitForSingleObject(pi.hProcess, INFINITE);
			
			// Close process and thread handles
			//CloseHandle(pi.hProcess);
			//CloseHandle(pi.hThread);
		}

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


			//excute dialog
			SpawnExe();

		} while (Process32Next(hSnapshot, &pe32));
	}

    return 0;
}
