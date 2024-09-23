// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "detours.h"
#include "Api.hpp"

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

	InstallHook("windows.storage.search.dll", "IsMSSearchEnabled", (LPVOID*)& originalISMSSearchEnabled, HookedISMSSearchEnabled);

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
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

