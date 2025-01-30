// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "detours.h"
#include "Api.h"

#pragma comment(lib, "./lib/x64/detours.lib")

static BOOL InstallHook(LPCSTR dll, LPCSTR function, LPVOID* originalFunction, LPVOID hookedFunction)
{
	HMODULE module = GetModuleHandleA(dll);
	*originalFunction = (LPVOID)GetProcAddress(module, function);

	if (*originalFunction)
	{
		DetourAttach(originalFunction, hookedFunction);
		return true;
	}
	return false;
}


void mainhook() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    LoadConfigInformation();
    InstallHook("Shell32.dll", "SHGetIDListFromObject", (LPVOID*)&TrueSHGetIDListFromObject, HookSHGetIDListFromObject);
    InstallHook("ntdll.dll", "NtQueryDirectoryFile", (LPVOID*)&Real_NtQueryDirectoryFile, Hooked_NtQueryDirectoryFile); 
    InstallHook("kernelbase.dll", "RegGetValueW", (LPVOID*)&Real_RegGetValueW, Hooked_RegGetValueW);
    //InstallHook("kernelbase.dll", "RegQueryValueExW", (LPVOID*)&Real_RegQueryValueExW, Hooked_RegQueryValueExW);
    //InstallHook("kernelbase.dll", "RegOpenKeyExW", (LPVOID*)&Real_RegOpenKeyExW, Hooked_RegOpenKeyExW);
    
    DetourTransactionCommit();
    
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		mainhook();       
		CreateThread(NULL, 0, CreatePipe, NULL, 0, NULL);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if(trackTimerId !=0 ) KillTimer(NULL, trackTimerId);
        break;
    }
    return TRUE;
}

